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
| Emitter callbacks | Reset publishes orbit/aim at DC, called by movement; draw calls a separate E0 slot. The prior reset source used E0 incorrectly |
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
| Emitter aim | Current movement8A0/8A4 gates remembered direction8AC/8B0; focused polar pairs at1C/24 |
| Death penalty42F6F0 | Real stack position forwarded through EBX to422560, despite the no-argument decompiler hypothesis |

These are physical views, not recovered original class/header identities.
Assertions check our declared layout after native review; they are not evidence
that the target had that layout.

## Complete owner boundaries

Actor host412E30 is16,556 bytes, not its initial15,780 code-only span. Full ownership
includes540 primary table bytes,195 selectors, one alignment byte and two nested
five-cell tables/40 bytes. All145 Ghidra external-to-code DATA refs become internal;
4,001 reachable instructions plus two alignment instructions account for the full
code. Its494074 virtual slot adjusts manager+103C through412E20. Origins remain
indeterminate; neither owner has a complete maintained implementation or exact credit. See
[HOST_DISPATCH](SECONDARY_HOST_DISPATCH.md).

| Owner | Reviewed complete extent |
| --- | --- |
| Actor state411750 |4230, including9 internal alignment bytes beyond sparse4221 |
| Controller431070 |2888=2867 code+one NOP+five switch entries |
| Reset432CC0 |2508=2457 code+3 alignment+two six-entry tables |
| Movement430290 |2896=2829 code+3 alignment+two eight-entry tables; sparse2811 missed18 internal bytes |
| Special406510 |268=241 code+3 alignment+six entries |
| Drop424230 |851=829 code+3 alignment+two entries+11 selectors;840 was still incomplete |
| Death4327D0 |703, including7 internal alignment bytes beyond sparse696 |
| Life HUD41A060 |144, including12 internal alignment bytes beyond sparse132; five real incoming entry calls |
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
| Scene clear334 / actor radius381 |342/218 and378/70; complete logical call/return contracts agree; frame/allocator ABI gaps |
| Collision transition546 |543/257; full8 direct calls/two imported section calls/two returns agree; frame/register scheduling remains open |
| Death703 |715/423 with independent real HUD/count/fragments and file/script/VM inputs; callee-save placement and emitter-loop layout remain open |
| Death effect229 / penalty102 |228/37 and102/14; native position EBX versus candidate EDI and mode/lifetime store scheduling |
| Movement2896 |2928/2447 after animation value API change; full22 direct calls/one indirect callback/RET4 agree; frame18 hex; register/branch/vector gaps |
| Horizontal wrap540 |499/415 with real movement/sound/reset source; counter-store folding, saved registers and refresh order |
| Reset2508 |2428/1965 after early position-marker correction; main factory grouping remains open |
| Publishers118/123 |111/33 and116/47; counter zero check/register allocation |
| Spawn853 |Real Sound846/733; real-only independent RNG856/690; both remain open |
| Actor state4230 |4161/3261 in full support context;64 calls/3 callbacks/5 returns audited |
| Damage1579 |1513/1414; full32-region/256-projectile source and corrected64/68 fields |
| Projectile896 / homing498 |908/765 and493/401 in earlier context; consult topic before reuse |
| Interpreter7093 |7013/5821 through sole actual caller; fixed120/all89 selectors/60-arm layout; arithmetic result storage and angle87 evaluation recovered, unary84/float publication/register gaps |

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
- Orbit255 and aim495 now close completely. Retain AngleMath's established normal
  COFF support: /GL support merges the repeated calls seen in the target.
  Float3 caller locals, separate signed conversions and component-level Player()
  reads recover the frame/conversion/global-load sequence naturally. The polar
  member remains a declaration with a real paired FSINCOS contract.
- Effect422560 and its real penalty caller42F6F0 now have complete source. The
 88-byte effect clears/constructs its node, appends at selected manager head78,
  copies float3 position and bounded text, then sets mode/lifetime. Reset/Append
  members recover the native constructor/list sequence; private position register
  and final stores remain open. Death loop/order probes do not improve715/423.
- Event helper456230 closes all67 bytes with the existing four-byte AnimationVmId
  value wrapper: the ID stays on the stack and event in SI. Flattening ID to unsigned
  int promotes it to EAX and gives53/57. Preserve independent renderer input for
  FindAnimationVmById; a global-only graph specializes away its actual argument.
