# TH11 working knowledge

Read [RE_HANDOFF.md](RE_HANDOFF.md) for current work and
[PROGRESS.md](PROGRESS.md) for generated counts. Source, target observations,
exact codegen, original provenance and runtime behavior are separate claims.
Older experiments are preserved in the explicitly
[historical notes](archive/KNOWLEDGE_BASE_2026-10-30.md).

## Target and compilation

- Only original Japanese TH11 1.00a, SHA-256
  `2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`.
  Use `scripts/repo-python scripts/verify-target.py` after recovery.
- VC9 RTM21022/SDK6.0A follows TH11-local PE/Rich/tool evidence. TH10 code is
  reference material, not target evidence. `verify-toolchain.py --check` verifies
  the installed candidate without execution.
- Direct `scripts/ghidra.py`, full mapped-text attestation, CPU1/nice10/768 MiB;
  serialize Ghidra and Wine/compiler jobs. Use `scripts/repo-python` throughout.
- Exact linked acceptance needs a unique complete public primary-TU PDB owner,
  current source/tool/artifact receipt, exhaustive independent field replay and
  full byte equality. Static/EH/split owners remain open. Never crop tables or
  padding, borrow a support owner, force registers/alignment, or embed target code.
- Normal-COFF equality does not prove original TU partition or /GL ownership.
  Do not remove /GL from an established LTCG owner to obtain COFF credit. The
  new seven-byte update callback had no established LTCG primary owner.

## Layout and ABI corrections

| Area | Current native observation |
| --- | --- |
| Projectile6C | Acceleration64, descriptor pointer68; old parameters64 was wrong |
| Descriptor34 | Signed period/phase0/1; negative period terminates; no zero-period guard |
| Emitters | Eight E4 records at7570..7C8F;75CC is record+5C coordinate prefix |
| Reset configuration | Branch3 focused base148; branch4 seven pairs at64..9B from28/28/28/78/C8/118/168 |
| Reset partial initialization | Branch2 sets float polar pairs and retains old integer offsets; auxiliary processing precedes slot equality |
| Controller431070 | One actual stack owner, EAX1 and two RET4; no decompiler dummy ECX/EDX inputs |
| Aim431D20 | Actual owner AND position; caller constructs `(0, position.y-224, 0)` |
| Clear40AF90/424FE0 | Real EBX/ESI position input beyond three stack words |
| Callback entries | Update431C50/7 and draw431C60/10 published to chain+8/context+20 |
| Region7C9C |32 records/stride74; word+C is float angular growth in controller update |
| Missing animation21 | Returns the found VM pointer, not bool; caller dereferences VM3B0 |
| Event/release | Event70 writes word37C; release66 marks04000000; related-list propagation is real |
| Program | Base vptr49404C has install/extension callbacks; it is not a destructor table |
| Actor | Base103C+state163C=2678; real state ctor126 invokes timer/record constructors |

These are physical views, not recovered original class/header identities.
Assertions check our declared layout after native review; they are not evidence
that the target had that layout.

## Complete owner boundaries

| Owner | Reviewed complete extent |
| --- | --- |
| Actor state411750 |4230, including9 internal alignment bytes beyond sparse4221 |
| Controller431070 |2888=2867 code+one NOP+five switch entries |
| Reset432CC0 |2508=2457 code+3 alignment+two six-entry tables |
| Movement430290 |2896=2829 code+3 alignment+two eight-entry tables; sparse2811 missed18 internal bytes |
| Special406510 |268=241 code+3 alignment+six entries |
| Drop424230 |851=829 code+3 alignment+two entries+11 selectors;840 was still incomplete |
| Death4327D0 |703, including7 internal alignment bytes beyond sparse696 |
| Interpreter45B860 |7093=6759 code+one NOP+244 table bytes+89 selectors |
| Factory4108F0 |621=545 code+3 alignment+20 table bytes+53 selectors |

Ghidra's sparse body and apparent foreign DATA refs do not determine ownership.
Reset's12 and the enlarged special/drop/movement owners'24 case references are
owned tables.43 alleged refs in4355E0..43562F are based renderer displacements;
49353C's alleged435246 pointer is ASCII FRC. Inspect actual operands/data.

