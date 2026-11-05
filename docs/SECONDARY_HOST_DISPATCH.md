# Complete secondary actor instruction dispatcher

The large host dispatcher at412E30 owns16,556 bytes, not the initial15,780-byte
code-only inventory span. Its complete boundary, tables, control flow and call
graph are reviewed. `src/SecondaryActorInstructions.cpp` now supplies the complete
ordinary state-member dispatcher and manager adapter, using four source fragments
for all154 handled values. A scoped diagnostic naturally reproduces native frame/
EH and scratch-slot layout; the real VM context still changes register pressure.
Original array capacity and whole-host exact byte proof remain open.
The existing7,093-byte RunSecondaryScript reconstruction remains non-exact.

## Full extent and entry contract

The complete interval is412E30..416EDB; four CC bytes separate the next owner
at416EE0. Its parts are:

| Part | Bytes | Observation |
| --- | ---: | --- |
| Code interval |15,780 |15,776 reachable bytes plus four internal alignment bytes |
| Primary jump table |540 |135 distinct targets, including the common default return |
| Primary selector |195 |Signed opcodes100..1C2;154 handled values,41 default values |
| Table alignment |1 |Between selector and nested tables |
| Two nested jump tables |40 |Five DWORD cells each; each has four unique code targets |

Direct Ghidra's full4,001-instruction export agrees byte-for-byte with independent
PE decoding at every exported instruction. Linear decoding also accounts for the
one-byte alignment gap at415B5F and three-byte gap at41645D, giving4,003 instructions.
CFG traversal reaches every other instruction. All immediate branch destinations
and all145 jump-table cells resolve to decoded instruction starts in this owner.
The145 references that Ghidra reports as entering its code interior originate
exactly in these owned tables; there is no foreign interior reference.

The actor-manager virtual slot at494074 points to the11-byte412E20 adapter.
It adds103C to ECX and tail-transfers to412E30, agreeing with the existing physical
manager/state view. The dispatcher reads state.owner at1614, owner.current at4
and current.instruction at4, then dispatches the signed opcode. Ordinary paths
clear EAX at416BAE. Blocking153/154 paths explicitly set EAX=-1 and jump to416BB0,
bypassing that clear. Both reach the same single RET without stack arguments;
the -1 result pauses RunSecondaryScript's instruction advancement. The earlier
zero-only return description was incorrect. Ghidra's inferred
fastcall/void prototype is not the ABI evidence. Ordinary member declarations are
the maintained source representation; original class identity remains unknown.

The native prologue aligns the stack to eight bytes, allocates310, registers an
exception frame with handler48AA9E and installs security-cookie checks. These are
target observations. Source lifetimes, EH/funclet ownership, original TU partition
and linked acceptance remain open; no constructor or forced frame is invented.

## Why this changes the interpreter work

The complete owner has465 direct call sites to64 targets, two indirect callbacks
and one RET. Among its direct calls are83 ReadSecondaryIntArgument and107
ReadSecondaryFloatArgument sites. It also uses the host-state argument adapters
at417D80/417DD0/417DF0/417E30 and calls SecondaryFloatDestination once.

The current interpreter diagnostic includes the actual UpdateSecondaryScripts
caller and independent API inputs, but has no complete source for this large host owner.
Thus it omits most real argument-reader call sites. This is a concrete context
gap, not proof that adding this owner will fix register allocation or tail sharing.
Source work is reconstructing the complete host switch and its actual
argument/data contracts; a synthetic caller that imitates register choices would
not establish original LTCG context. Keep all tables in the extent and retain
the established /GL owners.

Two targeted compiler hypotheses preceding this review are rejected. Replacing
the union-based integer pop wrapper with a typed int-pointer implementation leaves
the full Run result7,013/5,821, frame120 and both missing call sites unchanged.
Using memcpy bit transport instead gives7,381/6,120 and frame1B0, still with a
merged unary84 push and shared82/87 destination. Neither changes public source.
No broad behavior harness or runtime comparison was run.

## Motion source inside the large owner

