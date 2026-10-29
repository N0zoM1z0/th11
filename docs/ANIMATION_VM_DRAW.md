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

A small pinned-VC9 console check links the real CRT and calls `_frnd` on
+1.5/-1.5 in four x87 modes. Results are nearest2/-2, down1/-2, up2/-1 and
chop1/-1. Each mode remains unchanged across the calls. This resolves the
specific C ABI/rounding-mode question; it is not a game/runtime regression
suite or a general floating-point equivalence claim. Private evidence:
`.analysis/frnd-library/identity.json`, `abi-check.log`, `check.cpp` and
`run-check.py`. Compiler/library payloads and extracted objects stay private.

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
in the native code. Its source is still open; separate sin/cos approximations
or initialized fallback corners are not substituted.

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

Canonical artifacts: `build/match-linked/AnimationQuad` and
`build/match-linked/AnimationVmDraw`. Cold log: `.analysis/vm-draw-six-cold.log`.
Other private evidence: `frnd-contract-review.txt`,
`quad-unsnapped-rotated-review.txt`, `quad-dispatch-callees.c` and attestation
logs; `animation-vm-draw-*.log` compiler probes; complete current diagnostics
`vm-draw-final-diagnostic.json`. No copied CRT or target code is committed.
