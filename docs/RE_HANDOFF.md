# Agent handoff

Current source and ledgers are the authority. Historical checkpoint notes are
archived in [archive/RE_HANDOFF_2026-10-24.md](archive/RE_HANDOFF_2026-10-24.md);
its counts and next-target suggestions are historical.

## Current state

- Target: original Japanese TH11 v1.00a, private `resources/th11.exe`;
  identity in [config/target.toml](../config/target.toml).
- Inventory:1,992 provisional candidates;109 reviewed/source-present mappings.
- Exact:55 functions /11,554 owned bytes:8 normal-COFF and47 linked-PE units.
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

Do not replay all exact units on resume. Revalidate only units whose declared
source, headers or context changed. The latest easing checkpoint affected
these five canonical units across three families:

```bash
scripts/repo-python scripts/replay-exact-units.py \
  --unit set-stage-timer --unit pack-stage-fog \
  --unit evaluate-stage-easing \
  --unit stage-vector-tween-factor --unit stage-fog-tween-factor
```

## Latest completed code

| Function | Native bytes | State | Details |
| --- | ---: | --- | --- |
| StepStageVectorTween/StepStageFogTween,0x00405240/0x004055D0 | 883/737 | source785/534; non-exact | [STAGE_TWEEN.md](STAGE_TWEEN.md) |
| EvaluateStageEasing,0x00459730 | 764 including table | exact;35 fields | [STAGE_EASING.md](STAGE_EASING.md) |
| StageVectorTweenFactor/StageFogTweenFactor,0x00405960/0x00405990 | 39/45 | exact;1 field each | [STAGE_EASING.md](STAGE_EASING.md) |
| SetStageTimer,0x00406100 | 59 | exact;1 field | [STAGE_TWEEN.md](STAGE_TWEEN.md) |
| AdvanceStageTimer,0x00459270 | 83 | source83/8 diagnostic differences | [STAGE_TWEEN.md](STAGE_TWEEN.md) |
| ExecuteStageScript,0x00404470 | 2,736 including tables | source2,728; non-exact, EH open | [STAGE_SCRIPT.md](STAGE_SCRIPT.md) |
| PackStageFog,0x00405B70 | 171 | exact; no fields | [STAGE_SCRIPT.md](STAGE_SCRIPT.md) |
| UpdateStage,0x00402AA0 | 1,677 | source1,705; non-exact | [STAGE_UPDATE.md](STAGE_UPDATE.md) |
| UpdateStageObjects,0x004040F0 | 153 | source151; non-exact | [STAGE_UPDATE.md](STAGE_UPDATE.md) |
| DrawStage,0x00403130 | 1,128 | source1,106; non-exact | [STAGE_DRAWING.md](STAGE_DRAWING.md) |
| DrawStageOverlay,0x004035A0 | 854 | source852; non-exact | [STAGE_DRAWING.md](STAGE_DRAWING.md) |
| InitializeStageResource,0x004024C0 | 630 | exact,21 fields | [STAGE_LIFECYCLE.md](STAGE_LIFECYCLE.md) |
| StageEmbeddedVmConstructor,0x00402270 | 101 | exact,1 field | [STAGE_LIFECYCLE.md](STAGE_LIFECYCLE.md) |
| StageEmbeddedVmDestructor,0x00402170 | 34 | exact,1 field | [STAGE_LIFECYCLE.md](STAGE_LIFECYCLE.md) |
| StageResourceConstructor,0x004023F0 | 180 | diagnostic match; EH ownership open, no exact credit | [STAGE_LIFECYCLE.md](STAGE_LIFECYCLE.md) |
| StageResourceDestructor,0x00402740 | 603 | source597/143 differences; EH ownership open | [STAGE_LIFECYCLE.md](STAGE_LIFECYCLE.md) |
| CreateStageResource,0x004029C0 | 159 | diagnostic match; EH ownership open, no exact credit | [STAGE_LIFECYCLE.md](STAGE_LIFECYCLE.md) |
| StageUpdateCallback,0x00403900 | 7 | integrated source10 bytes; callee private ABI open | [STAGE_LIFECYCLE.md](STAGE_LIFECYCLE.md) |
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

