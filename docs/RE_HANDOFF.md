# TH11 reconstruction — next-agent handoff (2026-11-16)

## Target and repository state

Reconstruct only the original Japanese TH11 v1.00a executable:
resources/th11.exe, SHA-256
2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813.
Use the pinned VC9 / Visual C++ 2008 RTM build 21022 toolchain; preserve
/GL in established LTCG owners. Do not substitute th11c.exe, TH10 bytes,
or another compiler.

- The source baseline was commit c0a075a, gpt-web: align stage script tween
  spills; it matched origin/main before this handoff cleanup.
- This handoff adds two local documentation-only commits on top of that
  baseline. They are not pushed. The code baseline remains c0a075a.
- After those documentation commits, src/StageScript.cpp remains the only
  dirty tracked file; there are no staged changes or untracked files.
  **Preserve this diff**. It is current work, not disposable scratch.
- New commits use the gpt-web: prefix.
- docs/PROGRESS.md is generated from the canonical ledgers. Do not copy its
  totals into this handoff.

## Current stage reconstruction

The active large owner is ExecuteStageScript at 0x00404470. The reviewed
target extent is 2,736 bytes. The current diagnostic build is
build/match-linked/StageScriptFrontier; its PDB contribution is 2,768 bytes
at 0x00401100, 32 bytes larger than the target. Its receipt records
src/StageScript.cpp SHA-256
87c67d178f345117b9aa22b5b0d1d09202b0b7a0b7bcf5f3d36b01965cc3b701, which
matches the current worktree file. This is a frontier diagnostic, not an
exact claim or accepted replay.

The current dirty source experiments are:

- Opcode 2 snapshots the original camera position, publishes it to
  camera.updateOffset, applies the script vector, then writes the
  new-minus-old components using raw float-bit copies.
- Opcodes 3 and 5 preload their three float arguments and copy the raw bits
  into the tween target. Opcodes 10 and 11 do the same for all nine
  control/target components.
- StageFogView fog is function-scoped and reused by opcode 9 and the fog
  postpass.
- Opcode 17 currently uses a naked ESI adapter around ReleaseVmBindingBlock,
  and assigns phaseY = phaseX = phase to control store order.

These are code-generation experiments, not settled source-level conclusions.
Keep the diff intact while testing them against the target. In particular,
review the ESI adapter and EH behavior rather than assuming the current
inline-assembly bridge is the final solution.

The same StageScriptFrontier image currently contains:

| Owner | Target bytes | Candidate bytes | State |
| --- | ---: | ---: | --- |
| ExecuteStageScript | 2,736 | 2,768 | Non-exact; EH/funclet ownership and codegen remain open |
| PackStageFog | 171 | 171 | Exact linked leaf |
| StepStageVectorTween | 883 | 887 | Non-exact |
| StepStageFogTween | 737 | 794 | Non-exact in this family |

A separate wide secondary context still provides the exact 737-byte
StepStageFogTween proof. The 794-byte result is the candidate in the
StageTween/StageScript family; these /GL contexts are not interchangeable.
See docs/STAGE_SCRIPT.md and docs/STAGE_TWEEN.md.

Other useful stage results already recorded:

- AdvanceStageTimer — 83-byte exact linked-PE owner.
- UpdateStageObjects — 153-byte exact linked-PE owner.
- StageUpdateCallback — 7-byte exact adapter.
- UpdateStage and StepStageVectorTween remain large, non-exact owners.

## Next work

1. Start with git diff -- src/StageScript.cpp; do not reset or overwrite it.
2. Continue the ExecuteStageScript boss, comparing its complete 2,736-byte
   target extent against the current frontier. Prioritize opcode 2, tween
   argument loads/stores, opcode 17 register ownership, and EH/funclet gaps.
3. Keep StepStageVectorTween and the StageScript owner in their selected
   LTCG contexts. A changed support source/header may affect more than one
   exact unit; use the dependency ledger to select only affected replay work.
4. Build only the named diagnostic/frontier or affected exact units. Do not run
   a full cold replay for routine progress. A successful build or diagnostic
   similarity is not exactness; exact claims require the canonical strict
   receipt and accepted replay.
5. Save meaningful progress with gpt-web: ... commits. Keep this handoff
   short and update it when the active owner or blockers change.

## Working references

- docs/RE_WORKFLOW.md — reconstruction loop and evidence boundaries.
- docs/ORACLES.md — exactness and replay requirements.
- docs/KNOWLEDGE_BASE.md — durable target/compiler findings.
- docs/ARTIFACTS.md — retained private artifacts and cleanup policy.
- docs/PROGRESS.md — generated inventory and exactness totals.

The 2026-11-16 documentation/storage cleanup did not change source, target,
toolchain, Ghidra data, or canonical build/match* receipts. It did not run a
full build or cold replay.
