# Archive decompression

Direct Ghidra review covers the complete472-byte span
`0x004426C0..0x00442897`. The default sparse body omits eight internal
alignment bytes at0x00442708; the reviewed span includes them. Both return
paths, all four callers, the preceding return/CC padding and following CC
padding were checked. There are no external interior references or tables.

`src/ArchiveDecompress.cpp` maintains the full routine. Its current pinned
VC9 RTM `/TP /O2 /Ob1 /Gy /MT /Isrc /GL` primary contribution is473 bytes,
with177 unrelocated diagnostic differences. It receives no exact credit.
The source keeps ordinary stdcall; no custom register declaration is used.

## Four inputs, including allocation size

The observed linked convention receives output allocation size in EAX and
packed input pointer, signed packed length and destination on the stack;
both returns pop12 bytes. The size register is used only for `malloc` when
destination is null. The default Ghidra signature missed this fourth input.

Independent callers at0x00436C71,0x00437952 and0x004419F2 load EAX with an
unpacked size independently of the pushed packed length. Archive extraction
at0x00441865 likewise retains the entry's unpacked size in EAX. Its maintained
call now supplies that real argument. The public nominal argument order is
a source view; the observed private locations are independently reviewed.
`probes/archive-decompress-context.cpp` keeps both lengths independent in
two non-runnable calls.

## Bitstream and history

| Token | Encoding, most significant bit first | Action |
| --- | --- | --- |
| Literal | 1, then8 bits | Append byte |
| Copy | 0, then13-bit nonzero ring position, then4-bit length | Append length+3 bytes |
| End | 0, then13 zero bits | Finish and align bit mask |

The shared8192-byte history is rooted at0x004C0EE8. Each call starts its write
cursor at1, wraps with0x1FFF and retains existing storage. Copying is sequential,
so newly written bytes are visible to an overlapping reference. The ring
position is absolute, not a backward distance. No output-length guard exists;
output size controls allocation only.

The byte reader fetches at the current pointer before testing the signed
packed-length bound. At EOF it replaces that byte with zero and leaves the
pointer unchanged. Source preserves that order. An exhausted byte resets
the mask to0x80. Ending consumes only remaining bits of the cached byte.
Allocation failure returns null without reading the stream or changing history.

This is an LZSS-style format, but generic algorithm similarity and game
integration do not independently establish authored versus third-party origin.
Origin remains explicitly indeterminate.

## Bounded verification

A private single-CPU/nice10 Unicorn diagnostic executed the original complete
target body and the complete473-byte compiler contribution in62 cases. It
compared returns, allocation sizes, the full64KiB output region including
canaries, all8192 history bytes, callee-saved registers and stack cleanup.
Empty/literal streams, overlapping references, ring wrap,8300 literals,
allocation failure, zero/negative/truncated input lengths, independent output
allocation size and persistent history all agreed. This diagnostic has no
exact-codegen, complete-native-product or semantic-phase acceptance authority.

Private evidence: `.analysis/archive-decompress-span.txt`,
`archive-decompress-*-caller-registers.txt`, `archive-decompress-final-linked.json`,
`check-archive-decompress.py` and `archive-decompress-differential.json`.
Initial outlined/class/scalar helper contexts yielded277/407/456/473 bytes;
an output-reference helper yielded500 and changed the private convention.
Keeping the history in a separate normal object did not change473/177.
Do not repeat those probes or crop the target span.

Only the two canonical units whose header/call context changed are replayed:

```bash
scripts/repo-python scripts/replay-exact-units.py \
  --unit load-resource-file --unit find-archive-entry
```

Archive extraction, cipher, stream owners and full native resource integration
remain open. None receives new exact credit from this decoder work.
