# Paired byte transforms

Maintained source covers two complete medium functions:

| Source name | Target span | Current VC9 source extent |
| --- | --- | --- |
| ScatterXorBlocks | 0x004581C0..0x004582DB, 284 bytes | 276 bytes; non-exact |
| GatherXorBlocks | 0x004582E0..0x004583FB, 284 bytes | 276 bytes; non-exact |

Direct Ghidra review found no external interior entries. Both return the
input buffer in EAX and pop five stack arguments with RET 20; the initial key
enters in AL. The source keeps a conventional six-argument stdcall view until
the special optimizer ABI and linked context are recovered. It has no exact
unit. Code origin is also unresolved.

Scatter's Ghidra body has 280 addresses within its 284-byte span. The four
remaining bytes at 0x0045824C are an internal `LEA ESP,[ESP]` alignment
instruction. The complete span, loop edges, external calls, RET tails, and
following CC padding were reviewed. Gather has one contiguous 284-byte body.

Both functions allocate and copy `min(size, limit)` bytes. They reduce the
working size by its odd byte and, when `size % blockSize < blockSize / 4`,
also by that short remainder. For each processed block, Scatter writes
sequential temporary bytes into every other output position backwards, then
the other parity backwards. Gather reads that interleaving into sequential
output. Each byte is XORed with the current key, then the byte key increases
by the step with wraparound. The key continues across blocks. Allocation
failure leaves the input unchanged and returns its pointer.

The target adds no argument validation. A positive block size and enough
copied bytes for every processed block are caller requirements. The source
preserves the observed arithmetic and limit handling.

Focused private execution diagnostics used Unicorn 1.0.2 to execute each
original body and its freshly compiled VC9 candidate with their respective
ABIs. The target malloc, memcpy and free entries and the candidate's COFF
calls used the same deterministic shims. Each function passed 692 cases:
block sizes 4, 5, 7, 8, 16, 32, 64; empty/small/odd/tail-boundary and larger
buffers up to 1,024 bytes; zero, one-block, two-block and full limits;
successful and failed allocation. Buffers, return pointers, ABI stack
cleanup, and allocation/copy/free events agreed; guard bytes stayed intact.
The recorded results remain in `.analysis/byte-cipher-behavior.json`; the
retired execution driver is removed. These cases are historical observations,
not a required continuation suite.

These diagnostics cover the stated inputs and shimmed library calls. They do
not establish exact code generation, a complete native product, or completion
of the later semantic phase. The current candidates remain non-exact.
