# TH11 reconstruction handoff — 2026-11-26

## Start here

Work only against original Japanese TH11 1.00a (`resources/th11.exe`, SHA-256
`2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`). The
pinned code-generation target is VC9 RTM build 21022. Keep `/GL` for established
LTCG owners and use natural C/C++ only: no inline assembly, padding, byte stubs
or cropped extents. Use the repository shell, `scripts/repo-python` and direct
Ghidra through `scripts/ghidra.py`.

## Current focus: ExecuteAnimationVm (0x44B4B0, 12,056 B)

The maintained interpreter remains non-exact. Its current cold owner is
11,940 bytes: 11,524 code/alignment plus the complete 416-byte opcode table,
against native 11,640 + 416. It has 3,183 instructions, 260/262 calls,
256/259 conditional branches and 142/147 jumps (candidate/native). All direct
calls bind; only `ReadAnimationFloat` differs (76/78). The exhaustive inventory
has 407 fields versus native 415. Frame is `0xE4` versus native `0xCC`, and
VM/cursor are EBX/EDI versus native EDI/EBX. Diagnostic alignment is 1,439/3,203,
or 2,558 after a register-label swap. No exact credit follows.

The frontier uses the canonical argument TU's `/Ob1` profile and links both
actual timer TUs with `/Ob1 /GL`. Full support replays recover the native
124-byte/fourteen-field float destination and twenty-byte/one-field rewind
wrapper. Timer inputs are ESI-timer/EAX-integer/stack-float. Evidence:
`.analysis/gpt-6.1-sol-execution-real-timers-cold{,-code,-fields}.log` and
`gpt-6.1-sol-real-timers-{rewind,float-destination}-support.log`.
The preceding families remain in `build/probe-ltcg/ExecutionBeforeRealTimers`
and `ExecutionBeforeArgumentProfile`.

Fresh current-graph C2 traces confirm VM split 462 winning EBX at priority
7,466 over cursor 37 at 5,778. The cursor loses its only register and split
444 then takes EDI at 5,032. Both complete 33,446-byte debugger/ordinary text
controls match. Evidence: `.analysis/compiler-vc9/execution-registers-real-timers.txt`
and `execution-register-allocation-real-timers.txt`. A rotation-only fast-math
control repairs the two quad-call inputs but changes only four nonfield bytes;
it leaves these allocation roles, frame and reader count unchanged.

The dirty source is frozen. Keep
`.analysis/gpt-6.1-sol-strip-rounding-frozen.sha256` and
`.analysis/gpt-6.1-sol-player-field-split-frozen.sha256` intact. Pending changes
restore the shared quad call, direct child-ID temporaries, the effect-local
timer's native pre-memset flag clear, and the player initializer's opaque field
spelling. The latter still writes VM bytes `+41C/+41D`. Nested strip sums retain
two complete float-rounding stages; the former scalar flattening lost the first.
Do not change these sources before the affected replay finishes.

Only consumer controller **86905** is live. It runs
`.analysis/gpt-6.1-sol-replay-strip-rounding-last-four.py` and logs to
`.analysis/gpt-6.1-sol-strip-rounding-last-four.log`. Seventy prior unit proofs
are carried only after receipt checks. The retained scope is 73 exact owners
plus the pending quad helper, in 41 contexts. It is cold-building the final
`clear-scene-projectiles` context. `copy-secondary-host-vector`,
`load-secondary-debug-script` and `apply-menu-joystick-bindings` passed their
complete cold comparisons.
Sessions 2509, 5924 and 58199 are terminal failures; keep their logs and never
restart them merely because the new run is slow.

Two old draw claims were withdrawn after complete cold failures. The unrotated
wrapper is 53 bytes / seven fields versus native 51 / six. Dynamic renderer
input gives 49; a private word flag restores stack passing but still lacks
native's ECX slot. The player draw callback is seven bytes with a tail jump,
versus native's complete ten-byte call/return sequence. Explicit success return,
source-level cdecl callee and primary `/Oy-` controls do not recover it. All
controls are terminal and no C++ interface change was adopted. See
`ANIMATION_VM_DRAW.md` and `SECONDARY_PLAYER_CONTROLLER.md`. The canonical ledger
now has 491 exact owners; do not carry these failed rows as current proofs.

