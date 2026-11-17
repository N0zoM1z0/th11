#!/usr/bin/env python3
"""Negative tests for original VC9 compiler-generated EH static MAP bindings.

Only reads receipt-bound target and linked artifacts. Mutated MAPs live inside
an ephemeral analysis directory, never in the native linked-output folder.
"""
import importlib.util
from pathlib import Path
import tempfile
import tomllib

from linked_image import PEImage

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    "compare_linked_function", ROOT / "scripts/compare-linked-function.py")
compare = importlib.util.module_from_spec(spec)
spec.loader.exec_module(compare)
units = tomllib.loads((ROOT / "config/match-units.toml").read_text())["units"]
target = PEImage(ROOT / "resources/th11.exe")


def rejects(call, label):
    try:
        call()
    except ValueError:
        print("rejected:", label)
        return
    raise AssertionError("accepted a false EH static binding: " + label)


def check(owner):
    unit = units[owner]
    directory = ROOT / unit["output_dir"]
    image = PEImage(directory / "source.exe")
    map_path = directory / "source.map"
    pdb_path = directory / "source.pdb"
    fields = compare.fields(unit)
    handlers = [field for field in fields if field["eh_static"]]
    assert len(handlers) == 1 and handlers[0]["offset"] == 3

    result = compare.replay(unit, image, map_path, pdb_path, target)
    assert result["result"] == "exact" and result["difference_count"] == 0

    corrupted = dict(unit)
    corrupted["linkages"] = [dict(field) for field in unit["linkages"]]
    corrupted["linkages"][0]["symbol"] += "_not_this_owner"
    rejects(lambda: compare.fields(corrupted), owner + ": wrong owner identity")

    corrupted["linkages"][0]["symbol"] = unit["linkages"][0]["symbol"]
    corrupted["linkages"][0]["type"] = "REL32"
    rejects(lambda: compare.fields(corrupted), owner + ": non-DIR32 EH field")

    corrupted["linkages"][0]["type"] = "DIR32"
    corrupted["linkages"][0]["target"] += 1
    replay = compare.replay(corrupted, image, map_path, pdb_path, target)
    assert replay["result"] == "mismatch" and replay["difference_count"] > 0
    print("rejected:", owner, ": incorrect native EH target")

    original_map = map_path.read_text(encoding="cp1252")
    handler = unit["linkages"][0]["symbol"]
    original_lines = [line for line in original_map.splitlines() if handler in line]
    assert len(original_lines) == 1
    with tempfile.TemporaryDirectory(dir=ROOT / ".analysis") as location:
        altered_map = Path(location) / "mutated.map"
        altered_map.write_text(original_map.replace(
            original_lines[0], original_lines[0].replace(
                "CIL library: CIL module", "fake-provider")), encoding="cp1252")
        rejects(lambda: compare.replay(unit, image, altered_map, pdb_path, target),
                owner + ": counterfeit MAP provider")

        altered_map.write_text(original_map.replace(
            original_lines[0], original_lines[0] + "\n" + original_lines[0]),
            encoding="cp1252")
        rejects(lambda: compare.replay(unit, image, altered_map, pdb_path, target),
                owner + ": duplicate MAP EH static")


for name in ("secondary-spell-stage-factory-frontier",
             "secondary-spell-stage-destructor"):
    check(name)

print("factory + destructor EH strict replay and negative tests: PASS")
