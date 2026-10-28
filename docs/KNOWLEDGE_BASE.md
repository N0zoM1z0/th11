# TH11 working knowledge

Current code and ledgers are authoritative. [RE_HANDOFF.md](RE_HANDOFF.md) is the
continuation entry; [PROGRESS.md](PROGRESS.md) supplies generated counts. Earlier
checkpoint narratives are retained in
[the historical archive](archive/KNOWLEDGE_BASE_2026-10-28.md), not as instructions.

## Target and compiler

Only original Japanese1.00a, SHA-256
`2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`.
The mapped text occupies `0x00401000..0x0048AE14` inclusive,564,757 bytes. Target identity
and complete mapped-text attestation are independent of Ghidra's inferred types.

TH11 PE/Rich evidence points to VC9 RTM15.00.21022.08/linker9.00.21022.08,
SDK6.0A. Preserve established /GL contexts and the pinned component hashes.
Default compiler loops use O2/Ob1/Gy/MT; declared unit profiles are authoritative.
Direct local scripts only; no Factory MCP or REA.

## Exactness and ownership

Compare complete compiler-owned functions, including owned alignment/tables.
Normal-COFF matches do not establish original TU or LTCG ownership. Linked
proofs require complete public primary-TU PDB contributions, current receipts,
PE/MAP/PDB identity and independently reviewed exhaustive fields. Static/EH/
split-owner acceptance is still open. Source presence and diagnostics grant
no exact credit. Full game build/runtime/portability are separate later work.

Useful boundary corrections:

- EvaluateStageEasing459730 owns764 bytes including its table; the old7-byte
  inventory covered only its dispatch jump.
- ExecuteStageScript404470 owns2,736 bytes including both jump tables.
- StageScriptMeshConstructor389 includes7 internal alignment bytes;
  PublishStageMesh123 includes3.
- Transparent-pixel processing owns1,427 bytes including switch tables.

## Physical and ABI facts

- MainRuntime base `0x004C3280`: flags `+0x590`, critical sections `+0x810`
  with stride `0x18`, depth bytes `+0x930`. Section9/depth9 are `+0x8E8/+0x939`.
- Animation VM size `0x434`; flags `+0x404`, file pointer `+0x3B0`, timer `+0x5C`.
  Twelve-byte list nodes start at `+4/+0x10`; renderer heads at `+0x7B562C/+0x7B5634`.
- AnimationVmId is modeled as a four-byte value object with a value constructor.
  It recovers native by-value stack lookup and hidden-result factory returns
  naturally. Reconstructed names do not establish original class names.
- Mesh size `0x18`: rows/columns, IDs, VM pointers, vertices, positions. Row-array
  allocations are4*rows-1 bytes; grid arrays use28-byte vertices and12-byte
  positions. Preserve the observed sizes.
- D3DSURFACE_DESC is32 bytes/eight DWORDs, including MultiSampleQuality before
  Width+24 and Height+28. Older seven-DWORD fixture claims were wrong.
- Native resource loading preserves signed counts, publication order and its
  RET trace sink. Opaque calls can change register preservation/codegen.

## Current compiler findings

- Row-VM initialization: Z, rhw, color, then pointer increment yields the
  complete168/170-byte exact creators. VC9 schedules color/Z/rhw and selects
  the native rhw-based induction pointer.
- SquareMeshVmCallback: save the Y field address before each X sample to recover
  all398 bytes. Constant template scales avoid an unwanted aligned frame.
- Column initialization: tracked source1022/498 diagnostic differences; retained
  sample-first experiment998/269. The latter remains unpromoted and EH-bearing.
- ResetStageMesh: complete325-byte source still has11 x87 load/add differences
  at+243..248/+279..283. Source position-prefix copying avoids extra float stores.
- Factory initialization: three fresh zero-vector returns recover171 bytes;
  sharing one zero local gives133. File ID assignment before flags reproduces
  the191-byte no-reset binding schedule.
- Conditional-expression returns in tween factor getters preserve the final
  float rounding point. Double locals and assignment-only returns differ.
- Vector tween mode7 uses old Z plus delta Z, not old Y.

## Topic index

| Work | Evidence and remaining differences |
| --- | --- |
| Mesh and effects | [STAGE_MESH](STAGE_MESH.md), [MESH_EFFECTS](MESH_EFFECTS.md), [VM factory](ANIMATION_VM_FACTORY.md) |
| Stage interpreter and interpolation | [STAGE_SCRIPT](STAGE_SCRIPT.md), [STAGE_TWEEN](STAGE_TWEEN.md), [STAGE_EASING](STAGE_EASING.md) |
| Stage lifecycle, updates and drawing | [STAGE_LIFECYCLE](STAGE_LIFECYCLE.md), [STAGE_UPDATE](STAGE_UPDATE.md), [STAGE_DRAWING](STAGE_DRAWING.md) |
| Stage/ANM resources | [STAGE_RESOURCE](STAGE_RESOURCE.md), [ANIMATION_CHUNKS](ANIMATION_CHUNKS.md), [ANIMATION_TEXTURES](ANIMATION_TEXTURES.md), [pixel processing](ANIMATION_TRANSPARENT_PIXELS.md) |
| Renderer and device | [RENDER_CAMERA](RENDER_CAMERA.md), [DEVICE_INITIALIZATION](DEVICE_INITIALIZATION.md), [RENDER_STATE_RESET](RENDER_STATE_RESET.md), [RUNTIME_SURFACES](RUNTIME_SURFACES.md) |
| Text | [TEXT_RESOURCES](TEXT_RESOURCES.md), [TEXT_DRAWING_COMPOSITE](TEXT_DRAWING_COMPOSITE.md), [OUTLINED_TEXT](OUTLINED_TEXT.md), [FORMATTED_TEXTURE_TEXT](FORMATTED_TEXTURE_TEXT.md) |
| File/archive | [RESOURCE_FILES](RESOURCE_FILES.md), [SHARED_FILE](SHARED_FILE.md), [ARCHIVE_DECOMPRESSION](ARCHIVE_DECOMPRESSION.md) |
| Callbacks, input, frame and startup | [CALLBACK_CHAIN](CALLBACK_CHAIN.md), [INPUT_POLLING](INPUT_POLLING.md), [FRAME_LOOP](FRAME_LOOP.md), [STARTUP_WINDOW](STARTUP_WINDOW.md) |

Compiler probes and exact cold proofs support current work. Retired behavior
suites and historical test counts are not prerequisites; cleanup does not
require a full-cohort replay.
