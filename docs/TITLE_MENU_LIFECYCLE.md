# TH11 title menu lifecycle — full source recovered (2026-11-12)

The original title/menu controller comprises a large 0x5B94-byte storage
owner and five functions covering construction, resource initialization,
full teardown, worker startup, and singleton deletion. The entire source
for these paths is now maintained in `TitleMenuLifecycle.cpp`, rather than
simulating the small callback entries without their owning state.

| Original owner | Original bytes | VC9 normal-COFF diagnostic | Result |
| --- | ---: | ---: | --- |
| `0x439420` storage construction | 201 | 205 normal COFF / 201 linked | **201/201 strict exact** |
| `0x4395A0` chain/resource setup | 275 | 262 isolated / 275 linked | **275/275 strict exact** |
| `0x4396C0` full destruction | 561 | 584 | source-present, not exact |
| `0x439900` worker startup | 106 | 107 normal COFF / 106 linked | **106/106 strict exact** |
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
existing threading, chain, renderer, document, and VM view APIs. The storage
constructor has since achieved independent strict cold exactness under a
narrow title-lifecycle `/GL` context. The remaining resource, destructor,
startup and delete owner size comparisons are only diagnostics; private
register scheduling, EH and source-TU/LTCG ownership remain open. The title-thread callback at
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
  isolated VC9 generates a shorter 5-byte tail jump, but its actual
  title menu `/GL` caller now restores the 7-byte private register bridge. Both reference/padding
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

## Exact 201-byte title storage constructor (2026-11-12)

The native `0x439420..0x4394E8` owner returns its ESI object in EAX after
39 instructions. Its order is unusual but real: publish the title vtable,
initialize three early selection blocks and the replay selection, construct
the embedded `ThreadInf` state, **then clear all 0x5B94 bytes with `memset`**,
set flags bit 1, and publish the global title singleton. The maintained
natural constructor preserves this ordering without padding, machine-code
splices or fake branches.

The dynamic entry `probes/title-menu-lifecycle-context.cpp` is sufficient to
keep a complete unique public 201-byte primary-TU PDB/MAP symbol under pinned
VC9 `/GL`; its single call and all 39 instructions match. Four independently
reviewed address fields (title vtable `0x49637C`, worker vtable `0x4978D0`,
`memset` at `0x46A720`, singleton `0x4A8ECC`) fully replay in the canonical
`title-menu-constructor` unit with **zero byte differences** and cold current
source/tool/producer receipts. The old isolated normal COFF compiled to 205
bytes because it did not recover the original private ESI receiver. No
original production TU partition is inferred from this linked family.

Additional scoped `/GL` diagnostic comparisons for the neighboring full
resource initializer/destructor/startup still show **268/275, 569/561 and
107/106** bytes in the selected real resource/chain context; their full
bodies remain non-exact and are not cropped to improve metrics.

## Exact 7-byte title draw callback (2026-11-12)

The registered draw callback at `0x439F20..0x439F26` is separate from the
140-byte `DrawMenuController` switch boss. The original has `MOV EAX,ECX`
followed by a full `JMP 0x439E80`, even though ordinary isolated VC9 source
optimizes the C++ forwarder to only the 5-byte tail jump. Providing the real
`MenuControllerDraw.cpp` and `TitleMenuLifecycle.cpp` as genuine `/GL` support
restores the private EAX receiver and the **complete 7-byte owner** without
changing the source or ABI declarations.

The scoped producer `probes/title-menu-draw-callback-context.cpp` yields a
single public primary-TU 7-byte contribution and native 2/2 instructions.
The only original address field, a `REL32` at +3 to the reviewed complete
140-byte draw dispatcher `0x439E80`, is independently bound in the canonical
`menu-controller-draw-callback` strict linked Oracle. Cold VC9 comparison
with current source/tool/PE/MAP/PDB receipts reports **zero differences**.
The 140-byte dispatcher itself remains source-present but non-exact.

## 561-byte destructor: original lock pairing and resource-slot reload (2026-11-12)

The `0x4396C0..0x4398F0` destructor is now substantially closer in a bounded
VC9 `/GL` diagnostic family. Its original code enters and leaves each chain
critical section directly, without recording a separate exception-unwind
state for each lock. The maintained `RemoveTitleMenuChain` now uses these
actual paired operations, retaining the real manager pointer before locking.
For each renderer document slot (24 and 25), it preserves the pointer to the
slot, calls `DestroyAnimationDocument` with the loaded document, **reloads the
slot for scalar deletion**, then clears the slot. These are real lifetime and
side-effect observations, not artificial code-generation padding.

