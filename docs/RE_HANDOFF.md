# Agent handoff

Current source and ledgers are the authority. Historical checkpoint notes are
archived in [archive/RE_HANDOFF_2026-10-24.md](archive/RE_HANDOFF_2026-10-24.md);
its counts and next-target suggestions are historical.

## Current state

- Target: original Japanese TH11 v1.00a, private `resources/th11.exe`;
  identity in [config/target.toml](../config/target.toml).
- Inventory:1,992 provisional candidates;95 reviewed/source-present mappings.
- Exact:50 functions /10,476 owned bytes:8 normal-COFF and42 linked-PE units.
  [PROGRESS.md](PROGRESS.md) is generated from maintained ledgers.
- Full Windows i386 game build/runtime closure is open. Diagnostic executables
  are non-runnable link contexts; modeled cases are not full runtime acceptance.
- User requests direct local tools, no Factory MCP, TH11's VC9 RTM toolchain, useful medium functions,
  controlled CPU/memory and validation limited to changed units/dependencies.

## Resume

Read [AGENTS.md](../AGENTS.md), [RE_WORKFLOW.md](RE_WORKFLOW.md) and
[ORACLES.md](ORACLES.md), then the relevant topic/source/ledger rows. Work in
`/home/pentester/coding/codex_ida/th11-reconstruction/th11`.

```bash
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/progress.py --check
```

Do not replay all exact units on resume. Revalidate only the units whose source,
headers or declared context changed. The new StageResource.hpp/lifecycle context
affects these five units; it does not require replaying the older cohort:

```bash
scripts/repo-python scripts/replay-exact-units.py \
  --unit initialize-stage-resource \
  --unit stage-embedded-vm-constructor \
  --unit stage-embedded-vm-destructor \
  --unit stage-draw-callback \
  --unit stage-overlay-callback
```

## Latest completed code

| Function | Native bytes | State | Details |
| --- | ---: | --- | --- |
| DrawStage,0x00403130 | 1,128 | source1,106; non-exact | [STAGE_DRAWING.md](STAGE_DRAWING.md) |
| DrawStageOverlay,0x004035A0 | 854 | source852; non-exact | [STAGE_DRAWING.md](STAGE_DRAWING.md) |
| InitializeStageResource,0x004024C0 | 630 | exact,21 fields | [STAGE_LIFECYCLE.md](STAGE_LIFECYCLE.md) |
| StageEmbeddedVmConstructor,0x00402270 | 101 | exact,1 field | [STAGE_LIFECYCLE.md](STAGE_LIFECYCLE.md) |
| StageEmbeddedVmDestructor,0x00402170 | 34 | exact,1 field | [STAGE_LIFECYCLE.md](STAGE_LIFECYCLE.md) |
| StageResourceConstructor,0x004023F0 | 180 | diagnostic match; EH ownership open, no exact credit | [STAGE_LIFECYCLE.md](STAGE_LIFECYCLE.md) |
| StageResourceDestructor,0x00402740 | 603 | source597/143 differences; EH ownership open | [STAGE_LIFECYCLE.md](STAGE_LIFECYCLE.md) |
| CreateStageResource,0x004029C0 | 159 | diagnostic match; EH ownership open, no exact credit | [STAGE_LIFECYCLE.md](STAGE_LIFECYCLE.md) |
| StageUpdateCallback,0x00403900 | 7 | source7/3 differences; callee private ABI open | [STAGE_LIFECYCLE.md](STAGE_LIFECYCLE.md) |
| StageDrawCallback/StageOverlayCallback,0x00403910/0x00403920 | 7 each | exact,1 field each | [STAGE_LIFECYCLE.md](STAGE_LIFECYCLE.md) |
| FixAnimationTextureTransparentPixels,0x004534C0 | 1,427 including tables | source1383, non-exact | [ANIMATION_TRANSPARENT_PIXELS.md](ANIMATION_TRANSPARENT_PIXELS.md) |
| CreateAnimationFileTexture,0x00453D90 | 484 | source484,40 diagnostic differences | [ANIMATION_TEXTURES.md](ANIMATION_TEXTURES.md) |
| CreateAnimationEmbeddedTexture,0x00453F80 | 287 | exact,10 fields | [ANIMATION_TEXTURES.md](ANIMATION_TEXTURES.md) |
| CreateAnimationEmptyTexture,0x004540A0 | 67 | exact,5 fields | [ANIMATION_TEXTURES.md](ANIMATION_TEXTURES.md) |
| CreateAnimationRenderTarget,0x004540F0 | 69 | exact,5 fields | [ANIMATION_TEXTURES.md](ANIMATION_TEXTURES.md) |
| StoreAnimationSprite,0x00454B10 | 149 | exact, no fields | [ANIMATION_CHUNKS.md](ANIMATION_CHUNKS.md) |
| DecodeAnimationChunk,0x004545A0 | 811 | source811,10 diagnostic differences | [ANIMATION_TEXTURES.md](ANIMATION_TEXTURES.md) |
| AdvanceAnimationDocument,0x004544C0 | 212 | current texture family211,118 differences | [ANIMATION_CHUNKS.md](ANIMATION_CHUNKS.md) |
| LoadAnimationChunkSource,0x004543E0 | 221 | current texture family209,147 differences | [ANIMATION_CHUNKS.md](ANIMATION_CHUNKS.md) |
| ReleaseVmBindingBlock,0x00402300 | 231 | exact,6 bindings | [VM_BINDINGS.md](VM_BINDINGS.md) |
| LoadStageResource,0x00403FC0 | 297 | source297,17 diagnostic differences | [STAGE_RESOURCE.md](STAGE_RESOURCE.md) |
| BeginLoadAnimationDocument,0x00454190 | 454 | source434,337 differences | [STAGE_RESOURCE.md](STAGE_RESOURCE.md) |
| AcquireAnimationDocument,0x00454360 | 124 | source131,80 differences | [STAGE_RESOURCE.md](STAGE_RESOURCE.md) |

