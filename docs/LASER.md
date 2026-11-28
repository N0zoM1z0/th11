# Lasers (laser.cpp)

The target's RTTI names the laser classes: base `LaserDataInf` (vtable
0x494A0C) and derived `LaserLineInf` (0x494A64) and `LaserInfiniteInf`
(0x494ABC), each with 20 slots. The `.rdata` tags `laser.cpp:115 LaserInf`,
`laser.cpp:235 LaserLineInf` and `laser.cpp:244 LaserInfiniteInf` sit just
before the vtables. The code lies in 0x4246E0..about 0x428600, after the item
code (`SpawnSecondaryPlayerDrop`, 0x424230) and before loading.cpp.
Source: `src/Laser.hpp`, `src/Laser.cpp`.

## Exact

All 20 `LaserDataInf` default slots (0x4246E0..0x424820) and the two derived
slot-4 overrides (0x426220, 0x427720) are canonical exact as plain
`__thiscall` normal-COFF units, `laser-*`. Each body is followed by int3
padding to the next aligned entry and is referenced only from the three
vtables (`.analysis/claude-laser/extent-review.txt`). Code order of the
defaults is slots 0-4, 7, 6, 8, 5, 9, 10-19. Cold replay:
`.analysis/claude-laser/replay.log`.

`MarkSecondaryHostEffects` (0x424FB0, ECL opcode 0x16D) is also exact
(`laser-mark-for-clear`), through the new `LaserInf` model and
`g_LaserManager`, which shares 0x4A8E94 with the older
`g_SecondaryClearManager` view. Its neighbour in item.cpp,
`AddSecondaryPlayerValue` (0x424640, `src/SecondaryPlayerValue.cpp`), adds to
game-state +0x10 with the maximum at +0x8C (`g_SecondaryPlayerValueMaximum`,
0x4A576C). Replay: `.analysis/claude-laser/replay-2.log`.

The manager lifetime is exact in one linked `/GL` context
(`build/match-linked/LaserManager`, probe `probes/laser-manager-context.cpp`
with the real chain, document, log and chain-removal sources):
`LaserDataInf::LaserDataInf` (0x424830, ESI this), `InitializeLaserManager`
(0x424970), `LaserInf::~LaserInf` (0x424A70, EBX this) and
`CreateLaserManager` (0x424B60). 0x424A70 is the manager destructor: the game
teardown at 0x420055 is an inlined `delete g_LaserManager`. `LaserDataInf` has
no destructor, which is why its vtable has no deleting-destructor slot and the
release loop calls `operator delete` directly. The chain helpers are the same
as `SecondaryAuxiliaryOwner.cpp`'s, whose initializer at 0x40E0A0 has the same
shape and is still non-exact. Replay: `.analysis/claude-laser/replay-3.log`.

## Slots

| Slot | Offset | Name | Base | LaserLine | LaserInfinite | Evidence |
| ---: | ---: | --- | --- | --- | --- | --- |
| 0 | 0x00 | Unknown00 | ret | 0x425370 | base | |
| 1 | 0x04 | Initialize(request) | return 0 | 0x4250B0 | 0x4270F0 | called with the request after allocation in 0x424DF0 |
| 2 | 0x08 | Update | return 0 | 0x425CC0 | 0x4272C0 | nonzero result deletes the laser (0x424C10) |
| 3 | 0x0C | Draw | return 0 | 0x426160 | 0x427660 | draws the VMs at +0x624/+0xA58 (line) |
| 4 | 0x10 | Unknown10 | unlink, return 0 | return 0 | return 0 | called before deletion |
| 5 | 0x14 | Clear(mode, flags) | return 0 | 0x426DF0 | 0x428270 | `ClearSecondaryObjects` |
| 6 | 0x18 | Unknown18(4 args) | return 0 | 0x426230 | 0x427730 | |
| 7 | 0x1C | ClearInRadius(position, radius, mode, flags) | return 0 | 0x4267E0 | 0x427C00 | `ClearSecondaryObjectsInRadius` |
| 8 | 0x20 | Unknown20(4 args) | return 0 | base | base | |
| 9 | 0x24 | Unknown24(2 args) | return 0 | 0x426FD0 | 0x4284F0 | |
| 10-19 | 0x28-0x4C | Unknown28..Unknown4c | ret | 11: 0x425BD0, 13: 0x425AD0, 16: 0x425850; others base | base | |

