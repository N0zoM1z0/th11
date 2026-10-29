# Projected quads and software fog

`src/AnimationProjectedQuad.cpp` adds seven complete source mappings covering
1,896 target bytes. The two larger owners are the792-byte projected-coordinate
builder and794-byte software-fog submission. All seven remain non-exact.
Fourteen additional boundary/origin reviews cover nine engine/math owners and
five independently identified D3DX import thunks. The nine engine/math origins
remain indeterminate; the five import thunks are exclusions.

| Owner | Address | Target bytes | State |
| --- | --- | ---: | --- |
| AnimationDistanceRoot | 4196E0 | 31 | Complete source; non-exact |
| DrawAdaptiveUnroundedAnimationVm | 450690 | 111 | Complete source; non-exact |
| BuildBillboardAnimationVmQuad | 450700 | 976 | Complete review; source open |
| DrawBillboardAnimationVm | 450AD0 | 33 | Complete source; non-exact |
| DrawFoggedBillboardAnimationVm | 450B00 | 794 | Complete source; non-exact |
| BuildProjectedAnimationVmQuad | 450E20 | 792 | Complete source; non-exact |
| DrawProjectedAnimationVm | 451140 | 46 | Complete source; non-exact |
| DrawAnimationVmByMode | 451EF0 | 217 | Source added in subsequent immediate-mode checkpoint |
| BuildAnimationVmModeQuad | 452010 | 89 | Complete source; non-exact |

## Matrix cache and projection

The VM retains a base matrix at+2B4 and a transformed matrix at+2F4. Unless
flag4000 inhibits rebuilding, flags8/4 trigger a base copy, scaling of only
_11/_22, clearing8, ordered X/Y/Z rotation multiplications and clearing4.
Each rotation runs for nonzero or unordered angles; negative zero skips it.
These calls use the actual D3DX interfaces, including in-place multiplication.

The working world matrix is copied from the cache. X/Y translation add
offset+position+extraOffset to the cached _41/_42; Z replaces _43 with that
sum. Adding the old Z translation would change the original behavior.

Independent PE reads establish sprite-space constants -128/+128/-256/+256,
not unit-quad constants. Center anchors use -128/+128; the two aligned anchors
use0/+256 and-256/0. Selector3 leaves that local axis unwritten. Every corner Z
is zero. Four D3DXVec3Project calls re-read the active camera at runtime+54C,
use its view+4C/projection+8C/viewport+CC, and write the shared scratch corners.
The final world matrix is published at renderer+435140.

The46-byte draw wrapper builds, submits flag0, then resets all four RHW cells
to1 even when submission culled. The33-byte billboard wrapper returns-1 on
construction failure and otherwise returns submission's status. Adaptive
unrounded drawing chooses the unrounded builder for either zero angle and the
actual rotated boundary for nonzero/NaN, then always submits flag0. The89-byte
coordinate query instead selects solely by mode:0 floor,1 rotated,2/3 unrounded,
and no writes for other modes.

## Software fog

The794-byte owner first calls the actual976-byte billboard builder; failed
construction returns-1. It snapshots the active camera's fog-start minus
fog-end, selects primary/alternate VM color, and computes camera distance.
Position calculation has distinct float stores for offset+position, then
extraOffset addition, then camera subtraction. Collapsing these into one
extended expression would remove native rounding points. The square sum is
rounded to float before the31-byte square-root adapter; that adapter uses the
existing x87 sqrt intrinsic and rounds the returned value to float.

Tint scales red/green/blue/alpha in that order with unsigned product>>7 and
saturation at255. This owner inlines the algebra; the separate submission
owner calls the already recovered channel helper. If distance is ordered
greater than runtime fog-start at+300, the factor becomes
`(fogStart - distance) / (cameraFogStart - cameraFogEnd)`. Ordered factor>=1
returns-1. Unordered comparisons preserve the native paths; no clamping or
zero-range fallback is added.

Blue, green and red first truncate their fog float channels to signed int,
then subtract the truncated scaled channel difference. Alpha truncates
`alpha * (1 - factor)`. Final byte stores preserve wrapping. Each output byte
is written separately into the first scratch color, then the complete color
is copied to corners1/2/3 in order. Submission flag2 preserves these colors.

Physical views retain unknown storage and assert the observed matrix, flag,
fog, camera and renderer offsets. They do not claim original class layouts.

## Boundaries and origin evidence

All nine complete spans independently decode to their recorded sizes and RET
tails. Ghidra queries attest the complete mapped target text. The976-byte
owner has295 instructions: an explicit full-span query supplements the
default250-instruction review limit. It ends immediately before450AD0.