The pending `build-animation-vm-mode-quad` frontier still has a complete cold
108-byte public primary-TU proof: 89 code, three alignment and sixteen table
bytes, all eight fields and zero differences. Evidence:
`.analysis/gpt-6.1-sol-mode-quad-post-strip-compare.json`. Only that context's
rotation TU uses `/fp:fast`, restoring the helper's EAX-VM/ECX-output interface;
rotation itself remains non-exact. Promote the helper after the final consumer
context passes, refresh ledgers/progress and commit the coherent source
checkpoint with `gpt-6.1-sol: ...`. The ordinary main frontier keeps its own
rotation profile. Renderer-layer draw remains a 127-byte frontier with two
setup-MOV ordering differences.

Useful private source models remain separate. Full-word RGB reads and zero-vector
byte copies restore native channel lifetimes and all six missing DIR32 reads.
A partial opcode-17 variable tail restores its missing test without extra
float-destination calls. Constructed vector sums and actual constructed tween
result definitions can recover frame `0xCC` and the three native seven/seven/eight
instruction result-copy sequences, but still miss both opcode-97 readers and
keep the VM/cursor register inversion. These type models do not establish the
original nominal classes. Focused evidence and failed controls are in
`KNOWLEDGE_BASE.md`.

This turn's view controls do not recover the readers. Derived, flat independent,
formal-input and child-reference models all retain 76 reads. The flat/formal
models have identical nonfield bytes and field layouts to the maintained owner.
Old images really have 78 reads, but still narrow the script ID; two also publish
translation too early. Do not restore those differences to influence allocation.

Separating the active scratch union gives an 11,924-byte private owner and
frame `0xDC`, with the same instruction/call/field counts. Both debugger/ordinary
whole-text controls match. Stack colors fall 26→25 and payload 205→202:
a three-byte RGB target now shares the vertex-pointer word slot; the float
result remains on the stack. This is a concrete nonlocal packing effect, not
removal of the float spill. Both bodies retain immediate four-byte `FSTP`
stack stores at all four math calls; see
`.analysis/gpt-6.1-sol-active-scratch-math-rounding.txt`. Consider this source
model after the current replay finishes. A paired constructed-result control
keeps frame `0xCC` but shifts the recovered tween result slot from `ESP+24`
to `ESP+28`; smaller scratch/color layouts are not automatically better.
Its C2 traces again remove one three-byte color, with both complete ordinary
text controls exact. A fresh color-choice trace explains the chain: maximum
7 no longer excludes float element 8; later angle/color placements change,
while vertex 13's full neighbor bitmap stays identical. See
`.analysis/compiler-vc9/scratch-color-choice-comparison.txt`.
The inline child-setter control also retains 76 reads.
All private probes are terminal.

Rebuild and inspect only the main owner:

```bash
scripts/repo-python scripts/build-match-unit.py --unit animation-execution-tween-context
scripts/repo-python .analysis/compare-execute-animation-vm-20261122.py build/match-linked/AnimationExecutionTweenContext
```

## Accepted leaf progress

`AnimationFloatDestination` (`0x44B380`, 124 B) is now canonical exact. The
isolated `animation-float-destination` VC9 `/GL` cold build matches its complete
public primary-TU PDB contribution byte-for-byte after replaying all 14 reviewed
relocations. `AnimationIntegerDestination` (`0x44B400`, 104 B) is now canonical exact
in its complete interpreter LTCG support context. The strict linked replay
reproduces all 104 bytes after replaying 11 reviewed local jump-table
relocations. Its original source-TU attribution remains indeterminate. Neither
destination leaf implies exactness for the enclosing interpreter.

`StepAnimationFloatPairTween` now has a 673-byte / 221-instruction owner in
both standalone and boss-context LTCG builds. Four relocations replay and only
two mode-17 Y-component x87 operand bytes remain different; the X component
and mode-8 schedule align. The maintained expression is
`self->target + self->control2`. A controlled `/Op` trial did not resolve the
Y-order difference and the formal profile is unchanged. This leaf remains
frontier-only, not exact. `BindAnimationSprite` (389 B) remains non-exact.
See `docs/ANIMATION_EXECUTION.md`, `docs/ANIMATION_TWEEN.md` and
`docs/ANIMATION_TWEEN_DISPATCH.md` for focused evidence.

