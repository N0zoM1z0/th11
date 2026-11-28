#!/usr/bin/env python3
"""Check scripts/winpath.py against real `winepath -w` for the build's path kinds."""

from __future__ import annotations

import os
from pathlib import Path
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))
from winpath import windows_path  # noqa: E402


def real_winepath(path: Path, prefix: Path) -> str:
    environment = dict(os.environ, WINEARCH="win32", WINEPREFIX=str(prefix), WINEDEBUG="-all",
                       PATH=f"{ROOT / '.tools/factory-bin'}:{os.environ['PATH']}")
    output = subprocess.run(["winepath", "-w", str(path)], env=environment, capture_output=True,
                            text=True, check=True).stdout
    return output.strip().replace("\r", "")


def main() -> int:
    prefix = Path(os.environ.get("TH11_WINEPREFIX", ROOT / ".tools/wineprefix-msvc900")).resolve()
    msvc = ROOT / ".tools/msvc900"
    cases = [
        ROOT / "src/Laser.cpp",                                   # existing source
        ROOT / "build/match-linked/NoSuchContext/source.ltcg.obj",  # output that does not exist yet
        msvc / "VC/bin",                                          # compiler directory
        msvc / "Microsoft SDKs/Windows/v6.0A/Include",            # space in a component
        prefix / "drive_c/windows",                               # inside the C: drive
        Path("/"),                                                # drive root
    ]
    failures = 0
    for case in cases:
        expected, observed = real_winepath(case, prefix), windows_path(case, prefix)
        status = "ok" if expected == observed else "MISMATCH"
        failures += status != "ok"
        print(f"{status:8} {observed}  (winepath: {expected})")
    with tempfile.TemporaryDirectory() as empty:
        try:
            windows_path(ROOT / "src/Laser.cpp", empty)
        except (OSError, ValueError):
            print("ok       prefix without dosdevices is rejected")
        else:
            print("MISMATCH prefix without dosdevices was accepted")
            failures += 1
    print("winpath conversion OK" if not failures else f"winpath conversion FAILED ({failures})")
    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
