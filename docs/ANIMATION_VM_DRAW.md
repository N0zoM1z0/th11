# VM submission and draw dispatch

The complete1,112-byte submission body now has maintained source in
`src/AnimationVmDraw.cpp`, along with its unrounded coordinate builder and
three dispatch wrappers. These five mappings cover1,869 target bytes.
Six additional boundary/origin reviews cover the five neighboring owners
below and the existing CRT primitive. Five engine origins remain
indeterminate; the verified CRT member is classified as a library exclusion.

| Function | Address | Target bytes | Current state |
| --- | --- | ---: | --- |
| DrawAnimationVm | 44F880 | 1,112 | Source present; non-exact |
| DrawUnrotatedAnimationVm | 4500B0 | 51 | Source present; non-exact |
| BuildUnroundedAnimationVmQuad | 4500F0 | 537 | Exact linked proof |
| DrawUnroundedAnimationVm | 450310 | 54 | Exact linked proof |
| BuildRotatedAnimationVmQuad | 4503D0 | 571 | Reviewed boundary; source open |
| DrawAdaptiveAnimationVm | 450610 | 115 | Source present; non-exact |
| CRT _frnd | 489CED | 20 | Library origin; excluded |

Direct local Ghidra reviews attest the complete mapped target text and inspect
all control flow, references and neighbors. Independent pinned Capstone
decodes each complete span. These owners are contiguous and contain no tables
or external interior references. The CRT RET at489D00 is immediately followed
by a separate entry489D01; no intervening padding is borrowed.

## Existing CRT primitive resolves the rounding contract

The old assumption that submission source required an invented rounding
helper was incomplete. Original TH11 already contains `__frnd` at489CED.
It accepts a stack double under cdecl, executes FRNDINT, spills/reloads a
double result, and returns it in ST0. It does not change the rounding control.
Its entire20-byte code equals the complete no-relocation `.text` section of
`_frnd_.obj` from the paired VC9 RTM `libcmt.lib`; the archive symbol is also
`__frnd`, corresponding to C name `_frnd`.

| Identity | SHA-256 |
| --- | --- |
| libcmt.lib | `bd8fe9fa475deb84014af41870182b447bb969c91661d7012ae8d1b26bb6a86c` |
| _frnd_.obj | `98a2147113173dfda8a91fef3843c2d756b6f2527b0c494cbc317fe30cfcba8f` |
| Complete20-byte text | `72f961779c37bf40bf7b9ebf9eb6c9ae108b73a5b2679913630701ba5e7c65b2` |

`CrtRounding.hpp` declares this actual historical C boundary. The submission
source calls it for the four float inputs and then subtracts0.5 before the
float stores. Integral results from finite float inputs are exactly
representable as double, so the helper's double result store introduces no
additional value rounding there. No instruction bytes, inline assembly,
fixed-nearest assumption or floor substitution is introduced. The current
source still calls the CRT routine instead of reproducing the native inline
FRNDINT codegen, so the submission function earns no exact credit.

## Submission owner ABI correction (2026-11-21)

The complete body is a renderer-owned private member, not a free three-argument
`__stdcall` routine: ECX is the renderer, EAX carries the VM, and the byte flag
is the one stack argument (`RET 4`). Both the full source and the combined
renderer-owner context now express the body as
`AnimationVmDrawRendererView::DrawAnimationVm`; this zero-storage derived view
has a compile-time size equality check against `RenderBatchView`. A thin free
adapter remains for source callers that still use the shared three-argument
API, while the recovered immediate wrappers call the member directly. The
focused VC9 `/GL` build now emits the correctly decorated member contribution
(1,190 bytes against the 1,112-byte target). This corrects the owner ABI but
is still source-present/non-exact; the remaining codegen gap includes the
four CRT `_frnd` calls in place of inline FRNDINT and stack/register scheduling.

A pinned-VC9 console check linked the real CRT and called `_frnd` on +1.5/-1.5
in four x87 modes: nearest2/-2, down1/-2, up2/-1 and chop1/-1. Each mode remained
unchanged across calls. This resolves the C ABI/rounding-mode question.

## Submission order and unordered comparisons

The source preserves offset addition for all scratch vertices, optional
pixel alignment, UV assignment, bounds reduction, unsigned viewport overlap,
texture/geometry transitions, optional color/tint replacement, state setup,
and six-vertex enqueue. Viewport additions wrap as unsigned32 before
conversion, matching the native ADD/FILD/correction sequences. The pinned
candidate retains those values in x87 for comparison without an intervening
float store.

The original rejects only ordered viewport separations. NaN bounds therefore
do not satisfy those reject comparisons; rewriting this as a chain of
positive `<=` tests would change the paths. First-corner min/max selection
also has an explicit tie/unordered choice. The maintained comparisons were
reviewed against FCOM/FNSTSW/TEST branch conditions rather than accepting
Ghidra's higher-level floating-point predicates.

Texture cache+4355BC is updated before a flush, then re-read for SetTexture.
Geometry changes flush before storing1 at+4355C2. Flag2 preserves the scratch
colors. Otherwise VM flag8000 selects alternate color+378 instead of primary
+374; enabled tint+7BD88C/+7BD890 scales red, green, blue and alpha in that
order using the already recovered byte-channel helper. Submission returns0
on accepted and culled paths.

## Coordinate and dispatch contracts

