#!/usr/bin/env python3
"""Cold-build selected VC9 normal-COFF or public linked-PE contexts."""
from __future__ import annotations
import argparse
import json
from pathlib import Path
import subprocess
import sys
import tomllib
from artifact_receipt import ROOT, contract, receipt, receipt_path
import linked_receipt
from ltcg_link import cold_link, tool_environment
from target_identity import resolve_target, verify_target


def units() -> dict:
    with (ROOT / 'config/match-units.toml').open('rb') as stream:
        document = tomllib.load(stream)
    with (ROOT / 'config/target.toml').open('rb') as stream:
        target = tomllib.load(stream)['target']
    if document.get('schema_version') != 1 or document.get('target_sha256') != target['sha256']:
        raise ValueError('unit manifest is not bound to the current target')
    result = document.get('units', {})
    outputs = {}
    for name, unit in result.items():
        context = unit_context(unit)
        output = context.get('object', context.get('output_dir'))
        key = json.dumps(context, sort_keys=True)
        if output in outputs and outputs[output] != key:
            raise ValueError('different compilation contexts share an output')
        outputs[output] = key
        if not unit.get('functions') or not unit.get('symbol') or int(unit['size']) <= 0:
            raise ValueError(f'incomplete unit: {name}')
        if unit['artifact_kind'] == 'linked-pe':
            from importlib.util import spec_from_file_location, module_from_spec
            spec = spec_from_file_location('linked_oracle_graph', ROOT / 'scripts/compare-linked-function.py')
            oracle = module_from_spec(spec)
            spec.loader.exec_module(oracle)
            oracle.fields(unit)
            continue
        relocations = unit.get('relocations', [])
        occupied = set()
        for row in relocations:
            offset = int(row['offset'])
            fields = set(range(offset, offset + 4))
            if offset < 0 or offset + 4 > int(unit['size']) or fields & occupied:
                raise ValueError(f'overlapping/out-of-range relocations: {name}')
            occupied |= fields
    return result


def unit_context(unit: dict) -> dict:
    return linked_receipt.contract(unit) if unit.get('artifact_kind') == 'linked-pe' else contract(unit)


def build(unit: dict) -> None:
    if unit.get('artifact_kind') == 'linked-pe':
        context = linked_receipt.contract(unit)
        directory = ROOT / context['output_dir']
        directory.mkdir(parents=True, exist_ok=True)
        path = directory / 'linked.receipt.json'
        path.unlink(missing_ok=True)
        before = linked_receipt.provenance(unit)
        linker, environment = tool_environment()
        cold_link(ROOT / context['source'], directory, context['entry'], linker, environment,
                  profile=context['profile'],
                  support_sources=[ROOT / p for p in context['support_sources']],
                  normal_support_sources=[ROOT / p for p in context['normal_support_sources']],
                  support_profiles=context['support_profiles'])
        observed = linked_receipt.receipt(unit)
        if {k: v for k, v in observed.items() if k != 'artifacts_sha256'} != before:
            raise ValueError('linked inputs or tools changed during cold compilation')
        path.write_text(json.dumps(observed, indent=2) + '\n')
        return
    context = contract(unit)
    output = ROOT / context['object']
    output.parent.mkdir(parents=True, exist_ok=True)
    output.unlink(missing_ok=True)
    receipt_path(output).unlink(missing_ok=True)
    subprocess.run([str(ROOT / 'scripts/compile-probe.sh'), context['source'], context['object'], *context['profile']], cwd=ROOT, check=True)
    # No artifact can acquire a receipt after an unsuccessful or missing compile.
    observed = receipt(unit)
    receipt_path(output).write_text(json.dumps(observed, indent=2) + '\n')


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true')
    parser.add_argument('--unit', action='append', default=[])
    args = parser.parse_args()
    try:
        configured = units()
        selected = args.unit or list(configured)
        if set(selected) - set(configured):
            raise ValueError('unknown unit selector')
        if args.check:
            counts = {kind: sum(u['artifact_kind'] == kind for u in configured.values()) for kind in ('coff', 'linked-pe')}
            print(f'match graph OK: {counts["coff"]} VC9 normal-COFF, {counts["linked-pe"]} public linked-PE units')
            return 0
        _, problems = verify_target(resolve_target())
        if problems:
            raise ValueError('; '.join(problems))
        subprocess.run([sys.executable, str(ROOT / 'scripts/verify-toolchain.py'), '--check'], cwd=ROOT, check=True)
        built = set()
        for name in selected:
            unit = configured[name]
            key = json.dumps(unit_context(unit), sort_keys=True)
            if key not in built:
                build(unit)
                built.add(key)
            print(f'built: {name}')
    except (OSError, KeyError, TypeError, ValueError, subprocess.CalledProcessError) as exc:
        print(f'error: {exc}', file=sys.stderr)
        return 1
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
