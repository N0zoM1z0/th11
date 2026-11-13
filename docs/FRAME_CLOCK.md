# Frame clock

Direct Ghidra12.1.3 review of the original Japanese TH11 v1.00a identifies
`GetFrameClockSeconds` at `0x00446920..0x00446A23`,260 bytes. The database
passed complete mapped `.text` SHA-256 attestation. Its one contiguous body
has two return paths, no embedded data, no outside interior references and
following CC padding. Frame dispatch at0x00446650 and presentation at0x00446790
call it to record frame timestamps and duration; these custom game call sites
and timing state support the authored-game classification independently of
compiler similarity.

Private review commands:

```bash
scripts/repo-python scripts/ghidra.py query .analysis/frame-timing-review.txt review \
  0x00446650 0x00446790 0x00446880 0x00446920
scripts/repo-python scripts/ghidra.py query .analysis/frame-clock-xrefs.txt xrefs_to 50 \
  0x004C3DA0 0x004C3DA8 0x004C3DE0
```

## Behavior and storage

Runtime flag0x8000 enables critical section5 and depth byte5 in the partial
`MainRuntimeView`. It enters before selecting the clock path and leaves on
either return path. The lock is at0x004C3B08 and depth at0x004C3BB5.

With a nonzero signed64-bit frequency at0x004C3DA0, the function calls
QueryPerformanceCounter, subtracts the signed64-bit counter base at0x004C3DA8
and divides by the frequency. If the stored origin at0x004C3DE0 is greater
than the result, it lowers the origin. It releases the conditional lock and
then subtracts the origin for the return value. This read after unlocking is
preserved in the source.

With zero frequency, it converts the unsigned timeGetTime result to double,
lowers the same stored origin when appropriate, and computes exactly
`(milliseconds - origin * 1000.0) / 1000.0` before unlocking and returning.
The placement of the multiplication is observed target behavior. The unsigned
conversion uses2^32, not a signed interpretation of the DWORD.

The declarations in `FrameClock.hpp` describe observed storage cells; their
complete native owner remains open. Independent Ghidra references show
frequency/base addresses passed by initialization at0x00445FFC/0x00446007 and
an origin write at0x004457A4.

## Exact proof

Canonical unit `get-frame-clock-seconds` uses ordinary C++ and a separate
two-observation caller in `probes/frame-clock-context.cpp`, with pinned VC9
RTM `/TP /O2 /Ob1 /Gy /MT /Isrc /GL`. The full public primary-TU PDB
contribution is260 bytes. A cold source/header/tool/producer/PE/MAP/PDB receipt
and exhaustive field replay prove zero byte differences.

The independently reviewed26 DIR32 fields are grouped below. Offsets are
relative to the function start; each field occupies four bytes.

| Meaning | Field offsets | Target base and addend |
| --- | --- | --- |
| Runtime flags | 5,118,209 | 0x004C3280+0x590 |
| Critical section5 | 16,129,237 | 0x004C3280+0x888 |
| Depth byte5 | 28,146,249 | 0x004C3280+0x935 |
| EnterCriticalSection IAT | 22 | 0x0048B084 |
| LeaveCriticalSection IAT | 137,243 | 0x0048B088 |
| Frequency | 33,88;39 | 0x004C3DA0;0x004C3DA0+4 |
| Counter base | 62;72 | 0x004C3DA8;0x004C3DA8+4 |
| Origin | 99,112,152,184,203 | 0x004C3DE0 |
| QueryPerformanceCounter IAT | 52 | 0x0048B0D0 |
| timeGetTime IAT | 162 | 0x0048B298 |
| Double2^32 | 178 | 0x00497EB0 |
| Double1000 | 219 | 0x00497EA8 |

The constant cells independently decode to IEEE754 values4294967296.0 and
1000.0. Linked member fields bind the root symbol plus the reviewed addend.
No instruction bytes, stack padding or register declarations were imposed.
Normal COFF probing reaches260 bytes but differs in three stack-allocation
immediates (16 versus20); the natural linked caller produces the target's
20-byte allocation. Original production TU ownership remains open.

Only this new unit was cold-replayed for its addition. Existing exact functions
were not rebuilt. Full game build and native runtime validation remain open.

## 181-byte frame-tick worker handoff (2026-11-13)

A distinct 181-byte timing thread at `0x443140..0x4431F4` is now mapped to
`src/FrameTickWorker.cpp/.hpp` and the dynamic compile-only consumer
`probes/frame-tick-worker-context.cpp`. It sets running bit 1 of the worker
flags at `0x4A5790`, checks stop-request bit 0, repeatedly samples the
**independently exact** `GetFrameClockSeconds` function, adjusts the running
1/60-second target, counts frames and calls `Sleep(13)`. State fields occur at
`0x4A5794`, `0x4A579C`, `0x4A57A4` and `0x4A57BC`, so the partial view
requires four-byte packing for the double fields.

The original span includes a reached 2-byte forward jump and 6-byte
`LEA EBX,[EBX]` alignment at `0x443168..0x44316F`. Current source compiles
under pinned VC9; scoped dynamic `/GL` produces 183/181 bytes and an
independent real-clock `/GL` producer 168/181 bytes, with x87 scheduling and
exit-path differences. This is **source-present, nonexact** and carries **no
new exact credit**. Relevant local evidence is in
`.analysis/probe-frame-tick-worker-ltcg-20261113.py` and
`.analysis/probe-frame-tick-worker-shapes-20261113.py`; the mapped original
Ghidra review is `.analysis/media-timing-thread-native-20261113.txt`.
