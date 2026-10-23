#!/usr/bin/env python3
"""Replay one complete, receipt-bound VC9 public linked-PE function."""
from __future__ import annotations
import argparse
import csv
import hashlib
import json
from pathlib import Path
import sys
import tomllib
from linked_image import (PEImage, linked_functions, linked_code_fields, map_publics,
                          pdb_section_contributions, verify_capstone, _object_path,
                          IMAGE_SCN_CNT_CODE, IMAGE_SCN_MEM_EXECUTE)
from linked_receipt import contract, verify_receipt
from ltcg_link import ANCHOR_PREFIX

ROOT = Path(__file__).resolve().parents[1]
WIDTHS = {'DIR32': 4, 'REL8': 1, 'REL16': 2, 'REL32': 4}


def fields(unit: dict) -> list[dict]:
    rows = unit.get('linkages')
    if not isinstance(rows, list):
        raise ValueError('exhaustive linked linkage list required')
    result, occupied = [], set()
    for raw in rows:
        if not isinstance(raw, dict) or set(raw) - {'offset', 'type', 'width', 'symbol', 'target', 'addend', 'local'}:
            raise ValueError('unsupported linked field selector')
        kind = raw['type']
        if kind not in WIDTHS or raw.get('width', WIDTHS[kind]) != WIDTHS[kind]:
            raise ValueError('unsupported linked field width')
        offset, width = int(raw['offset']), WIDTHS[kind]
        extent = set(range(offset, offset + width))
        if offset < 0 or offset + width > int(unit['size']) or extent & occupied:
            raise ValueError('overlapping or escaping linked field')
        occupied.update(extent)
        local = raw.get('local', False)
        if type(local) is not bool:
            raise ValueError('local field marker must be boolean')
        symbol = raw.get('symbol')
        if not local and (not isinstance(symbol, str) or not symbol or symbol.startswith(ANCHOR_PREFIX)):
            raise ValueError('linked field requires its real source symbol')
        result.append(dict(offset=offset, width=width, type=kind, local=local,
                           symbol='<local>' if local else symbol, target=int(raw['target']),
                           addend=int(raw.get('addend', 0))))
    return sorted(result, key=lambda row: (row['offset'], row['type']))


def owner(image: PEImage, map_path: Path, pdb_path: Path, symbol: str, size: int) -> dict:
    linked = linked_functions(image.path, map_path, pdb_path)
    selected = [f for f in linked['functions'] if f['symbol'] == symbol]
    if len(selected) != 1 or int(selected[0]['size']) != size:
        raise ValueError('one complete PDB contribution of the declared size required')
    function = selected[0]
    expected = _object_path(image.path.parent / 'source.ltcg.obj')
    if _object_path(function['pdb_object']) != expected:
        raise ValueError('linked owner is not the primary source object')
    map_object = str(function['map_object']).replace('\\', '/').rsplit('/', 1)[-1]
    if map_object.lower() != 'source.ltcg.obj':
        raise ValueError('MAP and PDB object owners disagree')
    if any(c in str(function['map_object']) for c in ('/', '\\', ':')) and _object_path(function['map_object']) != expected:
        raise ValueError('qualified MAP object leaves its producer directory')
    section = image.section(function['section_number'])
    if section.characteristics & (IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE) != (IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE):
        raise ValueError('linked owner is not executable code')
    pdb = pdb_section_contributions(pdb_path)
    modules = [m for m in pdb['modules'] if str(m['object']).replace('\\', '/').rsplit('/', 1)[-1].lower() == 'source.ltcg.obj']
    if (len(modules) != 1 or _object_path(modules[0]['object']) != expected
            or modules[0]['index'] != function['pdb_module_index']):
        raise ValueError('ambiguous PDB module identity')
    start = int(function['section_offset'])
    overlaps = [r for r in pdb['contributions'] if r['section_number'] == function['section_number']
                and int(r['size']) > 0 and int(r['section_offset']) < start + size
                and start < int(r['section_offset']) + int(r['size'])]
    if (len(overlaps) != 1 or int(overlaps[0]['section_offset']) != start
            or int(overlaps[0]['size']) != size):
        raise ValueError('ambiguous or overlapping PDB owner')
    publics = map_publics(map_path)
    address = int(function['address'], 0)
    inside = [r for r in publics if address <= int(r['address']) < address + size]
    if len(inside) != 1 or inside[0]['symbol'] != symbol or 'f' not in inside[0]['flags']:
        raise ValueError('ambiguous MAP owner or interior public entry')
    return function


def aliases(path: Path) -> dict:
    result = {}
    if not path.exists():
        return result
    for line in path.read_text(encoding='ascii').splitlines():
        if not line.startswith('/alternatename:') or line.count('=') != 1:
            raise ValueError('invalid linked anchor directive')
        name, anchor = line[len('/alternatename:'):].split('=')
        if not name or name.startswith(ANCHOR_PREFIX) or not anchor.startswith(ANCHOR_PREFIX) or name in result:
            raise ValueError('invalid or repeated linked anchor')
        result[name] = anchor
    if len(set(result.values())) != len(result):
        raise ValueError('linked anchors are not unique')
    return result


