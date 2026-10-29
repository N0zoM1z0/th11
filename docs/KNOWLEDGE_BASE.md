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
 wrong; no invented constructor forces sret. Current673/352 stays non-exact.
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

## Pair arithmetic, omitted leaves and vendor floor

- Native pair673/vector883/fog737 form Hermite a as `(t-1)*(t-1)*(2*t+1)`;
  the former factor-first tree was wrong. Natural source now preserves the
  square-first products and all float rounding boundaries. Initial snapshots,
  separate terminal paths and component publication recover EDI/frame28/six
  returns and full673 bytes, with352 masked differences. No nominal return ABI,
  constructor or frame is invented. Shared vector785/824 and fog536/643 stay open.
- Twelve small pair/vector owners188 bytes receive complete source and boundary
  review; eleven were missing candidates.419520 was already present and4195D0
  is inside4195C0. Eight full primary owners130 bytes/one field pass strict cold
  proof; two preset copies19/8, duration4/1 and addition16/4 stay non-exact.
  Original class/header provenance stays indeterminate.4195F0/101 gets full
  origin/boundary review;419660/31 is the existing cosine wrapper, not atan2.
- Math source uses real VC9 float sin/cos/floor overloads and observed double100
  scaling/float spills. Quantizer99->88/74 and rotation86->82/73 retain native
  frame-alignment/x87 gaps. No forced aligned local or substitute FSINCOS is used.
  Full4230 state caller with actual pair/math support is4161/3261; independent
  audit again agrees on64 direct-call targets/multiplicities,3 callbacks,5 returns.
- Vendor COFF symbols/CodeView lengths split automatic floor3113 into64-byte
  dispatcher486590 and225-byte SSE owner4865D0. Complete289-byte text/10 fields
  and80 constants replay identically. This establishes VC9 CRT library origin
  and two exclusions, with no reconstruction/exact credit. Observed archive hash
  is not falsely described as pre-pinned. Fallback4870E4 and unrelated runtime
  helpers remain separate/open.
- Twenty-seven selected cold replays pass:8 new and19 affected old units, selected
  from actual source hashes. Fourteen new mappings cover373 target bytes;15 new
  origin reviews are13 indeterminate and2 library exclusions. No full cohort or
  routine behavioral suite. See [SECONDARY_PAIR_MATH](SECONDARY_PAIR_MATH.md).

## Full damage, collision and timer-owned regions

- Damage4347F0 owns1579 bytes/510 instructions/two RET8, with13 native alignment
  bytes beyond sparse1566. Real two-stack-argument source now covers all256
  projectile and32 region paths, callback/events/animation transitions, exact
  float boundaries, wrapped accumulation and nonlinear30/70/80 compression/score.
  Current1513/1414 is open; all14 direct-call targets/multiplicities, one callback
  and both returns agree independently. Ghidra's extra register parameters are
  spurious, not a reason to add dummy inputs.
- Preserve ordered rejection/unordered branches rather than rewriting overlap as
  `<=` conjunctions. Native distance differences and expanded graze radius stay
  on x87 until the actual final float stores. Removing extra float-local stores
  changes source1494/1427 and collision264/235 to1478/1417 and228/211; natural
  record-pointer/countdown loops give damage1513/1414. No forced aligned local.
- Region allocator433A90/192 returns one-past all32 occupied records, not null.
  Genuine0x74/0x34 clears, timer initialization and damage/limit/period stores are
  preserved. Local timer reference closes192/0 from192/24; a position snapshot
  does not. Timer member29, event2/event3 owners70 each and allocator192 pass
  complete strict cold proof:361 bytes/seven fields. Event3 never means zero.
- Projectile96C+54 is the existing partial actor-reference pointer9C0; do not
  rename the whole0x6C record into an actor. Native region timer-period skipping
  is opposite to kind4 projectile elapsed-period damage; preserve both predicates.
  Full positional sound148 source144/93 reads signed record+2, double x*1000/192,
  ordered12 slots and128 cap. Special dispatcher406720 owns100 code+24 table;
  six case DATA refs are owned, not foreign interior entries. Source83 stays open.
- Fourteen new full origin/boundary reviews include special helpers246/385/66,
  collision transition546, patterns142/183, projectile update896 and homing498.
  The896 owner needs278 instructions/full650-limit export, not truncated250.
  All14 origins remain indeterminate. Ten mappings add2557 target bytes. Actual
  old receipt hashes select zero affected old exact units; only four new units
  are cold replayed, all pass. See [SECONDARY_DAMAGE](SECONDARY_DAMAGE.md).

## Full projectile update and literal/iterator contracts

