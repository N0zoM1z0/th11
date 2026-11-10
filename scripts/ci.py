#!/usr/bin/env python3
"""Fast, target-independent checks of the complete public checkout."""
from pathlib import Path
import ast
import json
import subprocess
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[1]


def main():
    if sys.argv[1:] in (["--help"], ["-h"]):
        print("usage: ci.py [--full]\nDefault: checkout syntax, tracking, build declarations and private-artifact checks.\n--full also runs both public Oracle suites; exact replay remains scope-specific.")
        return 0
    if len(sys.argv) > 1:
        if sys.argv[1:] != ["--full"]:
            print("Unknown CI option; use no arguments or --full.", file=sys.stderr)
            return 2
        return subprocess.call([sys.executable, str(ROOT / "scripts/ci-full.py")], cwd=ROOT)
    # Inspect every tracked file, including force-added ignored files. This also
    # covers errors introduced before the last commit in shallow PR checkouts.
    ignored = subprocess.check_output(
        ["git", "ls-files", "--cached", "--ignored", "--exclude-standard", "-z"], cwd=ROOT
    ).decode().split("\0")
    if any(ignored):
        raise ValueError("tracked private/generated files: " + ", ".join(n for n in ignored if n))
    names = set(subprocess.check_output(
        ["git", "ls-files", "--cached", "--others", "--exclude-standard", "-z"], cwd=ROOT
    ).decode().split("\0")) - {""}
    for name in sorted(names):
        path = ROOT / name
        if not path.is_file() or path.is_symlink():
            continue
        if path.suffix.lower() in {".zip", ".7z", ".rar", ".tar", ".gz", ".xz", ".bz2"}:
            raise ValueError(f"archive must remain private: {name}")
        with path.open("rb") as stream:
            prefix = stream.read(128)
        if prefix.startswith((b"MZ", b"\x7fELF", b"MSCF", b"PK\x03\x04", b"7z\xbc\xaf\x27\x1c")):
            raise ValueError(f"binary payload must remain private: {name}")
        if path.suffix == ".py":
            ast.parse(path.read_text(), filename=name)
        elif path.suffix == ".json":
            json.loads(path.read_text())
        elif path.suffix == ".toml":
            tomllib.loads(path.read_text())
        elif path.suffix == ".sh" or prefix.splitlines()[0:1] == [b"#!/usr/bin/env bash"]:
            subprocess.run(["bash", "-n", str(path)], check=True)
    for command in (
        ["validate-tracking.py"], ["build-match-unit.py", "--check"],
        ["build.py", "--check"], ["verify-toolchain.py", "--check-declarations"],
    ):
        subprocess.run([sys.executable, str(ROOT / "scripts" / command[0]), *command[1:]], cwd=ROOT, check=True)
    subprocess.run(["git", "diff", "HEAD", "--check"], cwd=ROOT, check=True)
    print("TH11 public checkout checks passed; run affected exact/tool tests separately.")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, SyntaxError, subprocess.CalledProcessError) as exc:
        print(f"error: {exc}", file=sys.stderr)
        raise SystemExit(1)
