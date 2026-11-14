# Complete secondary actor instruction dispatcher

The native owner at `0x412E30` spans16,556 bytes including its dispatch data;
its executable code interval is15,780 bytes. Its complete boundary, tables,
control flow and call graph are reviewed. `src/SecondaryActorInstructions.cpp`
now provides the ordinary state-member dispatcher and manager adapter, with four
fragments covering all154 handled opcode values. The canonical VC9 `/GL` context
includes the real VM readers, actor creation/lifetime/animation/update and pair
access implementations. Its current code interval is15,272 bytes with a0x328
frame versus native15,780 bytes and0x310; the host remains non-exact. The original
decoder capacity is still a hypothesis. `RunSecondaryScript` remains non-exact.

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

## Interpreter and real-owner context

The native owner has465 direct call sites to64 targets, two indirect callbacks
and one RET. Its calls include the direct VM readers, the host-state adapters at
`0x417D80`/`0x417DD0`/`0x417DF0`/`0x417E30`, and `SecondaryFloatDestination`.
The current canonical context includes the complete host source plus the real
`SecondaryScript.cpp`, argument adapters, actor factory/lifetime/animation/update
and pair-access translation units under `/GL`. This restores the actual host-to-
factory-to-state-update-to-script-update-to-virtual-host cycle in the diagnostic
link; it does not establish the game's original TU partition or close the native
product. The interpreter remains a separate non-exact owner (see the current
measurements below).

Two targeted compiler hypotheses preceding this review are rejected. Replacing
the union-based integer pop wrapper with a typed int-pointer implementation leaves
the full Run result7,013/5,821, frame120 and both missing call sites unchanged.
Using memcpy bit transport instead gives7,381/6,120 and frame1B0, still with a
merged unary84 push and shared82/87 destination. Neither changes public source.
No broad behavior harness or runtime comparison was run.

## Motion source inside the large owner

`src/SecondaryActorInstructionMotion.inc` implements all28 values118..133,
covering16 physical primary targets and a4,517-byte union of entry intervals.
This fragment is included inside the complete state-member switch in
`src/SecondaryActorInstructions.cpp`; compiling it alone is only a diagnostic
probe, not a replacement for that complete owner.

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

`probes/secondary-host-motion-context.cpp` is an earlier isolated compile-only
probe for this fragment. Its4,368-byte contribution and89-call trace comparison
remain useful historical evidence for the28 motion opcodes, including the
12A/12B fallthrough. It is not the current complete-member build or an exact
comparison lane. The maintained fragments now cover all154 handled values and
are compiled through the actual state member in the canonical owner context.

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

The reviewed owner includes sparse alignment, every table/selector, reachable
instruction, branch and call site. Compiler experiments above remain separate
from complete-owner acceptance.

## Control and effects inside the same owner

`src/SecondaryActorInstructionControl.inc` implements all53 values140..174,
covering49 primary targets/2,663 entry-interval bytes. Combined source covers
103/154 handled values and87/134 nondefault primary targets at this stage.
The records group completes opcode coverage; all four fragments are included in
the complete state-member source mapping described below.

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

## Records, parameter packets and the remaining cases

`src/SecondaryActorInstructionRecords.inc` implements all51 values190..1C2,
covering47 primary targets/5,830 entry-interval bytes. The previous handoff's
suggested134..13F/175..18F work range was incorrect: those values use the default
entry. Independent selector replay proves that the four fragments now contain
every154 handled value exactly once and all134 nondefault primary targets.
The entry intervals total15,613 bytes;129 prologue and38 epilogue bytes account
for the remainder of the15,780-byte native code interval. This is coverage
accounting for the full member source, not a separate cropped comparison lane.

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
sets.29 static record/copy/FP/arithmetic/EH checks pass. This isolated probe is
historical; the records fragment is now part of the complete state-member source.

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

## Complete member source and recovered stack lifetimes

The following compiler-size and call-count snapshots are intermediate history.
Use the final canonical checkpoint at the end of this document for current values.

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
The earlier 12D/12F 790-versus663-byte interval predates the final pointer
staging and real pair-access TU; it is superseded by the current comparison below.
Original array capacity and specialized EH ownership remain unproved.

The public owner context keeps the actual manager-to-state member call visible.
The canonical adapter family includes the real SecondaryScript.cpp /GL
implementation so the host and its VM readers can be reviewed together.

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
open. Canonical unit: secondary-actor-instruction-adapter, with artifacts in
`build/match-linked/SecondaryActorInstructions/`.

The dispatcher and its adapter retain reviewed boundaries and indeterminate
origins. Adding the real state-member declaration changes SecondaryActorState.hpp;
recursive canonical contracts select61 existing exact units in22 compilation
families for cold replay; all61 pass4,197 bytes. Other source/tool/Oracle
implementations are unchanged.
The previous37-input freeze differs only at this header; current proof uses the
new129-input whole-owner source freeze and affected-unit results. No full cohort or broad
runtime suite is required for this declaration and owner work.

## Real argument bodies in the complete host context

The figures in this section are intermediate checkpoints, superseded by the
current full-context measurements at the end of this document.

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

