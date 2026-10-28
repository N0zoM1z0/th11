# Agent handoff

Current source and ledgers are the authority. Historical checkpoint notes are
archived in [archive/RE_HANDOFF_2026-10-24.md](archive/RE_HANDOFF_2026-10-24.md);
its counts and next-target suggestions are historical.

## Current state

- Target: original Japanese TH11 v1.00a, private `resources/th11.exe`;
  identity in [config/target.toml](../config/target.toml).
- Inventory:1,989 provisional candidates;83 reviewed/source-present mappings.
- Exact:45 functions /9,697 owned bytes:8 normal-COFF and37 linked-PE units.
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
headers or declared context changed. AnimationChunk.cpp and AnimationDocument.hpp
now belong to the new sprite exact context:

```bash
scripts/repo-python scripts/replay-exact-units.py \
  --unit create-animation-embedded-texture \
  --unit create-animation-empty-texture \
  --unit create-animation-render-target \
  --unit store-animation-sprite
```

## Latest completed code

| Function | Native bytes | State | Details |
| --- | ---: | --- | --- |
| CreateAnimationFileTexture,0x00453D90 | 484 | source483,70 diagnostic differences | [ANIMATION_TEXTURES.md](ANIMATION_TEXTURES.md) |
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

The three new texture units and affected sprite unit have current strict cold
primary-TU proofs. Earlier41 canonical inputs/proofs were not replayed or
modified. New private diagnostics agree on92 constructor cases, including four
matching faults, and58 chunk/sprite cases, including five integrated advance/
decode cases. The affected stage/document family was cold-rebuilt and its34
cases still agree. These model external helpers and grant no complete native/
runtime authority. Evidence/context details are in `ANIMATION_TEXTURES.md`.

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

## Useful next work

1. Review/recover transparent-pixel processing0x004534C0 (provisional1,381-byte
   candidate, ownership/tables not yet reviewed). Preliminary decompilation is
   `.analysis/animation-texture-create.c`; do not trust its COM/prototype errors.
   Its native EAX argument versus current opaque stack argument remains part
   of the file constructor's483/484-byte context gap.
2. Stage initialization0x004024C0 (630) and EH destruction0x00402740 (603) have
   private complete instruction/decompiler review but no source. Original EH/
   specialized ownership is outside the current public linked acceptance lane.
3. Improve existing non-exact functions only with new evidence; read the
   corresponding topic before repeating a compiler/context experiment.

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