`src/SecondaryActorInstructionMotion.inc` implements all28 values118..133,
covering16 physical primary targets and a4,517-byte union of entry intervals.
This is a source fragment for inclusion inside the complete host switch. It
does not define a partial native host, replace missing instructions with a
default success return or add a complete source mapping for412E30.

The natural C++ retains local/offset position changes, vector and pair tweens,
mirrored/shortest-path angles, polar/elliptic centers and radii, bounded random
direction, tracked-actor position copying and the complete motion merge/reset.
It preserves argument-read order, mode7/8, flags snapshots across math calls,
mutable vector/pair control presets and the individual timer-duration stores.
Physical motion/tween/bounds offsets are asserted in the new helper header.

Key source details required instruction-level checks:

- Native12A/12B sets Cartesian velocity, recomposes and updates the actor, then
  falls into12C/12E. The subsequent selection rereads the captured instruction's
  opcode; both original12A/12B values therefore select the offset motion in that
  second operation. Both argument0/1 reads and the following six reads remain.
- Sentinel gates use ordered value>-999999. The FNSTSW/test/branch sequences
  reject unordered inputs. A decompiler expression using a <= fallback can be
  wrong for NaN; the source uses the reviewed ordered predicate. Mirrored-angle
  and shortest-path stages retain native float roundings and the double
  constants holding float approximations of pi, half-pi and two-pi.
- Random-direction bound branches4149EB/414A0E/414A93 include unordered inputs;
  they are negated ordered greater-than tests, not ordered <= tests. The player
  comparison and upper-Y reflection remain ordered. The inner shortest-path
  branch likewise uses greater-than/else rather than a <= substitution.

`probes/secondary-host-motion-context.cpp` is a compile-only context for this
fragment. Cold pinned VC9 /GL compilation produces its complete4,368-byte probe
contribution, including its own16 dispatch cells and28 selectors. The opcode
partition agrees with the corresponding native groups. Both graphs contain89
physical direct call sites; every opcode's reachable call multiplicities and
normalized CFG call/index trace set agree, including fallthrough12A/12B.
Literal indices and the two reused zero-register indices are checked against
their reaching definitions; no guessed private-register declaration is used.

This is static structural evidence. Field stores/FP predicates are reviewed
against target instructions; call traces alone do not establish full semantic
equivalence. The probe is not the16,556-byte native owner and is never used as a
cropped exact-comparison unit. No whole-host byte match, original inline/TU
ownership, runtime or product closure is claimed. Remaining host opcode values
and EH lifetimes still need source before the real interpreter context can be
compiled. No additional leaf-function implementation or exact credit is added.

## Creation and animation source inside the same owner

`src/SecondaryActorInstructionAnimation.inc` adds all22 values100..115. Their22
primary entry intervals cover2,603 bytes; together with motion the maintained
fragments cover50 of154 handled values and38 of134 nondefault physical targets.
These are source-coverage observations, not complete-function mappings or exact
byte credit. Control/effect source below extends this coverage further.

The new physical packet is80 bytes: position, four integer fields, flags and a
48-byte parameter block. Its variable string payload uses wrapped(length+4)/4
with signed division, followed by late raw-value/reference readers. The original
instruction supplies the string/data pointers; those readers consult the current
VM reference mask. The source keeps world-X before the first reader, world-Y
after it, parameter copying after the integer readers, and the guarded109/10A/
10B/10C/10F entries falling into their unguarded counterparts.

Projected100 selects renderer camera0 before D3DXVec3Project and subtracts the
actual double224/16 biases. The camera starts at renderer+204; its view,
projection and viewport are represented within that single physical object,
not independent globals. The static argument audit verifies native addresses
4A5900/4C34D0/4C3510/4C3550 and the corresponding candidate object/addend roles.
The3D10E scene offsets also read the same camera storage after their readers.

Animation replacement retains release/clear/create/lookup order, repeated
argument reads and AnimationVmId value-object/hidden-result transport. In103,
argument1 is read for the gate, again for creation and again for the primary
slot's saved script. In106 it is captured before release and reused. Native
10D/112/114 passes the complete32-bit default script field; the decompiler's
short casts would introduce an unsupported narrowing. Only113's event argument
is narrowed to its observed16-bit payload.

