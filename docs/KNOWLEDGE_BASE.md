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
- Projected per-corner fog546 is a different path: fixed runtime eye+204,
  fractional fog channels, preserved alpha, no tint or far early rejection.
  Beyond range it uses the camera's packed fog color and original alpha.
- World draw1036 uses three real matrix objects, FVF102/stride20 and texture
  factor color arguments. Its UV cache repeats the U-offset test and never
  tests V. Keep that original condition. Matrix lifetime affects stack reuse.
- Modes9/C/D use triangle strip; B uses fan. The fan owner lacks eligibility
  gates, flushes global g_RenderBatch after local setup and disables Z writes
  without restoring them. Their full224-byte proofs and both258-byte UV
  proofs are exact. Dispatcher EAX forwarding establishes int wrapper returns.
- Explicit gaps recover six missed owners:451AE0/138,451B70/336,451CC0/336,
  452080/10,452090/44 and452680/25. All six have complete cold proofs,889 bytes;
  inventory at that checkpoint2,001 candidates. Ghidra DB remains unmodified.
- Fan805/598 uses4B0 storage,33 vertices,31 live edge radius/delta slots and
  a real30-byte polar boundary. Both UV axes use drift4A4;4A8 is unused.
  Negative UV shifts all33 vertices by1, positive values do not wrap. Edge Z
  accumulates after XY-only polar writes; preserve the closing copy1->32.
- Polar4526A0 preserves EDX in native code; opaque C++ declarations cannot
  supply that private fact. Its FSINCOS body remains open. Reversing world-draw
  source parameter order did not change1,038/750 codegen and was not adopted.
- VC9 rejects `#pragma intrinsic(_frnd)` with C4163. Precise sin/cos probes
  call CRT intrinsics; /fp:fast emits separate FSIN/FCOS. Neither experiment
  closes native inline FRNDINT or paired FSINCOS; those gaps remain open.
- Blend selector6 uses INVSRCCOLOR4, not SRCCOLOR3. Selector7 updates cache
  without blend API calls. The state application counter increments every call.
- Tint channels use byte inputs and an unsigned promoted product before SHR7.
  Leaving byte multiplication as signed promotion emits SAR, one opcode off.

- Renderer4526F0 is a2,601-byte constructor for allocation7BD894, with real
 4096/default/31 VM member lifetimes before whole-object memset. Its225-byte
 destructor saves next before reclaim and retains6 internal alignment bytes;
 EH and default-member inlining remain open. Neither is counted exact.
- Reclaim4558E0 needs rendererEDI and VMEAX, not a single VM parameter. The
 pool occupancy map is4096 bytes at renderer4340BC. Pool VMs free/clear400
 then reset; heap VMs free/clear400 then direct delete. This273-byte owner
 and487-byte world-buffer setup have full cold exact proofs.
- Projected scratch4C9248 is24-byte XYZ/fourth-scalar/UV, while world corners
 are20-byte XYZ/UV and shared4C91D8 vertices are28 bytes. Buffer setup leaves
 the scratch fourth scalar untouched. Avoid merging these physical layouts.
- Pixel ushort snapshots reproduce45-byte A1/A4 helpers; uint snapshots give42.
 The40-byte format selector tests runtime+200 as a byte, bit0. Six formats
 at496548 are followed by six bytes-per-pixel DWORDs, not extra formats.
- Renderer lifecycle/pixels add37 absent candidates and41 boundary/origin
 reviews, all indeterminate. Inventory at that checkpoint2,038; identity3-byte entries and
29 registration callbacks remain source-open. Five new full cold proofs add890
 bytes; no previous exact contexts changed. See the lifecycle topic below.

- Animation interpolation has four independently reviewed physical owners:
 integer XYZ768, integer scalar332, float pair673 and float scalar331. Integer
 XYZ truncates each weighted component before adding vectors; scalar integer
 sums before truncation. Ordinary paths also differ in when initial is added.
- Explicit completion returns/value snapshots and a four-float coefficient
 aggregate improve integer XYZ from723/647 to768/345 diagnostic differences.
 Its48-byte frame still differs from native40; equal size is not exactness.
- Real easing source support naturally closes four39-byte factor owners;
 opaque support pushed an extra curve argument and produced40-byte bodies.
 Real draw-dispatch support similarly closes renderer layer127. Allocator177,
 scalar timer reset52 and eight small physical accessors also have cold proofs.
- The renderer's31 extra VMs are29 ordinary layer sentinels plus layers29/30.
 Secondary update clears processedB8, remaps other layers to29 and uses a real
 two-slot tail array. Callback410 precedes interpreter44B4B0; callback414
 precedes drawing. Saved update-node next and late draw-next reads differ.