A full original-PE comparison with the genuine destructor caller, real
`AnimationDocumentDestructor.cpp` and `FrameRuntime.cpp` producers gives:

| Source variant | Linked bytes / native | Instructions / native | Calls / native | Return | Non-address differences |
| --- | --- | --- | --- | --- | --- |
| Earlier scoped locks, cached document | 569 / 561 | 154 / 155 | 15 / 15 | `RET` | 385 |
| Direct lock pairs, cached document | 561 / 561 | 150 / 155 | 15 / 15 | `RET 4` | 226 |
| Direct lock pairs, **slot reload** | **561 / 561** | **152 / 155** | **15 / 15** | **`RET 4`** | **231** |

The retained detailed probe is `.analysis/probe-title-menu-dtor-variants-20261112.py`
with its accompanying log; the typed producer is
`probes/title-menu-destructor-context.cpp`. The extra five positional
non-address differences in the final variant are not ignored: the slot reload
faithfully retains native document ownership even as it changes register
allocation. Neither of these candidates is exact. The native still saves a
separate ECX and uses different first-chain bit-mask/register lifetimes.

The only directly affected canonical units using `TitleMenuLifecycle.cpp`
were cold-replayed after the change: `title-menu-constructor` (201 bytes),
`title-menu-thread` (165 bytes), and `menu-controller-draw-callback` (7 bytes).
All three retained strict full-byte and field equality. See private scoped
verification `.analysis/title-menu-destructor-affected-exact-20261112.log`.

## Exact 106-byte menu worker startup (2026-11-12)

The full native `0x439900..0x439969` owner allocates a 0x5B94-byte title
object, calls the exact 201-byte title constructor, resets the associated
worker, stores the thread entry callback at `0x4394F0`, launches
`_beginthreadex` and records its thread handle. An isolated normal-COFF
producer had 107 bytes, obscuring the private worker register lifetime.

The existing natural `StartTitleMenu` source requires no expression hacks.
When the real `FrameRuntime.cpp` stop-worker producer participates in a
bounded genuine VC9 `/GL` context, it recovers the native **106/106 bytes,
32/32 instructions, four calls and plain `RET`**. The strict canonical
`start-title-menu` unit verifies one unique primary-TU MAP/PDB code owner
and seven fully reviewed fields: allocator and constructor calls, startup
state, worker-stop call, the two identical callback-address publications,
and `_beginthreadex`. Every field is replayed against the hash-pinned
original and the complete code has **zero differences**. The original TU
partition and runtime closure remain separate, unknown questions.

A separate experiment using a real typed `ThreadInf` placement constructor
in the title constructor changed the latter to 182 bytes and therefore
was **not** adopted: the existing 201/201 exact constructor remains intact.
Typed worker destruction without that constructor also worsened the nonexact
561-byte destructor. The untouched title destructor retains the improved
561/561-length, 15-call, `RET 4` diagnostic from the explicit lock-and-slot
restoration, without claiming byte exactness.

## Exact 275-byte title resource initializer boss (2026-11-12)

The complete `0x4395A0..0x4396B2` function constructs two 36-byte chain
elements, registers priority-6 update and priority-54 draw callbacks, then
loads animation documents in slots 24 and 25. Both distinct error returns
log the real format; success sets the selection flag and resource state.
It is a substantive seven-call boss, not a neighboring small callback.

Standalone VC9 codegen was 262 bytes. Linking only real chain and document
functions produced a 268-byte owner with different register scheduling. The
crucial missing context was a *separate generic-priority and dynamic-slot
caller*. The retained compile-only
`probes/title-menu-resources-generic-context.cpp` calls both real registration
functions with independent priority arguments and also acquires an arbitrary
animation document. Together with genuine `ChainRegistration.cpp`,
`AnimationDocument.cpp`, `LogBuffer.cpp`, and the original document-load
producer, this constrains VC9 `/GL` to the native **ESI element, EBX chain
priority, EBX pathname and ECX animation slot** convention. No artificial
register assignments, inline assembly or target byte splices are used.

Four bounded producer variants established that chain+document support alone
leaves four non-field bytes; adding only the real logger or only the generic
document caller leaves two; **both** together reproduce the original
**275/275 bytes, 90/90 instructions and 7/7 calls**.
The canonical `initialize-title-menu-resources` strict linked Oracle has a
unique public primary-TU MAP/PDB contribution and **15** independently
reviewed bindings: singleton, two allocations, both registered callbacks,
two chain operations, two animation names, two document loads, error format,
log root and logger call, plus success state. All 15 are replayed against the
hash-pinned original with **zero differences**. Original TU ownership and
whole-game runtime equivalence remain open.