The source also keeps primary hit dimensions from VM50*40 and4C*3C, bit2 clearing
through456650, default-script/direction/cache stores, direct/offset positioned
factories and111's two separate coordinate-mode reads. Its temporary ID is looked
up locally; no unobserved actor-ID publication or null guard is added. Angle
publication111/115 and spawn-parameter tails use the observed shared temporaries
and joins; there is no partial native host or default-success implementation.

Cold VC9 /GL compilation through `probes/secondary-host-animation-context.cpp`
produces a complete2,804-byte compile-only probe, including88 bytes of dispatch
data. Every opcode's normalized CFG call/index trace set agrees with the target,
including both coordinate-mode reads and all repeated reader calls. Instruction
dataflow, not physical predecessor guesses, resolves the shared angle index1/2.

Physical call placement is still non-exact: native89 sites versus candidate88.
ReadSecondaryHostIntegerValue is11/10, ClearMissingAnimationId6/5 and
ClearSecondaryHostAnimationFlag2 is2/3. All other per-target totals agree. The
initial straightforward fragment had94 sites with the same logical trace sets;
real shared packet/angle temporaries recover most of the native joins. These
remaining differences are retained for the complete owner/real callee context;
no padding, forced ABI or diagnostic slice is used to close them.

## Evidence and status

Private evidence is .analysis/secondary-host-dispatch-boundary-audit.json,
secondary-host-dispatch-complete.txt, secondary-host-getters-review.txt and their
mapped-text-attested Ghidra logs. The audit accounts for full ownership, sparse
alignment, every table/selector, reachable instruction, branch and call site.
Private decompiler output is a hypothesis for subsequent source work only.
Current motion evidence uses secondary-host-motion-{compile,call-audit,
call-traces,predicates,dependency-audit}.json, the complete Ghidra motion/helper/vector-reset
reviews and build/probe-ltcg/SecondaryHostMotion/. Source hashes stay frozen
through the final cold compile and its static audit.
Creation/animation evidence uses secondary-host-animation-{compile,call-audit,
arm-ranges,dependency-audit}.json and its fresh mapped-text-attested complete
Ghidra caller/callee reviews, plus build/probe-ltcg/SecondaryHostAnimation/.
The compile-only probes remain separate from complete-owner acceptance.

## Control and effects inside the same owner

`src/SecondaryActorInstructionControl.inc` implements all53 values140..174,
covering49 primary targets/2,663 entry-interval bytes. Combined source covers
103/154 handled values and87/134 nondefault primary targets at this stage.
The records group below completes opcode coverage; there is still no complete-host
source mapping.

The fragment retains extent/bounds stores, ten animation-ID updates, effect
parameter indexing and mirrored ID remapping, counter/HUD gauges, actor slots,
timer resets, screen effects, clearing, spell transitions and selection/difficulty
operations. State argument adapters417D80/417DD0 remain distinct boundaries from
direct VM readers;417D90/417DE0 resolve operand0 destinations through state.owner.
The physical selection object is at4A56E0 and the signed selector at4A5744;
these addresses alone do not establish their original names or game-level role.

Important target/compiler findings:

-153/154 may return -1 as described above. All other implemented control values
  return0. The caller's pause result must survive the adapter and common epilogue.
-14E reads2,1,0, writes the selected owner's life threshold, then conditionally
  calls417AD0 with that slot, instruction+20 and the saved frame limit. The helper
  writes frameLimit/lifeScript/frameScript.155 only publishes instruction+18 as
  the frame script.173 passes an integer group to the global actor clearer;
  Ghidra's inferred unused this pointer is not added to that contract.
-156/15C/165/166/167 share a decoder: signed length at instruction+1C, bytes+20,
  unsigned-byte key77/step7, then key+=step and step+=10 each iteration. There is
  no added terminator or length clamp. After argument0, the captured instruction's
  opcode is reread for wrapped difficulty offsets. Argument2 is still evaluated
  and discarded before argument1;40C650 receives name/id/value. Counter scaling
  and the shared14D frame reset follow. The enclosing stack-array capacity is
  unproved: the compile-only context accepts a workspace pointer rather than
  inventing a local capacity, allocation or native host ABI.
