# Stage script interpreter and fog packing

The original Japanese v1.00a interpreter now has complete maintained source in
`StageScript.cpp`. Direct local Ghidra review attested the entire mapped text
section and reconciled its full extent, instruction flow, callers and tables.

| Function | Full target owner | Source contribution | State |
| --- | ---: | ---: | --- |
| ExecuteStageScript,0x00404470 | 2,736 bytes through0x00404F1F | 2,712 bytes | Non-exact; 578/610 aligned instructions; EH/funclet ownership open |
| SeekStageScriptMarker,0x00404F20 | 113 bytes through0x00404F90 | 113 bytes | Exact normal-COFF, 2 relocations |
| BeginSecondaryStageTransition,0x00405130 | 159 bytes through0x004051CE | 159 bytes | Exact normal-COFF, 6 relocations |
| BeginPrimaryStageTransition,0x004051D0 | 103 bytes through0x00405236 | 103 bytes | Exact normal-COFF, 3 relocations |
| InitializePrimaryStageScripts,0x00405030 | 249 bytes through0x00405128 | 246 bytes in focused linked frontier (237 normal-COFF) | Source present; non-exact |
| ResetAndBindStageAnimationVm,0x00405B10 | 30 bytes through0x00405B2D | 30 bytes | Exact linked-PE, 2 REL32 fields (see [STAGE_VM_BIND](STAGE_VM_BIND.md)) |
| PackStageFog,0x00405B70 | 171 bytes through0x00405C1A | 171 bytes | Exact; defined inline in StageTween.cpp (see [STAGE_TWEEN](STAGE_TWEEN.md)) |

The interpreter's Ghidra code body ends at404EB8 and contains2,633 bytes.
Three alignment bytes follow. Its18-entry instruction table occupies
404EBC..404F03 and its seven-entry oscillator table404F04..404F1F, totaling100
bytes. All25 apparent external interior references originate in these owned
tables and resolve inside the primary code body. The next function starts at
404F20 without intervening padding. The initial2,633-byte inventory extent was
incomplete; the ledger now includes both tables. Separate EH handler/cleanup
ownership remains outside the exact lane. No target bytes or tables are embedded
in the source; ordinary switches generate the compiler's tables.

## Adjacent exact marker-seek helper

The native `0x00404F20..0x00404F90` function is a separate 113-byte owner,
not part of the preceding interpreter's jump tables. Its single reviewed
caller is `ExecuteSecondaryActorInstruction` at `0x00412E30`. In the internal
fastcall ABI the first ECX value is unused, while EDX supplies a marker ID.
It always reads `g_StagePrimary` at `0x004A8D60`, scans the stage's
`section008` instructions until a negative-frame sentinel, and searches for
opcode 16 with the requested argument. On a match it publishes the *next*
instruction as the interpreter cursor, then initializes/sets the script timer
to that next instruction's frame (current, previous=frame-1 and float fraction).
An absent marker leaves the timer and cursor alone; there is no stream bound
or allocation fallback in the native control flow.

The maintained `src/StageScriptSeek.cpp` is ordinary C++ and has a separate
pinned VC9 normal-COFF exact proof via `seek-stage-script-marker`: **113/113
bytes and zero differences** after exhaustive replay of `g_StagePrimary` at
field `+3` and `g_AnimationTimeScale` at `+0x59`. This does not grant exactness
to `ExecuteStageScript`, nor does it establish the helper's original object
ownership or authored provenance.

## Adjacent exact animation VM adapter

`ResetAndBindStageAnimationVm` at `0x00405B10` has an independently proven,
complete **30-byte exact** VC9 `/GL` primary translation-unit contribution.
Its natural C++ uses the real `ResetAnimationVm` and `BindAnimationScript`
callees. The scoped producer recovers the private EAX VM, EDI document and
EBX script registers, VM+41D/+41C writes, and final tail jump without custom
ASM or binary patching. Two independently reviewed REL32 call fields at +4
and +26 replay to 401FD0 and 44ACD0 with zero differences. See
[STAGE_VM_BIND](STAGE_VM_BIND.md). The large caller at 40C650 remains non-exact.

## Primary stage object/VM initialization

