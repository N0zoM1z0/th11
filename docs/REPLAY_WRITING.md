# Replay writing and stream-chunk allocation

The original Japanese TH11 v1.00a replay writer at `00436420..00436B5F`
now has maintained source in `src/ReplayWriter.cpp`, alongside the
`00437300..00437395` stream allocator. Both are **source-present,
non-exact**. The writer is a substantial 1856-byte owner, not a leaf.
Original TU/LTCG partition, data ownership and runtime fidelity are open.

## Boundaries and dependencies

Hash-pinned direct Ghidra review covers the writer's 1850 body bytes,
with a six-byte alignment LEA at `43652A..43652F` completing the
1856-byte span. Independent original-PE Capstone disassembly decodes
all 1856 bytes into 564 instructions. The next owner begins at
`436B60`, with no foreign interior entry or owned jump table.
Four callers originate at `42D2AE`, `42E21B`,
`42EF4B` and `4403C5`; 15 native callee identities were
reviewed. The writer receives a filename in ECX, name in EDX and mode
on the stack, returning with RET4.

The segment allocator is a separate full 150-byte owner. The native
passes recorder in EBX and stage index in EDI under an optimized private
ABI, returning the embedded node in EAX. Maintained C++ uses ordinary
typed parameters rather than misdeclaring the native ABI. Independent
full PE decode contains 40 instructions and one RET; Ghidra shows four
callers, no foreign interior reference and following CC padding.

The writer calls the source-open compressor at `4423A0` (795
bytes), the source-present but non-exact `GatherXorBlocks` at
`4582E0`, and the exact `OpenSharedFileForWrite` at
`4587A0`. The compressor dictionary-reset leaf at `4428D0` now reproduces all
52 native bytes including four exact field bindings via a cold pinned VC9
normal-COFF proof (unit `replay-compression-tree-reset`). Its 8-KiB
window and 8193 three-word tree nodes occupy adjacent mapped storage;
a three-byte compiler alignment LEA remains in the proven extent.
The original source/TU owner is not established by this COFF match.
The main compressor now has maintained source in `src/ReplayCompressionEncoder.cpp`;
its writer dependency is no longer source-open. The compiler does not yet
reproduce its complete native body, and its original TU/ABI remains unknown.

## Recovered behavior and layout

Recording state holds a 0x70-byte header, 0x24-byte file header,
eight optional 0x90-byte stage headers and intrusive segment lists.
A segment occupies 0x18B0 bytes, with event cursor +0x1518,
secondary-data cursor +0x18A0 and linked node +0x18A4.
Events are six bytes. Before saving in the relevant mode, the
writer emits a triple-FFFF end event and allocates another segment
at the 900-record threshold.

The writer first counts every stage's two data areas and updates the
stage record counts, then serializes stage headers, event data and
secondary data. It records the current score, stage count and slowdown
percentage based on doubles at +0x24/+0x2C of the timing object.
There are no added null, index or overflow guards on the main path.

After compression the native uses **two** byte gathers. Assembly
loads AL=0x3D with step 0x7A/block 0x40 for the first, and AL=0xAA
with step 0xE1/block 0x800 for the second, each with packed length
as the limit. These keys are omitted from default Ghidra decompiler
argument lists; they were recovered from the call instructions.
The writer outputs the 0x24-byte file header, encrypted compressed
payload and two four-byte-aligned USER metadata sections: Shift-JIS
replay summary and Japanese comment prompt. It closes the file and
marks the recorder finalized.

## Current codegen and next work

Pinned VC9 normal-COFF compilation with
`/O2 /Ob1 /Gy /MT /EHsc` succeeds, but the full function
diagnostics are not exact:

| Owner | Native span | Source COFF | Relocations | Matched comparable bytes |
| --- | ---: | ---: | ---: | ---: |
| WritePauseMenuReplay | 1856 | 1678 | 93 | 79 / 1484 |
| AppendReplayWriteSegment | 150 | 139 | 2 | 39 / 142 |

