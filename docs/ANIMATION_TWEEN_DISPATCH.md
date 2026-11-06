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

| Function | Address | Target bytes | Recorded candidate / diagnostic differences |
| --- | --- | ---: | --- |
| StepAnimationIntVectorTween | 44E4B0 | 768 | 768 /339 |
| StepAnimationIntScalarTween | 44E7C0 | 332 | 321 /283 |
| StepAnimationFloatPairTween | 44E910 | 673 | 673 /359 |
| StepAnimationFloatScalarTween | 44EBD0 | 331 | 326 /282 |
| UpdateAnimationRenderer | 455480 | 221 | 231 /180 |
| UpdateOtherAnimationRenderer | 455560 | 249 | 232 /193 |
| DrawAnimationRendererLayer | 455660 | 127 | Exact;9 fields |
| AllocateSceneAnimationVm | 4569C0 | 177 | Exact;5 fields |
| ResetAnimationScalarTweenTime | 44ED80 | 52 | Exact;1 field |
| Four interpolation factors | 44EDD0 /44EE00 /44EE30 /44EE60 | 39 each | Exact;1 field each |

The first four rows use the current canonical AnimationTween primary context;
their large bodies remain diagnostic, without acceptance units. Other historical
compiler contexts are distinguished below.

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

- initial: `((t - 1) * (t - 1)) * (t + t + 1)`;
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

That historical aggregate candidate used frame0x48 instead of native0x40. The
current integer value constructor and independent coefficients recover frame40
as described below; completion copies/epilogues and scheduling still differ.
Equal length and frame do not close any part of the full owner. Scalar/pair
diagnostics in the table likewise retain all instructions through their final RET or actual
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

## Pair value and hidden-result ABI checkpoint

Fresh full direct Ghidra review retains the complete673-byte pair owner,221
instructions, six returns, no foreign interior entry and15 CC bytes before the
next owner. A fresh caller query finds only ExecuteAnimationVm44B4B0; the actual
call is44DBEC. Native EDI receives the tween and EBX receives caller storage;
each of the six paths writes both DWORDs to that buffer and returns its pointer
in EAX. The physical two-float value remains eight bytes, with four value slots
and the same timer/duration/mode offsets.

The earlier POD pair compiled to an EAX/EDX bit result. The maintained pair now
has a two-component constructor that initializes X/Y from its arguments, and
helpers construct their actual arithmetic results through it. This naturally
recovers the reviewed hidden-result interface under the existing /GL profile.
It models a value type; it does not prove the original nominal math class,
constructor API or TU partition. The original diagnostics for other rows above
retain their separate interpolation/renderer contexts.

Complete source is now673/frame28 versus target673/frame28. All four address
fields (timer advance, time-scale pointer, double3 and pair factor) replay;359
full bytes still differ. The same673/359 is reproduced in the complete animation
family and the canonical AnimationTween primary context. Mode17 snapshots initial
before adding control2, retaining the observed four copy instructions. Before
that snapshot the value model was628/614. The preceding POD in the complete
animation family was615/593, with a different result ABI; its smaller byte count
was not a complete reconstruction.

Static arithmetic paths for modes7/17/8/ordinary contain target23/35/93/38 versus
source23/35/92/38 instructions. Corresponding float-round stores are2/4/19/7 on
both sides. A bounded operand trace now follows fields, temporary stores and
the result buffer through these four paths. Graphs agree only after commutative
add/multiply operand normalization; strict order still differs in17/8. This is
not an exceptional-value/status or full-function semantic proof. Separate X/Y
locals before pair construction were byte-identical in the earlier value model;
repeating that spelling is unhelpful.

The animation caller remains non-exact at12296/frameE0/11741 complete differences,
with407 fields and416 static opcode traces. It uses the correct EDI/EBX/EAX
callee contract but buffers the pair atESP+B0 versus native88. Its frame/alignment
and VM register remain open. The thirteen accepted factor/duration/scalar-setting
units affected by the shared header/source have one common cold compilation;
all thirteen strict full-field replays remain exact. They receive no new credit.

## Hermite grouping and pair temporary lifetimes

Fresh direct Ghidra reviews and bounded x87 operand traces establish the first
coefficient's multiplication grouping in all four complete interpolation bodies:
square t-1 first, then multiply by t+t+1. The old left-associated expression
multiplied the factor by one delta before the second delta. Matching float-store
counts had not detected this operand-tree difference. The shared helper now
uses the reviewed grouping; the other three coefficients keep their formulas.

The pair path uses four separate float coefficients and a named final pair for
both mode8 and ordinary interpolation. These are actual arithmetic values: the
rounded progress and promoted t are the same calculations previously inside
Hermite. A named pair restores the observed float temporary followed by DWORD
copies to the result buffer. Keeping independent coefficients instead of the
aggregate recovers the native28-byte frame and all local homes in that path.
No new class constructor, ABI declaration, padding or instruction is introduced.
These compiler observations do not identify the original math class or TU.