- Complete896/278-instruction projectile source preserves all256 records, events,
  motion, callback, inline lookup, asymmetric cleanup, culling, VM output/flags
  and timer. Current908/765 remains open; all13 calls, one callback and return
  agree independently. Full498 homing source493/401 keeps adjusted acquisition,
  actual fixed callback ABI and speed/turn gates; all5 calls/4 returns agree.
  Address escape at descriptor4A3A40 prevents false LTCG ESI-receiver specialization.
- Acquisition123 closes when next is captured before work and assigned at the
  end; immediate advance121/28 and normal for121/91 do not. Angle101 local double
  objects introduce117-byte aligned code. Static storage gives101/0 but lacks
  public data bindings; natural literal expressions emit ordinary __real symbols
  and close full101 with all3 field/constant values. Outside93 closes with the
  actual ordered -192/192/0/448 predicates. No Oracle or static-owner exception.
- RET4 on tracked-target73 proves a third unused stack word; old two-parameter
  hypothesis was wrong. Initial correct physical source61/68 kept cleanup width.
  Later explicit double sqrt API calls in the same TU naturally close73 bytes;
  nominal third-slot type/role remains unproved. Other47/48/31 wrappers stay open.
- Special damage385 scans exactly five script IDs, not the prior erroneous32.
  It begins at embedded VM node+10, includes root and preserves native lookup-null
  fault after clearing ID. Full246/385 source252/204 and385/337 stays non-exact.
  Four affected old exact helpers passed that checkpoint; later cross-owner
  review corrects the old parameters64 view to68, with acceleration64.
- Nineteen new reviews give17 indeterminate origins and two library exclusions.
  Vendor COFF/CodeView proves ordinary atan2/CIatan2 lengths10+10; entire20-byte
  text/four fields agrees, ordinary486C90 was missing. Library code receives no
  source/exact credit. Additional symbol lookup confirms __CIsqrt486AA0 without
  adding review credit for an incompletely reviewed runtime owner.
- Eleven mappings cover2541 target bytes. Three new317-byte/eight-field owners
  and four affected old361-byte/seven-field owners pass selected cold proof.
  Initial new-owner registration ordering was corrected using the same current
  artifacts, with no recompilation or full cohort. See
  [SECONDARY_PROJECTILES](SECONDARY_PROJECTILES.md).

## Spawn, shared-layout correction and integer leaves

- The old parameters64 assertion checked our own layout, not native truth.
  Spawn433FE7 publishes at68; damage biases record+4C then reads1C, update biases
  record+20 then reads48, and gravity4352CF reads acceleration64. Correct shared
  record fields are damage60, float acceleration64 and parameters68; size6C and
  target54/global9C0 remain fixed. Generic emitter/gravity callbacks need no
  alternate record interpretation to explain them.
- Full spawn853/258 instructions preserves the partial timer reset,256-record
  scan, kind4 slot gate, signed descriptor fields,1/128 emitter position,
  ordered995/1000 sentinels, two RNG draws and their actual float stores,
  angle perturbation pi/12, motion, inline VM lookup, callback/context, sound and
  VM position. Current824/745 is open; all8 direct calls/one callback agree,
  but source has one RET8 against native two. No forced ABI/frame/null guard.
- Emitter address75CC+index*E4 and ID7620+index*E4 close14/16 bytes. E4 is a stride;
  that earlier coordinate prefix alone did not establish the containing base.
  Later render/reset evidence proves eight physical E4 records at7570..7C8F,
  with coordinate prefix at+5C. Integer conversion26, nonzero20 and
  component publication6 also close. The six other integer-pair owners retain
  full source, wrapped arithmetic/signed division and complete open diagnostics.
- Explicit double sqrt calls with float results leave both length-helper bodies
  unchanged33/34, but restore tracked-target73's aligned frame in the same TU.
  This measured compiler effect closes73/0; it does not establish original TU
  partition or nominal callback type. Double fabs/atan2 variants do not close it.
- Controller431070 owns2888=2867 code+one NOP+five-entry table, correcting the old
  2867 count. Complete695 instructions/two returns and five table DATA references
  are checked independently. Replay initializer4356E0/1380 is state/record setup,
  not collision geometry; all406 instructions/four returns are covered.
- Twelve mappings add1087 bytes;17 new reviews are indeterminate,11 missing
  candidates are added. Six new exact owners pass155 bytes/two fields, and7
  affected old owners pass678 bytes/15 fields. All13 selected cold proofs pass
  with frozen source. Fresh damage/update diagnostics keep the previous numeric
  totals under the corrected header. See [SECONDARY_PATTERN](SECONDARY_PATTERN.md).

## Real pattern callers, emitter records and callback precision

