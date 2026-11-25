# Complete animation interpreter and update

ExecuteAnimationVm at44B4B0 now has complete natural C++ source, including the
instruction interpreter, events, all nine tween updates and both generated-strip
paths. Its full12056-byte owner remains non-exact. Physical views and compiler
observations do not establish original class identities, authorship or TU layout.

## Active focus lock (2026-11-24)

The sole active function is `ExecuteAnimationVm` at `0x0044B4B0`, with the
complete 12,056-byte physical owner ending at `0x0044E3C7`. No other function
becomes the focus until this owner, including its 104-cell dispatch table, is
reconstructed as natural C++ and its exactness is accepted. Helper implementations
may be changed only as dependencies of this locked function; that does not open
a second function track.

## Current status (2026-11-25)

The focused `animation-execution-tween-context` build was rebuilt after the
TH10-style instruction-cursor loop, result-lifetime reduction and VM-state
aliasing experiments. The current natural-C++ candidate owns 11,960 bytes and
3,355 decoded instructions against the native 12,056-byte owner and 3,308
instructions. The complete-extent mnemonic/operand-key comparison aligns
1,396 native instruction keys; calls are 261/262 and conditional branches are
379/406 (candidate/native). These are diagnostic measurements, not an exactness
score. Removing the named `scaleResult` pair temporary first raised alignment
from 1,382 to 1,389 keys. Routing the interpreter, opcode handlers and update
body consistently through the single `self` state alias raised it again to
1,396 keys and removed two decoded instructions without changing owner extent.
The candidate frame remains `0xDC` rather than native `0xCC`, and its prologue
still assigns the VM pointer to EBX and the instruction cursor to EDI, opposite
the native EDI/EBX allocation. The instruction cursor/register assignment and
complete 104-entry dispatch table remain open.

The VM event word at `+0x37C` is now declared `unsigned short`. Native entry
loads this field with `movzx` before testing and carrying the event through the
interrupt scan, so the unsigned type models the observed zero-extension rather
than sign-extending negative 16-bit values. This is a field-type correction,
not proof of the surrounding VM class identity or whole-owner exactness.

Child-creation opcodes 88, 90, 91, 92 and 95 now retain the full 32-bit script
operand read by `ReadI`; the previous `short` forms were rejected because they
truncate script IDs before the factory call. For opcodes 95, 96 and 97,
`const AnimationVmId &` binds the factory's hidden-result temporary through
`ClearMissingAnimationId`, which may clear the underlying non-const temporary
in place. This keeps the factory and cleanup calls adjacent and avoids copying
the returned ID into a second buffer. Opcode 96 still inserts the child into
the draw-related list before writing its position; opcode 97 does not insert it
and writes the decoded x/y position and translation. Opcode 95 copies the
parent position and translation. The code remains natural C++; no ASM, padding
or placeholder bytes were introduced.

The quad-mode helper is currently expressed as the natural C++ member
`void __fastcall AnimationVmView::BuildModeQuad(CameraVector *quad)`. It writes
the four output corners directly, selecting the standard, rotated or unrounded
quad builder from the VM mode flags. The candidate helper owns 100 bytes. The
native opcode-98 call site passes the VM in EAX and the 48-byte corner buffer
in ECX; this candidate passes the VM in EDX and the output buffer in EAX. The
helper boundary and its register assignment therefore remain non-exact, but
this output-buffer form is closer to the target call shape than the rejected
aggregate-return experiment. No register-forcing shim was introduced.

The separate `animation-integer-destination` leaf was rebuilt in the same
session: all 104 bytes match after replaying its 11 reviewed local jump-table
relocations (`difference_count = 0`). This is a focused leaf replay, not proof
that the enclosing interpreter or original source-TU ownership is exact.

The builds were limited to the selected VM owner and the integer-destination
leaf plus their direct dependencies:

```bash
scripts/repo-python scripts/build-match-unit.py --check
scripts/repo-python scripts/build-match-unit.py --unit animation-execution-tween-context
scripts/repo-python .analysis/compare-execute-animation-vm-20261122.py build/match-linked/AnimationExecutionTweenContext
scripts/repo-python scripts/build-match-unit.py --unit animation-integer-destination
scripts/repo-python scripts/compare-linked-function.py --unit animation-integer-destination
```

