# Animation interpolation, layer dispatch and allocation

Twenty-two complete source mappings cover3,129 target bytes, including integer
XYZ interpolation768, integer scalar332, float pair673 and float scalar331.
The renderer's two updates221/249, layer draw127 and allocation177 now also
have maintained source. Fifteen new complete cold linked proofs add551 bytes;
the four large interpolation bodies and two update bodies remain non-exact.

All22 boundaries and origins are reviewed. Nine tiny entries were absent from
the prior inventory, bringing it to2,047 candidates. Each origin remains
indeterminate: interpolation, scalar field access and engine roles do not prove
original authorship or distinguish templates/headers/reused code. No Ghidra
function was created and no maintained inventory was regenerated away.

| Function | Address | Target bytes | Current candidate / diagnostic differences |
| --- | --- | ---: | --- |
| StepAnimationIntVectorTween | 44E4B0 | 768 | 768 /345 |
| StepAnimationIntScalarTween | 44E7C0 | 332 | 321 /263 |
| StepAnimationFloatPairTween | 44E910 | 673 | 615 /581 |
| StepAnimationFloatScalarTween | 44EBD0 | 331 | 326 /269 |
| UpdateAnimationRenderer | 455480 | 221 | 231 /180 |
| UpdateOtherAnimationRenderer | 455560 | 249 | 232 /193 |
| DrawAnimationRendererLayer | 455660 | 127 | Exact;9 fields |
| AllocateSceneAnimationVm | 4569C0 | 177 | Exact;5 fields |
| ResetAnimationScalarTweenTime | 44ED80 | 52 | Exact;1 field |
| Four interpolation factors | 44EDD0 /44EE00 /44EE30 /44EE60 | 39 each | Exact;1 field each |

## Interpolation types and arithmetic

Four physical layouts are independently distinguished by target arithmetic,
copy sizes and timer offsets. Integer XYZ has four12-byte value slots followed
by a20-byte timer, duration44 and mode48. Integer scalar and float scalar use
four4-byte values, timer10, duration24 and mode28. Float pair uses four8-byte
values, timer20, duration34 and mode38; it is not another XYZ vector. Size
assertions retain4C/2C/3C/2C layouts without claiming original class names.

For positive duration, each function calls the real timer advance459270 and
then re-reads duration. When current reaches duration, it sets timer current to
duration, previous to duration-1 and fraction to rounded float duration, clears
duration and returns initial for mode7 or target for other modes. Timer
initialization installs scale4A7948 and flag bit0 when needed. Nonpositive
duration skips this advance/completion gate, then executes the selected mode;
no divide-by-zero or nonpositive-duration recovery is invented.

Mode7 adds target to initial and returns the updated value. Mode17 adds
control2 to initial, then target to control2, returning the updated initial.
Integer additions/subtractions retain32-bit wrap through unsigned arithmetic.
Float pair operations preserve aggregate rounding phases. These actual
position/velocity mutations are distinct from ordinary interpolation.

Mode8 uses rounded float progress `time.fraction / duration`. With double `t`
promoted from that value, four coefficients round individually to float:

- initial: `(2*t + 1) * (t - 1) * (t - 1)`;
- target: `t*t * (3 - 2*t)`;
- control1: `(1 - t) * (1 - t) * t`;
- control2: `(t - 1) * t*t`.

Integer XYZ converts every weighted component to integer before adding the
four integer vectors:12 native float-to-int conversion calls at4864E0. Integer
scalar instead sums all four weighted terms, then converts once. Its ordinary
path also adds initial before conversion; integer XYZ converts each delta
product before adding integer initial. A shared generic integer evaluator
would erase these observed truncation differences and is not substituted.

Float pair mode8 rounds component products and successive aggregate sums to
float. Float scalar keeps the arithmetic chain until its final float return
rounding. Ordinary modes use the actual factor function, signed/int or float
differences as appropriate, and the corresponding target-minus-initial path.
No clamp, saturation, reinterpretation of integer channels or replacement
math library is added.

The four39-byte factors divide fractional timer value by signed duration,
round the quotient to float, and call real EvaluateStageEasing459730 only for
unsigned mode-1<=15. Otherwise they return the quotient. The final float
store/reload matters. Scalar int/float factors have equal target bytes but
remain separate complete owners. Easing's own already reviewed764-byte
code/table ownership is not reinterpreted or newly credited here.

## Large-body compiler work

The first integer XYZ source produced723 bytes/647 diagnostic differences.
Explicit value snapshots for the mutating modes and separate completion
returns restored the full768-byte size and reduced differences to464. Returning
four coefficients as a real float aggregate reduced differences further to345.
That aggregate expresses the independently observed coefficient rounding; it
adds no inert code or padding.

The candidate still uses a48-byte local frame instead of native40, different
completion copies/epilogues, value-store scheduling and x87 scheduling. Equal
length does not close any part of the full owner. Scalar/pair diagnostics in
the table likewise retain all instructions through their final RET or actual
float-to-int tail jump, not a prefix chosen to fit a comparator.

A four-output-reference coefficient helper gave integer scalar323/268, float
pair619/574 and float scalar328/263. The aggregate form gives321/263,
615/581 and326/269. It is retained for the coherent four-coefficient value
representation and improved large integer-vector result; the remaining paths
stay explicitly non-exact. Earlier handoff/compiler hypotheses were not used
as byte evidence.

