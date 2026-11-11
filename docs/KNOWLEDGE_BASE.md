# TH11 working knowledge

Use [RE_HANDOFF](RE_HANDOFF.md) for the next action and [PROGRESS](PROGRESS.md)
for generated ledger counts. Topic notes below retain the target findings,
compiler observations and useful failed approaches.

## Evidence and recovery

- Only original Japanese TH11v1.00a, SHA256
  `2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`.
- TH11-local PE/Rich evidence selects VC9 RTM21022/SDK6.0A. TH10/VC7.1 bytes or
  compiler choices do not establish TH11 facts. Original TU partition is open.
- Use direct `scripts/ghidra.py`; preserve full code, layout, tables and late
  blocks. Ghidra bounding spans and sparse bodies are not reviewed extents.
- Source presence, diagnostic equality, canonical exactness and runtime/product
  closure are separate. A support function replaying zero adds no exact credit.
- /GL stays with established owners. Public primary-TU acceptance requires cold
  receipts, full PE/MAP/PDB contribution identity, exhaustive fields and zero
  complete differences. Do not encode private registers or add inert storage.
- Rebuild affected exact contexts through `config/match-units.toml`. Cache
  retention is described in [ARTIFACTS](ARTIFACTS.md).

## Latest continuation menu3192

42D9D0 owns3192 bytes through42E647, not its old3141 default. Full flow has872
instructions/3122 code+22 layout+48 table,88 calls/162 fields/15 RET4 GS paths.
Source covers states12..19, practice-score updates, replay mode0 save, separate
record-name writes and delayed runtime transition. The record-name and practice
score formulas differ; timestamp0C/stage68 must not alias difficulty64.

Historical63-support draft:3048 bytes/158 fields/2770 differences, frame58,
14/15 returns and four call-site gaps. After merging the case13 factory the
targeted63-support owner is3112 bytes/160 fields/2797 differences, with15/15
returns and no call-count gaps. Frame54 now differs from native58. Keeping
two explicit result objects restored frame58 but regressed to14 returns and
three call-count gaps (3016 bytes/157 fields/2799 differences), so it was
reverted. The factory-merging source is retained for its closer call graph. Old19
inspected contributions stay byte-identical. No exact unit was added. Required
close92/show64 replay zero only in support; score33 and factory105 remain open.
See [PAUSE_CONTINUE_MENU](PAUSE_CONTINUE_MENU.md).

The last missing actual child-finder caller42E6B0 retains an unreviewed3179-byte
default. PreparePauseContinueRecords42D850/380 and its insertion callee42BA80/365
now have maintained VC9-compiled source; the normal-COFF diagnostic sizes are
387/232 respectively, neither exact. The insertion percentage uses target
double100.0 at497E50; the preparer rechecks stage7 after insertion.
Replay writer436420/1856 and allocator437300/150 have VC9-compiled source,
but neither is exact. The adjacent compression dictionary reset4428D0/52 is
now full normal-COFF exact with four reviewed fields. The adjacent
416-byte tree matcher442910 has compiled source (same-length but nonexact
normal-COFF candidate). See
[REPLAY_WRITING](REPLAY_WRITING.md).
Existing ActorInstructions/HUD/gameplay sources do not implement these owners.

## Pause3212 and returned-ID lifetimes

42C8D0 owns3212 bytes, including both tables. All101 calls,164 native fields,
14 GS/RET4 paths and native frame68 were reviewed. The59-support candidate
3084/160 fields/3039 differences has frame68/cookie64. Seven direct child-query
expressions recover seven real hidden result slots; an inline wrapper reused
one slot and produced frame4C. Account for the actual source script push when
reasoning about stack addresses; never normalize byte comparison for credit.

Confirm/shortcut/cancel tests are independent. Replay mode1, transient state14,
two cancel timer resets, sound44, name persistence and raw metadata stores are
retained. Four native address-field occurrences are absent: session4A8E88,
active replay4A8EB8, name-string base4951E8 and runtime base4C3280. The shared
child finder is still76/native70/41; incomplete callers do not prove a forced
register ABI. See [PAUSE_MENU](PAUSE_MENU.md).

## Menu and shared-code findings to preserve

- Audio43AAD0/3836 is canonical linked exact. Caller43A6F0/992 still has nine
  prologue differences in its audio context. See [CONFIGURATION_AUDIO_MENU](CONFIGURATION_AUDIO_MENU.md).
- Difficulty1056, character1228, partner1516, practice1024, player-data2448/750,
  music2668 and joystick2439 are complete source owners but remain non-exact.
  Each topic records its own context. Smaller early extents omitted owned tables
  or late blocks; do not restore them.