- Allocator4569C0 checks only two pool slots, then constructs/resets a heap VM;
 it does not scan all4096. Signed nextPoolSlot is4350BC; used map4340BC.
 The updated physical header triggered cold replay of exactly two old units.
- Interpolation checkpoint:2,047 candidates,244 reviewed boundaries,201 mappings and108
 exact functions/18,282 bytes. Twenty-two new origin reviews remain
 indeterminate. Fifteen new proofs plus two affected old proofs passed;
 no full cohort or broad behavior harness ran. See interpolation/dispatch below.

- Audio44A340 owns1,460 bytes:1,428-byte code plus32-byte eight-command switch
 table through44A8F3. Initial inventory omitted the table and Ghidra's inferred
 two fastcall inputs are unused. This is audio state-machine code, despite ANM
 neighbors. Full code/table/reference review corrects the maintained extent.
- Command removal advances the cursor during the shift; load's repeat branch
 does not reset to the head. Shutdown waits256ms and cancels the common step
 increment on retry. Preserve these actual state-machine behaviors.
- Sound effects use a rounded cubic attenuation curve; music uses a square.
 Native scalar volume helper125, enqueue151 and worker177 now have full cold
 exact proofs. The worker needs its actual CreateThread escape and three
 distinct shared-pointer reads; original C++ qualifiers remain unknown.
- Real reclamation support changes renderer update diagnostics to213/170 and
249/195, still missing native8-byte stack alignment. Draw127 and allocator177
 retain full zero proofs. Source/body changes to other exact families were
 unnecessary; only these two affected old units were cold-replayed.
- Audio queue checkpoint:2,047 candidates,262 boundaries,206 mappings and111
 exact functions/18,735 bytes. Eighteen further origin reviews are indeterminate.
 Three new audio plus two affected renderer owners pass complete cold replay;
 no full cohort or broad behavior suite ran. See the audio topic below.

- Full decoder review corrects SoundTrackRecord+1C from the earlier loop label
 to dataSize. LoopStart is+18; remaining is decoder+8, total size+2C, memory
 base/cursor/extent80/84/88, file8C, record90, name94 and saved position98.
 The queue still passes dataSize!=0 at its actual refill sites.
- Streaming refill570 and notification543 now have complete source, including
 lost-buffer restoration, lock/read/loop/silence/position paths that untyped
 Ghidra COM calls had wrongly marked unreachable. Full raw control flow wins.
 Restore calls twice per retry iteration. Decoder file read subtracts remaining
 before ReadFile, ignores its BOOL return, and returns the actual S_OK.
- Memory read snapshots cursor before clearing output count; this order matters.
 Memory binder43 mutates record/base/cursor/size/mode then returns E_NOTIMPL;
 that actual failure is retained. Four fade modes stop/complete differently
 and do not all force a final volume. Seventeen new full cold proofs add1,185 bytes.
- SoundBuffer now models the complete COM virtual method order, checked against
 the installed public protocol declaration. No implementation or tool payload
 was imported. Target RTTI names CSound/CStreamingSound establish names but
 do not prove unmodified sample/library provenance; origins stay indeterminate.
- Streaming checkpoint candidates2,060, reviewed boundaries285, source mappings232 and exact
 functions128/19,920 bytes. Thirteen omitted entries and23 new origin reviews
 are maintained. Only17 new plus3 affected SoundQueue exact owners were replayed.
 Refill545/500 and notification544/421 remain open, as do private read arguments.

- Full353-byte45A4F0 review corrects its earlier OpenMusicDecoder label: it
 recreates device buffers and sixteen notification positions, with no decoder
 file open. Source/dispatcher/inventory now use RecreateMusicBuffers. Controller
 description38 is36 bytes, device-owner pointer5C and notification event74.
- Primary setup272 preserves the full GS-protected PCM44100/stereo16 format
 path and native failure lifetime. Device init66 ignores its format HRESULT
 after successful creation/cooperative setup. Ordinal11 thunk45F9B0/IAT48B008
 is independently verified; installed export metadata supplies DirectSoundCreate8
 name evidence only. Public protocol declarations import no implementation.
- Refill570 re-review found three missing zero initializations and a missing
 repeated-reset failure return. Those are corrected; first reset result stays
 ignored. Candidate569/504 remains open. BYTE-pointer and genuine member-read
 probes did not change the larger gaps; no ABI/register forcing is adopted.
