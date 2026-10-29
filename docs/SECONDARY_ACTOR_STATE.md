# Complete secondary actor state update

Sixteen new complete source mappings cover5,896 target bytes, including the full
4,230-byte state update and561-byte transition selector. Seven complete public
primary-TU owners pass294 exact bytes and14 independently reviewed address fields.
Thirteen new origin/boundary reviews remain indeterminate. Current totals are
2,097 candidates,416 reviewed boundaries,319 mappings and181 exact functions /
22,739 bytes. Origins are90 authored,314 indeterminate,10 exclusions and1,683 pending.

| Owner | Address | Full target bytes | Current result |
| --- | --- | ---: | --- |
| UpdateSecondaryActorState | 411750 | 4230 | Source4177/3219 diagnostic differences |
| SelectSecondaryActorTransition | 417B40 | 561 | Source521/497 |
| AdvanceSecondaryMotion | 459540 | 75 | Exact;2 fields |
| ComposeSecondaryMotion | 459590 | 215 | Source208/180 |
| OffsetSecondaryTimer | 459210 | 96 | Source96/6 |
| ClearMissingAnimationId | 456520 | 21 | Exact;2 fields; returns VM pointer |
| ValidateSecondaryAnimationId | 419A20 | 30 | Exact;2 fields; returns integer0/1 |
| SignalSecondaryAnimation | 4070E0 | 70 | Exact;2 fields |
| ReleaseSecondaryAnimation | 4562E0 | 66 | Source64/39 |
| PositionSecondaryAnimation | 4563A0 | 132 | Source126/104 |
| PositionSecondaryAnimationVm | 456430 | 110 | Source110/45 |
| PositionSecondaryAnimationRaw | 456330 | 97 | Source86/85 |
| SecondaryGlobalStatusBits | 410780 | 28 | Exact;1 field |
| AddSecondaryActorScore | 40CEB0 | 51 | Exact;3 fields |
| IncrementSecondaryCollisionCount | 40BB10 | 19 | Exact;2 fields |
| AddSecondaryCollisionValue | 40BAA0 | 95 | Source92/43 |

## Complete large-owner flow and compiler context

State update retains4230, including nine internal alignment bytes omitted from
the sparse4221-byte Ghidra body. Independent decoding covers1108 instructions and
five returns, with no table or foreign interior entry. Source implements all of:

- Processed4000 guard/publication and13-DWORD snapshot; four pair/two vector
  tweens, motion, scene offsets, composition/clamp and per-axis outside gates.
- Conditional animation scripts, script stepping and three callback contracts;
  damage modifiers/counter, transition restarts, score and terminal effects.
- Collision/timer gates, periodic effects/counters/sound, direction-dependent
  scripts, eight animation IDs, tracking, flashing and low-life status.
- Complete mesh row/column deformation, radius/fade progression, color-byte
  conversion, normalized3D displacement, sine offsets, Z/phase updates and timers.

Original double/float constants were decoded independently, including outside
bounds-192/192/0/448, mesh32/64, radius increment2, center32+192/16, extent20/40,
color255 and phase steps. Source preserves float stores and ordered comparisons,
including NaN-sensitive mesh/outside/direction paths. It reuses the existing D3DX
normalization declaration and native imported thunk45F9BC. No alternative SDK,
approximate normalization, missing-ID guard or geometry shortcut is introduced.

Motion field18 is Cartesian speed or angular increment depending on flags, and
1C is angle: SetPolar receives1C then18. Earlier names were ambiguous. New asserted
state fields cover hit dimensions, flash frames, script-pointer slots, mesh
limit/color/phases and callback/reference types. Spell/enemy views reuse existing
MeshEffect global pointer declarations. Original class identities remain open.

Transition561 retains both life-threshold and timed-slot selection, signed UI
seconds/hundredths, frame resets, distinct script returns and global-state gates.
Tracking preserves the counterintuitive native comparison between existing and
current horizontal distances, then the tracking gate and lock-byte publication.

