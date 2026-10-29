# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; the outer
folder is not Git. Read AGENTS.md and relevant source/ledger rows. Current
source, receipts and target bytes outrank historical notes.

## Checked-in state

- Original Japanese TH11 v1.00a only; target identity: `config/target.toml`.
- 1,995 candidates;163 reviewed boundaries,154 source-present mappings.
- 78 exact functions /14,988 bytes:8 normal COFF and70 linked PE.
- Origins:90 authored,65 reviewed indeterminate,6 exclusions,1,834 pending.
- Direct local scripts/Ghidra; no Factory MCP or REA. Pinned VC9 RTM/SDK6.0A.
- Commit prefix: `gpt-6.1-sol: `. All dirty work belongs to this effort.

This checkpoint adds seven complete projected-quad/software-fog mappings
covering1,896 bytes, including the792/794-byte owners. Fourteen further
boundaries/origins were reviewed: nine engine/math origins remain indeterminate,
five D3DX import thunks have independent full-body/PE-import exclusions.
No new exact claim. See [ANIMATION_PROJECTED_QUAD.md](ANIMATION_PROJECTED_QUAD.md).

The preceding checkpoint closed coordinate632, floor40, unrounded coordinate537
and dispatch54:1,263 exact bytes. Their canonical AnimationQuad/AnimationVmDraw
families and six cold receipts are unchanged. Real submission caller context
naturally produced the native16-byte coordinate and64-byte-aligned floor
frames. Original TU/data ownership remains separate.

## Current gaps

| Function | Target bytes | Current source / diagnostic differences |
| --- | ---: | --- |
| DrawFoggedBillboardAnimationVm,450B00 | 794 | 795 /666 |
| BuildProjectedAnimationVmQuad,450E20 | 792 | 779 /666 |
| AnimationDistanceRoot,4196E0 | 31 | 20 /26 |
| DrawProjectedAnimationVm,451140 | 46 | 48 /24 |
| DrawBillboardAnimationVm,450AD0 | 33 | 38 /30 |
| DrawAdaptiveUnroundedAnimationVm,450690 | 111 | 115 /67 |
| BuildAnimationVmModeQuad,452010 | 89 | 124 /90 |
| BuildBillboardAnimationVmQuad,450700 | 976 | Source open; complete295-instruction review |
| DrawAnimationVmByMode,451EF0 | 217 | Source open; complete code/table review |
| DrawAnimationVm,44F880 | 1,112 | 1,190 /858; canonical VM-draw context |
| BuildRotatedAnimationVmQuad,4503D0 | 571 | Source open; complete target review |
| ApplyAnimationVmDrawState,44F710 | 368 | 348 /216; previous context |

## Concrete next work

1. Continue the794/792-byte owners. Fog now preserves staged float rounding,
   ordered range tests, inline saturated tint, truncating byte writes and
   submission flag2. Distance helper lacks native8-byte-aligned frame; fog
   still differs in distance-expression scheduling and the opaque billboard
   callee's private ABI. Projection has frame/register gaps. /Oy- did not
   recover native layout and was not adopted; no inert local/alignment fix.
2. Complete976-byte billboard and571-byte rotated source without replacing
   paired FSINCOS semantics. Precise sin/cos probes call CRT intrinsics; fast
   probes emit separate FSIN/FCOS. Neither yields native paired code. The
   billboard has an initial pair overwritten later; retain its FP side effects.
   Selector3 leaves local axes unwritten in both these/projected paths.
3. Projection anchors are fixed +/-128 and +/-256, not unit coordinates.
   Rebuild only when !4000 and flagsC, clear8 before rotations and4 after;
   cache matrix+2F4, working X/Y add cached translations but Z replaces them.
   Four real D3DX projects publish renderer world+435140. The wrapper resets
   RHW after submission even when culled. Coordinate query modes2/3 always
   use the unrounded helper, independent of angle.
4. Continue the217-byte draw dispatcher and remaining modes7/8/9/B/C/D;
   14 table entries451FCC..452003 are decoded, with9/C/D sharing a route.
   The89-byte query's four-entry table45206C..45207B is also reviewed.
   The1,112-byte submission still calls real CRT `_frnd`; pragma intrinsic
   gives VC9 C4163 and does not inline it. Four-mode ABI proof remains valid.
   Continue larger script/tween/VM work and expand origin review as well.

## Evidence and checkpoints

Latest new-source probe: `.analysis/projected-quad-distance-lifetime.log` and
`build/probe-ltcg/projected-quad/distance-lifetime/diagnostic.json`. All seven
full primary-TU contributions compiled with real existing rendering support;
no masked diagnostic grants exact credit. No existing canonical source/family
changed, so no old exact unit or full cohort was cold-replayed this checkpoint.

Topic docs link full Ghidra/reference/neighbor/table reviews and compiler
experiments. Original exact receipts remain in `build/match[-linked]`; last
six-unit cold log is `.analysis/vm-draw-six-cold.log`. Target, compiler payloads,
extracted CRT objects and `ghidra-project/TH11` remain private. No tool job is live.

```bash
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py --require-target
```

Cold-replay changed exact units and directly affected dependencies only.
Update tracking/progress, check the diff, and save meaningful progress.
Broad behavior/failure/random/caller suites are retired. Full native product,
runtime and portability work remains later work; the reconstruction goal is active.
