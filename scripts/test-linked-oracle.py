#!/usr/bin/env python3
"""Synthetic falsification controls for the VC9 public linked Oracle."""
from copy import deepcopy
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import tempfile
import tomllib
import unittest
from unittest.mock import patch
import linked_receipt
from linked_image import PEImage
from linked_test_fixtures import Evidence, row, CODE

spec = importlib.util.spec_from_file_location('linked_oracle', Path(__file__).with_name('compare-linked-function.py'))
oracle = importlib.util.module_from_spec(spec)
spec.loader.exec_module(oracle)


class LinkedControls(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.e = Evidence(self.root)
        self.unit = dict(artifact_kind='linked-pe', symbol='_unit', size=12, target_address=0x401000,
                         linkages=[dict(offset=1, type='DIR32', symbol='_data', target=0x405000),
                                   dict(offset=6, type='REL32', symbol='_dep', target=0x404000)])
        self.code = b'\xa1' + struct.pack('<I', 0x401080) + b'\xe8' + struct.pack('<i', 0x20 - 10) + b'\xc3\x90'
        self.original = b'\xa1' + struct.pack('<I', 0x405000) + b'\xe8' + struct.pack('<i', 0x3000 - 10) + b'\xc3\x90'
        self.write()

    def write(self):
        self.e.write()
        data = bytearray(self.e.image.read_bytes())
        data[0x200:0x20c] = self.code
        # One authoritative HIGHLOW field plus an ABSOLUTE padding record.
        struct.pack_into('<II', data, 0x98 + 96 + 5 * 8, 0x1200, 12)
        struct.pack_into('<IIHH', data, 0x400, 0x1000, 12, 0x3001, 0)
        self.e.image.write_bytes(data)
        target = bytearray(data)
        target[0x200:0x20c] = self.original
        self.target = self.root / 'target.exe'
        self.target.write_bytes(target)

    def replay(self):
        return oracle.replay(self.unit, PEImage(self.e.image), self.e.map, self.e.pdb, PEImage(self.target))

    def test_complete_dir32_rel32_positive(self):
        result = self.replay()
        self.assertEqual(result['result'], 'exact')
        self.assertEqual(len(result['replayed_fields']), 2)
        self.assertEqual(result['owner']['size'], 12)

    def test_last_byte_is_compared(self):
        self.code = self.code[:-1] + b'\xcc'
        self.write()
        self.assertEqual(self.replay()['first_differences'], [11])

    def test_cropped_or_overlapping_pdb_extent_rejected(self):
        for contributions in [[(0, 11, CODE, 0), (0x20, 8, CODE, 1)],
                              [(0, 12, CODE, 0), (8, 8, CODE, 1)],
                              [(0, 12, CODE, 0), (0, 12, CODE, 0)]]:
            self.e.contributions = contributions
            self.write()
            with self.subTest(contributions=contributions), self.assertRaises(ValueError):
                self.replay()

    def test_interior_or_duplicate_map_owner_rejected(self):
        baseline = list(self.e.publics)
        for extra in [row('_Interior', 5), row('_Alias', 0), row('_unit', 0)]:
            self.e.publics = baseline + [extra]
            self.write()
            with self.subTest(extra=extra), self.assertRaises(ValueError):
                self.replay()

    def test_wrong_primary_object_rejected(self):
        self.e.modules[0] = str(self.root / 'wrong.ltcg.obj')
        self.write()
        with self.assertRaisesRegex(ValueError, 'primary'):
            self.replay()

    def test_wrong_map_object_rejected(self):
        self.e.publics[0] = row('_unit', 0, 'wrong.obj')
        self.write()
        with self.assertRaisesRegex(ValueError, 'owners'):
            self.replay()

    def test_qualified_map_path_and_duplicate_module_rejected(self):
        self.e.publics[0] = row('_unit', 0, '/elsewhere/source.ltcg.obj')
        self.write()
        with self.assertRaises(ValueError): self.replay()
        self.e.publics[0] = row('_unit', 0)
        self.e.modules.append(str(self.root / 'other/source.ltcg.obj'))
        self.write()
        with self.assertRaisesRegex(ValueError, 'module'):
            self.replay()

    def test_pe_map_pdb_identity_rejected(self):
        for attribute, value in [('pdb_guid', b'\xff' * 16), ('pdb_age', 2), ('dbi_age', 2),
                                 ('map_stamp', 3), ('map_base', 0x500000)]:
            old = getattr(self.e, attribute)
            setattr(self.e, attribute, value)
            self.write()
            with self.subTest(attribute=attribute), self.assertRaises(ValueError):
                self.replay()
            setattr(self.e, attribute, old)

    def test_undeclared_field_and_wrong_symbol_rejected(self):
        saved = deepcopy(self.unit['linkages'])
        self.unit['linkages'].pop()
        with self.assertRaisesRegex(ValueError, 'fields differ'): self.replay()
        self.unit['linkages'] = saved
        self.unit['linkages'][1]['symbol'] = '_not_the_callee'
        with self.assertRaisesRegex(ValueError, 'resolve uniquely'): self.replay()

    def test_wrong_replay_destination_is_mismatch(self):
        self.unit['linkages'][0]['target'] += 1
        self.assertEqual(self.replay()['result'], 'mismatch')

    def test_member_addend_must_bind_the_real_base(self):
        self.code = b'\xa1' + struct.pack('<I', 0x401084) + self.code[5:]
        self.write()
        self.unit['linkages'][0].update(target=0x404ffc, addend=4)
        self.assertEqual(self.replay()['result'], 'exact')
        # The encoded target bytes would still agree, but the candidate no
        # longer names _data plus its actual member displacement.
        self.unit['linkages'][0].update(target=0x404ff8, addend=8)
        with self.assertRaisesRegex(ValueError, 'resolve uniquely'):
            self.replay()

    def test_local_binding_cannot_change_offset(self):
        self.code = b'\xa1' + struct.pack('<I', 0x401005) + self.code[5:]
        self.write()
        self.unit['linkages'][0].update(local=True, target=0x401006)
        with self.assertRaisesRegex(ValueError, 'local linked destination'):
            self.replay()

    def test_invalid_field_selectors_and_overlaps_rejected(self):
        baseline = deepcopy(self.unit['linkages'])
        for change in [dict(symbol='_th11_ltcg_probe_anchor_0000'), dict(width=2),
                       dict(offset=11), dict(static_source='src/private.cpp'), dict(local='yes')]:
            self.unit['linkages'] = deepcopy(baseline)
            self.unit['linkages'][0].update(change)
            with self.subTest(change=change), self.assertRaises(ValueError): self.replay()
        self.unit['linkages'] = baseline + [deepcopy(baseline[0])]
        with self.assertRaisesRegex(ValueError, 'overlapping'):
            self.replay()

    def test_incomplete_decode_or_nonexecutable_section_rejected(self):
        self.code = self.code[:-1] + b'\x0f'
        self.write()
        with self.assertRaisesRegex(ValueError, 'decoder'):
            self.replay()
        self.code = self.code[:-1] + b'\x90'
        self.e.pe_characteristics = 0x40000040
        self.write()
        with self.assertRaisesRegex(ValueError, 'executable'):
            self.replay()

    def test_invalid_anchor_alias_rejected(self):
        path = self.root / 'link-aliases.rsp'
        path.write_text('/alternatename:_dep=_th11_ltcg_probe_anchor_0000\n')
        with self.assertRaisesRegex(ValueError, 'resolve uniquely'):
            self.replay()
        path.write_text('/alternatename:a=_th11_ltcg_probe_anchor_0000\n/alternatename:b=_th11_ltcg_probe_anchor_0000\n')
        with self.assertRaisesRegex(ValueError, 'not unique'):
            self.replay()

    def test_anchor_binding_and_wrong_producer(self):
        self.e.publics[1] = row('_th11_ltcg_probe_anchor_0000', 0x20, 'link-anchors.obj')
        self.write()
        (self.root / 'link-aliases.rsp').write_text('/alternatename:_dep=_th11_ltcg_probe_anchor_0000\n')
        self.assertEqual(self.replay()['result'], 'exact')
        self.e.publics[1] = row('_th11_ltcg_probe_anchor_0000', 0x20, 'wrong.obj')
        self.write()
        with self.assertRaisesRegex(ValueError, 'wrong producer'):
            self.replay()

    def control_plane(self):
        directory = self.root / 'build/unit'
        directory.mkdir(parents=True)
        self.e = Evidence(directory)
        self.e.image, self.e.map, self.e.pdb = [directory / ('source.' + p) for p in ['exe', 'map', 'pdb']]
        self.write()
        for name in ['src', 'config', 'resources']:
            (self.root / name).mkdir()
        (self.root / 'src/unit.cpp').write_text('int unit() { return 1; }\n')
        original_root = Path(__file__).resolve().parents[1]
        (self.root / 'config/tools.lock.toml').write_bytes((original_root / 'config/tools.lock.toml').read_bytes())
        data = self.target.read_bytes()
        (self.root / 'resources/th11.exe').write_bytes(data)
        sha = hashlib.sha256(data).hexdigest()
        (self.root / 'config/target.toml').write_text(f'[target]\nsha256="{sha}"\nsize={len(data)}\n')
        self.manifest = self.root / 'config/match-units.toml'
        self.manifest.write_text(f'''target_sha256="{sha}"
[units.fixture]
artifact_kind="linked-pe"
source="src/unit.cpp"
profile=["/TP", "/GL"]
entry="_unit"
output_dir="build/unit"
symbol="_unit"
size=12
target_address=0x401000
linkages=[{{offset=1,type="DIR32",symbol="_data",target=0x405000}},
          {{offset=6,type="REL32",symbol="_dep",target=0x404000}}]
''')
        self.bounds = self.root / 'config/function-boundaries.csv'
        self.bounds.write_text('address,span_end,state\n0x401000,0x40100B,reviewed\n')
        self.owners = self.root / 'config/functions.csv'
        self.owners.write_text('address,size,source_file\n0x401000,12,src/unit.cpp\n')
        for module in [oracle, linked_receipt]:
            context = patch.object(module, 'ROOT', self.root)
            context.start(); self.addCleanup(context.stop)
        # Receipt mechanics are independently exercised by ReceiptControls.
        context = patch.object(oracle, 'verify_receipt', lambda unit: None)
        context.start(); self.addCleanup(context.stop)

    def test_top_level_positive_and_reviewed_owner_gate(self):
        self.control_plane()
        self.assertEqual(oracle.compare_unit('fixture')['result'], 'exact')
        for contents in ['address,span_end,state\n0x401000,0x40100A,reviewed\n',
                         'address,span_end,state\n0x401000,0x40100B,needs_review\n']:
            self.bounds.write_text(contents)
            with self.assertRaisesRegex(ValueError, 'complete reviewed'):
                oracle.compare_unit('fixture')

    def test_top_level_source_target_and_receipt_gates(self):
        self.control_plane()
        self.owners.write_text('address,size,source_file\n0x401000,12,src/other.cpp\n')
        with self.assertRaisesRegex(ValueError, 'source owner'): oracle.compare_unit('fixture')
        self.owners.write_text('address,size,source_file\n0x401000,12,src/unit.cpp\n')
        with patch.object(oracle, 'verify_receipt', side_effect=ValueError('stale linked receipt')):
            with self.assertRaisesRegex(ValueError, 'stale'): oracle.compare_unit('fixture')
        self.manifest.write_text(self.manifest.read_text().replace('target_sha256=', 'wrong_target_sha256='))
        with self.assertRaisesRegex(ValueError, 'SHA-256'): oracle.compare_unit('fixture')

    def test_top_level_original_hash_gate(self):
        self.control_plane()
        path = self.root / 'resources/th11.exe'
        data = path.read_bytes()
        path.write_bytes(data[:-1] + bytes([data[-1] ^ 1]))
        with self.assertRaisesRegex(ValueError, 'target identity'):
            oracle.compare_unit('fixture')


class ReceiptControls(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        context = patch.object(linked_receipt, 'ROOT', self.root)
        context.start(); self.addCleanup(context.stop)
        for d in ['src', 'tools', 'config', 'scripts', 'build/unit']:
            (self.root / d).mkdir(parents=True)
        for p in ['scripts/ltcg_link.py', 'scripts/linked_receipt.py', 'scripts/compile-probe.sh',
                  'scripts/run-headless-wine.sh', 'src/unit.hpp', 'tools/compiler']:
            (self.root / p).write_text(p)
        (self.root / 'src/unit.cpp').write_text('#include "unit.hpp"\nint unit() { return 1; }\n')
        (self.root / 'config/target.toml').write_text('[target]\nsha256="' + '0' * 64 + '"\n')
        digest = hashlib.sha256((self.root / 'tools/compiler').read_bytes()).hexdigest()
        (self.root / 'config/tools.lock.toml').write_text('[msvc90]\nselection="tools"\n[msvc90.components.compiler]\npath="compiler"\nsha256="' + digest + '"\n')
        self.unit = dict(artifact_kind='linked-pe', source='src/unit.cpp', profile=['/TP', '/O2', '/GL'],
                         entry='_unit', output_dir='build/unit')
        for p in ['source.exe', 'source.map', 'source.pdb', 'source.ltcg.obj']:
            (self.root / 'build/unit' / p).write_text(p)
        self.path = self.root / 'build/unit/linked.receipt.json'
        self.path.write_text(json.dumps(linked_receipt.receipt(self.unit)))

    def test_receipt_and_recursive_header_binding(self):
        linked_receipt.verify_receipt(self.unit)
        self.assertIn('src/unit.hpp', linked_receipt.contract(self.unit)['inputs'])

    def test_changed_source_header_producer_and_artifacts_rejected(self):
        for p in ['src/unit.cpp', 'src/unit.hpp', 'scripts/ltcg_link.py',
                  'build/unit/source.exe', 'build/unit/source.map', 'build/unit/source.pdb',
                  'build/unit/source.ltcg.obj']:
            path = self.root / p; data = path.read_bytes()
            path.write_bytes(data + b'\n')
            with self.subTest(path=p), self.assertRaisesRegex(ValueError, 'stale'):
                linked_receipt.verify_receipt(self.unit)
            path.write_bytes(data)

    def test_tool_and_partial_anchor_family_rejected(self):
        pdb = self.root / 'build/unit/source.pdb'
        data = pdb.read_bytes(); pdb.unlink()
        with self.assertRaisesRegex(ValueError, 'incomplete'):
            linked_receipt.verify_receipt(self.unit)
        pdb.write_bytes(data)
        (self.root / 'tools/compiler').write_text('changed')
        with self.assertRaisesRegex(ValueError, 'toolchain'):
            linked_receipt.verify_receipt(self.unit)
        (self.root / 'tools/compiler').write_text('tools/compiler')
        (self.root / 'build/unit/link-anchors.obj').write_text('partial')
        with self.assertRaisesRegex(ValueError, 'partial'):
            linked_receipt.verify_receipt(self.unit)

    def test_profile_crop_and_path_contracts_rejected(self):
        for changes in [dict(profile=['/O2']), dict(profile=['/GL', '/GL']),
                        dict(profile=['/GL', '/GL-']), dict(profile=['/GL', '-GL-']), dict(compare_size=1),
                        dict(pdb_source='src/support.cpp'), dict(output_dir='../escape')]:
            with self.subTest(changes=changes), self.assertRaises(ValueError):
                linked_receipt.contract({**self.unit, **changes})

    def test_support_override_cannot_disable_ltcg(self):
        from ltcg_link import support_profile_overrides, ROOT as tool_root
        source = tool_root / 'src/support.cpp'
        for profile in [['/GL', '-GL-'], ['/GL', '/GL-'], ['/GL', '-GL']]:
            with self.subTest(profile=profile), self.assertRaises(ValueError):
                support_profile_overrides([source], (), {source: profile})


class BuildGraphControls(unittest.TestCase):
    def test_shared_output_requires_identical_context(self):
        path = Path(__file__).with_name('build-match-unit.py')
        spec = importlib.util.spec_from_file_location('linked_build_graph', path)
        driver = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(driver)
        root = path.resolve().parents[1]
        with (root / 'config/target.toml').open('rb') as stream:
            target = tomllib.load(stream)
        unit = dict(artifact_kind='linked-pe', source='src/JoystickButtons.cpp',
                    profile=['/TP', '/O2', '/GL'], entry='?PollJoystickButtons@@YAPAEXZ',
                    output_dir='build/test-linked-graph', functions=['PollJoystickButtons'],
                    symbol='?PollJoystickButtons@@YAPAEXZ', size=267, linkages=[])
        other = deepcopy(unit)
        other['profile'] = ['/TP', '/O1', '/GL']
        manifest = dict(schema_version=1, target_sha256=target['target']['sha256'], units=dict(a=unit, b=other))
        with patch.object(driver.tomllib, 'load', side_effect=[manifest, target]):
            with self.assertRaisesRegex(ValueError, 'share an output'):
                driver.units()
        other['output_dir'] = 'build/test-linked-graph-b'
        with patch.object(driver.tomllib, 'load', side_effect=[manifest, target]):
            self.assertEqual(len(driver.units()), 2)


if __name__ == '__main__':
    unittest.main()
