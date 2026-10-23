"""Bind complete VC9 linked artifacts to their cold compilation context."""
from __future__ import annotations
import hashlib
import json
import os
from pathlib import Path
import re
import tomllib
from ltcg_link import LINK_PROFILE, HARNESS_KIND, ANCHOR_SIZE, support_profile_overrides

ROOT = Path(__file__).resolve().parents[1]


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def below(value: str, *, build: bool = False) -> Path:
    path = (ROOT / value).resolve()
    path.relative_to((ROOT / 'build').resolve() if build else ROOT)
    return path


def contract(unit: dict) -> dict:
    if unit.get('artifact_kind') != 'linked-pe':
        raise ValueError('explicit linked-pe artifact kind required')
    if any(key in unit for key in ('compare_size', 'owner_visibility', 'pdb_source', 'data_spans')):
        raise ValueError('this lane accepts complete public primary-TU contributions only')
    profile = unit.get('profile', [])
    if (not isinstance(profile, list) or not profile
            or any(not isinstance(flag, str) or not flag or flag != flag.strip() for flag in profile)
            or [flag.lower() for flag in profile].count('/gl') != 1
            or any(flag.lower().startswith(('/gl', '-gl')) and flag.lower() != '/gl' for flag in profile)):
        raise ValueError('one explicit /GL profile required')
    source = str(below(unit['source']).relative_to(ROOT))
    supports = [str(below(p).relative_to(ROOT)) for p in unit.get('support_sources', [])]
    if len(set([source, *supports])) != 1 + len(supports):
        raise ValueError('linked source inputs must be unique')
    normals = sorted(str(below(p).relative_to(ROOT)) for p in unit.get('normal_support_sources', []))
    if len(set(normals)) != len(normals) or not set(normals).issubset(supports):
        raise ValueError('normal source must be a unique selected support')
    overrides = support_profile_overrides(
        [below(p) for p in supports], [below(p) for p in normals], unit.get('support_profiles', {}))
    overrides = {str(p.relative_to(ROOT)): flags for p, flags in overrides.items()}
    entry = unit.get('entry')
    if not isinstance(entry, str) or not entry or any(c.isspace() for c in entry):
        raise ValueError('linked context requires one exact entry symbol')
    inputs = {source, *supports, *unit.get('inputs', [])}
    pending = list(inputs)
    all_flags = [*profile, *(flag for flags in overrides.values() for flag in flags)]
    include_dirs = [ROOT / f[2:] for f in all_flags if f[:2].lower() in ('/i', '-i') and len(f) > 2]
    while pending:
        path = below(pending.pop())
        if not path.is_file():
            raise ValueError(f'missing linked source input: {path}')
        for name in re.findall(r'^\s*#\s*include\s*"([^"]+)"', path.read_text(), re.MULTILINE):
            resolved = next((p.resolve() for p in [path.parent / name, *(d / name for d in include_dirs)] if p.is_file()), None)
            if resolved is None:
                raise ValueError(f'unbound linked quoted include {name!r}')
            item = str(resolved.relative_to(ROOT))
            if item not in inputs:
                inputs.add(item)
                pending.append(item)
    directory = str(below(unit['output_dir'], build=True).relative_to(ROOT))
    return dict(artifact_kind='linked-pe', source=source, inputs=sorted(inputs), profile=profile,
                output_dir=directory, entry=entry, support_sources=supports,
                normal_support_sources=normals, support_profiles=overrides,
                link_profile=LINK_PROFILE, harness_kind=HARNESS_KIND, anchor_size=ANCHOR_SIZE)


def provenance(unit: dict) -> dict:
    context = contract(unit)
    with (ROOT / 'config/tools.lock.toml').open('rb') as stream:
        tool = tomllib.load(stream)['msvc90']
    selection = Path(os.environ.get('TH11_MSVC90_ROOT', str(ROOT / tool['selection'])))
    observed = {name: digest(selection / row['path']) for name, row in tool['components'].items()}
    if observed != {name: row['sha256'] for name, row in tool['components'].items()}:
        raise ValueError('linked toolchain identity mismatch')
    with (ROOT / 'config/target.toml').open('rb') as stream:
        target = tomllib.load(stream)['target']['sha256']
    producers = ['scripts/ltcg_link.py', 'scripts/linked_receipt.py',
                 'scripts/compile-probe.sh', 'scripts/run-headless-wine.sh']
    return dict(schema_version=1, target_sha256=target, context=context,
                source_sha256={p: digest(ROOT / p) for p in context['inputs']},
                tools_lock_sha256=digest(ROOT / 'config/tools.lock.toml'), tools_sha256=observed,
                producer_sha256={p: digest(ROOT / p) for p in producers})


def receipt(unit: dict) -> dict:
    result = provenance(unit)
    context = result['context']
    directory = ROOT / context['output_dir']
    required = ['source.exe', 'source.map', 'source.pdb', 'source.ltcg.obj']
    for i, p in enumerate(context['support_sources']):
        kind = 'normal' if p in context['normal_support_sources'] else 'ltcg'
        required.append(f'support-{i:02d}-{Path(p).stem}.{kind}.obj')
    optional = ['link-anchors.c', 'link-anchors.obj', 'link-aliases.rsp']
    if any(not (directory / p).is_file() for p in required):
        raise ValueError('linked artifact family is incomplete')
    present = [(directory / p).is_file() for p in optional]
    if any(present) and not all(present):
        raise ValueError('partial linked anchor artifact family')
    result['artifacts_sha256'] = {p: digest(directory / p) if (directory / p).is_file() else None
                                  for p in [*required, *optional]}
    return result


def verify_receipt(unit: dict) -> None:
    expected = receipt(unit)
    path = ROOT / expected['context']['output_dir'] / 'linked.receipt.json'
    if json.loads(path.read_text()) != expected:
        raise ValueError('stale linked receipt: cold-build the selected context')
