# TH11 title menu lifecycle — full source recovered (2026-11-12)

The original title/menu controller comprises a large 0x5B94-byte storage
owner and five functions covering construction, resource initialization,
full teardown, worker startup, and singleton deletion. The entire source
for these paths is now maintained in `TitleMenuLifecycle.cpp`, rather than
simulating the small callback entries without their owning state.

| Original owner | Original bytes | VC9 normal-COFF diagnostic | Result |
| --- | ---: | ---: | --- |
| `0x439420` storage construction | 201 | 205 | source-present, not exact |
| `0x4395A0` chain/resource setup | 275 | 262 | source-present, not exact |
| `0x4396C0` full destruction | 561 | 584 | source-present, not exact |
| `0x439900` worker startup | 106 | 107 | source-present, not exact |
| `0x439970` singleton release | 28 | 29 | source-present, not exact |

The constructor initializes the three early selection blocks and replay
selection with matching `current=0`, `count=999`, wrap and exclusion fields,
constructs the embedded worker, then zeroes the complete object and publishes
its singleton. The resource setup allocates chain nodes with update priority
6 and draw priority 54; it uses the real `AcquireAnimationDocument` call for
slots `0x18` and `0x19`, records each pointer, and logs failure without
undoing earlier successful operations. The two native failure returns are
reconstructed separately. Startup allocates the owner and launches the
`0x4394F0` background worker; the distinct success and startup status globals
at `0x4C2EF4` and `0x4C3E00` are not conflated.

The 561-byte destructor is a substantial owner: it stops the worker and
unregisters both chain nodes under conditional runtime locking, destroys
renderer animation documents **24 and 25** (the same slots acquired during
setup), destroys/free's up to 100 record objects, finds the tracked VM in two
renderer lists and marks its related VM nodes, frees an optional data blob,
clears the singleton, and closes the worker lifetime. The original span has
558 directly analyzed instruction bytes plus an internal 3-byte `LEA ECX,[ECX]`
NOP at `0x43983D..0x43983F`; direct Ghidra reviewed that gap separately,
so it has not been silently cropped.

These functions compile with pinned VC9 RTM using natural C++ and the real
existing threading, chain, renderer, document, and VM view APIs. Size-only
comparisons against the original are *diagnostics*: private register scheduling,
exception frames and source-TU/LTCG ownership remain open; there is no new
strict exact claim from this five-function batch. The title-thread callback at
`0x4394F0`, the draw callback at `0x439F20`, and the 429-byte record destructor
at `0x435C50` are distinct follow-up owners.
