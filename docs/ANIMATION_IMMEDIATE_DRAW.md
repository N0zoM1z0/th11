# Immediate animation drawing and mode dispatch

`src/AnimationImmediateDraw.cpp`, `src/AnimationImmediateDrawViews.hpp` and
`src/AnimationImmediateDrawWorldState.cpp` cover the immediate VM drawing and
world-state paths. The full world-space `ImmediateVmView::DrawWorld` body is
now grouped into `src/AnimationVmRendererOwner.cpp` with the quad builders,
so its real `/GL` owner context is shared rather than simulated by a wrapper. Six functions now have complete cold exact proofs: the four
strip/fan/UV leafs (964 bytes), the world-state code+table owner (600 bytes),
and the full world-space drawing owner (1,036 bytes). The projected fog and
mode-dispatch owners remain non-exact; the magnitude helper is still an
independently open 28-byte leaf. The complete276-byte `DrawByMode` owner
includes217 code bytes, three alignment bytes and its14-entry/56-byte switch
table. Its natural `__fastcall` member source now lives in
`AnimationVmRendererOwner.cpp`; the linked candidate is260 bytes (203 code,
one alignment byte and the full table). The remaining16-byte gap is in
callee-save prologue/epilogue placement, not the dispatch cases or table.
Original callers consistently supply VM in EAX and renderer in ECX.

Nine additional boundary/origin reviews cover eight engine/math owners and
one verified D3DX import thunk. Engine/math origins remain indeterminate;
the six-byte import thunk is an exclusion. The dispatcher now has a reviewed
complete276-byte owner:217 code+3 alignment+14 cells/56 bytes. Its former217-byte
code-only boundary is superseded; the existing review count is unchanged.

| Owner | Address | Target owned bytes | Current state |
| --- | --- | ---: | --- |
| AnimationExtentMagnitude | 40B930 | 28 | Source; candidate17 /28 diagnostic differences |
| ApplyAnimationVmWorldState | 44F4B0 | 600 (572 code + 28 table) | Exact; 21 fields, zero differences |
| DrawFoggedProjectedAnimationVm | 451170 | 546 | Source; candidate548 /150 |
| DrawWorldAnimationVm | 4513A0 | 1,036 | Exact; 23 fields, zero differences |
| UpdateTransposedStripVertexUv | 4517B0 | 258 | Exact; no address fields |
| UpdateStripVertexUv | 4518C0 | 258 | Exact; no address fields |
| DrawAnimationVertexStrip | 451A00 | 224 | Exact;7 reviewed fields |
| DrawAnimationVertexFan | 451E10 | 224 | Exact;10 reviewed fields |
| DrawAnimationVmByMode | 451EF0 | 276 | `AnimationVmRendererOwner.cpp`; candidate260/276, 16-byte callee-save gap |

The complete dispatcher bound451EF0..452003 includes its three-byte no-op LEA
and fourteen owned table cells. All14 apparent foreign DATA refs come from
that table; twelve CC precede452010. Independent full PE CFG/table/neighbor
review supplements the mapped-text-attested Ghidra code/table references.
The natural-C++ dispatcher lives in `AnimationVmRendererOwner.cpp` beside
`ImmediateVmView::DrawWorld`; the strip/fan submitters remain in
`AnimationImmediateDraw.cpp`. Its cold candidate
contribution is260 bytes:203 code bytes, one alignment byte and the complete
56-byte table. All14 mode slots select the reviewed helper paths in the original
order. The target has an additional16 bytes of callee-save prologue/epilogue
code, so the complete276-byte owner remains non-exact; no byte credit is
claimed. Earlier272/115 code-only diagnostics do not establish full-owner
equality. See
[HUD drawing](SECONDARY_HUD_DRAW.md) for the caller/context evidence.

## Physical immediate-VM view

The immediate-draw translation unit now names the complete 0x408-byte
physical view used by its world, fog and strip/fan paths. The typed tween
subobject occupies 0x90..0x2B3 (0x224 bytes) and reuses the established
stage-vector, integer-vector/scalar and float-pair/scalar tween layouts.
The view then names the three matrix blocks at 0x2B4, 0x2F4 and 0x334,
primary/alternate packed colors at 0x374/0x378, event and saved-script
state at 0x37C..0x3AB, sprite/file/strip state at 0x3AC..0x3B7,
integer/float variable banks at 0x3B8..0x3D3, and the position/offset
vectors plus vertex pointer and flags at 0x3DC..0x407.

