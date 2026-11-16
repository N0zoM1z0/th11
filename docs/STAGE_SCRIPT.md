# Stage script interpreter and fog packing

The original Japanese v1.00a interpreter now has complete maintained source in
`StageScript.cpp`. Direct local Ghidra review attested the entire mapped text
section and reconciled its full extent, instruction flow, callers and tables.

| Function | Full target owner | Source contribution | State |
| --- | ---: | ---: | --- |
| ExecuteStageScript,0x00404470 | 2,736 bytes through0x00404F1F | 2,744 bytes | Non-exact; EH ownership open |
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

The current execute-stage-script-frontier diagnostic build contributes
2,744 bytes for ExecuteStageScript, 8 bytes larger than the 2,736-byte
target owner. Its decoded switch body is 2,644 bytes versus the target's
2,633-byte body; the candidate frame reserves 0xb0 bytes versus the target's
0x98. A normalized instruction-key sequence comparison aligns 373 of the
target's 609 instructions. These are diagnostic measurements, not exactness.
The linked receipt's src/StageScript.cpp SHA-256 is
15f343bdd494cdecc66f75f7d6a5e99286e690c115b59f36d7e7ff9b0f113cc6,
matching the maintained source. The interpreter remains non-exact and
EH/funclet ownership remains open.

The frontier now compiles `src/StageMesh.cpp` and
`probes/stage-mesh-context.cpp` in their non-`/Oy-` profile. The probe keeps a
dynamic constructor call in the LTCG graph, preventing the Boss's constant
`flags == 0` call sites from specializing the shared constructor. The
StageScriptMesh constructor now contributes 366 bytes rather than the
237-byte specialized body; its generic ESI/EAX/stack argument ABI matches the
native caller shape, including `ret 8`. The constructor remains 20 bytes short
of its 389-byte target owner and is not exact.

The current source snapshots the camera vector for opcode 2 and writes the
new-minus-old delta through raw DWORD copies. Opcodes 3 and 5 use one
CameraVector argument temporary; opcodes 10 and 11 use three CameraVector
temporaries for the Bezier control/target groups. Opcode 9 has its own local
fog value, and the postpass has a separate result temporary. Opcode 17 uses
the ordinary C++ ReleaseVmBindingBlock call; LTCG recovers the private ESI
receiver ABI in the linked caller. The phase assignments remain ordered as
`phaseY = phaseX = phase`. These are source/codegen hypotheses, not exactness
claims.

In this same linked image, PackStageFog remains an exact 171-byte
contribution; StepStageVectorTween is 887 bytes against 883 target bytes,
and StepStageFogTween is 794 bytes against 737 target bytes. A separate wide
secondary context retains the exact 737-byte fog-tween proof. An older 2,744-byte/0x68-frame and 303/610-instruction measurement describes
an unrelated earlier candidate; the current 2,744-byte result has a 0xb0
frame and 373/609 normalized instruction-key alignments. Do not conflate the
two. The interpreter's unresolved EH/funclet context and remaining
code-generation differences receive no exact credit.

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