- Decoder snapshot across CloseHandle and buffer snapshot before active
 publication close Stop129/Resume77. Selector100 and any-playing78 also close
 through observed branch/initialization order. Nine new cold proofs add1,103
 bytes, including buffer recreation353 and primary272. Exactly9 new plus20
 affected old owners pass; no full cohort or broad suites ran.
- Device checkpoint:2,067 candidates,297 reviewed boundaries,237 mappings and137 exact
 functions/21,023 bytes. Twelve new origin reviews stay indeterminate; seven
 missing entries are preserved. Factory713/665 full intervals include10/8
 internal alignment omitted by Ghidra's sparse bodies; source/EH remains open.

- Factories713/665 now have full natural source and a real0x38 sound base /
 0x7C streaming derivative. Decoder0x9C constructor writes only record90,
 unknown DWORD0, dataSize2C and memoryMode7C; other state stays uninitialized.
 Memory binder failure remains ignored; file open rejects every nonzero HRESULT.
 Observed allocation/cleanup asymmetries and subsequent description publication
 are retained. Base ctor count1 specializes naturally from its derived caller.
- Ordinary base destructor133 and derived11 each have one unique complete public
 primary-TU contribution and six independent fields total; cold full-byte proof
 passes. Real inline empty derived destructor also emits scalar36 diagnostic0,
 but scalar/vector public aliases share both30/36 starts. Strict cold ownership
 rejects them. No Oracle exemption or artificial deleting wrapper is introduced.
- Removing extra Read/Refill diagnostic roots changes neither larger body even
 after real factory/lifetime callers are present; that hypothesis is rejected.
 Final factories675/590 and595/556, base ctor116/83, derived ctor48/23 remain
 open, including EH ownership. Other audio codegen gaps persist.
- Factory checkpoint:2,072 candidates,305 reviewed boundaries,245 mappings and139 exact
 functions/21,167 bytes. Eight new reviews are indeterminate; five missing stack/
 frame accessors are recovered. Adjacent584 full owner includes one alignment
 byte omitted by sparse583. Its6759-byte interpreter caller45B860 still needs
 complete owner/table review. Exactly2 new plus29 affected old units pass cold.

- Secondary interpreter45B860 was6,759 code-only bytes; full owner is7,093:
 6,759 code+1 native NOP+244 jump table+89 selectors. All61 Ghidra DATA edges
 agree independently with raw PE entries and become internal to corrected owner.
 Complete1,783-instruction review replaces any250-instruction prefix assumption.
- New secondary-script source covers all dispatch arms, typed stack/references,
 frame transfer, thread scheduler and lookup. VM top/frame are1008/100C;
 stack-local1000 is top,1004 frame. Old getter/setter frame-offset names were
 corrected. A missing83-byte typed pop writes output before reading/converting
 its tag; underflow leaves output unchanged. Reference and argument underflow
 sentinels differ and are not collapsed into a zero fallback.
- Opcode85 performs integer NEG on a float bit pattern, not floating negation.
 Frame lookup failure leaves new manager.current published and clears caller
 instruction. Squared-length coordinates are rounded before output lookup,
 arithmetic occurs after its callback. Trace int-to-double promotion avoids
 added float rounding. These follow raw code, not decompiler float casts.
- Per-operation instantiation and distinct branch/thread/unary arms reproduce
61 jump entries/89 selectors, but current candidate6,993/5,880 is still non-exact.
 Candidate stack aligns64 with an EBP frame; native uses ESP/120. /Zp4 changed
 none and was rejected. A private double-to-float-temporary probe retained
 alignment64, produced6873/5934 against the preceding source and risked changed
 rounding; it is rejected. Genuine Stack members close relative15 and setter7.
 Nine unique complete public owners pass296 raw-equal bytes with zero fields.
- Program vptr49404C has Install45D900 at0 and integer-result extension callback
40FFA0 at4, not a virtual destructor. Derived494090 overrides slot4 with410D50
 and adds filename loading4104D0 at8. Native base callback returns0/RET4; the
 derived callback's failure path returns-1. No invented destructor slot.
- The old410D50/25 and410D69/213 are one238-byte callback: gate JE enters its
 success continuation, which shares saved self/input/ABI and RET4 paths. Full
94-instruction read-only span and independent PE decoding have no foreign
 interior entries. Merge only these unmaintained auto candidates. The callback
 processes ANIM first, then optional aligned ECLI; the former include-only label
 was too narrow. Its complete source remains open.
- Full513-byte SCPT/version1 installation source preserves file-slot publication,
 invalid-header slot clearing, table copy/free and sorted insertion through the
 full allocated count, including unfilled slots. Candidate513/339 is not exact.
 Missing context entries45DBE0/28,45DC00/36 and45DD50/28 have distinct program
 publication and return contracts; do not invent current initialization or guards.
 The first two plus genuine base callback5 pass69 exact bytes/two REL32 fields.