-15B snapshots the integer counter after the float reader and before the last
  integer reader; x87 FIDIV keeps that integer divisor.15A squares the reader's
  ST0 value without an added float rounding.15F uses double1024 and1/2048 and
  rounds its result before destination lookup.162 wraps the product and divides
  signed by2048 with truncation toward zero.170 rounds each selected integer to
  float before subtracting VM time. These stages have separate instruction checks.
-15D/15E and160/161 preserve the real shared low/middle reader tails, the signed
  thresholds512/600/200/-200/-400 and destination-before-reader order.163/164
  retain both five-cell difficulty tables, including3/4 sharing and the unsigned
  range guard. The direct164 float reader precedes the VM destination lookup.

Cold pinned VC9 /GL compilation of `probes/secondary-host-control-context.cpp`
produces3,248 bytes, including212 primary-table and40 nested-table bytes. It
preserves49 primary targets and100 physical direct call sites, with each callee's
count equal to the corresponding native control paths. All53 acyclic call/index
skeleton sets agree. Cycle reentry is omitted from these skeletons; ten-ID loop
counts/strides and decoder byte arithmetic are checked separately. Return sets
and24 targeted target/compiler details also pass static checks. This does not
prove every predicate, full semantic equivalence or complete-owner byte equality.
There is no runtime harness or added leaf implementation/exact credit.

Private evidence uses secondary-host-control-{compile,call-audit,detail-proof,
arm-ranges,dependency-audit}.json, the attested direct Ghidra contract review and
build/probe-ltcg/SecondaryHostControl/. Frozen inputs identify the final compile.

## Records, parameter packets and the remaining cases

`src/SecondaryActorInstructionRecords.inc` implements all51 values190..1C2,
covering47 primary targets/5,830 entry-interval bytes. The previous handoff's
suggested134..13F/175..18F work range was incorrect: those values use the default
entry. Independent selector replay proves that the four fragments now contain
every154 handled value exactly once and all134 nondefault primary targets.
The entry intervals total15,613 bytes;129 prologue and38 epilogue bytes account
for the remainder of the15,780-byte native code interval. This is coverage
accounting, not a native function implementation or cropped comparison lane.

The new physical views assert complete214-byte records, eighteen24-byte pattern
entries,1E4-byte basic packets and204-byte advanced packets. Record-copy19B uses
133 DWORDs, while the four inherited-pattern cases copy108 DWORDs. The advanced
reset helper410880 clears204 bytes and initializes its2C float to8. The source
preserves variable indices, short narrowing, callback tables, radius-clear
arguments, object lookup/virtual clear/relookup, target position updates and the
score snapshot across both score callees.1C0 rereads its object ID argument after
each virtual clear and zeroes the old object's6C ID before that reread.

Target details that must survive later codegen work:

- Record positions select local+offset only for ordered offset.z greater than
  double0.89999997615814209. Otherwise they use local+world, including unordered
  values. The offset case publishes Z=0; the native also computes a temporary Z
  sum before overwriting it.1B7 uses a different predicate: offset.x>=-990 including
  unordered values sets its Z marker to1. Its x87 parity branch is not an ordinary
  ordered >= comparison. Both predicates have separate instruction checks.
-19C/1AF and19D/1B0 normalize their angle through458ED0;1AC/1B1 and1AD/1B2 do not.
  Basic packets put the two record values at1DC/1E0. Advanced19D puts them at40/44,
  while1AD puts them at44/48. This real one-word shift is retained.1AD resolves its
  destination before rereading the captured instruction's opcode for1B2; the
  other inheritance gates use the entry opcode snapshot.19D's flag4 branch
  reads argument0 as an ID override instead of looking up a destination.
