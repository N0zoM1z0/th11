#!/usr/bin/env python3
"""Select private TH11 tools; reuse shared analysis installations explicitly."""
from __future__ import annotations
import argparse
from pathlib import Path
import subprocess
import tomllib
ROOT = Path(__file__).resolve().parents[1]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--shared-tools', type=Path, help='directory containing ghidra, jdk and capstone-5.0.6')
    args = parser.parse_args()
    destination = ROOT / '.tools'
    destination.mkdir(exist_ok=True)
    if args.shared_tools:
        for name, leaf in [('ghidra', 'ghidra'), ('jdk', 'jdk'), ('capstone', 'capstone-5.0.6')]:
            source = (args.shared_tools / leaf).resolve(strict=True)
            output = destination / name
            if output.is_symlink() and output.resolve() == source:
                continue
            if output.exists() or output.is_symlink():
                raise ValueError(f'refusing to replace existing tool selection: {output}')
            output.symlink_to(source, target_is_directory=True)
    with (ROOT / 'config/tools.lock.toml').open('rb') as stream:
        tool = tomllib.load(stream)['msvc90']
    checkout = ROOT / tool['selection']
    if not checkout.exists():
        subprocess.run(['nice', '-n', '10', 'git', 'clone', '--no-checkout', tool['repository'], str(checkout)], check=True)
        subprocess.run(['git', '-C', str(checkout), 'checkout', '--detach', tool['commit']], check=True)
    commit = subprocess.check_output(['git', '-C', str(checkout), 'rev-parse', 'HEAD'], text=True).strip()
    if commit != tool['commit']:
        raise ValueError('existing compiler checkout differs from locked commit')
    subprocess.run([str(ROOT / 'scripts/repo-python'), str(ROOT / 'scripts/bootstrap-ntlm-auth.py')], cwd=ROOT, check=True)
    subprocess.run([str(ROOT / 'scripts/repo-python'), str(ROOT / 'scripts/verify-analysis-tools.py')], cwd=ROOT, check=True)
    subprocess.run([str(ROOT / 'scripts/repo-python'), str(ROOT / 'scripts/verify-toolchain.py'), '--check'], cwd=ROOT, check=True)
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
