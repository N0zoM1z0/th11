# TH11 replay-specific input repetition (2026-11-12)

The original `0x40DE10..0x40DE98` is a complete **137-byte** input-repeat
owner used by gameplay and replay recording. It is distinct from the UI
repeater at `0x459B50`: the replay input block has 32 held-frame counters
at `+0x94`, input `current/previous` at `+0x118/+0x11C`, repeat at
`+0x120`, pressed/released at `+0x124/+0x128`, and held mask at `+0x130`.
Two real callers include replay frame writer `0x435FE0`.

It advances each key's held-frame counter, turns on a held bit after eight
frames, enables repeat at 26 frames and subtracts eight, zeroes released
counters, then updates pressed/released XOR masks. Independent Ghidra review
confirms the complete loop and return, neighbors/CC padding, and lack of
foreign interior references or owned table data.

Natural `ReplayInputRepeatView` C++ source emits the exact original complete
function under pinned VC9 RTM `/TP /O2 /Ob1 /Gy /MT /Isrc`. An intermediate
codegen candidate matched 135/137 bytes, differing only in whether ESI was
copied from EAX or EDX before the final pressed-bit AND. Expressing pressed
and released as the original source-level XOR-and-mask formulas recovers
those two bytes without an ABI override or instruction patch. Canonical
`replay-input-repeat-state` strictly cold-replays **137/137 bytes, zero
relocations and zero differences**. The original source class/TU remains
unidentified; the physical fields and full code contribution are established.
