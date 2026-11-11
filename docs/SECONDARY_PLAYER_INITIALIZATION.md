# Secondary player initializer, 0x42F8A0 (1557 bytes)

The original Japanese TH11 1.00a initializer occupies the complete
`0x42F8A0..0x42FEB4` extent: 1557 bytes / **348 decoded
instructions**, with two RET4 exits and 13 native calls to nine callee
targets. The sole incoming call at `0x430206` belongs to
controller construction. Direct mapped-text Ghidra review and an
independent pinned-PE instruction decode confirm there is no owned
jump table, foreign interior entry or omitted alignment beyond the
complete span. CC separates the next function at `0x42FEC0`.

The maintained C++ implementation in
`src/SecondaryPlayerInitialization.cpp` reconstructs animation file
selection (`pl00.anm` or `pl01.anm`), failure logging, pattern table
loading/ownership, two 0x24-byte update/draw chain nodes with callbacks,
VM reset/script binding, four speeds converted to fixed-point units,
selected character hitbox dimensions, bounding vectors, and three
controller timers. It reuses the established
`SecondaryPlayerControllerView` and
`SecondaryPlayerMovementView` rather than inventing a second full
controller layout. Unknown external data symbols remain physical
observations, not asserted original source names.

Only the relevant owner and direct helper context were compiled:
the fixed VC9 normal-COFF candidate is **1492 bytes with 40 relocation
fields**. A local `/GL` link of the owner with its simple caller
produces **1479 bytes, 13 calls, 2 returns**. Adding six actual
source dependency TUs (chain registration, animation document, logging,
pattern dispatch, animation VM and player callbacks) produces a
**1520-byte** linked owner but changes the call/return count to
14/3. Both remain **non-exact**; the different register specialization
and producer TU arrangement are unsolved, and no complete runtime
behavior is claimed.

Evidence: `.analysis/pause-next-big-20261111-review.txt`,
`.analysis/player-initialize-native-linear.txt`,
`.analysis/player-initialize-main-coff-20261111.json`,
`.analysis/secondary-player-initialization-linked.log`, and
`.analysis/secondary-player-init-helpers-linked.log`.
Next useful work: reconstruct the 152-byte construction caller
at `0x4301B0` and determine whether it restores native prologue and
callee-specialized register ownership, without unrelated cold replay.

## Real factory caller, 0x4301B0 (152 bytes)

The reviewed source-visible caller at `0x4301B0..0x430247` has
**49 instructions, five calls and two plain returns**. It allocates
0x8D24 bytes, calls the separate constructed-storage owner at
42F760, invokes the 1557-byte initialization function, and
destroys/deallocates the player if initialization fails. The
native exception prologue preserves the unwind state around this
construction, and it has two known incoming calls.

The maintained implementation in `src/SecondaryPlayerFactory.cpp` now
constructs a genuine RAII storage object with a 0x8D24-byte physical
buffer, whose constructor invokes the 272-byte storage constructor
and whose destructor invokes the 415-byte cleanup routine. This
preserves C++ exceptional construction/deallocation and naturally
restores the original VC9 SEH/unwind stack frame, without assembly or
inert register/byte padding.

The first manual-allocation source compiled to just **67 bytes** and
linked to **66** in the narrow initializer/factory context; its native
SEH frame was missing. The RAII source now compiles to **150 bytes**
with seven relocation fields; a limited /GL link with the initializer
produces **149 bytes**, still five calls/two returns. A second, scoped
/GL link including the real constructor/destructor source yields a
**152-byte** complete PDB contribution, **identical length** to
native, with the native five-call/two-return shape. It is **not exact**:
native uses `SUB ESP,8; PUSH ESI` while the linked draft
uses `PUSH ECX; PUSH ESI; PUSH EDI`; the optimizer also
specializes helper call registers ESI/EDI, and **121 of 152 raw
bytes differ**. Only full independently reviewed native field replay
and zero-byte-difference comparison can confer exact credit.

Alternative natural source trials include a separate placement-new
raw pointer (156 normal COFF /144 limited linked) and equivalent
conditional delete forms (150 normal COFF), neither improving the
native stack lifetime. Source experiment records are in
`.analysis/player-factory-raii-*` and the scoped PE/MAP/PDB outputs
in `build/probe-ltcg/SecondaryPlayerInitializationFactory/` and
`build/probe-ltcg/SecondaryPlayerLifecycleFactory/`. The original
nominal class, EH table and TU partition are still unknown.
## Storage constructor and destructor, source-present

