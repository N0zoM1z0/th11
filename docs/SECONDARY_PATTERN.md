# Projectile spawn and integer-pair source

Twelve new complete mappings cover1,087 target bytes, including the853-byte spawn
owner. Seventeen new boundary/origin reviews remain indeterminate; eleven omitted
candidates are added. All evidence comes from the hash-pinned Japanese1.00a image,
direct local full mapped-text attested Ghidra and independent complete PE decoding.

## Corrected shared projectile layout

The old parameters64 interpretation was a real source-layout error. Its typedef
assertion only checked the layout we defined; it did not establish native layout.
Four independent target paths agree on the corrected record:

| Target observation | Independently calculated record field |
| --- | --- |
| Spawn433FE7 stores the descriptor at record+68 | parameters68 |
| Damage434835/434843 biases EBX by4C; subsequent descriptor loads use+1C | 4C+1C=68 |
| Update43444A/434455 biases ESI by20;43446D loads+48 | 20+48=68 |
| Gravity4352CF loads float64 and adds velocity.x20 | acceleration64 |

The record remains6C bytes. Existing target54 is still owner96C+54=global9C0.
The shared source now places acceleration at64 and parameters at68, correcting
both damage and update member reads. Gravity and emitter callbacks need no
alternate layout to explain those fields. Original class/header identity is open.

Spawn independently exposes signed damage2, offsetX/Y4/8, width/height0C/10,
angle14, speed18, signed index1C, kind1D, signed initialScript1E, replacement
script20, signed sound22 and callback24. Existing update28 and hit30 remain fixed.
The spawn callback receives owner ECX, record EDX and one stack context word;
its nominal word type and original callback identity remain provisional.

## Complete spawn and emitter views

Spawn433F90 owns853 bytes/258 instructions, including native alignment, two RET8
paths, eight direct calls and one callback. Native inputs are EAX descriptor,
ECX position and stack owner/context. The free source declaration describes those
four actual inputs; its current LTCG register allocation remains different.

The complete source preserves the kind4 slot gate and first-free scan across256
records. It initializes only state, descriptor, damage and the real timer fields;
there is no whole-record clear. Timer flags preserve conditional defaults before
the final0/0/-1 reset. Motion flags and other old record fields remain available.
Index0 copies the supplied position; other signed indices read E4-stride integer
emitter coordinates and multiply by double1/128, setting Z to positive zero.

Ordered angle>=1000 with nonzero index draws twice from the real shared RNG.
Each unsigned draw forms float-rounded `(draw * 2^-31 - 1)`. Angle adds this value
times double(float pi)/12 to the emitter angle, rounds and normalizes. Speed adds
the second rounded value to itself and the configured speed. Ordered995..1000
uses the emitter angle; index0 and unordered angles use the supplied angle.
These are actual995/1000 thresholds, not negative sentinels. Native x87 stores
determine rounding boundaries rather than the decompiler's float casts.

Cartesian motion uses the actual polar helper and clears velocity Z. Radial
motion accumulates radius and normalizes speed+angle. Position then receives
configured offset minus velocity before script creation. The signed initial
script gets+5, layer16. The genuine inline two-list VM lookup clears a missing ID
and still dereferences the result; no null guard replaces that native behavior.
Rotation uses the raw configured angle. Auxiliary ID clears before callback24,
optional sound follows it, and the VM gets separate double x+32+192/y+16 offsets.

Emitter helpers establish owner75CC+index*E4 and ID at owner7620+index*E4. The
helper type is the observed coordinate prefix through ID54. Later complete render
and reset review establishes the containing record base7570, eight physical E4
records and end7C90; prefix75CC is record+5C. See
[SECONDARY_PATTERN_CALLBACKS](SECONDARY_PATTERN_CALLBACKS.md). Placing whole records
at75CC was the unproved assumption behind the earlier overlap restriction.

Initial source735/731 retained out-of-line emitter/VM helpers. Ordinary inline
physical accessors and the native two-list lookup produce824/745 in that earlier context. The later real-caller source849/733 also
snapshots the float3 before publication and uses the observed unsigned-to-float
x87 conversion. Full
private ABI/frame, unsigned-conversion constant storage, comparisons and register
scheduling remain open. This complete diagnostic grants no exact spawn credit.

## Integer-pair source and exact owners