- Item attraction4245E0 closes all63 bytes. Its2198 records start at manager+478,
  stride478; state1 becomes3 and parameter+C receives player configuration+8 as
  float. Direct parameter publication avoids a temporary rounding spill; increasing
  index iteration gives the native pointer ADD/count SUB. Full wrap540 source
  retains inclusive +/-6800 thresholds, D000 wrap, counter8/45 and sound47/48.

## Continuing work and evidence

Operator priority: large, difficult functions. Main focus is the complete7,093-byte
interpreter; do not use leaf counts as the work-selection objective.

Next substantial source owner is the16,556-byte actor host dispatcher. Its465
direct sites include83 integer/107 float argument reads omitted from the current
Run diagnostic, plus real state argument adapters. Complete native context must
be reconstructed before attributing the VM ESI/EDI and tail-merging differences
to source alone. Typed int-pointer extraction leaves Run7013/5821 unchanged;
memcpy bit transport worsens it to7381/6120/frame1B0. Neither is adopted.

Host motion118..133 now has maintained natural source as an explicit switch
fragment:28 handled values/16 physical targets. Native12A/12B really falls into
12C/12E after updating velocity; it rereads the captured instruction and selects
offset for that second stage. Sentinel gates reject unordered inputs, so a
decompiler <= fallback must not be copied blindly. Mutable vector/pair presets,
mode7/8, mirrored/shortest-path float roundings, centers and every duration store
remain. A cold /GL compile-only probe and static audit agree on opcode grouping,
89 physical calls and all per-opcode call/index trace sets. The full16,556-byte
host and its remaining opcode/EH source are still open; no complete
source mapping, exact bytes or runtime credit is added. See the host topic.
Random-direction bound tests differ from sentinel tests: they include unordered
inputs and use negated greater-than conditions; the player/upper-Y comparisons
remain ordered. Nine native x87 predicates were checked over all four compare
relations before freezing the source.

The same large host now also has creation/animation100..115 source:22 more values,
50/154 handled values total. The80-byte spawn packet preserves signed wrapped
payload indexing and callback-sensitive snapshots; its shared tails and111/115's
angle join remain explicit. Projection uses the real camera0 within renderer+204
and the world/view/projection/viewport argument roles are independently checked.
Animation replacements preserve repeated reads, hidden-result ID transport and
late coordinate-mode selection. Native default-script reads are full32-bit;
decompiler short casts are not copied. Cold /GL probe2804 matches every opcode's
CFG call/index trace set, but native89/candidate88 sites still differ at raw-int
reader11/10, lookup6/5 and flag2-clear2/3. No complete owner mapping/exact credit
or runtime claim is added.

Control/effect140..174 adds53 values/49 primary targets, taking the same host to
103/154 handled values;51 values and EH lifetimes remain. Blocking153/154 returns
-1 through416BB0, bypassing the zero at416BAE; the previous zero-only host return
description was wrong and would lose the interpreter's pause contract. The spell
decoder uses signed length, byte key77/step7/step increment10 and a late opcode
reread; preserve the evaluated-but-discarded argument2. Its enclosing buffer
capacity is still unproved, so the compile-only context accepts workspace storage.
15D/15E and160/161 share actual low/middle tails and resolve destinations before
readers;163/164's difficulty guards/tables and opposite lookup order remain.
15A has no intermediate float rounding,15B uses an integer divisor,162 truncates
the wrapped signed product toward zero and170 rounds int to float before time
subtraction. Cold /GL probe3248 has all100 physical calls with matching callee
counts and all53 acyclic call/index skeletons; loops/numerical stages have24
separate static checks. These fragment observations grant no full-owner exact
credit. All prior source hashes remain unchanged; no existing exact unit is affected.

Records190..1C2 completes the host's154 handled opcode values/134 nondefault
targets.134..13F and175..18F are defaults, not missing handlers. The214-byte
records contain18 six-word pattern entries; basic/advanced packets are1E4/204.
Keep ordered offset.z>double0.899999976 for position selection and1B7's separate
unordered-inclusive not-less-than-990 marker. Three-bin record pairs use-512,
five-bin pairs-600, unlike earlier scalar-400. Advanced19D stores record values
at40/44,1AD at44/48;1B2 rereads the instruction after destination lookup. Real
1B8 new-expression cleanup is the sole unwind49FE50->48AA90 under FuncInfo49FE58.
The records cold /GL probe5568 agrees on186 direct/2 indirect sites, callee counts,
all51 acyclic call/index skeletons and29 static details. The combined switch
probe15776 retains all195 selector groups, all skeletons/projection roles/returns,
but native464/candidate460 body calls differ at raw-int-value11/10, actor creation
3/2 and ID lookup6/4. Decoder capacity/native wrapper and EH context remain open;
no complete host mapping or exact credit follows from opcode coverage. Real
StageMesh.cpp /GL support gives15772, still no EH, and specializes its sole17/17/0
caller to a213-byte constructor. Native has six constructor sites/five callers
and generic389. Preserve this context distinction; do not fake a private ABI or
add throwing/padding tricks. Those were workspace-probe observations, superseded
for current whole-owner measurement by the ordinary member source below.