def replay(unit: dict, image: PEImage, map_path: Path, pdb_path: Path, target: PEImage) -> dict:
    size, base = int(unit['size']), int(unit['target_address'])
    function = owner(image, map_path, pdb_path, unit['symbol'], size)
    address = int(function['address'], 0)
    publics = map_publics(map_path)
    decoded = linked_code_fields(image, publics, address, size)
    if not decoded['normalization_complete']:
        raise ValueError('linked decoder does not cover the complete owned extent')
    expected = fields(unit)
    key = lambda r: (int(r['offset']), r['type'], int(r['width']))
    if sorted(map(key, decoded['fields'])) != sorted(map(key, expected)):
        raise ValueError('exhaustive linked fields differ from manifest')
    observed = {key(r): r for r in decoded['fields']}
    names = aliases(image.path.parent / 'link-aliases.rsp')
    candidate = bytearray(image.read_address(address, size))
    for field in expected:
        actual = observed[key(field)]
        destination = int(actual['candidate_target']) - field['addend']
        if field['local']:
            if (not 0 <= destination - address < size
                    or destination - address != field['target'] - base):
                raise ValueError('local linked destination changes owner or offset')
        else:
            symbol = names.get(field['symbol'], field['symbol'])
            matches = [p for p in publics if p['symbol'] == symbol]
            if len(matches) != 1 or int(matches[0]['address']) != destination:
                raise ValueError('linked field does not resolve uniquely through its symbol')
            if symbol.startswith(ANCHOR_PREFIX) and str(matches[0]['object']).replace('\\', '/').rsplit('/', 1)[-1].lower() != 'link-anchors.obj':
                raise ValueError('linked anchor has the wrong producer')
        value = field['target'] + field['addend']
        signed = field['type'] != 'DIR32'
        if signed:
            if actual['instruction_end'] is None:
                raise ValueError('relative field lacks a decoded instruction end')
            value -= base + int(actual['instruction_end'])
        lo, hi = (-(1 << (field['width'] * 8 - 1)), (1 << (field['width'] * 8 - 1)) - 1) if signed else (0, 0xffffffff)
        if not lo <= value <= hi:
            raise ValueError('linked replay value is out of range')
        candidate[field['offset']:field['offset'] + field['width']] = value.to_bytes(field['width'], 'little', signed=signed)
    original = target.read_address(base, size)
    differences = [i for i, (a, b) in enumerate(zip(candidate, original)) if a != b]
    return dict(artifact_kind='linked-pe', result='mismatch' if differences else 'exact',
                symbol=unit['symbol'], size=size, target_address=f'0x{base:08X}', owner=function,
                replayed_fields=expected, difference_count=len(differences), first_differences=differences[:32])


def compare_unit(name: str) -> dict:
    with (ROOT / 'config/match-units.toml').open('rb') as stream:
        manifest = tomllib.load(stream)
    with (ROOT / 'config/target.toml').open('rb') as stream:
        target_spec = tomllib.load(stream)['target']
    if manifest.get('target_sha256') != target_spec['sha256']:
        raise ValueError('linked target SHA-256 contract mismatch')
    unit = manifest['units'][name]
    context = contract(unit)
    verify_receipt(unit)
    with (ROOT / 'config/tools.lock.toml').open('rb') as stream:
        verify_capstone(tomllib.load(stream)['capstone'])
    address, size = int(unit['target_address']), int(unit['size'])
    with (ROOT / 'config/function-boundaries.csv').open(newline='') as stream:
        bounds = [r for r in csv.DictReader(stream) if int(r['address'], 0) == address]
    with (ROOT / 'config/functions.csv').open(newline='') as stream:
        functions = [r for r in csv.DictReader(stream) if int(r['address'], 0) == address]
    if (len(bounds) != 1 or bounds[0]['state'] != 'reviewed' or int(bounds[0]['span_end'], 0) != address + size - 1
            or len(functions) != 1 or int(functions[0]['size']) != size or functions[0]['source_file'] != unit['source']):
        raise ValueError('linked unit lacks its complete reviewed target/source owner')
    target = PEImage(ROOT / 'resources/th11.exe')
    if len(target.data) != target_spec['size'] or hashlib.sha256(target.data).hexdigest() != target_spec['sha256']:
        raise ValueError('linked target identity mismatch')
    directory = ROOT / context['output_dir']
    result = replay(unit, PEImage(directory / 'source.exe'), directory / 'source.map', directory / 'source.pdb', target)
    result['unit'] = name
    return result


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--unit', required=True)
    args = parser.parse_args()
    try:
        result = compare_unit(args.unit)
    except (OSError, KeyError, TypeError, ValueError) as exc:
        print(f'error: {exc}', file=sys.stderr)
        return 1
    print(json.dumps(result, indent=2))
    return 0 if result['result'] == 'exact' else 1


if __name__ == '__main__':
    raise SystemExit(main())
