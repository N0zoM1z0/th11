# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; outer folder
is not Git. Read AGENTS.md/current source/ledgers. Target bytes/current receipts
outrank historical notes. All dirty work belongs to this active goal.

## Checked-in state

- Original Japanese TH11 v1.00a; config/target.toml pins identity.
- 2,110 candidates;464 reviewed boundaries;354 source mappings.
- 196 exact functions/23,547 bytes:8 normal COFF and188 linked PE.
- Origins:90 authored,358 indeterminate,14 exclusions,1,648 pending.
- Direct Ghidra/scripts; no Factory MCP/REA. VC9 RTM21022/SDK6.0A.
- Commit/push authorized; prefix `gpt-6.1-sol: `.

Latest checkpoint adds11 complete mappings/2,541 target bytes, including full
projectile896/homing498 and special damage246/385. Three primary owners/317
bytes/eight fields close;19 new origin reviews are17 indeterminate plus two CRT
atan2 exclusions. One omitted ordinary atan2 candidate is added. See
[SECONDARY_PROJECTILES](SECONDARY_PROJECTILES.md),
[SECONDARY_DAMAGE](SECONDARY_DAMAGE.md),
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
- Damage1579 has full510-instruction/two-RET8 source with both256-projectile and
  32-region paths, callback/animation transitions, ordered rejection, exact float
  boundaries and wrapped nonlinear damage/score. Last measured1513/1414 before the expanded projectile header; all14 direct
  call targets/multiplicities, one callback and two returns agree independently.
- Collision244 source228/211 preserves strict/unordered threshold behavior,
  real HUD/player gates and transition432A90. Sound148 last source144/93 preserves
  double pan scale, signed short record+2,12 slots and128 cap. No byte credit.
- Timer29 and event2/event3 owners70 each are exact. Region allocator192 also
  closes via a natural local timer reference; one-past return on full32 slots
  is real. Partial actor references9C0 are projectile96C+54 target pointers.
  Position35/6, special predicate66->68/62 and dispatcher124->83/97 stay open.
  Patterns142/183 remain source-open. New complete projectile896 source908/765
  preserves the256-record lifecycle, asymmetric primary/auxiliary cleanup, culling,
  callbacks and timer. All13 calls/one callback/return agree. Homing498 source
  493/401 preserves real callback address escape/fixed ECX/EDX; all5 calls/4 returns
  agree, aligned frame stays open. No forced local/register fixes it.
- Acquisition123, wrapped angle101 and outside93 are exact. Static named angle
  constants had no public data binding; natural literals emit compiler __real
  symbols for strict proof. Capture next before the actor body/advance afterward.
  Special385 really scans five IDs, not32; embedded VM node+10 includes root and
  retains native null-fault path. Full source252/204 and385/337 stays non-exact.
- Tracked-target73 has an extra unused stack word proved by RET4; two-input
  hypothesis was wrong. Source61/68 preserves cleanup, nominal slot role/type and
  aligned frame stay open. Length47/48 and angle31 also remain non-exact. Generic
  vector/emitter callbacks get individual indeterminate reviews. Effect457 and
  FSINCOS30 remain source-open.

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
| Projectile434440 / homing434E30 | 896 /498 | 908 /765;493 /401 |
| Special406E90 /407220 | 246 /385 | 252 /204;385 /337 |
| Damage4347F0 / collision431F70 | 1579 /244 | last1513 /1414;228 /211; pre-expanded header |
| Sound44A260 / region433A90 | 148 /192 | last144 /93;192 exact |
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

`.analysis/secondary-projectile-final-proof.json` records3 new+4 affected old
cold owners across SecondaryProjectileMath/SecondaryDamageSupport. New317 bytes/
eight fields and old361 bytes/seven fields pass complete strict primary-TU
ownership/receipt/field/byte proof. Source freeze is unchanged. Initial new-owner
gate preceded ledger registration; after registering the reviewed owners, the
three new units pass on the same cold artifacts. No recompilation/full cohort.
All jobs are terminal; recompute actual receipt dependencies next time.

Private diagnostics: `build/probe-ltcg/SecondaryProjectile/final/`, complete call
audit,23-owner independent PE/CFG/return records, literal/iterator/callback variants
and vendor atan2 entire20-byte/four-field/CodeView witness. Missing486C90 gets
read-only explicit span review; both10-byte vendor owners are exclusions only.
Native sqrt dependency also has vendor-symbol evidence, but no new origin credit.
Expanded shared damage header requires the four actual old receipt refreshes,
all passed. Damage1513/1414 remains a prior-context baseline, not an unperformed
new replay. Continue large owners and open codegen, including399-byte long homing,
132/112 alternate callbacks and real-special support in future damage work.
Bytes/decompiler/tool/build artifacts stay private; DB unchanged. Native product/
runtime/port closure remains later; goal stays active.
