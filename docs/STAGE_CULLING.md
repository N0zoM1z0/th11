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

## Rounded culling distance and ordered rejection (2026-11-18)

The original CullStageObject evaluates the squared world/eye distance,
stores the result to a 32-bit float local with x87 FSTP and reloads it
with FLD before the distance comparison. It rejects only when that
rounded value is strictly above the supplied maximum. In particular,
unordered/NaN comparisons do not reject the object. The prior C++ used
a potentially extended-precision expression and the negation of a
less-than-or-equal comparison; on unordered inputs it returned a
different result.

The natural C++ now explicitly rounds the distance to float and uses
the strict greater-than test. The scoped VC9 compiler emits the original
FSTP/FLD and TEST AH,5/JNP comparison sequence. The complete candidate
is 1,476 bytes with 294/504 aligned native instructions, compared with
the earlier 1,468 bytes and 297/504. The raw alignment tally regresses
slightly elsewhere in the owner, but the critical precision boundary and
unordered comparison behavior are restored. Both large culling/drawing
frontiers compile; neither is byte exact.

## Float-valued layer offset snapshot (2026-11-18)

The native culler caller copies X, Y and Z from the 16-byte layer
record into three local float slots with separate x87 loads/stores
before invoking CullStageObject. An aggregate POD assignment in C++
compiled to three integer MOVs instead. Explicitly assigning the
three typed float members of the existing CameraVector local recovers
the x87 load/store sequence. The focused StageLayer primary /GL output
keeps its 729-byte size, but improves alignment from 110 to
112 of 191 native instructions. No generated bytes or padding were
hand-written, and no exact status is claimed.

## Re-reading the published active camera (2026-11-18)

Original DrawStageLayer publishes the runtime camera, calls
ApplyPerspectiveCamera, then reloads FrameRuntime.activeCamera before
selecting the viewport. The previous C++ forwarded the pre-call local
camera pointer, assuming that the intervening call cannot change the
camera publication. The current source naturally obtains the viewport
from the freshly loaded active-camera field. The focused VC9 /GL
primary owner changes from 723 bytes and 98/191 aligned instructions
to 729 bytes and 110/191 aligned instructions versus 727 native bytes.
This restores the viewport data dependency but is not byte-exact.

## Focused VC9 /GL linked frontiers (2026-11-17)

The tracked `cull-stage-object-frontier` and `draw-stage-layer-frontier`
build only the two original rendering owners and their dynamic probe caller,
not the whole game. Both use pinned VC9 /GL; they are deliberately **frontiers**
and cannot acquire an exact result merely by matching a PDB extent.

| Primary owner | Native | Current PDB contribution | Instruction-key diagnostic |
| --- | ---: | ---: | ---: |
| CullStageObject | 1,643 | 1,476 | 294 / 504 target instructions |
| DrawStageLayer | 727 | 729 | 112 / 191 target instructions |

The culler initially emitted 1,316 bytes (206/504 aligned) with an
incorrect frame-pointer profile. Removing `/Oy-` restored the native
`sub esp,194h`-style frame and improved to 1,444 bytes (247/504
aligned). Preserving the eye's three **separately float-rounded**
coordinates across the distance subtraction increased the PDB
contribution to 1,468 bytes and alignment to 282/504. This source
change is semantic: in x87 compilation the explicit intermediate
rounding boundaries matter. Reversing the commutative X-addend order and
altering the snapshot's declaration order produced no further gain and
were not retained.

The layer dispatcher now emits 723 bytes in its own primary
translation-unit context. Its target uses EBP framing and a 0x14-byte
local frame; the current candidate uses 0x1C bytes. Its stack argument
and register scheduling still differ despite the near-sized PDB extent.
The current comparison uses normalized instruction keys for orientation
only; it does **not** mask a failed strict replay into an exact claim.
D3DX import linkage and private caller ABI still need explicit proof.

Continue both bosses with original control-flow, calling-convention and
temporary-lifetime evidence. Preserve already-exact StageCallbacks and
StageLifecycle owners; no unrelated bulk cold replay is warranted.

## Focused VC9 /GL comparison (2026-11-17 continuation)

The new `cull-stage-object-frontier` and `draw-stage-layer-frontier`
contexts compile only the two participating real source TUs and a dynamic
`StageLayerProbe` caller using pinned VC9, with PDB/MAP/PE receipts.
They are explicitly **diagnostic frontiers**: their code is non-exact and
their original REL32/DIR32 fields have not been exhaustively replayed.

The original culler contains **1,643 bytes and 504 decoded instructions**.
In the selected primary source context, forcing `/Oy-` incorrectly retained
an EBP frame and produced 1,316 bytes with 206 normalized instruction
keys aligned. Removing that profile option recovers the native frame-pointer
omission and yields 1,444 bytes/247 aligned keys. A separate
`eyePosition` three-float snapshot retains the native camera eye-coordinate
rounding boundaries, improving the result to **1,468 bytes/282 aligned keys**.

The original projection postpass loops over **two groups of six samples**,
with independent depth and min/max tests for each projected point. The
source now represents this explicitly with a tiny inlinable C++ helper;
VC9 may still fold the six source calls into its own loop. More
importantly, the old `if (point.x >= minimum) ... else minimum=point.x`
incorrectly assigned NaN to the minimum because unordered comparison
makes the first condition false. The native x87 instructions update
the minimum only on **strictly less** and the maximum only on
**strictly greater**; unordered samples update neither. The revised
source uses those ordered comparisons for **both X and Y**.
The final candidate remains 1,468 bytes, but normalization now aligns
**297 of 504 native instruction keys** (469 candidate instructions).
This is a source/behavioral correction with diagnostic codegen progress,
**not an exactness claim**.

The 727-byte `DrawStageLayer` in its own primary TU now produces
**723 bytes / 194 instructions**, with 98 of 191 native normalized
keys aligned (earlier 644 bytes / 175 instructions and 58 aligned).
The original passes a **local copy** of the record's 12-byte offset to
`CullStageObject`; the previous source passed a pointer into the
record. Restoring that snapshot yields 672 bytes. Explicitly retaining
the two Fog-enable/disable paths with their independent runtime checks
and renderer flushes improves this to 703 bytes, restoring the native
0x14-byte frame in that intermediate variant. Computing the optional
VM X/Y scale ratios into float locals before committing the flags and
results yields the current 723-byte source candidate. X-only and
Y-only variants produced 715 and 711 bytes respectively, without
improving normalized alignment. Linking the real `ApplyPerspectiveCamera`
callee as another producer produced 712 bytes, and that experimental
context was withdrawn.

The current stage frame remains 0x1C versus native 0x14, and
register/stack scheduling and imported D3DX bindings still need
correction. None of these numerical diagnostics is an exact result.
The previous rejected X-operand reversal and culler local-declaration
reordering likewise produced no gain. No ASM, executable-byte
injection, or inert padding was introduced.
