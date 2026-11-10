# Animation ID values and complete VM publication

All four release/position owners now close their complete native extents.
Release4562E0/66 and raw position456330/97 use
the existing four-byte AnimationVmId value object. The three value-taking APIs
in SecondaryActorRuntime.hpp now retain that object instead of flattening it to
unsigned int. Existing callers use its normal converting constructor; no adapters,
dummy arguments, private register declarations or executable bytes are introduced.
The primary source remains /GL. Original C++ class and TU identity remain unknown.

| Owner | Full target | Current result |
| --- | ---: | --- |
| ReleaseSecondaryAnimation4562E0 |66 | Exact linked; two fields |
| PositionSecondaryAnimationRaw456330 |97 | Exact linked; two fields |
| PositionSecondaryAnimation4563A0 |132 | Exact linked; five fields |
| PositionSecondaryAnimationVm456430 |110 | Exact linked; three fields |

## Target observations and source contract

Fresh direct local Ghidra full mapped-text attestation/reviews confirm the existing
reviewed spans and empty foreign interior-entry sets. Raw97 includes three internal
alignment bytes absent from sparse94; release66 is contiguous. No extent changes
or original-provenance upgrades are made.

Both new exact owners load the actual ID word from entry ESP+4 and call the complete
FindAnimationVmById79 implementation. Release gates on lookup-null, marks04000000
at VM404 and propagates to related nodes when marker18 is null. Raw position gates
on lookup-null, publishes all three input words unchanged to VM3E8/3EC/3F0 and
propagates to the same related list under marker18 zero. Node payloads remain
unchecked. Neither API clears the ID or substitutes a pointer/reference input.

The ordinary value-object source naturally retains stack transport of ID and RET4;
raw position's actual vector remains in the compiler-selected private ESI register.
The old scalar ID was promoted into a register, giving the shorter nonmatching
bodies. This follows the same already-observed value transport as the real
FindAnimationVmById and variable-event APIs, without asserting the original type.
The independent renderer input in the existing probe remains necessary to keep
the lookup API's genuine renderer argument.

## Exhaustive exact proof

Each owner has exactly two independently decoded external fields: DIR32 at6 binds
renderer global4C3268, and REL32 at12 binds FindAnimationVmById4561E0. No literal
payload or other external branch is omitted. Strict cold VC9 builds require current
source/header/support/tool/producer and PE/MAP/PDB receipts, unique complete public
primary-TU code ownership, exhaustive field replay and every owned byte equal.
Those two ID-value owners add163 exact bytes/four fields. Normal-COFF and original /GL
ownership remain separate questions; no established LTCG owner is downgraded.

## Early marker capture and complete offset-position owners

The ID change first brought offset position to its full132-byte size, with45
masked diagnostic differences; direct VM position stayed110/45. Fresh direct
Ghidra full reviews confirm both complete contiguous extents, no foreign interior
entries and one return each. Native compares marker18 before floating stores,
then branches using the preserved flags after all three stores.

A natural local pointer `void *marker = bound->marker018` before publication,
followed by `if (!marker)` after it, closes both full functions. The source
retains the actual read timing and leaves the compiler to schedule CMP and x87
operations. No flags intrinsics, volatile accesses or ABI declarations are added.
The x/y operations remain separate double x+32+192 and y+16; actual z and
child-list input reads remain intact. No passing prefix is registered.

The132-byte wrapper has DIR32 fields at6/28/36/55 and REL32 at12. These bind
renderer4C3268, lookup4561E0 and the three double literals32/192/16 at
497EE8/497EA0/497EE0. The110-byte direct owner has only the three literal fields,
at8/16/35. Complete independent instruction decoding finds43/37 instructions;
all8 fields are reviewed and all three target/emitted8-byte literal payloads agree.

Earlier bool snapshot emits SETE/test and gives115/96 direct and137/99 wrapper.
Duplicated parent-publication branches give153/71 and177/73; an early-return form
gives153/119 and181/122. A unified physical marker/position view leaves110/45 and
132/45 unchanged. A pointer-to-integer snapshot gives111/95 and133/98. None of
these variants is adopted. The pointer itself preserves the useful source fact;
original class and TU identity remain unknown.

## Selected validation

The first ID-value checkpoint cold-replayed40 affected old units and two new
owners:42 proofs/3,263 bytes/120 fields and relocations. The later marker change
only touches SecondaryActorAnimation.cpp. Recursive linked/COFF contracts select
30 affected old units; those and the two new position owners pass32 cold proofs,
2,602 bytes/104 fields. Both new complete public primary-TU owners pass with
current source/header/support/tool/producer/PE/MAP/PDB receipts and zero full-byte
differences. They add242 exact bytes/eight fields.

Maintained source/probe hashes stay frozen during replay.
Unrelated units and the full cohort are not rebuilt. No broad behavioral or public
Oracle/infrastructure suite is run because their implementations are unchanged.

Current cold caller diagnostics are movement2928/2447, controller2800/2285,
reset2428/1965 (previous2412/1951), and death715/423. These are complete
contributions in their recorded contexts, not new exact credits or runtime claims.
