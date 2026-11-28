# Bullets (bullet.cpp)

The bullet manager (`bullet.cpp:453 BulletInf`, global 0x4A8D68, also
`g_SceneClearPool` and `g_SecondaryActorFiles`) is a 0x46D678-byte object:
chain elements at +8/+0xC, a bullet cursor at +0x10, 0x7D1 bullets of 0x910
bytes from +0x64 (the last one is a sentinel whose state stays 5) and the
`bullet.anm` document at +0x46D674. A bullet has flags and kind, its VM at +8,
timers at +0x464 and +0x478, eleven 0x34-byte pattern records from +0x680 and
a timer at +0x8F0; `SceneClearProjectileView` is an older view of it.
Source: `src/Bullet.{hpp,cpp}`, context `build/match-linked/BulletManager`
(`/EHs`, like item.cpp).

Exact (2026-11-28): the implicit bullet constructor and destructor (0x4083D0,
0x408410), `InitializeBulletManager` (0x408450, EDI this; update priority 20,
draw priority 29), `ResetBulletManager` (0x408550), `BulletInf::~BulletInf`
(0x4085F0) and `CreateBulletManager` (0x408710). All six matched on the first
build with the laser/item template.

Open: `DeleteBulletManager` (0x408800) has the same one-instruction gap as
the item, ascii and laser delete functions: native saves ESI in the prologue
and loads the global into it, VC9 here loads EAX and copies it inside the
branch; guarded and local-variable forms give the same result. The update
callback 0x408EC0 overlaps a bogus inventory row at 0x408EDE; the draw
callback 0x408EF0 draws six layers through 0x408E20 (EAX this, ECX layer).
Unreferenced copies: the manager constructor 0x408390 and small getters
0x408350..0x408380.
