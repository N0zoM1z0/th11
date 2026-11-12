#!/usr/bin/env python3
"""Scoped VC9 /GL screenshot queue/copy experiment; no full replay or exact claim.

Run with: scripts/repo-python scripts/probe-screenshot-flush.py
The report includes complete primary-TU MAP/PDB owners and byte comparisons
excluding *only* independently decoded address fields. It intentionally does
not edit targets, source code, or any acceptance ledger.
"""
from pathlib import Path
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from ltcg_link import cold_link, tool_environment
from linked_image import PEImage, linked_functions, linked_code_fields, map_publics

ROOT = Path(__file__).resolve().parents[1]
CASES = [
    ('only', 'AnimationScreenshotFlushOnlyProbe',
     ['probes/animation-screenshot-flush-only-context.cpp']),
    ('generic', 'AnimationScreenshotFlushProbe@12',
     ['probes/animation-screenshot-flush-context.cpp']),
    ('real-child', 'AnimationScreenshotFlushProbe@12',
     ['probes/animation-screenshot-flush-context.cpp', 'src/AnimationSurfaceCopy.cpp']),
    ('real-child-renderer', 'AnimationScreenshotFlushProbe@12',
     ['probes/animation-screenshot-flush-context.cpp', 'src/AnimationSurfaceCopy.cpp',
      'src/RenderBatch.cpp']),
]
TARGETS = [
    ('?FlushScreenshotResources@@YGXXZ', 0x00445460, 48),
    ('?CopyAnimationTextureSurface@@YGXPAURenderBatchView@@PBUAnimationSurfaceCopyView@@@Z',
     0x00454EC0, 313),
]


def main():
    linker, env = tool_environment()
    native = PEImage(ROOT / 'resources/th11.exe')
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    for label, entry, support in CASES:
        out = ROOT / ('build/probe-ltcg/ScreenshotFlushScoped-' + label)
        cold_link(ROOT / 'src/AnimationSurfaceDispatch.cpp', out, entry, linker, env,
                  profile=['/TP', '/O2', '/Ob1', '/Gy', '/MT', '/Isrc', '/EHsc', '/GL'],
                  support_sources=[ROOT / p for p in support])
        img = PEImage(out / 'source.exe')
        owners = linked_functions(out / 'source.exe', out / 'source.map', out / 'source.pdb')['functions']
        pubs = map_publics(out / 'source.map')
        for name, addr, size in TARGETS:
            found = [f for f in owners if f['symbol'] == name]
            if not found:
                continue
            f = found[0]
            start = int(f['address'], 0)
            code = img.read_address(start, f['size'])
            orig = native.read_address(addr, size)
            fields = linked_code_fields(img, pubs, start, len(code))['fields']
            masked = {i for field in fields
                      for i in range(field['offset'], field['offset'] + field['width'])}
            mismatches = [i for i, (x, y) in enumerate(zip(code, orig))
                          if i not in masked and x != y]
            ops = list(md.disasm(code, start))
            print('%s %s: compiled %d/native %d bytes; %d instructions, '
                  '%d calls, %d fields, %d positional nonfield differences; first=%s'
                  % (label, name, len(code), size, len(ops),
                     sum(op.mnemonic == 'call' for op in ops), len(fields),
                     len(mismatches), [hex(i) for i in mismatches[:12]]), flush=True)


if __name__ == '__main__':
    main()