This remains a focused diagnostic frontier, not a full cold replay or an
accepted exact-owner result. Original class identities, source-TU provenance,
complete dispatch-table recovery and canonical ownership remain open.


## Historical status (2026-11-24)

The current reproducible boss diagnostic is
`animation-execution-tween-context`. The full linked candidate owns 11,972 bytes at
`0x00401870`: 11,556 bytes of executable code followed by the 416-byte,
104-entry dispatch table. The native owner has 11,640 executable bytes plus
the same table. The code-only decoder sees 3,183 candidate instructions versus
3,203 native instructions; 1,324 native instruction keys align under the
mnemonic/operand-key comparison without register rewriting. The native/candidate
call counts are 262/261, conditional-branch counts 259/260, and unconditional
jump counts 147/117. Only 4/104 dispatch entries have matching relative handler
offsets. A diagnostic EBX/EDI label swap raises normalized instruction
alignment to 2,384/3,203; this identifies the major register-allocation
inversion, not an exactness result or permission to rename registers in the
comparison. These are diagnostic counts, not an exactness score. The older
3,308/3,319 totals decoded dispatch-table bytes as instructions and are
superseded.

Focused handler counts (native / candidate) are opcode 96: 38 / 47, opcode 97:
35 / 45, opcode 98: 19 / 19, and opcode 100: 124 / 124. Equal instruction
counts do not imply matching instruction streams.

Opcode 100 (the position Bézier-tween initializer) now aligns all 124/124
instructions after the same EBX/EDI register-label normalization. The source
keeps the three float reads in native order, materializes the X result in its
own local, then initializes the target vector from the three locals. That
restores the native x87 spill/reload pair which disappeared when the X read was
embedded directly in the aggregate initializer. This is a per-handler
instruction-key result, not a byte-exact or whole-function result.

Opcode 96 and opcode 97 each retain their own two `ReadAnimationFloat` call
sites rather than tail-merging the decoder calls. Both child handlers hold the
factory result in a local `AnimationVmId` object and pass its value address to
`ClearMissingAnimationId`; this lets VC9 use each local as the hidden result
buffer and keeps the factory/clear calls adjacent, instead of copying the
returned integer through the shared scratch union. The returned child is now
viewed as an `AnimationExecutionView *`, so the child position/translation
fields are accessed directly in natural C++ rather than repeatedly casting
through the VM base view. In opcode 97, this changes the emitted order to load
the first x87 operand, test its variable bit, and only then move the returned
child pointer into ESI, matching the native order at that point. The complete
opcode handler is still non-exact: stack slots, later x87 spills, translation
copy placement, and the enclosing VM/instruction register allocation differ.

The update-mode quad helper is still an ABI mismatch. Native callsite
`0x0044D9B2` passes the VM in EAX and the 48-byte corner buffer in ECX; the
current natural C++ output-object member helper uses EDX for the VM and EAX
for the output object. Its semantics are represented, but its call boundary
and the helper body are not exact. The selected unit cold-builds successfully
with the pinned VC9 toolchain, but remains a non-runnable diagnostic frontier:
its linkage manifest is not exhaustive and there is no accepted full-owner
replay.

Native entry saves EBX/ESI/EDI, loads the stack VM pointer into EDI, and reserves
an aligned `0xCC` frame. The current candidate loads the VM pointer into EBX
and uses EDI for the instruction pointer, with a `0xDC` frame. A diagnostic
register-label swap raises normalized instruction alignment to 2,384/3,203;
this identifies the EBX/EDI allocation inversion as a major source of mismatch,
not an exactness result or permission to rename registers in the comparison.

The same linked image owns `StepAnimationFloatPairTween` at candidate
`0x00404D00`, exactly 673 bytes / 221 instructions. After replaying its four
reviewed relocation fields at `+19`, `+70`, `+363` and `+579`, only 2 bytes
differ and all 221/221 instruction keys align. Natural vector operators now
match the mode-8 x87 schedule; the remaining two bytes are the mode-17
Y-component add operand order. The pair leaf remains frontier-only and
non-exact.

