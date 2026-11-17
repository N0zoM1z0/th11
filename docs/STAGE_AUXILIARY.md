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

Reconstruct the main auxiliary updater at 406620 and its other
callers before refining private register allocation for these lifecycle
functions. Build locally only the affected source; do not conflate native
semantics, exact code generation, and complete executable closure.