## Current compiler findings

The values below are full source size / normalized diagnostic differences in
specific retained contexts, not exactness scores or complete-product results.

| Owner | Last useful diagnostic / next gap |
| --- | --- |
| Controller2888 |2800/2285; real normal-COFF callback support preserves stack input/two RET4; frame/vector stores remain open |
| Reset2508 |2444/1967; main factory sites1 versus native4 |
| Publishers118/123 |111/33 and116/47; counter zero check/register allocation |
| Spawn853 |Real Sound846/733; real-only independent RNG856/690; both remain open |
| Actor state4230 |4161/3261 in full support context;64 calls/3 callbacks/5 returns audited |
| Damage1579 |1513/1414; full32-region/256-projectile source and corrected64/68 fields |
| Projectile896 / homing498 |908/765 and493/401 in earlier context; consult topic before reuse |
| Interpreter7093 |6993/5880; required variadic double trace still gives64-byte candidate frame versus native ESP/120 |

- Scoped reset result temporaries give2556/1977 and six main factory sites versus
  native four. Signed counter with wrapped increments does not close publishers.
  Neither is adopted. Volatile, padding, fake return/class ABI and frame tricks
  are not substitutes for the remaining source/context gap.
- Explicit float unsigned conversion retains the native float2^32 correction and
  actual signed-unit rounding boundary. Do not add earlier stores from decompiler
  casts. Float literals emit public compiler constants for strict field binding.
- Capture iterator next before body work where observed; retain late reads where
  observed. VM missing/null paths, one-past region allocation return, partial
  initialization and wrapped integer operations must follow native code.
- Actual CIatan2, double sqrt with float result, x87 operand order and snapshot
  publication can affect codegen. Do not invent vector constructors to force ABI.

## Continuing work and evidence

Controller/reset have complete maintained source and remain non-exact. Movement
2896, death703, orbit255, aim495 and real clear/drop callees are concrete source
work. Use real callback/registration evidence for ABI rather than decompiler
parameter guesses. Gameplay roles and sibling similarity do not prove authored
versus reused origins; the latest13 reviews remain indeterminate.

Cold-replay only new/changed units and actual receipt dependencies. Cleanup alone
requires protected-file identity checks, not a full-cohort replay or broad
behavior suites. Keep canonical receipt-bound artifacts; unused experiment code/binaries
are regenerable; result records are retained. See [ARTIFACTS.md](ARTIFACTS.md) for retention and latest cleanup.

## Topic index

