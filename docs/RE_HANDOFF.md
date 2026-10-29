# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; outer folder
is not Git. Read AGENTS.md/current source/ledgers. Target bytes/current receipts
outrank historical notes. All dirty work belongs to this active goal.

## Checked-in state

- Original Japanese TH11 v1.00a; config/target.toml pins identity.
- 2,109 candidates;431 reviewed boundaries;333 source mappings.
- 189 exact functions/22,869 bytes:8 normal COFF and181 linked PE.
- Origins:90 authored,327 indeterminate,12 exclusions,1,680 pending.
- Direct Ghidra/scripts; no Factory MCP/REA. VC9 RTM21022/SDK6.0A.
- Commit/push authorized; prefix `gpt-6.1-sol: `.

Latest checkpoint adds14 complete mappings/373 target bytes and8 exact primary
owners/130 bytes/one field. Fifteen new origin reviews yield13 indeterminate and
two independently identified VC9 CRT floor exclusions. Pair673 is now673/352;
shared Hermite product trees are corrected. See
[SECONDARY_PAIR_MATH](SECONDARY_PAIR_MATH.md),
[SECONDARY_ACTOR_STATE](SECONDARY_ACTOR_STATE.md),
[SECONDARY_ACTOR](SECONDARY_ACTOR.md), [SECONDARY_LIFETIME](SECONDARY_LIFETIME.md).

## Current actor source and evidence

- State411750 owns4230 through4127D5, not sparse4221:1108 instructions/five
  returns/nine internal alignment bytes, no table/foreign interior entry. Complete
  source now includes all motion/tweens, script/hooks, damage/transitions, collision,
  eight IDs/tracking/flash, mesh vertices/colors/normalization/sine and timers.
- Initial4023/3510 has EAX receiver/frame100. Real support restores native stack
  receiver/frame92,4055/3295. Full frame body, separate sound branches and counter
  support give4177/3219; actual pair/math definitions yield current4161/3261. All64
  direct-call target multiplicities,3 callbacks and5
  returns agree independently. This does not prove exactness/runtime. No prefix
  or simplified-loop replacement, forced frame/register, or precision shortcut.
- Physical actor is base103C+state163C=2678. State ctor410440/126 is an empty
  out-of-line body plus11 timer/eight214-byte record constructors; exact. Actor
  then clears state again. Defaults24/32 are verified. New source fields expose
  script-pointer slots, hit sizes, flash, mesh limit/color/phases and callbacks.
- Motion18 is speed/angular increment selected by flags;1C is angle. Advance75
  exactly calls actual FSINCOS SetPolar30 and Normalize145. FSINCOS30 remains
  source-open. Quantizer99/rotation86 now have complete source88/74 and82/73;
  floor486590 is independently identified VC9 CRT. Compose215 stays non-exact.
- ClearMissingAnimationId456520/21 really returns the VM pointer: native state
  caller immediately dereferences VM3B0. Validate419A20/30 instead returns full
  integer0/1. Independent renderer probe input prevents LTCG from removing the
  lookup renderer parameter; full21/30/70 are exact. Event70 writes word37C=1;
  release66 marks04000000. Do not conflate these contracts or insert null guards.
- Transition561 source preserves life/time paths, signed UI time, frame/script
  returns and spell/global gates. Tracking comparison is counterintuitive but
  observed; preserve its direction. Timer96 is96/6, near-one FLD/FADD scheduling;
  compound/reversed spelling failed, no type/volatile trick. Score51 uses wrapped
  addition plus signed input/10/cap; count19 preserves its signed upper gate.
- Native complete damage1579 has510 instructions/two RET8 and13 alignment bytes
  missing from sparse1566. Full650-limit export covers both256/32-record paths,
  callbacks/animation/damage/score. Collision244, positional sound148, effect457
  and FSINCOS30 have full new origin review but remain source-open.

## Other corrected ownership

- Program1098 includes known prefix/table8C, raw90..108F and tail1090/1094.
  Inline base ctor writes49404C then memset erases it. Empty derived ctor naturally
  publishes494090 after clearing; derived38 exact, base38/3 support-owned/open.
  Nonvirtual program dtor36 frees only table; master cleanup374 frees32 file slots
  first. Do not move that ownership or invent a virtual program destructor.
- Manager494058 has five real zero/null/FLDZ defaults and deleting slot14.
  Defaults and ordinary dtor47 exact; generated scalar/vector aliases30 vs71 stay
  open. Actor deleting slot14 belongs494074. Native EH384, unwind48A730/8 and
  handler48A738/27 remain separate. Actual FuncInfo49FB78/unwind49FB70 points8;
  adapter411600/12 is ordinary/exact, never an EH action. SafeSEH49E960/56 agrees.
