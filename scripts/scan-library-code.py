#!/usr/bin/env python3
"""Find pinned VC9 CRT/STL archive code in the TH11 image.

Every code section of each archive member is matched against the target .text
with its relocated fields masked. A section is accepted only when it has one
hit and each of its REL32 relocations to another uniquely placed archive symbol
lands on that symbol. An inventory function fully covered by an accepted
section is reported as library code; --apply records that in the ledgers and
marks the boundary reviewed when the row equals the archive symbol extent.
"""
from __future__ import annotations

import argparse
import bisect
import collections
import csv
import importlib.util
import io
import json
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))
_spec = importlib.util.spec_from_file_location("cc", ROOT / "scripts/compare-coff-function.py")
cc = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(cc)

LIBRARIES = {"libcmt.lib": "MsvcCRT", "libcpmt.lib": "MsvcCPPRT"}
EVIDENCE = "vc9-crt-archive-scan-2026-11-14"


def archive_members(data: bytes):
    if data[:8] != b"!<arch>\n":
        raise ValueError("not a COFF archive")
    pos, longnames = 8, b""
    while pos + 60 <= len(data):
        header = data[pos:pos + 60]
        name, size = header[:16].decode().strip(), int(header[48:58])
        body = data[pos + 60:pos + 60 + size]
        if name == "//":
            longnames = body
        elif name not in ("/", ""):
            if name.startswith("/") and name[1:].isdigit():
                start = int(name[1:])
                name = longnames[start:longnames.index(b"\0", start)].decode()
            yield name.split("\\")[-1], body
        pos += 60 + size + (size & 1)


def code_sections(body: bytes):
    machine, count, _, symptr, nsym, optsize, _ = struct.unpack_from("<HHIIIHH", body)
    if machine != 0x14C:
        return []
    strings = body[symptr + nsym * 18:]

    def name_of(raw: bytes) -> str:
        if raw[:4] == b"\0\0\0\0":
            start = struct.unpack_from("<I", raw, 4)[0]
            return strings[start:strings.index(b"\0", start)].decode("latin1")
        return raw.rstrip(b"\0").decode("latin1")

    sections = []
    for index in range(count):
        fields = struct.unpack_from("<8sIIIIIIHHI", body, 20 + optsize + index * 40)
        sections.append({"size": fields[3], "ptr": fields[4], "relptr": fields[5],
                         "nrel": fields[7], "flags": fields[9], "symbols": []})
    names, index = [], 0
    while index < nsym:
        raw = body[symptr + index * 18:symptr + index * 18 + 18]
        name = name_of(raw[:8])
        value, section, kind, storage, aux = struct.unpack_from("<IhHBB", raw, 8)
        names.append(name)
        if (0 < section <= count and storage in (2, 3) and (kind & 0x20 or storage == 2)
                and not name.startswith((".", "$"))):
            sections[section - 1]["symbols"].append((name, value))
        names.extend([None] * aux)
        index += 1 + aux
    result = []
    for section in sections:
        if not section["flags"] & 0x20 or not section["size"] or not section["ptr"]:
            continue
        code = body[section["ptr"]:section["ptr"] + section["size"]]
        mask = bytearray(b"\1" * len(code))
        relocations = []
        for number in range(section["nrel"]):
            offset, symbol, kind = struct.unpack_from("<IIH", body, section["relptr"] + number * 10)
            mask[offset:offset + 4] = b"\0\0\0\0"
            relocations.append((offset, names[symbol], kind))
        result.append({"code": code, "mask": bytes(mask), "relocations": relocations,
                       "symbols": sorted(section["symbols"], key=lambda item: item[1])})
    return result


def longest_unmasked(mask: bytes) -> tuple[int, int]:
    best, start = (0, 0), None
    for index in range(len(mask) + 1):
        if index < len(mask) and mask[index]:
            start = index if start is None else start
        elif start is not None:
            if index - start > best[1] - best[0]:
                best = (start, index)
            start = None
    return best


def text_section(image: bytes) -> tuple[int, bytes]:
    pe = struct.unpack_from("<I", image, 0x3C)[0]
    count, optsize = struct.unpack_from("<H", image, pe + 6)[0], struct.unpack_from("<H", image, pe + 20)[0]
    base = struct.unpack_from("<I", image, pe + 52)[0]
    for index in range(count):
        name, vsize, va, _, raw = struct.unpack_from("<8sIIII", image, pe + 24 + optsize + index * 40)
        if name.rstrip(b"\0") == b".text":
            return base + va, image[raw:raw + vsize]
    raise ValueError("target has no .text")


