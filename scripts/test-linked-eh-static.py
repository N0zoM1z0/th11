#!/usr/bin/env python3
"""Narrow negative tests for VC9 compiler-generated EH static MAP binding.

Never writes to the original executable or linker products. The strict replay
continues to compare the entire function and every decoded relocation.
"""
import importlib.util
from pathlib import Path
import sys
import tempfile
import tomllib

from linked_image import PEImage

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    "compare_linked_function", ROOT / "scripts/compare-linked-function.py")
compare = importlib.util.module_from_spec(spec)
spec.loader.exec_module(compare)
unit = tomllib.loads((ROOT / "config/match-units.toml").read_text())[
    "units"]["secondary-spell-stage-factory-frontier"]
directory = ROOT / unit["output_dir"]
image = PEImage(directory / "source.exe")
target = PEImage(ROOT / "resources/th11.exe")

def fails(callback, label):
    try:
        callback()
    except ValueError:
        print("rejected:", label)
        return
    raise AssertionError("accepted a false EH binding: " + label)

manifest_fields = compare.fields(unit)
eh = [field for field in manifest_fields if field["eh_static"]]
assert len(eh) == 1 and eh[0]["offset"] == 3

result = compare.replay(unit, image, directory / "source.map",
                        directory / "source.pdb", target)
assert result["result"] == "exact" and result["difference_count"] == 0

mutated = dict(unit)
mutated["linkages"] = [dict(field) for field in unit["linkages"]]
mutated["linkages"][0]["symbol"] += "_not_this_owner"
fails(lambda: compare.fields(mutated), "wrong owning EH symbol")

mutated["linkages"][0]["symbol"] = unit["linkages"][0]["symbol"]
mutated["linkages"][0]["type"] = "REL32"
fails(lambda: compare.fields(mutated), "non-DIR32 EH field")

mutated["linkages"][0]["type"] = "DIR32"
mutated["linkages"][0]["target"] += 1
bad = compare.replay(mutated, image, directory / "source.map",
                     directory / "source.pdb", target)
assert bad["result"] == "mismatch" and bad["difference_count"] > 0
print("rejected: incorrect original EH target byte")

map_text = (directory / "source.map").read_text(encoding="cp1252")
handler = unit["linkages"][0]["symbol"]
lines = [line for line in map_text.splitlines() if handler in line]
assert len(lines) == 1
with tempfile.TemporaryDirectory(dir=ROOT / ".analysis") as temp:
    changed = Path(temp) / "mutated.map"
    changed.write_text(map_text.replace(lines[0], lines[0].replace(
        "CIL library: CIL module", "fake-provider")), encoding="cp1252")
    fails(lambda: compare.replay(unit, image, changed,
          directory / "source.pdb", target), "falsified static MAP provenance")

    changed.write_text(map_text.replace(lines[0], lines[0] + "\n" + lines[0]),
                       encoding="cp1252")
    fails(lambda: compare.replay(unit, image, changed,
          directory / "source.pdb", target), "ambiguous duplicated EH static")

print("factory EH strict replay and negative tests: PASS")