- Factory4108F0 owns621=545 code+3 alignment+20 table+53 selectors; five table
  DATA edges are internal. Valid attested code/refs plus independent PE/table
  review establish extent; failed mixed-data linear review_span is not evidence.
- Interpreter45B860 owns7093:6759 code+1 NOP+244 jump+89 selectors. Full1783
  instructions/61 table edges reviewed. Never crop to6759, restore false410D69
  callback or create an owner at table cell45D2D0.

## Current gaps and next work

| Owner | Full target | Source / normalized differences |
| --- | ---: | --- |
| Actor state411750 | 4230 | 4161 /3261; full actual support context |
| Actor transition417B40 | 561 | 521 /497 |
| Actor ctor4111C0 / EH dtor411480 | 699 /384 | 706 /442;390 /327 |
| Actor deleting410420 / unlink410B60 | 31 /82 | 30 /20 aliases;79 /74 |
| Actor position411610 / pair4191F0 | 309 /673 | 310 /269;673 /352 |
| Compose459590 / timer459210 | 215 /96 | 208 /180;96 /6 |
| VM release4562E0 / position456430 | 66 /110 | 64 /39;110 /45 |
| VM position4563A0 / raw456330 | 132 /97 | 126 /104;86 /85 |
| Collision value40BAA0 | 95 | 92 /43; private EAX vs stack input |
| Program install45D900 / restart45DD50 | 513 /28 | 513 /339;28 /10 |
| Run45B860 / enter45B610 / update45D420 | 7093 /584 /142 | 6993 /5880;572 /546;155 /142 |
| Float/int args45D550/45D4B0 | 175 /146 | 184 /168;124 /125 |
| Int/float refs45D600/45D670 | 100 /145 | 94 /85;147 /85 |
| Pop45DC80 / push45DC30 / end45DD20 | 83 /71 /41 | 83 /2;70 /15;41 /2 |
| Start thread45D7A0 | 147 | 154 /61 |

Continue complete large owners and codegen gaps. Pair673 naturally recovers EDI,
frame28 and six returns via initial snapshots, component publication and native
Hermite product trees, but352 differences remain. Original vector return ABI is
unknown; do not invent a constructor/frame/register. Quantizer99 and rotation86
have frame-alignment/x87 scheduling gaps; do not force aligned locals. New pair
preset copies19/8 each, duration4/1 and addition16/4 remain open. Position110
moves marker comparison after stores; timer96 swaps FLD/FADD memory operands.
Installer retains full-count insertion/publication/ignored extension result.

Interpreter64-byte alignment is localized to its required double variadic trace.
Ablations omit behavior; fixed/real trace bodies, Zp4, fp:fast and float temps
failed. Split calls6993/5877, float conditional7001/5937 and format arms7141/5984
retain64; none adopted. Native ESP/120 frame remains open. Opcode85 negates float
bits as integer; reference/argument underflow differs; frame failure leaves new
current and clears caller instruction; geometry86 resolves destination first.
Counter61/13/16 retain wrapped7 arithmetic/signed division/input low-bit semantics.
Audio aliases/factories/refill, VM11640, rendering and master374 remain open.

## Current proof and replay scope

`.analysis/secondary-pair-math-cold-replay.log` passes8 new+19 directly affected
prior units, across6 linked families. Full bytes, exhaustive fields, unique
primary-TU owners and current source/header/support/tool/producer/PE/MAP/PDB
receipts pass. Actual old receipt hashes select StageTween, StageScript,
AnimationTween, SecondaryPairTween and SecondaryActorMotion dependencies.
Source froze before the cold build; all jobs are terminal. No full cohort,
broad behavior or infrastructure suite. Recompute dependencies next time.

Private diagnostics: `build/probe-ltcg/SecondaryActorUpdate/pair-math-full/`,
`.analysis/secondary-pair-math-final-open-diagnostics.json`, full parent-call audit,
source-freeze and vendor-floor identity/relocation/CodeView records. Direct
read-only span reviews add11 missing pair entries and the225-byte CRT entry;
419520 already existed. Floor's auto3113 span is corrected to64; fallback4870E4
and unrelated helpers stay outside this reviewed vendor pair. No exact/source
credit is awarded for the289-byte library identity. Bytes/decompiler/tool/build
artifacts stay private; DB unchanged. Native product/runtime/portability remain
later; goal stays active.