-195/1A6/1A7 and1A9/1AA preserve the actual shared pair-reader tails. Three-bin
  selection uses512/-512, while five-bin selection uses600/200/-200/-600; these
  are different from the earlier scalar selection's -400 boundary.1A8 retains
  double1024 and1/2048;1AB wraps two signed products and truncates division before
  narrowing.1B3/1B4 reread difficulty after publishing the first value.
-1AE snapshots both tracked coordinates as doubles, publishes each rounded float
  delta as a real argument, then computes the angle before destination lookup.
  The 1B8 source uses the existing StageScriptMesh(17,17,0) constructor, guarded
  allocation and the real destroy boundary; no fabricated constructor or EH path
  is added.

Cold /GL `probes/secondary-host-records-context.cpp` produces5,568 bytes, including
204 dispatch-table bytes. Both target and candidate have186 direct/2 indirect
physical call sites, equal per-callee counts and all51 acyclic call/index skeleton
sets.29 static record/copy/FP/arithmetic/EH checks pass. The source remains a
fragment and its probe remains a diagnostic, not a full native mapping or proof
of every field and predicate.

Direct attested Ghidra review and independent PE decoding identify the host's
FuncInfo at49FE58: magic19930522, one unwind entry at49FE50 and no try blocks.
That entry transitions to-1 through48AA90, which deletes the saved allocation
using45FD49. Handler48AA9E checks cookies and passes49FE58 to484DAB. This establishes
the mesh new-expression's cleanup role; it does not establish original source
lifetimes, specialized EH ownership or exact acceptance.

## Earlier combined-switch diagnostics

The fragment/probe sizes above and in this section describe earlier frozen
contexts. Current source owns its decoder storage and is measured through the
real member entries below; do not treat old probe hashes as current receipts.

`probes/secondary-host-switch-context.cpp` compiles all four fragments together.
Its three diagnostic inputs are state, captured instruction and decoder workspace;
they are explicitly not the native host's ABI. The cold /GL contribution is15,776
bytes:15,000 code-interval bytes plus the same135-cell table,195 selectors, one
alignment byte and two five-cell tables. All195 handled/default selector groups,
all per-opcode acyclic call/index skeletons, projection argument roles and0/-1
return sets agree. Native464/candidate460 direct body calls and2/2 indirect calls
remain: raw integer-value reader11/10, actor creator3/2 and ID lookup6/4. The native
GS epilogue adds one more direct call to its complete465-call count.

The combined candidate has a2D4 local allocation and no native EH registration;
the target allocates310 and registers48AA9E. Decoder stack storage/capacity is
still unproved and remains an explicit workspace input. There is no invented
buffer size, padding or forced EH frame. The complete original host body and
specialized EH/cold byte acceptance remain open despite complete opcode coverage.

A second cold context includes the real StageMesh.cpp owner with /GL retained.
The switch becomes15,772 bytes and still has the same call/index skeleton sets;
EH registration is still absent. Its only constructor caller supplies17/17/0,
so LTCG specializes the constructor to213 bytes with constants and no stack
arguments. Native has six constructor call sites in five callers and a generic
389-byte constructor. This measured specialization is not evidence for a new
native constructor ABI. More real context is needed; do not encode those private
register choices in declarations or manufacture throwing helpers.

Private evidence uses secondary-host-records-{compile,call-audit,detail-proof,
arm-ranges,dependency-audit}.json; secondary-host-switch[-mesh]-{compile,call-audit}
.json; fresh mapped-text-attested record/helper/EH Ghidra exports and the matching
build/probe-ltcg/SecondaryHostRecords, SecondaryHostSwitch and
SecondaryHostSwitchMesh directories. These are static diagnostics, not runtime
or product closure.

## Complete member source and recovered stack lifetimes

The manager's ordinary virtual ExecuteInstruction returns state.ExecuteInstruction.
The state member captures owner.current.instruction itself, then executes the full
154-value switch and the real0/-1 returns. Neither entry accepts a synthetic
instruction or workspace argument. A nonvirtual member declaration adds no state
storage or vtable slot. The two complete bodies now have source mappings; original
class names and TU ownership remain separate unknowns.

