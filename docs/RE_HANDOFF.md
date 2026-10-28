# Agent handoff

Use current source, ledgers and canonical receipts with [AGENTS.md](../AGENTS.md),
[RE_WORKFLOW.md](RE_WORKFLOW.md) and [ORACLES.md](ORACLES.md). Historical notes
are not current requirements. All dirty work belongs to this effort.

## Current state

- Original Japanese TH11 v1.00a only; identity is in `config/target.toml`.
- 1,993 candidates;120 complete/source-present mappings.
- 61 exact functions /12,246 bytes:8 normal COFF and53 linked PE.
- Direct local Ghidra/compiler tools; no Factory MCP or REA. VC9 RTM is pinned.
- Commit meaningful progress with `gpt-6.1-sol: ...`.

The operator prioritizes exact reconstruction. Broad behavior/failure/null/
random/multiframe suites, caller regressions and per-function full CI are not
checkpoint requirements. Sixteen private stage behavior drivers/models were
removed; active topic documents were trimmed. Do not recreate those suites.
Use execution only for a concrete unresolved codegen/ABI question.

## Current codegen

| Function | Target bytes | Current source |
| --- | ---: | --- |
| FindAnimationVmById,4561E0 | 79 | 79; exact |
| CreateAnimationVmId,455A00 | 112 | 112; exact |
| InitializeSceneAnimationVm,456910 | 171 | 171; exact |
| BindAnimationFileScript,44ADA0 | 191 | 191; exact |
| PublishSceneAnimationVm,4556E0 | 123 | 116;47 diagnostic differences |
| StageScriptMeshConstructor,40E530 | 389 | 369; non-exact |
| ResetStageMesh,40E6C0 | 325 | 325;11 diagnostic differences |
| CreateStageMeshRow/CreateAlternateStageMeshRow | 168/170 | 167/169; non-exact |
| PublishStageMesh/StageMeshVmCallback | 123/16 | exact |
| EvaluateStageEasing/StageVectorTweenFactor/StageFogTweenFactor | 764/39/45 | exact |
| ExecuteStageScript,404470 | 2,736 | 2,728; non-exact; EH open |
| StepStageVectorTween/StepStageFogTween | 883/737 | 785/534; non-exact |

The four-byte AnimationVmId value object recovers stack-by-value lookup and
hidden-result factory returns naturally. Three separately returned zero vectors
recover171-byte initialization; one reused zero local produced133. Binding44ADA0
has no ResetAnimationVm call; fileId-before-flags ordering recovers its schedule.
Allocator4569C0 and execution member405D60 remain external declarations.
See [ANIMATION_VM_FACTORY.md](ANIMATION_VM_FACTORY.md) and [STAGE_MESH.md](STAGE_MESH.md).

## Next exact work

1. Reset's remaining11 differences are at+243..248 and+279..283: native loads
   position then adds cached step; candidate duplicates step then adds memory.
   Casts/reversed additions/helpers/loop forms keep11; arrays41; double locals
   are worse. `/fp:except`329 bytes and `/fp:fast`352 are rejected. Keep default
   profile and the current separate originY/axis-step source.
2. Row factories still anchor their induction pointer at Z rather than native
   rhw, leaving a one-byte extent gap. Constructor still needs register/loop
   context recovery. Scene publication116 versus123 has counter/return gaps.
3. Continue substantial non-exact owners too: script2,736, update/drawing and
   tween bodies. The VM interpreter44B4B0 still has no source. Do not count its
   execution declaration as implementation or crop EH/table extents.

## Checkpoint and recovery

The completed cold run passed11 units across five families: four new exact units,
plus `publish-stage-mesh`, `stage-mesh-vm-callback`, `pack-stage-fog`,
`set-stage-timer`, both tween factors and `evaluate-stage-easing`. This follows
the ID/header/context changes; older lifecycle/callback inputs are unchanged.
Current log is private `.analysis/scene-factory-cold.log`.

Verify target/toolchain once on resume. Cold-replay only changed exact units and
actual affected dependencies; refresh tracking/progress and check the diff.
Oracle/infrastructure tests are for changes to those tools, not routine source
or documentation edits. Native product/runtime and portability remain later work.
