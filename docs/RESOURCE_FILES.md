# Resource loading and complete-file writes

Direct Ghidra12.1.3 review of the attested original Japanese TH11 v1.00a
covers five complete contiguous bodies. Each has reviewed entry/return paths,
incoming references, following CC padding, no embedded data and no outside
references to its interior.

| Source name | Target extent | Bytes | State |
| --- | --- | ---: | --- |
| `FindArchiveEntry` | 0x004418D0..0x00441904 | 53 | Exact |
| `LoadResourceFile` | 0x00458400..0x004585D1 | 466 | Exact |
| `WriteWholeFile` | 0x00458670..0x00458798 | 297 | Exact |
| `ReadArchivedFile` | 0x00441760..0x0044188B | 300 | Source present; non-exact310-byte candidate |
| `LeaveRuntimeCriticalSection` | 0x0042B700..0x0042B724 | 37 | Source present; one SIB-byte difference |

The loader's16 reviewed callers include animation, gameplay, UI and replay
paths. Its custom archive state, cipher-table integration, trace messages and
shared game critical-section protocol support authored-game classification.
The three exact functions add816 owned bytes through cold linked-PE proofs.

## Resource loader

The ordinary three-argument stdcall source lets VC9 /GL pass filename in EAX
and leave size/mode on the stack. It enters shared-file section/depth2 when
runtime flag0x8000 is set. Zero mode selects the archive; nonzero mode selects
disk. A missing archive entry does not fall back to disk.

Archive mode first searches for the last backslash, then searches for the last
slash starting after that backslash. If the second search fails, it restores
the original filename, even when the first search found a backslash. Source
preserves this observed behavior rather than normalizing the path differently.
It scans16-byte archive entries case-insensitively for a size, writes that
size to a non-null output cell (including zero on a miss), allocates a buffer,
and invokes extraction. It ignores extraction's result and returns the
allocated pointer after the generic runtime unlock call.

Disk mode traces the original path and uses GENERIC_READ, FILE_SHARE_READ,
OPEN_EXISTING and normal/sequential-scan flags. It allocates GetFileSize bytes,
calls ReadFile with the same DWORD as count input/output, updates a non-null
size output with the returned count, closes and unlocks. The ReadFile BOOL is
not tested. Open/allocation failure leaves the caller's size output unchanged.

The target puts its shared failure cleanup before disk-read success. Explicit
failure joins preserve that layout. Earlier structured variants produced475,
407 and469 bytes; the final complete466-byte body replays all36 fields with
zero differences. The zero-size lookup assignment belongs after a failed
search; keeping it live across the compare loop changes register allocation.

## Complete-file writer

`WriteWholeFile` uses a local handle independently of the shared handle cell.
It conditionally enters section2, creates/truncates the file, writes the
requested byte count, closes and releases on each path. It returns-1 for
open failure, -2 for a short byte count and0 for a complete count. Open failure
formats GetLastError, emits the observed trace and frees the system message;
short/success paths have their respective traces. As in the target, count
comparison rather than WriteFile BOOL controls the result.

Ordinary stdcall source naturally yields filename-in-EDI, size-in-EBX and
buffer on the stack. It reproduces the complete297-byte body and29 fields,
including the compiler's reuse of the dead incoming buffer home for the
error-message pointer. No custom ABI, artificial padding or fake return was
introduced.

## Archive views and pending extraction

`Archive.hpp` describes partial observed views, not complete original owners:

| View | Fields |
| --- | --- |
| Archive state | entries+0, signed count+4, archive filename+8, stream+0xC |
| 16-byte entry | stored filename+0, file offset+4, unpacked size+8, unknown+0xC |
| Stream interface | Read at vtable+8, bool Seek at+0x18; other slots opaque |
| 12-byte cipher rule | key+0, step+1, unknown+2, block size+4, limit+8 |

The game archive is rooted at0x004C9680; cipher rules start at0x004A3480.
Packed length comes from the next entry's offset minus the selected offset,
so the view also exposes the observed sentinel-offset requirement.

`FindArchiveEntry` rejects null storage/nonpositive count, walks records and
uses `_stricmp`. Its53-byte primary-TU proof has one REL32 at offset24 bound
to CRT0x004616F8. It remains a real out-of-line dependency of extraction.

The maintained extractor rejects a null stream, finds the entry, reuses a
destination only for equal packed/unpacked size, otherwise allocates packed
storage, seeks and reads. It computes an8-bit checksum of the stored entry
name, selects one of eight cipher rules, calls ScatterXorBlocks and optionally
decompresses. Its observed failure/success free conditions are preserved,
including freeing a non-null reused destination on failure. It remains
non-exact: current310-byte code differs from the300-byte target in branch
placement, alignment and register allocation. Size-favoring and default Ob2
probes did not close it; no cropped or masked comparison is credited.

The generic unlock source uses the runtime/index private convention generated
naturally from ordinary stdcall. Its37-byte candidate has one different SIB
byte at offset31, commuting EDI and ESI in the same effective address. It has
no exact credit. No runtime header or previous canonical source was changed.

## Independent bindings and cold proof

`config/match-units.toml` records all66 independently reviewed fields:
36 for loading,29 for writing and one for lookup. They include MainRuntime
root0x004C3280 with flags+0x590, section2+0x840 and depth2+0x932; archive root
0x004C9680 with count+4; reviewed kernel32 IAT cells; CRT strrchr/stricmp/malloc;
the trace at0x00459BC0; extraction at0x00441760; unlock at0x0042B700; and the
observed format-string cells. Target instruction/reference review and literal
reads preceded these declarations.

Three cold families keep each accepted public PDB contribution in its own
primary TU. Profile is pinned VC9 RTM `/TP /O2 /Ob1 /Gy /MT /Isrc /GL`.
Receipts cover all primary/support source, headers, historical tools, producers
and PE/MAP/PDB identities. Complete byte replay reports zero differences for
all three units. Only these new canonical units were replayed; previous20
proofs were not replayed.

Unknown archive owners remain represented by independent caller contexts.
Allowing only the game-global caller lets LTCG specialize extraction/lookup
and changes their convention; source therefore includes the observed second
caller role with unknown owners. At0x00442229, the caller supplies the receiver
returned by0x004420E0 and a null destination, rather than loading the game-global
address directly. That full218-byte caller was reviewed for this context;
its EH body is not accepted or credited here. Actual production TU partition and complete
native archive/stream/decompression closure remain open.

Private evidence and reproduction:

```bash
scripts/repo-python scripts/ghidra.py query .analysis/file-load-review.txt review \
  0x00458400 0x00458670
scripts/repo-python scripts/ghidra.py query .analysis/archive-extract-review.txt review \
  0x00441760 0x0042B700
scripts/repo-python scripts/ghidra.py query .analysis/archive-lookup-review.txt review \
  0x004418D0 0x00441C80 0x004426C0
scripts/repo-python scripts/replay-exact-units.py \
  --unit load-resource-file --unit write-whole-file --unit find-archive-entry
```

Bounded failed contexts are saved under `.analysis/archive-read-*` and
`.analysis/resource-file-*`. Final loader, writer and lookup receipts live in
their separate ignored `build/match-linked/` directories.