Latest update checkpoint adds UpdateStage1,677 and UpdateStageObjects153
reviewed target bytes, with source1,705/151. No new exact unit. All853 modeled
cases agree, including nine matching faults under explicit dependencies; angle
normalization and RNG bodies execute directly, with additional full object-body
cases. New views refine camera+F0 and stage+3014..3028 without moving fields.
The same five affected canonical units passed cold replay. Drawing/loader
families were rebuilt with unchanged diagnostics;48 lifecycle cases also pass.
Private `.analysis/probe-stage-update.py`, `.analysis/stage_update_fixture.py`
and `.analysis/check-stage-update.py` use `build/probe-ltcg/stage-update`.
Its callback address is genuinely published by the probe to keep the fastcall
contract. Update compiles with private EBX instead of native EAX and the escaped
callback is10 bytes; the direct-only five-byte variant is not an exact proof.
See `STAGE_UPDATE.md` for floating-point and dependency-model limits.
The rebuilt drawing952 and loader34 regressions passed, along with public
CI8+25 checks. Exact/source ledgers remain the authority for separate claims.

Latest script checkpoint recovers the complete2,736-byte interpreter owner:
2,633 code bytes +3 alignment bytes +100 bytes in18+7 destination tables.
Source2,740 remains non-exact, with EH/funclet context open. Fog packing171 is
strictly exact with zero linked fields, raising exact totals to51/10,647. Only
its new canonical unit `pack-stage-fog` was cold-replayed; the prior50 contexts
have unchanged inputs. Complete modeled checks agree on709 cases including
five faults under declared dependencies and20 persistent six-frame programs.
They execute actual fog packing and angle normalization. Timers/interpolation,
VM reset/bind, construction and allocation remain models; full exception unwind
and native product closure are open.

Private `.analysis/probe-stage-script.py`, `.analysis/stage_script_fixture.py`
and `.analysis/check-stage-script.py` use the canonical
`build/match-linked/StageScript` for final checks. The fixture can select a
private context with `TH11_STAGE_SCRIPT_TEST_DIR`. Direct vector writes reduced
3,048 bytes/frame120 to2,724/frame68; hybrid tween aggregates gave2,924/frameF8.
Final normal-COFF angle support restores the target minimum-radius comparison,
producing2,740/frame68 while the primary source retains GL. Original TU
ownership remains unknown; the all-GL support had folded that comparison.
Native frame is98. Reversing scheduling-comparison operands did not help and
was reverted. These are diagnostic codegen observations, not extent shortcuts.
Final canonical mixed-context checks passed all709 cases; public CI passed8+25
checks. Fog packing's171-byte cold proof remains current in that same family.

The preceding tween checkpoint added nine complete bodies: two steps, two factor
getters, three fog operators and two timers. SetStageTimer59 is strictly exact
with one reviewed DIR32 scale binding; Pack171 remains exact after affected
cold replay. Other new bodies remain non-exact. Generic timer provenance is
indeterminate. Source counts108; exact52/10,706 (8 COFF,44 linked PE).

All2,171 direct-body cases pass, including four null-scale faults and200 extra
extended-precision randomized Hermite cases. All709 script cases also pass
with actual tween/timer/fog arithmetic bodies replacing their earlier models;
five remaining modeled-dependency faults and20 six-frame programs remain.
Candidate easing/conversion uses isolated original-machine dependencies, not
reconstructed source. VM reset/bind, mesh/heap and sine still use models.
Canonical script is now2,728 bytes/frame68 with2,217 address-masked diagnostic
differences; primary tween family's support interpreter is2,608, also non-exact.
Never reuse the previous2,740 size as the current canonical measurement.