The real manager/state ExecuteInstruction members now enclose all154 cases in
SecondaryActorInstructions.cpp. A case-local128-byte decoder array naturally
has the target132-byte distance to the cookie; its original declared capacity
is still unproved. Ending the spawn packet's shared lexical scope before other
families changes the initial member frame368 to native310, placing the basic/
advanced packets at8C and both spawn/name scratch at294, with cookie318. The
same /EHsc /GL member build also restores dual-cookie/EH registration and the
single allocation-delete unwind. No throwing helper/profile change is needed;
the earlier synthetic probe's absent EH was not a reliable native-source gap.
Complete member candidate15552 still differs from16556: five missing physical
body calls, FP/register/branch scheduling, decoder byte-key spills/length reload,
and cleanup's allocation slot EBP-310 versus native EBP-2F4. All195 selector
groups/call-index skeletons/projection roles/returns still agree. The manager
adapter naturally has11 bytes. This header declaration requires only its61
affected old exact units/22 families to be replayed; other source inputs remain
unchanged. Original TU/class/capacity and whole-host specialized EH exactness
remain separate open questions.

The current canonical owner also supplies real SecondaryScript.cpp /GL context:
host15696/frame320, packets9C/scratch2A4/cookie328. Its CL decoder key/cached length
and six ID lookup sites recover; remaining direct-site deltas are float107/106,
raw-int-value11/10, actor creation3/2 and vector-timer reset2/1. Candidate integer
index EDI versus native stack, float index EDX versus ECX and EH allocation slot
EBP-31C versus native EBP-2F4 remain concrete gaps. All195 static selector/call/
index skeletons, projection roles and0/-1 results agree. The7077-byte interpreter
support body still has VM ESI/frame134 and is not an acceptance unit. The complete
11-byte manager adapter alone closes through strict cold one-field replay;
the large host remains non-exact. All61 affected old exact units pass4,197 bytes,
with129 current source inputs frozen and only the state header changed among
the prior37. No full-cohort or behavior-suite rerun is required.

Release66/raw-position97 close through the existing ID value transport. Capturing
the actual marker pointer before position stores also closes offset132/direct110;
all8 new fields and the three double literal payloads are independently checked.

Controller/reset/death/movement and collision transition546 have complete maintained
source and remain non-exact. Collision preserves the primary plus32 VMs, section9
gates, both timers and age/selection branches; source543/257.
Real clear/drop callees remain concrete source work. Orbit255,
aim495, both12-byte callbacks, angle difference101 and life HUD144 close1,019
bytes with six strict cold primary-TU proofs and35 fields. Use real callback/
registration evidence for ABI rather than decompiler parameter guesses.
Gameplay roles and sibling similarity do not prove authored versus reused origins;
both additional death/HUD reviews remain indeterminate.

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

A local-lifetime experiment in the death-effect allocator gives233/41 instead of
228/37 and is not adopted; the maintained source remains unchanged.

Run7093 keeps the actual updater caller and typed trace branches. Arithmetic
50/52/54/56/58 publishes the updated left operand; predicate/bitwise results keep
a separate object. Native comparison tails remain shared, while extra50 and52/58
sharing is gone. Logical73/74 each use one push/result slot18. Angle87 now pushes
rounded X before reading Y, retaining math-before-destination lookup. Full7013/5821
is worse than prior7033/5779 as a masked diagnostic; local structural evidence
supports the source change, with no new exact credit. Native fixed120, all89
selectors/60-arm order and both RET4 paths remain. Missing calls localize to
unary84's merged converted/raw push and82/87's shared destination. Main VM remains
ESI versus native EDI. Ordinary member/accessor hypotheses are unproved; no frame,
ABI or trace workaround is adopted. See the interpreter topic for rejected probes.