Masked similarity is diagnostic only. Address fields require
independent binding before exact credit. A linked /GL context has
not yet been built for these owners. Next: narrow the main compressor codegen differences and
resolve caller-specific stream state, then compare
the affected writer context without replaying the unrelated cohort.
Private recovery evidence is in `.analysis/replay-writer-*`
and `.analysis/replay-deps-*`.

## Compression match search (00442910)

The 416-byte longest-prefix tree search is now maintained in
`src/ReplayCompressionSearch.cpp`. The search compares circular 8-KiB
window positions in explicitly unrolled groups of six until at most
18 bytes have been compared. It records the best matching source position,
walks the left/right children according to the signed byte difference,
inserts a new node when a child is missing, and calls the node-replacement
helper at `442B50` when the longest match reaches 18 bytes.

The direct target review establishes a contiguous 416-byte owner with
113 instructions, three RET4 exits, one node replacement call,
no interior foreign entry and no table. A conventional VC9 normal-COFF
source with a simple inner character loop produces 247 bytes. Restoring
the explicit six-comparison structure produces **416 bytes** and
270/340 matching relocation-masked diagnostic bytes with 19 fields.
This is **not exact**: the native optimized /GL ABI passes the position
in EDX and one output pointer on the stack, whereas the faithful
ordinary source declaration uses two stack arguments. Do not falsify
its ABI to make the normal COFF probe match. The next meaningful test is
a focused linked /GL compilation with the genuine compressor caller and
tree helpers once they have source.

## Dictionary tree update helpers (00442AB0..00442C10)

Four linked-tree operations are maintained together in
`src/ReplayCompressionTreeOps.cpp`. They implement recursive
node deletion, parent/child splicing (including the original zero-node
sentinel), full node replacement and rightmost-predecessor search.
Direct original Ghidra review and independent complete PE disassembly
establish all four boundaries and neighboring padding; the original
functions use privately optimized ECX/ESI/EDX/EAX argument registers.
Source code deliberately uses normal typed parameters instead.

| Operation | Original bytes | VC9 normal COFF | Address fields |
| --- | ---: | ---: | ---: |
| DeleteReplayCompressionNode | 79 | 95 | 9 |
| SpliceReplayCompressionNode | 80 | 97 | 8 |
| ReplaceReplayCompressionNode | 134 | 144 | 14 |
| FindReplayCompressionPredecessor | 49 | 51 | 3 |

Every function compiles with the pinned VC9 /O2 toolchain; every full
normal-COFF comparison is non-exact. The natural source follows the
native tree operations without inventing ABI parameters. These four
functions complete the source dependencies of the 416-byte search
function, but not yet the main 795-byte compressor. All candidate
objects and complete diagnostics remain ignored under `build/`
and `.analysis/`.

## Compression core (004423A0..004426BA)

The main encoder has 795 bytes, with 768 bytes of Ghidra-owned instruction
body spread across five ranges and 27 layout bytes. Independent Capstone
decodes 240 instructions over the complete span. Both incoming callers at
436782 and 437B98, all five native callees and adjacent function/padding
are reviewed. The new maintained source is in `src/ReplayCompressionEncoder.cpp`.

The loop preloads up to 18 literal bytes at ring index 1, seeds the 8192-node
root sentinel, and emits variable-width packed tokens: 1 bit and an 8-bit
literal, or 0 bit followed by 13 bits of match position and four bits of
match length minus three. The tree search uses an 18-byte match cap. As the
read cursor advances, it removes the outgoing window position, preserves
the tree and refills the circular buffer. End-of-stream emits a 0-bit
selector and 13 zeros; the original emits only completed output bytes,
without a forced partial-byte flush. A zero-length allocation retains
the original allocation-failure semantics; no new safety guards were added.

VC9 normal-COFF source trials (complete function owner, non-exact):

| Variant | Bytes | Address fields | Observation |
| --- | ---: | ---: | --- |
| Outgoing tree removal via helper | 571 | 11 | Too much native inline tree manipulation missing |
| Inline common deletion paths | 738 | 24 | Five native call targets represented; substantially closer size |

The exact source of the tree reset, the 416-byte match finder and the
four tree-edit helpers are maintained separately. Native optimized register
ABIs and /GL ownership remain open, so normal-COFF mismatch is expected;
these observations give no byte-exact credit.