The five new non-EH units have strict cold primary-TU proofs, adding779 bytes
and25 reviewed fields. Earlier45 exact contexts were not modified or replayed.
New complete-body diagnostics agree on48 lifecycle cases, including five
matching null-allocation faults; actual VM bodies run inside modeled CRT vector
iteration. The directly affected old loader family was cold-rebuilt and its34
cases still agree. EH exception injection/full unwind and the native product
remain unvalidated; constructor180/factory159 diagnostic matches do not grant
exact credit. See `STAGE_LIFECYCLE.md`.

Preserve these behavior details:

- Cleanup visits only count-1 ID slots, searches the two renderer lists in
  order and marks related VMs only when marker+18 is zero, without recursion.
- Document publication precedes raw-data/decoder success. A null result can
  leave an incomplete non-null cached document; cached acquisition does not
  check busy. Fresh acquisition sleeps before polling unless flags0x180 exit.
- Stage counts are signed16-bit; VM malloc failure is not checked. No extra
  validation or cleanup has been added to alter native failure behavior.
- The native trace sink is RET and preserves registers that the opaque source
  context spills. This old trace/LTCG gap remains open; no fake ABI or padding.
- Busy is a one-based postload cursor: the worker rescans counts and decodes
  at most one chunk per call, then increments or clears it.0x004543E0 only loads
  external source bytes;0x004545A0 is the actual decoder. The old role guesses
  were wrong. Sprite+44 remains indeterminate; do not zero-initialize it.
- D3DSURFACE_DESC is32 bytes: eight DWORDs including MultiSampleQuality before
  Width+24/Height+28. Do not reuse the old seven-DWORD descriptor fixtures.
- Pixel processing uses signed per-row pitch and signed pitch/2 or pitch/4
  vertical neighbor offsets. Alpha stays unchanged. Unsupported formats still
  lock/unlock/release, and HRESULTs remain ignored.
- Stage construction initializes eight+three embedded VMs and then clears the
  entire0x3158-byte object, overwriting their just-set-1 markers. Preserve that
  whole-object clear. Init publishes variant/current-stage before load; null
  chain allocations and null factory allocation retain native faults.
- Lifecycle tests need a separate FS TEB segment and real32-bit SS/CS/data GDT
  descriptors. Mapping the null page to fake FS hides null-chain faults; writing
  Unicorn FS_BASE alone does not work for this32-bit fixture.

Latest drawing checkpoint adds two complete non-exact owners totaling1,982
target bytes. Source contributions are1,106/852 bytes versus native1,128/854;
no new exact claim. Full modeled checks agree on952 cases including four null
faults and128 persistent three-frame sequences. The shared layout refinements
kept all offsets intact; only the five affected canonical lifecycle/callback
units were cold-replayed, all passing. Private probe/check scripts are
`.analysis/probe-stage-drawing.py`, `.analysis/stage_drawing_fixture.py` and
`.analysis/check-stage-drawing.py`; build family is
`build/probe-ltcg/stage-drawing`. Remaining private ABI dependencies explain
some codegen differences; do not substitute invented calling conventions.
The48 lifecycle and34 rebuilt loader cases still agree; public CI passed8+25
tests. Current stage-drawing logs and reports are private `.analysis/` files.

## Useful next work