| Full-animation context | Prior source bytes / differences | Current source bytes / differences |
| --- | ---: | ---: |
| Integer XYZ768 | 768 /353 | 768 /348 |
| Integer scalar332 | 329 /282 | 329 /286 |
| Float pair673 | 644 /597 | 673 /359 |
| Float scalar331 | 332 /279 | 332 /280 |

All complete bodies and fields remain in each comparison. The shared grouping
is retained from operand evidence even where the whole-byte diagnostic worsens.
The complete12292-byte interpreter is byte-identical to the prior candidate
after all407 fields replay; its pair result buffer remainsB0 versus native88.

Rejected probes: an explicit field-copy constructor gives648/569 but adds four
float-round stores to each of modes7/17, failing to reproduce their2/4 stores.
Named results with aggregate coefficients give674/frame30/625; separate
coefficients recover673/frame28/359. Using cached duration gives640/599 and
drops a target stack store. Reversing coefficient-B operands with a direct-field
completion comparison gives675/627 and retains the extra duration-register copy.
These variants receive no partial or masked credit.

Remaining pair gaps are the post-advance EAX-to-ECX duration copy, mode17's
control2-X add operand order, and mode8's doubling/subtraction/product scheduling.
Full extent,221 instructions, six complete result returns and frame28 now agree;
359 differing bytes still prevent acceptance. Fourteen existing exact units
affected by AnimationTween.cpp are cold-replayed in two families: thirteen in
AnimationTween and the1168-byte screen initializer. No new exact unit or source
mapping is added; totals remain234 exact functions/27,546 bytes.

## Integer XYZ value and actual color consumers

Fresh direct Ghidra retains44E4B0..44E7AF:768 bytes/274 instructions, no table
or foreign interior entry; the following four-byte getter is separate. EAX
supplies the tween, EBX supplies12-byte output storage, and all six returns
write all three words and return the storage pointer in EAX. Both native color
calls44DB94/44DC50 pass EBX=ESP+24 and read three BYTEs directly from that
stack temporary, in Z/Y/X order, before publishing primary/alternate RGB.

The maintained integer value now has a three-component constructor assigning
its actual X/Y/Z fields. Add/Subtract/Scale construct their actual results;
no default/copy constructor or layout/ABI annotation is added. Mode8 uses four
independent rounded coefficients, with the same progress/promotion/formulas.
This combination recovers frame40 from48 while retaining768 bytes/274
instructions. All19 fields replay with339 full differences, down from348,
in both the public full-animation and canonical tween contexts. A constructor
alone gives370 differences; independent coefficients alone give frame50/336.
Neither isolated result is retained. Nominal class, constructor API and original
TU remain hypotheses; layout stays12 bytes and tween4C.

A bounded static trace covers7/17/8/ordinary arithmetic after the timer gate.
All12 mode8 float-to-int arguments and their X/Y/Z conversion sequence agree
strictly; the ordinary path has three agreeing arguments. Output graphs agree
using only modular32-bit integer-addition reordering/flattening; no floating
commutation is required. Completion traversal checks all six full-buffer
returns. Factor/conversion operations remain opaque, self/result are disjoint,
and timer/status/exception/alias/full-runtime semantics are not established.

The two actual interpreter consumers now also read BYTEs directly from their
stack result, replacing DWORD reads through returned EAX. Their buffer is28
versus native24 and byte-register allocation differs. The full owner grows
12292 to12296 bytes, differences11727 to11741 after407 fields; ring/arc x87
add ordering and alignment code also change. FrameE0/VMEBP/no8 alignment,
262/260 calls and all416 static opcode skeletons remain. The target-backed
callee frame and consumer access forms motivate this retained type model;
the whole-byte diagnostic is not monotonically improved. No exact credit.

Other complete probes rejected: explicit weighted vectors and returning stored
initial are byte-identical to their respective coefficient baselines; output-
reference coefficients give frame48/507, or frame38/535 with the constructor.
For the pair, reversing timer-helper arguments or scalar/component multiplication
is fully byte-identical after four fields; direct helper results give677/551,
and 2.0*t gives673/367 with a changed ordinary-Y operand order. These probes
do not resolve the main alignment/frame or pair temporary-home gap.

The current source/header change cold-replays the same fourteen existing exact
units in two canonical families, all zero differences. Source mappings,
reviewed boundaries and234 exact functions/27,546 bytes are unchanged.

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

The earlier update contexts lacked native8-byte stack realignment while the
interpreter was opaque. Its complete12056-byte owner now has maintained source;
the current full-family candidate still lacks native alignment. Renderer-update
closure and the interpreter's remaining frame/register gaps stay non-exact.

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
