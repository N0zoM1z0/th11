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

## CRT library code and withdrawn asm claims (2026-11-14)

`scripts/scan-library-code.py` matches every code section of the pinned VC9
`libcmt.lib` and `libcpmt.lib` against the image with relocated fields masked.
A section counts only with a single hit, or with several hits where a checked
REL32 picks the right member. Every checked REL32 must also land on the
callee's matched address. It classifies 889 inventory functions
(150,480 bytes) as library code. All of them lie at 0x45FC2C or above, so the
game's own code ends at 0x45FC2B. 773 rows equal their archive symbol's extent
and are now boundary-reviewed. The other 116 sit inside a matched section but
are split differently, mostly SEH `__finally` blocks and one-byte tail
differences. They keep library origin but still need boundary review. About 139
CRT-region rows (12.5 KB) remain unmatched: `entry`, tiny duplicated wrappers
and code from objects outside these two archives.

The scan showed that the "exact" 0x45FD49 `FreeHeapBlock` is libcmt
`operator delete` (`delete.obj`), and that its source was a naked `__asm` copy
of the target. A sweep found the same pattern behind 24 exact claims
(8,310 bytes). Those included the replay writer, compressor, replay
initializer/loader/stage, record factories, score insertion/preparation, draw
publishers, pair leaves and `DestroyTitleMenuRecord`, which is aliased through
`/alternatename` to a naked destructor that uses `_emit`. Two more claims forced
their reads through volatile casts: `ConstructReplayRecordStageLink` 437490 and
`MusicStreamingThread` 44A900. All 26 claims are withdrawn, and their
`functions.csv` notes say why. Their match units remain only as measuring
harnesses for real C++ rewrites. The continue, game-over and pause menus and
`RecordReplayInputFrame` still contain `__asm` (including the
`DeletePauseMenuReplayRecord` helper in `PauseMenu.hpp`), so measurements of
those owners are not evidence of a C++ shape.

## Inline definitions and opaque COMDAT callees (2026-11-14)

- An inline-asm `call F` anywhere in the link makes LTCG keep `F` on the
  standard ABI. `AppendReplayWriteSegment` stayed on stack arguments until the
  asm writer was stubbed out; with C++ callers it gets native's EBX/EDI. So a
  private register convention can only be reproduced once every caller is C++.
  A probe that tail-calls `F` with its own stack arguments also pins it.
- LTCG can pass `this` on the stack. The replay record destructor 435C50 is an
  ordinary out-of-line destructor (`ret 4`, `this` at `[esp+4]`), and callers
  call it directly rather than through a wrapper.
- `UnregisterChainElement` 457080 is `__stdcall (manager, element)` in
  source. LTCG passes element in ECX and manager in EDX, and right-to-left
  argument evaluation makes callers set ECX first, as every native caller with
  both values in registers does. The former `__fastcall (element, manager)`
  reproduced the body as normal COFF but put EDX first at every call site. With
  the swap, the replay record destructor 435C50 matches byte for byte apart from
  relocated fields. The lock/unregister block also appears in the Ascii
  (401210) and stage (402740) destructors.

- VC9 LTCG uses a callee's real register usage only for ordinary functions.
  The out-of-line COMDAT of an `inline` function whose inlining was declined is
  treated as a standard call. Native callers that keep `this` in ESI and
  reload ECX around a thiscall (`mov ecx,esi; call; mov eax,esi`) point to an
  inline callee. This made the three fog operators exact; see
  [STAGE_TWEEN](STAGE_TWEEN.md).
- A function inlined in one TU but called out of line everywhere else, even
  from small callers, is defined `inline` in that .cpp and only declared in the
  shared header (`SetStageTimer`, `Pack`).
- The image keeps unreferenced COMDAT copies of inline helpers. Small
  uninventoried islands next to their users, such as 405A90..405B6F and
  405C20, are such copies. When every call to an inline member is inlined
  under LTCG, its dead copy allocates EDX for the loaded field and keeps ECX;
  an address-taken or non-inline definition uses ECX. This reproduces the
  42F340/42F350 comparison islands byte for byte (see
  [PAUSE_GAME_OVER_MENU](PAUSE_GAME_OVER_MENU.md)) and the timer `>=` copy at
  405AC0 has the same shape.
