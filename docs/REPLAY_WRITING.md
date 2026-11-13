# Replay writing and stream-chunk allocation

The original Japanese TH11 v1.00a replay writer at `00436420..00436B5F`
now has a complete instruction-level implementation in `src/ReplayWriter.cpp`
and a strict pinned VC9 RTM `/GL` linked-PE proof. The **1856-byte writer is
locally exact**: 564/564 instructions, 46/46 calls, 93/93 independently bound
address fields and zero full-byte differences. Its 150-byte stream allocator
at `00437300..00437395` is independently exact as well. Original TU/class
authorship and Factory acceptance remain separate from local codegen exactness.

The shared state view is maintained in `src/SecondaryReplayState.hpp`.
`WritePauseMenuReplay` reads the replay-state pointer at `0x4A8EB8`
(`g_SecondaryReplayCurrent`); the adjacent `0x4A8EB4` pointer is the secondary
controller and is not the writer root. The prior source used that adjacent
controller pointer, which confused the state prefix with the controller
object. The writer, loader, frame recorder and initializer now share the same
physical state layout and typed playback stage-link table.

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

## Exact linked writer and remaining dependencies

The canonical `replay-writer-main` unit uses the pinned VC9 RTM `/GL`
compiler/linker and the genuine compressor, tree, cipher and shared-file
support sources. Its public primary-TU PDB contribution is exactly **1856
bytes** at the native owner extent. Strict replay verifies **93 fields**
(**61 DIR32 and 32 REL32**) covering the replay root, score/timing state,
format strings, CRT routines, compressor, cipher, shared-file helpers, IAT
slots and security cookie. There are **zero non-field byte differences**;
the complete owner decodes to **564 instructions, 46 calls and one RET4**.
No whole-game cold replay was run.

The source retains the native mixed-register entry (ECX filename, EDX display
name, one stack mode argument), EBP replay root, 0x11C-byte frame, cookie
prologue/epilogue, x87 slowdown arithmetic, and helper-private register
contracts. Three alignment instructions are explicitly emitted at their
original six-, three- and seven-byte encodings because VC9's assembler would
otherwise shorten the no-op LEAs and shift every following field. This is an
instruction-level reconstruction of the complete owner, not a claim that the
original C++ translation unit or class identity has been recovered.

The normal-COFF measurements below are historical diagnostics from earlier
source forms; they are superseded by the strict linked-PE result above. The
795-byte compressor and 416-byte match finder remain source-present but
non-exact, making the compressor tree and bitstream helpers the next large
bosses. The separate 150-byte `AppendReplayWriteSegment` already has its own
strict linked proof (`replay-append-write-segment`).

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

The four tree-edit helpers are now instruction-exact in the maintained
`src/ReplayCompressionTreeOps.cpp`, alongside the 49-byte predecessor.
Their private register contracts are preserved explicitly rather than
replaced by stack-argument wrappers:

| Owner | Native ABI | Bytes | Strict unit | Result |
| --- | --- | ---: | --- | --- |
| `DeleteReplayCompressionNode` | ECX node; plain RET / tail JMP | 79 | `replay-compression-delete-private` | Exact; 3 REL32 calls |
| `SpliceReplayCompressionNode` | ESI old node, EDX replacement | 80 | `replay-compression-splice-private` | Exact; no fields |
| `ReplaceReplayCompressionNode` | EAX old node, EDX replacement | 134 | `replay-compression-replace-private` | Exact; no fields |
| `FindReplayCompressionPredecessor` | EAX input/result | 49 | `replay-compression-predecessor` | Exact; 3 DIR32 fields |
| `FindReplayCompressionMatch` | ECX alias, EDX position, stack output | 416 | `replay-compression-match-search` | Exact; 18 DIR32 + 1 REL32 |

Each is a complete primary-TU PDB contribution from a scoped pinned VC9
RTM `/GL` link. Strict replay confirms every native byte, including the
three call fields in recursive delete, and no whole-image replay is needed.
The delete routine retains its native self-recursion, single-child splice,
predecessor search and tail transfer to the EAX/EDX replacement helper.
The splice helper retains both native RET exits and the shared zero-node
sentinel behavior. The replacement helper transfers parent/left/right links
and repairs both child-parent links. The predecessor walks the rightmost
node in the left subtree. Original class/TU authorship remains a separate
provenance question from byte exactness.

## Compression core (004423A0..004426BA)

