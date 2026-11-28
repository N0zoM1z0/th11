# Items (item.cpp)

The item manager (`item.cpp:60 ItemInf`, global 0x4A8E90, also
`g_SecondaryAttractionItems`) is a 0x265E70-byte object holding 0x896 items of
0x478 bytes from +0x14; update and draw chain elements sit at +8/+0xC. Each
item starts with its `StageEmbeddedVm` and has a `StageTimerView` at +0x450.
Source: `src/Item.{hpp,cpp}`, context `build/match-linked/ItemManager`.

Exact (2026-11-28): the implicit item constructor and destructor (0x423220,
0x423240, called through the vector iterators), `InitializeItemManager`
(0x423270, update priority 21, draw priority 25), `ItemInf::~ItemInf`
(0x423330), `CreateItemManager` (0x423440) and `ItemManagerUpdateCallback`
(0x424090). The destructor and the create function need `/EHs`; see
KNOWLEDGE_BASE, "item.cpp needs /EHs, not /EHsc". The manager constructor is
inlined into the create function: members first (the vector constructor
iterator), then `memset`, flag 2 and the global publication.

`ItemManagerDrawCallback` (0x4240D0) is exact too. The draw loop
`DrawItemManager` (0x423F40, 336 bytes, after the update loop's switch table)
is a frontier at 334 bytes / 85 of 89 instructions: it loads
`g_SecondaryActorFiles` into EAX where native uses EDX before computing the
sprite argument. Item fields used there: position +0x434, state +0x464, type
+0x46C; the VM's position (+0x3DC), alpha byte (+0x377) and sprite number
(`marker`, +0x39C). Items above the playfield top (y < 8 after the +16
offset) are drawn as an arrow at y 24 with alpha `distance / 32 * 255`,
written as `static_cast<unsigned char>(static_cast<int>(...))`, which VC9
compiles to an inline truncating `fistp`. Writing the position through a
`CameraVector &` and the alpha test as `distance >= 32.0f` were needed for the
native shape.

Context notes: `DrawByMode` needs `AnimationVmRendererOwner.cpp` compiled with
`/DANIMATION_VM_RENDERER_OWNER_MATCH` (a support profile) and a probe call
with a generic renderer; otherwise it stays an anchor or LTCG drops its
renderer argument.

Open: `DeleteItemManager` (0x423530) loads the global into EAX and copies it
to ESI where native loads ESI directly (frontier). The update loop is
0x423550 (2,478 bytes).
