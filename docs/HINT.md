# Hints (hint.cpp)

The hint manager (`hint.cpp:194 HintInf`, global 0x4A8E8C, 0x1A8 bytes) loads
`hint/hint_auto.txt` and `hint/hint_user.txt` (0x421BD0) when the hint mode
byte at `g_MainRuntime + 0x1EA` is set, keeps sixteen lists of hint data
(two groups of eight) from +0x18 and ten animation VM IDs from +0xDC.
`PauseSecondaryGameplaySceneEffect` (0x421070) pauses its chains. Source:
`src/Hint.{hpp,cpp}`, context `build/match-linked/HintManager` (`/EHs`).

Exact (2026-11-28): `InitializeHintManager` (0x420F90, EDI this; update
priority 25, draw priority 51; the draw element clears its two extra callbacks
before the callback store) and `FreeHintLists` (0x421210, EAX this).

Open:
- The constructor (0x420E90) has the native size; only the order of its
  sixteen list-head stores differs. Native writes the sixteen lists as
  straight-line code (a loop stays a loop in VC9), and reordering the three
  field stores in source does not change the scheduled order.
- The destructor (0x4210A0) inlines both `FindAnimationVmById` and
  `ReleaseSecondaryAnimation` for the ten VM IDs, then saves user hints in
  hint mode 2 (0x422650) and frees the lists. Those two helpers are separate
  functions elsewhere, so ZUN's headers have inline versions; the create
  function (0x421270) needs the destructor's stack `this`.
