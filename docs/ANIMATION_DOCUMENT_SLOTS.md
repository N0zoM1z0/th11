# Animation document slot lifecycle

Two adjacent native owners, `UpdateAnimationDocumentSlots` at
`0x4548D0..0x454956` (135 bytes) and `FreeAnimationDocumentSlot` at
`0x4549A0..0x4549D2` (51 bytes), now have maintained C++ in
`src/AnimationDocumentSlotsLifecycle.cpp`. Both use the 32-entry document
array at renderer offset `0x4350C0` and the existing typed animation
document view. Their original C++ class and translation unit are unknown.

## Native behavior and ownership

The 135-byte scan contains 50 instructions, three calls and two returns.
It scans non-null slots, destroys documents with the +0x128 pending
flag, and advances +0x124 busy documents through the separate
`AdvanceAnimationDocument` owner at `0x4544C0`. The continuation
status returns zero for a successful advance or minus one for failure.
The pinned native body reloads the slot after destruction and clears
its pending flag **without introducing a new null check**. The source
retains this observed path; its apparently unsafe behavior is an open
original-runtime question rather than grounds for modifying the native
control flow. The complete scan remains **source-present, non-exact**.
The initial candidate was 115 bytes in both ordinary COFF and focused
`/GL`; focused codegen has now recovered the complete **135-byte**
native shape as described below.

The 51-byte indexed release contains 14 instructions, two calls and one
plain return. The native receiver resides in EBX and unsigned slot
index in ESI. It rejects indexes >=32, destroys a non-null document,
deletes its object and zeroes its slot. The complete source is a normal
C++ function; no custom register ABI or instruction bytes are added.

## Strict exact proof for indexed release

An isolated related `/GL` context includes both source owners, the
actual animation-document cleanup, menu VM marker, chunk advance and a
compile-only dynamic renderer/index caller. That context yields a
complete 51-byte primary PDB contribution, with **48/51 literal raw
bytes** already equal before address rebinding. The remaining three
byte differences fall entirely inside two reviewed REL32 call fields:
`+0x12` to `DestroyAnimationDocument` at `0x4549E0` and `+0x1F`
to C++ `operator delete` at `0x45FD49`.

Canonical unit `animation-document-slot-free` cold recompiles the
source and its scoped dependencies, checks the full native 51-byte
boundary, the PE/MAP/PDB function owner and all producer receipts,
then replays both fields. Result: **exact 51/51, zero differences**.
The newly exact release owner does not make the adjacent 135-byte
scan exact.

Scoped private evidence:
`.analysis/animation-document-adjacent-review-20261111.txt`,
`.analysis/animation-document-slots-context.log`,
`.analysis/animation-document-slot-free-cold-proof.json`,
and `build/match-linked/AnimationDocumentSlotFree/`.


## Scan control-flow recovery (2026-11-11)

The initial 115-byte focused `/GL` contribution omitted native
loop-helper's signed lower bound and selected a synthetic volatile
load/test for the busy status. It also passed the cached slot pointer
to `AdvanceAnimationDocument` rather than reloading the file by its
index from the saved renderer.

The maintained C++ now keeps the renderer base separately, explicitly
preserves the native signed nonnegative **and** unsigned upper-bound
checks on the inlined slot deletion, reads physical busy+124 as an
ordinary memory test, and independently reloads the indexed slot for
the busy call. This is all normal C++ state-machine logic, not
inserted machine code.

In the five-support-TU VC9 `/GL` context, the complete primary PDB
owner now generates **135 bytes / 50 instructions, three calls and
two returns** against native **135 / 50 / three / two**. One reviewed
DIR32 renderer field at offset5 and three native REL32 call fields
at offsets2F/37/77 account for 10 of 11 raw unequal bytes. The
remaining **single non-field byte at offset70** is the ModRM
register selector on the busy-path file reload: the original writes
EDI for the private callee, while the current linked
`AdvanceAnimationDocument` source accepts EAX. A separate normal
advance caller and multiple equivalent local return spellings
did not change that register assignment. No exact credit is
claimed for the scan or the currently non-exact 212-byte advance
callee.

The neighboring accepted `animation-document-slot-free` remains
independently proven exact (51/51 bytes with two REL32 fields),
including after a new **scope-only cold build** of the revised
source. Private comparisons and codegen variants:
`.analysis/animation-slot-native-branch-links.log`,
`.analysis/animation-slot-advance-abi-links.log`,
`.analysis/animation-slot-add-advance-probes.log` and
`build/probe-ltcg/AnimationDocumentSlots/`.


## Downstream ABI probe following decoder exact

Recompiling the sole 135-byte scanner under a limited /GL context
after `DecodeAnimationChunk` became independently exact retained
the full **135-byte / three-call / two-return** scan, but still
used EAX instead of the native EDI for the private parameter to the
212-byte `AdvanceAnimationDocument`. The 811-byte decoder is exact
under its own six-support-TU producer context; with the narrower
scanner producer context it is a different PDB contribution and
cannot be claimed exact merely by reusing the same source.
The scanner remains non-exact with the single previously reviewed
register ModRM difference.
Evidence: `.analysis/animation-slot-after-decoder-boss-linked.log`.


## Strict exact 135-byte scan via real decoder producer (2026-11-12)

The earlier narrow scanner link left one non-relocation ModRM byte at
`+0x70` (`MOV EAX,[EDX+EBX*4+...]` instead of the native
`MOV EDI,[EDX+EBX*4+...]`). The underlying issue was **not** the
scanner's final return spelling: its `AdvanceAnimationDocument` callee
was being over-specialized because the compiler could not see the actual
811-byte `DecodeAnimationChunk` producer family. The real decoder uses
six support translation units for texture conversion, transparent pixels,
sprites, diagnostics and resource loading.

Linking those actual sources with the existing native slot owner,
`AnimationChunk.cpp`, the real document cleanup, menu marker and dynamic
renderer/index caller naturally restores the EDI private argument ABI.
Now the complete **135/135-byte, 50/50-instruction, 3/3-call, 2-return**
`UpdateAnimationDocumentSlots` body is **canonical strict exact**. Its
four independently reviewed fields are renderer DIR32 `+0x05 -> 0x4C3268`
and three REL32 calls to `DestroyAnimationDocument`
`+0x2F -> 0x4549E0`, C++ `operator delete` `+0x37 -> 0x45FD49`,
and `AdvanceAnimationDocument` `+0x77 -> 0x4544C0`. There are **zero
remaining differences** in a fresh, fully owned PDB/MAP/PE contribution.
Source and codegen retain the native unchecked slot load after deletion;
no extra guards, register shims, custom machine bytes or padding were added.

The proof unit is `animation-document-slot-update` in
`config/match-units.toml`; acceptance receipt:
`.analysis/animation-document-slot-update-strict-20261112.log`.
The exact dependent 811-byte decoder and 51-byte indexed release also
compile with **zero differences** under the same diagnostic producer
(`.analysis/probe-slot-real-decoder-20261111.py`). The separate 212-byte
`AdvanceAnimationDocument` now compiles to **211/212 bytes** with genuine
decoder support; that callee remains **non-exact**, despite the scanner's
strict acceptance. Historical one-byte ABI diagnostic above is superseded.
No full cold replay was run.