## Efficient continuation

Keep the large boss and its real leaf dependencies in the same work stream.
After each meaningful natural-C++ change, build and compare only the affected
unit and directly affected exact consumers. Do not run the full cold replay.
Save each coherent checkpoint with a `gpt-6.1-sol: ...` commit. Preserve useful
failed experiments in `.analysis/`; do not replace maintained source with a
worse candidate merely to keep a trial active.

## Other large functions still in progress

These are **nonexact** (figures from 2026-11-20), measured on their focused
natural-C++ VC9 frontiers. Instruction-key alignment is diagnostic only:

| Owner | Native / candidate | Matched keys | Next concrete issue |
| --- | --- | ---: | --- |
| `ExecuteStageScript` (`0x404470`) | 2,736 / 2,692 B | 582/610 | Opcode 8 channel spill schedule, fog/tween return buffers, oscillator x87 scheduling, EH/funclet ownership |
| `CullStageObject` (`0x403950`) | 1,643 / 1,644 B | 362/504 | Private EAX camera input now modelled by the fourth source parameter; projected-point x87/local register schedule |
| `DrawStageLayer` (`0x404190`) | 727 / 740 B | 156/191 | Real callees and culler EAX input linked; volatile object-pointer lifetime recovers 22 keys, 13-byte size gap remains |
| `InitializePrimaryStageScripts` (`0x405030`) | 249 / 246 B | 68/78 | Early callee-saved pushes, stack layout and final store/return scheduling |
| `LoadStageResource` (`0x403FC0`) | 297 / 297 B | 84/99 | Real ResourceFile EAX-filename call restored; AcquireAnimationDocument ECX/EBX inputs, section pointer and VM-count register scheduling remain |
| `UpdateSecondarySpellStageState` (`0x40C0A0`) | 624 / 624 B | 165/167 | Only remaining instruction-key difference: final Z x87 load/add operand order |
| `InitializeSecondaryDebugHudAnimations` (`0x41A6D0`) | 1,619 / 1,619 B | 385/402 | Initial return buffer, frame reservation, zero-ID wrap and retained animation scheduling |
| `SecondaryEclDebugOwner::~SecondaryEclDebugOwner` (`0x40D320`) | 462 / 462 B | 126/127 | One compiler-generated EH state initializer (`1` native vs `0` candidate); decode the native FuncInfo/unwind map first, the likely cause is a missing member or base with a non-trivial destructor |
| `InitializeSecondarySpellStage` (`0x40C650`) | 1,777 B native | diagnostic | Real caller uses EAX spell-name pointer and two stack integers, not three stack arguments |

Read [STAGE_SCRIPT](STAGE_SCRIPT.md), [STAGE_CULLING](STAGE_CULLING.md),
[STAGE_RESOURCE](STAGE_RESOURCE.md), [STAGE_TWEEN](STAGE_TWEEN.md),
[SECONDARY_SPELL_STAGE](SECONDARY_SPELL_STAGE.md) and
[SECONDARY_DEBUG_LIFECYCLE](SECONDARY_DEBUG_LIFECYCLE.md) for native
observations, original owner extents and failed code-generation trials.

**Important known traps:** `ExecuteStageScript` owns two internal switch tables
(100 B) within its complete 2,736-byte target extent. Do not crop either or
confuse compiler-generated EH handlers with the public function body. The
large initializer's linked 246-byte result is newer than its historical
237-byte normal-COFF diagnostic. The spell updater's `scaled.z + position.z`
C++ expression still emits the *wrong x87 operand load order*; multiple
mathematically equivalent C++ trials did not resolve that difference.

