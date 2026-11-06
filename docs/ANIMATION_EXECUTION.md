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
the whole VM, not a falsely declared tween pointer. These twelve small source
mappings are direct dependencies of the full12056-byte owner and receive no
independent leaf acceptance units or exact credit.

Existing tween, angle, random, sprite, fan, ID creation and rendering bodies are
used in the cold family. Polar44EF80, front-draw factory456020 and the reviewed
random/unary wrappers still have source-open bodies. Their declarations do not
grant source or exact credit. HUD now calls existing ExecuteAnimationVm rather
than the older void prototype name for the same target.

## Complete comparison and remaining gaps

The maintained full cold /GL primary contribution is12884 bytes with frameE0 and stack
VM input, but lacks native8-byte stack alignment and uses EBP for VM versus EDI.
All104 table cells and403 independently reviewed address/table fields replay;
12471 bytes differ over the complete owner, including the length difference.
There are262 native versus257 source call sites. Physical decreases are float
read78/77, RNG4/3, missing-ID lookup5/4, related-node insertion3/2 and malloc2/1.
They are not missing instruction implementations or accepted byte matches.

All104 dispatch entries have equal static per-op call/instruction-loop/post-
update/return skeletons:416 distinct paths on each side. Constant opcode choices
are propagated through the source's grouped factory choices; other predicates,
operand identities, aliasing and dynamic loop reentry remain unproved. This
check localizes physical call merging but is not a runtime or semantic proof.

A direct predecrement spelling for instruction5 is byte-identical to the
wrapped subtraction helper: it still uses a long-livedEDI=-1 and memory ADD
versus native DEC. Repeating that spelling probe does not resolve the gap.

The real dependency family also retains HUD4652/frameC/4101 differences and all
27 calls. Dialogue stays6948 bytes but now has native frame140, with5357 full
replay differences versus earlier frame13C/5694; all454 fields,252 calls and46
static per-op traces remain. This is a compiler observation under a fuller
context, not proof of original TU ownership or whole-function equality.

Next gaps are the full interpreter frame/alignment, VM register, physical case
layout, source opcode caching, temporary/vector lifetimes and the five physical
call merges. Preserve complete extents and /GL; no volatile, fake returns,
private-register declarations, padding or cropped extents are used.

Private evidence uses animation-vm-update/arguments/operations/math Ghidra
exports, animation-execution-target-proof and complete owners-function/CFG
reports. Target/decompiler dumps, builds and mutable databases remain private.
