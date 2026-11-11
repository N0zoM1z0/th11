# Secondary player initializer, 0x42F8A0 (1557 bytes)

The original Japanese TH11 1.00a initializer occupies the complete
`0x42F8A0..0x42FEB4` extent: 1557 bytes / **348 decoded
instructions**, with two RET4 exits and 13 native calls to nine callee
targets. The sole incoming call at `0x430206` belongs to
controller construction. Direct mapped-text Ghidra review and an
independent pinned-PE instruction decode confirm there is no owned
jump table, foreign interior entry or omitted alignment beyond the
complete span. CC separates the next function at `0x42FEC0`.

The maintained C++ implementation in
`src/SecondaryPlayerInitialization.cpp` reconstructs animation file
selection (`pl00.anm` or `pl01.anm`), failure logging, pattern table
loading/ownership, two 0x24-byte update/draw chain nodes with callbacks,
VM reset/script binding, four speeds converted to fixed-point units,
selected character hitbox dimensions, bounding vectors, and three
controller timers. It reuses the established
`SecondaryPlayerControllerView` and
`SecondaryPlayerMovementView` rather than inventing a second full
controller layout. Unknown external data symbols remain physical
observations, not asserted original source names.

Only the relevant owner and direct helper context were compiled:
the fixed VC9 normal-COFF candidate is **1492 bytes with 40 relocation
fields**. A local `/GL` link of the owner with its simple caller
produces **1479 bytes, 13 calls, 2 returns**. Adding six actual
source dependency TUs (chain registration, animation document, logging,
pattern dispatch, animation VM and player callbacks) produces a
**1520-byte** linked owner but changes the call/return count to
14/3. Both remain **non-exact**; the different register specialization
and producer TU arrangement are unsolved, and no complete runtime
behavior is claimed.

Evidence: `.analysis/pause-next-big-20261111-review.txt`,
`.analysis/player-initialize-native-linear.txt`,
`.analysis/player-initialize-main-coff-20261111.json`,
`.analysis/secondary-player-initialization-linked.log`, and
`.analysis/secondary-player-init-helpers-linked.log`.
Next useful work: reconstruct the 152-byte construction caller
at `0x4301B0` and determine whether it restores native prologue and
callee-specialized register ownership, without unrelated cold replay.