- 20 source-present non-exact leaves have no direct calls and no stored
  address, so they are probably dead inline copies whose register conventions
  LTCG chose freely: 40FFC0, 410530, 42F340, 42F350, 433DD0, 433E10, 433E30,
  433E50, 433EA0, 433F10, 4353A0, 4353E0, 435430, 435460, 441650, 44ED70,
  453440, 459C60, 45DC80, 45DD50 (scan: E8/E9 targets plus little-endian
  address search over the attested image). The integer-pair group is already
  reproduced this way; see [SECONDARY_PATTERN](SECONDARY_PATTERN.md).
- 459C60 is `ChainElement::SetCallback` (callback=value, +0C=+10=0), not an
  audio helper: the same three stores are inlined in about two dozen chain
  registrations. As an inline member used by the ASCII lifecycle's chain
  helper it is exact (unit `chain-element-set-callback`). Searching the image
  for a dead copy's store shape is an effective way to find its real users.
- VC9 emits `add r,-4` (rather than `sub r,4`) for `if (x - 4 < 0) ...; x -= 4;`
  on a member: the test is formed on the unmodified member. Locals,
  `x += -4` and helper calls all canonicalize to `sub`.
- The length/angle copies 435400/435430/435460 do not reproduce as plain
  inline-copy probes. Native realigns their frame (`push ebp; and esp,-8`)
  around `_CIsqrt`/`_CIatan2`; probes with `sqrt(double)`, the float `sqrt`/
  `atan2` overloads, `sqrtf` or explicit double temporaries all give the
  unaligned 33/34/19-byte form. The alignment probably comes from LTCG's
  stack-alignment choice for the real (inlined) callers, so find those first.
- A global `/Ob2` is rejected: it inlines `ResetMusicDecoder` into the exact
  `ConfigureMusicDecoder`. Keep `/Ob1`; prefer source `inline` over
  `__forceinline` when testing why native inlined a helper.

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
| Title-menu and replay object lifecycles | [TITLE_MENU_LIFECYCLE](TITLE_MENU_LIFECYCLE.md), [REPLAY_RECORD_LIFECYCLE](REPLAY_RECORD_LIFECYCLE.md), [REPLAY_INPUT_RECORDING](REPLAY_INPUT_RECORDING.md), [REPLAY_FILE_BROWSER](REPLAY_FILE_BROWSER.md) |
| Pbg archives, disk paths and resource lookup | [PBG_RESOURCE_SESSION](PBG_RESOURCE_SESSION.md), [PBG_DIRECTORY_LOADER](PBG_DIRECTORY_LOADER.md), [PBG_RESOURCE_LOOKUP](PBG_RESOURCE_LOOKUP.md), [PBG_LOW_LEVEL_FILE](PBG_LOW_LEVEL_FILE.md) |
| Pbg memory streams and Win32 resource/process APIs | [PBG_MEMORY_FILE](PBG_MEMORY_FILE.md), [PBG_WINRESOURCE_MEMFILE](PBG_WINRESOURCE_MEMFILE.md), [PBG_PROCESS_LAUNCHER](PBG_PROCESS_LAUNCHER.md) |
| File/archive | [RESOURCE_FILES](RESOURCE_FILES.md), [SHARED_FILE](SHARED_FILE.md), [ARCHIVE_DECOMPRESSION](ARCHIVE_DECOMPRESSION.md) |
| Frame timing and thread lifetime | [FRAME_CLOCK](FRAME_CLOCK.md), [THREADINF_LIFECYCLE](THREADINF_LIFECYCLE.md) |
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
differs (EDI vs EAX in the **then-non-exact** AdvanceAnimationDocument4544C0/212 context).
This scanner remains **non-exact**; the neighboring 51-byte slot
release remains strict canonical exact. See
[ANIMATION_DOCUMENT_SLOTS](ANIMATION_DOCUMENT_SLOTS.md).


Animation chunk decoding at4545A0/811 is now a **cold linked-PE
exact 811/811-byte boss function**, with all 15 independently reviewed
DIR32/REL32 fields replayed and nine owned alignment bytes.
The natural `script[i].offset` loop and the native sprite
fileIndex/texture/textureIndex assignment order eliminate all ten
previous code differences. The 149-byte StoreAnimationSprite is
separately exact. The later AdvanceAnimationDocument4544C0/212 cold
proof supersedes this historical non-exact observation; see the later
entry below and [ANIMATION_CHUNKS](ANIMATION_CHUNKS.md). See [ANIMATION_CHUNKS](ANIMATION_CHUNKS.md).


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

