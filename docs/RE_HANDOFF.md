# Agent handoff

Current source, ledgers and canonical receipts are authoritative. See
[AGENTS.md](../AGENTS.md), [RE_WORKFLOW.md](RE_WORKFLOW.md) and
[ORACLES.md](ORACLES.md). Historical notes are not current requirements.

## Current state and priority

- Original Japanese TH11 v1.00a only; target identity is in `config/target.toml`.
- 1,993 provisional candidates;116 complete/source-present mappings.
- 57 exact functions /11,693 bytes:8 normal COFF and49 linked PE.
- Focus on full-function byte equality, including useful medium/large functions.
- Direct local Ghidra/compiler tools; no Factory MCP or REA. VC9 RTM remains pinned.
- Commit meaningful progress with `gpt-6.1-sol: ...`.

The operator explicitly removed broad behavior/failure/random/multiframe suites
and caller regressions from the default workflow. Do not recreate them or treat
historical test counts as obligations. Use execution only for a specific unresolved
codegen/ABI question. Public Oracle tests are for changes to those tools; full
CI is not a routine source/documentation checkpoint requirement.

## Resume and checkpoint

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`. Inspect the
worktree and current diff; all existing dirty work belongs to this effort.
Verify target/toolchain once on resume, then inspect the relevant source and
ledger entries. For a checkpoint, cold-replay only changed exact units and
actual declared dependencies, update tracking/progress, and check the diff.
Never replay the entire cohort by default.

```bash
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/progress.py --check
```

## Latest exact and source work

| Function | Target bytes | Current codegen |
| --- | ---: | --- |
| StageScriptMeshConstructor,40E530 | 389 | source362; non-exact |
| ResetStageMesh,40E6C0 | 325 | source325;75 address-normalized diagnostic differences |
| PublishStageMesh,40E890 | 123 | exact; no fields |
| StageMeshVmCallback,40E520 | 16 | exact; one REL32 |
| CreateStageMeshRow/CreateAlternateStageMeshRow,42B3D0/42B480 | 168/170 | source162/164; non-exact |
| FindAnimationVmById,4561E0 | 79 | source73; private ABI differs |
| EvaluateStageEasing,459730 | 764 including table | exact;35 fields |
| StageVectorTweenFactor/StageFogTweenFactor,405960/405990 | 39/45 | exact; one REL32 each |
| SetStageTimer,406100 | 59 | exact; one DIR32 |
| AdvanceStageTimer,459270 | 83 | source83;8 diagnostic differences |
| StepStageVectorTween/StepStageFogTween,405240/4055D0 | 883/737 | source785/534; non-exact |
| ExecuteStageScript,404470 | 2,736 including tables | source2,728; non-exact; EH open |
| PackStageFog,405B70 | 171 | exact; no fields |
| UpdateStage/UpdateStageObjects,402AA0/4040F0 | 1,677/153 | source1,705/151; non-exact |
| DrawStage/DrawStageOverlay,403130/4035A0 | 1,128/854 | source1,106/852; non-exact |

The mesh checkpoint adds seven complete mappings covering1,270 target bytes.
Callback40E520 was missing from the initial inventory; its full16-byte owner was
explicitly reviewed. Publisher123 includes3 internal alignment bytes;
constructor389 includes7. See [STAGE_MESH.md](STAGE_MESH.md).

Only these seven units were cold-replayed: new `publish-stage-mesh` and
`stage-mesh-vm-callback`, plus affected `pack-stage-fog`, `set-stage-timer`,
`stage-vector-tween-factor`, `stage-fog-tween-factor`, `evaluate-stage-easing`.
All pass. Existing source/header changes affect those four canonical families;
the older lifecycle/callback exact families have unchanged inputs.

Current canonical artifacts are `build/match-linked/StageMesh`, `StageScript`,
`StageTween` and `StageEasing`. Useful compiler probes remain private
`.analysis/probe-stage-mesh.py` and the subsystem probe scripts. Broad stage
behavior drivers/models were deleted; their historical paths are retired.

## Next exact work

1. Resolve ResetStageMesh's same-size codegen gap: tuple copies now match native
   integer copies, but temporary-slot reuse and scheduling differ. Scalar vertex
   assignments produced358 bytes; aggregate prefix copy restores325. Aggregate
   position/originY gives75 diagnostic differences, versus79 when assigning the
   rounded origin back to parameter y. Do not repeat these unchanged variants.
2. Recover constructor/row-helper/lookup caller context. Native constructor takes
   rows in EAX, while this probe selects columns; row helpers take columns in
   native EBX versus candidate EAX, and lookup ID is native stack versus candidate
   ECX. Preserve honest declarations; do not force private ABI with fake parameters.
3. Pursue the interpreter's complete2,736-byte owner and EH/codegen gaps, or the
   existing update/drawing/pixel functions using fresh evidence. The animation VM
   interpreter44B4B0 still has no source; its declaration is not implementation.

## Durable cautions

- Full extents include owned switch tables/alignment. Easing's old seven-byte
  inventory was only its JMP; its complete owner is764 bytes. Script is2,736,
  not the old code-only2,633.
- Getter conditional-expression returns preserve the final float rounding point.
  Double locals add unwanted frames; assignment-only returns skip a native store.
- Vector mode7 uses old Z+delta Z. The old-Y hypothesis missed a POP and is wrong.
- Mesh+0C contains row VM pointers, not indices; row-array sizes are4*rows-1 bytes.
- D3DSURFACE_DESC is32 bytes, including MultiSampleQuality before Width/Height.
- Existing non-exact diagnostic matches and specialized EH owners get no exact
  credit. Native product/runtime/portability remain later work.

Topic documents and [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) retain target/compiler
findings. Old experiment/test histories do not add checkpoint requirements.