`AnimationFloatDestination` now lives in `AnimationExecutionArguments.cpp` as
a public natural-C++ leaf. The isolated `animation-float-destination` linked
unit reproduces the complete 124-byte owner, replays all 14 reviewed relocation
fields, and reports zero byte differences. A broader interpreter LTCG context
emitted a 140-byte PDB contribution for the same function under a different
support profile, so the isolated context remains the canonical leaf check;
full-owner integration is measured separately. Its function-inventory status
remains unclassified and the replay is not yet an accepted canonical
exact-ledger row.

`AnimationIntegerDestination` is retained in the interpreter TU as an internal
natural-C++ helper. Its dedicated `animation-integer-destination` unit uses the
full interpreter LTCG dependency context and reproduces the complete 104-byte
owner after replaying its 11 reviewed local jump-table relocations; the result
is zero byte differences. The emitted EAX encoded-pointer / EDX VM / EAX result
convention comes from the original TU's compiler-selected internal calling
convention, not a hand-written ABI shim. The helper remains origin-indeterminate,
its function-inventory status remains unclassified, and it is not promoted to
canonical exact inventory solely by this linked replay. `BindAnimationSprite` is
likewise source-present and non-exact (see ASCII_LIFECYCLE.md and
MENU_JOYSTICK.md). Complete extent/PDB ownership is not exactness.

`UpdateStageRealInterpreterAnimationOy` is stale: its receipt records an older
`src/AnimationTween.cpp` hash (`b527a00b...`) than the current source. Do not
quote its 11,552-byte interpreter as a current full-Stage result; rebuild that
frontier before using its measurements. The current focused context is built by:

```bash
scripts/repo-python scripts/build-match-unit.py --unit animation-execution-tween-context
scripts/repo-python .analysis/compare-execute-animation-vm-20261122.py build/match-linked/AnimationExecutionTweenContext
scripts/repo-python scripts/build-match-unit.py --unit animation-integer-destination
scripts/repo-python scripts/compare-linked-function.py --unit animation-integer-destination
```

The rejected 2026-11-23 component-expression experiment is preserved as
`.analysis/AnimationTween.mode8-components-20261123.cpp`. It built a 677-byte
pair owner with 219 normalized byte differences and 219/221 instruction keys
aligned, worse than the committed 673/33 baseline; maintained source is restored
to HEAD `1cc8b50`.

Older dated tables below are historical snapshots. Use this section and the
current linked receipt before treating any old size or alignment count as live.

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

`AnimationUnaryMathC` at `0x0042B6D0` is the opcode-44 tangent wrapper. Its
31-byte body loads the float argument from the stack, calls the pinned VC9
`__CItan` entry at `0x004869B0`, rounds the x87 result through an aligned
float local, reloads the rounded value and returns with `RET 4`. The Ghidra
signature reports `__fastcall`; as expected for a floating-point parameter on
this x86 ABI, the value is still stack-passed and callee-cleaned. Natural C++
`tan(double)` with an aligned float local emits the complete 31-byte owner and
the single `__CItan` REL32 at offset `+13`. Three native callers are reviewed;
there are no external interior references. The source is wired into the
interpreter diagnostic frontier, but exactness remains pending the targeted
linked comparison.

`AnimationExecutionPolarPoint::Set` at `0x0044EF80` is now maintained as
natural C++ in `AnimationExecutionPolarPoint.cpp`. The complete 30-byte target
body is a single `FSINCOS`, two radius multiplies, two stores and `RET 8`; four
calls originate in the ring/arc strip loops, with no external interior entry.
The natural C++ implementation uses double-precision `cos` and `sin`, with a
volatile output-pointer spill so the public `__thiscall` receiver remains in
ECX across both CRT calls. A local frame-pointer-omission pragma keeps the
normal-COFF body at 45 bytes in both isolated and full Stage Update contexts;
all four interpreter call sites use the native ECX receiver plus two stack
float arguments. This replaces the earlier LTCG-private EAX/ESI receiver
variants. The target's single `FSINCOS` still makes its body 30 bytes, so the
helper remains non-exact; no assembly or padding is used.

