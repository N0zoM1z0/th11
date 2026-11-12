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

## Background thread and draw callback now source-present

Direct native instruction review also recovered two independently bounded
function-pointer owners Ghidra missed in its initial inventory:

- `0x4394F0..0x439594` (165 bytes), `TitleMenuThread`:
  `InitializeTitleMenuResources` runs before a load-progress wait on the
  singleton at `0x4A8E98`, whose `+0x474` counter must reach 180 unless
  runtime flag `0x180` interrupts waiting. `Sleep(16)` is used between
  progress reads. Renderer slot 1 is then destroyed, the menu update-chain
  flag is enabled, and the thread state is published. Failure builds
  flags from the runtime's `0x2000` state and exits early. The verified
  zero-input `__cdecl` callback is now **165/165 strict exact** in a real
  title/document producer context, with all 12 native address/call fields
  replayed (details below).
- `0x439F20..0x439F26` (7 bytes), `MenuControllerDrawCallback`:
  the original is `MOV EAX,ECX` then a tail `JMP` to the 140-byte draw
  dispatcher at `0x439E80`. Its body is present as a natural C++ forwarder;
  isolated VC9 generates a shorter 5-byte tail jump, so the private
  register bridge and linked owner are still open. Both reference/padding
  spans were directly reviewed, and the title resource setup uses this
  callback rather than inventing a no-op.

The 429-byte replay-record owner called from title destruction is now in
`TitleMenuRecord.cpp`; it is source-present and independently matched in
size, not strict codegen. The separate 165-byte thread has now become an independently exact unit.

## Exact 165-byte title worker callback (2026-11-12)

Ghidra's full mapped-PE review at `0x4394F0..0x439594` shows a callback
which does **not** consume its `_beginthreadex` context parameter and has two
ordinary `RET` exits. Declaring it as `unsigned int __cdecl TitleMenuThread()`
reflects that real ABI, while the title launcher retains the required explicit
cast to the Win32 thread callback pointer. In normal COFF this restores most
of the original failure and readiness code, but leaves a 163-byte non-exact
owner.

The remaining real semantic difference is at the loader-only animation slot
(`g_RenderBatch` + `0x4350C4`, slot index 1). The native releases the document
using the original file pointer in EDI, **then reloads the slot pointer** for
`operator delete` before clearing the slot. The earlier source reused the
cached file pointer and compiled differently. Expressing the slot as
`AnimationDocumentView **slot`, then `DestroyAnimationDocument(file)`,
`operator delete(*slot)`, and finally `*slot = 0` recovers its full native
ESI/EDI register lifetimes and preserves any intervening slot mutation.

A narrow genuine producer comprises `TitleMenuThread.cpp` as primary,
`TitleMenuLifecycle.cpp` and `AnimationDocumentDestructor.cpp` as real `/GL`
support, and the simple dynamic caller
`probes/title-menu-thread-context.cpp`. Pinned VC9 RTM creates a single
complete public 165-byte PDB/MAP owner, **47/47 instructions, four calls,
two RETs, 12 independently reviewed DIR32/REL32 target fields, and zero
byte differences**. All 12 fields cover the real resource initializer,
runtime flag offset `+0x590`, startup error state, loading singleton, Sleep
import, renderer, document destructor, scalar delete, title singleton and
thread-active global. Canonical `title-menu-thread` cold replay verifies its
current source/header/tool/producer/PE/MAP/PDB receipt. Original production
TU ownership remains open, but this complete owner is strictly exact.
