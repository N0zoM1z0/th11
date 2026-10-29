# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; the outer
folder is not Git. Read AGENTS.md and relevant source/ledger rows. Current
source, receipts and target bytes outrank historical notes.

## Checked-in state

- Original Japanese TH11 v1.00a only; target identity: `config/target.toml`.
- 1,995 candidates;137 source-present mappings and boundary reviews.
- 72 exact functions /13,558 bytes:8 normal COFF and64 linked PE.
- Direct local scripts/Ghidra; no Factory MCP or REA. Pinned VC9 RTM/SDK6.0A.
- Commit prefix: `gpt-6.1-sol: `. All dirty work belongs to this effort.

This checkpoint adds five complete source mappings totaling2,007 target bytes:
formatted append83, small-font append106, full glyph drawing1,636, overlay101,
and UV-only setter81. All five origins were reviewed: four ASCII functions
are authored; the shared UV setter remains indeterminate. Four new exact
proofs add371 bytes. See [ASCII_DRAWING.md](ASCII_DRAWING.md).

DrawAsciiManager's old1,618-byte inventory omitted two alignment bytes and the
four-entry switch table. Complete owner401670..401CD3 is1,636 bytes. All four
apparent external interior references originate in that owned table. Code,
table bindings and neighbors were independently reviewed.

## Current gaps

| Function | Target bytes | Current source / diagnostic differences |
| --- | ---: | --- |
| DrawAsciiManager,401670 | 1,636 | 1,208 /1,358 |
| InitializeAsciiManager,401080 | 391 | 391 /30 |
| AsciiManagerDestructor,401210 | 475 | 433 /290; EH |
| BindAnimationSprite,44AB40 | 389 | 388 /289 |
| InitializeColumnMeshEffect,406920 | 998 | 998 /269; EH |
| InitializeSquareMeshEffect,407BB0 | 342 | 343 /240; EH |
| CreatePositionedAnimationVmId,455B10 | 171 | 169 /101 |
| ResetStageMesh,40E6C0 | 325 | 325 /11 |
| ExecuteStageScript,404470 | 2,736 | 2,728; EH open |
| StepStageVectorTween/StepStageFogTween | 883/737 | 785/534; non-exact |

## Concrete next work

1. Reconstruct larger renderer helpers44FE30 (632 bytes, quad positions) and
   44F880 (1,112 bytes, VM draw submission). Private decompiler hypotheses and
   native call-register evidence are in `ascii-render-deps-next.c` and
   `ascii-draw-native.asm`; their full boundary/origin reviews remain open.
   DrawAsciiManager currently calls the UV setter in all font cases; native
   styles0/1 inline UV stores while special fonts retain calls. An out-of-line
   inline-marked definition was not emitted and failed linking; rejected.
2. Small-font formatting's native count*130+86C store selects record+120,
   not draw channel+128. VM+374 is diffuse ARGB; the historical generalized
   view's `spriteIndex` name is misleading. Default advance converts drawMode
   as signed (native FILD), despite its older unsigned storage label.
3. Independent renderer/camera inputs prevent global-argument specialization:
   overlay87 becomes native101. The full main owner remains non-exact.
   Initializer30 differences concern document index/log registers and final
   VM/file scheduling. Binder388/289 still lacks native selected-sprite EDX
   caching; cached-pointer/member variants did not close it.
4. Continue column initialization (frame20 versus native24, hidden-result/file
   placement), larger script/tween bodies, and VM interpreter44B4B0 without
   source. Keep extending origin review. Do not pad bodies or falsify ABI;
   EH/static/split-owner exact acceptance remains separate and open.

## Evidence and checkpoints

Canonical outputs/receipts: `build/match` and `build/match-linked`.
Target, pinned tools and `ghidra-project/TH11` remain private. Latest cold
proofs: `.analysis/ascii-drawing-seven-cold.log` (four new/three affected
lifecycle units) and `ascii-drawing-final-four-cold.log` (final signed-advance
source). Main diagnostics: `ascii-draw-final-diagnostic.json`. Topic docs link
complete target reviews and further private compiler experiments.

```bash
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py --require-target
```

Cold-replay changed exact units and affected dependencies only. Update tracking
and generated progress, check the diff, and save meaningful progress promptly.
Broad behavior/failure/random/caller suites are retired. Full native product,
runtime and portability work remains later work.