The 2026-11-22 full Stage Update diagnostic (historical; its linked receipt is
now stale after `AnimationTween.cpp` changed) recorded an 11,520-byte natural
C++ `ExecuteAnimationVm` owner against the 12,056-byte native body. Its
disassembly had 3,351 instructions, 964 aligned native instruction forms, 262
calls and 376 conditional branches. These are archived measurements, not a
current full-Stage result or an exact replay.

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

At HEAD `1cc8b50`, the source pair callee is673/frame28 versus native673/frame28.
After the four reviewed relocation fields replay, 33 bytes differ and 215/221
instruction keys align. Six paths each fill both buffer words and return the
buffer pointer. Named final pair values restore the observed temporary-to-result
DWORD copies; four independent float coefficients recover the frame. The
target/source7/17/8/ordinary paths have23/35/93/38 versus23/35/92/38 instructions
and2/4/19/7 float-round stores. The remaining strict-order differences are in
mode17 component addition and mode8 x87 scheduling; no floating exception/status
or full-function semantic proof is claimed. The first Hermite coefficient still
squares t-1 before multiplying by t+t+1, as directly reviewed in all four
interpolation bodies.
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
result-buffer home, temporary/vector lifetimes and the remaining call-site
differences. Float destinations for17/27/45/46 share a native tail; the source
now uses one natural C++ result local and common destination writeback for
exactly those four opcodes, leaving assignment7 on its separate path. This
restores the native15 AnimationFloatDestination calls (the earlier source had
18). Child creation also uses two separate shared C++ tails: opcodes88/91 and
90/92. Each pair shares its native ClearMissingAnimationId and
InsertAnimationRelatedNode calls. Integer random40 now assigns its zero result
inside each RNG branch, which restores the native fourth RandomNext32 call.
Preserve complete extents and /GL; no volatile, fake returns,
private-register declarations, padding or cropped extents are used.

## 2026-11-22 shared float-result and child-creation tails

The four float-result handlers converge on one C++ writeback label after their
original operand reads and calculations. Child creation now has two distinct
shared tails: opcode88 and91 share the draw-child publication path, while
opcode90 and92 share the front-child publication path. Each pair keeps its own
function-scope result word so LTCG does not incorrectly fold the two distinct
native tails together. The two shared paths restore the native helper call
counts without deleting any opcode handler or changing its payload.

The full stage-update graph was rebuilt with the AnimationExecution.cpp
support translation unit explicitly using VC9 /Oy- /Ob2; the isolated
interpreter unit uses the same profile for comparison. Both receipts are
current and verified.

| Context | PDB-owned interpreter | Instructions | Aligned target keys |
| --- | ---: | ---: | ---: |
| Full stage-update graph | 11,504 B | 3,173 | 765/3,308 |
| Isolated interpreter | 10,672 B | 3,144 | 641/3,308 |
| Native | 12,056 B | 3,308 | 3,308 |

The full graph's call-site census now has15/15 float-destination calls,
78/78 float reads,76/76 integer reads,13/13 integer destinations,4/4 VM-ID
creations,4/4 timer setups,6/6 AddAngles calls,5/5 ClearMissingAnimationId
calls and3/3 InsertAnimationRelatedNode calls. The remaining named delta is
RandomNext32 3/4 (candidate/native). Direct call instructions total258/259 and
branch mnemonics375/406. These are static code-shape diagnostics, not semantic
or exactness proof.

The full-graph owner is now552 bytes shorter than native and aligns22 more
instruction keys than the preceding shared-float-only checkpoint. Its frame
is0xE0, it does not emit the native stack-alignment instruction, and the VM
pointer remains in EBX rather than EDI. The isolated candidate emits the
native eight-byte stack-alignment instruction, but its frame is0xDC rather
than0xCC and it also keeps the VM pointer in EBX. The different prologues show
that whole-graph LTCG context changes frame/register decisions. Neither
candidate has the native complete extent, and neither receives exact credit.
No bytes are padded to close the size gap.

## 2026-11-22 split random-integer zero results

Opcode 40 chooses the scene or text RNG from the VM flag and returns zero for
a zero maximum. Declaring the result uninitialised and assigning `0` in each
branch's `else`, instead of initialising it once before the branch, stops
VC9 from folding the two RNG paths into one call. This restores the native
fourth RandomNext32 call site.

