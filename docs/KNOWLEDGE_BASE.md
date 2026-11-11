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
targeted63-support owner is now3172 bytes/160 fields/2767 differences, with15/15
returns and no call-count gaps. Frame54 now differs from native58. Keeping
two explicit result objects restored frame58 but regressed to14 returns and
three call-count gaps (3016 bytes/157 fields/2799 differences), so it was
reverted. The factory-merging source is retained for its closer call graph. Old19
inspected contributions stay byte-identical. No exact unit was added. Required
close92/show64 replay zero only in support; score33 and factory105 remain open.
See [PAUSE_CONTINUE_MENU](PAUSE_CONTINUE_MENU.md).

The formerly missing game-over child-finder caller42E6B0 has a fully
reviewed3208-byte owner including a seven-entry switch table, and VC9-compiled
state20..26 source. Targeted linked candidate3060/native3208 has88/88 calls
and14/14 returns; still nonexact. See
[PAUSE_GAME_OVER_MENU](PAUSE_GAME_OVER_MENU.md).
The native overlay queue42F400/103 now has a cold linked-PE **exact**
primary-TU proof with all103 bytes equal and zero fields. Its caller42F480/101
also has compiled C++ source but remains **non-exact**; the width/height
argument order was corrected using the established sprite layout. The neighboring 42F340/350
14-byte comparison candidates are 12/14 with ordinary C++ thiscall
source and unproved incoming references. See
[PAUSE_OVERLAY_RECTANGLES](PAUSE_OVERLAY_RECTANGLES.md) and
[PAUSE_GAME_OVER_MENU](PAUSE_GAME_OVER_MENU.md).
PreparePauseContinueRecords42D850/380 and its insertion callee42BA80/365
now have maintained VC9-compiled source; the normal-COFF diagnostic sizes are
387/232 respectively, neither exact. The insertion percentage uses target
double100.0 at497E50; the preparer rechecks stage7 after insertion.
Replay compression main4423A0/795 now has compiled source (normal COFF738/795,
non-exact), in addition to search442910/416 and four tree edits.
Replay writer436420/1856 and allocator437300/150 have VC9-compiled source,
but neither is exact. The scoped writer link now generates1765 bytes with
46/46 native call sites; the chunk constructor uses operator new and the
unconditional generic doubly linked-list insertion. ABI remains open. The adjacent compression dictionary reset4428D0/52 is
full normal-COFF exact with four reviewed fields, and predecessor442BE0/49
is now cold linked-PE exact with three reviewed node fields. The adjacent
416-byte tree matcher442910 has compiled source (same-length but nonexact
normal-COFF candidate); its four node splice/delete/replace/predecessor
helpers now also have compiled but nonexact source. Focused five-TU
VC9 /GL gives encoder760/795 and tree matcher414/416; same-length
node replacement still differs in 65 unmasked bytes. See
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

The player controller's eight-pair VM reset42F580/286 now has
complete cold linked-PE exactness (286/286, one reviewed global DIR32)
from a naturally compiled 304-byte controller reset caller.
Parent42FEC0/304 also has compiled source but remains non-exact
(319-byte linked draft); see [SECONDARY_PLAYER_ANIMATION_RESET](SECONDARY_PLAYER_ANIMATION_RESET.md).

Controller initializer42F8A0/1557 now has full VC9-compiled source,
13 native calls/2 returns in the narrow linked context; both narrow1479
and helper-linked1520 remain non-exact. Refer to
[SECONDARY_PLAYER_INITIALIZATION](SECONDARY_PLAYER_INITIALIZATION.md)
for caller and codegen gaps.


The player storage constructor's native vector-loop callbacks
at4021D0 and433F00 are now distinct **3/3-byte cold normal-COFF exact**
owners with zero fields; each is an identity-only ECX-to-EAX constructor.
The initializer42F8A0/1557 also has a source-level data-flow correction:
the second collision extent reads table4A7AB0, not focus table4A7AC0.
The large initializer and storage constructor remain non-exact;
see [SECONDARY_PLAYER_INITIALIZATION](SECONDARY_PLAYER_INITIALIZATION.md).


The TH11 player allocation factory4301B0/152 now has a natural C++
RAII source whose 0x8D24-byte typed storage restores VC9 exception
handling: normal COFF150, focused initializer /GL149, and
initializer+storage lifecycle /GL **152/native152** with five calls
and two returns. It remains **non-exact** (121 raw byte differences
and distinct optimized helper ABIs); see
[SECONDARY_PLAYER_INITIALIZATION](SECONDARY_PLAYER_INITIALIZATION.md).


The complete 33-byte original compiler-style element iterator4021A0
has reviewed ownership and maintained source in
`SecondaryPlayerElementIterator.cpp`. It is called twice per
player emitter in constructor42F760. Generic VC9 /GL gives 33 bytes
with29/33 identical but with the count/array registers swapped;
the actual seven-element callsite specializes it to31 bytes,
so **not exact**. Restoring explicit caller-level helper invocations
in the 272-byte storage constructor recovers its original five calls,
one RET4, and a focused linked **278/native272** contribution
(previously236/three calls). See
[SECONDARY_PLAYER_INITIALIZATION](SECONDARY_PLAYER_INITIALIZATION.md).


The player storage constructor42F760/272 now uses a physical inline
two-embedded-VM prefix (members at14/448) instead of two unrelated
placement-new statements. Pinned normal COFF improves
287/27-of-232 comparable bytes to **271/46-of-232**.
Scoped /GL remains **278/native272**, with all five native calls
and one RET4. This is still non-exact; see
[SECONDARY_PLAYER_INITIALIZATION](SECONDARY_PLAYER_INITIALIZATION.md).