`InitializePrimaryStageScripts` at `0x00405030..0x00405128` has a complete
natural C++ reconstruction in `src/StageScriptInitialization.cpp`. The native
routine is reached from `UpdateSecondaryGameplay` at `0x00420390`; it reads
`g_StagePrimary`, sets bit 1 (`0x2`) in each of the stage's update, draw and
overlay chain-element flags, then walks the signed 16-bit object count. Each
object has its active byte set to 1 at `+3`. Its variable-length VM command
stream starts at object `+0x1c`: a signed command type at `+0`, signed byte
stride at `+2`, signed script ID at `+4`, and written VM index at `+6`.
A negative type terminates the current object's stream; the subsequent object
continues global VM numbering.

Each nonnegative command resets the corresponding 0x434-byte VM, sets the two
`+0x41d`/`+0x41c` bytes to `0x10`, binds its script using the animation document
at stage `+0x178`, writes its sequential VM index, and steps by the signed
command byte stride. Finally, the main script cursor at stage `+0x4c` is set
to section008 at `+0x1c`, even when no objects were present. The native
routine introduces no extra object count/VM count guard, null fallback, or
cycle detection.

A focused canonical-graph diagnostic `initialize-primary-stage-scripts-frontier`
now links the real `src/AnimationVm.cpp` producer plus the callable
`probes/stage-script-initialization-context.cpp` under VC9 `/GL`. The resulting
function is **246 bytes versus 249 target**. Its two native private calls use
ESI for `ResetAnimationVm` and EDI / EBX / EAX for `BindAnimationScript`,
without any manually assigned registers or assembly. The natural C++ body now deliberately reloads `stage->objects[objectIndex]`
after marking the current object active, matching the target's second table
load rather than accidentally extending a local pointer's lifetime. Declaring
`scriptId` before `file` also restores the native script/file-load order. The
246-byte linked candidate matches **182 of 249 raw byte positions**;
inside its common instruction body at `+0x42..+0xDC`, after excluding the
reviewed global and call address fields, only **nine stack-slot displacement
bytes** remain different. This is diagnostic agreement, not a strict replay.
The remaining three-byte extent gap, stack frame `0x10` versus target `0x14`,
and early callee-save scheduling still preclude exactness. This reproducible linked frontier supersedes
the old standalone-only comparison, but does not prove the original producer
translation unit. Rebuild only the affected frontier with:

```bash
scripts/repo-python scripts/build-match-unit.py --unit initialize-primary-stage-scripts-frontier
```

A targeted pinned VC9 normal-COFF compilation yields a 237-byte code body,
compared with a 249-byte complete native extent (245 decoded code addresses).
The standalone compiler uses conventional stack calls while the target calls
`ResetAnimationVm` and `BindAnimationScript` through their private LTCG
register ABIs. This is **source/behavioral recovery only**, not an exact
match; original TU linkage and code generation remain open. No inline ASM,
instruction padding, or fabricated control-flow was used.

## Exact secondary stage screen transition (0x00405130)

`BeginSecondaryStageTransition` is the adjacent 159-byte original stage
handoff called by secondary gameplay. It loads `g_StageSecondary` at
`4A8D5C`, creates a real 0x44-byte `SecondaryHudScreenEffect` using its
existing C++ constructor, and on successful allocation initializes mode 2,
30-frame duration, zero colors and priority 10 via the genuine
`InitializeSecondaryHudScreenEffect` at `448A90`. Regardless of allocation
success, it conditionally initializes the secondary stage's timer at
`+0x2ff4`, sets current 30, previous 29, fraction 30.0f, and publishes the
transition bit `2` at stage `+0x2ff0`.

This is **159/159-byte pinned VC9 RTM normal-COFF strict exact** with all
six native address fields independently reviewed: the stage-secondary
pointer, C++ `operator new`, CRT `memset`, real screen-effect initializer,
shared animation time scale, and float 30.0f. It introduces no handwritten
register ABI, assembly, target bytes, or padding. Run only the selected unit:

```bash
scripts/repo-python scripts/replay-exact-units.py --unit begin-secondary-stage-transition
```

## Exact primary stage transition timer (0x004051D0)

`BeginPrimaryStageTransition` is a separate 103-byte original no-argument
stage timer helper called by secondary gameplay update. It initializes the
`StageResourceView::timer2ff4` only if its initialization bit is unset,
installing `g_AnimationTimeScale` and `-999999` as the previous-frame
sentinel. Regardless of initialization it publishes current frame 60, previous
frame 59, fraction 60.0f, and sets bit 2 of stage flags at `+0x2ff0`.
`src/StageTransitionTimer.cpp` expresses this directly as ordinary C++.