`AcquireAnimationDocument` (0x454360) remains open. Its native slow path
passes the filename in ECX with renderer/index on the stack. The 2026-11-20
attempt to get that through a `__thiscall` member on a cast filename view
broke four exact callers and was reverted on 2026-11-22; see
[STAGE_RESOURCE](STAGE_RESOURCE.md) ("Acquisition and waiting"). Any new
attempt must replay the 19 exact units that link `AnimationDocument.cpp`.

The `DrawStageLayer` frontier links the real camera, render-batch flush and
immediate VM-draw implementations with their direct source dependencies. The
real-callee anchor-only candidate was 742 bytes / 134/191; modelling the
culler's private camera input as a fourth source parameter moved it to 754
bytes / 134/191. Using double-precision ratio locals and casting only at the
VM stores then removed the X/Y x87 stack spill/reloads, producing a 738-byte /
134/191 layer candidate. `CullStageObject` itself is 1,644 bytes
with 362/504 aligned instructions. Its sole caller loads `g_MainRuntime +
0x434` into EAX, and the focused `/GL` caller/callee pair now passes that
pointer in EAX; the candidate callee copies EAX to ESI at entry. The fourth
source parameter models this private LTCG live-in, not a new public stack
argument. Both owners remain non-exact. Do not promote them based on
instruction alignment or size alone.

## Efficient continuation

Keep large Boss functions and exact-capable leaves in the same work stream.
Start with the full original owner plus call/reference review, change natural
C/C++, then build/compare **only the affected unit**, its owned extent and
relocation fields. Cold-replay only directly affected canonical exact units.
Do not run a full cold replay. Successful checkpoints should be saved as
`gpt-6.1-sol: ...` commits; retain meaningful failed experiments in prose/logs.

Current animation owner entry points:

```bash
scripts/repo-python scripts/build-match-unit.py --unit animation-execution-tween-context
scripts/repo-python .analysis/compare-execute-animation-vm-20261122.py build/match-linked/AnimationExecutionTweenContext
```

The old `animation-execution-isolated` unit is a historical register/frame
experiment. `update-stage-real-interpreter-animation-oy` is configured but its
current linked receipt is stale; rebuild it before using the old comparison
command. Do not compare those outputs as current metrics.

Other focused owners:

```bash
scripts/repo-python scripts/build-match-unit.py --unit execute-stage-script-frontier
scripts/repo-python .analysis/compare-stage-script-frontier-20261118.py
scripts/repo-python scripts/build-match-unit.py --unit cull-stage-object-frontier
scripts/repo-python .analysis/compare-cull-stage-object-20261118.py
scripts/repo-python scripts/build-match-unit.py --unit draw-stage-layer-frontier
scripts/repo-python .analysis/compare-draw-stage-layer-20261118.py
scripts/repo-python scripts/build-match-unit.py --unit initialize-primary-stage-scripts-frontier
scripts/repo-python .analysis/compare-stage-init-current-20261119.py
scripts/repo-python scripts/build-match-unit.py --unit animation-document-acquire-trace-frontier
```

These `.analysis/` helpers and review logs are **local ignored evidence**, not
tracked source. The retained logs include
`.analysis/stage-script-final-current-20261120.log`,
`.analysis/draw-stage-layer-final-current-20261120.log` and
`.analysis/cull-stage-final-current-20261120.log`. Verify any exact claim with
the strict unit replay command before updating `config/matches.csv`.

## Storage and current workspace

The 2026-11-22 cleanup removed 488 MB of regenerable build intermediates and
packed Git; see [ARTIFACTS](ARTIFACTS.md). On 2026-11-23 a further 37
unconfigured/unreferenced linked output directories (281 files, 4,853,458
bytes) were removed; build/ now reports approximately 689 MiB. Configured exact
families, active frontiers, target, compiler, Wine prefix, Ghidra project and
retained analysis evidence remain intact. The 63.28 MiB AnimationVmRendererOwner
output was kept because tracked configuration/docs still reference it.

The 11,552-byte UpdateStageRealInterpreterAnimationOy receipt is stale after
AnimationTween.cpp changed; rebuild that frontier before using its metrics.
Receipts can become stale when any hashed input changes, even when generated
code does not. A stale receipt is not a lost proof; rebuild the affected unit
and cold-replay only affected exact units. No full cold replay was run for this
handoff cleanup.
