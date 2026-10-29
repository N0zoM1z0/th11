# Animation quad construction and submission review

Six further complete target owners have boundary and origin reviews. Five helper owners have source in `src/AnimationQuad.cpp`. The1,112-byte
submission body is now maintained separately in `src/AnimationVmDraw.cpp`; see
[VM submission](ANIMATION_VM_DRAW.md). Authorship is indeterminate for
all six: their custom renderer/ANM role is established, but that does not
independently distinguish game authorship, reused engine utilities or (for the
floor adapter) compiler/library support.

| Function | Address | Complete target bytes | Source |
| --- | --- | ---: | --- |
| ScaleAnimationColorChannel | 44F490 | 25 | Present |
| ApplyAnimationVmDrawState | 44F710 | 368 | Present |
| DrawAnimationVm | 44F880 | 1,112 | Present; non-exact |
| AppendRenderQuad | 44FDA0 | 142 | Present |
| BuildAnimationVmQuad | 44FE30 | 632 | Present |
| FloorAnimationCoordinate | 4520C0 | 40 | Exact linked proof |

## Complete boundaries

Direct local Ghidra queries verify the complete mapped TH11 text before
reviewing callers, references, code and neighboring bytes. Pinned Capstone
independently decodes every byte in the five contiguous owners, including all
instructions after the standard review's250-instruction display limit in
DrawAnimationVm. No external interior references or embedded tables occur in
those five bodies.

The state function's old338-byte inventory ended at44F861. Its jump at44F74D
indexes a seven-entry table at44F864. Two alignment bytes44F862..44F863 and
all28 table bytes through44F87F belong to this owner:368 bytes total. The
seven references back into case bodies all originate in that owned table;
there is no remaining external interior reference. The next byte starts the
independent44F880 body. Do not crop the table to obtain a match.

## Position construction and alias order

The VM view has scales+3C/+40, dimensions+4C/+50, position+3DC, offset+3E8,
extra offset+3F4 and flags+404. Scaled width and height have float rounding
points. Centered X/Y subtract float-rounded half-dimensions, then use the
separate float-to-double floor adapter. Anchors1/2 align directly at the
origin or one complete dimension before it. Anchor3 leaves that axis's
coordinates unchanged. Y's selector is re-read after X stores. Every path
assigns all four Z values.

The corner order is top-left, top-right, bottom-left, bottom-right. Source
preserves the native write order and repeated VM reads: output pointers can
alias each other or VM storage. In particular, direct anchored branches
recompute the origin after the first coordinate stores; caching all origins
would change the observed data dependencies.

4520C0 converts its float input to double, calls486590, rounds the result back
to float and returns through x87. The callee's independently observed fast
path truncates nonnegative fractional mantissas; for negative nonintegral
inputs it subtracts1 from the truncated value. This supports `floor`, not
current-mode rounding. The adapter's64-byte-aligned frame is now reproduced naturally in the real submission
caller context. Natural `float(floor(double(value)))` source has no artificial
alignment or padding; the complete40-byte owner has a cold exact proof.

Ghidra's inferred486590 function is sparse and extends into4870E4 fallback
code across many other candidate functions. That auto-analysis extent has not
been accepted, replaced or origin-classified by this checkpoint.

## Cached draw state

Renderer state fields are applications+A8, cached texture+4355BC, blend byte
+4355C0, geometry byte+4355C2 and point-filter byte+4355C6. Blend selection is
VM flags bits4..6. On change, the function flushes, re-reads the flags, updates
the cache and selects these D3D9 source/destination blend pairs:

| Selector | Source | Destination |
| --- | --- | --- |
| 0 | SRCALPHA (5) | INVSRCALPHA (6) |
| 1 | SRCALPHA (5) | ONE (2) |
| 2 | ZERO (1) | INVSRCCOLOR (4) |
| 3 | ONE (2) | ZERO (1) |
| 4 | INVDESTCOLOR (10) | INVSRCALPHA (6) |
| 5 | DESTCOLOR (9) | ZERO (1) |
| 6 | INVSRCCOLOR (4) | INVSRCALPHA (6) |

