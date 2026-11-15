#!/usr/bin/env python3
"""Cold-build and strictly replay every selected canonical TH11 unit."""
from __future__ import annotations
import argparse
from pathlib import Path
import subprocess
import sys
import tomllib
ROOT = Path(__file__).resolve().parents[1]


def select_units(units: dict, names: list[str], sources: list[str]) -> list[str]:
    """Units named with --unit, else exact units matching --source (all when unscoped).

    Frontier (nonexact) units are built only when named explicitly.
    """
    return [name for name, unit in units.items()
            if name in names
            or (not unit.get('frontier') and ((not names and not sources) or unit['source'] in sources))]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--unit', action='append', default=[])
    parser.add_argument('--source', action='append', default=[])
    args = parser.parse_args()
    with (ROOT / 'config/match-units.toml').open('rb') as stream:
        units = tomllib.load(stream).get('units', {})
    if set(args.unit) - set(units) or set(args.source) - {row['source'] for row in units.values()}:
        parser.error('unknown unit/source selector')
    selected = select_units(units, args.unit, args.source)
    command = [sys.executable, str(ROOT / 'scripts/build-match-unit.py')]
    for name in selected:
        command.extend(['--unit', name])
    if not selected:
        print('no canonical exact units configured')
        return 0
    subprocess.run(command, cwd=ROOT, check=True)
    for name in selected:
        kind = units[name]['artifact_kind']
        comparator = 'compare-linked-function.py' if kind == 'linked-pe' else 'compare-coff-function.py'
        command = [sys.executable, str(ROOT / 'scripts' / comparator), '--unit', name]
        if kind == 'coff':
            command.append('--json')
        subprocess.run(command, cwd=ROOT, check=True)
    print(f'cold replay passed: {len(selected)} units')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