With an opaque easing declaration, the four factor candidates were40 bytes:
they pushed curve on the stack. Compiling the maintained StageEasing source as
real support naturally recovered the native EAX-curve/stack-progress contract
and all four39-byte bodies. No ABI declaration was falsified to remove the
push. Similarly, real AnimationImmediateDraw support replaces two stack pushes
with the native private register call and closes layer draw127; the opaque
context was125 bytes with32 differences.

## Renderer update and layer traversal

The31 extra VMs at renderer7B563C are sentinel storage. Update455480 clears
next-draw fields at+1C of the first29 and builds29 local tail pointers. It
walks activeHead7B562C, saving the update-node next before callbacks or reclaim.
Flag bit26 causes immediate reclaim. Otherwise callback410 runs first and its
return is ignored; the actual44B4B0 interpreter then runs. A nonzero interpreter
result reclaims the VM; a surviving VM appends to its layer chain via+1C.
Every visited VM increments processedVmCountB8, including reclaimed VMs.

Update455560 first clears processedVmCount and the last two sentinel next-draw
fields. It walks otherHead7B5634 with the same callback/interpreter/reclaim
order. Surviving layer values other than29/30 become29. Its native biased
stack addressing is represented by a real two-element tails array indexed
layer-29, without an out-of-bounds31-element fiction or extra initialized slots.

Layer draw starts at `additionalVms[layer].nextDraw`. Bit26 suppresses both
callback414 and DrawAnimationVmByMode451EF0. Callback return is ignored and
nextDraw is read after drawing, so no pre-saved next is invented. There is no
layer clamp. All three functions independently test runtime flags8000 at entry
and exit, entering/leaving critical section9 and updating its byte depth.
They return chain status1.

Both update candidates lack native8-byte stack realignment. The opaque
11,640-byte interpreter remains source-open, and no alignment padding, false
signature or fabricated interpreter body is used to force their frames. The
actual interpreter's full reconstruction remains substantial future work.

## Pool allocation and physical fields

Allocation reads signed nextPoolSlot4350BC through the global renderer4C3268.
It considers the current slot and, if occupied, advances using signed remainder
4096 and checks exactly one more. If that is also occupied, it allocates434
bytes, constructs a real StageEmbeddedVm and resets it. Native reset is still
called on the null allocation branch; no recovery is invented. A free pool
slot is merely marked used, relying on the reviewed reclaim/reset lifecycle.
All outcomes advance nextPoolSlot once more before returning the selected VM.
There is no scan of the other4,094 slots.

This recovers nextPoolSlot in the prior unknown4350BC prefix and confirms
used[4096] at4340BC. Header assertions check both offsets and the unchanged
7BD894 allocation size. Processed B8 and the already observed publication
counter7BD888 now have physical field names. Existing constructor/destructor,
buffer and reclamation source logic did not change.

## Small owners, extents and cold evidence

Nine newly recovered tiny owners are complete, independent functions:
44E7B0/4 reads integer-vector duration;44EBC0/4 reads pair duration;
44EDC0/4 reads scalar duration;44ED20/4 stores mode;44ED70/4 stores duration;
44ED30/5,44ED40/6,44ED50/6 and44ED60/6 copy pointed floats to the four scalar
value slots using x87 load/store. These physical names do not prove original
classes. Eight have exact proofs; duration setter44ED70 differs in one register
encoding byte and remains source-only. No arbitrary register promise is added.

The complete52-byte reset changes only the scalar timer prefix: lazy scale/flag
initialization followed by previous-1, current0 and fraction0. The value types
in its shared physical prefix remain unknown. It does not clear values,
duration or mode. Full reset code retains its sole scale-address field.

The integer-vector owner ends44E7AF; a separate getter starts immediately at
44E7B0. Other tiny entries are separated by CC padding. The explicit small-gap
review has two external interior references, both to the already partitioned
44ED80 reset entry; none reaches an instruction inside an individual function.
No jump tables, borrowed padding or cropped extents enter these proofs. The
original250-instruction vector query was truncated, so a separate full768-byte
explicit span supplies complete target review.

Canonical families are `AnimationTween` and `AnimationRendererDispatch`.
Fifteen new full primary-TU contributions replay551 bytes and19 independently
reviewed fields with zero differences. Fields bind easing459730, scale4A7948,
renderer4C3268, allocation45FCE4, VM ctor402270/reset401FD0, draw451EF0 and the
existing runtime/critical-section/IAT cells. Because the renderer physical
header changed, the two directly affected old units (buffer487/reclaim273)
were also cold-replayed, retaining760 bytes and29 fields. The17-unit selection
contains only15 new and these two affected old owners. No full cohort or broad
behavior suite was run; Oracle/build infrastructure is unchanged.

Private target evidence: `animation-tween-review.txt`,
`int-vector-tween-full-review.txt`, `animation-tween-small-gap-review.txt`,
`animation-int-vector-duration-review.txt`, `animation-pair-duration-review.txt`,
`renderer-dispatch-review.txt` and their full mapped-text attestations.
Independent receipts: `animation-tween-dispatch-independent-extents.json` and
`animation-tween-dispatch-independent-bindings.json`. Compiler probes include
`animation-tween-baseline.log`, `animation-tween-natural-copies.log`,
`animation-tween-coefficient-aggregate.log`, `renderer-dispatch-baseline.log`
and `renderer-dispatch-real-draw-owner.log`. Acceptance is recorded in
`animation-tween-dispatch-affected-cold.log`; private PE/MAP/PDB/receipts remain
under the corresponding canonical build directories. Target dumps, decompiler
hypotheses and tool payloads remain private.