- Non-float-first typed-pop conversion reduces83/27 to83/2; remaining register
 SUB4 versus ADD-negative bytes stay unmasked. A true Stack-member probe changes
 neither Pop83/2 nor End41/2 and is rejected. Shared inline changes alter larger
 diagnostics, so nine prior exact script units were revalidated.
- Installation checkpoint:2,075 candidates,339 reviewed boundaries,272 mappings and151 exact
 functions/21,532 bytes. Twelve new origin reviews add eleven indeterminate and
 one PE-import-verified DirectInput8Create exclusion; one false split is removed.
 Browser934's267-instruction full export supersedes its truncated250 view;
 adjacent188/177/84 are effects, not script lifecycle. Three new plus nine
 affected units pass cold; no other family or full cohort was replayed.

- Full ANIM/ECLI extension238 and filename entry95 now reproduce complete
 public primary-TU owners. The parser publishes each loaded animation pointer
 before testing failure, preserves signed relative alignment and ignores ECLI
 load statuses. A normal repeated remainder expression closes the66 differences
 caused by a retained temporary. Independent PE decoding binds all ten external
 address fields across both functions; no table/padding/ABI injection.
- Real resource-loader support needs independent variable filename/size/mode
 inputs. Otherwise LTCG specializes away the file entry's two null/zero pushes,
 producing91 bytes. The proper context restores the native95-byte entry.
- Animation trace456AD0 and script trace45B5B0 are genuinely RET-only. Actual
 bodies preserve caller volatile registers; an opaque declaration does not.
 Native animation cached calls keep both format and filename pushes. All values
 are variadic in the reconstructed trace API; a fixed unused format argument
 would be trimmed by LTCG and loses native pushes. There is no invented success
 implementation. AnimationTrace.cpp supplies the real body only to its declared
 new contexts; other families retain their existing source partitions.
- Trace-arm ablation changes interpreter alignment64 to8. Removing only its
 double variadic trace call does the same, localizing that alignment request.
 Both omit native behavior and are rejected. A fixed-format trace-body probe
 and /fp:fast retain64; neither is adopted. Source keeps the actual QWORD trace
 value and established /O2 /Ob1 /GL. Full real-trace Run6993/5880 stays open.
- Seven omitted entries include file getter40FFB0/5, base constructor40FFC0/38,
 four singleton helpers410530/11,410540/9,410550/10,410560/21 and derived
 constructor410D20/38. Base writes vptr then memset1098 erases it; derived
 memset1098 then writes vptr. Native browser order was real, not a decompiler
 artifact. At that checkpoint neither constructor had source; the lifetime work
 now supplies raw1098 storage and genuine inline base/ordinary derived source.
- New host initialization318 and state reset126 are fully reviewed; all ten new
 origin reviews remain indeterminate. Extension checkpoint:2,082 candidates,349 reviewed
 boundaries and277 mappings. Five new exact owners add340 bytes:156 exact /
21,872 bytes total. Affected cold selection uses actual receipt dependencies,
 including transitive headers:79 prior units plus five new; no full cohort.

- Lifetime checkpoint:2,097 candidates,385 reviewed boundaries,291 mappings and
168 exact/22,118 bytes. Fourteen new source mappings cover355 bytes; twelve
 unique complete primary-TU contributions close246 bytes/eight fields. Thirty-six
 new origin reviews add34 indeterminate and two compiler exclusions;15 omitted
 entries are recovered. Only12 new+16 affected prior units cold-replayed.
- Program raw storage is1098, with known prefix through8C, unknown90..108F and
 observed DWORD1090/1094. A normal inline base constructor enables derived38 to
 inline the real zeroing and publish494090 afterward. Base38 still has3 schedule
 differences and support ownership. Program destructor36 frees only the table;
 master cleanup374 separately frees all32 file slots before deleting program.
- The manager really has six virtual slots: five concrete default bodies and
 virtual deleting destructor14. Former pure-virtual declarations were incomplete.
 Actual defaults3/5/5/5/5 and ordinary destructor47 now pass full proof. Generated
 scalar/vector aliases emit30 versus native71 with inlined cleanup; remain
 source-backed/non-exact, no fabricated deleting wrapper or Oracle exemption.
- Counter fields0/C/10/14 preserve wrapped scaled7 arithmetic and signed division
 by7, direct subtraction when flag0 is clear, and low-bit replacement rather
 than boolean normalization. Full61/13/16 agree; full original counter type is
 not inferred from matching offsets or repeated primitive accessors.
