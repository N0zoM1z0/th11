# Buffered drawing and randomized animation fan

`AnimationFan.cpp` adds eight complete source mappings covering2,292 target
bytes, including the805-byte initializer and598-byte update callback. Six
functions have new complete cold exact proofs totaling889 bytes. Nine more
boundaries/origins are reviewed; all nine origins remain indeterminate.

Six complete functions were absent from the initial inventory. Explicit
read-only Ghidra spans and independent PE/Capstone decoding recover them,
bringing the provisional candidate count from1,995 to2,001. No database was
mutated or inventory regenerated over maintained rows.

| Owner | Address | Bytes | State |
| --- | --- | ---: | --- |
| DrawAnimationQuadVertices | 451AE0 | 138 | Exact;5 fields |
| DrawAnimationColoredStrip | 451B70 | 336 | Exact;16 fields |
| DrawAnimationColoredFan | 451CC0 | 336 | Exact;16 fields |
| IsAnimationVmActive | 452080 | 10 | Exact; no fields |
| AnimationVectorLength | 452090 | 44 | Exact;1 field |
| InitializeFanAnimationVm | 4520F0 | 805 | Source; candidate805 /417 diagnostic differences |
| UpdateFanAnimationVm | 452420 | 598 | Source; candidate614 /436 |
| DrawFanAnimationVm | 452680 | 25 | Exact;2 fields |
| AnimationPolarVector::SetPolar | 4526A0 | 30 | Actual boundary reviewed; body source open |

## Missing owners and buffered drawing

The138-byte owner has an early failure RET4 and a final success RET4, then six
CC bytes before451B70. Two336-byte functions follow at451B70 and451CC0; the
second ends immediately before the already reviewed451E10 fan owner. The
816-byte explicit span decodes all three functions and their six padding bytes.
Each control-flow graph is independent, with no borrowed neighboring bytes.

The64-byte span452080..4520BF contains a10-byte active-bit getter, six CC bytes,
a44-byte vector-length adapter, then four CC bytes before the existing floor
adapter4520C0. The25-byte draw callback452680..452698 is separately bounded by
padding and referenced by the initializer's VM+414 callback store. All nine
reviewed code spans decode completely to their final RETs, with no tables or
external interior references. Missing entry points stay explicit hypotheses
in Ghidra; the maintained CSVs record the independently reviewed owners.

Quad drawing checks active/visible/primary alpha, stores a changed texture cache
before flushing and re-reading it for SetTexture, flushes before geometry1,
applies VM draw state and enqueues four supplied vertices. It returns0 after
enqueue and-1 on failed eligibility. Byte access to alpha reproduces the native
CMP byte; a DWORD mask produced a141-byte candidate instead of138.

The two colored owners use20-byte XYZRHW/diffuse vertices, with no UV field.
They select FVF44/geometry4, apply normal VM state, then set color/alpha
operations to SELECTARG1 with diffuse ARG1. They unconditionally flush the
global renderer after local setup, disable Z writes and draw count-2 unsigned
primitives. Strip uses type5, fan uses6. Neither has an eligibility gate.

After drawing, the global renderer's geometry/C1/C3 cache bytes becomeFF and
blend becomes7. Color/alpha operations restore MODULATE and ARG1 restores
texture. This modifies the global caches even when the passed renderer differs.
Z writes remain disabled. Unknown cache bytes are retained as physical C1/C3
fields rather than assigned unproved meanings.

The active getter returns full EAX flags&1. The vector adapter rounds the square
sum to float, calls the actual31-byte AnimationDistanceRoot4196E0 and returns
its float result. The callback submits VM+400 with33 vertices through global
g_RenderBatch to the actual224-byte triangle-fan function451E10, then returns0.

## Fan storage and initialization

The initializer optionally frees old VM+400 storage and clears that cell, then
allocates exactly4B0 bytes without initialization. It publishes storage and
update/draw callbacks at VM+410/+414 before the random/vertex work. Native
allocation failure is not given an invented fallback.

The physical storage view contains33 RenderVertices, radius storage at39C,
radial deltas at420, drift values at4A4/4A8 and four unknown trailing bytes.
Only indices1..31 of the radius/delta arrays are initialized or stepped.
Offset/size assertions preserve the observed allocation; they do not claim the
original class or meanings for unused cells.

