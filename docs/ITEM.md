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

Open: `DeleteItemManager` (0x423530, `delete g_ItemManager`) loads the global
into EAX and copies it to ESI where native loads ESI directly (frontier). The
draw callback (0x4240D0) tail-jumps to the draw loop at 0x423F40 with EAX this,
so it needs that function's source. The update loop is 0x423550 (2,478 bytes).