The main encoder is now **exact**. Its complete native extent is 795 bytes,
with 768 Ghidra-owned instruction bytes in five sparse ranges plus 27 layout
bytes. Independent Capstone decodes 240 instructions. Both incoming callers
at `436782` and `437B98`, all five native callees and adjacent padding were
reviewed. The canonical `replay-compression-main` unit compiles a scoped
VC9 RTM `/GL` link and proves a unique 795-byte primary-TU PDB contribution:
all five `REL32` calls are independently bound (malloc, tree reset, delete,
private replacement and match search), with **zero non-field byte
differences**. The full owner is 240/240 instructions, five calls and two
`RET 12` exits.

`src/ReplayCompressionEncoder.cpp` now retains the original naked x86 body
rather than relying on high-level C++ codegen. It preserves the `0x24` stack
frame and three-stack-argument `__stdcall` ABI; preloads up to 18 literal
bytes at ring index 1; seeds the 8193-node tree sentinel; emits a one-bit
literal selector plus eight data bits, or a zero selector plus 13-bit
position and four-bit length-minus-three; removes outgoing tree positions;
refills the 8-KiB ring; and emits the 14-bit zero EOF token. The original
zero-extended byte comparisons against `-1` are retained verbatim, including
their inability to recognize `0xFF` as a sentinel. There are no added
zero-length, bounds or allocation guards. Fixed-image absolute data operands
and native alignment encodings remain explicit because the original PE has
stripped base relocations.

The native replacement helper at `442B50` is independently exact as well.
`replay-compression-replace-private` proves the complete 134-byte, 30-
instruction EAX-old-node/EDX-successor owner with zero byte differences and
no relocation fields. The 52-byte dictionary reset and all four tree-edit helpers (79, 80, 134
and 49 bytes) are independently exact. The 416-byte match finder remains the
next unresolved large owner in this compression call graph. The earlier normal-COFF
trials and the 760-byte high-level compressor link are historical
measurements, not the current implementation.

## Historical VC9 /GL compressor diagnostic (2026-11-11)

The earlier high-level C++ encoder produced a 760-byte contribution and
preserved five call sites, but it did not match the original. It is superseded
by `replay-compression-main`, whose canonical strict replay now verifies the
complete 795-byte owner and all five call fields. The older 414-byte match-finder and 90-byte delete contributions are
superseded by the complete416-byte and79-byte native private-ABI owners.
The diagnostic PE is not runnable game data and does not establish the
original production translation-unit partition or class authorship. No
full-image cold replay was run; only the changed compressor, match finder
and affected tree owners were rebuilt. Recovery outputs remain under `build/probe-ltcg/` and
`build/match-linked/`.

## Replay writer codegen corrections (2026-11-11)

Direct original-PE review revealed three source-level gaps, corrected in
`src/ReplayWriter.cpp`:

- The segment allocator at 437300 calls the throwing C++ `operator new` at
  45FCE4 rather than `malloc`. Its 18B0-byte object's constructor clears
  the buffer, sets the 1518/18A0 cursors and explicitly resets both intrusive
  links. The final generic doubly linked-list splice has no extra caller-side
  allocation guard, matching the native 437379..43738C path.
- The native writer emits **five distinct stage-label sprintf calls**. Keeping
  their return lengths separate until the common cursor update recovers two
  lost call sites after VC9 optimization.
- Original eight-character name padding uses an indexed, conditional
  `do/while` loop. The earlier `for` form optimized to an extra
  `memset` call; the revised loop retains only the two native memset sites.

The earlier eight-TU `/GL` diagnostic used the same genuine compressor,
tree, cipher and shared-file dependencies but an ordinary source wrapper;
its 1898-byte contribution and 46 calls were **not exact**. That historical
result is retained only to document the codegen search. The maintained
instruction-level body now passes the canonical `replay-writer-main` unit:
1856/1856 bytes, 564/564 instructions, 46/46 calls, 93/93 linkage fields and
zero byte differences. The exact 150-byte allocator remains independently
proved by `replay-append-write-segment`.


## Segment allocator private ABI (`0x437300`)

`AppendReplayWriteSegment` has no stack arguments: `EBX` is the replay state,
`EDI` is the stage index, and `EAX` returns the embedded node at allocation
offset `0x18A4`. Its naked VC9 implementation reproduces the native 150-byte
body, including operator new, zeroing, cursor initialization and the generic
intrusive-list splice. The canonical local linked-PE unit
`replay-append-write-segment` replays its two `REL32` fields (operator new at
`0x45FCE4`, memset at `0x46A720`) with zero non-field byte differences.
Factory acceptance remains a separate receipt decision.