Shared RandomNext32 uses the already recovered state at4C2F00. Each signed
sample is rounded to float after `uint32 * 2^-31 - 1`; both UV drifts scale it
by the float1/120 promoted to double. The pinned float-conversion variant uses
the native single-precision2^32 correction with no extra float store before
scaling. This compiler observation does not establish portable excess-precision
behavior for other toolchains.

The center gets rounded position/offset XYZ, RHW1 and UV0.5/0.5. Thirty-one edge
vertices get polar UVs of radius0.5, RHW1, Z0, randomized radii80+8*sample and
correlated radial deltas. Each old delta is stored, then perturbed by a sample
times float1/30 and clamped to +/-float1/15. Radius offsets, perturbations and
angle steps retain the native float stores. Angles start at negative float pi,
conditionally subtract promoted float2*pi, and advance by the promoted float
step0.20268340408802032. Positions re-read VM offset/position after polar calls.

The center/edge colors and closing vertex32 are not initialized here. No clear,
early closure copy or extra random call is added. Initialization uses65
RandomNext32 calls in the observed order. The known RNG implementation is
compiled as real support, with an independent receiver probe to retain its
general calling contract.

## Update behavior and polar boundary

The update overwrites center XYZ and primary color. Both U and V use drift4A4;
the separately initialized drift4A8 is unused. Every UV increment rounds to
float. A negative value shifts that coordinate of all33 vertices by1, including
within each edge iteration. There is no positive wrap, modulo or repeated
normalization substituted for these one-step shifts.

Edges receive primary RGB and alpha0, radii advance by their stored deltas, and
the actual polar boundary overwrites XY. Z is not reset before adding the VM's
rounded offset/position vector, so edge Z accumulates across updates. The final
closing vertex32 copies all28 bytes from vertex1. Center/ring position copies
use the physical vector aggregate, preserving the native bit-copy phase.

The existing30-byte function takes an XY output in ECX and two stack floats,
executes paired FSINCOS, multiplies each result by radius, stores two floats,
and returns the output pointer in EAX. `AnimationPolarVector::SetPolar` models
this actual narrow thiscall boundary; it is not an invented replacement helper
or a claim about the original class name. Its body remains open. Source
initialization/update call it without substituting separate sin/cos operations.

The native implementation preserves EDX, allowing the update to keep its VM
there across calls. The opaque C++ boundary cannot convey this private
preservation; the candidate uses different registers and a1C-byte frame versus
native18. Initializer805 also differs in private registers and scheduling;
equal length grants no exact credit. No ABI misdeclaration or padding closes
these gaps.

## Cold proofs, origins and remaining work

The six exact units use `build/match-linked/AnimationFan` with full public
primary-TU contributions, current source/header/tool/producer receipts and
PE/MAP/PDB identity. All40 address fields were independently bound before
acceptance:5 for quad,16 for each colored draw,1 for vector length and2 for the
draw callback. The getter has none. Reviewed targets are runtime4C3280+8,
renderer root4C3268, flush44FD10, VM state44F710, enqueue44FDA0, distance4196E0
and triangle fan451E10. Every field and every remaining byte replay with zero
differences. No contribution or extent was cropped.

Only these six new units were cold-replayed. Existing canonical sources,
headers, families and receipts did not change; no old exact cohort or broad
behavioral suite was run. Aggregate exact state is88 functions/16,841 bytes.
Thin getter/math roles and custom effect/rendering roles are insufficient to
prove original authorship, so all nine new origin reviews remain indeterminate.

A separate private world-draw experiment reversed logical source parameter
order consistently in definitions/callers. It left the1,036-byte target's
candidate unchanged at1,038 bytes/750 diagnostic differences. That hypothesis
was rejected; no world-draw interface change was committed.

Private evidence: `vm-buffer-gap-full-review.txt`, `vm-query-gap-full-review.txt`,
`vm-fan-effects-review.txt`, `vm-fan-draw-callback-review.txt`,
`vm-buffer-draw-review.txt` and their complete mapped-text attestations;
`animation-fan-independent-review.json`, `animation-fan-exact-items.json`;
`animation-fan-six-cold.log`; complete canonical diagnostics in
`animation-fan-current/diagnostic.json` and `animation-fan-current.log`.
Baseline/float-conversion compiler probes and `world-vm-first.log` record the
experiments. Decompiler hypotheses, target text, compiler payloads and the
mutable Ghidra database remain private.
