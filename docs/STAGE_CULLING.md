# Stage object culling and layer drawing

The original Japanese TH11 v1.00a executable has two adjacent stage-rendering
owners with complete maintained C++ source. Their current pinned VC9 /GL
frontier results are:

| Native owner | Native size | Candidate PDB contribution | Aligned native instructions | State |
| --- | ---: | ---: | ---: | --- |
| CullStageObject (0x403950) | 1,643 B | 1,646 B | 492/504 (2026-11-27) | Nonexact |
| DrawStageLayer (0x404190) | 727 B | 722 B | 155/191 (2026-11-27) | Nonexact |

These are diagnostic frontiers, not strict replay results. The original
translation-unit ownership and remaining byte/relocation differences are
still open. The native bodies and their callers were reviewed in Ghidra.

## CullStageObject: 2026-11-27 natural-source progress

Alignment rose from 362 to 492/504 (mnemonics 0.98) with plainer source:

- Projected bounds use the symmetric `if (x < smallest) smallest = x; else if
  (x > largest) largest = x;` form for both axes, and the depth test is
  `z >= 0 && z <= 1` (decoded from the native `test ah,0x41; jp`).
- The twelve points are a simple 12-iteration loop; VC9 itself produces the
  native two-by-six unrolled loop (identical output to the old hand-unrolled
  groups).
- Corners are written as twelve aggregate entries of in-place sums
  (`p.x + h.x`, ...); VC9's CSE then computes each value on first use, as
  native does. Separate `maxX`/`minX` locals computed everything first.
- The bound locals are declared smallestX, smallestY, largestX, largestY,
  the native initialization order.

- The bounds test is written directly in the loop. Passing each point by
  reference to a helper made VC9 base the loop pointer at `.y` (`[ecx-4]` for
  x); inline field access restores the native `[ecx]` base.

Remaining: the start still copies `*offset` to the stack, and `eye.x`'s operand
order and native's dead copy of the difference vector into a named `delta`
(`esp+0x78`, later reused by `corners`) are missing. Rewriting the start with
D3DX-style member operators fixes `center` and eye.y/z but perturbs the corner
schedule (459), so it is not kept; see the KNOWLEDGE_BASE note on x87 operand
order.

## DrawStageLayer: 2026-11-27

The `volatile` object pointer and the `register` flags local are gone. Native
keeps the object in EBX across the culling call, so no forced reload is
needed, and it updates `vm->flags` directly (a memory `or` in the second scale
branch, then a reload for the fog test). With direct field code the candidate
is 722 bytes (was 747; native 727) and 155/191 aligned. Native does not keep
the stage pointer in a register: it reloads `[ebp+8]` for each use and gives
EBX to the object, while ours holds the stage in EBX. The first scale branch
still differs (native `or ecx, 8` on the cached flags).

## CullStageObject: native algorithm

The native function has three formal `__fastcall` parameters: the stage
object, layer offset and maximum squared distance. The original body also
consumes the active perspective camera through a private EAX input. Its only
caller loads EAX with `0x004C36B4` (`g_MainRuntime + 0x434`) at
`0x00404222`, then calls the culler at `0x00404241`. The callee copies EAX into
ESI and reads the camera position and eye offset through it.
The caller also publishes that camera in `FrameRuntime().activeCamera`.

The natural C++ source models the three public fastcall arguments plus a
fourth source-level parameter for the private camera input. The sole caller
passes the embedded camera pointer used by `ApplyPerspectiveCamera`; the
focused VC9 `/GL` call graph assigns that parameter to EAX, and the candidate
callee begins by copying EAX to ESI, matching the native entry. This does not
claim that the native decorated public signature has a fourth stack argument:
the fourth source parameter models the private LTCG live-in. It is carried in
EAX without assembly or a fabricated stack argument.

The focused candidate is now 1,644 bytes with 362/504 aligned instructions,
up from the previous 1,648-byte / 361-key global-camera version. The remaining
projection/local schedule and the one-byte contribution gap are still open.

It first computes the object center in world space using the layer offset,
then adds the camera eye offset to the camera position. The squared distance
is evaluated in the native Z, X, Y order, rounded through a float local, and
compared with a strict greater-than test. A distance above the maximum rejects
the object. The ordered comparison preserves the native unordered/NaN result.

