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
shape and is now exact as well. Replay: `.analysis/claude-laser/replay-3.log`.

The per-frame code is exact in the same context: `LaserInf::Update`
(0x424C10, EDI this), `LaserInf::Draw` (0x424D40, EAX this) and the two chain
callbacks `LaserManagerUpdateCallback` (0x424D70) and
`LaserManagerDrawCallback` (0x424DD0). The last three had no ledger row. Two
source shapes mattered:

- In the update loop the three removal paths (clear byte reaches 2, state 1,
  Update slot nonzero) share one `delete laser` after an if/else chain. Three
  separate `...; delete laser; continue;` blocks give the right size but a
  different tail merge for the state-1 path.
- The update callback tests `GameInf` flag bits 0 and 2 with a bitwise `|`
  of two one-bit members (`flag0 | flag2`), which VC9 emits as
  `(word | word >> 2) & 1`; `||` on the same members, or masks, give
  `test al, 5`. `src/GameInf.hpp` models the word as bit fields.

The timer advance in the update loop is `LaserTimer::Tick`, the inline form
of `AdvanceStageTimer`. Replay: `.analysis/claude-laser/replay-4.log`.

Both derived constructors are exact (0x424EA0, 0x424EE0; EAX this). Their
layout: a parameter block whose own constructor clears it (line 0x1E4 bytes;
infinite 0x204 bytes with 8.0f at +0x2C), for the infinite laser an
uninitialized word at +0x644, then two separate `StageEmbeddedVm` members (an
array would call the vector constructor iterator). The context links
`StageLifecycle.cpp` so that the VM constructor body is visible: as an
anchor it may throw, and `/EHsc` then adds unwind frames to both constructors
and to the spawn.

The ECL request for an infinite laser is the same parameter block:
`ResetSecondaryHostEffectSnapshot` (0x410880, exact, in the enemy.cpp range)
is `LaserInfiniteParams`'s constructor emitted out of line, and the
interpreter calls it before `CreateSecondaryHostObject(&packet, 1)`. So the
parameter types are header-defined with inline constructors, inlined in
laser.cpp and called out of line from enemy.cpp.

The spawn `CreateSecondaryHostObject` (0x424DF0) is a frontier
(`create-laser`): the body agrees apart from the request argument, which
native receives in EDI (`ret 4`) and this context keeps on the stack
(`ret 8`), and the register choice for the new laser that follows from it.
Probes with two or eight call sites passing a local request and constant
kinds, like the native ECL callers, did not change LTCG's choice. The real
callers are the ECL interpreter and slot 16 of `LaserLineInf` (0x425850).
`Append` copies the tail into a local first; reading `tail` twice reloads it
after the `laser->previous` store, as the out-of-line 0x4248C0 does not.
Replay: `.analysis/claude-laser/replay-5.log`.

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
| 0x424BF0 | `DeleteLaserManager`: `delete g_LaserManager` (no caller); exact | no arguments |
| 0x424C10 | `LaserInf::Update`: a set clear byte +0x68 deletes the laser one frame later; otherwise Update, then advance timer +0x14; exact | LTCG EDI manager |
| 0x424D40 | `LaserInf::Draw`: Draw slot of every laser not in state 1; exact | LTCG EAX manager |
| 0x424D70 | `LaserManagerUpdateCallback`: returns early on `GameInf` flag bits 0, 2 or 10; under bit 1 runs with the time scale set to zero; exact | ECX manager |
| 0x424DD0 | `LaserManagerDrawCallback`: skips under bit 2, else tail-jumps to the draw loop; exact | ECX manager |
| 0x424DF0 | `CreateSecondaryHostObject` spawn: kind 0 new `LaserLineInf` (0xE8C), kind 1 new `LaserInfiniteInf` (0xEB0), link at tail, Initialize(request); frontier | LTCG EDI request, stack kind |
| 0x424EA0 | `LaserLineInf` constructor: base, vtable, `memset` +0x440 (0x1E4), VMs +0x624/+0xA58; exact | LTCG EAX this |
| 0x424EE0 | `LaserInfiniteInf` constructor: base, vtable, `memset` +0x440 (0x204), float +0x46C, VMs +0x648/+0xA7C; exact | LTCG EAX this |
| 0x424FB0 | `MarkSecondaryHostEffects`: mark every live laser for clearing (+0x68 = 1); exact | no arguments |

Object layout so far: +0 vptr, +4 previous, +8 next, +0xC state (1 is
removed), 0x14-byte timers at +0x14 and +0x28, position +0x3C, velocity
+0x48, angle +0x54, speed +0x60, the clear byte +0x68 and the started byte
+0x69, ID +0x6C, eighteen 0x34-byte pattern records from +0x70 (a timer and
eight words; word 5 is the duration), pattern flags +0x41C (bit 2 pattern 1,
bit 4 pattern 3) and a timer at +0x424. Sizes: base 0x440, line 0xE8C,
infinite 0xEB0.

`LaserLineInf::Unknown2c` (slot 11, 0x425BD0) is pattern 1, an acceleration:
speed += word 0 x time scale, velocity += words 2-4 x time scale, the angle
follows `atan2(vy, vx)` unless both components are below `(double)0.0001f`,
and bit 2 clears on expiry; the timer then advances out of line. It is a
frontier (`laser-line-acceleration`, 237 vs 235 bytes): native has an 8-byte
aligned frame and reloads the stored velocity before `fabs`; a named
time-scale local adds a spill. Slot 13 (0x425AD0) is the matching pattern 3
(angular change with a repeat count) and calls the FSINCOS helper copy at
0x4288B0.

The functions marked unreferenced are absent from `config/functions.csv`
(0x424BD0 is listed); see KNOWLEDGE_BASE, "Unreferenced functions survive".

## Next

`LaserLineInf::Initialize` (0x4250B0) starts with `params = *request`, a
0x79-dword copy of the 0x1E4-byte `LaserLineParams`; 0x4248B0 is that copy
assignment out of line. It then sets state 2, resets and binds both VMs with
the bullet sprite table at 0x4A3EC0 (0xD0 bytes per entry, indexed by the
params' type at +0x464 and color at +0x466) and runs `ExecuteAnimationVm`
once on each, so it depends on the animation VM frontier.

The unreferenced helpers above need ledger rows before they can be claimed.
`LaserInf::Remove` is 0x4248E0's inline form; the spawn (0x424DF0) inlines
the 0x4248C0 append. 0x425070 (unlisted) sums slot 9 over all lasers.
The derived slot bodies (181-1,644 bytes) are the bulk of the TU. TH10's
`EnemyLaser*` sources are a structural reference only.
