# Auxiliary stage instance, chains and lifetime

The Japanese TH11 v1.00a target has a separate 0x4A0-byte auxiliary stage
instance, published at global 0x004A8D64. It is **not** the primary 0x3158-byte
stage. The complete native bodies and references were inspected with direct
Ghidra, including the two callback targets missing from its provisional
function inventory.

| Native owner | Bytes | Reconstructed C++ |
| --- | ---: | --- |
| 4061B0 | 173 | RegisterStageAuxiliaryChains |
| 406260 | 477 | StageAuxiliaryView destructor |
| 406440 | 105 | CreateStageAuxiliary |
| 4064D0 | 7 | StageAuxiliaryUpdateCallback |
| 4064E0 | 6 | StageAuxiliaryDrawCallback |
| 4064F0 | 28 | DestroyStageAuxiliary |

These **796 complete target bytes** now have natural C++ definitions in
`src/StageAuxiliaryLifecycle.cpp`. They are source reconstructions, not
exact-codegen matches. The original source class and TU boundaries remain
unknown. The current C++ class is a verified physical-layout view; its
construction and destruction should be checked under a proper VC9 /GL caller
context before claiming original code generation. In particular, the destructor
has an SEH prologue and still needs full EH ownership validation.

## Construction and callbacks

The two timers at +14 and +28 have their initialization flag at +24 and +38.
The 0x434-byte embedded VM begins at +48 and has its own real constructor.
Creation first constructs those members, then clears **all 0x4A0 bytes** of
the instance, sets flag bit 1 and publishes the global pointer. That reset
overwrites VM constructor state; do not "repair" it in the reconstruction.
The factory calls the registration helper even after a null allocation.
Any nonzero registration result causes destruction and deallocation.

The registration helper allocates two 0x24-byte chain elements with the
observed seven-field initialization, sets flags `|=3`, installs callbacks
at 4064D0 and 4064E0, and registers priorities **17 and 33**. The callbacks
are respectively a 7-byte ECX-to-EAX tail adapter into 406620 and a 6-byte
constant-one return. The stage pointers are stored at +8 and +C after
registration; null chain allocations are not preemptively guarded.

## Destruction

When +3C is nonzero and the character/partner index condition is 5, the
destructor first releases animation ID +40, zeros it, and calls the 420A00
effect reset with code 10. It then unconditionally releases +40 and +44 in
order and zeros each. The releases use the existing exact
`ReleaseSecondaryAnimation(AnimationVmId)` implementation at 4562E0.

The update chain +8, draw chain +C, and auxiliary overlay chain +49C are
unregistered in order. Each removal captures the chain manager, conditionally
enters runtime critical section 0 when flag 8000 is active, unregisters, and
**retests** the flag independently before leaving. A helper can change this
flag between the two tests. The stage singleton at 4A8D64 is then cleared,
the binding block at +480 is released and deleted, cached data +47C is freed,
and the embedded VM destructor frees its storage pointer at **+448**
(VM offset +400). The conditional free/zero order is significant.

The 28-byte global destroy entry captures the current singleton once, checks
for null, then destroys and deletes it. The resource allocation size and
member offsets are enforced in `StageAuxiliary.hpp`.

## Next work

Reconstruct the other six mode-specific updater bodies and their original
/GL owner context before refining private register allocation for these lifecycle
functions. Build locally only the affected source; do not conflate native
semantics, exact code generation, and complete executable closure.

## Main auxiliary update, 0x406620 (2026-11-17)

The update callback at 4064D0 copies ECX to EAX and jumps to **406620**.
Ghidra did not promote the entry to a function. A direct original-PE
Capstone decode establishes a complete 232-byte owner: 208 code bytes at
406620..4066EF and the six 32-bit jump-table entries at 4066F0..406707.
CC alignment follows; there are no externally owned table entries.

The natural C++ `UpdateStageAuxiliary` now preserves the native state
machine. Inactive state (+3C == 0) returns 1 immediately, without advancing
the timer. State 1 dispatches six modes by
`g_MeshEffectSelection0 * 3 + g_MeshEffectSelection1`. A mode-specific
nonzero return clears +3C and returns 1; a zero return, an out-of-range
index, or a non-one active state advances the timer at +14 before returning 1.
Only mode 0's callee, `UpdateSecondaryEffect` at 45DE30, is source-present;
the other five calls remain legitimate external source boundaries.

The physical singleton is the **existing** `g_MeshEffectActor` at 4A8D64,
not a newly allocated independent global. The auxiliary constructor,
destructor, and global destroy have been revised to use this established
owner, and the character/partner selection names now reuse the already
maintained secondary-gameplay globals. This addresses semantic ownership,
not exactness. Native private EAX/EBX/EDI call registers and the generated
jump table still need a realistic original VC9 /GL producer comparison.

## Two recovered medium-sized mode updates (2026-11-17 continuation)

Both paths now live in `src/StageAuxiliaryModeUpdates.cpp`, after direct
Ghidra review of the complete target spans (zero foreign interior xrefs,
independent RET/CC boundaries). Both use the *existing* actor singleton,
renderer VM-ID lookup at 4561E0, and the real
`EndSecondarySpecialState(40)` function. Missing VM lookup sets animation
ID +40 to zero, **still invokes the state transition**, then returns -1.

- **Mode 1 (0x407AC0..0x407B2C, 109 bytes).** With a live VM, it
  updates the effect via the native 407B50 boundary. When timer +18 reaches
  200, it plays sound 0x26, queues the additional sound pulse and signals
  the animation at the address of the ID +40 *once*, before adding the
  growth step +494 to radius +490. Subsequent frames continue radius growth
  without repeating the once-only actions.
- **Mode 4 (0x4071A0..0x40720A, 107 bytes).** Every tenth timer frame
  strictly inside 1..50, it queues positional sound 0x26 using origin.x at
  +484. It always calls the existing 4073B0 effect boundary with a live
  VM, and then copies the VM's float scaleY (+40) to radius +490.

The auxiliary physical layout now gives semantic names to origin +484,
radius +490, radiusStep +494 and transitionActive +498, retaining size
0x4A0 and all field offsets. These are **source-complete but not exact**
linked-machine-code owners. Their mode-effect callees 407B50 and 4073B0
remain separate restoration tasks.