Private `.analysis/stage_tween_fixture.py`, `stage_tween_oracles.py` and
`check-stage-tween.py` use `TH11_STAGE_TWEEN_TEST_DIR` to select the canonical
`build/match-linked/StageTween`; `TH11_STAGE_SCRIPT_REAL_TWEENS=1` enables real
integration in the canonical script fixture. The private probe family remains
`build/probe-ltcg/stage-tween`. Logs are `.analysis/stage-tween-check.log`,
`stage-tween-canonical-check.log`, `stage-tween-script-check.log` and
`stage-tween-cold-replay.log`. See `STAGE_TWEEN.md` for scope and ABIs.

An early supposed mode7 vector Z bug was disproved: POP changes stack offsets;
actual native Z is oldZ+deltaZ. Corrected source plus independent XYZ assertion
preserve this finding. Do not reintroduce the oldY hypothesis.
Final canonical tween cases peaked at191,132 KiB; script integration140,584 KiB.
Target/toolchain/tracking/progress and public CI8+25 checks all passed.

Latest easing checkpoint closes the complete764-byte dispatcher owner and the
39/45-byte factor getters, adding848 exact bytes. All three new primary-TU
units pass cold linked ownership/receipt/field replay:35 fields for easing,
one REL32 per getter. The affected existing setter59 and pack171 also pass.
Only these five units across three families were checked; other exact inputs
are unchanged. Counts109 mappings,55 exact/11,554 (8 COFF,47 linked PE).

Standalone easing executes actual original/candidate bodies without dependency
models:35,760 cases across16 curves,447 bit patterns and five x87 control words,
plus28 independent formula assertions. Real easing also passes the rebuilt
canonical tween2,171 cases/four null-scale faults and script709 cases/five
remaining-model faults, including20 six-frame programs. Their peaks are198,284
and121,020 KiB. CRT conversion remains an original-machine Oracle; script
VM/mesh/heap/sine remain models. No native game closure claim is added.

Current reproduction uses `TH11_STAGE_TWEEN_REAL_EASING=1` with
`TH11_STAGE_TWEEN_TEST_DIR=build/match-linked/StageTween`, and
`TH11_STAGE_SCRIPT_REAL_TWEENS=1 TH11_STAGE_SCRIPT_REAL_EASING=1` for script.
The direct pure easing checker uses `TH11_STAGE_EASING_TEST_DIR` set to
`build/match-linked/StageEasing`. Easing review source, owner/field proofs and
execution commands are in `STAGE_EASING.md`. Private final logs are
`.analysis/stage-easing-canonical-check.log`, `stage-easing-tween-canonical-check.log`,
`stage-easing-script-check.log`, `stage-easing-cold-replay.log`,
`stage-easing-final-cold-replay.log` and `stage-easing-ci.log`.

Expression experiments: shared double easing locals792 bytes, scoped doubles804,
repeated expressions748, original out-in arithmetic764. Getter if-assignment
skips a final float store on the raw-progress branch; double locals introduce
61/67-byte frames. Conditional-expression returns plus unsigned<=15 reproduce
native39/45 bytes. Do not repeat these discarded forms unchanged.
Final target-bound tracking/progress, public CI8+25 and diff checks all pass.

## Useful next work

1. Recover ResetStageMesh0x0040E6C0 (325 provisional bytes) and mesh
   constructor0x0040E530 after fresh full boundary/ABI reviews. These remain
   modeled dependencies in stage script/update checks. Script, both tween steps
   and the complete easing dispatcher now have source; easing and both factor
   getters are exact. Do not repeat entry-only easing ownership review or the
   documented factor double-local experiment.
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

Fresh review corrected the two factor getters' earlier authored-origin labels
to indeterminate: five other layouts share the generic factor pattern, which
establishes reuse rather than original authorship. Source/55 exact proofs are
unchanged; generated origin-specific counts now reflect this correction.