- Render431BC0/130 visits8 callback slots at7650, passes slot minusE0 in ECX and
  advancesE4. Reset2508 and position refresh110 independently corroborate record
  base7570, coordinates5C, angleA8, IDB0, refreshD4 and callbackE0, ending exactly
  at slot7C90. The old75CC prefix was an internal coordinate view; the earlier
  whole-array restriction based on that prefix is superseded by this evidence.
- Loader431C70 closes all162 bytes, including14 alignment bytes beyond sparse148.
  ESI receiver and ECX filename are actual inputs, despite Ghidra's no-argument
  hypothesis. Unsigned16 file count2, eight-byte entry268 rebasing and all four
  indexed callback tables are preserved. Descriptor0/1 are signed period/phase;
  negative period ends the list, and frame%period drives emission without a new
  zero guard. Actual tables establish shared onSpawn/onUpdate signatures.
- Full source now covers frame142, update183, render130, long homing399,
  gravity132, emitter112, initialization12/59/70, detach59, fixed caller31,
  timer13 and multiplier12/12. Long homing releases at age>=120, correcting the
  earlier >120 description. No registration proves its nominal fixed callback
  ABI, so one observed EDX input does not become a fabricated ECX owner slot.
- RNG state specialized away in a global-only context. An independent real state
  input restores ESI and leaves one opcode difference in acceleration70. Explicit
  float unsigned conversion emits native float2^32 correction, while retaining
  x87 precision until the actual signed-unit float store; all70 bytes close.
  Four public constant payloads agree across28 bytes. Spawn's two draws adopt
  the same contract, and its float3 now snapshots before publication.
- Spawn remains full853 versus source849/733 with real callers; real-only/independent
  RNG859/690 also remains open. Actual positional-sound support and the complete
  controller are concrete next context work. Double APIs/conditional expressions,
  offset helper and opaque normalization do not close gravity/emitter source;
  no forced register, local alignment, fake ABI or padding is adopted.
- Reset432CC0 owns2508=2457 code+3 alignment+two six-entry tables, including15
  internal alignment bytes beyond sparse2442. All696 instructions/12 table targets
  and incoming DATA refs are reviewed. Complete source remains open. Forty-three
  alleged interior refs in4355E0..43562F are based renderer member displacements,
  and49353C's435246 value is ASCII FRC. Byte attestation does not validate Ghidra's
  reference interpretation; native operand/data structure decides it.
- Sixteen mappings add1564 bytes;16 new reviews remain indeterminate and10 omitted
  candidates are added. Eight new exact owners pass360 bytes/17 fields and10
  affected old owners pass781 bytes/16 fields. All18 selected cold proofs pass
  with frozen source; no full cohort. See
  [SECONDARY_PATTERN_CALLBACKS](SECONDARY_PATTERN_CALLBACKS.md).

## Complete emitter reset and list factory review (2026-10-29)

- Full reset432CC0/2508 now has maintained C++; root2444/1967 remains non-exact.
  Raw instructions correct branch3 focused base148, branch4 seven pairs at64..9B
  with bases28/28/28/78/C8/118/168, and full32-bit dynamic scripts. Branch2 writes
  float polar pairs but uses previous integer offsets. Maximum-power auxiliary
  processing precedes slot equality. No clamp8/blanket callback or ID clear.
- Native/source have30 integer conversions,5 auxiliary factory calls and2 angle
  normalizations. Main factory call sites4 versus1 expose a real VC9 merging gap;
  source presence does not prove the open owner's call shape or runtime behavior.
  Slot4355F0/24 closes naturally with the real reset definition present.
- Primary prepend455760/118, secondary append4557E0/123 and secondary prepend
  455860/118 are distinct. Shared ID counter increments and skips0. A natural
  pair of head tests restores EBX in factory455BC0/112; it and455DF0/112 close.
  Publishers stay111/33 and116/47 with counter/register differences. Generic
  insertion-helper and externally named registry variants do not close them.
- Thirteen new complete reviews cover1694 bytes/427 instructions; previously
  omitted455E60/151,455F00/167 and455FB0/105 have independent RET/CC boundaries.
  Positioned factories use direct Bind44ACD0/204, not zero-position initializer
  456910/171 with Bind44ADA0/191. Offset factories preserve double x+32+192/y+16.
  All13 origins stay indeterminate; gameplay role/common scaffolding is no
  original class/header/library-versus-game authorship evidence.
- Real Sound source with actual Spawn853 callers gives846/733; real-only with
  independent RNG856/690 also fails. No speculative Sound source is adopted.
- Six new mappings cover2997 bytes. Three new exact owners close248 bytes/24
  independently reviewed fields. Ten old units selected by actual Pattern.hpp
  receipt dependencies also pass:13 selected cold units/638 bytes/41 fields,
  frozen source, no full cohort. See [SECONDARY_EMITTER_RESET](SECONDARY_EMITTER_RESET.md).

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