The object bounds use three independently float-rounded half-extents. The
twelve projected positions are the eight AABB corners plus four samples on
the middle X plane. A real identity-valued D3DMATRIX is initialized before
D3DXMatrixTranslation; D3DXVec3ProjectArray then projects all twelve points
using 12-byte source and destination strides.

Projected depth is strictly positive and at most one: 0 < z <= 1. The
native Ghidra body explicitly tests positive depth; a sample exactly on the
near plane is excluded. For accepted depths, X and Y extrema use the native
asymmetric branch sequence: X compares `minimum <= point` before checking
`maximum < point`, while Y compares `minimum < point` before checking
`maximum < point`. A failed first comparison writes the point into the
minimum, including the x87 unordered/NaN case. The final screen-space
intersection test
includes the four viewport edges: maximum X >= 32, minimum X <= 416,
maximum Y >= 16, and minimum Y <= 464. Intersecting bounds accept the object.
The four middle-plane samples are part of the native algorithm and must not
be dropped. The natural C++ source spells the equivalent extrema comparisons
point-first on both axes: `point.x >= smallestX`, `point.x > largestX`,
`point.y > smallestY`, and `point.y > largestY`. These preserve the native
X `minimum <= point` / `maximum < point` and Y `minimum < point` /
`maximum < point` ordered and unordered behavior, including writing NaN into
the minimum when the first comparison is false. The point-first spelling
improves focused VC9 `/GL` alignment from 328/504 to **361/504** without
changing clipping or dropping samples.

## DrawStageLayer: native algorithm

The dispatcher selects and publishes the runtime camera, applies perspective,
then reloads the published active camera before setting the D3D viewport. The
culler separately receives the embedded camera pointer used for perspective
application through its private EAX input.
It sets the renderer's stage-mode flag and scans the 16-byte records at
stage->section004 until a negative signed-16 object index.

For matching objects, the record's three float offsets are passed to
CullStageObject. Rejection increments the stage rejection counter and
clears the record's active flag. Acceptance sets the object-visible flag and
walks the signed-stride primitive stream at object +0x1C until a negative
signed-16 type.

Only primitive type zero has rendering side effects. Its offsets are combined
with the record offset and stored at VM +0x3E8/+0x3EC/+0x3F0. Optional
nonzero X/Y dimensions set scale ratios and VM flag 8. The native loads
each command scale and sprite dimension, performs the floating divide, then
publishes flag 8 before storing the quotient. The source keeps `vmFlags` live in a register across both optional scale
updates, mirrors the resulting flag value back to the VM, and reuses that
local for the subsequent fog and Z-mode tests. Each sprite-dimension pointer
is now loaded inside its own nonzero-scale branch, after the native zero test;
this recovers the native pointer-load placement for both X and Y. The focused
At the earlier anchor-only checkpoint the candidate was 731 bytes and
alignment improved from 112/191 to **127/191**. Linking the real camera,
render-batch and VM-draw callees raised that checkpoint to 742 bytes and
134/191. Modelling the private culler camera input in EAX then moved it to
754 bytes / 134-of-191. The X/Y scale locals are now `double`, with an explicit
conversion to float only at the VM store. This keeps the x87 quotient in ST(0)
across the integer flag update and removes the prior stack spill/reload,
reducing the complete candidate to **738 bytes** while retaining 134/191
aligned instructions. The owner remains nonexact: stage/section pointer
register lifetimes, command/object traversal, register allocation and the
11-byte contribution gap remain open. VM mode bits 0x03C00000 select fog;
bit 0x800 controls Z writing. State changes flush the renderer before D3D9
state updates. Drawing calls the maintained DrawAnimationVmByMode owner.
Accepted objects and drawn primitives update their respective stage
counters. Disabled Z writing is restored after the record stream ends.

## Code-generation findings retained

The following checkpoints record successive source experiments. Their
instruction-alignment counts are diagnostics and are not additive.