The decoder uses a case-local128-byte source array as an explicit capacity
hypothesis. The target has132 bytes between its observed decoder start and GS
cookie. Cold VC9 /GL generates that same distance for the128-byte array, without
added bounds, terminator, padding or a forced frame. This establishes a compatible
physical layout, not proof of the original array type or declared bound.

Initial ordinary-member compilation has a368 allocation because the shared
80-byte spawn packet still has function-wide source lifetime. The target reuses
its294 slot for spawning and decoded names. Giving the spawn cases their real
shared lexical scope naturally reduces the allocation to310 and reproduces:

- Packet base8C, shared spawn/name base294 and post-push GS cookie318.
- Eight-byte stack alignment, dual cookie setup and EH registration at320.
- Manager/state/instruction in ECX/EBX/ESI/EDI at the entry's observed stages.
- A one-state/no-try FuncInfo and an allocation-delete cleanup for the mesh new.
- The complete11-byte manager adapter, using the ordinary member call.

This corrects the earlier inference that missing throwing/factory context must
explain the absent EH in the synthetic switch probes. The ordinary member
enclosure with real local lifetimes restores EH under the existing /EHsc /GL
profile; no throwing helper, forced exception frame or compiler-profile change
is needed. It does not establish why every earlier probe lost EH.

The scoped member diagnostic is15,552 bytes, including the complete776-byte
dispatch-data tail. It preserves all195 selector groups, all acyclic call/index
skeletons, projection roles and0/-1 results. Native464/candidate459 body calls
remain: float reader107/106, raw-int-value reader11/10, actor creator3/2 and ID
lookup6/4; indirect sites are2/2. Both full bodies also have the GS check.
Whole-owner raw comparison remains nonzero; this is not a cropped exact lane.

Differences in that scoped context include opcode/cache register selection, the
decoder's spilled byte keys and length reload, FP scheduling/temporary stores,
call sharing and the EH cleanup's saved allocation offset: candidate EBP-310
versus native EBP-2F4. Entry-interval deltas are diagnostic attribution only;
12D/12F's790 versus663 bytes and the spawn joins are substantial next targets.
Original array capacity and specialized EH ownership remain unproved.

Private whole-owner evidence uses secondary-host-owner-{scoped-compile,
scoped-receipt,scoped-call-audit,layout-proof,interval-deltas}.json and
build/probe-ltcg/SecondaryHostOwner/scoped/. The public owner context keeps the
actual manager-to-state member call visible. The new canonical adapter family
also includes the existing real SecondaryScript.cpp /GL implementation so the
large host and its real VM readers can be reviewed together.

The first canonical family kept SecondaryScript.cpp as real /GL support.
The complete host becomes15,696 bytes/320 allocation, with packets9C, scratch2A4
and cookie328. The132-byte name-to-cookie distance remains. All195 selector
groups, call/index skeletons, projection roles and return sets still agree, but
body calls are native464/candidate460: float reader107/106, raw-int-value11/10,
actor creator3/2 and vector-timer reset2/1. ID lookup6/6 is recovered. The real
reader context also recovers the native CL decoder key and cached length, so the
earlier byte-key spills/length reload are not current gaps.

In that earlier context, optimized reader entries explained an ABI difference: the
candidate integer index is in EDI versus native stack; its float index is in EDX
versus native ECX. The static audit derives these from each complete PDB-bounded
callee, rather than changing declarations to encode private registers. That
cleanup used EBP-31C versus native EBP-2F4. The7077-byte RunSecondaryScript
support contribution still uses VM ESI/frame134, not native EDI/frame120; this is
context evidence, not primary-TU acceptance or proof of interpreter improvement.

Only the complete11-byte manager adapter is accepted as exact: cold canonical
build, public primary-TU PDB owner agreement, exhaustive sole REL32 replay to the
separately reviewed state dispatcher and zero full-byte differences. This adds11
exact bytes, not16,556; the host's EH/static ownership and full byte proof stay
open. Canonical unit: secondary-actor-instruction-adapter. Private current evidence
uses secondary-host-owner-{canonical-summary,canonical-call-audit,adapter-proof,
affected-proof,checkpoint-proof,source-freeze}.json and
build/match-linked/SecondaryActorInstructions/.

