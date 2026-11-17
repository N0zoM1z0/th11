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
the terminal RET and following CC alignment. The bounded real-caller VC9 /GL producer now generates
**all 83 original bytes exactly**. The selected cold strict
linked-PE replay validates both real CRT `free` calls at +1C/+3D,
all other owner bytes, the PDB section contribution and current
source/tool receipts, with **zero differences**. The recovered local
`char *filename` value preserves native load/test/free/slot-zero
scheduling: a direct repeated indexed expression instead generated
87 bytes. This is a genuine natural C++ improvement, not ASM or
padding. The leaf is now canonical exact under unit
`release-secondary-debug-filenames`.

## Next ownership frontier

The neighboring 462-byte `0x40D320` destructor calls this leaf
before unregistering the two chains and destroying the subordinate
debug/session singletons in native order. Reconstruct its complete
body using existing real component destructors and the embedded
thread lifetime; never replace it with an ASM or padding shim.

## Full secondary debug-session destructor (0x40D320, 462 bytes)

`src/SecondaryDebugSessionLifecycle.cpp` now owns the complete
462-byte original destructor through a genuine natural C++
`SecondaryEclDebugOwner` class. Its 0x710-byte physical layout
retains the same fields already used by the non-owning
`SecondaryEclDebugView`: update/draw chains at +8/+C,
embedded thread at +10, filenames/count at +34/+38 and
owned heap data at +6C8. The owning thread member has a **real
virtual C++ destructor**, so compiler-generated destruction writes
its genuine vtable pointer and calls the existing
`StopRuntimeThread` rather than hardcoding the original vtable
address or emulating its instructions.

Native teardown order is preserved:

1. Release every allocated filename via the separate 83-byte
   `ReleaseSecondaryDebugFilenames` owner.
2. Unregister update and draw chains separately, retaining the
   conditional runtime critical-section enter/leave operations
   and their independent post-callback flag checks.
3. Destroy and delete the existing HUD, player storage, scene
   projectile pool, enemy master, auxiliary stage, attraction
   items and replay browser **in that order**. Each owner calls
   its genuine separately bounded original destructor before
   `operator delete`.
4. Clear the debug-session singleton at 4A8D70, free and null
   its +6C8 heap allocation, and let ordinary C++ member
   destruction stop the embedded thread.

There are **no invented second singleton globals** and no
naked/ASM machine-code wrappers. The original 462-byte span,
14 callee sites, SEH ownership, boundaries and neighboring
alignment were reviewed directly. The focused real-caller
`secondary-debug-session-destructor-frontier` emits a
**460-byte candidate with 118/127 normalized native instruction
keys aligned**. Remaining differences are native private
ECX/EDX chain-removal calls, a stage-auxiliary destructor
calling-convention difference, and the compiler's member
destruction/EH scheduling. It is source-complete but **nonexact**.