Selector7 updates the cache and issues no blend API calls. Filter selection
comes from VM bit31. Changes flush before re-reading/storing the selector,
then set stage0 MAGFILTER/MINFILTER to LINEAR2 or POINT1. The applications
counter increments once on every invocation, including unchanged state; it is
not a count of actual D3D calls. Neither texture nor geometry transitions are
performed in this state function.

## Quad queue and tint channel

AppendRenderQuad copies six complete28-byte vertices in order0/1/2/1/2/3,
reloading the destination base before every copy. It advances nextVertex by
168 bytes, increments quadCount and returns0. It does not flush, test capacity
or alter the source quad. These details matter for aliased inputs.

ScaleAnimationColorChannel zero-extends both input bytes, multiplies them,
shifts the unsigned product right7, and clamps results>=256 to255. Scale128
is the identity. Byte parameters preserve the native AL/CL contract; an
explicit unsigned promotion preserves logical SHR. With byte promotion left
as signed int, VC9 emits SAR instead, differing at the single shift opcode.

## Submission source and current-mode rounding

44F880 adds renderer camera offsets to all four scratch corners, optionally
rounds positions, fills UVs using VM+54/+58 offsets, derives quad bounds and
compares them against unsigned viewport X/Y/Width/Height from runtime+54C's
camera pointer. The comparisons include touching edges. Accepted quads
perform cached texture and geometry transitions, optionally replace colors
from VM+374/+378 and tint state, apply the state function, and enqueue the
scratch quad. The function returns0 even when culled.

Native pixel alignment is four FRNDINT operations followed by subtraction of
float0.5 at4973F8 and propagation of the aligned rectangle corners. FRNDINT
uses the current x87 rounding-control mode. `floor`, integer truncation or
an assumed fixed nearest mode is not equivalent. The existing CRT primitive
`_frnd` at489CED has now been identified against the pinned VC9 library in all
20 bytes. Complete submission source uses that real primitive in natural C++;
its current-rounding-mode ABI was checked independently. Native inline
instruction codegen remains a gap. See ANIMATION_VM_DRAW.md for the evidence.

## Reproduction

Private target evidence: `.analysis/quad-render-review.txt`,
`quad-helpers-review.txt`, `quad-positions-native.asm`, `quad-draw-native.asm`,
`quad-owners-review.json` and their direct-Ghidra attestation logs. Floor
callee hypotheses remain private `quad-floor-callee.c`.

Compiler probes: `.analysis/probe-animation-quad.py` and
`probe-quad-source.py`; output variants under `build/probe-ltcg/animation-quad`.
Source profiles stay VC9 RTM `/TP /O2 /Ob1 /Gy /MT /Isrc /EHsc /GL`.
`probes/animation-quad-context.cpp` supplies independent owners, corners,
colors and floor input, with real `RenderBatch.cpp` support. The linked image
is a diagnostic context, not the game.


## Current exact and compiler evidence

The expanded canonical caller context includes the actual submission and its
dispatch wrappers. It closes the complete632-byte coordinate function and
40-byte floor adapter with three/one independently reviewed fields. Color25
and appender142 remain exact after cold replay in that same context. The new
VM-draw primary family also closes the537-byte unrounded coordinate function
and54-byte unrounded dispatch. All six full primary-TU PDB contributions,
receipts, PE/MAP/PDB identities and exhaustive field/byte replays agree.
Original TU partition and complete data ownership remain unknown.

The former narrow caller context produced632/53 and29/35: every coordinate
difference was ESP-relative addressing or frame allocation. Arrays/two-field
structs worsened the coordinate differences to72; double locals produced
651-byte coordinate bodies and were rejected. Real caller reconstruction
closes the gaps without modifying either helper or introducing padding,
explicit alignment or ABI tricks. The state contribution remains348/216
against368; shared blend-call blocks and register/filter scheduling are open.

Latest cold log: `.analysis/vm-draw-six-cold.log`. The old narrow-context
compiler findings remain private `animation-quad-*.log` and
`animation-quad-final-diagnostic.json`; they are historical context, not the
current exact state. [VM submission](ANIMATION_VM_DRAW.md) records the current
proofs and the still non-exact1,112-byte submission owner.