The original storage constructor at `42F760..42F86F` has 272 bytes,
71 instructions, five native calls and one RET4 with EH cleanup.
Its two embedded 0x434-byte VM objects are initialized through
`StageEmbeddedVm::StageEmbeddedVm`, an independently exact native
constructor at `402270`. The native vector-construction iterator
`4021A0` visits two **seven-element 8-byte-stride arrays** in each of eight emitter
records; the callback bodies `4021D0` and `433F00` are identity-only
constructors with no field writes. They now have independently tracked
cold normal-COFF exact source in `src/SecondaryPlayerEmitterPairConstructors.cpp`:
both **3/3 bytes**, zero relocations. Their tentative C++ type names
do not establish original class or translation-unit identity. The remaining work clears the
flags of 256, eight and 33 records, resets the 0x8D24-byte storage,
and publishes the singleton. Earlier maintained source only cleared the
emitter flags and compiled to **255 normal-COFF bytes / 236 linked bytes**,
with the two original array-construction calls absent. Updated source
in `src/SecondaryPlayerStorageLifecycle.cpp` now explicitly calls
`ConstructSecondaryPlayerElementArray` twice per eight-byte-stride
emitter pair, via ordinary typed C++ placement construction callbacks.
In a focused VC9 /GL context with the player factory, initializer and
exact three-byte element constructors, the complete constructor
produces **278 bytes / five calls / one RET4**, compared with
native **272 bytes / five calls / one RET4**. Its ordinary COFF
is now **271 bytes with ten relocation fields** (the earlier
separate placement-member source produced 287). This is a substantial
call-graph recovery, but still **non-exact** (different private
register allocation and EH/loop code generation).

The matched destructor-shaped native owner at
`430010..4301AE` occupies 415 bytes /103 instructions and makes
12 physical calls. Maintained source in the same TU reconstructs
both update/draw chain removals with optional runtime locks, conditional
animation document disposal or retention, pending pattern ownership
handoff, and the reverse-order destructors of two embedded VM members.
Its scoped lock and member destructors now naturally express the native
C++ EH and object lifetime. The **current normal COFF** is 389 bytes,
and a focused /GL link with real `StageLifecycle.cpp` code produces
**400 bytes, 12 calls and one return**, against original
**415 bytes, 12 calls and one return**. Earlier hand-coded unlock/free
source compiled to COFF371 and /GL357 with two returns.
The native ABI and all bytes still do **not** match.
Neither helper modifies the already exact 286-byte animation-pair reset.
Evidence: `.analysis/player-storage-lifecycle-review-20261111.txt`,
`.analysis/player-lifecycle-ConstructSecondaryPlayerStorage-coff-compare.json`,
`.analysis/player-lifecycle-DestructSecondaryPlayerStorage-coff-compare.json`,
`.analysis/secondary-player-lifecycle-factory-link.log`.


## Collision extent table correction and identity constructors (2026-11-11)

Original native instructions at `0042FAF2` and `0042FB50` both read the
**same** character-indexed table at `0x004A7AB0`: once to set the pattern
configuration's diagonal/extent field, then again to compute the second
collision-bounds half-size. Earlier source instead used the *focus*
table at `0x004A7AC0` for that second calculation. The maintained
initializer now uses `g_SecondaryPlayerHalfSizeZ[character]`, the same
abstract source table used by the first store. This is an actual
data-flow/behavior correction, **not a code-size match claim**. The
pinned VC9 normal COFF remains 1492 bytes with 40 fields; full
1557-byte original exactness is still open and the current unresolved
symbol names require independent address bindings.

The native generic array iterator at `0x4021A0` receives a count of
**7**, an 8-byte stride, and callbacks at `0x4021D0` (first array)
and `0x433F00` (second array). The two callbacks are not separately
identified as functions by default Ghidra analysis, but the original PE
contains distinct complete three-byte `MOV EAX, ECX; RET` bodies and
direct callback-pointer references at `0x42F7ED` / `0x42F801`. The
out-of-line empty C++ constructors in
`src/SecondaryPlayerEmitterPairConstructors.cpp` reproduce both
**3/3 bytes** in independently cold-built, strict VC9 normal-COFF
Oracle units with no relocations:

