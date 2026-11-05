# Complete secondary actor instruction dispatcher

The large host dispatcher at412E30 owns16,556 bytes, not the initial15,780-byte
code-only inventory span. Its complete boundary, tables, control flow and call
graph are reviewed. Motion, creation/animation and control/effect source fragments implement
103 handled opcode values; the complete host implementation and exact byte proof
remain open.
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
the prospective source representation; original class identity remains unknown.

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
103/154 handled values and87/134 nondefault primary targets. Remaining51 values
and full EH lifetimes are open; there is still no complete-host source mapping.

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

The dispatcher and its necessary virtual adapter now have reviewed boundaries
and indeterminate origins. Complete source mappings and exact totals are unchanged.
The prior27-unit cold proof remains current: all37 receipt source/header/support
inputs are byte-identical. Recursive canonical contracts select no existing exact
unit depending on the fragment/header/probe additions. This control checkpoint
scans86 canonical receipt contracts; all earlier motion/animation source hashes
also stay unchanged. No
existing source, tool or Oracle implementation changed, so no prior unit is
cold-replayed here.