The dispatcher and its adapter retain reviewed boundaries and indeterminate
origins. Adding the real state-member declaration changes SecondaryActorState.hpp;
recursive canonical contracts select61 existing exact units in22 compilation
families for cold replay; all61 pass4,197 bytes. Other source/tool/Oracle
implementations are unchanged.
The previous37-input freeze differs only at this header; current proof uses the
new129-input whole-owner source freeze and affected-unit results. No full cohort or broad
runtime suite is required for this declaration and owner work.


## Real argument bodies in the complete host context

At the argument-body checkpoint, the canonical family added SecondaryHostArguments.cpp as real /GL support,
without changing the public stdcall declarations or removing /GL. Direct Ghidra
and independent PE review cover all six complete adapters:417D80/16,417D90/61,
417DD0/14,417DE0/16,417DF0/49 and417E30/57. Their213 bytes/71 instructions have
no foreign interior references; tails and neighboring CC gaps are reviewed.
These are source dependencies of the large owner, not new exact acceptance units.

The supplied-value adapters test the current instruction mask but resolve the
supplied value, not a reread of the instruction payload. The float adapter stores
and reloads through its float32 parameter slot. Integer destination lookup treats
all negative references, including-1, as a virtual GlobalIntPointer request on the
current VM's owner; it does not pop. Float destination delegates with index0.

Native int-argument and both reference-pop paths overwrite their incoming
parameter slots. Natural C++ reuse of those parameters reproduces that storage;
integer argument underflow preserves the incoming index, reference underflow the
incoming sentinel. Float argument retains its separate union initialized to the
index bits. The reference-mask expression uses the variable shift directly:
pinned VC9/x86 emits the native hardware-masked CL shift without an extra AND31.
This is pinned x86 codegen evidence, not a portable shift-range guarantee.

With those dependencies and storage contracts, that complete host measured
15,840 bytes/frame310. Packet8C, shared spawn/name294, cookie318 and EH registration
320 recover in the canonical real-VM context. Integer argument index is now on
the stack and float argument/destination index in ECX, matching native. The
16-byte host integer adapter still swaps state/index ECX/EAX versus native
EAX/ECX. Cleanup remains EBP-310 versus native EBP-2F4. Raw int argument144/native146,
int reference98/100 and float argument179/175 still have instruction differences;
equal helper extents do not grant exact credit. Run support is7057/ESI/frame134,
not a public primary-TU acceptance unit or a closed7093-byte interpreter.

All195 selector groups, acyclic call/index skeletons, projection roles and0/-1
returns still agree. Physical body direct sites are464 native/456 candidate:
float argument107/106, integer argument83/82, supplied integer11/9, actor creator3/2,
object creator4/3 and missing-animation clear6/4. Both have two indirect sites and
one separate GS call. This audit does not prove loop counts, predicates or full
byte equality. Opcode/cache registers, FP/control scheduling and physical call
sharing remain work on the full owner. The original decoder capacity is unknown.

The complete11-byte manager adapter retains strict cold one-REL32 zero-difference
proof. No new exact count is claimed for the helpers or the large host. Recursive
source contracts select28 affected existing exact units in five families for this
change; all pass1,038 bytes cold. This replaces neither the prior61-unit header
proof nor its historical source freeze. Current private evidence uses
secondary-host-arguments-{boundary-proof,call-audit,current-functions,adapter-proof,
affected-proof,source-freeze}.json and the canonical linked family artifacts.

Creation-context review also confirms4111C0 receives the script name: the saved
master pointer at4108FE is a local, while41090E reads the incoming script argument.
The default Ghidra creator body still reports545 code bytes; its already reviewed
public621-byte boundary includes3 alignment bytes,20 table bytes and53 selectors.
Use that complete ownership for all creation diagnostics and future acceptance.


## Complete creation source and six-vector staging

