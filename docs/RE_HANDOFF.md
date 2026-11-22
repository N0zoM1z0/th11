# TH11 reconstruction handoff — 2026-11-22

## Start here

Work against **Japanese TH11 1.00a only**: private `resources/th11.exe`,
688,128 bytes, SHA-256
`2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`.
The code-generation target is **VC9 RTM build 21022**; retain `/GL` in
LTCG owner contexts. Use the repository shell, `scripts/repo-python`, and
`scripts/ghidra.py` for direct Ghidra analysis. Reconstruction stays natural
C/C++: no inline assembly, padding, byte stubs or cropped extents.

Run `git status --short --branch` and `git log -8 --oneline` before
continuing. Dirty changes in the worktree are the current agent's work to
integrate. `main` was in sync with `origin/main` on 2026-11-22.

## Canonical local progress

[PROGRESS](PROGRESS.md) is generated from the ledgers with
`scripts/repo-python scripts/progress.py`; read the counts there rather than
copying them here. On 2026-11-22 it reported 491 canonical exact functions
(56,045 bytes), 68 of them with confirmed authored origin (15,328 bytes),
out of 2,175 tracked candidates with 562 boundaries still needing review.
These are local exact-codegen ledger claims, not native game closure.

Since the 2026-11-20 handoff:

- Six new exact owners in the animation VM area: `DrawWorldAnimationVm`
  (0x4513A0, 1,036 B), `ApplyAnimationVmWorldState` (0x44F4B0, 600 B with its
  owned blend table), `LoadAnimationChunkSource` (0x4543E0, 221 B) and the
  three random-conversion leaves at 0x4070A0, 0x458DA0 and 0x458DD0.
- `StageUpdateCallback` (0x403900) and `UpdateStageObjects` (0x4040F0) were
  withdrawn from exact when their sources returned to ordinary C++ forwarding
  and traversal (`656511a`). The natural callback compiles to 10 bytes
  against the native 7-byte ECX-to-EAX adapter. See
  [STAGE_LIFECYCLE](STAGE_LIFECYCLE.md) and [STAGE_UPDATE](STAGE_UPDATE.md).
- An Oracle review on 2026-11-22 found one exact row whose source called the
  wrong function. `DrawSecondaryProgramBrowser` called `AsciiManagerPrint`
  while the native calls `AsciiManagerPrintSmall`, and the manifest targets
  hid the difference. The source now calls the native callee, and
  `validate-tracking.py` checks that relocation targets agree with the
  symbols they name. See [KNOWLEDGE_BASE](KNOWLEDGE_BASE.md) ("Relocation
  targets must match the named symbol").
- The same review found four exact rows that no longer reproduced at HEAD:
  `RegisterSecondaryHudChains`, `InitializeSecondaryDebugHudVisuals`,
  `ProcessSecondaryProgramExtensions` and `InitializeTitleMenuResources`.
  `git bisect` traced them to `24ec565`, which changed the shared
  `AnimationDocument.cpp` without replaying its exact callers. That change
  was reverted and the callers pass again. Lesson: when a frontier edits a
  shared source, replay the exact units that link it before committing.

## Current main thread: `ExecuteAnimationVm` (0x44B4B0, 12,056 B)

The animation interpreter is the active large owner. It is measured in two
frontier contexts with `.analysis/compare-execute-animation-vm-20261122.py`
(whole-extent mnemonic/operand alignment, immediates normalised):

| Context | Unit | Size | Aligned keys | Calls |
| --- | --- | ---: | ---: | ---: |
| Full stage-update graph | `update-stage-real-interpreter-animation-oy` | 11,520 B | 964/3,308 | 262/262 |
| Isolated | `animation-execution-isolated` | 10,672 B | 1,173/3,308 | 259/262 |

The native frame uses EBP as frame pointer, aligns ESP to 8, reserves 0xCC
bytes and holds the VM pointer in EDI. The full graph reserves 0xE0 without
alignment; the isolated build aligns but reserves 0xDC. Both hold the VM
pointer in EBX. The call census is now complete in the full graph (the
opcode 40 split-zero change restored the fourth RandomNext32 call). Details
and failed trials are in [ANIMATION_EXECUTION](ANIMATION_EXECUTION.md).

Next experiments, ordered by expected value (TH09 evidence in
[KNOWLEDGE_BASE](KNOWLEDGE_BASE.md), "Lessons from TH09"):

1. Find which handlers decide EBX versus EDI. In scratch, remove groups of
   opcode handlers and watch the VM register; then replace any hand-shared
   tails in the coupled handlers with plain statements.
2. Move handler temporaries into block scope to approach the 0xCC frame.
3. Replace the remaining diagnostic anchors in the interpreter graph with
   real `/GL` callee bodies; opaque callees change caller allocation.
4. Find all native callers and address-taken references, to learn why the
   isolated build emits the stack alignment and the full graph does not.

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

Useful current scoped entry points:

```bash
scripts/repo-python scripts/build-match-unit.py --unit animation-execution-isolated   # ~30 s
scripts/repo-python scripts/build-match-unit.py --unit update-stage-real-interpreter-animation-oy   # ~5 min
scripts/repo-python .analysis/compare-execute-animation-vm-20261122.py build/match-linked/UpdateStageRealInterpreterAnimationOy
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
packed Git (`build/` 1.15 GB to 676 MB, `.git` 172 MB to 5.7 MB); see
[ARTIFACTS](ARTIFACTS.md). Canonical exact families, active frontiers, the
target, compiler, Wine prefix and Ghidra project are intact. Many receipts are
stale after shared-header edits; the comparator then refuses, so cold-replay a
unit before relying on its receipt. No compilation or replay is required just
to edit docs.