- Actor update411750 owns4230, not sparse4221:1108 instructions, five returns,
 nine internal alignment bytes, no tables/foreign interior entry. Full instruction
 export and independent decoding replace the initial250-instruction prefix. Complete motion/tween,
 script/hooks, damage, animation IDs, mesh deformation and time/status flow is
 reviewed; full source now emits4177/3219 and remains non-exact. Constructor699 and EH destructor384 now have
 complete natural source but remain non-exact; base103C+state163C gives actor2678.
- Ordinary actor adapter411600 is12 bytes through41160B; its first13-byte review
 span included a neighboring CC. It calls the real4230 owner on object+103C and
 now passes12 exact bytes. It is not an EH action: FuncInfo49FB78/unwind49FB70
 explicitly points48A730/8, then to ordinary base destructor40FF10. Handler48A738
 checks GS cookie and selects that FuncInfo/CxxFrameHandler3; PE SafeSEH table
49E960/56 independently contains its RVA. These two compiler entries are excluded.
- More trace experiments fail to close interpreter alignment: split calls give
6993/5877, float conditional7001/5937 and per-format arms7141/5984; all retain64.
 None is adopted. Canonical full Run6993/5880 and /O2 /Ob1 /GL remain; no source
 type/precision/alignment trick replaces the still-open native ESP/120 context.

## Actor members and complete interpolation

- State410440/126 is explained by real member construction: nine early and two
 late timer-bit clears, with eight214-byte record constructors between them.
 Empty out-of-line state construction naturally reproduces all126 bytes. The
 actor then genuinely clears the state again. Native float defaults are24/32.
- Actual script support removes extra actor-constructor EH, reducing766 to706
 against699. A stack-constructor model704/493 does not close the gap and is
 rejected; common headers remain unchanged. EH destructor384 has full source390,
 with private receiver/base-inlining/alias ownership still open.
- Pair interpolation673 has complete source and observed output-buffer/returned
 pointer contract; original vector return ABI is open. POD register return was
 wrong; no invented constructor forces sret. Current632/627 stays non-exact.
 Actual easing support closes factor39 naturally. Thread release45 leaves head
 untouched; reset64 preserves VM stack cursors/program. Both pass full proof.
- Actor factory4108F0 owns621=545 code+3 alignment+20 table+53 selectors. Five
 table DATA edges are internal, not foreign interior entries. The previous545
 boundary omitted76 owned bytes. No factory source/exact claim follows.
- Six new complete primary owners/327 bytes/six fields pass selected cold proof;
 actual prior receipt dependencies select no affected old units. Eighteen new
 origin reviews remain indeterminate. See [SECONDARY_ACTOR](SECONDARY_ACTOR.md).

## Full actor state and pointer-return contracts

- Complete4230 state source preserves motion/tweens, script/hooks, damage and
  transitions, collision, animation/tracking/flash, mesh geometry/color and timers.
  Real support naturally restores stack receiver/frame92,4055/3295 from initial
  4023/3510. Full frame body/counter context yields4177/3219. All64 direct-call
  target multiplicities,3 callbacks and5 returns agree; exact/runtime stay open.
- VM helper456520/21 returns the resolved pointer, independently required by
  the next VM3B0 dereference. Earlier no-bool-only description was incomplete.
  Separate419A20/30 returns integer0/1. Retaining independent renderer input
  prevents false LTCG specialization of lookup79 and closes full21/30/70 owners.
  Event70 writes word37C, not the release flag. Native alignment stays included.
- Motion18 is speed/angular increment by flags and1C is angle; prior names were
  ambiguous. Advance75 exact uses actual FSINCOS30/Normalize145 boundaries;
  FSINCOS and quantizer99 remain source-open. Timer96/6 is operand scheduling,
  not a reason to change precision or add volatile accesses. Transition561 has
  full life/time/UI/global source521/497, not a reduced script-name lookup.
- Score51 exact preserves wrapped addition, signed input/10 and upper cap; direct
  global assignment recovers native scheduling. Count19 exact preserves signed
  upper gate. Damage1579 has full510-instruction/two-return review with13 native
  alignment bytes missing from sparse1566; both256/32-record loops are covered.
- Seven new primary owners/294 bytes/14 fields and seven affected old owners pass
  final selected cold proof; declaration-header comment correction adds adapter12. Thirteen new origin reviews remain indeterminate. See
  [SECONDARY_ACTOR_STATE](SECONDARY_ACTOR_STATE.md); no full-cohort replay.

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

Compiler probes and exact cold proofs support current work. Retired behavior
suites and historical test counts are not prerequisites; cleanup does not
require a full-cohort replay.