- `secondary-player-polar-pair-constructor` (physical `0x4021D0`)
- `secondary-player-offset-pair-constructor` (physical `0x433F00`)

These exact leaf proofs do not imply that the 272-byte containing
constructor or 1557-byte initialization main is exact.


## Vector-constructor codegen experiment

The native 272-byte storage constructor explicitly calls the
generic array constructor at `4021A0` twice per emitter, each
time with count 7, stride 8 and an identity-only callback. A
genuine C++ placement-new array expression for the two seven-element
types was compiled as a local experiment. It increased normal COFF
from 255 to **367 bytes**, but the whole-function /GL optimizer
eliminated the empty constructors and emitted only **215 bytes** with
three calls (native: 272 bytes and five calls), even when the
out-of-line callbacks were marked noinline. This is a codegen
regression, not evidence that native omitted those calls. The
maintained constructor's existing physical traversal is retained
until a realistic compilation context can recover the native
shared array-constructor helper. Local experiments:
`.analysis/player-storage-vector-ctor-v1-compare.json`,
`.analysis/player-storage-vector-variant-linked.log`,
`.analysis/player-storage-vector-noinline-linked.log`.


## Maximum-corner floating operand order (2026-11-11)

The original 1557-byte initializer contains **12 FADD instructions** to
build maximum corners for four overlapping three-component boxes, each
loading the extent before adding the position. In the previously
maintained code, VC9 generally loaded the position first. Reversing
the four natural `Plus` call argument orders in
`src/SecondaryPlayerInitialization.cpp` retains all geometry values
but restores **11 of the 12 native FADD operand sequences**, compared
with just one matching sequence before the edit. The residual FADD
is the X component of the final duplicate full maximum; it appears
subject to the optimizer's common-expression scheduling.

The focused original-toolchain `/GL` link produces the same
**1479/1557** bytes, **13/13 calls**, **2/2 returns**. Normal COFF
remains 1492 bytes, and the relocation-masked comparable-byte
agreement moves 58/1397 -> 60/1397. Alternate natural C++ attempts
(copy-then-add, a separate final vector, scalar assignments and
statement reorder) either introduce unnecessary stack copies or
reduce the FADD ordering match, so were not adopted. They are kept
only as ignored evidence in `.analysis/player-init-plus-*` and
`.analysis/player-init-swap-plus-args-linked.log`. The owner remains
**non-exact**; no broader cold exact cohort was rerun.


## Array construction loop ownership and codegen (2026-11-11)

The native generic iterator at `0x4021A0..0x4021C0` has been
independently reviewed via pinned hash-attested Ghidra and a full
**17-instruction / 33-byte** original PE decode. It is the only helper
called twice from the 272-byte player storage constructor:
the array receiver is in ECX, element count in EAX, callback in EBX
and stride as the single stack argument. It performs a count-minus-one
signed loop with an indirect callback call and returns via RET4.

Source is now in `src/SecondaryPlayerElementIterator.cpp` with a
shared declaration in `src/SecondaryPlayerElementIterator.hpp`.
The ordinary source-level signature is not a fabricated native
register ABI; it is lowered privately by VC9 /GL as compiler context
allows. Normal COFF generates **50 bytes**; with a generic dynamic
caller, a complete PDB-owned linked contribution is **33 bytes**,
with **29/33 literal bytes matching native**. The four differences
at offsets 8, 10, 11 and 13 correspond to the count/receiver
register assignment and the initial `ADD -1` versus native
`SUB 1`. When the real seven-element caller supplies constant
count and stride, /GL instead specializes the helper to
**31 bytes**. It is thus **source-present, not exact**.

The accepted storage-constructor source retains two genuine calls,
one per array, and all five native call-site occurrences. A
source-level countdown-loop experiment changes its three loops
to native-style JNS branches but grows the scoped 278-byte
contribution to 279 bytes. Replacing the two placement-constructed
VMs with a **non-inlined** physical aggregate reduced the linked
contribution to 246 bytes and lost a call; a header-local
`__forceinline` aggregate constructor instead retains both
VM constructor calls and the full five-call owner. A native-looking C++ array
placement-constructor experiment also collapses to a 215-byte
linked contribution after optimizer dead-code elimination. All
three alternatives were rejected as the maintained source.
Scoped artifacts:
`build/probe-ltcg/PlayerStorageWithIterator/`,
`.analysis/player-storage-loop-*-linked.log`,
`.analysis/player-storage-embedded-batch-linked.log` and
`.analysis/player-array-dual-context.log`.

