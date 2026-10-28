# Stage construction, initialization and destruction

The stage lifecycle now has maintained natural C++ source, including real
member constructors/destructors and two automatic VM arrays. This is still
part of the reconstruction, not a closed native game build.

| Function | Complete native code span | Current source bytes | State |
| --- | --- | ---: | --- |
| StageEmbeddedVmDestructor | 0x00402170..0x00402191, 34 bytes | 34 | canonical linked-PE exact |
| StageEmbeddedVmConstructor | 0x00402270..0x004022D4, 101 bytes | 101 | canonical linked-PE exact |
| StageResourceConstructor | 0x004023F0..0x004024A3, 180 bytes | 180 | diagnostic address-field match; EH owner remains open |
| InitializeStageResource | 0x004024C0..0x00402735, 630 bytes | 630 | canonical linked-PE exact |
| StageResourceDestructor | 0x00402740..0x0040299A, 603 bytes | 597 | non-exact; EH owner also open |
| CreateStageResource | 0x004029C0..0x00402A5E, 159 bytes | 159 | diagnostic address-field match; EH owner remains open |
| StageUpdateCallback | 0x00403900..0x00403906, 7 bytes | 7 | non-exact private callee ABI |
| StageDrawCallback | 0x00403910..0x00403916, 7 bytes | 7 | canonical linked-PE exact |
| StageOverlayCallback | 0x00403920..0x00403926, 7 bytes | 7 | canonical linked-PE exact |

Direct hash-attested Ghidra review covers complete contiguous code, all exits,
incoming/interior references, adjacent RET/CC and helper/caller contracts.
There are no embedded tables or external code-interior references in these
spans. The three callback entries were missing from the provisional inventory;
explicit seven-byte span reviews establish them without changing the private
database or refreshing maintained inventory rows. Their implementation origin
is indeterminate: wrappers cannot be attributed to hand-written source versus
compiler-generated adapters from instructions alone. Other new lifecycle rows
have reviewed game-specific stage/VM ownership and initialization integration.

Private evidence: `.analysis/stage-initialize-review.txt`,
`stage-vm-review.txt`, `stage-update-entry.txt`, the three
`stage-*-callback-span.txt` files, `stage-vector-helper-review.txt` and
`stage-lifecycle.c`. Default decompiler signatures omit the initializer's
filename and some real callback inputs; complete instructions take precedence.

## Physical view and constructors

The factory's allocation and constructor's clear independently establish a
0x3158-byte stage storage span. Unknown fields retain byte regions; the source
class names do not claim the original class declaration or TU partition.
The first fields are flags+0, callback pointers+8/+C, copied blob+10, object/
section pointers+14/+18/+1C and six20-byte timer objects. Embedded VM arrays
start at+180 (eight0x434-byte objects) and+2348 (three); distanceSquared is+2320,
flags+2FF0, final timer+2FF4, variant+3008, reset+300C, binding block+3010,
overlay callback+3034, source/size+3038/+303C and camera+3040 (0x118 bytes).
Static size/offset assertions check the recovered physical layout.

StageEmbeddedVm constructs eleven timer flags, clears its entire0x434 bytes,
then sets only the signed16-bit marker+39C to-1. It does not perform the identity
matrix/scale setup of ResetAnimationVm. Its destructor frees optional storage
at+400 and clears that pointer even when initially null. StageResourceView's
constructor constructs five timers, eight VMs, three VMs and the final timer in
member order, then clears the entire0x3158 bytes and ORs flags with2. Thus its
whole-object clear deliberately overwrites the VM markers just initialized to
-1. The source preserves that behavior rather than repairing those markers.

## Complete initializer

InitializeStageResource copies the variant from0x004A5728 and publishes the
stage at0x004A8D60 before LoadStageResource. Any nonzero load result logs the
stage error and returns-1 without undoing publication. Success copies0x118 bytes
from runtime+434, then sets camera position(0,0,-600), target offset(0,300,600),
up(0,1,0), eye offset(0,0,0) and distanceSquared9610000.

It allocates three0x24-byte chain elements, initializes their real link/callback
fields, preserves unrelated flag bits, clears bit1 and sets bit0. Registration
is update priority12, draw priority2 and overlay draw priority5. Each pointer is
published only after registration returns; its return value is ignored. A null
allocation is not checked before later field access and retains the native
fault. After registration it clears+300C, sets the timer+38 frame to-1 with the
observed lazy-scale initialization, ORs+2FF0 with1 and clears+94/+E0.

The first family gave625 bytes/293 diagnostic differences: the only update
registration caller passed12, allowing VC9 to specialize that argument away.
An independent receiver/priority probe restores the real variable-priority
registration contract. Restoring vector temporary evaluation before the scalar
distance store also reproduces native scheduling. Natural C++ then gives the
complete630-byte body with zero differences; no ABI annotations, padding or
instruction bytes were used.

## Destruction and factory behavior

The destructor unlinks update, draw and overlay callbacks in that order.
For each non-null pointer it captures the chain manager, conditionally enters
critical section0 and increments its byte depth, unregisters, and re-tests
runtime0x8000 before leaving/decrementing. Helpers may change flags or the
manager; the captured manager and asymmetric conditional-leave behavior are
retained. The stage's callback fields are not cleared by this body.

It then frees/clears copied data, cached source and the VM allocation; releases
and deletes the captured binding block; optionally destroys the animation
document in slot `(variant&1)+3` when global0x004A5758 bit0 is clear; and clears
the two global stage aliases only when equal to this object. Document deletion
reloads the slot after its helper returns. Automatic member destruction then
visits the three VMs in reverse order, followed by the eight VMs in reverse
order. Natural C++ provides the real EH/member cleanup structure; no target
exception bytes or fabricated ABI are embedded. The source destructor remains
597 bytes/143 unequal diagnostic positions and grants no exact credit.

CreateStageResource allocates/constructs a stage, invokes initialization, and
destroys/deletes it on initialization failure. Native null allocation still
reaches initialization and faults; the source adds no early-null return.
The180-byte constructor and159-byte factory have diagnostic zero differences
after address-field masking, but their EH handlers/funclets and ownership are
outside the current exact acceptance lane. They are not canonical exact units.

## Strict exact proofs

```bash
scripts/repo-python scripts/replay-exact-units.py \
  --unit initialize-stage-resource \
  --unit stage-embedded-vm-constructor \
  --unit stage-embedded-vm-destructor \
  --unit stage-draw-callback \
  --unit stage-overlay-callback
```

These five non-EH public primary-TU contributions add779 exact bytes.
Initializer bindings number21; VM construction/destruction and the two drawing
callbacks have one each, for25 independently reviewed fields. Sources, headers,
tools, producer and PE/MAP/PDB identity/ownership are receipt-bound; complete
extents and all bytes agree after exhaustive replay. The four float constant
contents were separately checked against the target. Callback proofs use
StageCallbacks.cpp as primary, not a borrowed support-TU owner. Earlier45 exact
contexts were unchanged and were not replayed.


Constructor/destructor/factory EH and specialized-owner gaps remain open.
The older loader contexts remain non-exact:297/434/131 source bytes. The new
lifecycle context's loader has297 bytes/25 diagnostic differences, compared
with297/17 in the earlier family. Keep those contexts distinct when pursuing
byte equality; broad runtime/fault matrices are not checkpoint requirements.
