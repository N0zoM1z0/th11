# TH11 reconstruction handoff — 2026-11-26

## Start here

Work only against original Japanese TH11 1.00a (`resources/th11.exe`, SHA-256
`2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`). The
pinned code-generation target is VC9 RTM build 21022. Keep `/GL` for established
LTCG owners and use natural C/C++ only: no inline assembly, padding, byte stubs
or cropped extents. Use the repository shell, `scripts/repo-python` and direct
Ghidra through `scripts/ghidra.py`.

## Current focus: ExecuteAnimationVm (0x44B4B0, 12,056 B)

The complete natural-C++ interpreter, event paths, 104-entry signed-opcode
dispatch table, nine tween updates and both generated-strip paths are present.
The focused `animation-execution-tween-context` cold build currently owns
11,976 bytes and aligns 1,484/3,308 native instruction keys; calls are 262/262
and conditional branches 404/406. The twelve integer/float relation handlers
now match their complete decoded bodies after only the proven EBX/EDI VM-vs-
instruction register-label normalization, except for five temporary-register
instructions in opcode 39's shared continuation. This was achieved by changing
`ReadI` to test the variable bit before loading its operand, matching the
native conditional-read order. The frame is `0xD4` rather than native
`0xCC`, and the VM/cursor allocation remains EBX/EDI instead of native
EDI/EBX. Parameter-type, helper-argument-order and register-declaration-order
trials were rejected; snapshots remain in `.analysis/`. The dispatch handler
order is largely consistent with the native table.

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
relocations. `AnimationIntegerDestination` (`0x44B400`, 104 B) also has a
zero-difference linked diagnostic, but remains unclassified pending its own
canonical acceptance. Do not infer interpreter exactness from either leaf.

The pair-tween helper still has a two-byte mode-17 operand-order gap and remains
frontier-only. `BindAnimationSprite` (389 B) remains non-exact. See
`docs/ANIMATION_EXECUTION.md`, `docs/ANIMATION_TWEEN.md` and
`docs/ANIMATION_TWEEN_DISPATCH.md` for focused evidence.

## Efficient continuation

Keep the large boss and its real leaf dependencies in the same work stream.
After each meaningful natural-C++ change, build and compare only the affected
unit and directly affected exact consumers. Do not run the full cold replay.
Save each coherent checkpoint with a `gpt-web: ...` commit. Preserve useful
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
`gpt-web: ...` commits; retain meaningful failed experiments in prose/logs.

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