def scan(lib_dir: Path) -> list[dict[str, object]]:
    image = cc.verified_target()
    text_va, text = text_section(image)
    matches = []
    for library, module in LIBRARIES.items():
        for member, body in archive_members((lib_dir / library).read_bytes()):
            for section in code_sections(body):
                code, mask = section["code"], section["mask"]
                first, last = longest_unmasked(mask)
                if len(code) < 6 or last - first < 4:
                    continue
                hits, pos = [], 0
                while (pos := text.find(code[first:last], pos)) >= 0:
                    start = pos - first
                    if start >= 0 and start + len(code) <= len(text) and all(
                            not mask[k] or text[start + k] == code[k] for k in range(len(code))):
                        hits.append(text_va + start)
                    pos += 1
                if hits:
                    matches.append({"library": library, "module": module, "member": member,
                                    "size": len(code), "symbols": section["symbols"],
                                    "relocations": section["relocations"], "hits": hits})
    placed = collections.defaultdict(set)
    for match in matches:
        if len(match["hits"]) == 1:
            for name, offset in match["symbols"]:
                placed[name].add(match["hits"][0] + offset)
    def check(match, start):
        checked, consistent = 0, True
        for offset, name, kind in match["relocations"]:
            if kind != 0x14 or len(placed.get(name, ())) != 1:
                continue
            field = start + offset
            target = field + 4 + struct.unpack("<i", cc.target_bytes(image, field, 4))[0]
            checked += 1
            consistent &= target in placed[name]
        return checked, consistent

    # A repeated section is placed only where a checked call proves the member;
    # newly placed symbols can disambiguate further sections, so iterate.
    accepted: dict[tuple[int, int], dict[str, object]] = {}
    while True:
        before = len(accepted)
        for number, match in enumerate(matches):
            for start in match["hits"]:
                checked, consistent = check(match, start)
                if consistent and (len(match["hits"]) == 1 or checked):
                    if (number, start) not in accepted:
                        accepted[number, start] = {**match, "start": start, "checked": checked}
                        for name, offset in match["symbols"]:
                            placed[name].add(start + offset)
        if len(accepted) == before:
            break
    return sorted(accepted.values(), key=lambda match: match["start"])


def classify(accepted, functions):
    starts = [match["start"] for match in accepted]
    rows = []
    for row in functions:
        address, end = int(row["address"], 16), int(row["span_end"], 16) + 1
        index = bisect.bisect_right(starts, address) - 1
        if index < 0:
            continue
        match = accepted[index]
        if not match["start"] <= address or match["start"] + match["size"] < end:
            continue
        offsets = sorted({offset for _, offset in match["symbols"]} | {match["size"]})
        relative = address - match["start"]
        names = [name for name, offset in match["symbols"] if offset == relative]
        symbol_end = offsets[offsets.index(relative) + 1] if relative in offsets[:-1] else None
        rows.append({"address": row["address"], "module": match["module"], "library": match["library"],
                     "member": match["member"], "section_va": f"0x{match['start']:08X}",
                     "section_size": match["size"], "symbol": names[0] if names else "",
                     "relocations": len(match["relocations"]), "rel32_checked": match["checked"],
                     "extent_equal": symbol_end is not None and match["start"] + symbol_end == end})
    return rows


def rewrite(path: Path, rows: list[dict[str, str]], fields: list[str]) -> None:
    out = io.StringIO()
    writer = csv.DictWriter(out, fieldnames=fields, lineterminator="\n")
    writer.writeheader()
    writer.writerows(rows)
    path.write_text(out.getvalue())


def apply(found) -> None:
    config = ROOT / "config"
    by_address = {item["address"]: item for item in found}
    tables = {}
    for name in ("functions", "function-origins", "function-boundaries"):
        with (config / f"{name}.csv").open(newline="") as stream:
            reader = csv.DictReader(stream)
            tables[name] = (list(reader), reader.fieldnames)
    for row in tables["functions"][0]:
        item = by_address.get(row["address"])
        if not item or row["source_file"] or row["status"] in ("exact", "excluded"):
            continue
        where = f"{item['library']} {item['member']} section at {item['section_va']} ({item['section_size']} bytes)"
        row.update(status="excluded", owner="library", module=item["module"])
        if item["symbol"] and not row["proposed_name"]:
            row["proposed_name"] = item["symbol"]
        boundary = ("archive symbol extent equals this row" if item["extent_equal"]
                    else "row lies inside the section but differs from its symbol extent; boundary unreviewed")
        row["evidence"] = (f"VC9 archive identity ({EVIDENCE}): {where} matches with relocations masked; "
                           f"{item['rel32_checked']}/{item['relocations']} relocations checked as REL32 targets; {boundary}.")
        row["notes"] = f"Pinned VC9 library code from {item['member']}."
    for row in tables["function-origins"][0]:
        item = by_address.get(row["address"])
        if item and row["disposition"] == "review":
            row.update(origin="library", subsystem=item["module"], disposition="exclude",
                       confidence="high", evidence_id=EVIDENCE)
    for row in tables["function-boundaries"][0]:
        item = by_address.get(row["address"])
        if item and item["extent_equal"] and row["state"] == "needs_review":
            row.update(state="reviewed", confidence="high", evidence_id=EVIDENCE,
                       notes=f"Extent equals the {item['symbol'] or 'unnamed'} symbol extent in {item['member']} "
                             f"({item['library']}), matched with relocations masked.")
    for name, (rows, fields) in tables.items():
        rewrite(config / f"{name}.csv", rows, fields)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--lib-dir", type=Path, default=ROOT / ".tools/msvc900/VC/lib")
    parser.add_argument("--apply", action="store_true", help="record the classification in config/*.csv")
    args = parser.parse_args()
    with (ROOT / "config/functions.csv").open(newline="") as stream:
        functions = list(csv.DictReader(stream))
    found = classify(scan(args.lib_dir), functions)
    if args.apply:
        apply(found)
    print(json.dumps({"functions": len(found),
                      "bytes": sum(int(r["size"]) for r in functions if r["address"] in {f["address"] for f in found}),
                      "extent_equal": sum(f["extent_equal"] for f in found)}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
