# Animation ID values and complete VM publication

Release4562E0/66 and raw position456330/97 close their complete native owners with
the existing four-byte AnimationVmId value object. The three value-taking APIs
in SecondaryActorRuntime.hpp now retain that object instead of flattening it to
unsigned int. Existing callers use its normal converting constructor; no adapters,
dummy arguments, private register declarations or executable bytes are introduced.
The primary source remains /GL. Original C++ class and TU identity remain unknown.

| Owner | Full target | Current result |
| --- | ---: | --- |
| ReleaseSecondaryAnimation4562E0 |66 | Exact linked; two fields |
| PositionSecondaryAnimationRaw456330 |97 | Exact linked; two fields |
| PositionSecondaryAnimation4563A0 |132 |132/45 masked diagnostic |
| PositionSecondaryAnimationVm456430 |110 |110/45 masked diagnostic |

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
The two new owners add163 exact bytes/four fields. Normal-COFF and original /GL
ownership remain separate questions; no established LTCG owner is downgraded.

## Open offset-position owners

The ID change brings offset position to its full132-byte size, with45 masked
diagnostic differences. Direct VM position stays110/45. Native compares marker18
before its floating stores; maintained source checks it afterward. The x/y
operations remain separate double x+32+192 and y+16, with actual z and child-list
reads preserved. No passing prefix is registered.

An early bool snapshot emits SETE/test and gives115/96 direct and137/99 wrapper.
Duplicated parent-publication branches give153/71 and177/73; an early-return form
gives153/119 and181/122. A unified physical marker/position view leaves110/45 and
132/45 unchanged, so that view alone does not explain the gap. None is adopted.
These are compiler experiments, not runtime regressions or original-class evidence.

## Selected validation

Recursive current receipt inputs select40 affected old units, including the
seven-byte normal-COFF player callback through its transitive header includes.
Those and the two new units are cold-replayed; unrelated units and the full cohort
are not. All42 pass3,263 bytes/120 fields and relocations with frozen maintained
changes. The earlier movement24-unit, motion/HUD6-unit and attraction summaries
are refreshed from these current results; older versions are retained privately.
No broad behavioral or public Oracle/infrastructure suite is run because
their implementations are unchanged. Private target reviews, variants, field
audits and selected results live under `.analysis/secondary-animation-values-*`.

The changed API also changes unaccepted caller diagnostics. Fresh current cold
movement context gives2928/2447 (previous2836/2434), controller context remains
2800/2285, reset becomes2412/1951, and death stays715/423. These are complete
contributions in their recorded contexts, not new exact credits or runtime claims.