The217-byte dispatcher owns a separately reviewed14-entry table at451FCC
through452003; modes9/C/D share one destination and modeA shares the default.
The89-byte query has four entries at45206C..45207B, with modes2/3 sharing a
destination. These tables explain all14/four external interior data
references. Alignment/table bytes are separate from the code extents. Neither
function borrows neighboring bytes or earns exact credit.

| Six-byte import thunk | IAT slot | Original PE import name |
| --- | --- | --- |
| 45F9CE | 48B2E0 | d3dx9_37.dll!D3DXVec3Project |
| 45F9EC | 48B2CC | d3dx9_37.dll!D3DXMatrixRotationX |
| 45F9F2 | 48B2C8 | d3dx9_37.dll!D3DXMatrixMultiply |
| 45F9F8 | 48B2C4 | d3dx9_37.dll!D3DXMatrixRotationY |
| 45F9FE | 48B2C0 | d3dx9_37.dll!D3DXMatrixRotationZ |

Each full thunk is one JMP [IAT]. Independent original-PE import-descriptor
decoding verifies DLL and symbol names; full Ghidra review verifies the body,
references and adjacent separate entries. These exclusions cover the thunks,
not the external DLL implementations, and grant no maintained-source credit.
Custom engine utility roles and a math/header-adapter role are insufficient
to establish authored origins for the other nine owners.

## Compiler investigations and remaining work

Private probes cold-compile all seven source owners with pinned VC9 /GL,
the real quad/submission/batch support and independent renderer/VM/output,
distance and flag inputs. A baseline, inline tint, dynamic-flag context,
frame-pointer profile and distance-lifetime variants were compared over full
PDB contributions. No diagnostic comparison grants exact credit.

The /Oy- experiment does not recover native alignment or allocation and is
not adopted. The sqrt adapter still lacks its native8-byte-aligned frame.
Projected construction differs in frame allocation and cached-register use;
fog retains billboard private-ABI, distance-expression and scheduling gaps.
Remaining wrappers also depend on the opaque rotated/billboard callees.
The maintained source uses the ordinary /O2 /Ob1 /GL diagnostic profile.

| Initial projected-context contribution | Candidate bytes / diagnostic differences |
| --- | ---: |
| AnimationDistanceRoot | 20 /26 |
| DrawAdaptiveUnroundedAnimationVm | 115 /67 |
| DrawBillboardAnimationVm | 38 /30 |
| DrawFoggedBillboardAnimationVm | 795 /666 |
| BuildProjectedAnimationVmQuad | 779 /666 |
| DrawProjectedAnimationVm | 48 /24 |
| BuildAnimationVmModeQuad | 124 /90 |

Initial full diagnostics: `build/probe-ltcg/projected-quad/distance-lifetime/diagnostic.json`
and `.analysis/projected-quad-distance-lifetime.log`. These positional byte
differences include shifts from different entry sequences; neither equal
length nor a small aligned region establishes whole-function equality.

The976-byte billboard and571-byte rotated bodies use paired FSINCOS with float
result stores. VC9 /O2 /Oi probes of float/double sin/cos calls invoke CRT
intrinsics under precise mode; /fp:fast emits separate FSIN/FCOS. Neither
recovers FSINCOS, so those bodies remain open. The billboard owner also has an
initial pair of operations whose outputs are overwritten by a later pair;
their floating-point side effects are not assumed removable.

For the1,112-byte submission body, a direct `#pragma intrinsic(_frnd)` probe
produces VC9 warning C4163 and retains the real CRT call. This does not resolve
its inline FRNDINT gap. No target instruction bytes or replacement trigonometric
approximations were introduced.

Private evidence: `vm-draw-more-review.txt`, `vm-rotation-neighbors-review.txt`,
`billboard-full-span-review.txt` and their attestation logs;
`projected-quad-independent-review.json`; `projected-d3dx-boundaries-review.txt`
and `projected-d3dx-import-proof.json`; `square-root-callee-entry-review.txt`;
`math-pair-precise.asm`, `math-pair-fast.asm` and `frnd-intrinsic.cpp`.
Current probe artifacts/logs are under `build/probe-ltcg/projected-quad` and
`.analysis/projected-quad-*.log`. Compiler payloads, extracted math objects,
target text and decompiler hypotheses remain private.

At the initial projected checkpoint no existing canonical family changed, so
no old exact unit or full cohort was cold-replayed. Its seven mappings were
non-exact and exact state remained78 functions/14,988 bytes.

The subsequent [immediate-mode checkpoint](ANIMATION_IMMEDIATE_DRAW.md) adds
dispatcher source, corrects its topology labels to strip/fan and establishes
int status returns for four wrappers, including adaptive unrounded drawing.
Its ten-unit cold proof refreshes the affected prior families; the projected
diagnostic snapshot above predates this return-type correction.