Compile-time offset assertions pin the update/draw nodes, rotation and
angular velocity, scale/size/UV state, timer, tween region, matrices,
colors, script pointers, sprite/file pointers, variable banks, vectors,
vertices and final flags. The four independently strict strip/fan/UV
units were rebuilt against this layout and remain exact (964 code bytes,
17 address/call fields, zero differences). This is a source-local physical
view overlay; it does not by itself establish the original C++ class
identity or authorship.

## World drawing

The1,036-byte owner gates on active/visible flags and the primary color's alpha,
then flushes a nonempty local quad batch. It preserves the base/transformed
matrix cache and dirty-flag protocol described in
[projected quads](ANIMATION_PROJECTED_QUAD.md). Nonzero and unordered rotations
call actual D3DX X/Y/Z rotation/multiplication boundaries.

Working X/Y translation differs from projected drawing: center anchors use
offset+position+extraOffset; the aligned anchors subtract/add the float-rounded
absolute half scaled dimension. Selector1 snapshots translation as double
before the magnitude call; selector2 reads it after the call. Selector3 retains
that cached matrix translation. Z is written before world-state setup, then
re-read with extraOffset afterward before SetTransform(WORLD).

World, rotation and texture matrices have genuine independent uses. Their
function-scope lifetimes preserve the native stack layout. The natural C++
owner is `ImmediateVmView::DrawWorld`; pinned VC9 RTM `/GL` emits its complete
1,036-byte primary-TU PDB contribution. Strict linked-PE replay binds all 23
call/data fields (including the D3DX imports, magnitude calls, world-state
method, float literal and eight runtime-global references) to their reviewed
original addresses, then reports zero byte differences across the full owner.
The private entry ABI is recovered naturally: VM in EBX, renderer in the
caller stack slot, EAX status, 0xD0 frame and `RET 4`. No ASM, padding or
synthetic contribution is involved.

The four magnitude call fields are correctly bound to the original 0x40B930
leaf, but that 28-byte helper remains independently non-exact (the current
natural helper contribution is 17 bytes). This owner proof therefore credits
the complete world-draw body and its call-site relocations, not the helper's
own implementation. The world-state call independently replays as a strict
600-byte exact contribution.

The world-state helper applies blend changes, primary/alternate color with
ordered saturated red/green/blue/alpha tint, cached D3DRS_TEXTUREFACTOR, filter
changes, and the unconditional applications counter. Flushes precede cache
updates as observed. Blend7 stores its cache without a blend API call. The
572-byte executable body is immediately followed by its owned 28-byte,
seven-entry blend jump table; the complete 600-byte linked contribution is
strict exact. A volatile read of the runtime device pointer preserves the
native cache-store-before-device-load ordering without assembly or patching.

After world and texture bindings, the original UV cache predicate checks
`offsetU != 0` twice. It does not check offsetV. The source preserves this
duplicate condition. A texture transform copies VM+334 and sets _31/_32 from
the first UV plus offsets. Geometry2 selects stream0's cached buffer with
stride20, FVF102, and texture-factor COLORARG2/ALPHAARG2. DrawPrimitive uses
TRIANGLESTRIP, start0, count2; HRESULT is ignored and the function returns0.
The20-byte world vertex contains XYZ/UV, without a diffuse color field.

## Projected fog differs from billboard fog

The546-byte owner builds projected corners and processes all four. Actual
D3DXVec3Transform writes four-component results from renderer corners+4355D0
and world+435140. Distance subtracts the fixed runtime eye+204, not the active
camera's position. The float distance and ordered fog comparisons preserve
the native unordered paths.

This path does not tint or fade alpha. Intermediate colors subtract scaled
differences against fractional fog channels, without the earlier integer fog
conversion used by the794-byte billboard path. At factor>=1 it uses the active
camera's packed fog color while restoring the selected VM alpha. It does not
return early for far corners. Submission flag2 preserves the per-corner colors;
all RHW values reset to1 afterward, including culled submissions.

A named float temporary for the red difference introduced an extra float
store and changed native precision/codegen; that private variant was rejected.
The maintained expression retains the direct subtract/multiply form. Floating
operation scheduling, alpha-store order and the submission's private flag ABI
remain concrete codegen gaps.

## Strip, fan and UV updates

