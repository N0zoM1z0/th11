# Secondary actor construction, destruction and pair interpolation

Twelve new complete source mappings cover2,505 target bytes, including actor
construction699, EH-bearing destruction384, position update309 and pair
interpolation673. Six complete public primary-TU contributions pass327 exact
bytes and six independently reviewed address fields. Eighteen new boundary and
origin reviews remain indeterminate. Totals at this actor checkpoint were2,097 candidates,
403 reviewed boundaries,303 mappings and174 exact functions/22,445 bytes.
Origins are90 authored,301 indeterminate,10 exclusions and1,696 pending.

Later full4230-byte state source, the VM pointer-return correction and current
proof/gaps are recorded in [SECONDARY_ACTOR_STATE](SECONDARY_ACTOR_STATE.md).

| Owner | Address | Full target bytes | Current result |
| --- | --- | ---: | --- |
| SecondaryActorStateConstructor | 410440 | 126 | Exact;1 field |
| SecondaryActorConstructor | 4111C0 | 699 | Source706/442 diagnostic differences |
| CreateSecondaryHostActor | 4108F0 | 621 | Complete source629; EBX packet/RET4 restored; full bytes open |
| SecondaryActorDestructor | 411480 | 384 | Source390/327; EH acceptance open |
| SecondaryActorDeletingDestructor | 410420 | 31 | Generated30/20; aliases/ABI open |
| DetachSecondaryActor | 410B60 | 82 | Source79/74 |
| UpdateSecondaryActorPosition | 411610 | 309 | Source310/269 |
| TrackSecondaryActor | 410660 | 21 | Exact;1 field |
| SecondaryActorReferenceStatus | 410690 | 32 | Exact;1 field |
| ReleaseSecondaryAuxiliaryThreads | 40FE80 | 45 | Exact;2 fields |
| ResetSecondaryMainThread | 40FED0 | 64 | Exact;0 fields |
| StepSecondaryPairTween | 4191F0 | 673 | Source673/352 |
| SecondaryPairTweenFactor | 419580 | 39 | Exact;1 field |

## Real member construction and complete actor lifetime

The actor is base103C plus appended state163C, total2678. The state view retains
unknown storage and asserts observed offsets; it does not recover original class
names. Four52-byte motion records precede ten observed animation-ID words.
Frame timer154, intrusive node168, vector tweens174/1C0 and six pair tweens20C
through338 explain the actual timer flag offsets164/1B4/200/23C/278/2B4/2F0/32C/368.
Eight214-byte records begin374 and end1414; timers1558/156C explain the final two
flag clears at1568/157C. The script owner is1614 and mesh pointer1618.

The former reset-only model of410440 was incomplete. An ordinary out-of-line
state constructor with an empty body naturally calls eleven timer constructors
and eight record constructors. Each record clears its214-byte observed storage
and sets208 to-1. All126 bytes, including the loop and receiver return, agree.
No manual vptr assignment, instruction bytes or padding appears in source.

Actor construction then clears the entire state, including those member defaults,
and explicitly initializes VM/current/head, owner, eight tween durations, motion,
node, timers and script lookup. Four native floating defaults at497F88 are24,
not1; the two values at497EF0 are32. Three counter words are cleared separately;
eight slot pairs receive-1 before their value words are cleared. Index158C is-1.
The master view preserves indexed storage beginning1C without inventing a slot
capacity or treating overlapping animation-file storage as actor pointers.

The first opaque-support constructor emitted766 bytes with extra EH. Including
the actual SecondaryScript source removes that EH naturally and gives706/442
against699. Separating VM endpoint assignments leaves706/442; moving their
initialization into a stack constructor gives704/493 and is not retained. Common
headers are unchanged. Current gaps include base/main address reuse, store and
register scheduling, and the effect-record address; no ABI/alignment workaround
is adopted.

Natural actor destruction unlinks the node, optionally clears the indexed master
reference, searches both renderer lists for ten IDs, marks found VMs/related
nodes, clears IDs, clears a tracked reference and256 physical records, releases
mesh bindings, deletes the mesh and invokes base thread destruction. The mesh
cleanup is the existing complete231-byte ReleaseVmBindingBlock at402300; it is
not a guessed new mesh routine. Full native384 includes real GS/EH registration
and inlined base cleanup; source390 remains non-exact. Native receiver placement,
base inlining, associated unwind8/handler27 and scalar/vector deleting aliases
remain open. Main-body similarity grants no EH exact credit.

Position update preserves rounded vector sums/differences, the actual motion
helper459590, double0.5 extent arithmetic, ordered boundary comparisons and
recomputed local position. Its full310-byte candidate is compared against the
entire309-byte target. Motion composition itself remains source-open.

