# Complete scene and secondary-object clearing

Seven complete source mappings cover 1,291 target bytes. Five owners are exact
(576 bytes). `ClearSceneProjectiles` is exact only in the wide link
(`probes/wide-context.cpp`); the others use the clearing family.
`ClearSceneProjectile` became exact once 455B10 was declared as the member
`AnimationFileView::CreatePositionedVmId`: native callers push the file
before the hidden result pointer, which is the member-function order. Scene
projectiles are the2000-record pool at global4A8D68+64/stride910; these are distinct
from the player's256 records of6C bytes. Original class/TU and game-versus-engine
origins remain indeterminate.

| Owner | Address | Full target | Current result |
| --- | --- | ---: | --- |
| ClearSceneProjectile |40AE90 |242 | Exact (clearing family); five fields |
| ClearSceneProjectilesInRadius |40AF90 |334 |328 bytes, 68/93 aligned (wide link) |
| ClearSceneProjectiles |40B5C0 |99 | Exact (wide link); three fields |
| SceneClearOutside |40B8A0 |93 | Exact; four fields |
| ClearSecondaryActorsInRadius |412990 |381 |375 bytes, 92/111 aligned (wide link) |
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

Canonical artifacts are in `build/match-linked/SecondaryClearing/`.

## Horizontal-band variant (2026-11-17)

`ClearStageAuxiliaryHorizontalBand` at 40B0E0 now has complete 300-byte
natural C++ in `src/StageAuxiliaryHorizontalBand.cpp`. It shares the
scene projectile pool and real clear/drop helpers above, but selects
a horizontal X interval and a downward Y threshold rather than a
radial distance. After the 2000-record scan it calls the separate
302-byte actor-band function at 412B10. This source addition is
non-exact and does not change the shared exact clearing producers.
See [STAGE_AUXILIARY](STAGE_AUXILIARY.md).

## Actor horizontal-band clearance (2026-11-17)

`src/SecondaryActorHorizontalBand.cpp` now owns the full native
0x412B10..0x412C47 extent (312 bytes: 302 instruction bytes and 10
in-function alignment bytes). It traverses the actual
`g_SecondaryMaster->first` actor list, retains the next pointer across
side effects, checks actor state flag 0x800, and accepts only an
**open** X interval and a strict Y-below-center test. These bounds
are not the radial formula used by `ClearSecondaryActorsInRadius`.

Accepted actors lose 99999 from their native counter; flagged counters
update their scaled field and recompute the integer result with signed
division by 7 and a baseline. An unflagged counter subtracts directly.
For eligible actors, an independent viewport helper at 419920 decides
whether the standard kind-8 drop can spawn. The helper is a real
thiscall member on the three-float point, separate from
`SceneClearOutside` at 40B8A0. This owner returns void using RET12.

The complete original code and two call targets were inspected; the
natural C++ compiles under pinned VC9. Its original TU and linked
machine-code equality remain open. The adjacent viewport leaf is a
separate owner.

### Exact native actor viewport member (0x419920)

The 93-byte viewport test called by `ClearSecondaryActorsInHorizontalBand`
is now recovered as natural C++
`SecondaryActorViewportPoint::IsOutside(float, float) const`.
Its native thiscall receiver arrives in ECX, the two half-extents are
stack floats, and both returns use RET8. It rejects positions whose
expanded bounds are outside `[-192,192] x [0,448]` using the observed
ordered inequalities. The synthetic layout-compatible class view
asserts no original source class identity.

Pinned VC9 RTM normal-COFF compilation plus strict cold replay of the
**complete 93-byte owner** gives 93/93 matching bytes. All four DIR32
fields match their reviewed native double literals: -192 at 497EC8,
192 at 497EA0, 0 at 497EC0 and 448 at 497EB8. No operand masking,
padding or assembly is used. Canonical match unit:
`secondary-actor-viewport-outside`. Native TU/type attribution remains
indeterminate despite the byte-exact codegen.
