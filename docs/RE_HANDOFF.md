# Agent handoff

Use current source, ledgers and receipts with [AGENTS.md](../AGENTS.md),
[RE_WORKFLOW.md](RE_WORKFLOW.md) and [ORACLES.md](ORACLES.md). Historical notes
are not requirements. All dirty work belongs to this effort.

## Current state

- Original Japanese TH11 v1.00a only; identity is in `config/target.toml`.
- 1,993 candidates;126 complete/source-present mappings.
- 64 exact functions /12,675 bytes:8 normal COFF and56 linked PE.
- Direct local Ghidra/compiler tools; no Factory MCP or REA. VC9 RTM is pinned.
- Commit meaningful progress with `gpt-6.1-sol: ...`.

Focus on complete byte-exact reconstruction. Broad failure/null/random/
multiframe suites, caller regressions and per-function full CI are not checkpoint
requirements. Do not recreate retired behavior suites. Execution is only for a
concrete unresolved codegen/ABI question.

## Latest source and exact work

CreateStageMeshRow168, CreateAlternateStageMeshRow170 and status updater91 now
have full primary-TU cold proofs with5/5/4 reviewed fields. Assigning vertex Z,
rhw, color, then incrementing the pointer recovers native rhw-based induction.
All five mesh exact units total556 bytes. Source/header context is in match-units.

MeshEffects.cpp adds six full owners covering2,125 target bytes, including the
998-byte column initializer,342-byte square initializer and398-byte deformation
callback. Real RNG, VM, factory and binding code supplies their compiler context;
independent probe inputs prevent constant state/layer/sound specialization.
See [MESH_EFFECTS.md](MESH_EFFECTS.md), [STAGE_MESH.md](STAGE_MESH.md), and
[ANIMATION_VM_FACTORY.md](ANIMATION_VM_FACTORY.md).

## Open codegen

| Function | Target bytes | Current source / diagnostic differences |
| --- | ---: | --- |
| InitializeColumnMeshEffect,406920 | 998 | 1022 /498; EH |
| InitializeSquareMeshEffect,407BB0 | 342 | 343 /240; EH |
| SquareMeshVmCallback,408070 | 398 | 398 /12 |
| QueueMeshEffectSound,44A1E0 | 125 | 126 /73 |
| CreatePositionedAnimationVmId,455B10 | 171 | 169 /101 |
| StageScriptMeshConstructor,40E530 | 389 | 369 /276 |
| ResetStageMesh,40E6C0 | 325 | 325 /11 |
| PublishSceneAnimationVm,4556E0 | 123 | 116 /47 |
| ExecuteStageScript,404470 | 2,736 | 2,728; EH open |
| StepStageVectorTween/StepStageFogTween | 883/737 | 785/534; non-exact |

## Next exact work

1. The398-byte callback's remaining12 differences are Y load/add scheduling at
   +183..188 and+318..323. A constant template scale removes an unwanted aligned
   frame; an inline coordinate updater improves22 to12. Y postincrement retains12.
   The positioned factory's file-member model retains169/101 and was rejected.
2. Constructor row-ID reference/resolver forms give389 bytes but RET12 and232
   differences; member-column access381/277. Adding the two real effect callers
   alone retains369. Preserve correct declarations; do not force register ABI.
3. Reset's11 differences are+243..248 and+279..283: native loads position then
   adds cached step; candidate duplicates step then adds memory. Cast/helper/
   loop rewrites retain11; arrays41; doubles worse. `/fp:except`329 and `/fp:fast`
   352 were rejected. Keep default profile and separate originY/axis-step source.
4. Continue substantial owners too:998-byte effect initialization,2736-byte
   script, update/drawing and tween bodies. VM interpreter44B4B0 still has no
   source. Specialized EH ownership is open; do not crop owners or grant masked
   exact credit. Check target evidence rather than treating old hypotheses as facts.

## Checkpoint and recovery

The completed cold run passed13 units across five families: MeshEffects status,
five StageMesh units, three AnimationVmFactory units, pack-stage-fog, set-stage-timer
and both tween factors. This is three new plus ten affected exact units. Easing,
older lifecycle and chain inputs are unchanged and were not replayed.
Private log: `.analysis/mesh-effects-cold.log`; non-exact full-owner diagnostics:
`.analysis/mesh-effects-canonical-diagnostic.json`.

Verify target/toolchain once on resume. Cold-replay only changed exact units and
actual affected dependencies; refresh tracking/progress and check the diff.
Oracle/infrastructure tests apply to changes to those tools. Native product/
runtime and portability remain later work.