The nine integer helpers preserve wrapped32-bit addition/subtraction/products,
signed division by100 and alias-sensitive read/publication order. Their explicit
output views do not assert an original constructor or aggregate-return ABI.

| Owner | Address | Target bytes | Current result |
| --- | --- | ---: | --- |
| SpawnSecondaryProjectile | 433F90 | 853 | Real Sound846/733; real-only independent RNG856/690; non-exact |
| GetSecondaryEmitter | 433F30 | 14 | Exact |
| CopySecondaryEmitterId | 433F40 | 16 | Exact |
| ConvertSecondaryIntegerPair | 433DB0 | 26 | Exact |
| AddAssignSecondaryIntegerPair | 433DD0 | 23 | Exact (inline member, movement TU) |
| SecondaryIntegerPairNonzero | 433DF0 | 20 | Exact |
| SubtractSecondaryIntegerPair | 433E10 | 18 | Exact (inline member, movement TU) |
| ScaleSecondaryIntegerPair | 433E30 | 19 | Exact (inline member, movement TU) |
| DivideSecondaryIntegerPair100 | 433E50 | 51 | Exact (inline member, movement TU) |
| SetSecondaryIntegerPair | 433E90 | 6 | Exact |
| AddSecondaryIntegerPair | 433EA0 | 18 | Exact (inline member, movement TU) |
| AssignSecondaryIntegerPair | 433F10 | 23 | Exact (inline member, movement TU) |

**2026-11-14: the integer-pair bodies are dead inline copies.** None of the nine
pair functions at 433DB0..433F26 has a direct call or a stored address. An
in-class inline pair type whose every use is inlined leaves dead COMDAT copies
(the link keeps unreferenced COMDATs), and those copies reproduce **all nine**
native bodies at native sizes and in native order: constructor `(x, y)`,
`ToFloat` (x/128, y/128, 0), `operator+=` returning a copy, nonzero test,
`operator-`, `operator*(int)`, divide-by-100, `operator+` and `operator=`
returning a copy. Eight are raw-identical; `ToFloat` differs only in its
double-constant address. The six non-exact rows above are fixed by this form;
the free-function output-pointer form cannot choose the dead copies' register
conventions. Probe: `.analysis/IntegerPair-inline-20261114.cpp`.

`SecondaryIntegerPair` now has inline member operators (`+=`, `-`, `*`, `/`,
`+`; no constructors, so it stays an aggregate), and the emitter's four
coordinates are two pairs, `pending` and `position`. The emitter blend in
`UpdateSecondaryPlayerMovement` is written as native computes it inline:

```cpp
SecondaryIntegerPair delta = (emitter->pending - emitter->position) * owner->blend / 100;
if (delta.x || delta.y) emitter->position += delta;
else emitter->position = emitter->pending;
```

The movement view's fixed-point player position is also a pair
(`fixedPosition`), so the emitter target is `owner->fixedPosition + *offset`.
That TU then emits the dead copies itself. `-`, `*`, `/` (LTCG specializes the
divisor to 100) and `+` are exact units in the SecondaryPlayerMovement family.
The earlier free-function versions of these four were removed.

`+=` (433DD0) is exact too. Its copy keeps loads and adds in order only when
some use passes operands that may alias. The native use is the per-frame
player step: the movement view's `stepX/stepY` are a pair `step` at +8B8, and
`owner->fixedPosition += owner->step` (same base pointer) reproduces the copy
and brings the movement owner closer (83 vs 47 aligned instructions). With a
local operand VC9 schedules the second load early. The copy-returning
`operator=` (433F10) is also exact: the type declares a user copy-assignment
that returns a copy, and the movement TU's pair assignments emit its dead
copy. All 86 units in the affected families stayed exact with it. All seven
pair operators besides the three probe-proved helpers (`ToFloat`, nonzero,
constructor) are now exact from their real owner; the movement owner is
2920/2896 bytes with 85 aligned instructions.

Tracked-target435030/73 also closes after the two length helpers explicitly call
the double sqrt API and narrow its result to float. Their own33/34-byte bodies
remain unchanged/non-exact, while VC9 restores the tracked callback's native
8-byte-aligned frame. This is a measured TU effect; it proves neither the original
TU partition nor the callback's original nominal third-slot type. Explicit double
fabs/atan2 variants do not close the callback. No local double, alignment annotation,
volatile access, fabricated ABI or padding is added.