A real 48-byte four-slot animation surface-copy producer at445460 calls the
313-byte D3D9 texture blitter at454EC0 through a private EAX record/ECX
renderer ABI. The slots are the existing renderer four40-byte prefix entries.
After a managed-texture check and batch flush, the child acquires backbuffer
and texture level0, **copies from backbuffer into texture surface** with
source rectangle fields at command +8 and destination fields at +18 (filter2),
marks the texture dirty after success, and releases both surfaces. The 313-byte
native has113 instructions; reloading document/texture entries after each
opaque COM call rather than keeping the texture pointer cached recovers a
312-byte /113-instruction focused VC9 /GL candidate. Two RECT stack slots and
last reload codegen remain non-exact. See `ANIMATION_SURFACE_COPY.md`.

The source-open 976-byte billboard boss at450700 now has C++ that projects a
translated origin and camera-right vector, bounds-checks normalized depth,
derives a projected half-size and rotates an anchored four-corner screen
quad. The first real-parent /GL build yields927 bytes versus native976;
native paired FSINCOS and float evaluation lifetimes remain open. See
`ANIMATION_PROJECTED_QUAD.md`.

The two outstanding animation renderer **update-chain** callbacks `455120`
and `455130` also reconstruct exactly under the genuine constructor producer.
Each is a 7-byte `MOV EAX,ECX; JMP` adapter to the larger update routines
`455480`/`455560`; independently strict linked-PE proof replays one REL32 field
per owner. They were absent from the earlier set of 27 draw callbacks.
See `ANIMATION_RENDERER_LIFECYCLE.md`.

The secondary ECL program browser at438AC0/934 and its 438570/83 filename
cleanup now have source and reviewed physical selection fields. The native
934-byte state machine has267 instructions/22 calls; a focused VC9 /GL
independent-caller family gives859/249/22. That selection-init gap is now partially closed: a native-aligned
post-store count reload and three-way split reconstruct the full267 native
instructions in a scoped912/934-byte VC9 /GL candidate. The large browser
still has hundreds of byte differences and remains NON-EXACT. See
[SECONDARY_PROGRAM_BROWSER](SECONDARY_PROGRAM_BROWSER.md).


The 83-byte filename cleanup `ClearSecondaryProgramBrowserFiles` at438570 is
now strict VC9 /GL exact. A saved `char *filename` local before the null test
restores native MOV/TEST/PUSH rather than CMP-memory/LEA/MOV; both original
CRT free calls at 45FC3B are independently bound. The larger934-byte ECL
browser retains its 859-byte /22-call diagnostic and remains non-exact.
See [SECONDARY_PROGRAM_BROWSER](SECONDARY_PROGRAM_BROWSER.md).

Original callback at438900 was missed by Ghidra's initial function inventory.
The browser's actual registration at438CBE takes its address, and the complete
8 bytes are CALL438910, XOR EAX,EAX, RET. Natural VC9 /GL linked code is
exact 8/8 with one reviewed REL32, independent of the still non-exact 277-byte
`LoadSecondaryProgramSelection` body. The bigger934-byte browser has been
corrected to register this callback. The separate83-byte filename cleanup
remains exact after a scoped cold recheck. See
[SECONDARY_PROGRAM_BROWSER](SECONDARY_PROGRAM_BROWSER.md).


The 934-byte ECL browser now uses the actual post-store selection count
reload and separate zero/positive/negative initializer branches. Its real
VC9 /GL narrow body grows from859/249 instructions to912/267, equal to
all267 native decoded instructions while retaining22/22 calls. It is not
byte exact (659 nonfield diagnostic differences). The shared83-byte file
cleanup stays strict exact after a scoped cold replay. See
[SECONDARY_PROGRAM_BROWSER](SECONDARY_PROGRAM_BROWSER.md).


