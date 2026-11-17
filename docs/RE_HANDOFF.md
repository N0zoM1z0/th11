# TH11 reconstruction handoff — 2026-11-17

## Target and workflow

Reconstruct only the original Japanese TH11 v1.00a executable at `resources/th11.exe` (SHA-256 `2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`). Use the pinned VC9 / Visual C++ 2008 RTM build 21022 toolchain. Keep the original executable, `.tools/`, Wine prefix and Ghidra project private. Use direct `scripts/ghidra.py`; do not substitute another game executable or toolchain.

Current branch is `main` (no push). The latest session reconstructed mode-one/four initialization, the complete 1,777-byte stage-spell initializer, 370-byte spell HUD owner, 80-byte checksum leaf, and 368-byte timing settlement function, all in natural C++ with `gpt-web:` commits through `417a201`. Diagnostic LTCG frontiers and source refinements were saved through `234493f`. Preserve existing dirty work; compile only affected units, never the full cold replay.

## Current spell-stage chain and evidence (2026-11-17)

- **Six complete new source mappings / 2,823 target bytes**: mode-one (407A40,125 B) and mode-four (407130,103 B) special initialization, `InitializeSecondarySpellStage` (40C650,1,777 B), `DrawSecondarySpellStage` (40C4A0,370 B), `CheckSecondarySpellRecordCode` (40BB50,80 B), `SettleSecondarySpellStageTime` (40C310,368 B). Their physical runtime view is shared in `src/SecondarySpellStage.hpp`; no new executable ASM or padding was introduced. All compile with pinned VC9. **Current ledger: 2,171 candidates, 789 source mappings, 458 canonical exact units.** These six owners remain non-exact.
- **Original jump-table ownership:** The 1,777-byte native initializer covers 40C650..40CD40, with its seven-entry switch table separately at 40CD44..40CD5F. Ghidra's seven interior references are compiler table edges, not foreign code/function entries. The source uses a normal C++ `switch`. `secondary-spell-stage-init-frontier` produces a 1,560-byte candidate with **115/468** normalized native instruction keys aligned; the original private EAX string/RET8 ABI and real full-caller closure remain open.
- **HUD drawing:** `secondary-spell-stage-draw-frontier` has a **384-byte** PDB owner vs 370 native and **65/89** aligned keys. Branch-specific natural float-position initialization improved the earlier 352-byte / 53-key candidate, but the native private EAX receiver and `AsciiManagerPrint` call ABI are not exact.
- **Score/clock chain:** Timer double fields at stage+8FC/+904 require *4-byte packing*; the 368-byte settlement uses original `GetFrameClockSeconds`, `fmod` by 0.0167, threshold 0.00835, signed floor/hundredths and the real attempt/replay record tables. The 80-byte signed checksum leaf at 40BB50 checks stage+90C. Normal-COFF diagnostic for timing settlement is 356 bytes vs 368 native (25/300 comparable bytes); **not exact**. No full cohort was replayed. See [SECONDARY_SPELL_STAGE](SECONDARY_SPELL_STAGE.md).

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
- Latest generated ledger: **2,171 candidates; 785 source mappings; 458 canonical exact units**. This counts existing exact functions, not this session's new boss coverage. Verification: `scripts/repo-python scripts/validate-tracking.py --require-target`. The new source files and affected `StageDrawing.cpp` compile with pinned VC9; no full-cold replay was used.
- The mode-3 clearing dependency now has natural source for 406F90 (212 bytes), 424F30 (114-byte virtual-area dispatcher), and 40B0E0 (300-byte projectile-band scan). These reuse the real scene/secondary manager and do not alter any prior exact clearing units. Target original 406F90 only initializes the first two floats of the area extent; its third float is unknown, not assumed zero. See [STAGE_AUXILIARY](STAGE_AUXILIARY.md) and [SECONDARY_CLEARING](SECONDARY_CLEARING.md).
- The 40B0E0 horizontal-band dependency now has `ClearSecondaryActorsInHorizontalBand` (412B10, complete 312-byte extent) and the **exact** `SecondaryActorViewportPoint::IsOutside` (419920, 93 bytes). That native thiscall leaf has a fresh 93/93 strict pinned VC9 COFF proof and all four reviewed double-constant relocations. Two adjacent heavy retaliation owners also have source: 40B210 (395 bytes; scene-projectile scan and retaliating spawn) and 412C50 (449 bytes; actor-radius scan and retaliating spawn). Both remain **non-exact**.
- The two new narrowly scoped LTCG frontiers use a real mode-1 dynamic caller: `auxiliary-scene-radius-frontier` emits 397 bytes vs native 395 (85/107 aligned native instruction keys), `auxiliary-actor-radius-frontier` emits 441 vs native 449 (121/125 aligned). The actor source now preserves original unordered viewport comparison outcomes, while the scene source avoids rounding the heading before adding pi. Both still have private spawn/drop call ABI gaps; a trial adding full `SecondaryPattern.cpp` and `SecondaryPlayerDrop.cpp` support reduced actor alignment and was reverted. Details: [SECONDARY_CLEARING](SECONDARY_CLEARING.md).

