# Complete animation interpreter and update

ExecuteAnimationVm at44B4B0 now has complete natural C++ source, including the
instruction interpreter, events, all nine tween updates and both generated-strip
paths. Its full12056-byte owner remains non-exact. Physical views and compiler
observations do not establish original class identities, authorship or TU layout.

## Complete owner and ABI

Fresh direct mapped-text-attested Ghidra review/disassembly and independent full
PE decoding/control-flow/reference/neighbor checks establish:

| Component | Bytes |
| --- | ---: |
| Reachable code:3202 instructions | 11637 |
| Internal loop alignment | 3 |
| Primary104-cell table | 416 |
| Full owner44B4B0..44E3C7 | 12056 |

The automatic11640-byte span ended before the table at44E228. All104 apparent
foreign DATA references are owned cells, covering signed opcodes-1..102. There
is no foreign interior entry. Eight CC bytes separate the owner from the missed
following entry44E3D0; later44E430/44E460 are also separate code, not more data.
Those neighboring bodies are not assigned source by this work.

Native entry takes the stack VM pointer, aligns ESP to8 and reserves frameCC.
It returns EAX0/1 through three RET4 points. The decompiler's phantom register/
float inputs and eight-byte return are not APIs. The post-update callback at428
is a fastcall VM callback; the sprite transform42C receives VM/input integer in
ECX/EDX and returns an integer. The source retains conventional interfaces rather
than forcing the target's private register allocation.

## Complete maintained operations

The primary color is a packed DWORD at374, with alpha377; the alternate color
occupies378..37B. The older AnimationVmView.spriteIndex name at374 is a legacy
label. The new physical view records color explicitly without changing the
shared header and invalidating unrelated exact contexts.

The instruction header is signed16 opcode, unsigned16 byte size, signed16 frame
and unsigned16 variable mask. Its payload begins at8; argument flags select
literal values versus the reviewed variable decoders. Headers/payloads retain
unchecked sizes and accesses, and frame comparison is signed.

| Instructions | Operations |
| --- | --- |
| -1/1/2 | Clear active bit where observed, null instruction, restore time scale and return1 |
| 0/64 | Normal encoded-size advancement;64 is also the event label opcode |
| 3/102 | Set active, decode/optionally randomize sprite, apply optional sprite transform, bind sprite and publish sprite frame |
| 4/5 | Raw byte-offset/time jump; decrement destination then conditionally jump |
| 6..27 | Integer/float assignment, in-place arithmetic and separate-operand arithmetic; integer wrap and signed division/remainder retained |
| 28..39 | Six integer/float relation pairs and their raw jump/time operands |
| 40/41 | Integer random modulus with the observed zero guard; float random scaling |
| 42..47 | Existing unary math boundaries, acos, atan and normalized addition with zero |
| 48..54 | Position target selected by bit100, rotation, scale, color/alpha and rotation/scale velocity |
| 55..60 | Alpha, position, RGB, rotation and scale tween configuration, retaining argument order and raw mode bytes |
| 61/62 | Flip flags and multiply the corresponding scale by double-1 |
| 63/69/81 | Event pause, active-bit clear variant, and saved timer/instruction return |
| 65..68/72..74 | Raw packed flag fields, fan-mode initialization and byte layer selection |
| 70/71/75 | UV velocity and actual RewindSecondaryTimer call |
| 76..80 | Alternate RGB/alpha and their tweens, plus alternate-color selection bit |
| 82..87/89 | Packed mode bits, translation transfer and unchecked strip allocations |
| 88/90..92/95..97 | All seven child-creation paths, ordered lookup/insertion and position/translation publication |
| 93/94 | UV velocity scalar tweens |
| 98/99 | Quad-derived UV setup and its continuous-update bit |
| 100 | Full position Hermite controls, mode8, selected initial position and reversed target XYZ argument reads |
| 101 | Second strip mode and allocation |

