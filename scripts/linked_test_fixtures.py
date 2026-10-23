#!/usr/bin/env python3
"""Synthetic PE/PDB fixture builder adapted from the MIT TH10 reconstruction.

Builds tiny synthetic PE32 and MSF7/PDB evidence in a temporary directory.
No native compiler, target binary, source edits, or ledger writes are used.
"""
from __future__ import annotations

from pathlib import Path
import struct

import linked_image as linked

CODE = 0x60000020
SYMBOL = '?LocalOwner@@YAXXZ'
GUID = bytes(range(16))
STAMP = 0x12345678


def row(symbol=SYMBOL, offset=0x20, obj='source.ltcg.obj', flags='f', va=None):
    if va is None:
        va = 0x401000 + offset
    return f' 0001:{offset:08x} {symbol} {va:08x} {flags} {obj}'


def write_msf(path, streams):
    """Serialize a genuine one-directory-block MSF7 container."""
    blocks = [bytearray(512) for _ in range(3)]
    directory = bytearray(struct.pack('<I', len(streams)))
    directory.extend(struct.pack('<' + 'I' * len(streams), *(len(s) for s in streams)))
    for stream in streams:
        numbers = []
        for start in range(0, len(stream), 512):
            numbers.append(len(blocks))
            blocks.append(bytearray(stream[start:start + 512].ljust(512, b'\0')))
        if numbers:
            directory.extend(struct.pack('<' + 'I' * len(numbers), *numbers))
    assert len(directory) <= 512
    blocks[0][:32] = linked.MSF7_SIGNATURE
    struct.pack_into('<6I', blocks[0], 32, 512, 0, len(blocks), len(directory), 0, 1)
    struct.pack_into('<I', blocks[1], 0, 2)
    blocks[2][:len(directory)] = directory
    path.write_bytes(b''.join(blocks))


class Evidence:
    def __init__(self, directory):
        self.image = Path(directory) / 'fixture.exe'
        self.map = Path(directory) / 'fixture.map'
        self.pdb = Path(directory) / 'fixture.pdb'
        self.publics = [row('_unit', 0, 'source.ltcg.obj'), row('_dep', 0x20, 'dependency.obj'), row('_data', 0x80, 'dependency.obj', flags='')]
        self.statics = []
        self.modules = [str(Path(directory).resolve() / 'source.ltcg.obj'), str(Path(directory).resolve() / 'dependency.obj')]
        self.contributions = [(0, 12, CODE, 0), (0x20, 8, CODE, 1)]
        self.pe_characteristics = CODE
        self.map_stamp = STAMP
        self.map_base = 0x400000
        self.pdb_guid = GUID
        self.pdb_age = 1
        self.dbi_age = 1
        self.suffix = []

    def write(self):
        data = bytearray(0x600)
        data[:2] = b'MZ'
        struct.pack_into('<I', data, 0x3c, 0x80)
        data[0x80:0x84] = b'PE\0\0'
        struct.pack_into('<HHIIIHH', data, 0x84, 0x14c, 1, STAMP, 0, 0, 224, 0)
        optional = 0x98
        struct.pack_into('<H', data, optional, 0x10b)
        struct.pack_into('<I', data, optional + 28, 0x400000)
        struct.pack_into('<I', data, optional + 92, 16)
        struct.pack_into('<II', data, optional + 96 + 6 * 8, 0x1100, 28)
        section = optional + 224
        struct.pack_into('<8sIIII', data, section, b'.text\0\0\0', 0x400, 0x1000, 0x400, 0x200)
        struct.pack_into('<I', data, section + 36, self.pe_characteristics)
        data[0x200:0x280] = b'\x90' * 0x80
        cv = b'RSDS' + GUID + struct.pack('<I', 1) + b'fixture.pdb\0'
        struct.pack_into('<IIHHIIII', data, 0x300, 0, STAMP, 0, 0, 2, len(cv), 0x1140, 0x340)
        data[0x340:0x340 + len(cv)] = cv
        self.image.write_bytes(data)
        self.map.write_text('\n'.join([
            f' Timestamp is {self.map_stamp:08x}',
            f' Preferred load address is {self.map_base:08x}',
            ' Address Publics by Value Rva+Base Lib:Object', *self.publics,
            ' entry point at 0001:00000000', '', ' Static symbols', '',
            *self.statics, *self.suffix, '',
        ]), encoding='cp1252')
        modules = bytearray()
        for obj in self.modules:
            record = bytearray(64)
            struct.pack_into('<H', record, 34, 0xffff)
            record.extend(obj.encode('cp1252') + b'\0' + obj.encode('cp1252') + b'\0')
            record.extend(b'\0' * (-len(record) % 4))
            modules.extend(record)
        contributions = bytearray(struct.pack('<I', linked.DBI_SECTION_CONTRIB_V60))
        for offset, size, chars, module in self.contributions:
            contributions.extend(struct.pack('<H2xiiIH2xII', 1, offset, size, chars, module, 0, 0))
        dbi = bytearray(64)
        struct.pack_into('<iII', dbi, 0, -1, 19990903, self.dbi_age)
        struct.pack_into('<ii', dbi, 24, len(modules), len(contributions))
        info = struct.pack('<III', 20000404, 0, self.pdb_age) + self.pdb_guid
        write_msf(self.pdb, [b'', info, b'', bytes(dbi + modules + contributions)])