### New stage-special activation and spell owners (2026-11-17)

- Fixed a real source bug in `ActivateSecondaryPlayerSpecial` (406510): native owner is **4A8D64** auxiliary stage, *not* player 4A8EB4; all six mode cases now receive the actual auxiliary owner. Positional sound still independently reads the player.
- Three complete initialization leaves are now natural C++: `InitializeSecondarySpecialMode1` (407A40, 125 bytes), `InitializeSecondarySpecialMode4` (407130, 103 bytes), and `InitializeSecondarySpecialMode5` (407680, 159 bytes). Mode-five reloads the player pointer **after** VM creation, whereas mode-one snapshots it before the optional sound.
- `UpdateSceneProjectileBoundaryEffects` at 40B3A0 now covers a 537-byte original owner scanning the scene projectile pool, performing bounds intersection, spawning a positioned VM and selecting one of three sprite-index tables by sprite width. `UpdateSecondarySpellStageState` at 40C0A0 now covers 624 original bytes: two embedded VMs, timer, score, three related-animation events and player/scene position interpolation. Both compile but remain non-exact. Details in [SECONDARY_CLEARING](SECONDARY_CLEARING.md) and [SECONDARY_SPELL_STAGE](SECONDARY_SPELL_STAGE.md).
- Scoped pinned VC9 frontiers: six-way activation has a 244-byte PDB code candidate versus 241 code bytes/268 full native extent (65/88 native code keys aligned); mode-five initializer is 158 versus 159 bytes (25/40). The seven wide exact-context families which include the modified activation TU were **not** rerun; the narrow diagnostic builds are not replacements for their canonical receipts.
- `StepStageVectorTween` remains at its 883-byte/272-of-280-instruction diagnostic baseline. Several natural term-order variants were tried and reverted; the highest aligned variant reached 274/280 but became 887 bytes, not a byte-exact improvement.

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
- `SecondaryActorViewportPoint::IsOutside` at 0x419920: 93-byte exact member, four reviewed native double-pool DIR32 bindings, no assembly or padding.

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

1. Continue `InitializeSecondarySpellStage` (1777/1560 native/candidate bytes, 115/468 diagnostic keys) with its **real original caller** and VC9 private EAX/stack ABI. The switch table is separately owned at 40CD44; keep it out of the function-size claim. Also refine `DrawSecondarySpellStage` (370/384, 65/89), `SettleSecondarySpellStageTime` (368/356 normal COFF), and the 80-byte checksum leaf. Prefer natural expressions and exact spell-record field semantics to code-size tricks.
2. Resume `ExecuteStageScript` (2,736 bytes, candidate 2,728, 531/610 keys), `StepStageVectorTween` (883 bytes, candidate 883, 272/280 keys), and the large `CullStageObject`/ `DrawStageLayer` pair. The tween has measured source-order variants in ignored `.analysis`; none met strict exactness and existing source was restored.
3. Recheck native 0x406510 six-way special activation and its 407A40/407130/407680 mode initializers in a real caller context; the singleton receiver is **4A8D64**, not 4A8EB4. Do not claim seven unrelated broad downstream exact units were rebuilt by the local source compiles.
4. Keep validations bounded to edited sources and their actual producer dependencies. Run a full cold replay only if explicitly needed and explained; no ASM, executable patch or padding.

## Working references

- `AGENTS.md` — current priorities and exactness rules.
- `docs/RE_WORKFLOW.md` and `docs/ORACLES.md` — workflow and proof requirements.
- `docs/STAGE_SCRIPT.md`, `docs/STAGE_TWEEN.md` and `docs/STAGE_MESH.md` — detailed stage findings.
- `docs/STAGE_CULLING.md` and `docs/STAGE_AUXILIARY.md` — freshly recovered large culling/dispatch and auxiliary-lifetime owners.
- `docs/KNOWLEDGE_BASE.md` — durable cross-subsystem compiler and target findings.
- `docs/PROGRESS.md` — generated inventory; update from canonical ledgers, not by hand.