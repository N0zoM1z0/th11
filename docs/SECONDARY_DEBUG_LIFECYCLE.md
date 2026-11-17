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
**464-byte candidate with 124/127 normalized native instruction
keys aligned** after adding real `ChainRemove`,
`SecondaryMasterDestruction`, `StageAuxiliaryLifecycle`
and `SecondaryDebugHudLifetime` producer inputs. The native
private call registers are recovered in these contexts without
ASM or special-purpose call shims. Remaining differences involve
the compiler's initial EH state and final virtual thread member
destructor scheduling. Linking the entire `FrameRuntime.cpp`
or changing the owned heap pointer to volatile made the candidate
less faithful and was rejected. The large owner is
source-complete but **nonexact**.

## Full 729-byte debug HUD destructor at 0x41AFC0

The original HUD singleton at 0x4A8D84 now has a **natural C++ owning
type**, `SecondaryDebugHudOwned`, implemented in
`src/SecondaryDebugHudLifetime.cpp` and its companion header. It
shares physical storage with the existing non-owning
`SecondaryGameplayHudView` and `SecondaryActorHudView`, rather than
creating another singleton.

This is a large, genuine C++ destructor. Its three **real**
`StageEmbeddedVm` member arrays are 9 elements at +0x10,
2 elements at +0x25E4, and 4 elements at +0x2E4C (each
VM is 0x434 bytes). The compiler automatically frees these
arrays in the original reverse order **4, 2, 9** after the
destructor body; no EH vector-iterator assembly or fake
padding is authored.

Before member destruction the native owner calls the separately
bounded 0x41AD30 HUD precleanup, independently unregisters
update, draw, and overlay chains (+8/+C/+43DC), clears +8,
releases two animation IDs at +43AC/+43B8 and zeros them,
then visits the eight renderer ID slots at +4350..+436C.
For each live ID the source searches both genuine renderer
lists, marks its VM and eligible related children for cleanup
using flag 0x04000000, and clears that ID. It then walks both
renderer VM lists to mark entries whose +0x3B0 group field
matches the owner marker at +0x444C. Finally it clears
`g_SecondaryActorHud`, frees optional +0x431C data and
nulls the pointer.

Direct Ghidra and native PE review support the entire **729-byte**
owner, seven actual callees, all returns and boundaries.
A dedicated real-caller VC9 /GL scoped compilation produces a
**740-byte candidate with 92/195 native instruction keys aligned**:
source-complete but not byte-exact. The native EH/register
ownership, especially the 0x41AD30 precleanup and three
C++ member-array iterator calls, remains to be optimized.
The adjacent 462-byte debug-session destructor now calls
`delete` on this **actual owning class**, not a placeholder
external destructor; its own codegen alignment remains 124/127
in the focused context containing the real chain/master,
auxiliary stage and HUD destructors.


## Exact attraction-item pool reset (0x40D040, 25 bytes)

`src/SecondaryDebugItemReset.cpp` now restores the complete native
`ResetSecondaryGameplayItems` body with an ordinary
`memset(g_SecondaryAttractionItems + 0x14, 0, 0x265E50)` call.
This retains the existing pool's original header and clears the
contiguous item data without changing the object layout.
The source does not contain hand-coded machine bytes or padding.

Selected cold pinned VC9 RTM normal-COFF replay confirms the entire
**25/25 bytes** match original TH11 1.00a. The original DIR32
global-pointer field at +01 resolves to 0x004A8E90 and REL32 at
+11 resolves to CRT memset at 0x0046A720. Both fields are
independently reviewed, with zero unmatched bytes. The exact
unit is `reset-secondary-gameplay-items`; nominal original
class/translation-unit attribution is still indeterminate.


## Player-chain pause and emitter reset (0x40D190, 31 bytes)

`src/SecondaryDebugPlayerPause.cpp` reconstructs the complete native
`PauseSecondaryGameplayPlayer` function with ordinary C++. It captures
the *existing* player singleton at 4A8EB4, sets bit1 in each player
update/draw chain element (+8/+C of that player owner), and calls the
separately maintained **2,508-byte** `ResetSecondaryPlayerEmitters`
at 432CC0, without adding null guards or substitute global data.

The original routine is 31 bytes and 10 instructions; two original
callers and its one emitter-reset callee were reviewed. The pinned VC9
/GL diagnostic `pause-secondary-gameplay-player-frontier` emits exactly
31 bytes with the same temporary/or/call structure, but selects **EDI**
instead of the original preserved **EBX**. Four source-shape trials and
four bounded real-callee context variants did not solve that original
private register assignment. The function is therefore **source-complete
but nonexact**; no fabricated C++ register constraint, ASM or padding
was inserted.


## Exact dialogue animation cleanup (0x419F80, 101 bytes)

The 101-byte native owner now has fully maintained natural C++ in
`src/SecondaryDebugHudDialogue.cpp`. Eight distinct
`AnimationVmId` member references correspond to the eight fields
at dialogue+0x40..0x5C. Each field is passed by value to the real
`ReleaseSecondaryAnimation` and is immediately set to zero.
The dialogue class identity is a physical compatible view, not an
assertion about the original C++ class name.

The selected native VC9 /GL strict cold replay reports **101/101
bytes exact** and 0 differences. The eight reviewed REL32 fields at
+06,+14,+20,+2C,+38,+44,+50,+5C all bind to the actual original
animation release at 0x4562E0. Declaring eight typed value references
causes the compiler to emit all 36 native instructions in order;
direct unsigned indexing only aligned 18/36 and a loop was far
shorter. No user-authored ASM, jump bytes, padding or NOPs were added.
The companion 468-byte HUD precleanup and 729-byte owning destructor
remain separate, nonexact owners.
