# TH11 reconstruction handoff — 2026-11-26

## Start here

Work only against original Japanese TH11 1.00a (`resources/th11.exe`, SHA-256
`2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`). The
pinned code-generation target is VC9 RTM build 21022. Keep `/GL` for established
LTCG owners and use natural C/C++ only: no inline assembly, padding, byte stubs
or cropped extents. Use the repository shell, `scripts/repo-python` and direct
Ghidra through `scripts/ghidra.py`.

## Current focus: ExecuteAnimationVm (0x44B4B0, 12,056 B)

The maintained interpreter remains non-exact. Its latest canonical cold
frontier owns 11,816 bytes: 11,400 code/alignment bytes plus the complete
416-byte opcode table, against native 11,640 + 416. Candidate/native calls
are 260/262, conditional branches 256/259 and jumps 142/147. The only
callee-count deficit is `ReadAnimationFloat` (76/78). The complete candidate
field inventory has 407 entries versus native 415. Code-key alignment is
1,422/3,203, or 2,524 after a diagnostic EBX/EDI swap; neither grants exact credit.

Pending source changes restore the shared quad-mode call in the UV update
and pass child IDs directly from local objects to lookup. They remove 144
and 16 owned bytes respectively. Three real geometry TUs now belong to the
frontier's `/GL` graph. The quad helper has the native 108-byte size but still
uses ECX-VM/EAX-output instead of native EAX/ECX. The interpreter frame stays
`0xD4` versus `0xCC`, with EBX-VM/EDI-cursor instead of native EDI/EBX.

Affected replay found two older failures and one stale claim:

- The signal caller now uses the existing normal-COFF interpreter producer
  to preserve its native stack input; the interpreter remains `/GL`.
  Its complete 95-byte/four-field cold comparison passes.
- An effect-local timer constructor restores the native pre-memset flag
  clear lost when `StageTimerView` became POD. The 80-byte factory and
  1,168-byte initializer pass complete cold comparisons (three/54 fields).
- HUD front draw's old fastcall symbol binding is corrected to the ordinary
  member decoration; all 105 bytes/five fields pass. The renderer-layer caller
  still reverses two setup MOVs at +75/+77. Separate real `/GL` TUs retain
  the same gap. Its prior exact claim is withdrawn in `08af80a`; the canonical
  ledger now has 493 exact functions. Full 127-byte/nine-field frontier retained.

The timer header expanded the affected scope to 76 units/43 contexts. After
the renderer withdrawal, 75 exact units/42 contexts remain to verify. Four
source files are frozen by
`.analysis/gpt-6.1-sol-execution-timer-frozen.sha256`.
Only shell session **37580** remains live, running
`.analysis/gpt-6.1-sol-resume-execution-timer-remaining.py`; its log is
`.analysis/gpt-6.1-sol-execution-timer-remaining.log`. At this checkpoint,
50 current proofs are carried and 13 more have passed; context 31/42
(`reset-animation-vector-tween-time`) is building. Earlier replay sessions
are terminal. Reuse current cold receipts, preserve this run, and finish
remaining comparisons before the source checkpoint. Several remaining
contexts have 132/214 support TUs; standard cold builds remain in use.

Compiler findings are committed in `a84e90d`, `cdf4d28`, `ccb9cb8` and
`f4baeee`; see the focused VC9 sections in `KNOWLEDGE_BASE.md`:

- Stack packing uses class and integer weight/size ratio. The ID-local
  change regroups colors across the owner; it does not shrink the frame.
- Final late flow removes both opcode 97 float reads in one common-tail
  merge with opcode 96, including the XY stores and translation copy.
- Integer `Spillcosts` reduces the original VM mask to EBP and the cursor to
  EBX/EBP. Frame constraints remove EBP. Later main split ranges already
  require EBX-VM/EDI-cursor before final selection. Nine hardware-watchpoint
  hits identify fixed uses in timer setup, child-ID buffers, float destinations
  and tween returns. Debugger/control `.text` sections match all 31,798 bytes.

A case-local opcode 97 child pointer leaves that handler unchanged but swaps
x87 operand order in opcodes 9/13. Explicitly loading X before its variable
flag grows the owner to 11,840 bytes without restoring the missing calls.
Both controls are rejected and remain under `.analysis/execution-opcode97-local`
and `.analysis/gpt-6.1-sol-opcode97-*.log`. Maintained source is unchanged.

The `build-animation-vm-mode-quad` frontier now has a complete cold strict
proof: 108 bytes/eight fields/zero differences, public primary-TU ownership.
Only its rotation support TU uses `/fp:fast`, removing CRT trig calls and
restoring the helper's EAX-VM/ECX-output interface. The rotation remains
non-exact (482 versus 571 bytes, no paired FSINCOS). Proof files:
`.analysis/gpt-6.1-sol-mode-quad-cold.log` and
`gpt-6.1-sol-mode-quad-cold-compare.json`. Keep the unit pending promotion
until the shared-source checkpoint; the canonical count remains 493.

The refined allocation trace shows the main VM split taking EBX at priority
7,466 ahead of the original cursor at 5,760. `UpdateAffectedGlobregs` empties
the cursor's mask; a new cursor split then takes EDI at 5,134. Evidence:
`.analysis/compiler-vc9/execution-cursor-deferral.txt`. `/G6`, a shared renderer
owner, a live 30-TU drawing graph and an inline update helper do not improve
the interpreter's nonfield bytes. Preserve the rejected controls in `.analysis/`.

After the source checkpoint, inspect split costs and real callee register
use before more declaration-order trials. Keep the reader merge and result
lifetimes in the same full-function analysis. Useful evidence:
`.analysis/compiler-vc9/execution-merges.txt`, `execution-initial-splits.txt`
and `execution-mask-writes.txt`. Native result homes are vector/color
`ESP+0x24`, pair `+0x88`, quad `+0xA8`; current homes are `+0x38/+0xA0/+0xB0`.

The comparison-handler diagnostic discovers the current dispatch table
from the owner disassembly and reports the twelve handlers independently:

```bash
scripts/repo-python .analysis/compare-animation-comparison-handlers-20261126.py
```

Rebuild and compare only this owner:

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