## Pair interpolation and thread contracts

The complete673-byte pair interpolation has timer completion, mode7 incremental
addition, mode17 acceleration, mode8 Hermite and generic easing branches. Source
retains float intermediates, double Hermite coefficient arithmetic, timer flag
initialization, wrapped previous-frame subtraction and output publication order.
The target writes an eight-byte result buffer and returns its address. Source
models that physical buffer contract directly; the original vector class's
nominal return ABI remains unknown. The initial POD-by-value model incorrectly
returned in EDX:EAX. No invented vector constructor forces a hidden result.
The complete current673/352 candidate remains non-exact. Native product trees,
initial snapshots and publication/terminal order are now preserved; see
[SECONDARY_PAIR_MATH](SECONDARY_PAIR_MATH.md) for the current probes and CRT floor identity.

Factor39 divides fraction by duration, rounds to float, dispatches only unsigned
mode-1<=15 to the real easing function and returns float. Opaque easing support
adds a stack argument and emits40 bytes; the actual StageEasing source naturally
recovers native EAX dispatch and all39 bytes. Supported timer/easing/vector
owners gain no additional exact credit in this family.

Auxiliary release45 snapshots each next pointer before deleting the VM and node,
leaving the head untouched. Separate reset64 clears main flags bit0, time,
instruction and extra word, sets owner/id/current and reconstructs the head links.
It preserves stack cursors and program. Adding a head clear to release or cursor
clear to reset would change native behavior. Tracking21 checks only byte7C98
before publishing7C94. Status32 tests8BB4 and8BC4 bit1; neither adds a null guard
or claims an original player type.

## Wider origin and full-boundary review

Twenty-four complete owners were independently decoded, including six earlier
reviews and18 new reviews. New work covers pair673/factor39, transition561,
factory621, effect156, motion75/215, rotation86, quantization99, ID validation30/21,
animation position132/script switch36, global predicate28, tracking/status and
thread release/reset. Incoming references, returns, branches, actual neighbors
and tables were checked. Engine callers and generic math patterns do not
independently prove original authorship; all18 remain indeterminate.

Factory4108F0 owns621, not the automatic545-byte code extent. After545 code bytes
are3 native alignment bytes, a five-cell20-byte table and53 selector bytes.
Every table value targets a real arm and every selector is0..4. The five Ghidra
DATA references are internal to the complete owner; three CC bytes separate
410B5D..5F from unlink410B60. The full factory allocates2678, calls699 construction,
copies spawn payload, sets difficulty/timers/flags, runs state update4230 and
publishes the intrusive node. Allocation failure is not given an invented guard.
That initial review added no source or exact credit. A linear review_span attempt over
its mixed code/data failed; acceptance uses the successful attested code/refs
review plus independent complete PE/table/selector analysis, not that failed dump.

Transition561 includes both life-threshold and timed-slot selection, UI
seconds/hundredths calculation, timer resets and global state gates. Its role is
larger than a subroutine-name lookup. Motion215 retains polar/elliptic branches
and quantization;99-byte quantization's64-byte frame and math-helper identity
remain observations. Similar ID validators30/21 have different return contracts.
These source-open dependencies guide the full4230-byte actor-state implementation;
that owner is not replaced by a partial prefix or simplified update loop.

## Strict selected cold proof

Independent target fields are state constructor REL32 at80 to memset46A720;
tracking/status DIR32 at1 to reference global4A8EB4; auxiliary release REL32 at23
and29 to operator delete45FD49; factor REL32 at27 to easing459730. Reset has none.
The six complete contributions have unique public primary-TU ownership and pass
current source/header/support/tool/producer/PE/MAP/PDB receipts, exhaustive field
replay and every byte. Source was frozen before cold building three contexts.

The six new units passed affected cold replay.
Actual prior receipt dependencies intersect no changed source/header; no old
unit or full cohort was replayed. The first comparison ran before new source
mapping registration and correctly rejected the missing reviewed owner. After
registration, those unchanged cold artifacts/current receipts passed all six
comparisons; no redundant rebuild was needed. Build/Oracle tools are unchanged,
so no broad runtime or infrastructure suite ran.

## Complete factory source in the large host context

CreateSecondaryHostActor now has complete maintained source in
SecondaryActorCreation.cpp. The full621-byte boundary retains545 code bytes,
3 native alignment bytes,20 table bytes and53 selectors. Source keeps the captured
master, ordinary new2678/manager construction, spawn payload/difficulty/timer/flag
setup,4230-byte state update, effect remapping, render/script selection and both
intrusive-node publication paths. It adds no allocation-failure guard. Fresh
attested direct Ghidra code/constructor review and independent full PE/selector
replay agree with the existing reviewed boundary.