- BindAnimationSprite44AB40 uses the QWORD1/256 coefficient, not0.5. The shared
  correction was cold-replayed over11 affected exact units; do not revert it.
- Void history push can naturally leave EAX live under /GL; this does not prove
  an original return-this API. Saved character/partner values may be published
  raw after clamp calls. The target's ordering must survive tidy-up.
- Normal/practice transitions use480/392 and a five-argument screen effect.
  Fade is float6.0; its FSTP replaces a pushed pointer slot. Partner writes
  runtime558=7 before fade; practice writes it afterward.
- MenuController and MenuPrimary measurements, including the isolated primary30
  differences, belong to historical contexts. Cold-build their public recipes
  before a new measurement.

## Trimmed VC9 environment

VC9 RTM21022 works without its installation Git database, ATL/MFC, CRT sources,
64-bit tools/libraries, managed/debug CRT libraries or unused SDK tools. The
retained x86 compiler, SDK headers/libraries and runtime passed normal C/C++, /GL,
resource and MAP/PDB link smoke. Temporary cold builds fully matched FileExists141,
sprite UV81 and screen-effect initialization1168, including all68 fields.
Factory's NTLM helper and complete dependency tree match the existing lock and
can be shared; keep the TH11 Wine prefix independent.

## Other large source owners

These native reviewed extents are source-present, not current byte-equality
claims. Rebuild a selected context before quoting a new candidate metric.

| Owner | Native bytes | Detailed evidence |
| --- | ---: | --- |
| ExecuteSecondaryActorInstruction412E30 |16556|[host dispatch](SECONDARY_HOST_DISPATCH.md)|
| ExecuteAnimationVm44B4B0 |12056|[animation](ANIMATION_EXECUTION.md)|
| RunSecondaryScript45B860 |7093|[script](SECONDARY_SCRIPT.md)|
| RunSecondaryDialogue41D380 |6980|[dialogue](SECONDARY_DIALOGUE.md)|
| UpdateSecondaryHud41B380 |4716|[HUD](SECONDARY_HUD.md)|

## Topic index

| Work | Evidence and remaining differences |
| --- | --- |
| Continuation/game-over menu3192 and replay/record names | [PAUSE_CONTINUE_MENU](PAUSE_CONTINUE_MENU.md) |
| Pause3212, replay slots and returned-ID lifetimes | [PAUSE_MENU](PAUSE_MENU.md) |
| Difficulty, character and partner selection | [difficulty](MENU_DIFFICULTY_SELECTION.md), [character](MENU_CHARACTER_SELECTION.md), [partner](MENU_PARTNER_SELECTION.md) |
| Practice, player data, music and joystick | [practice](MENU_PRACTICE_STAGE.md), [data](MENU_PLAYER_DATA.md), [music](MENU_MUSIC_ROOM.md), [joystick](MENU_JOYSTICK.md) |
| Complete animation interpreter12056, events and all tween/strip paths | [ANIMATION_EXECUTION](ANIMATION_EXECUTION.md) |
| HUD4716, real dialogue caller and full boss/animation lifecycle | [SECONDARY_HUD](SECONDARY_HUD.md) |
| Dialogue6980 and complete27-opcode script source | [SECONDARY_DIALOGUE](SECONDARY_DIALOGUE.md) |
| Gameplay and ECL debug factory callers | [SECONDARY_GAMEPLAY](SECONDARY_GAMEPLAY.md) |
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
| Exact orbit/aim callbacks, player death and life HUD | [SECONDARY_EMITTER_MOTION](SECONDARY_EMITTER_MOTION.md) |
| Exact VM release/position publication, ID values and marker read timing | [SECONDARY_ANIMATION_VALUES](SECONDARY_ANIMATION_VALUES.md) |
| Large interpreter caller context, trace types and native fixed frame | [SECONDARY_INTERPRETER_CODEGEN](SECONDARY_INTERPRETER_CODEGEN.md) |
| Large actor host dispatcher, full tables and missing interpreter caller context | [SECONDARY_HOST_DISPATCH](SECONDARY_HOST_DISPATCH.md) |
| Complete scene/actor/object clearing and floating comparison distinctions | [SECONDARY_CLEARING](SECONDARY_CLEARING.md) |
| Complete collision transition, primary plus32 VMs and timer/spell gates | [SECONDARY_COLLISION](SECONDARY_COLLISION.md) |
| Complete player movement, callback-slot correction and event setter | [SECONDARY_PLAYER_MOVEMENT](SECONDARY_PLAYER_MOVEMENT.md) |
| Complete horizontal wrap and exact item attraction | [SECONDARY_PLAYER_WRAP_ITEMS](SECONDARY_PLAYER_WRAP_ITEMS.md) |
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
