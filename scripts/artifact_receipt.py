"""Source/tool/target provenance for canonical TH11 normal-COFF artifacts."""
from __future__ import annotations
import hashlib
import json
import re
from pathlib import Path
import tomllib

ROOT = Path(__file__).resolve().parents[1]


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def local_file(value: str, *, build: bool = False) -> Path:
    path = (ROOT / value).resolve()
    path.relative_to((ROOT / 'build').resolve() if build else ROOT)
    if not build and not path.is_file():
        raise ValueError(f'missing source input: {value}')
    return path


def contract(unit: dict) -> dict:
    if unit.get('artifact_kind') != 'coff':
        raise ValueError('only reviewed normal-COFF units are accepted; LTCG/linked contexts need a separate Oracle')
    profile = unit.get('profile', [])
    if not profile or any(flag.lower().replace("-", "/", 1).startswith('/gl') for flag in profile):
        raise ValueError('explicit non-/GL compiler profile required')
    source = str(local_file(unit['source']).relative_to(ROOT))
    output = str(local_file(unit['object'], build=True).relative_to(ROOT))
    inputs = {source, *unit.get('inputs', [])}
    pending = list(inputs)
    include_dirs = [ROOT / flag[2:] for flag in profile if flag[:2].lower() in ('/i', '-i') and len(flag) > 2]
    while pending:
        path = local_file(pending.pop())
        for name in re.findall(r'^\s*#\s*include\s*"([^"]+)"', path.read_text(), re.MULTILINE):
            candidates = [path.parent / name, *(directory / name for directory in include_dirs)]
            resolved = next((candidate.resolve() for candidate in candidates if candidate.is_file()), None)
            if resolved is None:
                raise ValueError(f'cannot bind quoted include {name!r} from {path}')
            item = str(resolved.relative_to(ROOT))
            if item not in inputs:
                inputs.add(item)
                pending.append(item)
    inputs = sorted(inputs)
    return {'artifact_kind': 'coff', 'source': source, 'object': output, 'profile': profile, 'inputs': inputs}


def receipt_path(output: Path) -> Path:
    return Path(str(output) + '.receipt.json')


def receipt(unit: dict) -> dict:
    context = contract(unit)
    with (ROOT / 'config/target.toml').open('rb') as stream:
        target = tomllib.load(stream)['target']['sha256']
    with (ROOT / 'config/tools.lock.toml').open('rb') as stream:
        compiler = tomllib.load(stream)['msvc90']
    tool_paths = compiler['components']
    import os
    selection = Path(os.environ.get('TH11_MSVC90_ROOT', str(ROOT / compiler['selection'])))
    observed = {name: digest(selection / row['path']) for name, row in tool_paths.items()}
    expected = {name: row['sha256'] for name, row in tool_paths.items()}
    if observed != expected:
        raise ValueError('toolchain component identity mismatch')
    return {'schema_version': 1, 'target_sha256': target, 'context': context,
            'source_sha256': {name: digest(ROOT / name) for name in context['inputs']},
            'tools_lock_sha256': digest(ROOT / 'config/tools.lock.toml'),
            'tools_sha256': observed, 'object_sha256': digest(ROOT / context['object'])}


def verify_receipt(unit: dict) -> None:
    expected = receipt(unit)
    path = receipt_path(ROOT / expected['context']['object'])
    if json.loads(path.read_text()) != expected:
        raise ValueError('stale artifact receipt: cold-build the canonical unit again')
