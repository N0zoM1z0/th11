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
control flow. The complete scan is **source-present but non-exact**:
pinned VC9 ordinary COFF and focused `/GL` each generate 115 bytes,
versus native 135, retaining the three calls and two returns.

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
