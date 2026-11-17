# Stage object culling and layer drawing

The original Japanese TH11 v1.00a executable contains a substantial pair of
stage-rendering functions immediately before the main script interpreter.
They now have complete maintained natural C++ source.

| Native address | Size | C++ owner | Current state |
| --- | ---: | --- | --- |
| 0x403950..0x403FBA | 1,643 bytes | `CullStageObject` in `src/StageCulling.cpp` | Source present, non-exact |
| 0x404190..0x404466 | 727 bytes | `DrawStageLayer` in `src/StageLayerDrawing.cpp` | Source present, non-exact |

Direct local Ghidra review verified the complete contiguous bodies, both
separate functions' exits, their producer/caller connections, and stage/VM
field observations. The culler has one native caller (the layer dispatcher)
and two D3DX import calls. The dispatcher has twelve native call sites in
DrawStage and DrawStageOverlay, with eight background and four overlay layers.
The 2,370 bytes are **native source-coverage extents**, not exact-codegen
bytes: original /GL translation-unit boundaries, register allocation and
complete relocation replay are open.

## Culling: 1,643-byte boss function

The culler receives an object, a layer-record offset, a camera, and a
maximum squared distance. It computes the object's center in world coordinates
and compares squared distance to the camera's position **plus eye offset**.
If the distance fails, it returns 1 (reject).

It constructs 12 positions: all eight corners of the object bounding box
at the object's own center/half-extents, plus four points on the middle X
plane. Half-extents use the native double 0.5 constant; the world matrix is
a translation by the 16-byte layer record's offset. An actual
`D3DXMatrixTranslation` and `D3DXVec3ProjectArray` call project all twelve
points using the runtime camera's view/projection/viewport, with 12-byte
source/destination stride. A regular 8-corner AABB-only shortcut would discard
four native samples.

The projected Z test is `0 < z <= 1`. For valid projected points, the
native x87 comparisons update minima/maxima with an asymmetric comparison
structure which matters for unordered values. The clip rectangle is
X=(32, 416), Y=(16, 464); intersecting projected bounds return 0 (accept).
No extra viewport or object-null safeguards were introduced.

## Layer dispatcher: 727-byte boss function

The dispatcher first selects the runtime camera, applies perspective, calls
the device viewport setter and publishes camera index 2. It sets the
renderer stage-mode flag at its native +0x4355C4 field. The 16-byte records at
`stage->section004` are scanned until a negative signed16 object index.

For matching objects (`object->layer` is signed byte +2), the culler
receives the record's three-float offset. Rejection increments the stage's
+0x2FE8 integer counter and clears record flag bit0. Acceptance sets object
flag bit1, then visits its signed-stride primitive stream at object+0x1C
until a negative signed16 type. VM index is signed16 at command+6 and
addresses VMs through the native 0x434 stride, without sanitizing indices.

Only primitive type 0 has rendering side effects. For relevant VM modes,
the command's three offsets are combined with the record offset and stored
at VM+0x3E8/3EC/3F0. Nonzero optional X/Y dimensions produce VM scale
ratios using the sprite's +0x38/+0x34 values and set VM flag 8. VM mode
bits 0x03C00000 control fog, comparing equal to 0x02000000; bit 0x800
controls Z writing. Each state transition flushes the renderer before the
D3D9 state update. Drawing calls the already maintained
`DrawAnimationVmByMode` at 0x451EF0 and increments the +0x2FEC
primitive counter.

After each accepted object, the record flag gets bit0 and the +0x2FE4
object counter increments. Once the record stream ends, any disabled Z
writing is restored. The function always returns zero via RET8.

The three integer counters reuse twelve physical bytes that another source
view calls `drawOffset`. This is a *local overlay* with verified offsets;
no shared exact unit's class layout was changed.

## Remaining work

Build a realistic VC9 /GL producer and compare both entire PDB contributions,
rather than treating isolated normal-COFF compilation as original ownership.
The 1,643-byte D3DX projection and its 727-byte rendering caller are the
priority. Preserve the existing exact StageDrawCallbacks and StageLifecycle
owners; this change does not alter their machine instructions.