The537-byte coordinate builder uses the same dimension/anchor storage as the
632-byte builder, but never floors centered positions. It still rounds half
sizes to float, re-reads VM fields after stores that can alias, leaves an axis
unchanged for selector3 and assigns every Z. Those float rounding points and
the early half-height value are needed for the complete native codegen.

Unrotated dispatch builds shared corners and submits flag1. Unrounded dispatch
builds with the537-byte helper and submits flag0. Adaptive dispatch tests the
float at VM+2C: positive/negative zero selects the floor-centered path with
flag1; nonzero or unordered selects the real rotated boundary with flag0.
No missing-VM or fallback behavior is invented.

The571-byte rotated owner contains FSINCOS, anchored local corners and rotation
around the origin. Selector3 leaves the corresponding local axis unwritten
in the native code. Natural C++ source remains in `AnimationVmRotation.cpp`;
its separate `sin`/`cos` operations still do not reproduce paired x87 FSINCOS.
The shared renderer context now exposes a complete479-byte natural-C++ candidate
against the reviewed571-byte owner; the remaining gap includes the native
FSINCOS lowering and surrounding float/register scheduling. No initialized
fallback corners are substituted. The adaptive wrapper
and mode dispatcher now share the broader `AnimationVmRendererOwner.cpp`
context; both remain non-exact while private register allocation is open.
The complete mode dispatcher is276 bytes:217 code, three alignment bytes and
56 bytes of jump table. Its current `/O2 /Ob1` natural-C++ candidate is260
bytes:203 code bytes, one alignment byte and the same complete table. The
target saves EBX/ESI/EDI at entry and restores all three on every exit; the
candidate saves ESI/EDI at entry but shrink-wraps EBX around the world branch,
leaving a16-byte callee-save placement gap. Reviewed callers consistently
pass the VM in EAX and renderer in ECX; the source uses the recovered default
member `ImmediateVmView::DrawByMode`. Its 0x408-byte view asserts the accessed
color/flag offsets, but the original C++ class identity remains unproved.

## Full cold proofs and ownership limits

The real submission/dispatch caller context also closes two previously open
helpers in `AnimationQuad.cpp`: the complete632-byte coordinate builder and
40-byte floor adapter. Neither helper's source was padded or given an
explicit alignment. The native16-byte coordinate frame and64-byte-aligned
floor frame arise naturally in this context. The old narrow context produced
632/53 and29/35 diagnostics. The original TU partition remains unknown.

Four new exact units add1,263 bytes:

| Unit | Bytes | Reviewed fields |
| --- | ---: | ---: |
| build-animation-vm-quad | 632 | 3 |
| floor-animation-coordinate | 40 | 1 |
| build-unrounded-animation-vm-quad | 537 | 1 |
| draw-unrounded-animation-vm | 54 | 6 |

Fields bind the double0.5 constant at497E60, floor adapter4520C0, double floor
entry486590, all four shared-quad addends rooted at4C91D8, and the two native
builder/submission calls. Both families have current source/tool/artifact
receipts, complete public primary-TU PDB ownership and PE/MAP/PDB identity.
Every declared field and every remaining byte replay with zero differences.
The color25 and appender142 units were replayed because their canonical
AnimationQuad caller context changed; both remain exact. Only these six
units were cold-built, not the full cohort.

Current source contributions are submission1,190 bytes/858 diagnostic
differences, unrotated wrapper49/27, adaptive wrapper117/60 and state348/216.
The unrotated frame and the opaque rotated callee's private register behavior
remain concrete wrapper gaps. Submission still differs in CRT call/inlining,
frame and operand scheduling; complete source is not byte equality.

Canonical artifacts are `build/match-linked/AnimationQuad` and
`build/match-linked/AnimationVmDraw`; rebuild selected units through
`config/match-units.toml`.

The subsequent [projection/fog review](ANIMATION_PROJECTED_QUAD.md) supplies
seven more source mappings, reviews the complete976-byte billboard owner and
records the precise/fast sin-cos and rejected `_frnd` intrinsic experiments.
Those new diagnostic families do not replace the canonical receipts above.

The [immediate-mode caller audit](ANIMATION_IMMEDIATE_DRAW.md) establishes int
status returns for the three draw wrappers. The54-byte unit's decorated symbol
reflects that return type; all six affected units passed full exact replay.

## Rotated quad owner now source-present (2026-11-11)

The complete reviewed **571-byte** `BuildRotatedAnimationVmQuad` boundary at
`0x4503D0` now has maintained natural C++ in `AnimationVmRotation.cpp`.
The source uses the VM angle at +2C, dimensions and scales at +4C/+3C,
three position vectors beginning at +3DC, and the two independent 2-bit
anchor selectors at flag +404 bits 18 and 20. It constructs four local
corners, rotates each with cosine and sine, adds the shared XY center, and
publishes one common Z. Selector 3 does not initialize the corresponding
local axis, matching the native absence of an assignment; no invented
fallback/corner validation is added.

The pinned VC9 `/O2` normal-COFF probe compiles to **609 bytes** rather than
the original 571, making separate CRT sin/cos calls instead of the target's
single `FSINCOS` operation. `/GL` compilation also succeeds. This owner is
**source-present, non-exact**; recovering paired x87 codegen and native
private-register placement remains the next issue. The existing exact
unrounded builder and wrapper have separate canonical contracts.
