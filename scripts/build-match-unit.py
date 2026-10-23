#!/usr/bin/env python3
"""Cold-build declared VC9 normal-COFF units; linked/LTCG contexts remain open."""
from __future__ import annotations
import argparse
import json
from pathlib import Path
import subprocess
import sys
import tomllib
from artifact_receipt import ROOT, contract, receipt, receipt_path
from target_identity import resolve_target, verify_target


def units() -> dict:
    with (ROOT / 'config/match-units.toml').open('rb') as stream:
        document = tomllib.load(stream)
    with (ROOT / 'config/target.toml').open('rb') as stream:
        target = tomllib.load(stream)['target']
    if document.get('schema_version') != 1 or document.get('target_sha256') != target['sha256']:
        raise ValueError('unit manifest is not bound to the current target')
    result = document.get('units', {})
    for name, unit in result.items():
        contract(unit)
        if not unit.get('functions') or not unit.get('symbol') or int(unit['size']) <= 0:
            raise ValueError(f'incomplete unit: {name}')
        relocations = unit.get('relocations', [])
        occupied = set()
        for row in relocations:
            offset = int(row['offset'])
            fields = set(range(offset, offset + 4))
            if offset < 0 or offset + 4 > int(unit['size']) or fields & occupied:
                raise ValueError(f'overlapping/out-of-range relocations: {name}')
            occupied |= fields
    return result


def build(unit: dict) -> None:
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
            print(f'match graph OK: {len(configured)} declared VC9 normal-COFF units; LTCG acceptance not configured')
            return 0
        _, problems = verify_target(resolve_target())
        if problems:
            raise ValueError('; '.join(problems))
        subprocess.run([sys.executable, str(ROOT / 'scripts/verify-toolchain.py'), '--check'], cwd=ROOT, check=True)
        built = set()
        for name in selected:
            unit = configured[name]
            key = json.dumps(contract(unit), sort_keys=True)
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