Events search from scriptStart for a matching64 label, remembering a wildcard-1
label until the terminator. The chosen label saves all five timer words and the
instruction pointer, sets its timestamp, then continues at the following record.
A missing label or ordinary pause offsets time by-1 before post-update work.
There is no invented bound or missing-label early return.

Post-update work preserves angular/scale velocity, one-step UV wrapping, runtime
translation delta, optional quad-to-UV refresh, all nine tween applications,
closed ring mode9 and open arc mode13, then the optional callback and inline
clock. The ring emits count-1 pairs into its allocated buffer and copies the first
pair to close the strip. The arc emits count pairs with denominator count-1. There
are four actual polar-point calls across the two loop bodies; unchecked counts,
allocation results and required child pointers retain the target's behavior.

Time-scale override bit29 saves the global scale and temporarily publishes1.
Normal timer completion and script termination restore it. A nonzero post-update
callback returns1 directly without restoring it. This observed path is retained;
no RAII cleanup is added to change it.

Float comparisons are checked against x87 condition masks, rather than copied
from decompiler Boolean expressions. In particular the floating greater-than
jump is ordered; an initial negated-less-or-equal spelling admits unordered and
was corrected before the maintained comparison. Integer payloads/remainders
remain integer words despite misleading float temporaries in the decompiler.

## Real direct dependencies

AnimationExecutionArguments.cpp provides the four variable decoders/destinations
and UV conversion. Their full reviewed extents include all tables/selectors:

| Address / source role | Full extent |
| --- | ---: |
| 44B080 float read | 540=447 code+1 alignment+92 table |
| 44B2A0 integer read | 219=145 code+3 alignment+48 table+23 selectors |
| 44B380 float destination | 124=76 code+48 table |
| 44B400 integer destination | 104=62 code+2 alignment+40 table |
| 44B470 UV conversion | 52 |

References10000..10022 expose four integers, four floats, two more integers,
three RNG float forms, XYZ position, six runtime floats and raw RNG output.
Destination helpers preserve the original pointer on unsupported references.
Scene RNG4C2EF8 and existing text RNG4C2F00 are distinct; the VM flag selects them
where the actual code does. The random-sprite instruction uses text RNG and has
no zero-modulus guard. The ordinary integer-random instruction has its real
zero guard. These behaviors are not generalized into a new random API policy.

AnimationExecutionSupport.cpp supplies the actual vector timer reset, related
node insertion and RGB/alpha/scale setup bodies. Alternate-alpha setup leaves
its controls untouched, unlike the primary-alpha setup. Scale setup receives
the whole VM, not a falsely declared tween pointer. It now also implements the
actual range-RNG dependency40B9F0..40BA21/50, whose only target caller is44D550.
These thirteen small source mappings are direct dependencies of the full12056-
byte owner and receive no independent leaf acceptance units or exact credit.

Range RNG consumes the state in ESI and one stack float, returning ST0/RET4.
Natural conventional C++ parameters and real /GL bodies recover that private
placement without register annotations. The unsigned-to-float conversion uses
the target's float2^32 correction, followed by double2^-32 scaling, a float unit
round, multiplication by maximum and a second float round. Combining the last
two arithmetic stages changes rounding: raw16777217/maximum3 gives3C400000 with
the target's two rounds versus3C400001 with one. The complete dependency's50
bytes and three reviewed call/literal fields replay with zero diagnostic
differences; its support-TU ownership supplies no independent exact acceptance.

Existing tween, angle, random, sprite, fan, ID creation and rendering bodies are
used in the cold family. Polar44EF80, front-draw factory456020 and the other
reviewed random/unary wrappers still have source-open bodies. Their declarations do not
grant source or exact credit. HUD now calls existing ExecuteAnimationVm rather
than the older void prototype name for the same target.

## Complete comparison and remaining gaps

