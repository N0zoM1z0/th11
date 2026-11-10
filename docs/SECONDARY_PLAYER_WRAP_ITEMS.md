# Horizontal wrap and item attraction

Two complete source mappings cover603 target bytes in SecondaryPlayerWrapItems.cpp.
Both full native owners are newly reviewed; origins remain indeterminate.

| Owner | Native bytes | Current result |
| --- | ---: | --- |
| UpdateSecondaryPlayerHorizontalWrap430E50 |540 | Source499/415 masked diagnostic |
| AttractSecondaryPlayerItems4245E0 |63 | Exact linked; two fields |

The540-byte wrap owner has136 instructions/seven returns/four direct calls,
one actual movement caller and no foreign interior entries. It is contiguous,
with no table or cropped padding. The63-byte attraction owner has19 instructions,
one return, no calls, one movement caller and no foreign interior entries.
Fresh direct Ghidra mapped-text attestation and independent PE flow/neighbor
decoding agree on both full extents.

## Native state and data contracts

Wrap returns immediately for state0. States below99 require released input mask9,
unblocked HUD4438, and either null host or enabled host70. Cancellation clears
state/counter then increments counter to1. States at least99 bypass this initial gate.

States1/3 wait for direction release, then enter2/4 with counter1. A different
nonzero direction clears state but retains/increments the prior counter. States2/4
allow counters through8; repressing direction3/4 with unblocked HUD and a nonnull,
enabled host starts state99/100, clears the counter, queues real sound47/48,
resets emitters and advances the counter. Host-null permits waiting but cannot
start either wrap. Counter expiration clears state/counter and advances to1.

State99 wraps at x<=-6800 by addingD000;100 wraps at x>=6800 by subtractingD000.
These inclusive thresholds differ from orbit's strict +/-6600 thresholds. A wrap
requests all eight actual emitterD4 refreshes. Counter45 ends either state after
that frame's wrap check. Other nonzero states advance once if the initial gate
permits. Wrapped32 arithmetic and real sound/reset inputs are retained. All seven
native returns belong to the reviewed complete extent.

Attraction captures item global4A8E90 and player global4A8EB4. It visits exactly
2198 records, beginning at manager+478 and stepping478. Each nonzero state equal
to1 becomes3 and receives the float at player configuration92C+8 in record+C.
Other states are untouched. The manager prefix/record view describes these affine
accesses, not original class identity or complete data ownership. Layout assertions
check the source after native review; no bound/null clamp is added.

## Compiler observations and exact proof

An explicit float temporary emits an extra stack rounding store/reload and gives
73/60 for attraction. Direct parameter publication followed by state assignment
removes that temporary. A decrementing count loop gives63/7; an increasing bounded
index loop naturally produces the native pointer ADD, count SUB and JNE, closing
all63 bytes. Native instruction scheduling interleaves the state store between
the parameter load and store. No padding or target instructions are introduced.

The real movement and existing MeshEffects sound/reset implementations form the
linked context. Complete wrap source remains499/415. VC9 folds counter-clear plus
advance into a constant1 store where native retains clear/ADD; saved registers,
sound's EDX versus native ESI input, and refresh-store order also differ. Plain
inline helpers give458/421, an ordinary inline counter alone stays499/415, and
unmarked helpers give467/430. None of those variants is adopted. Attraction remains
63/0 in all three experiments. Existing Sound/spawn experiments are not repeated.

One strict cold VC9 /GL unit proves attraction's unique complete63-byte public
primary-TU PDB contribution. Current source/header/support/tool/producer and
PE/MAP/PDB receipts agree. Independent native decoding finds exactly two DIR32
fields: offset1 binds4A8E90 and offset8 binds4A8EB4. Both replay exhaustively with
every remaining byte equal. Original TU partition and /GL ownership remain open.

The new unit alone was cold-replayed: earlier canonical inputs do not depend
on these files. Canonical artifacts are in
`build/match-linked/SecondaryPlayerWrapItems`.