Native mode9/C/D is TRIANGLESTRIP5; modeB is TRIANGLEFAN6. This corrects the
previous unproved triangle-list label in the dispatcher notes. Both UP calls
use28-byte XYZRHW/diffuse/UV vertices and unsigned `vertexCount - 2` with no
clamping. Both ignore HRESULT and return0.

Strip drawing checks active/visible/primary alpha, conditionally flushes the
local batch, updates texture then geometry3/FVF144, applies normal VM draw
state and sets diffuse texture-stage arguments. Fan drawing has no eligibility
gate, applies geometry/state before texture, then unconditionally flushes the
global `g_RenderBatch`, disables Z writes and draws. It does not restore Z writes.
The global flush must not be replaced with the passed renderer.

UV updates reject signed count<3 and leave positions untouched. The two passes
write even vertices, then odd vertices, with independently re-read primary
colors and RHW1. Each descending coordinate update rounds to float. The step
uses the rounded UV span divided by signed `(count + 1) / 2 - 1`; source uses
unsigned addition then signed conversion to retain native count+1 wrapping.

Normal mapping holds U at the two sides while V descends. Transposed mapping
puts the descending coordinate in U and the side in V, including the native
swapped offset roles. Natural index+=2 loops reproduce the complete258-byte
owners; manually expressed remaining-count loops produced263 bytes with extra
signed division/branch work. Separate eligibility returns likewise close the
strip's former236-byte candidate to224 bytes.

## Dispatcher and return-type correction

The source dispatches modes0/1/2/3 to unrotated/adaptive/unrounded/adaptive
unrounded paths;4/5/6/7/8 to billboard/projected/fogged billboard/fogged
projected/world;9/C/D to strip andB to fan. ModeA and out-of-range modes return0.
Inactive/invisible/zero primary-alpha VMs return-1. Buffered routes double
VM+3B4 with unsigned wrapping and pass VM+400 unchanged.

The original dispatcher forwards EAX from every draw wrapper. Four maintained
wrappers previously declared void now return the actual submission status.
This is a caller-supported signature correction, not a fabricated result.
The54-byte unrounded canonical symbol now has an int return; its complete
cold proof remains exact after updating the manifest's decorated symbol.

## Full extents, origin and cold evidence

Independent Capstone decoding covers every recorded code byte and RET tail.
The1,036-byte world-draw owner has302 instructions, so a full explicit Ghidra
span review supplements the250-instruction default limit. The world-state
owner is a 600-byte contiguous linked contribution: 572 executable bytes
followed by its owned table at44F6EC..44F707. Its 21 fields replay with zero
differences. The dispatcher retains fourteen entries451FCC..452003 and
remains non-exact; no table or contribution is cropped for acceptance.

The full6-byte thunk45FA04 jumps through IAT48B2BC. Independent original-PE
import decoding binds d3dx9_37.dll!D3DXVec3Transform; full hash-attested Ghidra
review confirms the body, references and separate neighboring entries.
Only the thunk is excluded. No external DLL or authored-source credit follows.
The math adapter and custom rendering roles do not prove original authorship.

The four strip/fan/UV exact units use build/match-linked/AnimationImmediateDraw,
the real projected/quad/submission/batch support and independent probe inputs.
The world-state helper uses its dedicated build/match-linked/AnimationImmediateDrawWorldState
context. Across these five exact units, all38 reviewed address/call fields and
all1,564 owned bytes replay with zero differences. Strip fields are REL32
offsets47/136 and DIR32 offsets78/106/141/163/189. Fan fields are REL32
offsets16/60/116 and DIR32 offsets30/91/111/121/141/163/189. The helper adds
three FlushRenderBatch calls, ten runtime-device references, one table-base
reference and seven table entries. Independently reviewed targets include
Flush44FD10, runtime4C3280+8 and global renderer4C3268.

Eleven units were cold-replayed: the five exact units plus six directly
affected AnimationQuad/AnimationVmDraw units. The latter remain exact despite
the corrected wrapper return types. No full cohort or behavioral suite ran.
Original TU ownership and a complete native product remain open. The subsequent
[fan review](ANIMATION_FAN.md) recovers three additional draw owners from the
gap451AE0..451E0F and three other missing entries, with six new exact proofs.

Compiler probes of natural loops, blend-pair/red-temporary expressions, matrix
lifetimes and restricted caller graphs closed the world-state owner. The
world-draw private ABI/frame and flags-register allocation, plus the dispatcher
private ABI, remain open.
The findings above and maintained source retain the useful results.