| Work | Evidence and remaining differences |
| --- | --- |
| Mesh and effects | [STAGE_MESH](STAGE_MESH.md), [MESH_EFFECTS](MESH_EFFECTS.md), [VM factory](ANIMATION_VM_FACTORY.md) |
| Stage interpreter and interpolation | [STAGE_SCRIPT](STAGE_SCRIPT.md), [STAGE_TWEEN](STAGE_TWEEN.md), [STAGE_EASING](STAGE_EASING.md) |
| Stage lifecycle, updates and drawing | [STAGE_LIFECYCLE](STAGE_LIFECYCLE.md), [STAGE_UPDATE](STAGE_UPDATE.md), [STAGE_DRAWING](STAGE_DRAWING.md) |
| Stage/ANM resources | [STAGE_RESOURCE](STAGE_RESOURCE.md), [ANIMATION_CHUNKS](ANIMATION_CHUNKS.md), [ANIMATION_TEXTURES](ANIMATION_TEXTURES.md), [pixel processing](ANIMATION_TRANSPARENT_PIXELS.md) |
| Renderer quad construction/submission | [ANIMATION_QUAD](ANIMATION_QUAD.md), [ANIMATION_VM_DRAW](ANIMATION_VM_DRAW.md), [ANIMATION_PROJECTED_QUAD](ANIMATION_PROJECTED_QUAD.md), [ANIMATION_IMMEDIATE_DRAW](ANIMATION_IMMEDIATE_DRAW.md) |
| Secondary script interpreter, typed stack and threads | [SECONDARY_SCRIPT](SECONDARY_SCRIPT.md) |
| Secondary program installation and context selection | [SECONDARY_PROGRAM](SECONDARY_PROGRAM.md) |
| Secondary extension parser and trace context | [SECONDARY_EXTENSION](SECONDARY_EXTENSION.md) |
| Secondary lifetime, counters and large actor owners | [SECONDARY_LIFETIME](SECONDARY_LIFETIME.md) |
| Actor construction/destruction, pair interpolation and source contexts | [SECONDARY_ACTOR](SECONDARY_ACTOR.md) |
| Complete actor state, transitions, motion and VM return contracts | [SECONDARY_ACTOR_STATE](SECONDARY_ACTOR_STATE.md) |
| Pair arithmetic, omitted vector leaves and vendor floor | [SECONDARY_PAIR_MATH](SECONDARY_PAIR_MATH.md) |
| Complete damage/collision, projectile records and region allocation | [SECONDARY_DAMAGE](SECONDARY_DAMAGE.md) |
| Complete projectile update/homing, iterator/literal math and atan2 | [SECONDARY_PROJECTILES](SECONDARY_PROJECTILES.md) |
| Complete spawn, corrected record layout and omitted integer/emitter leaves | [SECONDARY_PATTERN](SECONDARY_PATTERN.md) |
| Real pattern file/callers, complete callbacks and containing emitter array | [SECONDARY_PATTERN_CALLBACKS](SECONDARY_PATTERN_CALLBACKS.md) |
| Complete emitter reset, list directions and factory ownership | [SECONDARY_EMITTER_RESET](SECONDARY_EMITTER_RESET.md) |
| Complete player controller, real callback contracts and full movement/drop tables | [SECONDARY_PLAYER_CONTROLLER](SECONDARY_PLAYER_CONTROLLER.md) |
| Streaming factories and lifetime ownership | [MUSIC_FACTORIES](MUSIC_FACTORIES.md) |
| Device setup and buffer recreation | [AUDIO_DEVICE](AUDIO_DEVICE.md) |
| Streaming buffers, decoder state and fades | [MUSIC_STREAM](MUSIC_STREAM.md) |
| Audio command queue and streaming worker | [SOUND_QUEUE](SOUND_QUEUE.md) |
| Animation interpolation, dispatch and pool allocation | [ANIMATION_TWEEN_DISPATCH](ANIMATION_TWEEN_DISPATCH.md) |
| Renderer lifecycle and pixel helpers | [ANIMATION_RENDERER_LIFECYCLE](ANIMATION_RENDERER_LIFECYCLE.md) |
| Buffered drawing and randomized fan | [ANIMATION_FAN](ANIMATION_FAN.md) |
| Renderer and device | [RENDER_CAMERA](RENDER_CAMERA.md), [DEVICE_INITIALIZATION](DEVICE_INITIALIZATION.md), [RENDER_STATE_RESET](RENDER_STATE_RESET.md), [RUNTIME_SURFACES](RUNTIME_SURFACES.md) |
| ASCII lifecycle, glyph drawing and formatting | [ASCII_LIFECYCLE](ASCII_LIFECYCLE.md), [ASCII_DRAWING](ASCII_DRAWING.md) |
| Text | [TEXT_RESOURCES](TEXT_RESOURCES.md), [TEXT_DRAWING_COMPOSITE](TEXT_DRAWING_COMPOSITE.md), [OUTLINED_TEXT](OUTLINED_TEXT.md), [FORMATTED_TEXTURE_TEXT](FORMATTED_TEXTURE_TEXT.md) |
| File/archive | [RESOURCE_FILES](RESOURCE_FILES.md), [SHARED_FILE](SHARED_FILE.md), [ARCHIVE_DECOMPRESSION](ARCHIVE_DECOMPRESSION.md) |
| Callbacks, input, frame and startup | [CALLBACK_CHAIN](CALLBACK_CHAIN.md), [INPUT_POLLING](INPUT_POLLING.md), [FRAME_LOOP](FRAME_LOOP.md), [STARTUP_WINDOW](STARTUP_WINDOW.md) |
