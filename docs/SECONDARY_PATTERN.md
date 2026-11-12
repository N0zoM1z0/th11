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
| AddAssignSecondaryIntegerPair | 433DD0 | 23 | Source23/6 |
| SecondaryIntegerPairNonzero | 433DF0 | 20 | Exact |
| SubtractSecondaryIntegerPair | 433E10 | 18 | Source20/16 |
| ScaleSecondaryIntegerPair | 433E30 | 19 | Source21/16 |
| DivideSecondaryIntegerPair100 | 433E50 | 51 | Source49/50 |
| SetSecondaryIntegerPair | 433E90 | 6 | Exact |
| AddSecondaryIntegerPair | 433EA0 | 18 | Source20/16 |
| AssignSecondaryIntegerPair | 433F10 | 23 | Source20/11 |

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
