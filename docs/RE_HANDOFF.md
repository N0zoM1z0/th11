# TH11 reconstruction handoff — 2026-11-17

## Target and workflow

Reconstruct only the original Japanese TH11 v1.00a executable at `resources/th11.exe` (SHA-256 `2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`). Use the pinned VC9 / Visual C++ 2008 RTM build 21022 toolchain. Keep the original executable, `.tools/`, Wine prefix and Ghidra project private. Use direct `scripts/ghidra.py`; do not substitute another game executable or toolchain.

Current branch is `main`; auxiliary singleton ownership was corrected in `619578d`, and the focused culling /GL producer was committed in `0eba967`. This session reconstructed six auxiliary update modes and their neighboring helpers through `cef3185`, `37d22ef`, `74bef7f`, `6563f1c`, and `9750526`; all use natural C++ and remain non-exact. Preserve the `gpt-web: ...` commit prefix. Follow `AGENTS.md`; compile and replay only affected units, not the full cohort.

## Large owners

## Focused large stage-rendering frontiers

- `CullStageObject` (0x403950): target **1,643 bytes / 504 instructions**.
  The pinned VC9 focused link without `/Oy-` now compiles the natural
  camera-eye float snapshot and restores strict NaN-aware bounds checks.
  Its current candidate is **1,468 bytes**, with **297/504**
  normalized instruction keys aligned, compared with 1,316 bytes/206
  on the earlier forced-frame profile. The native six-samples/two-block
  traversal is explicitly represented in C++; the compiler can still
  optimize it to a different loop. Remains **non-exact**.
- `DrawStageLayer` (0x404190): its own scoped VC9 /GL producer now
  yields **723 bytes / 194 instructions** against 727 bytes / 191 native
  (98 normalized keys aligned). The natural C++ now snapshots the
  record offset, retains independent Fog state transitions, and evaluates
  scale results before their flag stores. Native stack frame 0x14 vs
  current 0x1C and caller register scheduling remain open.
  See [STAGE_CULLING](STAGE_CULLING.md).
- Two newly declared `frontier=true` units run these real TUs with a
  minimal scoped dynamic caller. Neither claims strict relocated-field
  replay or exactness. Build only `cull-stage-object-frontier` and
  `draw-stage-layer-frontier` when changing this pair.

## New source coverage (2026-11-17)

- `CullStageObject` 0x403950 (1,643 bytes) and `DrawStageLayer` 0x404190 (727 bytes) now have natural C++ with the actual 12-point D3DX projection, variable-length primitive traversal, and fog/Z state transitions. Both are source-present, **not byte-exact**. See [STAGE_CULLING](STAGE_CULLING.md).
- The auxiliary stage object at global 4A8D64 now has six source-present lifecycle owners totaling 796 bytes, including the 477-byte destructor. It shares the existing `g_MeshEffectEnemy` data owner; `g_MeshEffectActor` remains at 4A8EB4. The previously uninventoried `UpdateStageAuxiliary` at 406620 now covers the complete 232-byte switch **including its six-entry jump table**. All five mode updater callees now have maintained source (the original register/codegen ownership remains open). See [STAGE_AUXILIARY](STAGE_AUXILIARY.md).
- `SelectRuntimeCamera` at 405E80 adds 62 bytes of camera selection, ApplyCamera and D3D viewport source. All ten new/recovered function mappings are non-exact, apart from separately existing canonical functions revalidated below.
- A stale tracking mismatch was fixed: the three stage vector/timer exact rows now use their real C++ member symbols. Previously missing exact ledger entries for `UpdateStageObjects` (153 bytes), `StageUpdateCallback` (7 bytes) and `AdvanceStageTimer` (83 bytes) have fresh, individually cold-built strict zero-difference proofs. `AsciiManagerExpireStrings` is correctly diagnostic/non-exact, not a canonical exact unit.
- Latest generated ledger: **2,171 candidates; 773 source mappings; 457 canonical exact units**. This counts existing exact functions, not this session's new boss coverage. Verification: `scripts/repo-python scripts/validate-tracking.py --require-target`. The new source files and affected `StageDrawing.cpp` compile with pinned VC9; no full-cold replay was used.