The maintained full cold /GL primary contribution is12296 bytes with frameE0 and
stack VM input, but lacks native8-byte stack alignment and uses EBP for VM versus
EDI. All104 table cells and407 independently reviewed address/table fields
replay;11741 bytes differ over the complete owner, including length differences.
The source has3278 reachable instructions/11878 code bytes+2 alignment+416 table.
Native/source calls are262/260: integer RNG4/3 and float destination15/14 remain.
Lookup5/5, related insertion3/3, malloc2/2 and float read78/78 now agree physically.
These counts grant neither semantic nor exact credit.

The current IntVectorValueOwners family retains all47 public support
sources, the real HUD/factory callers and /GL. Replacing the range-RNG declaration
with its actual support body removes a passed state-pointer stack word: ESI
carries that pointer and only maximum occupies the outgoing stack slot. A
bounded static ABI check follows scene/text and literal/decoded maximum paths;
all four place ESI/one word as the target does. At that range checkpoint,
compared with the12300-byte baseline, the interpreter loses8 bytes/3 instructions
and its complete rebased diagnostic differences fall11742 to11727. Main frame/alignment, pair
buffer placement and the two call-site merges remain open.

A float-writing helper for17/27/45/46 is byte-identical to the baseline after
all407 fields replay. Explicit zero assignments in both integer-RNG40 arms
restore the fourth RNG site but worsen the full owner to12312/11787/408 fields;
that variant is not retained. Direct Ghidra confirms assignment7 has its own
destination call while17/27/45/46 share another. The target's8-byte alignment
does not coincide with any eight-byte ESP/EBP memory operand in the full body;
that absence does not identify the original local types or justify invented
double locals. Both rejected source spellings are recorded to avoid repetition.

The former12884-byte contribution spilled a ReadF literal to a temporary before
loading it again on the direct path. Returning directly from each helper branch
removes that unnecessary round trip:12272 bytes before the RNG correction,
versus12884. A bounded inspection found55 old EBX-payload load/store/reload
triples and none after the change; that pattern count does not account for every
removed instruction. Separating the four child cases alone costs112 bytes and
is rejected. Keeping them separate in reviewed native entry order restores the
single signed opcode dispatch, the three factory/allocation site counts above,
and the12272-byte size. The handlers now live together in
AnimationExecutionInstructions.inc; related fallthrough labels remain atomic.

Instruction41 also required a semantic correction. Native44D523 selects scene
state4C2EF8 when VM flag40000000 is set, while44D543 selects text state4C2F00
otherwise. The old source decoded in both arms but always passed text state.
Separate complete arms preserve selection before decoding and recover the
missing physical float read. That correction added48 bytes, giving the preceding
12320-byte checkpoint.
A targeted static argument replay checks both flag values with literal and
variable payloads, including zero/one ReadAnimationFloat call, against both
native and source. The old candidate fails the scene-state case. This bounded
check does not validate the rest of the interpreter's arguments.

The ring9 count also needs a rounded float denominator. At44DCF4 the target
decrements the integer count, then FILD/FSTP float at44DCF9/44DD03 rounds it before
both divisions. The earlier source divided directly by the integer. Under
round-to-nearest, count16777217 with V numerator1 gives old337FFFFF versus target
33800000, one float ULP apart. The explicit float denominator restores the native
10-instruction x87 ring setup; the arc13 setup retains its matching12 instructions.
Those checks rename stack slots and the VM register and grant no exact credit.
The correction alone gives12336 bytes,16 more than the prior checkpoint.

EmitStripPair now advances its local caller pointer after each vertex. This
recovers two1C cursor advances per loop (native EBX; source ring EBX/arc ESI)
and keeps color in a register across the two polar calls, removing the extra
second-vertex pointer
and per-iteration color reload. That checkpoint fell44 bytes to12292,28 below
its preceding version; the subsequent pair-value ABI adds8 bytes. Both loops still emit their complete pair counts and the
ring retains its two closing copies. Nested vector sums alone retain frameE0
and12336 bytes; no stack reduction follows from just shortening those lifetimes.