Two new independently decoded address fields require review: conversion offset4
binds double1/128 at497ED0; tracking offset47 binds double224 at497F70. Public
compiler __real symbols and the actual candidate/target constant bytes agree.
The four other new exact owners have no address fields.

## Larger boundary and origin review

Controller431070 owns2,888 bytes:2,867 code, one NOP and five switch entries at
431BA4..431BB7. All695 instructions/two returns are independently decoded.
Every apparent foreign DATA reference into a case comes from this owned table;
all five targets are actual instruction starts. The old2,867 count omitted21 bytes.
Complete controller source now exists at2800/2285 and remains non-exact; aim101,
inverse aim106 and refresh54 are exact. Real callbacks431C50/7 and431C60/10
are independently reviewed. See [SECONDARY_PLAYER_CONTROLLER](SECONDARY_PLAYER_CONTROLLER.md).

Other complete new reviews cover callback433690/12, orbit4336A0/255,
aim4337B0/495, spawn433F90/853 and replay initialization4356E0/1380. The last
owner is replay-state setup, not collision geometry: nine allocations, actual
24/70/90-byte structures, snapshots, RNG restoration and callback installation.
All406 instructions/four returns are covered. The eleven omitted integer/emitter
owners use explicit native spans, full independent branches/returns and real
neighbor bookends. The DATA reference to433F00 belongs to a separate identity
owner, not an interior entry into any selected helper. No source/credit is added
for that unresolved identity/constructor hypothesis or surrounding padding.

All17 origins remain indeterminate. Specific game, replay, vector, timer or engine
roles alone do not prove original authorship or reused-library identity.

## Earlier checkpoint cold proof

Six new owners pass155 bytes/two fields; seven affected old owners pass678 bytes/
15 fields. All13 selected cold replays pass833 bytes/17 fields, with unique complete
public primary-TU PDB ownership, current source/header/support/tool/producer/
PE/MAP/PDB receipts, exhaustive fields and full zero-byte comparison. Source/probe
hashes remain frozen throughout. Actual old receipt dependencies select only
SecondaryDamageSupport and SecondaryProjectileMath; the other new families are
SecondaryIntegerPair and SecondaryPattern.

Fresh corrected-layout damage/collision diagnostics remain1513/1414 and228/211;
update/homing remain908/765 and493/401. Equal diagnostic totals do not erase the
semantic correction from parameter loads64 to68. The spawn call audit agrees
on all eight direct calls and the one callback; source merges its return paths
into one RET8 while target has two. No exact or runtime spawn claim follows.
No full cohort or routine behavioral suite is run.

Canonical contexts are declared in `config/match-units.toml`.

Later full caller/callback source, period/phase fields, containing emitter array,
44 false-reference classifications and that checkpoint's18-unit cold proof are recorded
in [SECONDARY_PATTERN_CALLBACKS](SECONDARY_PATTERN_CALLBACKS.md).

## Complete 1380-byte replay initialization boss (2026-11-12)

`src/SecondaryReplayInitialization.cpp` now maintains the complete
`InitializeSecondaryReplay` at `0x4356E0..0x435C43` in natural C++.
The native Japanese 1.00a owner spans **1380 bytes / 406 instructions**,
with **22 calls, four returns** and independent complete-flow review.
Its three native modes remain separate: fresh replay recording (0),
loading and restoring a replay (1), and a validating-only mode (2).

Mode 0 publishes the current replay root, constructs the observed 0x24
file header (magic/version), 0x70 replay header and one 0x90 stage record,
appends the real replay-write segment, snapshots the current selection,
score, difficulty, seed/call counter, lives, bonus and other replay-stage
fields, initializes all **20** recorded words with the native descending
`0x21522153` pattern, and registers three distinct chain elements with
the native recording, auxiliary and draw callbacks. Stage and stream
records are indexed with the existing stage selector, not silently clamped.

Mode 1 loads the replay, restores 60 bytes of snapshot data, copies the
stage stream bookkeeping, restores the captured RNG seed/counter and
selected gameplay globals (including capped power), registers the
playback/auxiliary/draw chains, and records the -1 playback stage sentinel.
Mode 2 calls the same separately observed 441-byte loader without creating
those three chains, reporting the native success/failure result. The
original code does not use synthetic resource failure cleanup; the source
preserves the observed unchecked allocation paths and unknown original
class ownership. `ReplayInitStateView` asserts the physical writer-compatible
0x14/0x18 header offsets, +0x40 streams, +0x9C last-stream tail and
+0x1D4 stage selector, rather than inventing a different replay object.

