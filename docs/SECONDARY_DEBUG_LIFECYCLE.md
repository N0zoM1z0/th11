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


## HUD destructor precleanup — 468-byte native owner

`src/SecondaryDebugHudPrecleanup.cpp` restores
`PrepareSecondaryDebugHudDestruction` at 0x41AD30..0x41AF03,
including the original 468-byte function boundary and all six native
call targets. The source uses the same 0x4450-byte physical HUD
object as the 729-byte destructor, with typed member offsets in
`src/SecondaryDebugHudLifetime.hpp`.

The precleanup first releases the renderer's real animation document
slot 27 if stage flags mask 9 is clear, or marks the retained
animation file's VMs otherwise. It clears the retained file pointer,
destroys/deletes the dialogue, and either frees the prepared HUD
cache or publishes the existing pointer at global 4C3238. It then
clears the update-chain pause bit, releases the specific VM IDs at
+4370, +4374 and +4380, zeroes all eleven additional VM slots at
+4380..+43A8, and releases eight tracked VM IDs through the real
renderer ID lists. Finally, it sets HUD state bits 0x1C0 and clears
the +43FC field, retaining the original cleanup sequence.

The original Japanese TH11 1.00a body, its one caller at 41AFC0,
six callees and return/neighbor bytes were reviewed in Ghidra,
without foreign interior entry points. The scoped VC9 /GL
candidate is 445 bytes with 62/128 normalized native instructions
aligned; this is **source coverage, not exactness**. The separate
dialogue destructor is independently strict exact (101/101), as
recorded above; that fact does not imply this larger caller is exact.

## Owning HUD destructor source-order refinement

The 729-byte native `SecondaryDebugHudOwned` destructor at
0x41AFC0 remains an independent nonexact owner. Its source now
uses a typed pointer interval for the eight animation IDs, and
preserves the **next** VM-list node before inspecting and marking
the current one. The latter is important if an affected callback
changes a list while the current VM is marked; it also reflects
the original decompiler's explicit snapshot on both renderer lists.

In the bounded VC9 /GL context containing the newly reconstructed
468-byte `PrepareSecondaryDebugHudDestruction` and the already
**101/101 byte exact** dialogue leaf, the destructor's natural C++
PDB contribution is **772 bytes**, with **113/195 native instruction
keys** aligned (the initial simpler source aligned 92/195).
This diagnostic does not prove linked byte equality and does not
increase canonical exact credit. The outstanding differences
involve the original private ESI/EDI calling convention, the
three registration-removal callsites, and register/loop scheduling
around the two renderer lists and genuine 9/2/4 VM arrays.
No ASM, forced instruction sequence or padding was inserted.

## Sixteenth embedded HUD VM and actual 0x4458-byte allocation (2026-11-17)

The native 0x41B2A0 HUD factory allocates **0x4458 bytes** (the
`push 0x4458` before operator new at 41B2C2), not 0x4450 as the
early destructor-only source view assumed. Its complete 729-byte
destructor at 41AFC0 also frees a VM storage field at +0x431C
*between* clearing the HUD singleton and destructing its existing
4/2/9 VM arrays. Those bytes are at +0x400 inside a previously
unnamed sixteenth `StageEmbeddedVm` starting at +0x3F1C.

`SecondaryDebugHudOwned` now contains the real named
`persistentVm` member at +0x3F1C and retains the original full
0x4458-byte allocation. Ordinary C++ destruction invokes this
member's real destructor in reverse declaration order, before the
4/2/9 VM arrays, rather than manually freeing its +0x400 storage.
The last eight bytes are left physically unknown. Both source
offset/size assertions compile under the pinned VC9 toolchain.

In the bounded `secondary-debug-hud-destructor-frontier`, this
semantic source fix preserves the current 772-byte candidate and
raises normalized instruction alignment to **114/195** from
113/195. It is still nonexact; its source class layout and actual
destruction sequence are now better supported by the native
factory and destructor. No assembly, naked function or padding
has been inserted.


## Strict exact owning HUD constructor (0x41A2F0, 208 bytes)

A direct reconstruction of the original 208-byte constructor now lives
in `src/SecondaryDebugHudConstruction.cpp`. The class physical size is
**0x4458 bytes**, not the older 0x4450 prefix. It owns genuine C++
`StageEmbeddedVm` arrays of **9, 2 and 4** at +10, +25E4
and +2E4C, plus a **sixteenth standalone** `StageEmbeddedVm` at
+3F1C. The latter's real storage pointer is +431C; the C++
destructor owns its release without an extra hand-coded `free`.
Two genuine `StageTimerView` members start at +43C8 and +4424;
their actual `flags` fields at +43D8 and +4434 have bit0 cleared
by the ordinary timer constructors before the game deliberately resets
*all* 0x4458 bytes, sets owner flag bit1 and publishes the singleton
at 4A8D84. The previously anonymous four-byte flag subobjects are now
recovered as full twenty-byte timer members; no duplicate storage is
introduced and the physical offsets remain the same.

