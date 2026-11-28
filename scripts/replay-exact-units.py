#!/usr/bin/env python3
"""Cold-build and strictly replay every selected canonical TH11 unit.

Selection: --unit NAME (also frontier units), --source FILE (exact units whose
primary source is FILE), --file FILE (exact units whose compilation context
includes FILE, directly or through quoted includes) or nothing for every exact
unit. Each compilation context is cold-built once and every selected unit in
it is then compared strictly. Target and toolchain identity are verified once
per run. Without --keep-going the run stops at the first failure.
"""
from __future__ import annotations
import argparse
import importlib.util
import json
from pathlib import Path
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'scripts'))


def load_builder():
    spec = importlib.util.spec_from_file_location('build_match_unit', ROOT / 'scripts/build-match-unit.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def select_units(units: dict, contexts: dict, names: list[str], sources: list[str], files: list[str]) -> list[str]:
    """Units named with --unit, else exact units matching --source/--file (all when unscoped).

    Frontier (nonexact) units are built only when named explicitly.
    """
    unscoped = not names and not sources and not files
    return [name for name, unit in units.items()
            if name in names
            or (not unit.get('frontier') and (unscoped or unit['source'] in sources
                                              or any(f in contexts[name]['inputs'] for f in files)))]


def compare(name: str, unit: dict) -> tuple[bool, str]:
    linked = unit['artifact_kind'] == 'linked-pe'
    comparator = 'compare-linked-function.py' if linked else 'compare-coff-function.py'
    command = [sys.executable, str(ROOT / 'scripts' / comparator), '--unit', name, *([] if linked else ['--json'])]
    completed = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    detail = (completed.stdout + completed.stderr).strip().splitlines()
    return completed.returncode == 0, detail[-1] if detail else ''


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--unit', action='append', default=[])
    parser.add_argument('--source', action='append', default=[])
    parser.add_argument('--file', action='append', default=[])
    parser.add_argument('--keep-going', action='store_true')
    args = parser.parse_args()
    builder = load_builder()
    units = builder.units()
    if set(args.unit) - set(units) or set(args.source) - {row['source'] for row in units.values()}:
        parser.error('unknown unit/source selector')
    missing = [f for f in args.file if not (ROOT / f).is_file()]
    if missing:
        parser.error(f'unknown file selector: {missing}')
    contexts = {name: builder.unit_context(unit) for name, unit in units.items()}
    selected = select_units(units, contexts, args.unit, args.source, args.file)
    if not selected:
        print('no canonical exact units selected')
        return 0
    groups: dict[str, list[str]] = {}
    for name in selected:
        groups.setdefault(json.dumps(contexts[name], sort_keys=True), []).append(name)
    print(f'selected {len(selected)} units in {len(groups)} contexts', flush=True)

    _, problems = builder.verify_target(builder.resolve_target())
    if problems:
        print('error: ' + '; '.join(problems), file=sys.stderr)
        return 1
    subprocess.run([sys.executable, str(ROOT / 'scripts/verify-toolchain.py'), '--check'], cwd=ROOT, check=True)

    failures: list[tuple[str, str]] = []
    passed = 0
    start = time.monotonic()
    for index, names in enumerate(groups.values(), 1):
        unit = units[names[0]]
        label = contexts[names[0]].get('output_dir', contexts[names[0]].get('object'))
        began = time.monotonic()
        try:
            builder.build(unit)
        except (OSError, KeyError, TypeError, ValueError, subprocess.CalledProcessError) as exc:
            for name in names:
                failures.append((name, f'build failed: {exc}'))
            print(f'[{index}/{len(groups)}] BUILD FAILED {label}: {exc}', flush=True)
            if not args.keep_going:
                break
            continue
        results = []
        for name in names:
            ok, detail = compare(name, units[name])
            results.append(ok)
            if ok:
                passed += 1
            else:
                failures.append((name, detail))
                print(f'  FAIL {name}: {detail}', flush=True)
        print(f'[{index}/{len(groups)}] {label}: {sum(results)}/{len(names)} passed, '
              f'{time.monotonic() - began:.0f} s', flush=True)
        if failures and not args.keep_going:
            break
    elapsed = time.monotonic() - start
    if failures:
        print(f'cold replay FAILED: {passed} passed, {len(failures)} failed in {elapsed:.0f} s')
        for name, detail in failures:
            print(f'  {name}: {detail}')
        return 1
    print(f'cold replay passed: {passed} units in {elapsed:.0f} s')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