Measured with `.analysis/compare-execute-animation-vm-20261122.py`, which
aligns mnemonic-plus-operand keys over the full 12,056-byte extent after
replacing every hexadecimal immediate. Its counts are not comparable with the
key totals in earlier tables, so both columns below come from the same
script and the same build settings:

| Context | HEAD 5a688d6 | Split zero results |
| --- | ---: | ---: |
| Full graph size | 11,504 B | 11,520 B |
| Full graph aligned keys | 949/3,308 | 964/3,308 |
| Full graph `call` mnemonics (candidate/native) | 261/262 | 262/262 |
| Isolated size | 10,672 B | 10,672 B |
| Isolated aligned keys | 1,171/3,308 | 1,173/3,308 |

The full graph's call count now equals native. The frame (0xE0 full, 0xDC
isolated, native 0xCC), the missing stack alignment in the full graph and the
EBX-held VM pointer are unchanged.

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

AnimationRandomSignedRange is now independently linked and accepted as the
complete 56-byte owner at4070A0. Its caller is ReadAnimationFloat case10010 at
44B18F, which chooses scene/text RNG from the VM flag and supplies PI. The
isolated context compiles the real ReadAnimationFloat caller together with
RandomNext32 and the signed-range helper; this reproduces the target private
ESI state/one stack-float ABI without register annotations. The four reviewed
fields (one REL32 call and three x87 literal references) replay with zero byte
differences across the complete PDB-owned extent. The original 4070A0..4070D7
body ends in RET4; eight CC bytes precede the separate 4070E0 function. This
proves the source/codegen match, not original TU or class ownership.

All three random-conversion leaves now have independent canonical linked-PE
units and complete public primary-TU PDB extents:

| Function | Target extent | Replayed relocation fields | Result |
| --- | ---: | ---: | --- |
| AnimationRandomUnit | 36 B | 3 | 0 differences |
| AnimationRandomSignedUnit | 42 B | 4 | 0 differences |
| AnimationRandomSignedRange | 56 B | 4 | 0 differences |

The unit and signed-unit boundaries are reviewed at 0x458DA0..0x458DC3 and
0x458DD0..0x458DF9. Raw reference scans identify their direct callers and find
no interior control-flow or absolute-pointer references. The signed-range
boundary is reviewed at 0x4070A0..0x4070D7, with its sole direct caller at
0x44B18F and no interior references.

The non-runnable caller-context probe enters through ReadAnimationFloat rather
than calling these helpers directly. This lets LTCG infer the private ESI
random-state input and, for signed-range, the one stack float/RET4 ABI. A
synthetic direct signed-range caller instead produced a 64-byte ordinary
stack-argument function and was rejected. The probes grant no runtime
execution or original-TU authorship claim; exactness comes from the complete
56/42/36-byte linked-owner replays and all reviewed relocation fields.

The argument-reader leaf also improved:

| Function | Native extent | Current candidate | Aligned instruction keys |
| --- | ---: | ---: | ---: |
| ReadAnimationFloat | 540 B / 132 code ins | 540 B / 132 code ins | 128/132 |
| ReadAnimationInteger | 219 B / 33 code ins | 219 B / 33 code ins | 32/33 |
| AnimationFloatDestination | 124 B / 22 code ins | 124 B / 22 code ins | 21/22 |
| AnimationIntegerDestination | 104 B / 17 code ins | 104 B / 17 code ins | 16/17 |

ReadAnimationFloat now has the native entry sequence
fld [esp+4], push esi, fld st(0), mov esi,eax. Its 23-entry jump table has the
same relative case offsets as the target, including the non-monotonic random
case order 10011, 10012, then 10010. The code region is 448 bytes / 132
instructions on both sides, followed by the 92-byte jump table. The three
random paths now share the native float spill/reload return shape and call the
real signed-range, unit and signed-unit C++ helpers. The remaining four
code-only differences are localized to the signed-range path: the candidate
selects the random-state address before loading the 3.1415927f argument, while
the target loads that constant first. This is a diagnostic match, not an exact
leaf claim.

