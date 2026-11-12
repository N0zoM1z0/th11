# Replay record lifetime and embedded stage links (2026-11-12)

The original Japanese TH11 1.00a replay record embeds eight 0x24-byte
intrusive stage nodes. Each node's unlink destructor at `0x4374D0..0x4374F4`
has a complete, independent **37-byte strict match**. The predecessor at
`+0x1C` has its successor at `+8`, the successor at `+0x20` has its
predecessor at `+4`, and the destructor finally clears the local `+0x1C`
then `+0x20` links. Source is the natural member destructor in
`ReplayRecordStageLink.cpp`. The final store order is visible in the original
and reproduces the native instruction sequence without byte patches.

Direct Ghidra review independently identifies all 37 bytes, two conditional
branches, the plain `RET`, next-function alignment padding, no external
interior references, and callback references from the replay record EH
vector destructors (`0x435DCA`, `0x435E41`, `0x435F31`). The locked VC9 RTM
normal-COFF context generates a unique 37-byte destructor COMDAT with **zero
relocations**; the dedicated canonical `replay-record-stage-link-destructor`
unit strictly cold-replays all 37 bytes with zero differences.

The enclosing **429-byte replay record destructor at `0x435C50`** is a larger
separate owner. It frees record data, clears eight segment streams, deletes
several owned buffers, unregisters up to three chain elements with optional
runtime critical-section locks, clears a singleton if owned, and destructs the
eight stage links above. That enclosing body requires separate source and
linked EH/codegen analysis; the stage-link match alone makes no claim for it.

## Full 429-byte record destructor source

The enclosing `0x435C50..0x435DFC` is now mapped to
`TitleMenuRecord.cpp`, with its complete 429-byte cleanup represented in
natural C++. The physical `TitleMenuRecordView` has streams at `+0x40`,
`ReplayRecordStageLink stages[8]` at `+0xA4` and a third chain link at
`+0x1D0`; compile-time offset assertions protect those observations. C++
member lifetime generates an actual vector of eight nested stage-link
destructors, avoiding a synthetic call or dummy EH stack frame. Cleanup
performs each original ownership action and preserves its three independent
conditional chain unregisters. The wrapper is separately callable by the
title/menu owner and other replay consumers.

The locked VC9 normal-COFF `/O2 /Ob1 /Gy /MT /EHsc` compilation yields a
**complete 429-byte destructor function** with 36 source relocation fields,
but the full strict comparison still has differences, including EH and
private caller-register scheduling. This is a size-aligned *source-present*
boss, **not an exact claim**; its private cold-linked primary-TU context
remains to be established. The independently exact 37-byte stage-link
function and its canonical receipt are unchanged.
