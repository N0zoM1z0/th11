#!/usr/bin/env python3
"""Provision the pinned Ubuntu amd64 NTLM helper privately, without daemons."""
from __future__ import annotations

import argparse
import hashlib
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import tomllib
from urllib.parse import urlparse
from urllib.request import urlopen

ROOT = Path(__file__).resolve().parents[1]


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b''):
            digest.update(block)
    return digest.hexdigest()


def tree_sha256(root: Path) -> str:
    digest = hashlib.sha256()
    for path in sorted(root.rglob('*')):
        name = path.relative_to(root).as_posix().encode()
        if path.is_symlink():
            payload = b'L\0' + os.readlink(path).encode()
        elif path.is_file():
            payload = b'F\0' + sha256(path).encode()
        else:
            continue
        digest.update(name + b'\0' + payload + b'\0')
    return digest.hexdigest()


def verify(root: Path, lock: dict) -> None:
    binary = root / 'usr/bin/ntlm_auth'
    if not binary.is_file() or sha256(binary) != lock['binary_sha256']:
        raise ValueError('private ntlm_auth binary identity mismatch')
    if tree_sha256(root) != lock['tree_sha256']:
        raise ValueError('private NTLM runtime tree identity mismatch')
    library = root / 'usr/lib/x86_64-linux-gnu'
    environment = os.environ.copy()
    environment['LD_LIBRARY_PATH'] = f'{library}:{library}/samba'
    version = subprocess.run([str(binary), '--version'], env=environment, check=True,
                             capture_output=True, text=True).stdout.strip()
    if version != lock['version_banner']:
        raise ValueError('private ntlm_auth version mismatch: ' + version)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true', help='verify private files without downloads')
    args = parser.parse_args()
    with (ROOT / 'config/tools.lock.toml').open('rb') as stream:
        lock = tomllib.load(stream)['ntlm_auth']
    destination = ROOT / str(lock['selection'])
    destination.resolve().relative_to((ROOT / '.tools').resolve())
    if destination.is_symlink():
        raise ValueError('refusing to replace a symlinked NTLM runtime tree')
    if args.check:
        verify(destination, lock)
        print('private ntlm_auth and runtime tree OK')
        return 0
    if destination.is_dir():
        try:
            verify(destination, lock)
        except (OSError, ValueError, subprocess.CalledProcessError):
            pass
        else:
            print('private ntlm_auth already matches the lock')
            return 0
    if shutil.which('dpkg-deb') is None:
        raise ValueError('dpkg-deb is required for private package extraction')
    cache = ROOT / '.tools/winbind-debs'
    cache.mkdir(parents=True, exist_ok=True)
    cached = {sha256(path): path for path in cache.glob('*.deb')}
    packages = []
    for package in lock['packages']:
        expected = package['sha256']
        path = cached.get(expected)
        if path is None:
            path = cache / Path(urlparse(package['url']).path).name
            partial = path.with_suffix('.deb.part')
            with urlopen(package['url'], timeout=60) as response, partial.open('wb') as output:
                shutil.copyfileobj(response, output, 1024 * 1024)
            if sha256(partial) != expected:
                raise ValueError('package identity mismatch: ' + package['name'])
            partial.replace(path)
        packages.append(path)
    with tempfile.TemporaryDirectory(prefix='ntlm-stage-', dir=ROOT / '.tools') as temporary:
        staging = Path(temporary) / 'root'
        staging.mkdir()
        for package in packages:
            subprocess.run(['dpkg-deb', '-x', str(package), str(staging)], check=True)
        verify(staging, lock)
        backup = None
        if destination.exists():
            backup = Path(temporary) / 'previous'
            destination.replace(backup)
        try:
            staging.replace(destination)
        except OSError:
            if backup is not None:
                backup.replace(destination)
            raise
    print('private ntlm_auth provisioned from pinned packages; no daemon installed or started')
    return 0


if __name__ == '__main__':
    try:
        raise SystemExit(main())
    except (OSError, KeyError, ValueError, subprocess.CalledProcessError) as error:
        print('error: NTLM helper bootstrap failed: ' + str(error), file=sys.stderr)
        raise SystemExit(1)
