# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11` (the outer
directory is not the Git repository). Read [AGENTS.md](../AGENTS.md), this file
and the relevant source/ledger rows. Current source and receipts are authoritative;
[working knowledge](KNOWLEDGE_BASE.md) indexes detailed topic findings.

## Checked-in state

- Original Japanese TH11 v1.00a only; target identity: `config/target.toml`.
- 1,993 candidates;126 complete/source-present mappings.
- 65 exact functions /13,073 bytes:8 normal COFF and57 linked PE.
- Direct local scripts/Ghidra; no Factory MCP or REA. Pinned VC9 RTM/SDK6.0A.
- Commit prefix: `gpt-6.1-sol: `. All dirty work belongs to this effort.

The handoff checkpoint completes SquareMeshVmCallback's full398-byte public
primary-TU proof with24 reviewed RNG/call/float bindings. Saving each Y field
address before its X sample recovers the native x87 schedule. Only that new unit
and affected `update-mesh-effect-spell-state` were cold-replayed, both passing.
No reconstruction/compiler/Ghidra job remains running.

Focus on complete exact functions, including substantial owners. Broad failure/
null/random/multiframe suites and caller regressions were retired. Historical
counts/paths are not rerun requirements. Preserve honest source, ABI and full
owners; specialized EH/static/split-owner acceptance is still open.

## Open codegen

| Function | Target bytes | Checked-in source / diagnostic differences |
| --- | ---: | --- |
| InitializeColumnMeshEffect,406920 | 998 | 1022 /498; EH |
| InitializeSquareMeshEffect,407BB0 | 342 | 343 /240; EH |
| QueueMeshEffectSound,44A1E0 | 125 | 126 /73 |
| CreatePositionedAnimationVmId,455B10 | 171 | 169 /101 |
| StageScriptMeshConstructor,40E530 | 389 | 369 /276 |
| ResetStageMesh,40E6C0 | 325 | 325 /11 |
| PublishSceneAnimationVm,4556E0 | 123 | 116 /47 |
| ExecuteStageScript,404470 | 2,736 | 2,728; EH open |
| StepStageVectorTween/StepStageFogTween | 883/737 | 785/534; non-exact |

## Useful next work

1. Retained unpromoted input `.analysis/column-sample-first.cpp` samples RNG before
   selecting each row address. It produces the full998-byte owner with269
   diagnostic differences, while preserving callback equality. The argument-form
   equivalent gives the same result. Current source still has1022/498. Inspect
   frame20 versus native24 bytes and positioned-ID argument/return placement;
   do not add inert padding. See [MESH_EFFECTS.md](MESH_EFFECTS.md).
2. Constructor reference/resolver variants give389 bytes but RET12/232 differences;
   member-column access381/277. Actual effect callers alone retain369. A file-member
   model for positioned creation retains169/101. These variants were rejected.
3. Reset's11 differences at+243..248/+279..283 concern cached x87 step versus
   position loads. Cast/helper/loop variants retain11; arrays41; doubles worse.
   `/fp:except`329 and `/fp:fast`352 were rejected. Keep the declared default profile.
4. Continue larger owners too:998-byte initialization,2736-byte script, update/
   drawing and tween bodies. VM interpreter44B4B0 still has no source. Recheck
   target evidence instead of treating historical hypotheses as facts.

## Retained local evidence and first commands

Canonical artifacts/receipts stay in `build/match` and `build/match-linked`.
Target, pinned tools and `ghidra-project/TH11` are retained. Review dumps and
compiler observations remain in `.analysis`; obsolete binaries, behavior drivers,
one-off ledger mutation scripts and unused snapshots were removed. Details:
[ARTIFACTS.md](ARTIFACTS.md). Old knowledge history is under `docs/archive`.

```bash
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py --require-target
```

Latest proof log: `.analysis/handoff-callback-cold.log`. Retained next experiment:
`.analysis/column-sample-first.cpp`, `column-sample-first-diagnostic.json`,
`column-sample-first.asm`, `column-sample-first.log`; reusable compiler driver:
`.analysis/probe-effect-source.py`. Run it only if pursuing that gap.
Cold-replay changed exact units and affected dependencies only; update tracking/
progress and check the diff at checkpoints. Cleanup alone does not require replay.