The following were the **initial** scoped links, before the mode-0 stage
cleanup correction documented below; they are historical diagnostics:

Three **scoped** pinned VC9 `/GL` PE/MAP/PDB links successfully compile
this full source: **1241/1380 bytes** independent,
**1229/1380** with real chain producer, and **1236/1380** with the real
chain and `ReplayWriter.cpp`. All retain the **22/22 original calls and
four returns**. The closest context has **371 versus 406 instructions**,
66 candidate linkage fields and 939 positional nonfield byte differences.
The remaining differences are real constructor inlining, register/stack
scheduling, source-known-global identity and callback/stream lifetimes;
no strict exact byte credit is taken. The complete source, not a passing
prefix, is the maintained starting point. Reproduce the local comparison
with `.analysis/probe-secondary-replay-init-20261112.py` and its log,
and the public independent caller
`probes/secondary-replay-initialization-context.cpp`. No full cold replay
was executed.

## Full 441-byte replay stage loader (2026-11-12)

The 1380-byte replay initializer now has a maintained source-level callee:
`LoadReplayStageData` at `0x436B60..0x436D18`, implemented in
`src/SecondaryReplayLoad.cpp`. Direct original PE/Ghidra review confirms the
complete **441-byte, 137-instruction** owner with **14 calls, one RET8**, three
incoming callsites, no foreign interior reference or owned table.

The loader copies the supplied filename into replay state +0x1DC. In disk
mode it formats `replay/%s`, checks existence, opens the shared read handle,
reads and validates the 0x24-byte `t11r`/version-4 file header, then reads
its packed payload and closes the shared file. When replay status bit 0x20
is set, it instead loads the resource data and uses the packed payload after
the inline header. The source allocates the header's stated unpacked size,
performs the two original scramble passes with key/step/block combinations
`0xAA/0xE1/0x800` and `0x3D/0x7A/0x40`, then invokes the existing archive
decompressor and stores the unpacked pointer at +0x18/+0x1C4. Starting from
the unpacked header +0x70, it walks at most six stages when the recorded
stage count reaches eight, indexing each by its signed 16-bit stage number
and computing its event start/end from the native +0x90 header and six-byte
event stride. Only a disk-allocated packed buffer is freed at exit.

The complete loader is now expressed as a native mixed-ABI owner in the
maintained `src/SecondaryReplayLoad.cpp`: ESI carries the shared-file path,
EDI carries the read allocation size, AL carries each cipher key, and EAX
carries the decompressor output size. The original 0x108-byte stack-cookie
frame and RET8 exit are preserved. The pinned VC9 RTM `/GL` primary-TU linked
owner reproduces **441/441 bytes, 137/137 instructions, 14/14 calls and one
RET8**. All 18 independently bound fields (security cookie, replay status,
format string, file/resource/read/close helpers, malloc/free, both scramblers,
decompressor and cookie check) replay with **zero full-byte differences**.
The canonical unit is `secondary-replay-load-stage-data`; its local strict
replay is separate from Factory oracle acceptance.

Reproduce with `.analysis/probe-secondary-replay-load-20261112.py` for the
rejected natural-C++ attempts, or `scripts/replay-exact-units.py --unit
secondary-replay-load-stage-data` for the current complete linked proof. Only
the loader and its probe are compiled for the canonical owner; unrelated
whole-program/cold replay is not run.

## Exact shared replay close helper and source-backed read helper (2026-11-12)

`src/SharedFileReadHelpers.cpp` now implements both complete read-path
helpers used by the 441-byte replay loader. `ReadSharedReplayBytes`
(`0x458980`, 69 bytes) checks the global shared handle, allocates a
caller-sized buffer, closes the handle if allocation fails, and otherwise
uses `ReadFile` to fill it. The native read size arrives in EDI through a
private calling convention. The first scoped VC9 `/GL` code was **69/69 bytes,
35 instructions and three calls**, but exchanged EBX and EDI for ten byte
differences. Eleven bounded natural source variants revealed that checking
`g_SharedFile == INVALID_HANDLE_VALUE` **before** copying the handle to a
local variable restores the original EBX file / EDI size allocation. The
revised owner now passes **69/69 strict exact bytes, 35/35 instructions and
all four independent DIR32/REL32 fields**: shared handle `0x4A34E0`,
`malloc` `0x460192`, `CloseHandle` IAT `0x48B0A0` and `ReadFile` IAT
`0x48B0A4`. Neither register was forced or explicitly named in C++.

