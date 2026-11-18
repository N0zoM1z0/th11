# TH11 reconstruction handoff — 2026-11-18

## Target and repository state

Reconstruct only the original Japanese TH11 v1.00a executable:
resources/th11.exe, 688,128 bytes, SHA-256
2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813.
The target and PE/Rich identity were rechecked on 2026-11-18. The compiler
candidate is pinned VC9 / Visual C++ 2008 RTM build 21022; preserve /GL
for established LTCG owners.

The source checkpoint entering this handoff cleanup was
5e0a53b (gpt-web: restore indexed six-point culling traversal).
The documentation commits in this cleanup are local; check the live
upstream relationship before assuming they have been pushed. The source
worktree is clean after preserving and withdrawing the unhelpful spell-view
method experiment described below.

scripts/progress.py currently reports 2,175 tracked candidates, 1,607
reviewed boundaries, 813 source mappings, and 477 canonical exact functions
covering 52,309 code bytes. These are local ledger totals. The Factory
inventory reports 2,175 subjects, zero imported Oracle results, and an
inventory-import-only warning; do not present local exact counts as
Factory-accepted results or whole-game closure.

## Current large-function frontiers

| Function | Native owner | Current focused VC9 /GL contribution | State |
| --- | ---: | ---: | --- |
| CullStageObject (0x403950) | 1,643 B | 1,648 B; 328/504 aligned instructions | Main active boss; strict depth, NaN extrema, three-argument fastcall; EAX camera input remains open |
| DrawStageLayer (0x404190) | 727 B | 724 B; 112/191 aligned instructions | Nonexact; caller now uses native three-argument culler boundary |
| ExecuteStageScript (0x404470) | 2,736 B | 2,720 B; 551/610 aligned instructions | Nonexact; per-mode oscillator tail restored; EH/funclet ownership open |
| StepStageVectorTween (0x405240) | 883 B | 883 B; 276/280 aligned instructions | Nonexact; four fallback x87 differences remain |
| InitializeSecondarySpellStage (0x40C650) | 1,777 B | Diagnostic frontier exists | Nonexact; see SECONDARY_SPELL_STAGE.md |
| UpdateSecondarySpellStageState (0x40C0A0) | 624 B | 628 B in the current wide callback producer | Nonexact |

Instruction alignment is a navigation aid, not exactness. The two stage
rendering owners have complete maintained C++ bodies and useful target
control-flow notes in STAGE_CULLING.md.

### Useful exact leaves

- SetSecondaryPairDuration (0x419510, 4 B) and AddSecondaryPair
  (0x4195E0, 16 B) are natural C++ and passed selected strict cold replay.
  Evidence: .analysis/secondary-pair-duration-strict-20261118.log,
  .analysis/secondary-pair-add-strict-20261118.log, and
  .analysis/secondary-pair-full-source-exact-20261118.log.
- SecondarySpellDrawCallback (0x40C640, 7 B) passed strict cold linked-PE
  replay, including its REL32 field:
  .analysis/secondary-spell-draw-callback-cold-20261118.log.
- SecondarySpellUpdateCallback (0x40C630, 10 B) is still nonexact.
  Native saves EDI around the updater call; the current candidate saves ESI.
  Do not transfer the draw callback's exact result to this separate owner.
- PackStageFog (0x405B70, 171 B), ResetAnimationVm (0x401FD0, 303 B),
  and ResetAndBindStageAnimationVm (0x405B10, 30 B) all passed targeted
  strict cold linked-PE replay in this session with zero byte/relocation
  differences. Opcode 9's FISTP is round-toward-zero because native sets
  x87 RC=11b; keep the C++ float-to-int conversion truncating.

## Immediate next work

0. Continue the stage-spell initializer through its real caller. The opcode
   owner now calls `InitializeSecondarySpellStage` directly, and the focused
   `secondary-actor-instruction-adapter` graph links the initializer beside the
   16,260-byte instruction owner. Its difficulty dispatch now keeps cases 1 and
   2 separate, uses the native shared overlay tail, and delays the event-owner
   lookup to its use sites (134/468 aligned keys).
   The native caller uses EAX for the spell-name
   pointer plus two stack integers (`ret 8`); the candidate still uses three
   stack arguments (`ret 12`). Recover this boundary only through natural C++
   and whole-caller LTCG context; do not add register shims or ASM.
1. Continue CullStageObject from the indexed two-group traversal, inclusive
   viewport-edge tests, strict 0 < z <= 1 depth gate, and native asymmetric
   X/Y extrema branches. Unordered coordinates follow the native fallback
   into the minimum bound. The focused candidate is 1,648 B / 328 of 504
   aligned instructions. Its natural three-argument fastcall now models the
   caller's object/offset/distance registers; the native private EAX camera
   input remains open. Rebuild only cull-stage-object-frontier and work on
   the remaining stack/local and x87 ordering differences.
2. Rebuild draw-stage-layer-frontier when a shared source/header changes.
   Its candidate is 724 B / 112 of 191 aligned instructions after the culler
   ABI change. Preserve the active-camera reload, explicit float offset
   stores and full-width bit-11 extraction documented in STAGE_CULLING.md.
3. ExecuteStageScript now has per-mode oscillator timer advance/wrap and
   return paths, reaching 2,720 B / 551 of 610 aligned instructions (from
   2,736 B / 537). Continue its opcode/timer dataflow and EH/funclet work with
   only execute-stage-script-frontier. StepStageVectorTween and the
   1,777-byte spell-stage initializer remain active alternatives. Avoid broad
   replay.
4. The previous SecondarySpellStageView::Update() method experiment did
   not improve the callback ABI: the candidate still saved ESI, not native
   EDI. It also promoted a physical view to a guessed class identity. Its
   three-file patch is retained at
   .analysis/secondary-spell-view-method-experiment-20261118.patch;
   the tracked sources were restored to the free-function form.

## Focused commands

    scripts/repo-python scripts/build-match-unit.py --unit cull-stage-object-frontier
    scripts/repo-python .analysis/compare-cull-stage-object-20261118.py
    scripts/repo-python scripts/build-match-unit.py --unit draw-stage-layer-frontier
    scripts/repo-python .analysis/compare-draw-stage-layer-20261118.py

Refresh local tracking/progress after a ledger change:

    scripts/repo-python scripts/validate-tracking.py --require-target
    scripts/repo-python scripts/progress.py
    git diff --check

For source changes, cold-replay only affected canonical exact units and
direct dependencies. Do not run the full cold replay. Keep source natural
C++; no inline ASM, embedded instruction bytes or padding. Commit meaningful
checkpoints with the gpt-web: prefix.

## Storage and retained evidence

See ARTIFACTS.md. The private target, VC9 toolchain and Wine prefix,
Ghidra project, configured linked-output directories, exact receipts,
current comparison scripts and useful .analysis evidence are retained.
Unreferenced compiler-generated source variants and duplicate manifest
snapshots from the 2026-11-18 exploration were pruned; the selected findings
and the one live experiment patch remain documented.
