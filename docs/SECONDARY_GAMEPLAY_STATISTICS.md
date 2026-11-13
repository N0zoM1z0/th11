# TH11 secondary gameplay statistics file writer

## Native owner `0x00437A70..0x00437CEE` (639 bytes)

`SaveSecondaryGameplayStatisticsFile` serializes the secondary gameplay
statistics root at `0x4A8EBC` to `scoreth11.dat`. It has no arguments,
returns `0` or `-1` in EAX, has three return exits, 19 calls (11 direct and
eight imported indirect calls), and two incoming callers at `0x0041FE10`
and `0x00428C30`. Independent Capstone decoding covers all 639 bytes in
201 instructions. Ghidra's function body contains 636 bytes; the remaining
three bytes are the native alignment LEA at `0x00437ACD`, retained as part
of the complete owner. Trailing `CC` padding starts at `0x00437CEF`. A scan
of the rest of `.text` found no external direct branch/call reference into
the owner's interior.

The function allocates a 2-MiB scratch buffer and copies the six-DWORD file
header. It walks seven fixed-size `0x68D4` records, accepts records whose
leading word is `0x5243`, writes their slot index, computes the record
checksum with `SumPayloadBytes`, and copies each accepted record. It then
computes a four-lane checksum over `0x110` four-byte groups starting at
statistics offset `0x2DDDD`, stores the result at `0x2DDD8`, copies the
`0x112`-DWORD trailing statistics region, and sets the uncompressed payload
length. The payload is passed to the exact `CompressReplayData` owner, then
to `GatherXorBlocks` with key `0xAC`, step `0x35`, and block size `0x10`.

The output path opens `scoreth11.dat`, writes the 0x18-byte header and then
the packed payload through the original WriteFile IAT. Short writes retain
the original CloseHandle/LeaveCriticalSection/depth cleanup sequence. The
error path appends the native Japanese score-file error to `g_RuntimeLog`.
Both scratch allocations are freed on the original success/failure paths.

The maintained source is `src/SecondaryGameplayStatisticsWriter.cpp`; its
naked x86 owner retains the original stack frame, register allocation,
private helper ABIs, IAT calls and all alignment/layout bytes. The canonical
linked unit is `secondary-gameplay-statistics-writer`; the 37 linkage fields
cover the statistics root, checksum/compressor/cipher/file/log helpers,
strings, shared-file state, IAT slots and critical-section state. Strict
pinned VC9 RTM `/GL` replay proves a complete 639-byte primary-TU PDB
contribution, all 37 fields and zero byte differences. The original C++
class and translation-unit attribution remain unknown and are not inferred
from this machine-code reconstruction.