### ExecuteStageScript — 0x00404470

- Native extent: 2,736 bytes = 2,633 code bytes, 3 alignment bytes and 100 bytes of switch tables.
- Current `execute-stage-script-frontier` PDB contribution: 2,728 bytes. Candidate switch-table boundary is +0xA44 versus native +0xA4C. Both reserve a 0x98-byte stack frame.
- Normalized `objdump -w` instruction-key alignment: 531 of 610 target instructions (621 candidate instructions). This is diagnostic only; the Boss remains non-exact and EH/funclet ownership is open.
- Current `src/StageScript.cpp` SHA-256: `80705232bd45ba1ea6bbd2109a104c19fe84d4ddc0ae1c1981912f1fc5a5f1a4`.
- Opcode 2 reuses the live Bezier A scratch vector for the new-minus-old camera-position delta. This improved alignment from 526 to 529 keys without changing the 0x98 frame.
- Opcodes 3 and 5 use `ScriptTweenFrame` with `_ReadWriteBarrier()` between observable field writes. The candidate retains the 0x98 frame and reaches 531 keys. One scheduling difference remains: candidate stores the scale pointer before `OR flags,1`; native performs the OR first. Do not call this timer block exact.
- Opcode 14 branches on the signed script ID before calculating the VM address and calls the real VM reset/bind routines. Keep the StageMesh profile override and dynamic constructor probe: they prevent LTCG from specializing the constructor for only the Boss call sites.

Rebuild and inspect only this owner with:

```bash
scripts/repo-python scripts/build-match-unit.py --unit execute-stage-script-frontier
```

### StepStageVectorTween — 0x00405240

- Native extent: 883 bytes. Current StageTween-family PDB contribution: 883 bytes.
- Normalized instruction-key alignment: 272/280. Matching size is not exactness; the remaining differences include Hermite x87 multiply order and final interpolation component scheduling.
- `StepStageFogTween` is 794 bytes in this StageTween context versus 737 native bytes. A separate wide secondary context retains the exact 737-byte proof; do not mix the two producer contexts.
- Seven neighboring exact StageTween functions were rebuilt and compared after the latest StageScript source change: `PackStageFog`, `SetStageTimer`, `StageVectorTweenFactor`, `SubtractStageFog`, `ScaleStageFog`, `AddStageFog` and `StageFogTweenFactor`. All seven comparisons passed with zero differences.

Rebuild the large tween owner with:

```bash
scripts/repo-python scripts/build-match-unit.py --unit step-stage-vector-tween
```

## Exact nearby owners worth preserving

- `AllocateStageHeapBytes` at 0x405D80: 10 bytes, one REL32 to CRT malloc.
- `ScaleStageVectorByReciprocal` at 0x405D90: 43 bytes; `StageVectorSquaredLength` at 0x405DC0: 33 bytes.
- `AddStageVectorComponents` at 0x405DF0: 25 bytes; natural member `operator+` recovers hidden result EAX, left ECX and right EDX.
- `StageTimerComparisonView::IsAtOrBefore` at 0x405E10: 15 bytes.
- `OffsetStageTimerByFrames` at 0x405E20: 16 bytes; `OffsetStageTimerByNegativeFrames` at 0x405E30: 20 bytes; `OffsetStageTimerByNegativeOne` at 0x405E50: 16 bytes.
- `ResetStageRenderTint` at 0x405E60: 26 bytes.
- `SetStageTweenControl1/2` at 0x405B50/0x405B30: 18 bytes each.
- `ResetAndBindStageAnimationVm` at 0x405B10: 30 bytes; two reviewed REL32 fields.
- `SeekStageScriptMarker` at 0x404F20: 113 bytes; `BeginSecondaryStageTransition` at 0x405130: 159 bytes; `BeginPrimaryStageTransition` at 0x4051D0: 103 bytes.
- `AdvanceStageTimer` at 0x459270: 83 bytes in its isolated linked-PE context.