The ECL browser deferred loader438910 now compiles to full native277 bytes
and81 instructions under the real `ResourceFile` archive/critical dependency
producer, with8/8 calls. Changing its filename storage from256 to MAX_PATH260
restores native `SUB ESP,108`; expressing an explicit 0/positive/negative
selection rule restores original code length. Only18 nonfield bytes remain,
mostly the ordering of constructor vptr write and the final EBX/ECX index
register allocation. Resource loader466/466 remains exact and the separately
accepted 8-byte thread caller still passes a scoped strict cold replay.
See [SECONDARY_PROGRAM_BROWSER](SECONDARY_PROGRAM_BROWSER.md).

The `SecondaryManager` 77-byte factory at45DB90 now reproduces every native
instruction and three reviewed fields in a real browser+`SecondaryScript`
VC9 /GL producer. Its two embedded `SecondaryVm` stack-zero stores require
`stack.top=0; stack.frame=0;` in that order; the previous chain assignment
reversed the two writes. The full distinct factory is cold linked exact;
see `SECONDARY_PROGRAM_BROWSER.md`.


Mode4 of the 934-byte browser has a second native manager-null check after
script completion, not merely a delete nested under the earlier guard. A
natural local snapshot plus independently reread manager field restores it
under the real browser/debug caller+manager producer: 915/259 instructions
becomes928/262 with22 original calls, and positional nonfield differences
drop629 to412. Both exact browser dependents, the83-byte cleanup and77-byte
manager factory, cold replay unchanged. See `SECONDARY_PROGRAM_BROWSER.md`.

The 548-byte secondary ECL browser ASCII overlay at438E70 and adjacent
10-byte draw-chain adapter at4390B0 now have natural C++ sources. The overlay
contains130 complete instructions, 11 calls and three returns, rendering
file/ECL/quit/position state via the existing AsciiManager. Narrow VC9 /GL
produces504 bytes/11 calls; compiling actual ASCII print dependencies changes
that context to445 bytes/10 calls. Neither is exact. The distinct size getter
at4390D0 returns0x5B94 and is **6/6 strict exact** with no fields. The
header-dependent83-byte cleanup, 8-byte loader callback and77-byte manager
factory all rechecked exact. See `SECONDARY_PROGRAM_BROWSER.md`.


The 548-byte ECL ASCII draw owner at438E70 must reload g_AsciiManager
after opaque AsciiManagerPrint calls. Retaining its pointer throughout the
function causes VC9 /GL to keep it in ESI and use EDI for the browser, losing
one of the native11 calls when real ASCII support is compiled. Removing the
long-lived cache naturally recovers native browser-in-ESI, the complete x87
opening position setup, and11/11 calls: the real-ASCII candidate improves
from445/548 (10 calls,298 nonfield differences) to526/548 (11 calls,262
nonfield differences). The related six-byte size getter remains strict exact.
See `SECONDARY_PROGRAM_BROWSER.md`.

Another ECL browser owner missed by initial Ghidra function discovery:
4390A0..4390A4, a complete five-byte update-chain tail JMP to438AC0.
The real registration stores its address at4386A2. With the maintained
934-byte update source visible to VC9 /GL, the natural five-byte forwarding
body is strict linked-PE exact with the sole REL32 at+1; the neighboring
six-byte size getter independently rechecks exact. See
`SECONDARY_PROGRAM_BROWSER.md`.

The browser draw callback4390B0 is now fully **10-byte exact**. Natural
`PUSH ESI/MOV ESI,ECX/CALL 438E70/POP ESI/RET` appears only when the real
update/draw-chain registration producer at438660 and the large ASCII drawing
owner are visible together to VC9 /GL. The full 173-byte registration is **now strict exact**, all60 instructions,
four calls and six reviewed fields. Its initial159/153-byte diagnostics were
caused by over-specialization when no independent dynamic-priority chain
caller was present; linking that caller naturally restores native EBP/EBX/ESI
ABI. Splitting `link.next=0; link.previous=0;` repairs the two remaining
constructor store-order differences. It binds priority5 callback4390A0 and
priority44 callback4390B0. See `SECONDARY_PROGRAM_BROWSER.md`.

The Ghidra-missed ECL browser storage constructor4385D0..43865D is now
**142/142 strict exact** in natural VC9 /GL C++. Its 32 instructions preserve
the worker initializers, three selector initializers and final 0x2E4-byte
memset/global publication order. All three fields (worker table4978D0,
memset46A720, browser singleton4A8EC4) replay exactly. It directly precedes
the independently exact 173-byte chain registration. See
`SECONDARY_PROGRAM_BROWSER.md`.