The pinned VC9 RTM `/O2` normal-COFF function is **103/103 strict exact**;
all three original `DIR32` fields are independently reviewed and replayed:
`+0x01` to `g_StagePrimary` at 4A8D60, `+0x35` to the time-scale global
at 4A7948, and `+0x41` to float constant 60.0f at 4981B8.
The following is the complete, narrowly scoped proof:

```bash
scripts/repo-python scripts/replay-exact-units.py --unit begin-primary-stage-transition
```

This does not confer exactness on the nearby 249-byte stage initializer,
2736-byte interpreter Boss, or any unrelated stage subsystem.

## Script state and execution

Instructions contain a signed32 frame, signed16 opcode, signed16 byte size and
variable32-bit arguments. The cursor is stage+4C, the script timer+38 and the
jump base is section008 at+1C. While an instruction's frame is at most the
timer current, the interpreter executes it. Ordinary advancement reloads the
current cursor and adds its signed byte size. Jump reloads its argument from
the current cursor after updating the timer and sets cursor to jump-base plus
the supplied offset. There are no stream bounds checks or cycle guards.

Reaching a future instruction advances the script timer once. Opcode0 keeps
the cursor and skips that timer advance. Both paths still run the interpolation
and oscillator postpass, and both return0. A stop instruction does not stop
camera interpolation or oscillation.

| Opcode | Behavior |
| ---: | --- |
| 0 | Stop dispatch without advancing cursor or script timer. |
| 1 | Set script timer from argument1, then jump to section008+argument0. Timer previous becomes frame-1, fraction becomes float(frame). |
| 2 | Set camera position and publish new-minus-old position in camera+F0. |
| 3 | Configure position interpolation: duration, mode and target vector. |
| 4 | Set camera target offset directly. |
| 5 | Configure target-offset interpolation. |
| 6 | Set camera up vector. |
| 7 | Set field of view. |
| 8 | Set packed fog color, expand its four bytes to float channels, and set fog start/end. |
| 9 | Configure fog interpolation from current fog to argument fog/color. |
| 10 | Configure cubic position interpolation, mode8, with two control vectors and target. Argument1 is unused. |
| 11 | Configure cubic target-offset interpolation in the same order. |
| 12 | Set oscillator from the low argument byte and reset its timer to-1. Zero also restores eye offset0 and up(0,1,0). |
| 13 | Write the argument DWORD to runtime+9C0; its broader meaning is open. |
| 14 | Bind/reset an embedded VM when script index is nonnegative; otherwise clear only VM flags mask01. |
| 15,16 | No operation beyond ordinary cursor advancement. |
| 17 | Release/delete an old mesh, reset distortion fields and allocate a new mesh. |
| Other | Ordinary cursor advancement, including negative opcodes. |

Target interpolation lives at stage+50 and position interpolation at+9C. Each
contains initial, target and two control vectors, a timer, duration and mode.
Fog interpolation starts at+E8, with four28-byte fog values, timer+158,
duration+16C and mode+170. A fog value is start/end, four byte-domain float
channels and the packed DWORD. Opcode 9 calls PackStageFog after populating
the four channels. Its x87 conversion is explicitly truncating: the native
helper ORs 0xC00 into the saved control word before FISTP, then restores the
original word. This is round-toward-zero, not nearest rounding, so the
ordinary C++ `static_cast<int>` in PackStageFog is semantically correct.
The existing stage layout is accessed through an additional physical view;
shared source headers and old exact contexts were not changed.

The postpass tests nonzero duration independently, applies target-offset,
position and fog interpolation in that order, then applies oscillation.
Modes 1, 5, 6 and 7 each advance the oscillator timer in their own switch arm,
compare the resulting signed current against their literal period (512, 2048,
1024, 512), reset it when reached, and return from that arm. Other modes
return without advancing. This matches the native per-mode control-flow shape
and avoids a synthetic shared period local. Interpolation routines remain
external dependencies, not source coverage. Vector/fog return values are
copied in full, including the packed fog DWORD.

Opcode14 uses signed argument0 with0x434 stride and has no eight-element check.
The animation document and script number are captured before ResetAnimationVm.
Bytes VM+41C/+41D become10 before BindAnimationScript. Cursor advancement still
reloads the stage cursor after these calls. Negative script numbers only clear
the active flag; they do not reset the VM.

