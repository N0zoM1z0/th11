#!/usr/bin/env python3
"""Focused real-parent VC9 /GL copy-boss comparison, NOT strict acceptance.

This script tests just the 313-byte TH11 animation backbuffer-to-texture
copy and its native four-record caller. It does not run a full cold replay.
"""
from pathlib import Path
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from ltcg_link import cold_link, tool_environment
from linked_image import PEImage, linked_functions, linked_code_fields, map_publics

ROOT = Path(__file__).resolve().parent.parent
SUPPORT = [
    'src/AnimationSurfaceDispatch.cpp',
    'src/RenderBatch.cpp',
    'src/ScreenshotHotkey.cpp',
    'probes/animation-surface-copy-real-context.cpp',
]
FUNCTIONS = [
    ('CopyAnimationTextureSurface', 0x454EC0, 313),
    ('FlushScreenshotResources', 0x445460, 48),
]


def main():
    output = ROOT / 'build/probe-ltcg/AnimationSurfaceCopyTracked'
    linker, env = tool_environment()
    cold_link(
        ROOT / 'src/AnimationSurfaceCopy.cpp', output,
        'AnimationSurfaceCopyProbe@0', linker, env,
        profile=['/TP', '/O2', '/Ob1', '/Gy', '/MT', '/Isrc', '/EHsc', '/GL'],
        support_sources=[ROOT / s for s in SUPPORT],
    )
    linked = PEImage(output / 'source.exe')
    original = PEImage(ROOT / 'resources/th11.exe')
    functions = linked_functions(output / 'source.exe', output / 'source.map', output / 'source.pdb')['functions']
    publics = map_publics(output / 'source.map')
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    for symbol, address, expected_size in FUNCTIONS:
        f = next(x for x in functions if x['symbol'].startswith('?' + symbol + '@@'))
        a = int(f['address'], 0)
        b = linked.read_address(a, f['size'])
        native = original.read_address(address, expected_size)
        fields = linked_code_fields(linked, publics, a, len(b))['fields']
        masked = set()
        for field in fields:
            masked.update(range(field['offset'], field['offset'] + field['width']))
        differences = [i for i, (x, y) in enumerate(zip(b, native))
                       if x != y and i not in masked]
        ins = list(md.disasm(b, a))
        print('%s: source %s / native %s bytes, %s instructions, %s calls, '
              '%s fields, %s positional nonfield differences; first=%s'
              % (symbol, len(b), expected_size, len(ins),
                 sum(i.mnemonic == 'call' for i in ins), len(fields),
                 len(differences), [hex(i) for i in differences[:15]]), flush=True)
        if symbol == 'CopyAnimationTextureSurface':
            assert len(b) == 311
            assert len(ins) == 112
            assert len(differences) == 46
            assert sum(i.mnemonic == 'call' for i in ins) == 8


if __name__ == '__main__':
    main()
