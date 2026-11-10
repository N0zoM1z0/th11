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

Historical Unicorn1.0.2 diagnostics executed each native body and its compiled
VC9 candidate with their respective ABIs and deterministic malloc/memcpy/free
shims. Each passed692 cases covering block sizes4..64, tail boundaries, buffers
up to1,024 bytes, limits and allocation failure. Buffers, return pointers, stack
cleanup and allocation/copy/free events agreed. Exact acceptance uses the
complete byte comparison; these execution cases are not a continuation suite.

These diagnostics cover the stated inputs and shimmed library calls. They do
not establish exact code generation, a complete native product, or completion
of the later semantic phase. The current candidates remain non-exact.