SecondaryActorCreation.cpp now implements the complete621-byte4108F0 factory,
using the already reviewed physical spawn argument layout. It retains2678-byte
allocation and ordinary manager construction, local position and payload copies,
difficulty byte, masked flags, lazy timer initialization with current/fraction2
and previous1, the full4230-byte state-update call, effect remapping, sparse
animation-script selection and both intrusive-list publication paths. The master
pointer is captured before allocation and reused after state update. The native
allocation-failure path is not given an invented guard. Full selector replay
checks all53 bytes and five table cells, including0/49,5/50,10/51 and15/52.

The canonical host family now also supplies the existing real actor lifetime,
animation and state-update implementations under /GL. This closes the observed
host -> creation -> state update -> script update/interpreter -> virtual host
call cycle inside the diagnostic link. It is not native product closure or proof
of original TU partition. The current creation contribution is625/native621:
source stack arguments/RET8 versus native EBX spawn arguments, stack script and
RET4; constructor706/native699. Earlier opaque state-update support gives621
bytes but503 differing full rebased bytes. Equal extents do not prove exactness.
Current full creation rebasing differs545 bytes; no creator exact credit follows.
The native factory also has four other caller sites in three owners, including
unreconstructed40D600/1212 and420390/1149; native caller context remains incomplete.

The12D/12F branch previously constructed each initial pair before selecting its
target, with aggregate X/Y selection order and one common tween index. Native
selects motion plus three tween pointers before argument reads. After six rounded
real reads, each family selects target Y then X, stages its target pair and then
captures its initial pair; only then are duration/mode read and three tween resets
called. The maintained source now expresses those pointer selections and float
stages directly. Its69 x87 instructions match native operation order, operand
widths and register forms after erasing memory addresses; the previous source
has57. This comparison deliberately does not prove memory aliases, stack slots,
predicates, original vector classes or full bytes. No constructor, volatile,
forced ABI or padding is introduced to obtain it.

The complete current host is15,904 bytes/frame328, not exact. PacketsA4, shared
spawn/name2AC, cookie330 and EH registration338 differ from native8C/294/318/320;
cleanup is EBP-328 versus native EBP-2F4. The128-byte decoder capacity remains a
hypothesis; its132-byte cookie distance persists. The earlier310 frame was
compatible with the older floating storage representation, not proof of final
source correctness. Do not shrink or pad storage to preserve that earlier result.
The12D/12F entry interval is757/native790, a diagnostic attribution only. VC9
consolidates the four native pointer-selection comparisons into one; scalar
preset reloads, register allocation and stack storage remain concrete gaps.
Separate if statements compile identically to the ternaries; two manually shared
working floats worsen the frame to330 and are not retained. A private reordering
of all154 cases by native physical entry also leaves the excess sharing unresolved.

All195 selector groups, acyclic call/index skeletons, projection roles and return
sets continue to agree. Physical body direct sites are464/455: float107/106,
integer83/82, supplied integer11/9, actor creator3/2, object creator4/3,
missing-animation clear6/4 and emitter creation6/5. Indirect2/2 and GS1/1 remain.
These structural checks do not establish complete semantics or byte equality.

Real state-update support restores RunSecondaryScript's fixed120 frame in the
current family: complete6989/native7093, VM ESI versus EDI. All89 selectors,
physical case groups and both RET4 paths agree. Native32/source31 push sites,
unary84's merged paths and82/87's shared float destination remain. The4083-byte
state-update support contribution is still non-exact against4230. Neither support
contribution has primary-TU acceptance in this family.

Only secondary-actor-instruction-adapter is affected among existing exact units;
its complete11 bytes/one REL32 still pass a fresh cold family replay. The other
sources, headers, tools and Oracles are unchanged. Source mappings rise by one;
exact totals do not change. The current132-input union freeze changes the motion
fragment and adds creation/state-update inputs; prior freezes remain historical.
Private evidence uses secondary-host-pairs-{canonical-functions,canonical-call-audit,
detail-proof,creation-proof,interpreter-diagnostic,adapter-proof,source-freeze}.json,
secondary-interpreter-tails-host-pairs-cfg.json and the canonical linked family.
