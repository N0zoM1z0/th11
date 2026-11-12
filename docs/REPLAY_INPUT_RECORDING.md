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

## Complete 417-byte replay frame recording boss

The native `0x435FE0..0x436180` recorder is a larger 417-byte function,
called by the replay update callback at `0x436D22`. Maintained natural C++
source in `src/ReplayFrameRecording.cpp` now reproduces every branch's
intended data flow, not just the two leaf calls. In the original, the record
pointer has private EAX receiver, the active replay gate resides at
`0x4A8E88`, an input-modifier bit at runtime `+0x200` changes how
`current` and held-frame counters are updated, and the exact 137-byte input
repeater runs before writing each recorded event.

Every 30 frames the original converts a sampled float from physical
`0x4A8D80 + 0x34` with a 0.5 rounding adjustment and a 256.0 saturation
threshold, then appends one byte through the current replay segment's
`+0x18A0` cursor. Every frame it writes 3 two-byte values (current,
pressed, released) through the `+0x1518` event cursor and advances it by 6.
When 900 entries have been used, it calls the existing full
`AppendReplayWriteSegment` source using the record's selected stage at
`+0x1D4`. The real segment node is held at record `+0x9C` and the frame
counter at `+0x1CC`. These offsets are asserted in the maintained view.

Direct native Ghidra review independently confirms the complete 417-byte
boundary, 108 instructions, two calls (to `0x40DE10` and `0x437300`),
ordinary return1 and following padding. Under pinned VC9 normal COFF the
natural source emits 393 bytes; scoped `/GL` with real input support emits
389 bytes, and adding the real replay writer produces 390. Both native
calls remain and neither outcome is byte-exact. Open issues include private
EAX register ABI and x87 float-to-int control-word codegen; no bytes are
cropped or counted as exact. Retained diagnostic:
`.analysis/probe-replay-frame-recording-ltcg-20261112.log`.