The two exactly reconstructed three-byte element constructors now
share `src/SecondaryPlayerEmitterPairConstructors.hpp` with the
player storage caller, and both canonical cold-compiled units include
that header in their build receipts. This does **not** change their
exact machine code.


## Inlined physical embedded-VM members (2026-11-11)

The player object stores two 0x434-byte embedded VM objects at offsets
`0x14` and `0x448`. Rather than separately invoking placement-new
at those two raw addresses, the maintained storage constructor now
placement-constructs one physical `SecondaryPlayerEmbeddedVms`
prefix whose two real C++ member fields have those exact offsets.
Its short constructor is inlined by the ordinary VC9 optimizer,
while the two `StageEmbeddedVm` constructor calls remain separately
observable. This models C++ member lifetime without hardcoded native
instructions or fake private-register declarations.

That change improves the **normal COFF** body from 287 bytes /
27 of 232 relocation-masked comparable bytes to **271 bytes /
46 of 232**, against native 272 bytes. In a focused five-TU
VC9 `/GL` link, the complete primary PDB contribution remains
**278 bytes with five calls and one RET4**, versus native
**272 bytes, five calls and one RET4**; the 33-byte generic
array iterator still privately specializes to 31 bytes in
this context. All owner and return inventories were rerun after
the source edit. It remains **non-exact**, with native control-flow
and EH/stack allocation differences open.

The non-inlined aggregate experiment previously produced 246 bytes
with four calls: its C++ aggregate constructor remained a separate
helper, merging the two embedded VM calls into one parent call.
The inlined source version is intentionally distinct and supersedes
that rejected variant. Local evidence:
`.analysis/player-storage-embedded-forced-inline-coff.json`,
`.analysis/player-storage-embedded-forced-inline-linked.log`,
`.analysis/player-storage-inline-aggregate-current-compare.json`,
and `.analysis/player-storage-inline-aggregate-current-linked.log`.


## Player destructor EH and reversed VM ownership (2026-11-11)

The last two native releases in destructor `430010` are exactly
`owner+0x848` and `owner+0x414`, in that order. These are respectively
the `storage400` fields of the second and first `StageEmbeddedVm`
members declared at `owner+0x448` and `owner+0x14`. They correspond to
the real `StageEmbeddedVm::~StageEmbeddedVm` lifetime logic, not
independent anonymous heap fields. The maintained source now
destroys the whole physical `SecondaryPlayerEmbeddedVms` aggregate,
allowing C++ to automatically destroy the second member and then the
first. Compile-time offset assertions protect both releases.

The original also enters/leaves a critical section around the update
and draw chain removals, checking the runtime flag independently
before and after each unregister. The new source uses a normal
`ScopedPlayerChainLock` with the same two checks and C++ scope
cleanup, preserving exception unwinding. No artificial SEH prologue,
hardcoded machine code, or fake register ABI was inserted.

In the scoped VC9 /GL link **including the actual
`StageLifecycle.cpp`**, this changes destructor `430010/415` from
357 bytes / 12 calls / 2 returns to **400 bytes / 12 calls /
1 return**, matching the native call/return structure. The new
normal COFF has 389 bytes and 32 relocation fields (formerly
371/31). Linking without the VM destructor source instead gives
387 bytes / 11 calls / 1 return because the compiler emits a
specialized member-destruction tail; this narrower variant is
**not** treated as the native call graph. Full machine-code and
address-field equality remain open.

Trial results: the lock-only RAII version generated 454 linked bytes,
and implicit VM destruction without scoped locks generated 312 bytes
and lost the matching return path. Their combination was necessary
to recover the 400/12/1 owner. Evidence:
`.analysis/player-storage-dtor-lock-members-linked.log`,
`.analysis/player-storage-dtor-raii-final-coff.json` and
`.analysis/player-storage-dtor-raii-current-linked.log`.
The constructor and the already exact 286-byte animation reset retain
separate validation scopes.