| Finding | Focused result | Why it matters |
| --- | ---: | --- |
| Explicit float rounding for the distance and strict greater-than rejection | Earlier culler checkpoint: 1,476 B / 294 of 504 aligned | Recreates the x87 store/reload boundary and unordered comparison behavior |
| Separate float half-extents, then grouped CameraVector | 1,506 B / 299, then 315 of 504 aligned | Preserves native half-extent rounding before corner construction |
| Layer-offset snapshot | 1,530 B / 334 of 504 aligned | Models the caller-provided three-float displacement lifetime |
| Pointer-cursor traversal of two six-point groups | 1,525 B / 383 of 504 aligned | Tested but superseded by indexed traversal |
| Identity matrix initialization before D3DX translation | 1,641 B / 402 of 504 aligned | Restores the sixteen observable float stores (twelve zeros, four ones) |
| Axis-specific offset reads and inclusive depth lower bound (rejected) | 1,639 B / 411 of 504 aligned | Earlier hypothesis; native Ghidra confirms the lower bound is strict |
| Indexed two-group traversal, six points per group | 1,644 B / 442 of 504 aligned | Emits the native pointer advance and two-group loop |
| Inclusive viewport-edge intersection tests with inclusive depth (superseded) | 1,644 B / 446 of 504 aligned | Four viewport edges are inclusive; the depth lower bound is not |
| Strict positive depth plus asymmetric X/Y extrema branches | 1,648 B / 328 of 504 aligned | Matches 0 < z <= 1 and native unordered-coordinate updates to minima |
| Reverse the equivalent Y extrema comparisons (`point.y > min/max`) | 1,648 B / 352 of 504 aligned | Same ordered and unordered behavior, with a better VC9 x87 branch schedule |
| Reverse the equivalent X extrema comparisons (`point.x >= min`, `point.x > max`) | 1,648 B / 361 of 504 aligned | Preserves the asymmetric X predicates while improving the same x87 branch schedule |
| Keep `vmFlags` live and load sprite dimensions inside each scale branch | 731 B / 127 of 191 aligned in the anchor-only context | Recovers native branch-local pointer loads and 11 more instruction keys; x87 quotient spill remains open |
| Hold X/Y scale quotients in `double` and cast only at the VM store | 738 B / 134 of 191 in the real-callee/private-camera context | Keeps the x87 quotient live across flag updates and removes both stack spill/reloads |
| Keep the per-iteration object pointer as a volatile local pointer | 740 B / 156 of 191 | Recovers 22 instruction keys across object/VM traversal by preserving the pointer reload lifetime; 13-byte extent gap remains |

The current indexed loop visits projected[group * 6 + 0..5] for two groups.
It preserves all twelve samples and their order. Earlier pointer-cursor and
fully unrolled variants were worse and are not selected.

### Real callee context added (2026-11-20)

The focused `DrawStageLayer` link now includes the maintained implementations
of `ApplyPerspectiveCamera`, `FlushRenderBatch`, and `DrawAnimationVmByMode`,
plus their projected-quad, VM-draw, quad and render-batch dependencies. This
replaces generic external call anchors with the real VC9 `/GL` call context.
The target's private inputs are EDI for `ApplyPerspectiveCamera`, ESI for
`FlushRenderBatch`, EAX/ECX for `DrawAnimationVmByMode` (VM/renderer), and EAX
for the culler camera pointer. The layer's fourth source-level culler argument
models this last private live-in; the linked caller passes the embedded camera
pointer in EAX and the callee copies it to ESI at entry.

With these callees present, the earlier complete candidate was 742 bytes
with 134/191 aligned instructions. Modelling the culler's private camera input
as a fourth source parameter, passed by the linked caller in EAX, moved the
layer candidate to 754 bytes. The `double` X/Y ratio locals then removed the
x87 spill/reloads and reduced it to **738 bytes** with 134/191 aligned
instructions. Keeping the per-iteration object pointer as a volatile local
pointer moves the candidate to **740 bytes** with **156/191** aligned
instructions, recovering 22 more native instruction keys through the object
and VM traversal. This remains diagnostic and non-exact: stage/section pointer
placement, the remaining object/command register schedule and the 13-byte
contribution-size gap remain open. The unit declaration in
`config/match-units.toml` records the support-source closure.

## Focused build and comparison

    scripts/repo-python scripts/build-match-unit.py --unit cull-stage-object-frontier
    scripts/repo-python .analysis/compare-cull-stage-object-20261118.py
    scripts/repo-python scripts/build-match-unit.py --unit draw-stage-layer-frontier
    scripts/repo-python .analysis/compare-draw-stage-layer-20261118.py

Rebuild the layer owner when shared headers or its source change. Cold-replay
only affected canonical exact units and direct dependencies; do not run the
full replay for these diagnostic frontiers.
