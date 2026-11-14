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

## Status after the 2026-11-14 asm withdrawal

The writer (436420), segment allocator (437300) and compressor (4423A0) had
been credited exact from naked `__asm` copies of the target. That is not a
reconstruction and the credit is withdrawn. All three are natural C++ again.

The writer follows native statement order (446/564 aligned instructions,
1877/1856 bytes). Native re-reads `state->header`, `state->stages[i]` and
`node->segment` in the sizing loop, but keeps `chunk` locals in the copy loops.
The end marker uses the recorder's three-store event helper. The timing doubles
sit at +0x24/+0x2C (4-byte packing), the write check is `size != written`, and
the function returns 0. The allocator gets native's EBX/EDI convention only when
no inline-asm caller remains in the link; in the writer unit it is 38/40
aligned instructions, differing only in the segment constructor's store
schedule.

## LZSS compressor (004423A0..00442C10)

The compressor is Okumura-style LZSS: an 8-KiB ring, an 18-byte lookahead and
an 8193-node binary tree `{parent, left, right}`. Natural C++ now reproduces
the tree code exactly; each function is a cold `/GL` primary-TU proof with all
relocated fields reviewed:

| Owner | Bytes | Unit | Source shape that matters |
| --- | ---: | --- | --- |
| `FindReplayCompressionMatch` 442910 | 416 | `replay-compression-match-search` | plain `for (length < 18)` loop, which VC9 unrolls by six; `bestLength = length` before `*matchPosition = candidate` |
| `DeleteReplayCompressionNode` 442AB0 | 79 | `replay-compression-delete-private` | `__fastcall` (native ECX, plain RET); recursive |
| `SpliceReplayCompressionNode` 442B00 | 80 | `replay-compression-splice-private` | ordinary function; LTCG picks ESI/EDX |
| `ReplaceReplayCompressionNode` 442B50 | 134 | `replay-compression-replace-private` | whole-node copy `nodes[new] = nodes[old]` |
| `FindReplayCompressionPredecessor` 442BE0 | 49 | `replay-compression-predecessor` | rightmost node of the left subtree |

All of them index `g_ReplayCompressionNodes[...]` directly. Going through a
local `tree` pointer, as the earlier C++ did, changes the addressing and
register choice in every helper. Separate field-wise copies in Replace give
23/30 aligned instructions instead of the struct copy's 30/30.

The encoder is natural C++ but not exact yet (795/795 bytes, 166/240
aligned instructions). Native code shows how the source is shaped:

- Input is read through a `getc`-style expression,
  `cursor - input < size ? *cursor++ : -1`, followed by a test against -1.
  VC9 rebuilds the consumed count as `cursor - input` before the inner loop.
- The tree unlink is written out in the encoder, Okumura style: pick the
  replacement with `if (!right) ... else if (!left) ... else`, where the
  two-child branch walks to the predecessor, calls `Delete` and `Replace`,
  and skips the shared splice. Native `Delete`, `Splice` and `Predecessor` use
  custom register conventions, so they are not `inline` functions.
- The bit buffer is an `int` in EBX and the mask an `unsigned char` on the
  stack. The bit selectors are unsigned (`shr`).
- The preload loop has its own counter that is copied into `length`
  (native copies EAX into ECX and a stack slot); this gives 150/240. The inner
  loop counts down from `matchLength` (`for (i = matchLength; i > 0; --i)`)
  rather than through an `advance` copy, which reaches the native size.
- Declaration order of the bit state and match variables, a `do`/`while`
  outer loop, and initialising the match variables after the root setup
  made no difference.

The remaining encoder gap is register allocation. Native keeps `matchLength`
in ESI and spills `length` to the stack (it reuses ESI as the constant 1 in
the inner loop), and the candidate does the opposite.

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
