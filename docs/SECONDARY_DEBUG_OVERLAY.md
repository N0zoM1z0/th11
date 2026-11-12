# TH11 auxiliary debug display and VM overlay (2026-11-12)

The previously unsourced **761-byte owner at 0x40DAC0..0x40DDB8** now has
natural C++ implementation in `src/SecondaryDebugOverlay.cpp/.hpp`. Its real
session storage (`SecondaryEclDebugView`) has the state selector at `+0x30`,
filename array and count at `+0x34/+0x38`, selection records beginning at
`+0x3C`, source position floats at `+0x6FC/+0x700`, render mode at `+0x70C`,
conditional VM draw at `+0x2C8`, and three destination coordinate floats at
`+0x6A4`. The ASCII destination is the existing `0x4A8D58` manager, the
renderer is `0x4C3268`, and the active script list can be reached through
the independently observed master root at `0x4A8D7C`.

The complete original owner prints a `SprtView` label and dispatches four
modes. Mode 1 prints selected file status and ECL name/index, a quit entry and
selection marker, restores ASCII color, then optionally draws the VM. Mode 2
colors and prints an in-progress loading filename. Mode 3 displays two
float-to-integer position values, restores color and conditionally draws the
VM. Mode 4 displays the enemy count. The renderer path uses the actual
`DrawUnrotatedAnimationVm` source and the ASCII path uses the actual
`AsciiManagerPrintSmall` source, including source-level floating coordinates.
No defensive bounds checks have been added to the original unchecked
filename or script selections.

The complete native Ghidra owner contains an indirect four-mode switch.
Its four apparent foreign *interior* references are all DATA cells in the
separate **16-byte jump table at 0x40DDBC..0x40DDCB**, with native targets
`0x40DB5E`, `0x40DAFC`, `0x40DCF5` and `0x40DCB1`. They are not separate
entry functions. The code body ends at `0x40DDB8`; alignment and table bytes
remain separately accounted rather than expanding or truncating that bound.
The **10-byte** adjacent callback `0x40DDE0..0x40DDE9` preserves ESI, calls
the 761-byte owner and returns. Both boundaries were reviewed with the
hash-pinned original mapped executable and direct Ghidra instructions.

Pinned VC9 normal-COFF compiles the natural source to **644/761 bytes** and
its callback to 5/10. A bounded `/GL` primary source with a true dynamic
callback, the actual debug update producer and real ASCII/renderer producers
yields candidates ranging **620–720/761 bytes**; the native full private ESI
receiver and floating layout are not yet reproduced. The callback remains a
five-byte tail jump instead of the original ten-byte save/call/restore.
These are substantial maintained-source owner recoveries, **not byte-exact
claims**; no masked bytes or fake assembly have been credited. Reproduce via
`probes/secondary-debug-overlay-context.cpp` and
`.analysis/probe-secondary-debug-overlay-20261112.py`.