A primary-only /Oy- probe gives11688/frameF0 with VM EBX, no8-byte alignment and
264 calls (float destinations18/native15; RNG3/native4). It does not explain the
native frame/alignment/VM register and is not adopted. The maintained profile
remains /O2 /Ob1 /GL. The native pair tween's hidden result is now recovered:
44DBE5 supplies EBX=ESP+88 before44DBEC and copies through returned EAX; current
natural C++ likewise passes EBX and returns the pointer in EAX, with EDI tween.
Its result buffer is still ESP+B0. The earlier POD pair returned bits through
EAX/EDX. The maintained two-component value constructor writes the actual X/Y
components; nominal class/constructor identity and original TU remain hypotheses.
See ANIMATION_TWEEN_DISPATCH.md for the complete673-byte callee and six returns.

The source pair callee is now673/frame28 versus native673/frame28, with359
complete byte differences after its four reviewed fields replay. Six paths each
fill both buffer words and return the buffer pointer. Named final pair values
restore the observed temporary-to-result DWORD copies; four independent float
coefficients recover the frame. Target/source7/17/8/ordinary paths have
23/35/93/38 versus23/35/92/38 instructions and2/4/19/7 float-round stores.
A bounded operand trace agrees only after commutative add/multiply normalization;
strict order still differs in17/8. It grants no floating exception/status or
full-function semantic proof. The first Hermite coefficient now squares t-1
before multiplying by t+t+1, as directly reviewed in all four interpolation
bodies. Separate X/Y float locals were byte-identical in the earlier value model.
Thirteen already accepted exact units affected by AnimationTween.hpp/cpp share
one cold context and all strictly replay with zero differences. No new exact
unit, source mapping or original-type credit follows from this ABI correction.
Those pair-header replays are historical. The range-body/header change affects
only initialize-secondary-hud-screen-effect; one new canonical cold replay
retains its complete1168 bytes/all54 fields/zero differences. Totals remain234
exact functions/27,546 bytes; the interpreter and range dependency gain no exact
credit. The range mapping adds one reviewed boundary and one source mapping.
The subsequent Hermite/pair-value change affects only AnimationTween.cpp. Its
fourteen existing exact units receive two new canonical cold-family replays;
all remain zero. At that Hermite/pair checkpoint the full interpreter stays
byte-identical after407 fields replay.
See ANIMATION_TWEEN_DISPATCH.md for all four whole-body diagnostics and rejected
copy-constructor, aggregate-result, cached-duration and operand-order probes.

The subsequent integer XYZ constructor/independent-coefficient change recovers
the callee768/frame40/339 complete differences. Its two actual color consumers
44DB94/44DC50 now read three BYTEs directly from the result stack temporary,
in Z/Y/X order, matching native access width/location and publication order.
Previously they loaded DWORDs through returned EAX. EAX tween/EBX output/EAX
result-pointer contract is unchanged; caller buffer28/native24 and byte-register
allocation differ. The full interpreter grows12292 to12296, differences11727
to11741 after407 fields, including changed ring/arc x87 add order and alignment
code. This target-backed value model improves specific callee/caller evidence
without establishing whole-owner equality or original class/TU. The same fourteen
affected exact units cold-replay in two families, all zero differences.

All104 dispatch entries have equal static per-op call/instruction-loop/post-
update/return skeletons:416 distinct paths on each side. Constant opcode choices
are propagated through the source's grouped factory choices; other predicates,
operand identities, aliasing and dynamic loop reentry remain unproved. This
check localizes physical call merging but is not a runtime or semantic proof.

A direct predecrement spelling for instruction5 is byte-identical to the
wrapped subtraction helper: it still uses a long-livedEDI=-1 and memory ADD
versus native DEC. Repeating that spelling probe does not resolve the gap.

