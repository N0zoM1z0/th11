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
and publishes the singleton. Maintained source in
`src/SecondaryPlayerStorageLifecycle.cpp` compiles to 255 bytes
in normal COFF and 236 bytes in a local factory /GL link, non-exact.

The matched destructor-shaped native owner at
`430010..4301AE` occupies 415 bytes /103 instructions and makes
12 physical calls. Maintained source in the same TU reconstructs
both update/draw chain removals with optional runtime locks, conditional
animation document disposal or retention, pending pattern ownership
handoff, and two backing-buffer frees. Normal COFF produces 371 bytes
and factory-scoped /GL produces 357 bytes and 12 calls, but two
synthetic returns instead of the native single RET4; it is non-exact.
The native C++ EH and original class lifecycle are still unresolved.
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
