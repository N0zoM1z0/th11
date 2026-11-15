# TH11 secondary gameplay statistics file writer

`SaveSecondaryGameplayStatisticsFile` (0x437A70, 639 bytes) serializes the
statistics root at 0x4A8EBC to `scoreth11.dat` and returns 0 or -1. It is
plain C++ since 2026-11-14 (the former body was an `__asm` copy) and reaches
178/201 aligned instructions; it is not exact.

The writer allocates a 2-MiB buffer and copies the 0x18-byte header. It then
walks the seven 0x68D4-byte blocks, and for each block tagged `CR` (0x5243) it
stores the slot index, sets the checksum through `SumPayloadBytes(0x68D4)`
and copies the block. Next it byte-sums the trailer's 0x440 payload bytes into
its checksum field and copies the 0x448-byte trailer. The header size fields
are set and the payload goes through `CompressReplayData` and
`GatherXorBlocks(0xAC, 0x35, 0x10)`. If `OpenSharedFileForWrite` fails, the
writer logs the Japanese error to `g_RuntimeLog` and frees both buffers.
Otherwise it writes the header and the packed payload and closes the file.

Remaining gap: VC9 turns the trailer sum into four accumulators, as native
does, but combines them in a different order. The probe gives
`SumPayloadBytes`, `GatherXorBlocks` and `OpenSharedFileForWrite` live callers
so that LTCG keeps their arguments.