At the previous complete-source checkpoint, the real dependency family gave
HUD4652/frameC/4101 differences and all27 calls. Dialogue was6948 bytes with
native frame140 and5357 full
replay differences versus earlier frame13C/5694; all454 fields,252 calls and46
static per-op traces remain. This is a compiler observation under a fuller
context, not proof of original TU ownership or whole-function equality.

Next gaps are the full interpreter frame/alignment, VM register, pair-tween
result-buffer home, temporary/vector lifetimes and the two remaining call merges.
Float destinations for
17/27/45/46 share a native tail; the source also folds7 into it. Integer random40
shares its RNG call in source but has two physical native sites. Preserve complete
extents and /GL; no volatile, fake returns,
private-register declarations, padding or cropped extents are used.


## Earlier full stage-update interpreter graph baseline

The following figures are the earlier /Oy- baseline checkpoint, before the
random conversion leaves below were restored as real C++ functions. They are
kept as a comparison point rather than current build metrics.

The `update-stage-real-interpreter-graph` diagnostic frontier now links the
complete natural C++ `ExecuteAnimationVm` implementation and its actual source
dependencies into the same image as `UpdateStage`. It includes
`AnimationExecution.cpp`, `AnimationExecutionArguments.cpp`,
`AnimationTween.cpp`, and the real VM drawing, fan, projected-quad, mesh,
renderer and secondary-animation support sources. Remaining external helper
edges are still anchored to reviewed target addresses. No private-register
assembly adapter, target-byte stub or padding is used.

| Function | Native extent | Linked natural C++ candidate | Diagnostic alignment |
| --- | ---: | ---: | ---: |
| ExecuteAnimationVm | 12,056 B | 11,620 B | 704/3,308 instruction keys |
| ReadAnimationFloat | 540 B | 544 B | 55/169 instruction keys |
| ReadAnimationInteger | 219 B | 219 B | 42/63 instruction keys |
| RandomNext32 | 153 B | 153 B | 37/39 instruction keys; two import-thunk address differences |
| ResetStageMesh | 325 B | 325 B | 100/104 instruction keys |

The interpreter candidate has264 call instructions versus262 native and380
conditional branches versus406. The native prologue uses an EBP frame, aligns
ESP to eight bytes and reserves0xCC bytes; the current candidate reserves0xE0
bytes without the explicit alignment and keeps the VM pointer in EBX instead of
EDI. Scalar component sums in `EmitStripPair` preserve both native operand
orders while avoiding two materialized 12-byte temporary vectors; this reduced
the candidate by84 bytes, reduced its frame by8 bytes and added one aligned
instruction key. The full owner is present and callable from the stage-update
candidate, but its control-flow and code-generation differences remain large.
The byte-size reduction is not exactness evidence.

The 153-byte `RandomNext32` candidate uses the real generic `RandomStateView *`
caller in the interpreter's argument readers. Its dedicated canonical
`random-next32` unit remains the independent cold-replayed exact proof; the
broad graph's two remaining instruction-key differences are only the linked
addresses of `EnterCriticalSection` and `LeaveCriticalSection`.
Rebuild the expanded context selectively with:

```bash
scripts/repo-python scripts/build-match-unit.py --unit update-stage-real-interpreter-graph
```


## Fast isolated interpreter iteration

The animation-execution-isolated frontier compiles AnimationExecution.cpp
as the primary LTCG translation unit and calls it from a separate normal-COFF
C++ probe. Missing dependencies are diagnostic linker data anchors, not code
stubs. This keeps the public stack-argument ABI at the call boundary while
allowing a one-minute source/codegen experiment without rebuilding the full
stage-update support graph. The image is not runnable and carries no exact
credit.

| Isolated profile | Candidate | Aligned instruction keys |
| --- | ---: | ---: |
| /Oy- /Ob1 | 10,744 B / 3,115 instructions | 588/3,308 |
| /Oy- /Ob2 | 10,744 B / 3,199 instructions | 589/3,308 |
| /Oy /Ob1 | 10,744 B / 3,199 instructions | 589/3,308 |