The canonical host family supplies real creation, lifetime, animation, state
update and script implementations under /GL. A natural state-subobject reference
now restores EBX spawn arguments and stack script/RET4. Complete source629/native621
still differs478 replayed bytes, versus prior625/545 with stack arguments/RET8.
Source retains a state base where native uses the whole-actor base. Constructor
706/native699 and full bytes remain open; no private ABI declaration is used.
An earlier equal621 extent still differs503 bytes. These are complete diagnostics,
not primary-TU acceptance, and add no creator exact credit. Both larger callers
40D600/1212 and420390/1149 have complete source;418460/189 remains source-open.
See [SECONDARY_HOST_DISPATCH](SECONDARY_HOST_DISPATCH.md) for whole-host evidence
and [SECONDARY_GAMEPLAY](SECONDARY_GAMEPLAY.md) for the real caller/helper context.

## Complete 156-byte actor effect finishing owner (2026-11-12)

`src/SecondaryActorEffectFinish.cpp` now implements the complete native
`FinishSecondaryActorEffect` at `0x4127E0..0x41287B` in ordinary C++.
The target is **156 bytes, 46 instructions, four call sites and one RET**.
The caller's actor has the full `SecondaryActorStateView` at +0x103C.
Its settings array at actor offsets **+0x2580/+0x2584/+0x2588** controls
independent sound, script and animation-file indices. When the sound index
is nonnegative, the original calls `QueueSecondaryActorSound` at `0x44A260`
with the horizontal world position; a nonnegative script index calls the
native `CreatePositionedAnimationVmId` (`0x455B10`) with layer 3 and the
animation file fetched from master `+0x40 + 4*fileIndex` (not a fixed file).

It then checks the ID at actor+0x2544; a positive ID triggers the real
`SpawnSecondaryPlayerDrop` (`0x424230`) with the original opaque color,
angle and speed. Independently of that ID, it calls the full **457-byte**
`EmitSecondaryActorEffects` (`0x4101D0`) with the same state record and
world position; it always clears that ID and returns 1. This deliberately
does not dispatch through the separate 58-byte `ResetSecondaryStateWithEffect`
entry, because the native caller inlines its own gate.

Four pinned VC9 RTM `/GL` primary-TU PE/MAP/PDB experiments retain **all
four calls**: an independent live argument caller yields **150/156 bytes**,
real emission producer **146/156**, real audio/positioned animation producers
**148/156**, and their combined context **147/156**. A second bounded family
uses the genuine **4230-byte** `SecondaryActorUpdate.cpp` caller and yields
**150/156**, **146/156** and **151/156** depending on available direct
producer TUs. The original 46-instruction scheduling and native private
register/hidden-result arrangement are still non-exact; **no strict match
credit** is claimed. Reproduce with
`.analysis/probe-secondary-actor-effect-finish-20261112.py` and
`.analysis/probe-secondary-actor-effect-finish-real-parent-20261112.py`
plus their logs; public producers under
`probes/secondary-actor-effect-finish*-context.cpp`. No unrelated full cold
replay was performed.

## Full 36-byte secondary animation script switch (2026-11-12)

`src/SecondaryAnimationScriptSwitch.cpp` now owns the complete natural source
of `ChangeSecondaryAnimationScript` at `0x456770..0x456793`. The original
Japanese PE has **36 bytes, 10 instructions, two calls, one RET4** and no
foreign interior entry. Both original callsites (`0x411A22`, `0x411A92`)
belong to the already source-backed **4230-byte**
`UpdateSecondaryActorState` boss. The small owner reads the animation ID
through its pointer, calls the existing independently exact
`FindAnimationVmById` (`0x4561E0`), then, only when found, calls the real
animation file/script binder (`0x44AE60`) with the VM's file pointer at
+0x3B0. No substitute VM handling or null recovery has been invented.

Four bounded VC9 `/GL` owner contexts compile the complete source at
40/36 bytes isolated, 33/36 with real lookup, 37/36 with real binder and
**30/36** with both, retaining the original **two calls and RET4**. Repeating
all four with the real 4230-byte parent yields the same respective output.
The native private interface moves the animation ID pointer in EAX and the
script on the stack, then prepares ECX script / EDX file for the binder;
our public signatures still differ at this optimization boundary.
This is maintained complete source, **not an exact function**. Reproduction:
`.analysis/probe-secondary-animation-script-switch-20261112.py` and
`.analysis/probe-secondary-animation-switch-real-parent-20261112.py` with
their logs. The previously exact VM lookup source was not changed.