These exact owners have focused strict replay records in the canonical match ledgers. Do not infer original class or translation-unit attribution from the source-side layout views where the ledger records provenance as unknown.

## Other open stage work

- `InitializePrimaryStageScripts` at 0x405030: 246-byte natural C++ linked candidate versus 249 native bytes; non-exact. Its frontier includes the real AnimationVm producer and private call ABI.
- `StageScriptMeshConstructor` at 0x40E530: 366-byte candidate versus 389 native bytes; generic ESI/EAX/stack ABI restored, still non-exact.
- `UpdateStage` and the large secondary gameplay caller remain non-exact.

## Shared contexts and outstanding verification

`src/StageScript.cpp` is also a support input to wide menu and clearing contexts. The `WideMenuJoystickDisplay` build exceeded the bounded 900-second run. `WideSecondaryClearing` was not rebuilt in this handoff pass. Do not describe either as reverified after the latest source change. Their retained output directories are kept for a future targeted attempt.

Useful current build outputs retained:

- `build/match-linked/StageScriptFrontier`
- `build/match-linked/StageTween`
- `build/match-linked/WideMenuJoystickDisplay`
- `build/match-linked/WideSecondaryClearing`
- `build/match-linked/SecondaryWideStepStageFogTween`

## Storage cleanup

On 2026-11-17, removed only six ignored, unreferenced experiment-output directories under `build/match-linked/`: `StageScriptVariants`, `StageMeshVariants`, `WidePauseSelectionComparisons`, `WideReplayLeaderboardRuntime`, `WideSecondaryHostMiscLeaves` and `WideSecondarySpecialEffectLifecycle`. Also removed regenerable `scripts/__pycache__`. No tracked files, source, probes, canonical manifest outputs, `.analysis` evidence, target, toolchain or Ghidra project were removed.

`build/` fell from 1,128,792 KiB to 517,212 KiB, reclaiming 611,580 KiB (about 597 MiB). The exact removal and size receipt is `.analysis/handoff-storage-cleanup-20261117.txt`. See `docs/ARTIFACTS.md` for retained-artifact policy.

## Next actions

1. Build focused VC9 /GL primary-TU contexts for the new **1,643-byte `CullStageObject`** and **727-byte `DrawStageLayer`** pair, then compare their complete native spans and real D3DX/device field bindings. Their current standalone source compilation is only a codegen sanity check.
2. Resolve the auxiliary stage's 406620 switch ABI and its five source-open mode updaters (407AC0, 407D10, 406D10, 4071A0, 407720). Confirm constructor/destructor EH ownership; do not label the new lifecycle owners exact on source coverage alone.
3. Continue `ExecuteStageScript` from 2,728/2,736 bytes and `StepStageVectorTween` from 883/883 bytes (still 272/280 normalized instructions). Focus on source-faithful x87 operand ordering and EH/funclet ownership without ASMs or inert padding.
4. Revisit `InitializePrimaryStageScripts` (246/249 diagnostic), `StageScriptMeshConstructor` (366/389), and wide contexts only when their affected producer/ABI paths warrant a bounded build. Never replay the full cohort for a local edit.

## Working references

- `AGENTS.md` — current priorities and exactness rules.
- `docs/RE_WORKFLOW.md` and `docs/ORACLES.md` — workflow and proof requirements.
- `docs/STAGE_SCRIPT.md`, `docs/STAGE_TWEEN.md` and `docs/STAGE_MESH.md` — detailed stage findings.
- `docs/STAGE_CULLING.md` and `docs/STAGE_AUXILIARY.md` — freshly recovered large culling/dispatch and auxiliary-lifetime owners.
- `docs/KNOWLEDGE_BASE.md` — durable cross-subsystem compiler and target findings.
- `docs/PROGRESS.md` — generated inventory; update from canonical ledgers, not by hand.