# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; the outer
folder is not Git. Read AGENTS.md and relevant source/ledger rows. Current
source, receipts and target bytes outrank historical notes.

## Checked-in state

- Original Japanese TH11 v1.00a only; target identity: `config/target.toml`.
- 1,995 candidates;143 reviewed boundaries,142 source-present mappings.
- 74 exact functions /13,725 bytes:8 normal COFF and66 linked PE.
- Direct local scripts/Ghidra; no Factory MCP or REA. Pinned VC9 RTM/SDK6.0A.
- Commit prefix: `gpt-6.1-sol: `. All dirty work belongs to this effort.

This checkpoint reviews six quad-rendering owners totaling2,319 target bytes;
five have complete source covering1,207 target bytes. The1,112-byte submission
body remains source-open. All six origins remain indeterminate. New exact
proofs close the25-byte tint channel and142-byte six-vertex appender, adding
167 bytes. See [ANIMATION_QUAD.md](ANIMATION_QUAD.md).

The state owner44F710 is368 bytes through44F87F: the previous338-byte inventory
omitted two alignment bytes and seven switch entries. Adjacent44F880 is a
separate function. The complete632-byte coordinate candidate differs only in
53 ESP-relative/frame bytes; no masks or padding give exact credit.

## Current gaps

| Function | Target bytes | Current source / diagnostic differences |
| --- | ---: | --- |
| BuildAnimationVmQuad,44FE30 | 632 | 632 /53; frame12 versus16 |
| ApplyAnimationVmDrawState,44F710 | 368 | 348 /216 |
| FloorAnimationCoordinate,4520C0 | 40 | 29 /35 |
| DrawAnimationVm,44F880 | 1,112 | No source; complete target review |
| DrawAsciiManager,401670 | 1,636 | 1,208 /1,358; older opaque render context |
| InitializeAsciiManager,401080 | 391 | 391 /30 |
| AsciiManagerDestructor,401210 | 475 | 433 /290; EH |
| BindAnimationSprite,44AB40 | 389 | 388 /289 |
| InitializeColumnMeshEffect,406920 | 998 | 998 /269; EH |
| ResetStageMesh,40E6C0 | 325 | 325 /11 |
| ExecuteStageScript,404470 | 2,736 | 2,728; EH open |

## Concrete next work

1. Continue the632-byte coordinate frame gap: all53 differences were classified
   as ESP displacement/allocation bytes after exhaustive field enumeration.
   Arrays/two-field structs retained632 but worsened differences to72.
   Double width/half-height locals yielded651/604 or651/607, while naturally
   making the floor adapter40/0; rejected as a whole. No explicit alignments
   or unused locals. Double return or a separate double caller did not help.
2. Complete44F880 source faithfully. Its pixel alignment uses four FRNDINT
   instructions with current x87 rounding mode, then subtracts float0.5.
   It is not floor or fixed nearest. Source remains open rather than adding
   an approximate path, invented helper, or target instruction sequence.
   Full native code is `quad-draw-native.asm`; decompiler hypotheses are
   `ascii-render-deps-next.c`. Quad bounds touch viewport edges inclusively;
   cached texture/geometry transitions precede optional color/tint updates.
3. State source replays all seven blend modes and filter transitions, but
   native per-case blend calls differ from current shared code. Selector6
   source is INVSRCCOLOR4; selector7 changes only the cached selector. The
   application counter increments even without actual D3D calls.
4. Investigate original TU/caller context where useful: floor has a native
  64-byte-aligned frame; the scalar source has29 bytes. Ghidra's486590 floor
   callee extent is sparse and spans other functions; it was NOT accepted.
   Existing ASCII canonical families still use opaque rendering calls.
   Adding real support must answer a concrete codegen question and replay
   only the directly affected units. Continue script/tween/VM interpreter
   work and extend origin review; EH/static/split-owner acceptance is open.

## Evidence and checkpoints

Canonical outputs/receipts: `build/match` and `build/match-linked`.
Target, pinned tools and `ghidra-project/TH11` remain private. Latest proof:
`.analysis/animation-quad-cold.log` (two new units only). Complete current
source diagnostics and stack classification:
`.analysis/animation-quad-final-diagnostic.json`. Topic docs link full native
reviews and compiler experiments. No reconstruction/compiler job is live.

```bash
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py --require-target
```

Cold-replay changed exact units and directly affected dependencies only.
Update tracking/progress, check the diff, and save meaningful progress.
Broad behavior/failure/random/caller suites are retired. Full native product,
runtime and portability work remains later work.