## Manager

`g_SecondaryClearManager` (0x4A8E94) is the laser manager, a 0x47C-byte
object. Its embedded `LaserDataInf` at +0x10 is the list head, so the first
laser is at +0x18 (`head.next`). Further fields: +0x450 tail, +0x454 count
(limit 0x100), +0x458 next ID (starts at 0x10000), +0x45C clear position,
+0x474 the `bullet.anm` animation document (slot 6; on failure it logs
"敵弾データが見つかりません。データが壊れています").

| Address | Role | ABI |
| --- | --- | --- |
| 0x424830 | `LaserDataInf::LaserDataInf`: vtable, timer flag clears, `memset(this, 0, 0x440)`, `timer.SetFrame(-1)`; exact | LTCG ESI this |
| 0x4248B0 | Unreferenced: copies 0x1E4 bytes (0x79 dwords) from EDX to EAX | EAX/EDX |
| 0x4248C0 | Unreferenced: append ECX to the manager (EAX) list tail, count++ | EAX manager, ECX laser |
| 0x4248E0 | Unreferenced: count--, unlink EAX, move the tail back if it was the tail | ECX manager, EAX laser |
| 0x424910, 0x424920 | Unreferenced: cursor at +0x478 begin (first laser) and next | no arguments |
| 0x424940 | Unreferenced out-of-line `LaserInf` constructor (inlined in 0x424B60) | LTCG EDI this |
| 0x424970 | `InitializeLaserManager`: animation document 6, update chain 19 (callback 0x424D70), draw chain 27 (0x424DD0), tail = head; exact | stdcall manager |
| 0x424A70 | `LaserInf::~LaserInf`: unregister both chain elements, delete every laser after slot 4, clear the global; exact | LTCG EBX this |
| 0x424B60 | `CreateLaserManager`: new 0x47C, inlined constructor, initialize, delete on failure; exact | cdecl |
| 0x424BD0 | Unreferenced: destructor then `operator delete`, returns the pointer | LTCG EAX this |
| 0x424BF0 | Unreferenced: `delete g_LaserManager` | no arguments |
| 0x424C10 | Update loop: a set clear byte +0x68 deletes the laser one frame later; otherwise Update, then advance timer +0x14 | LTCG EDI manager |
| 0x424D70 | Update callback: returns early on flags 1, 4 or 0x400 of `[0x4A8E88]+0x60`; under flag 2 runs with the time scale set to zero | ECX chain element |
| 0x424DF0 | Spawn: kind 0 new `LaserLineInf` (0xE8C), kind 1 new `LaserInfiniteInf` (0xEB0), link at tail, Initialize(request) | LTCG EDI request, stack kind; this is `CreateSecondaryHostObject` |
| 0x424EA0 | `LaserLineInf` constructor: base, vtable, `memset` +0x440 (0x1E4), VMs +0x624/+0xA58 | LTCG EAX this |
| 0x424EE0 | `LaserInfiniteInf` constructor: base, vtable, `memset` +0x440 (0x204), float +0x46C, VMs +0x648/+0xA7C | LTCG EAX this |
| 0x424FB0 | `MarkSecondaryHostEffects`: mark every live laser for clearing (+0x68 = 1); exact | no arguments |

Object layout so far: +0 vptr, +4 previous, +8 next, +0xC state (1 is
removed), 0x14-byte timers at +0x14 and +0x28, the clear byte +0x68 and
+0x69, ID +0x6C, eighteen 0x34-byte pattern records from +0x70 and a timer at
+0x424. Sizes: base 0x440, line 0xE8C, infinite 0xEB0.

The functions marked unreferenced are absent from `config/functions.csv`
(0x424BD0 is listed); see KNOWLEDGE_BASE, "Unreferenced functions survive".

## Next

The unreferenced helpers above need ledger rows before they can be claimed;
the 0x4248C0/0x4248E0 bodies show how the spawn (0x424DF0) and update loop
(0x424C10) inline their list operations. The update loop inlines the timer
advance (`AdvanceStageTimer`'s body).
The derived slot bodies (181-1,644 bytes) are the bulk of the TU. TH10's
`EnemyLaser*` sources are a structural reference only.
