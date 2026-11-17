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

## Mode 3 mesh jitter (0x406D10, 356 bytes)

`src/StageAuxiliaryMode3.cpp` reconstructs the complete 356-byte native
mode-3 updater. The native first resolves animation ID +40. An absent VM
clears that ID and returns -1 **without** releasing the mesh; this is
distinct from mode 2. After frame +18 exceeds 90, it signals the current
animation at its ID's address, releases/deletes binding +480 when present,
zeros it and returns -1.

During earlier frames it updates the X coordinates of **two distinct mesh
rows**: for each column `i` strictly before `mesh.columns-1`, it samples
the original text RNG twice and updates vertex
`[mesh.columns+i]` and vertex `[2*mesh.columns+i]`, respectively,
from the parallel source-position X coordinates. The randomness is an
unsigned 32-bit value transformed by `sample * 2^-31 - 1`, rounded to float,
then multiplied by 8 and rounded to float again. It does **not** perturb
Y/Z, and it does not reset vertices from scratch. The separate 212-byte
mode-3 collision/effect helper at 406F90 runs after the loop. Original
signed/unsigned bounds are retained without adding null checks.

Direct Ghidra review covers the complete 406D10..406E73 body, six callees,
all returns, zero external interior references, and following CC bytes. The
VC9 source compiles; its private EBX/ESI allocation and exact byte equality
are still open.

## Mode 2 radial deformation boss and two radius-clearing leaves

The 665-byte `UpdateSecondarySpecialMode2` at 407D10..407FA8 now has
natural C++ in `src/StageAuxiliaryMode2.cpp`. Two separate 84-byte helpers,
`UpdateStageAuxiliaryMode5Effect` at 4079C0 and
`UpdateStageAuxiliaryMode2Effect` at 408010, live in
`src/StageAuxiliaryRadiusClearing.cpp`.

Mode 2 looks up animation ID +40 and **always** calls the existing
`EndSecondarySpecialState(40)`, including on VM lookup failure. A missing
VM releases and deletes binding +480 when non-null, clears that field and
returns -1. A live VM copies the real player position (+87C at the global
player owner 4A8EB4) to the auxiliary origin +484, then calls the genuine
VM position publisher and the 408010 radius-clearing helper. The mode
subsequently stores the VM's scaleY at +40 into auxiliary radius +490.

The grid's working radius is `float(radius + 0.5*radius)`. Its center
is `(origin.x+224, origin.y+16, origin.z)`, and reset bounds start at
`(origin.x-radialRadius-20, origin.y-radialRadius-20)`, with square side
`2*radialRadius+40`. The source retains the observed separate position
pointer capture before reset and vertex pointer capture afterward.

Each mesh point computes rounded XY distance and remaining radius.
Negative **or unordered** remaining distances clear vertex alpha.
Inside points use full vertex color **0xFFFF4040**, proven by the native
`MOV` (Ghidra reports an ambiguous `-NAN` instead). The normalized
three-component displacement is scaled by
`float((remaining/radiusSquared)*64)`, applied to vertex X/Y, and both
position and vertex Z are set to zero. No extra guards are introduced for
zero radius or unusual floating-point inputs.

The two distinct 84-byte clearing helpers each read
`(~spell.flags)&1`, call `ClearSceneProjectilesInRadius` using the
current radius and filter 1, **re-read** spell flags after that call,
and pass the new mask to `ClearSecondaryObjectsInRadius`.
The native keeps these two separate owners; they are not collapsed into
one compiled symbol. These 833 bytes have natural source and VC9 source
compilation, but no full linked byte-exact claim.

## Mode 5 stage-fade and player-spell boss (0x407720, 636 bytes)

`src/StageAuxiliaryMode5.cpp` restores the complete mode-5 native
owner, with 191 instructions, 13 distinct call targets, all returns and no
foreign interior xrefs. It checks animation ID +40 but updates stage-primary
tint +2328 **before** testing whether the VM is missing:

- With the second timer +2C above 1, grayscale is
  `(timer * 96)/30 + 32`.
- Otherwise, age +18 at least 890 uses
  `((age*3 - 2670)*32)/30 + 32`.
- Age below 30 uses `128 - (age*96)/30`. The remaining active
  window sets color `0xFF202020`.

The grayscale uses the observed OR/shift packing with opaque alpha, **not**
a saturating byte pack; signed integer division truncates toward zero.
A missing animation clears player flags bit2, queues positional sound 0x33
using the real player X position, resets the effect with code 10 and refreshes
the player emitters, returning -1.

With an active VM the function copies the real player position to the
auxiliary origin and calls the existing animation-position helper. It starts
the transition if player bit2 is clear or player mode is 4 or 2: update
spell state, increment the shared event counter, set player mode1, then
lazily initialize the second timer from zero to frame1. That branch queues
sound0x26 and creates a real damage region at the origin with radius32,
growth20, frame30 and damage75.

While the second timer is positive it clears player bit2, signals the
current animation, executes the now-source-present 84-byte mode-5 radius
clearing, adds radiusStep to radius, advances **that** timer and sets
secondary special state40. Only afterward does it test if the timer
exceeds30. Completion clears player bit2 again, releases animation
ID+40, zeros the ID and returns -1; the outer auxiliary update owner
then clears effectActive. Otherwise mode5 returns0. This preserves
the native order and does not suppress repeated timer/flag side effects.

The VM, spell and timer calls reuse existing maintained C++ contracts.
`MeshEffectEventView` retains its original global symbol type through a
local physical counter view; no second global or synthetic VM pool was
introduced. Private native LTCG ABI and byte equality remain non-exact.