At the earlier baseline, before the event-scan source spelling experiment, the
complete interpreter owner was 12,084 B with 1,050/3,308 target instruction
keys aligned. The argument-reader code regions are now measured without
disassembling their jump tables as instructions: the integer reader is
32/33, the float destination is 21/22, and the integer destination is 16/17.
For each of these three leaves, the only unmatched code key is the absolute
jump-table base; all case bodies and default returns align after relocation
normalization. Their full function extents and trailing data remain intact.

A source-only event-scan comparison-order simplification now produces a
12,372-byte full interpreter owner, with 1,114/3,308 whole-extent instruction
keys aligned (the full owner disassembly contains 3,475 candidate keys). The
candidate has 262 call mnemonics, equal to the target, and 384 branch mnemonics
versus 406. This improves the diagnostic key alignment over the earlier
12,084-byte baseline, but also grows the owner by 288 bytes; it is a compiler
experiment, not an unconditional size improvement or an exactness claim. The
event scan still reloads the event field during the loop and uses different
registers from the native sequence.

The main owner remains open. The current full-graph candidate establishes
EBP as its frame pointer, reserves 0xE0 bytes without the native eight-byte
ESP alignment, and keeps the VM pointer in EBX. The native owner aligns ESP
to eight bytes, reserves 0xCC bytes and keeps the VM pointer in EDI.

### Two-float tween result lifetime

ExecuteAnimationVm now gives the scale-tween result an explicit natural C++
lifetime in the caller. The named AnimationFloatPair scaleResult is passed
through the compiler's ordinary hidden structure-return parameter, then copied
into self.scale; it is not an ABI shim or byte patch.

The 2026-11-22 full Stage-linked snapshot showed the StepAnimationFloatPairTween
call using the native private register roles: EDI addresses scaleTween, EBX
carries the hidden result buffer, and EAX returns its address; the caller then
copies the two float members and sets the scale flag. The native call at
0x44DBEC uses the same EDI/EBX/EAX roles. That old candidate call was at
0x41B25D in an 11,552-byte ExecuteAnimationVm contribution, but its receipt is
now stale after AnimationTween.cpp changed. Treat this as historical ABI
evidence, not a current call-site address or full-owner match.

### Full tween-context diagnostic

The dedicated `animation-execution-tween-context` frontier compiles the
interpreter together with the maintained `AnimationTween.cpp`, `StageTween.cpp`
and `StageEasing.cpp` sources. The tween TU is built with frame-pointer omission
(`/Oy`) while the interpreter retains its own profile. This is a real linked
C++ dependency context, not an opaque helper declaration.

PDB contribution ownership independently identifies the complete
`StepAnimationFloatPairTween` body at candidate `0x00404760`, exactly 673 bytes
and 221 instructions, matching the native extent `0x0044E910..0x0044EBB0`.
Its four relocation fields are the timer call at `+19`, time-scale address at
`+70`, double literal at `+363`, and factor call at `+579`. At HEAD `1cc8b50`,
replaying those fields leaves 33 differing bytes; 215/221 instruction keys
align. The remaining differences include the mode-17 component-add order and
mode-8 x87 operand scheduling. This leaf is still non-exact.

In this same linked context the complete PDB-owned interpreter is 11,636 bytes
with 3,423 candidate instructions, 1,139/3,308 native instruction keys aligned,
258/262 calls and 375/406 conditional branches. Its prologue aligns ESP to
eight bytes and reserves `0xD4`, but keeps the VM argument in ESI; native uses
EDI and a `0xCC` frame. These measurements make the dependency context
reproducible and expose the next boss-level gap; they do not grant exactness or
runtime credit. Build it with:

    scripts/repo-python scripts/build-match-unit.py --unit animation-execution-tween-context

The linked image is still a diagnostic graph with unrelated dependency anchors;
it is not runnable and earns no exactness credit. No ASM, byte stubs, padding,
or clipped extents are used. The targeted incremental link reuses the other
52 support objects and recompiles only the affected translation unit.

Build the bounded frontier with:

    scripts/repo-python scripts/build-match-unit.py --unit update-stage-real-interpreter-animation-oy