The player destructor430010/415 now models two scope-guarded
chain unregister operations and **automatic reverse C++ destruction**
of embedded VM members. Their `storage400` fields are precisely
controller offsets848 and414, matching the native cleanup order.
In a limited VC9 /GL context including StageLifecycle.cpp the
owner improves from357/12 calls/2 returns to **400 bytes/
12 calls/1 return**, versus native415/12/1. It is still non-exact;
see [SECONDARY_PLAYER_INITIALIZATION](SECONDARY_PLAYER_INITIALIZATION.md).


Animation document cleanup4549E0/243 is now **strict cold-linked
exact 243/243**, with eight reviewed native fields and the full
six-byte internal alignment. Maintained C++ in
AnimationDocumentDestructor.cpp frees texture COM objects, source
buffers and five document allocations in observed order. The
separate menu marker4564C0/78 is also **cold-linked exact 78/78**
with zero fields, when a real generic renderer/file caller
prevents LTCG over-specialization. Both strict units retain
complete source/support/tool/PE/MAP/PDB receipts. See
[ANIMATION_DOCUMENT_DESTROY](ANIMATION_DOCUMENT_DESTROY.md).


The player storage destructor430010/415 now directly inlines the
observable two StageEmbeddedVm `storage400` cleanup operations after
its configuration free. With exact animation-document/marker callees,
the scoped /GL parent has all **three native direct free calls**
and the full **12-call/one-RET** graph, producing435/native415 bytes.
It remains non-exact; the storage constructor42F760 is unaffected.
See [SECONDARY_PLAYER_INITIALIZATION](SECONDARY_PLAYER_INITIALIZATION.md).


Animation document slot free4549A0/51 now has strict cold-linked
**exact 51/51** from natural C++ in AnimationDocumentSlotsLifecycle.cpp,
replaying two REL32 calls to exact document cleanup4549E0 and operator
delete45FD49. Its neighboring 32-slot scanner4548D0/135 has source
but is non-exact (115-byte scoped /GL), with the original unchecked
pending-destroy reload retained. See
[ANIMATION_DOCUMENT_SLOTS](ANIMATION_DOCUMENT_SLOTS.md).


The animation-document slot scanner4548D0/135 is now a focused
VC9 /GL **135/135-byte**, 50-instruction, 3-call/2-return complete
candidate. Its ordinary C++ restores the signed lower index check,
nonvolatile busy memory comparison and renderer-index reload; after
one DIR32 and three REL32 native fields only **one instruction byte**
differs (EDI vs EAX for non-exact AdvanceAnimationDocument4544C0/212).
This scanner remains **non-exact**; the neighboring 51-byte slot
release remains strict canonical exact. See
[ANIMATION_DOCUMENT_SLOTS](ANIMATION_DOCUMENT_SLOTS.md).


Animation chunk decoding at4545A0/811 is now a **cold linked-PE
exact 811/811-byte boss function**, with all 15 independently reviewed
DIR32/REL32 fields replayed and nine owned alignment bytes.
The natural `script[i].offset` loop and the native sprite
fileIndex/texture/textureIndex assignment order eliminate all ten
previous code differences. The 149-byte StoreAnimationSprite is
separately exact, while AdvanceAnimationDocument4544C0/212 remains
non-exact. See [ANIMATION_CHUNKS](ANIMATION_CHUNKS.md).


The 221-byte `LoadAnimationChunkSource` real parent context now includes
`BeginLoadAnimationDocument`: it reproduces 221 bytes, native six calls
and two RET4 exits with only two non-field SIB selector differences
at B3 and BD (commutative ECX/EDX addressing). It remains **non-exact**
and must not replace the separately exact 811-byte decoder family.
See [ANIMATION_CHUNKS](ANIMATION_CHUNKS.md).

Renderer priority callbacks 455140..45531E: the real
`AnimationRenderer` constructor provides the natural /GL producer context
for all 22 formerly source-open wrappers. They carry renderer in EDI,
selected dense layer in EAX and call dispatcher455660. Ordinary COFF
collapses the wrappers to 9 bytes, but the real producer reconstructs
12/15 bytes exactly, with one independently reviewed REL32 field per
callback and zero remaining byte differences under strict cold replay.
The five stateful priorities are now independently exact (see below).
See [ANIMATION_RENDERER_LIFECYCLE](ANIMATION_RENDERER_LIFECYCLE.md).

The 221-byte animation source loader4543E0 remains non-exact after eight
additional natural direct-array, signed-index and byte-offset expressions:
all preserve the full221-byte producer body but still select SIB0A rather
than native SIB11 at offsets B3/BD. Those alternatives are isolated in
`.analysis/try-animation-load-sib-20261111.py` and must not replace the
separately exact 811-byte decoder.

Five renderer-priority stateful callbacks 455170/126, 455320/46,
455350/74, 4553A0/74 and 4553F0/133 become strict linked exact under
the real constructor, existing exact ApplyCamera and batch-flush code.
All 43 DIR32/REL32 fields bind the runtime's three cameras at root
+204/+31C/+434, active pointer+54C, index+550, fog cache+990,
renderer offsets, and the existing camera/layer dispatcher functions.
The 133-byte priority-62 owner restores screen camera after drawing
the alternate layer and retains the first draw result. No full-cohort
cold replay was needed. See
[ANIMATION_RENDERER_LIFECYCLE](ANIMATION_RENDERER_LIFECYCLE.md).