1. Stage update0x00402AA0 (1,677 bytes) still has no source. Complete private
   disassembly is now in `.analysis/stage-main-full-review.txt`, with decompiler
   hypotheses in `.analysis/stage-main.c`. Reconcile all math dependencies and
   ownership before source credit. Update receives private EAX, explaining the
   remaining seven-byte update adapter mismatch. Draw/overlay now have complete
   source and952 full-body modeled comparisons; see `STAGE_DRAWING.md`. Their
   unresolved external helpers remain prototypes, not source coverage.
2. Improve the pixel helper's source0x3C/target0x44 frame and the file
   constructor's source0x44/target0x4C frame with new evidence. The ordinary
   source pixel call now naturally uses native EAX. Byte versus typed32-bit
   pixel and truth versus unsigned-positive tests produced identical1383-byte
   code; do not repeat them. Keep the complete1427-byte owner, including tables.
   The current Oracle cannot account for a final selector byte and no pixel
   exact unit is configured; do not crop or claim masked equality.
3. Improve existing non-exact functions only with new evidence; read the
   corresponding topic before repeating a compiler/context experiment.

Neighbor code at0x00453A60 is absent from the provisional inventory and has no
mapping/source credit. Its existence was checked after the pixel owner's13-byte
CC padding, but its full extent/ABI/origin require review before recovery.

The three callback entries0x00403900/10/20 were absent from initial inventory;
they were explicitly reviewed and added, raising the candidate count by3.
Their source attribution remains indeterminate. Stage constructor/factory/EH
destructor now have natural C++ source, but specialized EH handler/funclet
ownership acceptance remains open.

Decoder0x004545A0 now has811 source bytes/10 diagnostic differences. Remaining
positions are five sprite setup register/store positions and five script-loop
entry arithmetic positions. Do not repeat the documented cached-entry,
float-pointer, pointer-temporary or extra-direct-caller contexts unchanged.

The worker dispatcher0x004548D0 (135 bytes) was privately reviewed but is not
mapped/source-present. Its destruction path clears a slot then writes through
the null slot; preserve the observed fault rather than silently fixing it.

The mode-transition function0x00406510 is unrelated to VM lookup. Its241-byte
code candidate has a trailing six-entry table and needs a complete ownership
review before implementation or credit.

## Existing context gaps

| Area | Retained evidence and limits |
| --- | --- |
| Formatted text | [FORMATTED_TEXTURE_TEXT.md](FORMATTED_TEXTURE_TEXT.md):156/224 exact; centered212 has13 stack-offset differences. Editing the shared source affects both exact siblings. |
| Outlined/plain text | [OUTLINED_TEXT.md](OUTLINED_TEXT.md), [TEXT_DRAWING_COMPOSITE.md](TEXT_DRAWING_COMPOSITE.md):full source and bounded models; original frame/type/TU gaps remain. |
| Device/startup/window | [DEVICE_INITIALIZATION.md](DEVICE_INITIALIZATION.md), [STARTUP_WINDOW.md](STARTUP_WINDOW.md):do not repeat recorded vector/aggregate/member/argument-order grids. |
| Renderer/surfaces/frame | [RENDER_STATE_RESET.md](RENDER_STATE_RESET.md), [RUNTIME_SURFACES.md](RUNTIME_SURFACES.md), [FRAME_LOOP.md](FRAME_LOOP.md), [WINDOW_SURFACE_BUFFER.md](WINDOW_SURFACE_BUFFER.md):near matches are not exact. |
| Camera/archive/screenshot | [RENDER_CAMERA.md](RENDER_CAMERA.md), [ARCHIVE_DECOMPRESSION.md](ARCHIVE_DECOMPRESSION.md), [SCREENSHOT.md](SCREENSHOT.md):keep complete extents and observed error/ABI behavior. |
| Callback/runtime data | [CALLBACK_CHAIN.md](CALLBACK_CHAIN.md):MainRuntime grouping preserves real store/test order; do not migrate unrelated partial views gratuitously. |

Detailed durable facts remain in [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md). The
animation VM interpreter0x0044B4B0 remains unimplemented; its prototype is not
source coverage.

## Tools and local evidence

VC9 RTM compiler15.00.21022.08/linker9.00.21022.08, SDK6.0A, Ghidra12.1.3 and
JDK21 remain pinned in [tools.lock.toml](../config/tools.lock.toml). Ghidra/Wine
share `.tools/heavy-tool.lock`, one CPU and nice10; Ghidra heap768 MiB plus JVM
native overhead. Use `scripts/ghidra.py` directly, without REA.

Private current proofs are under `build/match*`. Current diagnostic artifacts,
probe/check scripts, dumps and reports remain available; obsolete compiled
variants were removed. See [ARTIFACTS.md](ARTIFACTS.md) for what was retained,
what can be regenerated and the cleanup audit. No original target, tools or
Ghidra database was removed. Keep private payloads out of commits.
