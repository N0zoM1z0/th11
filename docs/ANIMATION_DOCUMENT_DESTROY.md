# Animation document cleanup, 004549E0

This is the complete **243-byte** owner at `0x4549E0..0x454AD2`,
now source-present in `src/AnimationDocumentDestructor.cpp`. It is
called by the player destructor at 43011C and twelve other reviewed
caller groups.

## Original code and behavior

The pinned original contains 75 x86 instructions / 243 bytes, with
six bytes of alignment LEA between Ghidra's body ranges
4549E0..454A09 and 454A10..454AD2. It makes eight calls and has one
plain return, with no owned jump table or foreign interior references.

If the document has no source data at +108, it returns. Otherwise
it marks file-owned VMs via the renderer, iterates over each 20-byte
texture entry at +120 (count +10C), releases non-null Direct3D
textures through their virtual Release slot and frees per-texture
source data. Finally it frees and nulls five optional document
allocations: textures +120, sprites +118, scripts +11C,
auxiliary storage +130, and source data +108. The original makes
a final data-pointer null test independent of its initial guard.
The existing shared view declares +130 as an unknown DWORD; the
new source locally models the pointer without migrating other
translation units' shared input.

## Focused compiler diagnostics

Pinned VC9 normal COFF is **246 bytes / eight relocations**, with
**167/211 comparable bytes** agreeing with the original after
relocation masking. Natural for, while and do/while forms of the
texture loop all generate identical COFF code. The explicit final
data guard improved nonfield agreement from the initial 152/211.

Two isolated `/GL` contexts compile the source: the direct owner
and real menu marker, and a larger context with the source-present
player destructor, initializer, stage VM lifecycle and factory.
Both produce a full PDB-owned **229-byte** document-destruction
function with **eight calls and one return**, against original
243/8/1. The per-texture release loop is close to native. Remaining
differences include the ESI renderer register passed to the
menu-marking callee (95 linked bytes versus native 78) and the
native six-byte loop alignment. This remains **non-exact**.

Scoped evidence:
`.analysis/player-animation-document-release-review-20261111.txt`,
`.analysis/animation-document-destructor-final-check-compare.json`,
`.analysis/animation-document-destructor-contexts.log`, and
`build/probe-ltcg/AnimationDocumentDestructor-{simple,player}/`.
Next useful step is improving the real menu-marking call ABI, not
changing the already matching per-texture cleanup loop.
