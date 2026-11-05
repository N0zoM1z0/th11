# Complete scene and secondary-object clearing

Seven complete source mappings cover1,291 target bytes. Three complete public
primary-TU owners pass235 exact bytes/six independently reviewed fields. Scene
projectiles are the2000-record pool at global4A8D68+64/stride910; these are distinct
from the player's256 records of6C bytes. Original class/TU and game-versus-engine
origins remain indeterminate.

| Owner | Address | Full target | Current result |
| --- | --- | ---: | --- |
| ClearSceneProjectile |40AE90 |242 |241/111 masked diagnostic |
| ClearSceneProjectilesInRadius |40AF90 |334 |342/218 |
| ClearSceneProjectiles |40B5C0 |99 |97/81 |
| SceneClearOutside |40B8A0 |93 | Exact; four fields |
| ClearSecondaryActorsInRadius |412990 |381 |378/70 |
| ClearSecondaryObjectsInRadius |424FE0 |95 | Exact; one field |
| ClearSecondaryObjects |425040 |47 | Exact; one field |

Direct local mapped-text-attested Ghidra full reviews and independent complete
PE decoding establish all seven flows, references, neighbors and returns. Three
new reviews cover individual242/57 instructions, rectangle93/31 and actor381/111;
all have empty foreign-interior sets. Existing four boundary/origin decisions
remain unchanged. The47-byte traversal retains its three internal alignment bytes.

The scene record has flags0/kind4, VM8, position43C, width45C, timer464,
clear script4A4 and state4B2. Individual clear accepts states1/2, tests viewport8,
sets event1/state3, optionally creates a positioned VM at layer16 and resets the
timer. Outside records receive flag8. Allocation results receive no new guard.

Spell phases9E..A1 under active flag1 make whole-pool clear return early, whereas
radius clear rounds radius/3 to float and continues. The radius loop preserves
float dx/dy, rounded width/2+radius and float sum-of-squares. Its ordered greater
rejection leaves unordered comparisons on the clear path. The actor radius helper
instead accepts only ordered less/equal after radius+16, subtracts99999 through
the actual wrapped/scaled counter, and preserves the separate viewport2/drop gate.
Both use the real kind8 drop with colorFFFFFFFF/angle-1.5707964f/speed0.6f.

The secondary-object manager at4A8E94 has first18 and copied position45C/460/464.
Objects have next8/stateC. Both loops snapshot next before calling through the
actual virtual slot14 or1C and skip state1. Radius results accumulate with wrapped
32-bit addition; whole-list clear returns1. The abstract physical interface only
calls these reviewed slots; uncalled slots carry no original signature claim and
no fake method bodies are implemented.

A single combined bounds predicate recovers the native93-byte rectangle body;
four separate early returns give93/55 and are not adopted. Reordering the natural
pool iteration to pointer increment then countdown and retaining explicit actor
next locals gives current342/218 and378/70, with the native RET12. Every owner
agrees on all direct/virtual call multiplicities and return cleanup. Remaining
non-exact gaps include frame/save scheduling, allocator input registers and the
positioned factory's file/hidden-result stack order. No ABI workaround is used.

Strict cold VC9 /O2 /Ob1 /GL proof binds source/header/support/tool/producer and
PE/MAP/PDB receipts, complete unique public primary-TU ownership and all bytes.
Rectangle DIR32 fields14/29/51/67 bind doubles-192/192/0/448 at
497EC8/497EA0/497EC0/497EB8; all target/emitted8-byte payloads agree. The object
owners each have one DIR32 at1 binding4A8E94. Dynamic virtual calls add no absolute
field. All three owners pass235 bytes/six fields. Recursive input contracts select
zero affected prior units: only new source/header/probe files are introduced.
No full cohort, broad runtime or unchanged-infrastructure suite is run.

Private reviews, variants, source freeze, final diagnostics and cold proof use
.analysis/secondary-clearing-*; canonical artifacts use
build/match-linked/SecondaryClearing/. Native product/runtime/port closure remains
open. Further work prioritizes large, difficult owners over accumulating leaves.