Initial full source emits4023/3510 with a private EAX receiver and100-byte local
frame. Actual script/actor/tween/animation/mesh/math support recovers the native
stack receiver and92-byte frame naturally, giving4055/3295. Complete native frame
logic, separate sound branches and real counter support give4177/3219. This final
context has17 support TUs plus the primary; no forced register/frame, instruction
bytes or padding. An independent audit finds all64 direct-call targets and their
multiplicities, three indirect callbacks and five returns agree. This supports
source completeness, not byte equality or whole-game runtime closure.

All3219 normalized differences and the full53-byte size gap remain open. Early
gaps include pair-helper receiver allocation, then vector/motion/call scheduling
and mesh arithmetic/control-flow layout. Original vector return ABI is open.
Refreshed old diagnostics remain actor699->706/442, EH384->390/327, deleting31->
30/20, unlink82->79/74, position309->310/269 and pair673->632/627. No EH/static/
split-owner context is cropped or exempted.

## Pointer-return correction and exact helpers

The21-byte456520 helper returns the VM pointer in EAX: the complete state caller
immediately dereferences VM3B0. The earlier "no boolean promise" description was
insufficient. Separate30-byte419A20 returns full integer0/1. Both clear a missing
ID and use the complete79-byte lookup. The initial probe always passed the global
renderer, allowing LTCG to remove that lookup parameter and six-byte caller load.
An independent variable renderer input retains both real inputs; complete21/30/70
then agree naturally. Receipts capture this explicit context, without a fabricated
target calling convention. Event70 writes only word37C=1 and propagates when
marker18 is null. Release66 marks04000000; raw position97 includes three native
alignment bytes. Those full non-exact owners remain open.

Advance75 retains the real FSINCOS-member and angle-normalizer boundaries.
FSINCOS30 remains source-open; no sin/cos substitute is invented. Variable timer96
remains96/6, differing only in near-one FLD/FADD scheduling. Compound and reversed
addition spelling do not close it; no precision/volatile/ABI workaround is used.
Score51 preserves signed-truncated input/10, wrapped addition and signed cap at
999999999, without lower clamping or overflow saturation. Direct global assignment
recovers51/0 from50/29. Count19 increments only below99999999. Value95 wraps/caps
at13000, gates only scale initialization and stores current10/fraction10/previous9;
its private EAX/native stack-input gap remains open. No generic timer reset adds
unobserved stores.

## Wider review and selected cold proof

Twenty-one complete owners were decoded, thirteen newly reviewed. New reviews
cover damage1579, collision244, positional sound148, effect457, FSINCOS30,
timer96, event70/release66/direct position110/raw position97 and three counters.
Full650-limit damage export replaces the bounded prefix:510 instructions/two RET8,
256-record/32-record collision paths, callbacks, animation/damage transitions,
nonlinear accumulation and score cap. Sparse1566 omits13 alignment bytes. Effect457
covers eleven random count buckets and a48-byte count-storage clear. Full flow,
returns, neighbors and incoming references are checked. Engine callers and generic
math/queue patterns do not independently establish authorship.

Independent exact bindings: Advance REL32 at27/66 to459AA0/458ED0; ID/event helpers
DIR32 at4 to4C3268 and REL32 at10 to4561E0; status DIR32 at1 to4A8D6C; score DIR32
at10/32/40 to4A56E4; count DIR32 at2/14 to4A5754. All14 fields and every byte pass
unique public primary-TU ownership, current source/header/support/tool/producer/
PE/MAP/PDB receipts and exhaustive replay.

`.analysis/secondary-state-affected-cold.log` passes7 new+7 directly affected prior
units across seven contexts. Actual receipt dependencies select the old actor
lifetime/thread/pair families and ordinary12-byte adapter. Common script/stage/renderer headers are
unchanged; no full cohort or unrelated family replay. The first13-unit pass preceded correction of the declaration header
comment that still called state source-open. Because receipts bind complete
header hashes, that correction requires the final14-unit selected replay; no
old receipt is relabeled. Source froze again before those cold builds. Oracle/build tools are unchanged; no broad
behavior or infrastructure suite ran.

Private evidence includes full state/dependency/sound/effect reviews, complete
damage export, independent21-owner CFG/hash/return records, full call audit,
seven binding recipes, source-freeze record, compiler variants, refreshed open
diagnostics and selected cold receipts. Native bytes/decompiler/tool/build
artifacts stay private. Product/runtime/port closure and the active goal stay open.