The complete11-byte manager adapter passed cold one-REL32 zero-difference
comparison. Recursive source contracts selected28 affected existing exact units
in five families; all1,038 bytes passed. Helpers and the large host gained no
exact credit.

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
of original TU partition. The current creation contribution is629/native621:
a natural state-subobject reference restores native EBX spawn arguments and stack
script/RET4; constructor706/native699. Source still uses a state base where native
keeps the whole-actor base. Earlier opaque state-update support gives621
bytes but503 differing full rebased bytes. Equal extents do not prove exactness.
Current full creation rebasing differs478 bytes, versus prior625/545 with RET8;
no creator exact credit follows.
The native factory also has four other caller sites in three owners. The two
larger owners40D600/1212 and420390/1149 now have full source and a separate mixed
compiler context;418460/189 remains source-open. See
[SECONDARY_GAMEPLAY](SECONDARY_GAMEPLAY.md). Native TU/caller context is still
not proved by the partial diagnostic link.

## Current canonical host checkpoint (2026-11-14)

The `0x12D`/`0x12F` source now selects the motion record and three independent
pair-tween pointers before reading its six real arguments. It stages the three
initial/target pairs before reading duration and mode, then resets all three
pair timers. The source no longer derives all three tween addresses from one
shared `pairIndex`; that form let VC9 fold the native independent selections.

The canonical `/GL` support list now includes `src/SecondaryPairAccess.cpp`.
This matters for the native 52-byte `ResetSecondaryPairTimer` leaf at
`0x419520`: its pair pointer is passed in EAX, not on the stack. With the real
leaf definition visible to LTCG, all three `0x12D`/`0x12F` and both
`0x121`/`0x123` call sites use that no-push EAX form.
The timer leaf is independently strict-exact as part of the eight pair-access
leaves; see [pair math](SECONDARY_PAIR_MATH.md).

Current cold canonical build (`build/match-linked/SecondaryActorInstructions/`):

| Measure | Native | Candidate |
| --- | ---: | ---: |
| Full owner extent / PDB contribution | 16,556 B | 16,048 B |
| Executable code before dispatch data | 15,780 B | 15,272 B |
| Stack allocation | `0x310` | `0x328` |
| Direct calls | 465 | 459 |
| Indirect callbacks | 2 | 2 |

The candidate PDB contribution includes the same776-byte dispatch-data tail;
its code interval is508 bytes shorter than native. All154 handled selector values
are present in the member source. The separate11-byte manager adapter remains
strict-exact; a fresh selected cold replay after adding pair-access support
passed its sole REL32 field with zero byte differences. The complete host is
not exact.

Largest current entry-interval size differences (native to candidate) are:

| Opcode group | Native | Candidate | Delta |
| --- | ---: | ---: | ---: |
| `0x105` | 82 B | 196 B | +114 B |
| `0x1A7` | 136 B | 216 B | +80 B |
| `0x1AA` | 118 B | 171 B | +53 B |
| `0x101` | 133 B | 185 B | +52 B |
| `0x15E`, `0x161` (each) | 121 B | 170 B | +49 B |
| `0x12D` / `0x12F` | 790 B | 812 B | +22 B |
| `0x100` | 392 B | 268 B | -124 B |
| `0x104` | 222 B | 100 B | -122 B |
| `0x191` | 244 B | 183 B | -61 B |
| `0x195` | 74 B | 15 B | -59 B |

The `0x12D`/`0x12F` interval is212/216 decoded instructions with13 calls;
`0x121`/`0x123` is127/138 instructions with9 calls. These interval sizes are
triage data, not semantic or exactness measures. Prioritize the large positive
deltas and compare full instruction/field behavior before changing source.
The code-size shrink in `0x100`/`0x104` also needs explanation rather than being
assumed to be an improvement.

The current host still differs in opcode/register preservation, x87 sentinel
predicates, spawn/name scratch reuse, FP scheduling, pair-field store order and
stack allocation. The native frame is `0x310`; the canonical candidate remains
`0x328`. The current `RunSecondaryScript` context is6,989/7,093 bytes with its
native0x120 frame but uses VM ESI rather than native EDI; it is also non-exact.

For the next pass, keep the complete member and real `/GL` support context. Do
not use the deleted scratch aggregates, invent an ABI, force volatile accesses,
add padding, or compare a cropped owner. Build/replay only the affected canonical
unit; the complete host still has no exact acceptance.

## 2026-11-14: spawn-case tails

The per-opcode "entry interval" deltas above are partly layout artifacts:
cases that share a tail by `goto` move the tail's bytes into whichever block
VC9 lays it after. Native 0x104 (222 bytes) holds the mirrored spawn tail and
0x105 (82 bytes) jumps into it; 0x100 (392) and 0x101 (133) each keep their own
tail. Writing the four spawn cases without `goto`, each with its complete tail,
gives 0x100/0x101/0x104/0x105 = 379/92/222/200 and the owner 16160 bytes (from
16048; native 16556), with 1016 masked-aligned instructions (from 977). A
backward `goto` from 0x105 into a label after 0x104 is laid out after 0x105
again (375/131/100/196, 1002 aligned) and was not kept. Still open: making
VC9 merge 0x105's tail into 0x104 while keeping 0x101's own.
