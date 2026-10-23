# Shared file operations and screenshot writer

Direct Ghidra12.1.3 review of the attested Japanese TH11 v1.00a identifies
four complete authored-game functions. All now pass cold pinned VC9
linked-PE exact proofs over their complete public primary-TU PDB contributions.

| Source name | Target extent | Bytes | Linkage fields |
| --- | --- | ---: | ---: |
| `ScreenshotWriteThread` | 0x00429BB0..0x00429C94 | 229 | 26 |
| `OpenSharedFileForWrite` | 0x004587A0..0x00458853 | 180 | 17 |
| `OpenSharedFileForRead` | 0x00458860..0x00458913 | 180 | 17 |
| `WriteSharedFile` | 0x00458920..0x00458977 | 88 | 8 |

Each body is contiguous, has reviewed returns and following CC padding, and
has no embedded data or outside references to its interior. The wrappers have
game callers and a shared handle/critical-section/error-trace protocol rather
than an unclassified CRT implementation.

## Shared handle and lock protocol

The handle storage cell is0x004A34E0, initially0xFFFFFFFF in the target. Flag
0x8000 at MainRuntime+0x590 enables critical section2 at root+0x840
(0x004C3AC0), with depth byte2 at root+0x932 (0x004C3BB2).

Both open functions enter/increment before CreateFileA. Success stores the
handle, traces the filename and returns0 while retaining the lock. Failure
formats GetLastError with flags0x1300/language0x400, traces the allocated
message, calls LocalFree, conditionally leaves/decrements and returns-1.

Write mode uses GENERIC_WRITE, FILE_SHARE_READ, CREATE_ALWAYS and normal
attributes. Read mode uses GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING and
FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN (0x08000080). Both observed
failure strings say `write error`; the read source preserves that text.

`WriteSharedFile` returns-1 when the stored handle is invalid. Otherwise it
calls WriteFile and compares the written DWORD with the requested size.
A mismatch closes the handle, conditionally unlocks and returns-2; a full
count returns0. The WriteFile BOOL is not the condition used by this code.
Close operations do not reset the stored handle. Source preserves this
protocol, including the worker's subsequent operations after a short write.

The trace target0x00459BC0 is the same release-disabled variadic sink already
named `InputAcquireHook` by the input reconstruction. The new source reuses
that symbol and signature without modifying input headers or existing proofs.

## Worker recovered from an inventory gap

The initial Ghidra function inventory omitted0x00429BB0 despite its DATA
reference from capture at0x00429E81, passed to `_beginthread`. The new bounded
`review_span` query uses Ghidra's PseudoDisassembler without changing the
database, function table or listing. It labels its extent as a review
hypothesis, requires a complete decode and accepts at most4096 bytes within
one executable memory block. Target and complete mapped `.text` attestation
still run before the query.

The reviewed worker has229 decoded bytes, one RET, no existing function
overlap, no outside interior references, preceding CC padding after the prior
RET, and eleven following CC bytes before capture at0x00429CA0. Its flow and
callback reference independently justify the newly curated ledger entry;
the caller-requested range alone grants no extent or exact credit. The
inventory therefore grows from1988 to1989 candidates.

The ordinary cdecl callback ignores its context. It opens the stored filename,
conditionally writes the14-byte BMP file header, calls the write helper for
the40-byte info header and0xE1000 pixel bytes, then closes. It ignores open
and helper status values as observed. Finally it frees non-null info/pixel
buffers, clears those pointers and writes zero to the thread-state cell.
It preserves the header short-write close/unlock path before later writes.

## Independent bindings

The17 open-function fields share the same offsets and target values in both
modes. Offsets are relative to each function start.

| Offsets | Meaning | Target |
| --- | --- | --- |
| 3,130 | Runtime flags | root0x004C3280+0x590 |
| 14,141 | Section2 | root+0x840 |
| 26,153 | Depth2 | root+0x932 |
| 20;147 | Enter/LeaveCriticalSection IAT | 0x0048B084;0x0048B088 |
| 51 | CreateFileA IAT | 0x0048B098 |
| 56 | Shared handle | 0x004A34E0 |
| 81;95;124 | GetLastError/FormatMessageA/LocalFree IAT | 0x0048B1B8;0x0048B0F8;0x0048B0FC |
| 105;164 | Failure/success format strings | 0x00497818;0x00497860 |
| 110,169 | Variadic trace CALL | 0x00459BC0 |

The write helper's eight fields at2,27,37,44,50,61,67,73 bind the shared handle,
WriteFile IAT0x0048B0A8, shared handle, CloseHandle IAT0x0048B0A0, flags,
section2, LeaveCriticalSection IAT and depth2 respectively.

The worker's26 exhaustive fields bind existing runtime screenshot offsets
(thread+0x6D0, file header+0x6D4, info+0x6E4, pixels+0x6E8, filename+0x6EC),
the shared handle, flags/section/depth2, WriteFile/CloseHandle/Leave IATs,
open/write helper CALLs and free at0x0045FC3B. Exact offsets, symbol identities
and member addends are declared in `config/match-units.toml`; the complete
target instructions were independently reviewed before those declarations.

## Cold proofs and reproduction

All four units use pinned VC9 RTM `/TP /O2 /Ob1 /Gy /MT /Isrc /GL`. Ordinary
stdcall declarations naturally produce filename-in-ESI and buffer-in-EDX /
size-in-ESI; no custom register ABI or padding was introduced. Writing the
source comparison as `size != written` reproduces the target's operand order.

Two cold families keep every accepted owner in its primary TU: the worker is
primary with SharedFile support, while the three shared-file functions are
primary with worker and independent filename-caller support. No support-TU
contribution is accepted by borrowing its caller's proof. Current source,
header, tool, producer, PE/MAP/PDB identities and complete field replay all
verify zero differences. Only these four new canonical units were replayed;
the prior sixteen proofs were not replayed.

```bash
scripts/repo-python scripts/ghidra.py query .analysis/screenshot-worker-review.txt \
  review_span 0x00429BB0 0x00429C94
scripts/repo-python scripts/ghidra.py query .analysis/screenshot-file-review.txt review \
  0x004587A0 0x00458920
scripts/repo-python scripts/ghidra.py query .analysis/file-open-read-review.txt review \
  0x00458860
scripts/repo-python scripts/replay-exact-units.py \
  --unit screenshot-write-thread --unit open-shared-file-for-write \
  --unit open-shared-file-for-read --unit write-shared-file
```

The complete native data owner, production TU/link partition and full game
runtime remain open. Capture/hotkey codegen differences are tracked separately
in `SCREENSHOT.md`; the worker proof does not close those functions.
