#!/usr/bin/env python3
"""Public synthetic negative controls for the exact Oracle's acceptance boundary."""
import importlib.util
import json
from pathlib import Path
import struct
import tempfile
import tomllib
import unittest
from unittest.mock import patch

spec = importlib.util.spec_from_file_location('coff_oracle', Path(__file__).with_name('compare-coff-function.py'))
oracle = importlib.util.module_from_spec(spec)
spec.loader.exec_module(oracle)


def synthetic_object(path, code, *, definition_size=None, relocation=False):
    # One synthetic function-definition record; no game bytes are used here.
    raw = 60
    reloc_bytes = struct.pack('<IIH', 1, 2, 0x14) if relocation else b''
    sym_offset = raw + len(code) + len(reloc_bytes)
    symbols = struct.pack('<8sIhHBB', b'_unit', 0, 1, 0x20, 2, 1)
    symbols += struct.pack('<IIIIH', 0, len(code) if definition_size is None else definition_size, 0, 0, 0)
    if relocation:
        symbols += struct.pack('<8sIhHBB', b'_dep', 0, 0, 0x20, 2, 0)
    header = struct.pack('<HHIIIHH', 0x14c, 1, 0, sym_offset, 3 if relocation else 2, 0, 0)
    section = struct.pack('<8sIIIIIIHHI', b'.text', 0, 0, len(code), raw, raw + len(code), 0, int(relocation), 0, 0x60000020)
    path.write_bytes(header + section + code + reloc_bytes + symbols + struct.pack('<I', 4))


class AcceptanceControls(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)
        (self.root / 'build').mkdir()
        self.obj = self.root / 'build/unit.obj'
        self.units = self.root / 'units.toml'
        self.write_unit()
        for attr, value in [('ROOT', self.root), ('UNITS_MANIFEST', self.units),
                            ('verify_receipt', lambda unit: None), ('verified_target', lambda: b'original'),
                            ('target_bytes', lambda data, addr, size: bytes.fromhex('b801000000c3')[:size])]:
            context = patch.object(oracle, attr, value)
            context.start(); self.addCleanup(context.stop)

    def write_unit(self, *, relocations='[]'):
        with oracle.TARGET_MANIFEST.open('rb') as stream:
            target_hash = tomllib.load(stream)['target']['sha256']
        self.units.write_text('target_sha256="' + target_hash + '"\n[units.synthetic]\nartifact_kind="coff"\nsource="synthetic.cpp"\nobject="build/unit.obj"\nsymbol="_unit"\nsize=6\ntarget_address=4198400\nrelocations=' + relocations + '\n')

    def test_complete_extent_equal(self):
        synthetic_object(self.obj, bytes.fromhex('b801000000c3'))
        self.assertEqual(oracle.compare_unit('synthetic')['result'], 'exact')

    def test_changed_last_byte_fails(self):
        synthetic_object(self.obj, bytes.fromhex('b801000000cc'))
        self.assertEqual(oracle.compare_unit('synthetic')['result'], 'mismatch')

    def test_smaller_object_cannot_borrow_following_bytes(self):
        synthetic_object(self.obj, bytes.fromhex('b801000000'))
        with self.assertRaisesRegex(ValueError, 'size'):
            oracle.compare_unit('synthetic')

    def test_auxiliary_extent_cannot_escape_section(self):
        synthetic_object(self.obj, bytes.fromhex('b801000000c3'), definition_size=7)
        with self.assertRaisesRegex(ValueError, 'beyond'):
            oracle.compare_unit('synthetic')

    def test_undeclared_relocation_fails(self):
        synthetic_object(self.obj, bytes.fromhex('e800000000c3'), relocation=True)
        with self.assertRaisesRegex(ValueError, 'relocations differ'):
            oracle.compare_unit('synthetic')

    def test_declared_rel32_replays_entire_function(self):
        synthetic_object(self.obj, bytes.fromhex('e800000000c3'), relocation=True)
        self.write_unit(relocations='[{offset=1,type="REL32",symbol="_dep",target=4198656}]')
        with patch.object(oracle, 'target_bytes', lambda data, addr, size: bytes.fromhex('e8fb000000c3')):
            self.assertEqual(oracle.compare_unit('synthetic')['result'], 'exact')
        with patch.object(oracle, 'target_bytes', lambda data, addr, size: bytes.fromhex('e8fc000000c3')):
            self.assertEqual(oracle.compare_unit('synthetic')['result'], 'mismatch')

    def test_wrong_target_manifest_fails(self):
        synthetic_object(self.obj, bytes.fromhex("b801000000c3"))
        self.units.write_text(self.units.read_text().replace("target_sha256=", "wrong_target_sha256="))
        with self.assertRaisesRegex(ValueError, "target SHA-256"):
            oracle.compare_unit("synthetic")

    def test_stale_receipt_fails(self):
        from artifact_receipt import verify_receipt
        receipt = {'context': {'object': 'build/unit.obj'}, 'object_sha256': 'current'}
        path = Path(str(self.obj) + '.receipt.json')
        path.write_text(json.dumps({**receipt, 'object_sha256': 'stale'}))
        with patch('artifact_receipt.ROOT', self.root), patch('artifact_receipt.receipt', lambda unit: receipt):
            with self.assertRaisesRegex(ValueError, 'stale'):
                verify_receipt({})


if __name__ == '__main__':
    unittest.main()
