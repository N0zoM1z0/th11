# Stage script interpreter and fog packing

The original Japanese v1.00a interpreter now has complete maintained source in
`StageScript.cpp`. Direct local Ghidra review attested the entire mapped text
section and reconciled its full extent, instruction flow, callers and tables.

| Function | Full target owner | Source contribution | State |
| --- | ---: | ---: | --- |
| ExecuteStageScript,0x00404470 | 2,736 bytes through0x00404F1F | 2,728 bytes | Non-exact; EH ownership open |
| PackStageFog,0x00405B70 | 171 bytes through0x00405C1A | 171 bytes | Exact linked-PE unit `pack-stage-fog` |

The interpreter's Ghidra code body ends at404EB8 and contains2,633 bytes.
Three alignment bytes follow. Its18-entry instruction table occupies
404EBC..404F03 and its seven-entry oscillator table404F04..404F1F, totaling100
bytes. All25 apparent external interior references originate in these owned
tables and resolve inside the primary code body. The next function starts at
404F20 without intervening padding. The initial2,633-byte inventory extent was
incomplete; the ledger now includes both tables. Separate EH handler/cleanup
ownership remains outside the exact lane. No target bytes or tables are embedded
in the source; ordinary switches generate the compiler's tables.

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
channels and the packed DWORD. The existing stage layout is accessed through
an additional physical view; shared source headers and old exact contexts were
not changed.

The postpass tests nonzero duration independently, applies target-offset,
position and fog interpolation in that order, then applies oscillation.
Interpolation routines remain external dependencies, not source coverage.
Vector/fog return values are copied in full, including the packed fog DWORD.

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

## Code generation and proofs

The current real-tween support context produces a2,728-byte primary
interpreter contribution with a0x68-byte local reservation versus native0x98.
The preceding opaque-tween context produced2,740 bytes. An initial
aggregate-return helper produced3,048 bytes/0x120; a hybrid with aggregate tween
vectors produced2,924 bytes/0xF8. Retained direct writes pass the same full-body
checks. Reversing the scheduling comparison operands did not recover the native
load order or improve the3,048-byte baseline and was reverted. An all-GL angle-math context produced2,724 bytes but removed the original
minimum-radius comparison by proving the callee could not change it. Using the
existing normal-COFF angle source restores that comparison while preserving
GL on the primary interpreter. This supports an opaque-callee hypothesis; it
does not establish the original TU partition. Current2,217
address-masked diagnostic positions differ; shifting instructions make this
count unsuitable as a completion percentage. The interpreter receives no exact
credit, including for its unresolved EH/funclet context.

Fog packing is a non-EH public primary-TU PDB contribution. A cold pinned VC9
build has identical171 bytes and no linked address fields. PE/MAP/PDB identity,
owner agreement, source/tool/producer receipts, complete target extent and
exhaustive field replay are checked by the canonical linked Oracle:

```bash
scripts/repo-python scripts/replay-exact-units.py --unit pack-stage-fog
```

Its canonical family is `build/match-linked/StageScript`, primary
`src/StageScript.cpp`, supporting `probes/stage-script-context.cpp`, `src/StageTween.cpp`,
`probes/stage-tween-context.cpp`, `src/StageEasing.cpp`, `src/StageMesh.cpp` and
`src/AngleMath.cpp` (normal COFF), entry `StageScriptProbe@12`, primary profile
`/TP /O2 /Ob1 /Gy /MT /Isrc /EHsc /GL`. The normal support keeps the same
switches except GL. The constructor and other unresolved
dependencies are diagnostic data anchors; this PE is not a runnable game.