The /Oy variant emits the native eight-byte stack-alignment instruction,
but still reserves 0xD4 bytes rather than 0xCC and keeps the VM pointer in EBX
rather than EDI; the instruction cursor is in ESI rather than EBX. The /Ob2
variant does not materially change the body. These results show that the
alignment sequence is sensitive to frame-pointer policy, but changing those
flags alone does not recover the native register assignment or frame layout.
The larger full stage-update graph remains the primary diagnostic for
callee/inlining and call-context effects.

Rebuild the quick isolated frontier with:

    scripts/repo-python scripts/build-match-unit.py --unit animation-execution-isolated


## Full stage-graph interpreter and random leaves

The bounded update-stage-real-interpreter-animation-oy frontier now includes
AnimationExecutionSupport.cpp as a real support translation unit. Three
previously unresolved random-conversion dependencies are implemented in
natural C++ and linked as real functions, not as diagnostic aliases:

| Function | Native extent | Candidate extent | Aligned instruction keys |
| --- | ---: | ---: | ---: |
| AnimationRandomUnit | 36 B / 12 ins | 36 B / 12 ins | 12/12 |
| AnimationRandomSignedUnit | 42 B / 13 ins | 42 B / 13 ins | 13/13 |
| AnimationRandomSignedRange | 56 B / 16 ins | 56 B / 16 ins | 16/16 |

Their instruction sequences match after normalizing relocated call and data
addresses. The signed conversion retains the native 2^-31 scale and -1
subtraction order; the unsigned conversion retains 2^-32. The signed-range
owner performs the final maximum multiplication after converting the random
integer. All three call the real RandomNext32 implementation.

The argument-reader leaf also improved:

| Function | Native extent | Current candidate | Aligned instruction keys |
| --- | ---: | ---: | ---: |
| ReadAnimationFloat | 540 B / 132 code ins | 540 B / 132 code ins | 128/132 |
| ReadAnimationInteger | 219 B / 63 ins | 219 B / 62 ins | 37/63 |
| AnimationFloatDestination | 124 B / 40 ins | 124 B / 46 ins | 20/40 |
| AnimationIntegerDestination | 104 B / 34 ins | 104 B / 46 ins | 17/34 |

ReadAnimationFloat now has the native entry sequence
fld [esp+4], push esi, fld st(0), mov esi,eax. Its 23-entry jump table has the
same relative case offsets as the target, including the non-monotonic random
case order 10011, 10012, then 10010. The code region is 448 bytes / 132
instructions on both sides, followed by the 92-byte jump table. The three
random paths now share the native float spill/reload return shape and call the
real signed-range, unit and signed-unit C++ helpers. The remaining code-only
difference is localized to the signed-range path: the candidate selects the
random-state address before loading the 3.1415927f argument, while the target
loads that constant first. This is a diagnostic match, not an exact leaf
claim.

With the actual helper definitions linked, the complete interpreter owner is
12,084 B, with 1,050/3,308 target instruction keys aligned. It has 262 call
instructions, matching the native count, and 383 conditional branches versus
406 native. Its prologue still uses EBP as the VM-state pointer and reserves a
0xD8 frame; the native owner keeps EBP as its frame pointer, aligns ESP to
eight bytes, reserves 0xCC bytes and keeps the VM pointer in EDI. The main
owner and the other three argument leaves remain open.

The linked image is still a diagnostic graph with unrelated dependency anchors;
it is not runnable and earns no exactness credit. No ASM, byte stubs, padding,
or clipped extents are used. The targeted incremental link reuses the other
52 support objects and recompiles only the affected translation unit.

Build the bounded frontier with:

    scripts/repo-python scripts/build-match-unit.py --unit update-stage-real-interpreter-animation-oy
