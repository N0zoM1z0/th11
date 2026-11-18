# Stage object culling and layer drawing

The original Japanese TH11 v1.00a executable has two adjacent stage-rendering
owners with complete maintained C++ source. Their current pinned VC9 /GL
frontier results are:

| Native owner | Native size | Candidate PDB contribution | Aligned native instructions | State |
| --- | ---: | ---: | ---: | --- |
| CullStageObject (0x403950) | 1,643 B | 1,644 B | 403/504 | Nonexact |
| DrawStageLayer (0x404190) | 727 B | 729 B | 112/191 | Nonexact |

These are diagnostic frontiers, not strict replay results. The original
translation-unit ownership and remaining byte/relocation differences are
still open. The native bodies and their callers were reviewed in Ghidra.

## CullStageObject: native algorithm

The function receives a perspective camera, a stage object, a layer offset,
and a maximum squared distance. Its current source-level parameter ABI remains
provisional.

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
near plane is excluded. For accepted depths, X and Y extrema update through
the native ordered less-than / greater-than branches. Unordered X/Y values
therefore update neither bound. The final screen-space intersection test
includes the four viewport edges: maximum X >= 32, minimum X <= 416,
maximum Y >= 16, and minimum Y <= 464. Intersecting bounds accept the object.
The four middle-plane samples are part of the native algorithm and must not
be dropped.

## DrawStageLayer: native algorithm

The dispatcher selects and publishes the runtime camera, applies perspective,
then reloads the published active camera before setting the D3D viewport.
It sets the renderer's stage-mode flag and scans the 16-byte records at
stage->section004 until a negative signed-16 object index.

For matching objects, the record's three float offsets are passed to
CullStageObject. Rejection increments the stage rejection counter and
clears the record's active flag. Acceptance sets the object-visible flag and
walks the signed-stride primitive stream at object +0x1C until a negative
signed-16 type.

Only primitive type zero has rendering side effects. Its offsets are combined
with the record offset and stored at VM +0x3E8/+0x3EC/+0x3F0. Optional
nonzero X/Y dimensions set scale ratios and VM flag 8. VM mode bits
0x03C00000 select fog; bit 0x800 controls Z writing. State changes flush
the renderer before D3D9 state updates. Drawing calls the maintained
DrawAnimationVmByMode owner. Accepted objects and drawn primitives update
their respective stage counters. Disabled Z writing is restored after the
record stream ends.

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
| Strict positive projected depth | 1,644 B / 403 of 504 aligned | Matches native 0 < z <= 1; excludes samples exactly on the near plane |

The current indexed loop visits projected[group * 6 + 0..5] for two groups.
It preserves all twelve samples and their order. Earlier pointer-cursor and
fully unrolled variants were worse and are not selected.

For DrawStageLayer, three independent source details improved the focused
owner: reloading FrameRuntime.activeCamera after perspective application,
copying the layer offset as three typed float members (recovering x87
FLD/FSTP), and retaining the Z-write bit extraction as a full-width unsigned
integer rather than bool. The current owner is 729 B with 112/191 aligned
native instructions.

## Focused build and comparison

    scripts/repo-python scripts/build-match-unit.py --unit cull-stage-object-frontier
    scripts/repo-python .analysis/compare-cull-stage-object-20261118.py
    scripts/repo-python scripts/build-match-unit.py --unit draw-stage-layer-frontier
    scripts/repo-python .analysis/compare-draw-stage-layer-20261118.py

Rebuild the layer owner when shared headers or its source change. Cold-replay
only affected canonical exact units and direct dependencies; do not run the
full replay for these diagnostic frontiers.
