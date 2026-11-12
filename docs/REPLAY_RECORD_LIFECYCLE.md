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

The first locked VC9 normal-COFF `/O2 /Ob1 /Gy /MT /EHsc` candidate
emitted 429 bytes with 36 relocations but a private receiver and three extra
lock EH scopes. That earlier unlinked diagnostic was **not** exact. The stage
link's independent strict 37/37 receipt remains unchanged.

## Narrowed full-length `/GL` boss (2026-11-12)

A minimal genuine producer at `probes/title-replay-record-context.cpp` calls
the real 429-byte record owner. With both real `ClearReplayStageSegments`
and `ReplayRecordStageLink::~ReplayRecordStageLink` as `/GL` support, the
compiler reproduces its private **stack-argument `RET 4`** and real loop ABI.
The source now uses explicit paired conditional critical-section calls
rather than three local scoped lock objects; the native destructor did not
emit EH-state stores for those locks. Loading the chain manager before each
lock check also restores the original manager lifetime, register allocation
and complete body size. Natural local pointers for the two delete paths and
`this == g_TitleReplayRecord` recover the target load/compare encodings.

| Scoped linked diagnostic | Body bytes | Instructions | Calls | Address/call fields | Non-address byte differences |
| --- | ---: | ---: | ---: | ---: | ---: |
| Initial worker with real stages (RAII locks) | 432 / 429 | 115 / 114 | 14 / 14 | 36 | 182 |
| Explicit lock pairs | 417 / 429 | 112 / 114 | 14 / 14 | 36 | 153 |
| Preload chain manager | 429 / 429 | 114 / 114 | 14 / 14 | 38 | 11 |
| Preserve delete locals; reverse singleton equality | **429 / 429** | **114 / 114** | **14 / 14** | **38** | **6** |

The six differences are exactly the instruction-byte register move order in
three calls to `UnregisterChainElement`, each of which moves the correct
pointer values into `ECX` and `EDX` before the call. Natural reference,
inline-forwarder and helper-argument-order variants did not improve them;
including the full chain-remove producer instead of its external call changed
the link-time owner to 370 bytes, so that context is not promoted. Evidence:
`.analysis/probe-title-record-real-callees-20261112.log`,
`.analysis/probe-title-record-11diffs-20261112.log`,
`.analysis/probe-title-record-last-six-20261112.log`, and
`.analysis/verify-title-record-current-20261112.py` (complete original
function and candidate bytes plus all decoded fields). In ordinary COFF the
updated source produces **425 bytes**; that different context grants no
comparison credit.

The complete 429-byte linked diagnostic is deliberately still **non-exact**:
no masked six-byte exception or altered function boundary is accepted, and
original translation-unit ownership remains unknown. The strongest next step
is recovering native argument-move scheduling without changing the real
callee ABI or the established `/GL` sources.

## Mixed producer check (2026-11-12)

The native 429-byte owner was re-linked with its two genuine stage producers
and either an opaque chain-removal anchor or the real `ChainRemove.cpp` built
as normal COFF. Both preserve the exact **429-byte/114-instruction/14-call**
layout and the same **six** non-field bytes of argument-move scheduling.
Building both stage producers as normal COFF instead shortens it to 427
bytes and changes over 200 non-field bytes. The best maintained source stays
unchanged; neither alternate context is strict exact. Private evidence:
`.analysis/probe-title-record-mixed-producer-20261112.log`.

## Exact 51-byte stage construction (2026-11-12)

Independent Ghidra review of native `0x437490..0x4374C2` recovered the
constructor missed by the initial function inventory. It is a complete
**51-byte** member owner referenced by both 199-byte and 198-byte replay
record factories as the eight-element vector constructor callback. The
function zeroes each of nine DWORDs in the 0x24-byte stage, reads two of the
cleared cursor fields back into the companion slots, then writes the stage's
self-node pointer at +0x18 and clears the two intrusive links. The constructor
has a plain `RET`, following CC padding and no foreign interior entries.

An ordinary C++ loop collapses two stores/reads and generates 48 bytes. The
maintained constructor expresses each *observed memory write* explicitly
through a volatile physical field view; it then performs the two cursor copies
and normal typed link publication. This preserves the actual native memory
operations, not inert padding or fabricated machine instructions. The pinned
VC9 RTM normal-COFF unit `replay-record-stage-link-constructor` strictly
replays **51/51 original bytes with zero relocations or differences**. The
paired 37-byte stage-link destructor remains independently exact after the
shared header change. The original constructor's class/TU name is still open.