The complete 439-byte `DestroySecondaryProgramBrowser` at438710 now has
natural C++ source. Full original PE/Ghidra review shows116 instructions,
16 calls, one RET4 and an SEH frame. Cleanup is files -> chain update/draw
with separate optional locks -> raw ECL -> program -> manager -> ANM slots6/5
-> singleton -> worker thread. Scoped VC9 /GL has455/439 (112 instructions,
16 calls) with external dependencies or372/439 (91 instructions, 16 calls)
with real helper TUs; EH and ESI receiver ABI remain open. See
`SECONDARY_PROGRAM_BROWSER.md`.


The browser destructor438710/439 now expresses the nontrivial worker-member
lifetime as a scoped C++ destructor, not a manual final call. A separate
independent call probe that lets its stdcall function address escape restores
original SEH entry and RET4 rather than producing a specialized ECX/RET.
The real 16-call producer now yields448/439 bytes and118/116 instructions,
still non-exact (233 non-address-field positional differences). See
`SECONDARY_PROGRAM_BROWSER.md`.


The previously open 548-byte ECL browser ASCII overlay at438E70 is now
**strict full exact** with all130 native instructions and11 call sites. A
fresh distinct local `float position[3]` in each case1/2/3 restores
the target's x87 redundant XYZ initialization/stack lifetimes; earlier
526/548 and 123/130 candidate was non-exact. The full primary-TU source
passes independent pinned VC9 `/GL` cold-linked PE/MAP/PDB replay, with
all43 fields verified: 11 calls, 12 AsciiManager globals, nine native
string literals and 11 byte-exact float/double literals; zero differences.
The 10-byte draw callback and 6-byte size getter retain separate exact units.
See `SECONDARY_PROGRAM_BROWSER.md`.

`StartSecondaryDebugThread` at4594A0 is **56/56 strict exact** under a
natural `/GL` dynamic-callback producer and genuine exact stop-thread source.
Its actual ABI passes worker in EAX, callback in EDI, context on stack, and
returns with RET4; two reviewed REL32 calls go to StopRuntimeThread459430
and CRT __beginthreadex46184B. It is the real thread-start dependency of
934-byte ECL browser update. See `SECONDARY_PROGRAM_BROWSER.md`.

The 30-byte six-instruction `SetSecondaryGameplayRuntimeMode` helper at40DEA0
is now **strict exact** with no relocations. Its actual generic EAX runtime,
ECX mode ABI requires an independent dynamic-mode user; literal mode3 in
the 934-byte ECL browser would otherwise be specialized away by VC9 /GL.
When combined with real 56-byte worker-start and 101-byte stop sources,
tracked `scripts/probe-secondary-browser-mode-thread.py` reproduces the
large browser at928/934 bytes and262/267 instructions (all22 calls), with
nonfield mismatches reduced from412 to173. That browser remains non-exact.
See `SECONDARY_PROGRAM_BROWSER.md`.

The 313-byte `CopyAnimationTextureSurface` owner now uses a short-lived
texture-table lookup only after a successful backbuffer-to-texture D3DX
copy, preserving every COM/RECT direction and release. Its real parent plus
screenshot producer (`scripts/probe-animation-surface-copy.py`) creates a
311/313-byte 112/113-instruction candidate with all8 native calls and46
positional nonfield differences, vs the old312-byte/55-difference candidate.
The missing instruction is in the final indexed texture AddDirtyRect setup;
this remains non-exact. See `ANIMATION_SURFACE_COPY.md`.


The 135-byte `UpdateAnimationDocumentSlots` scan at4548D0 is now **strict
exact**. Its formerly lone ModRM byte +70 (EAX versus target EDI) is
naturally recovered when the real 811-byte decoder dependency TUs are
linked alongside the scanner and `AdvanceAnimationDocument`. All135 bytes,
50 instructions, three calls, two returns, and four native DIR32/REL32
fields now match, independently cold accepted. This proves the non-exact
212-byte advance callee's private EDI input without asserting that callee
is exact. See `ANIMATION_DOCUMENT_SLOTS.md`.


