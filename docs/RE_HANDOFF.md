# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; the outer
folder is not Git. Read AGENTS.md, this file and relevant source/ledger rows.
Current source, receipts and target bytes outrank historical notes.

## Checked-in state

- Original Japanese TH11 v1.00a only; target identity: `config/target.toml`.
- 1,995 candidates;132 complete/source-present mappings and boundary reviews.
- 68 exact functions /13,187 bytes:8 normal COFF and60 linked PE.
- Direct local scripts/Ghidra; no Factory MCP or REA. Pinned VC9 RTM/SDK6.0A.
- Commit prefix: `gpt-6.1-sol: `. All dirty work belongs to this effort.

This checkpoint adds six source mappings totaling1,369 target bytes: ASCII
constructor98, initializer391, destructor475, missed callbacks9/7 and sprite
binding389. Six new origin reviews classify the five ASCII owners as authored;
sprite binding remains indeterminate. Explicit callback spans4014C0/4014D0
were absent from the initial Ghidra inventory. See [ASCII_LIFECYCLE.md](ASCII_LIFECYCLE.md).

The98-byte constructor and9/7-byte callbacks have complete cold linked proofs,
with five/one/one reviewed bindings. Column initialization now samples RNG
before choosing each row address:998 bytes/269 diagnostic differences versus
1022/498. Both affected existing MeshEffects exact units remain exact.

## Open codegen

| Function | Target bytes | Current source / diagnostic differences |
| --- | ---: | --- |
| InitializeAsciiManager,401080 | 391 | 391 /30 |
| AsciiManagerDestructor,401210 | 475 | 433 /290; EH |
| BindAnimationSprite,44AB40 | 389 | 388 /289 |
| InitializeColumnMeshEffect,406920 | 998 | 998 /269; EH |
| InitializeSquareMeshEffect,407BB0 | 342 | 343 /240; EH |
| QueueMeshEffectSound,44A1E0 | 125 | 126 /73 |
| CreatePositionedAnimationVmId,455B10 | 171 | 169 /101 |
| StageScriptMeshConstructor,40E530 | 389 | 369 /276 |
| ResetStageMesh,40E6C0 | 325 | 325 /11 |
| PublishSceneAnimationVm,4556E0 | 123 | 116 /47 |
| ExecuteStageScript,404470 | 2,736 | 2,728; EH open |
| StepStageVectorTween/StepStageFogTween | 883/737 | 785/534; non-exact |

## Concrete next work

1. ASCII initializer's30 differences concern AcquireAnimationDocument's index
   register, log-context register and final VM/file register scheduling. Real
   document/error/sprite support and independent inputs reduce the first374-byte
   initializer to native391 bytes. Do not declare a false ABI to fit it.
2. Sprite binding's70..8C fields are four UV corners, not two current/base
   rectangles. Correct stores are U0:80/70, U1:88/78, V0:7C/74, V1:8C/84.
   Current full owner388/289 differs in selected-sprite pointer caching and x87
   scheduling; original duplicates ECX into EDX before UV stores. Source keeps
   signed16 marker39C distinct from reset's cell374. Drawing remains external.
3. Column initializer's frame is20 versus native24 bytes; positioned-ID hidden
   result/file argument placement differs. Promoted source already recovers
   the998-byte size. No padding or ABI tricks. Constructor, destructor and
   script EH/specialized ownership remain open.
4. ResetStageMesh has11 differences at+243..248/+279..283 (cached x87 step
   versus position loads). Prior cast/helper/loop forms retain11. Changed FP
   profiles were rejected. VM interpreter44B4B0 still has no source; keep
   pursuing substantial complete functions and extending origin review.

## Retained evidence and commands

Canonical outputs/receipts: `build/match` and `build/match-linked`. Target,
pinned tools and `ghidra-project/TH11` remain private. ASCII target reviews and
compiler probes are listed in ASCII_LIFECYCLE.md. Latest proof logs:
`.analysis/ascii-mesh-cold.log` (five affected/new units) and
`.analysis/ascii-final-cold.log` (final ASCII support inputs). Mesh improvement:
`.analysis/column-promoted-diagnostic.json`.

```bash
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py --require-target
```

Cold-replay changed exact units and affected dependencies only. Update tracking
and generated progress, check the diff, and save meaningful progress promptly.
Broad behavior/failure/random/caller suites are retired; old counts are not
rerun requirements. Full native product/runtime/port work remains later work.