`CloseSharedReplayReader` (`0x4589D0`, 49 bytes) closes that global handle
when valid and conditionally releases the input runtime's critical section,
then decrements its depth byte. Its complete independently reviewed native
owner has **49 bytes, 12 instructions, two Win32 calls** and six data/IAT
fields. A natural VC9 `/GL` public primary-TU PE/MAP/PDB link produces
**49/49 strict exact bytes**. All six DIR32 targets were independently
checked: shared handle `0x4A34E0`, `CloseHandle` IAT `0x48B0A0`, runtime
flags `0x4C3810`, critical section `0x4C3AC0`, `LeaveCriticalSection`
IAT `0x48B088` and depth byte `0x4C3BB2`. Complete cold replay has
`difference_count=0`, with current source/tool/producer receipts.

Canonical exact unit: `close-shared-replay-reader`;
`.analysis/close-shared-replay-strict-20261112.log`. Diagnostic comparisons
for both functions are in `.analysis/probe-shared-replay-helpers-20261112.py`
and its log, with an independent dynamic caller in
`probes/shared-file-read-helpers-context.cpp`. Only the 49-byte exact unit
was cold replayed.

## Exact 93-byte replay segment cleanup (2026-11-12)

`src/ReplayStageSegmentCleanup.cpp` implements the full, independently
reviewed `ClearReplayStageSegments` at `0x437420..0x43747C`. The function
selects the replay stream by stage index (state +0x40, 12-byte stride),
walks its linked nodes with a saved `next`, and, for each non-null owned
segment, detaches the segment from its intrusive pair of links at
+0x18A8/+0x18AC, clears those links and invokes `operator delete`.
The node traversal and two-way unlink are retained without a new null
check or guessed container ownership.

All four focused VC9 `/GL` contexts—an independent dynamic caller, the
real `ReplayWriter.cpp`, the 1380-byte replay initializer, and the two
real producers together—compile **93/93 bytes, 33/33 instructions and the
one native delete call** with zero nonfield differences. Strict PE/MAP/PDB
replay independently binds the single native REL32 at +0x4D to the exact
`operator delete` target `0x45FD49`; all 93 bytes match with current
source/tool/producer receipts and `difference_count = 0`. Canonical unit:
`clear-replay-stage-segments`. The local replay log is
`.analysis/replay-stage-segment-clear-strict-20261112.log`, with a bounded
probe `.analysis/probe-replay-segment-cleanup-20261112.py` and public
producer `probes/replay-stage-segment-cleanup-context.cpp`.

## Corrected mode-0 replay segment lifecycle call (2026-11-12)

The native prologue of `InitializeSecondaryReplay` loads `g_StageVariant`
into EAX and the replay state into ECX immediately before calling exact
`ClearReplayStageSegments` (`0x437420`). The subsequent
`AppendReplayWriteSegment` (`0x437300`) also receives the **selected stage**
in its private EDI argument, while the returned tail node is stored in the
separate fixed `state+0x9C` field. Earlier source had mistaken the first
call for a header initializer and passed a fixed stage 7 to the second.
`src/SecondaryReplayInitialization.cpp` now models both selected-stage
operations and uses the complete reconstructed cleanup owner.

Scoped VC9 `/GL` re-compilation of the corrected complete 1380-byte function
preserves all 22 native calls and four returns; with the real cleanup and
writer producer, the diagnostic is **1247/1380 bytes**, still non-exact.
The replay loader's separate 441-byte owner was also relinked with both
exact shared-file helpers and the actual codec/file producers: full source
continues to compile, with 14/14 calls and one native return, but
**402/441 bytes** versus **387/441** in the original opaque-helper context.
The previous diagnostics are retained and no unsupported exactness is claimed.
Local reports: `.analysis/probe-secondary-replay-init-with-true-cleanup-20261112.log`
and `.analysis/probe-secondary-replay-load-with-helpers-20261112.log`.
