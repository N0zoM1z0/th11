# Immediate animation drawing and mode dispatch

`src/AnimationImmediateDraw.cpp` adds nine complete source mappings covering
3,363 recorded target code bytes, including the1,036-byte world draw,
572-byte world-state setup and546-byte projected fog path. Four functions
have new complete cold exact proofs: two224-byte draw functions and two258-byte
UV updates, totaling964 bytes. The larger owners remain non-exact.

Nine additional boundary/origin reviews cover eight engine/math owners and
one verified D3DX import thunk. Engine/math origins remain indeterminate;
the six-byte import thunk is an exclusion. The217-byte dispatcher was already
reviewed and now acquires source. Its fourteen table entries remain separately
reviewed data, not additional new function reviews.

| Owner | Address | Target code bytes | Current state |
| --- | --- | ---: | --- |
| AnimationExtentMagnitude | 40B930 | 28 | Source; candidate17 /28 diagnostic differences |
| ApplyAnimationVmWorldState | 44F4B0 | 572 | Source; candidate580 /411 |
| DrawFoggedProjectedAnimationVm | 451170 | 546 | Source; candidate548 /150 |
| DrawWorldAnimationVm | 4513A0 | 1,036 | Source; candidate1,038 /750 |
| UpdateTransposedStripVertexUv | 4517B0 | 258 | Exact; no address fields |
| UpdateStripVertexUv | 4518C0 | 258 | Exact; no address fields |
| DrawAnimationVertexStrip | 451A00 | 224 | Exact;7 reviewed fields |
| DrawAnimationVertexFan | 451E10 | 224 | Exact;10 reviewed fields |
| DrawAnimationVmByMode | 451EF0 | 217 | Source; candidate272 /115 |

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

World, rotation and texture matrices have genuine independent uses. Restoring
their function-scope lifetimes prevents reuse of the rotation/texture stack
slot. The candidate frame changes from94 toD4 bytes, approaching the nativeD0,
but native alignment and private register parameters remain unrecovered.
No inert local, explicit alignment, padding or ABI annotation is added.

The world-state helper applies blend changes, primary/alternate color with
ordered saturated red/green/blue/alpha tint, cached D3DRS_TEXTUREFACTOR, filter
changes, and the unconditional applications counter. Flushes precede cache
updates as observed. Blend7 stores its cache without a blend API call.

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
The1,036-byte owner has302 instructions, so a full explicit Ghidra span review
supplements the250-instruction default limit. The572-byte helper's seven
external interior references are its independently decoded table44F6EC..44F707.
The dispatcher retains fourteen entries451FCC..452003. Code and owned table
extents must be reconciled with complete PDB contributions before any future
exact claim for these two switch owners; candidate sizes above include data
where attributed by PDB. No table or contribution is cropped for acceptance.

The full6-byte thunk45FA04 jumps through IAT48B2BC. Independent original-PE
import decoding binds d3dx9_37.dll!D3DXVec3Transform; full hash-attested Ghidra
review confirms the body, references and separate neighboring entries.
Only the thunk is excluded. No external DLL or authored-source credit follows.
The math adapter and custom rendering roles do not prove original authorship.

The four new exact units use `build/match-linked/AnimationImmediateDraw`, the
real projected/quad/submission/batch support and independent probe inputs.
Every complete primary-TU public contribution has current source/header/tool,
producer and PE/MAP/PDB receipts. Strip fields are REL32 offsets47/136 and
DIR32 offsets78/106/141/163/189. Fan fields are REL32 offsets16/60/116 and
DIR32 offsets30/91/111/121/141/163/189. Independently reviewed targets are
Flush44FD10, state44F710, runtime4C3280+8, and global renderer4C3268. UV owners
have no fields. All17 fields and every other byte replay with zero differences.

Only ten units were cold-replayed: the four new units plus six directly
affected AnimationQuad/AnimationVmDraw units. The latter remain exact despite
the corrected wrapper return types. No full cohort or behavioral suite ran.
At this checkpoint exact state reached82 functions/15,952 bytes; original TU
ownership and a complete native product remain open. The subsequent
[fan review](ANIMATION_FAN.md) recovers three additional draw owners from the
gap451AE0..451E0F and three other missing entries, with six new exact proofs.

Private logs: `.analysis/immediate-draw-ten-cold.log` and
`immediate-draw-current.log`; complete current diagnostics in
`immediate-draw-current/diagnostic.json`. Target reviews:
`vm-remaining-modes-review.txt`, `vm-world-quad-full-span-review.txt`,
`vm-immediate-helpers-review.txt` and their full mapped-text attestations;
`immediate-draw-independent-review.json` and `immediate-draw-exact-items.json`.
Compiler probes compare baseline/natural loops, blend-pair/red-temporary,
matrix lifetimes and restricted caller graphs. The blend-pair and caller-graph
variants did not close the world-state/private-ABI gaps. Private target text,
decompiler hypotheses, compiler payloads and databases remain uncommitted.