The focused VC9 /GL context generates **208/208 native bytes with
53/53 x86 instructions**. All **14 native relocation fields** are
bound to the actual compiler-generated EH handler, security cookie,
three real VM-array constructor iterators, the standalone VM ctor,
VM array destructor callback, `memset`, and the singleton.
Selected strict cold linked-PE replay reports **zero byte
differences**. The handler's original CIL MAP static owner is
authenticated exactly, not substituted by a dummy or hidden field.
The constructor is therefore canonical byte-exact; the external
EH handler body and 729-byte owning HUD destructor are separately
scoped code owners. No ASM or executable padding was used.


## Exact owning HUD factory (0x41B2A0, 150 bytes)

The factory in `src/SecondaryDebugHudFactory.cpp` naturally
allocates a genuine 0x4458-byte `SecondaryDebugHudOwned`,
calls its separate 208-byte C++ constructor, and invokes the
independently bounded 339-byte registration function. Nonzero
registration result destroys and deletes the owner before
returning null; successful construction returns the real object.

The focused VC9 /GL unit `secondary-debug-hud-factory-frontier`
must include the **real constructor and destructor** sources,
rather than exposing them as opaque external signatures. With
those ordinary C++ owner bodies visible, the private stack
argument convention and compiler EH scheduling match the
original. Its primary PDB contribution is **150 bytes and all
49 original instructions align**. The selected strict cold
linked-PE replay proves **150/150 bytes with zero differences**,
checking all seven address fields: owner-specific static EH
handler, security cookie, operator new, constructor, registrar,
destructor and operator delete. The handler's identity is
verified against the real compiler MAP Static symbols section,
not guessed or masked.

This public factory body is now canonical exact. Its external
C++ EH-handler *code*, the 339-byte registrar and the 729-byte
destructor remain separately bounded owners; their exactness
must not be inferred from this success. No hand-authored machine
code, ASM or padding was added.

## Full HUD registration owner (0x41A3C0, 339 bytes)

The native 339-byte `RegisterSecondaryHudChains` now has natural
C++ in `src/SecondaryDebugHudRegistration.cpp`. Its physical
receiver is the existing `SecondaryDebugHudOwned` singleton,
whose 0x4458-byte allocation and 208-byte constructor are already
strict exact.

The registrar acquires the *actual* `front.anm` animation document
(index 5), stores it at owner+0x444C, and on failure writes the
original resource error through `AppendLog` and returns -1. A
successful file load calls the independent 336-byte
`InitializeSecondaryDebugHudVisuals` before installing any chain
callbacks; a nonzero return likewise prevents registration.
The native creates **three distinct 0x24-byte chain nodes**, assigns
callbacks at 41CFB0, 41CFC0 and 41CFD0, registers them with
update priority **24**, draw priority **57**, and overlay draw
priority **42**, and stores them at HUD+8,+C,+43DC respectively.
No allocation-error shortcut was added.

Ghidra verified the complete 339-byte body, six callees, unique
factory caller, terminal RET4 and absence of foreign interior
references. The scoped `secondary-debug-hud-registration-frontier`
compiles a real dynamic owning factory and real chain source under
pinned VC9 /GL, yielding a **328-byte candidate with 104 of 112
normalized native instructions aligned** in the current live
context. The critical remaining differences are compiler-private
`AcquireAnimationDocument` (native filename in EBX, index in ECX),
`AppendLog` (native ECX log receiver), and a small prologue/epilogue
register lifetime choice. None of these is masked by padding or
ASM. This owner is **source-complete, not yet byte-exact**.

### Physically typed HUD timers (2026-11-17 continuation)

The independent native HUD visual initializer at 0x41A520 uses the
first timer at +43C8 and tests its flags at +43D8; the existing HUD
updates read its frame and time-scale pointer at +43D4. The transition
timer lives at +4424, with the second flag bit at +4434.
`SecondaryDebugHudOwned` now declares both as real
`StageTimerView` values, enabling normal C++ initialization and
future `SetFrame` calls instead of representing only their flag
words. Both the **208-byte constructor** and **150-byte factory**
continue to pass their own selected strict cold VC9 /GL replay
with zero differences after this type refinement. The exact
machine code is unchanged; the source's physical semantics are better.
