# Replay record lifetime and embedded stage links

The 0x2DC-byte replay record (`TitleMenuRecordView`) embeds eight 0x24-byte
`ReplayRecordStageLink` members at +0xA4. All sources are plain C++; no
`__asm`, alias or volatile tricks remain.

| Function | Address | Bytes | State |
| --- | --- | ---: | --- |
| `ReplayRecordStageLink` constructor | 437490 | 51 | Exact (`replay-record-stage-link-constructor`) |
| `ReplayRecordStageLink` destructor | 4374D0 | 37 | Exact (`replay-record-stage-link-destructor`) |
| `SumPayloadBytes` | 437500 | 79 | Exact (`replay-payload-checksum`) |
| `TitleMenuRecordView` destructor | 435C50 | 429 | Byte-identical apart from fields; SEH-framed, not claimed |
| `CreateReplayRecording` | 435E00 | 199 | C++ in `ReplayRecordFactories.cpp`; SEH-framed |
| `CreateReplayPlayback` | 435EF0 | 198 | C++ in `ReplayRecordFactories.cpp`; SEH-framed |

## Stage link

The constructor is `memset(this, 0, sizeof *this)`, two cursor copies that
re-read memory, then the node publication; VC9 expands the memset into the
nine native stores. The destructor unlinks its node and clears both links.

## Record destructor 435C50

An ordinary out-of-line destructor that callers invoke directly; LTCG passes
`this` on the stack (`ret 4`). It frees the entry data, clears the eight stage
segment lists, frees the buffers, removes the update, secondary and draw chain
elements under critical section 0, clears `g_SecondaryReplayCurrent` if it
points here, and the compiler destroys the stage links. Buffers are released
with typed `delete`, and the chain manager is loaded after the null check.
With `UnregisterChainElement(manager, element)` and the chain remover linked,
all 429 bytes match apart from relocated fields. The SEH handler field
(`__ehhandler$`, FuncInfo at 0x49F8C0) is outside what the linked oracle
compares, so the unit is not claimed.

## Factories

`new TitleMenuRecordView` (the member array makes the constructor
EH-protected), then `g_ReplayRecordRegistry.InitializeRecord(record, mode)`
or `LoadReplayStageData`, destroying the record on failure. In the C++
initializer's link context the playback factory matches 60/60 instructions;
both stay unclaimed for the same EH reason.

## Payload checksum 437500

A plain byte-sum loop over the bytes after the 8-byte header. VC9 splits it
into the two accumulators and the odd trailing byte seen in native.
