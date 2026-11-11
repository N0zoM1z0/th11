#!/usr/bin/env python3
"""Focused pinned VC9 /GL comparison for TH11's 934-byte ECL browser.

Never replays the full exact cohort. Uses real independent, dynamic-argument
callers for both the runtime mode setter and the worker thread starter;
otherwise LTCG specializes the literal mode 3 and callback address in the
large browser, silently losing native EAX/ECX/EDI argument instructions.
This is a diagnostic for a NON-EXACT big function, not an acceptance tool.
"""
from pathlib import Path
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from ltcg_link import cold_link, tool_environment
from linked_image import PEImage, linked_functions, linked_code_fields, map_publics

ROOT = Path(__file__).resolve().parent.parent
SUPPORT = [
    'probes/secondary-program-browser-context.cpp',
    'src/SecondaryProgramManagerFactory.cpp',
    'src/SecondaryScript.cpp',
    'src/ConfigurationMenuSelection.cpp',
    'src/SecondaryGameplaySupport.cpp',
    'src/SecondaryEclDebug.cpp',
    'probes/secondary-gameplay-mode-dynamic-context.cpp',
    'src/SecondaryDebugThread.cpp',
    'src/FrameRuntime.cpp',
    'probes/secondary-debug-thread-context.cpp',
]
ORIGINAL = [
    ('UpdateSecondaryProgramBrowser', 0x438AC0, 934),
    ('ClearSecondaryProgramBrowserFiles', 0x438570, 83),
    ('StartSecondaryDebugThread', 0x4594A0, 56),
    ('StopRuntimeThread', 0x459430, 101),
    ('SetSecondaryGameplayRuntimeMode', 0x40DEA0, 30),
]


def main():
    output = ROOT / 'build/probe-ltcg/SecondaryBrowserModeThreadTracked'
    linker, env = tool_environment()
    cold_link(
        ROOT / 'src/SecondaryProgramBrowser.cpp', output,
        'SecondaryProgramBrowserProbe@4', linker, env,
        profile=['/TP', '/O2', '/Ob1', '/Gy', '/MT', '/Isrc', '/EHsc', '/GL'],
        support_sources=[ROOT / x for x in SUPPORT],
    )
    image = PEImage(output / 'source.exe')
    native = PEImage(ROOT / 'resources/th11.exe')
    functions = linked_functions(output / 'source.exe', output / 'source.map', output / 'source.pdb')['functions']
    publics = map_publics(output / 'source.map')
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    for name, original_addr, size in ORIGINAL:
        found = [f for f in functions if f['symbol'].startswith('?' + name + '@@')]
        if not found:
            print(name, 'not linked as an independent owner')
            continue
        f = found[0]
        a = int(f['address'], 0)
        candidate = image.read_address(a, f['size'])
        original = native.read_address(original_addr, size)
        fields = linked_code_fields(image, publics, a, len(candidate))['fields']
        mask = set()
        for field in fields:
            mask.update(range(field['offset'], field['offset'] + field['width']))
        diffs = [i for i, (x, y) in enumerate(zip(candidate, original))
                 if x != y and i not in mask]
        ops = list(md.disasm(candidate, a))
        print('%-40s %3d/%3d bytes %3d instructions %2d calls %2d fields '
              '%3d positional nonfield differences, first=%s' % (
                name, len(candidate), size, len(ops),
                sum(o.mnemonic == 'call' for o in ops), len(fields),
                len(diffs), [hex(i) for i in diffs[:12]]), flush=True)
        if name == 'UpdateSecondaryProgramBrowser':
            assert len(candidate) == 928 and len(ops) == 262
            assert sum(x.mnemonic == 'call' for x in ops) == 22
            assert len(diffs) == 173
        else:
            assert len(candidate) == size and not diffs, 'Affected exact source regressed: ' + name


if __name__ == '__main__':
    main()
