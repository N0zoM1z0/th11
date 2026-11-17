# Secondary ECL debug owner lifecycle — TH11 v1.00a

The original Japanese executable has a debug-session object with two
chain pointers at +8/+C, an embedded runtime thread at +0x10, a
separately allocated filename table at +0x34 and signed filename
count at +0x38. The existing partial `SecondaryEclDebugView` in
`src/SecondaryGameplay.hpp` already models those original offsets.
All source changes use **natural C++** and the pinned VC9 /GL toolchain.

## Filename-array leaf (0x40CFE0, 83 bytes)

`ReleaseSecondaryDebugFilenames` in
`src/SecondaryDebugFilenameCleanup.cpp` now covers the entire
83-byte native leaf. If the table exists, it iterates its signed
count, frees each non-null filename and sets its slot to zero.
Afterward it separately checks, frees and clears the table pointer.
A null table has no side effects; a nonpositive signed count
skips the inner loop but not the table release.

Direct Ghidra review of the complete 40CFE0..40D032 target verified
two callers, one native free callee, no unrelated interior refs,
the terminal RET and following CC alignment. The current C++
implementation compiles under the pinned compiler, but original
private ESI register assignment and full linked-byte equivalence
remain **non-exact** until a real caller-context replay.

## Next ownership frontier

The neighboring 462-byte `0x40D320` destructor calls this leaf
before unregistering the two chains and destroying the subordinate
debug/session singletons in native order. Reconstruct its complete
body using existing real component destructors and the embedded
thread lifetime; never replace it with an ASM or padding shim.