Opcode17 captures the old binding before release/delete, then clears the slot.
It sets minimum radius112, radius192, word301C toFFFFFFFF and both phases to
NormalizeAngle(0). It reloads the mode from the current instruction afterward.
When minimum radius is positive it constructs a24-byte mesh with `(17,25,0)`
for mode1 and `(17,17,0)` otherwise. Null allocation publishes null. Source uses
ordinary C++ new expressions and their genuine exception cleanup; constructor
exceptions and complete EH unwind are not yet validated. The reviewed mesh
constructor at40E530 now has complete non-exact source; see
[STAGE_MESH.md](STAGE_MESH.md).

## Oscillation and numerical details

Only modes1,5,6,7 modify the camera and advance oscillator time. Modes2,3,4,
out-of-range values and zero leave that postpass unchanged. The phase uses
float-rounded pi promoted to double, `((time*pi)+(time*pi))/period - pi`, rounded
to float before CRT sine. The sine result is rounded to float before scaling.

| Mode | Period | Eye X | Up X | Extra |
| ---: | ---: | --- | --- | --- |
| 1 | 512 | -50*sine | -0.1f*sine | |
| 5 | 2048 | 70*sine | -0.1f*sine | Eye Z=200*sin(NormalizeAngle(float(phase+phase))). |
| 6 | 1024 | -50*sine | -0.1f*sine | |
| 7 | 512 | -15*sine | -0.01f*sine | |

Constants0.1f/0.01f are float-rounded doubles in the target; do not substitute
decimal double0.1/0.01. After advancing, a signed current at least the period
calls SetStageTimer(0). Direct position assignments retain their float operations
and signal-NaN quieting. Initial/tween aggregate copies retain their original
bit representations where the target uses integer copies.

Fog packing performs four float-to-signed32 conversions with truncation, stores
each integer's low byte, and restores the original x87 control word after each
conversion. It does not clamp to0..255 or normalize from0..1. Negative, oversized,
fractional, infinite and NaN inputs were checked against the actual bodies.

## Current code-generation frontier

The latest focused VC9 `/GL` build contributes **2,712 bytes** for the
2,736-byte `ExecuteStageScript` owner, with **578/610** normalized native
instruction keys aligned (610 candidate instructions). The owner remains
nonexact; byte/relocation equality and EH/funclet ownership are open.

Two 2026-11-19 natural-C++ changes produced the current result:

- Opcode 10 and 11 now make their own `ScriptTweenFrame` call rather than
  jumping backward into the opcode 3/5 timer blocks. This restored the native
  per-case timer-initialization layout and improved alignment from 551/610
  to 573/610 at the 2,720-byte intermediate checkpoint.
- Both oscillator paths now express the double angle as `angle * 2.0`
  instead of `angle + angle`. This preserves the mathematics and improves the
  complete owner to 2,712 bytes and 577/610 aligned keys.
- Opcode 17 now places a compiler read/write barrier between phase
  normalization and the mode read from the live instruction cursor. This
  prevents VC9 from hoisting the cursor load ahead of `NormalizeAngle`, as the
  native body reloads it afterward; focused normalized alignment advances
  from 577/610 to **578/610** without changing the 2,712-byte contribution or
  introducing handwritten instructions. Remaining opcode-17 instruction
  scheduling and exception-handler ownership are still unresolved.

The current comparison is
`.analysis/stage-script-double-angle-combined-20261119.diff`. The changed
source uses ordinary C++; no target instructions or padding are inserted.
The StageTween-family exact dependencies were selectively rebuilt and
compared after the source change; see the retained affected-unit logs.


The 2026-11-18 opcode-8 revision explicitly snapshots the four unsigned-byte
packed color channels into signed int locals before converting to float.
This restores more of the native load-all-then-FILD sequence while preserving
the original 0..255 range, the four float channels, and the packed field.
An unsigned int local instead adds inappropriate unsigned-to-float handling
in VC9 and increases the candidate by 28 bytes. The old checkpoint was
2,728 bytes and 531/610 aligned instructions; this one is 2,736 bytes and
537/610, with no ASM or artificial byte padding.

The frontier now compiles `src/StageMesh.cpp` and
`probes/stage-mesh-context.cpp` in their non-`/Oy-` profile. The probe keeps a
dynamic constructor call in the LTCG graph, preventing the Boss's constant
`flags == 0` call sites from specializing the shared constructor. The
StageScriptMesh constructor now contributes 366 bytes rather than the
237-byte specialized body; its generic ESI/EAX/stack argument ABI matches the
native caller shape, including `ret 8`. The constructor remains 20 bytes short
of its 389-byte target owner and is not exact.

