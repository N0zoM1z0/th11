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
- DrawAsciiManager401670 owns1,636 bytes through401CD3: the old1,618-byte
  span omitted two alignment bytes and the four-entry switch table.
- ApplyAnimationVmDrawState44F710 owns368 bytes through44F87F, including
  two alignment bytes and seven switch entries after its338-byte code body.

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
- Column initialization: sample RNG before selecting each row address. Current
  source recovers998 bytes/269 diagnostic differences instead of1022/498; EH
  ownership and the remaining codegen are open.
- ResetStageMesh: complete325-byte source still has11 x87 load/add differences
  at+243..248/+279..283. Source position-prefix copying avoids extra float stores.
- Factory initialization: three fresh zero-vector returns recover171 bytes;
  sharing one zero local gives133. File ID assignment before flags reproduces
  the191-byte no-reset binding schedule.
- Conditional-expression returns in tween factor getters preserve the final
  float rounding point. Double locals and assignment-only returns differ.
- Vector tween mode7 uses old Z plus delta Z, not old Y.

- ASCII lifecycle: the98-byte constructor really clears the full object after
  constructing its two VMs, including its vptr. Missing callback entries4014C0
  and4014D0 own9/7 bytes. Sprite binding writes four UV corners at70..8C;
  interpreting those eight floats as two current/base rectangles is wrong.

- ASCII small-font formatting writes `strings[count-1].unknown120 = 1`.
  Native count*130+86C selects font style, not the draw channel at+128.
- VM+374 is diffuse ARGB, not a sprite index. Native default ASCII advance
  converts drawMode184A4 as signed (FILD), despite its older unsigned view label.
- Independent renderer/camera inputs restore the full101-byte overlay owner;
  global-only support had specialized parameters away and produced87 bytes.

- BuildAnimationVmQuad: real submission caller context naturally restores the
  native16-byte frame and closes all632 bytes, together with the40-byte floor
  adapter and its native64-byte-aligned frame. The former narrow context
  produced53 stack-only differences; masking never granted exact credit.
- Submission uses current-x87-RC FRNDINT then subtracts0.5. Original CRT
  `_frnd`489CED exactly equals the20-byte VC9 archive member, providing a real
  cdecl primitive for complete natural-C++ submission source. Four-mode ABI
  checks retain the rounding control. It is a library exclusion, not authored
  source/exact credit.
- Native viewport rejection uses ordered separating comparisons. Rewriting
  them as positive <= tests rejects NaN bounds and changes the actual paths.
- Projected quads use fixed sprite-space anchors +/-128 and +/-256. Rebuilding
  copies VM+2B4 to+2F4, scales only _11/_22 and clears dirty flags8/4 around
  nonzero/NaN X/Y/Z rotations. Working _41/_42 add cached translations; _43
  replaces the cached translation. D3DX imports are independently verified.
- Software fog retains float stores between offset+position, extraOffset and
  camera subtraction; it rejects ordered factor>=1 and truncates all four
  output channels. The projected wrapper resets RHW after submission/culling.
- VC9 rejects `#pragma intrinsic(_frnd)` with C4163. Precise sin/cos probes
  call CRT intrinsics; /fp:fast emits separate FSIN/FCOS. Neither experiment
  closes native inline FRNDINT or paired FSINCOS; those gaps remain open.
- Blend selector6 uses INVSRCCOLOR4, not SRCCOLOR3. Selector7 updates cache
  without blend API calls. The state application counter increments every call.
- Tint channels use byte inputs and an unsigned promoted product before SHR7.
  Leaving byte multiplication as signed promotion emits SAR, one opcode off.

## Topic index

| Work | Evidence and remaining differences |
| --- | --- |
| Mesh and effects | [STAGE_MESH](STAGE_MESH.md), [MESH_EFFECTS](MESH_EFFECTS.md), [VM factory](ANIMATION_VM_FACTORY.md) |
| Stage interpreter and interpolation | [STAGE_SCRIPT](STAGE_SCRIPT.md), [STAGE_TWEEN](STAGE_TWEEN.md), [STAGE_EASING](STAGE_EASING.md) |
| Stage lifecycle, updates and drawing | [STAGE_LIFECYCLE](STAGE_LIFECYCLE.md), [STAGE_UPDATE](STAGE_UPDATE.md), [STAGE_DRAWING](STAGE_DRAWING.md) |
| Stage/ANM resources | [STAGE_RESOURCE](STAGE_RESOURCE.md), [ANIMATION_CHUNKS](ANIMATION_CHUNKS.md), [ANIMATION_TEXTURES](ANIMATION_TEXTURES.md), [pixel processing](ANIMATION_TRANSPARENT_PIXELS.md) |
| Renderer quad construction/submission | [ANIMATION_QUAD](ANIMATION_QUAD.md), [ANIMATION_VM_DRAW](ANIMATION_VM_DRAW.md), [ANIMATION_PROJECTED_QUAD](ANIMATION_PROJECTED_QUAD.md) |
| Renderer and device | [RENDER_CAMERA](RENDER_CAMERA.md), [DEVICE_INITIALIZATION](DEVICE_INITIALIZATION.md), [RENDER_STATE_RESET](RENDER_STATE_RESET.md), [RUNTIME_SURFACES](RUNTIME_SURFACES.md) |
| ASCII lifecycle, glyph drawing and formatting | [ASCII_LIFECYCLE](ASCII_LIFECYCLE.md), [ASCII_DRAWING](ASCII_DRAWING.md) |
| Text | [TEXT_RESOURCES](TEXT_RESOURCES.md), [TEXT_DRAWING_COMPOSITE](TEXT_DRAWING_COMPOSITE.md), [OUTLINED_TEXT](OUTLINED_TEXT.md), [FORMATTED_TEXTURE_TEXT](FORMATTED_TEXTURE_TEXT.md) |
| File/archive | [RESOURCE_FILES](RESOURCE_FILES.md), [SHARED_FILE](SHARED_FILE.md), [ARCHIVE_DECOMPRESSION](ARCHIVE_DECOMPRESSION.md) |
| Callbacks, input, frame and startup | [CALLBACK_CHAIN](CALLBACK_CHAIN.md), [INPUT_POLLING](INPUT_POLLING.md), [FRAME_LOOP](FRAME_LOOP.md), [STARTUP_WINDOW](STARTUP_WINDOW.md) |

Compiler probes and exact cold proofs support current work. Retired behavior
suites and historical test counts are not prerequisites; cleanup does not
require a full-cohort replay.
