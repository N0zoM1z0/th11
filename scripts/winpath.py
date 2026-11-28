#!/usr/bin/env python3
"""Convert absolute Unix paths to Wine DOS paths without starting Wine.

`winepath -w` maps a path through the prefix's `dosdevices` drive links: the
drive whose target is the longest prefix of the path wins, and the rest of the
path is appended with backslashes. Each `winepath` call is a full Wine launch
(about one second under the Factory lock), and the build ran several per
compile, so the build scripts use this equivalent pure conversion instead.
`scripts/test-winpath.py` checks it against real `winepath` output.

usage: winpath.py PREFIX PATH...   (one converted path per line)
"""

from __future__ import annotations

import os
from pathlib import Path
import sys


def drive_roots(prefix: Path) -> list[tuple[str, str]]:
    devices = Path(prefix) / "dosdevices"
    roots = []
    for entry in sorted(os.listdir(devices)):
        if len(entry) != 2 or entry[1] != ":" or not entry[0].isalpha():
            continue
        link = devices / entry
        if not link.is_symlink():
            continue
        target = os.path.realpath(devices / os.readlink(link))
        roots.append((entry[0].upper(), target))
    if not roots:
        raise ValueError(f"no drive mappings in {devices}")
    return roots


def windows_path(path: str | Path, prefix: str | Path) -> str:
    absolute = os.path.realpath(os.fspath(path))
    best = None
    for letter, root in drive_roots(Path(prefix)):
        if absolute == root or root == "/" or absolute.startswith(root.rstrip("/") + "/"):
            if best is None or len(root) > len(best[1]):
                best = (letter, root)
    if best is None:
        raise ValueError(f"path is outside every Wine drive: {absolute}")
    letter, root = best
    rest = absolute[len(root):] if root != "/" else absolute
    return letter + ":" + ("\\" + rest.strip("/").replace("/", "\\") if rest.strip("/") else "\\")


def main(argv: list[str]) -> int:
    if len(argv) < 2:
        print(__doc__.strip().splitlines()[-1], file=sys.stderr)
        return 2
    prefix, paths = argv[0], argv[1:]
    for path in paths:
        print(windows_path(path, prefix))
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