The opcode-14 case now branches on the signed script ID **before** computing
its VM receiver. The positive branch loads its file and calculates the VM
pointer before `ResetAnimationVm`, then writes `+0x41d` and `+0x41c` in
native order and binds the script. The negative branch separately computes
the receiver and clears only its active flag. Under normal pinned VC9 `/GL`
code generation this natural control-flow change preserves the 0x98 stack
frame, increases switch-body extent by 16 bytes and improves normalized
instruction alignment from **451 to 526 target keys**; in particular the
positive/negative VM branches and both private VM calls now track the native
register assignment. No target bytes, ASM or padding were introduced.

Opcode 2 now reuses the already-live function-scope `bezierA`
`CameraVector` as its movement-delta temporary. All three float subtractions
are evaluated before the camera update-offset vector is assigned, mirroring
more of the original x87/register scheduling. In the scoped 2,736-byte
primary Boss frontier this advances normalized instruction alignment from
526/610 to **529/610**, without changing the `0x98` stack reserve or the
2,728-byte PDB contribution. The underlying stage behavior is unchanged:
each Bezier case overwrites the scratch vector before using it. No ASM,
opaque bytes or inert source was introduced. Seven affected StageTween
primary-context exact functions were rebuilt once and compared successfully;
two very wide indirect support contexts are not revalidated, as one required
link exceeded the bounded 900-second run.

Opcodes 3 and 5 now share the inline `ScriptTweenFrame` helper, which uses
compiler barriers to preserve the observable order of timer-field writes.
At the earlier checkpoint the pinned VC9 frontier remained 2,728 bytes
with the native 0x98 stack frame and had improved alignment from 529 to **531/610**. One instruction
ordering difference remains: the candidate stores the time-scale pointer
before `OR flags,1`, while native performs the OR first. The helper is a
codegen experiment, not an exact timer reconstruction. Seven neighboring
StageTween exact functions were rebuilt and compared with zero differences.
The wide menu build exceeded its 900-second bound; the wide secondary
clearing context was not rerun, so neither is revalidated after this change.

The current source snapshots the camera vector for opcode 2 and writes the
new-minus-old components through a shared CameraVector scratch copy. Opcodes 3 and 5 retain
case-local CameraVector argument temporaries. Opcodes 10 and 11 share two
function-scope CameraVector temporaries for the Bezier control1/target pair;
control2 remains case-local. This reduces the Boss frame from 0xb0 to the
target's 0x98 without changing the script's data flow. Opcode 9 has its own
local fog value, and the postpass has a separate result temporary. Opcode 17
uses the ordinary C++ ReleaseVmBindingBlock call; LTCG recovers the private
ESI receiver ABI in the linked caller. The phase assignments remain ordered
as `phaseY = phaseX = phase`. These are source/codegen hypotheses, not
exactness claims.

In this same StageScriptFrontier linked image, PackStageFog remains an
exact 171-byte contribution; StepStageVectorTween contributes 887 bytes
against its 883-byte target, and StepStageFogTween contributes 794 bytes
against 737 target bytes. These are context-specific contributions, not the
standalone StageTween-family measurements. The independent StageTween
producer now contributes an 883-byte StepStageVectorTween candidate
(278/280 normalized instruction keys), still non-exact. A separate wide
secondary context retains the exact 737-byte fog-tween proof. The prior
2,744-byte candidate (0xb0 frame, 373/609 aligned instructions) is
superseded by the 2,728-byte/0x98-frame Boss candidate above. An older
2,744-byte/0x68-frame and 303/610-instruction measurement describes a
different earlier candidate; do not conflate those results with the current
frontier. The interpreter's unresolved EH/funclet context and remaining
code-generation differences
receive no exact credit.

Fog packing is a non-EH public primary-TU PDB contribution. A cold pinned VC9
build has identical171 bytes and no linked address fields. PE/MAP/PDB identity,
owner agreement, source/tool/producer receipts, complete target extent and
exhaustive field replay are checked by the canonical linked Oracle:

```bash
scripts/repo-python scripts/replay-exact-units.py --unit pack-stage-fog
```

Since 2026-11-14 `Pack` is defined `inline` in `src/StageTween.cpp`, matching
its native placement in the tween region, and its canonical family is
`build/match-linked/StageTween` (entry `StageTweenProbe@24`). The interpreter
still calls it out of line through the plain header declaration, as native
does. See [STAGE_TWEEN](STAGE_TWEEN.md) for why the inline definition is
needed by the fog operators.