`AdvanceAnimationDocument` at4544C0 is now **212/212 strict exact** under
real 811-byte decoder + 135-byte exact document slot scanner production.
Its original EDI document receiver, five argument decoder call, first four
DWORD stack locals, 32-bit decoded flag, sprite/script prefix order and
non-volatile direct CMP of busy+124 restore all74 instructions. Three native
DIR32/REL32 fields are independently reviewed and the full primary-TU VC9
/GL PE/MAP/PDB cold replay reports zero differing bytes. The exact 135-byte
scanner, 811-byte decoder and51-byte slot release remain compatible in the
same linked diagnostic context. `LoadAnimationChunkSource` remains nonexact.
See `ANIMATION_CHUNKS.md` and `ANIMATION_DOCUMENT_SLOTS.md`.


`ClosePauseContinueMenu` at42D7F0 is now **92/92 strict exact** (22
instructions, two calls, four reviewed native fields). A dynamic caller
and actual `SecondaryActorAnimation.cpp`/`StageMesh.cpp` TUs recover the
native ESI receiver and two direct signal calls; standalone source without
the real callee was94 bytes due over-specialized stack argument pushes.
Full unique PE/MAP/PDB owner cold replays with zero differences. The helper
restores animation time scale, not audio. Parent3192 continuation owner is
still non-exact. See `PAUSE_CONTINUE_MENU.md`.


The 64-byte `ShowPauseMenuAnimationTree` helper at456610 is **strict exact**:
20 native instructions including a reached six-byte loop-alignment LEA,
one call and two independently reviewed DIR32/REL32 fields. The genuine
`StageMesh.cpp` finder alone specializes away the renderer and yields only
52 bytes; adding `probes/stage-mesh-context.cpp` with a dynamic renderer
restores the original EAX ID reference / EDX renderer private ABI. The
linked primary owner is byte-exact with zero differences; the 3192-byte
parent remains non-exact. See `PAUSE_CONTINUE_MENU.md`.


The title-menu thread at `0x4394F0` is now **165/165 strict linked exact**.
Its native callback has no consumed argument and uses plain `RET` even though
the caller passes its address to `_beginthreadex`. After the real animation
document destructor, the original re-reads renderer slot 1 before scalar
delete; preserving that pointer reload naturally restores ESI/EDI register
allocation and all 47 instructions. The genuine title/document `/GL`
producer and 12 reviewed address/call fields replay with zero differences.
See [TITLE_MENU_LIFECYCLE](TITLE_MENU_LIFECYCLE.md).


`TitleMenuView` construction at `0x439420` is now a strict **201/201-byte**
public primary-TU `/GL` match: 39 native instructions, one memset call and
four reviewed target fields. VC9's dynamic title-lifecycle producer recovers
the original private ESI-this/EAX-return convention. Surprisingly, the
original initializes selection records and ThreadInf before zeroing the
complete 0x5B94-byte object, then publishes flags and its singleton. This
exact match does not establish original TU partition. See
[TITLE_MENU_LIFECYCLE](TITLE_MENU_LIFECYCLE.md).


The complete title resource initializer at `0x4395A0` is now **275/275
strict exact**; its important producer context is the separate generic
priority/slot caller together with the real chain, document, log and document
load sources. A mere real-document link left two instruction-byte differences,
but constraining both dynamic priority and dynamic file slot restored all
90 native instructions/7 calls and 15 reviewed fields. The same real producer
also gives the 28-byte singleton deletion exact. The title startup at
`0x439900` is **106/106 strict exact** under the true runtime-thread stop
producer. The ThreadInf constructor + non-deleting destructor are **21/21 +
16/16**; its 36-byte deleting destructor is **non-exact owner identity**
despite identical bytes because MAP aliases the `??_E` and `??_G` entries.
See [TITLE_MENU_LIFECYCLE](TITLE_MENU_LIFECYCLE.md),
[THREADINF_LIFECYCLE](THREADINF_LIFECYCLE.md).

`DestroyTitleMenuStorage` 561 still differs in 231 position-wise nonfield
bytes under its 561-byte/15-call/RET4 real-worker context. A true C++ virtual
title class generated 559 bytes and ordinary RET, and direct record deletion
spelling or real chain removal did not help. The related 429-byte replay
record destructor still differs in exactly three ECX/EDX load pairs, 6
nonfield bytes. No unsupported match credit is assigned; probe evidence is
retained in `.analysis/probe-title-*20261112.log`.
