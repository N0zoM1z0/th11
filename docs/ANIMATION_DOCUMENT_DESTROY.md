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

The original two scoped `/GL` contexts linked the document destructor to
the menu marker with only a single global-renderer caller. They produced
a **229-byte** document owner and a 95-byte marker because LTCG
specialized the original private callee ABI. A separate, genuinely
generic renderer/file caller was added to the existing compile-only
`probes/animation-document-destructor-context.cpp`. Rebuilding only
this related scope yields the complete native **243-byte destructor**,
its **eight calls/one RET**, and a separate exact **78-byte marker**.

The strict canonical unit `animation-document-destroy` cold-compiles the
current source, marker callee, and both diagnostic callers. All
**243/243 bytes are identical after the eight reviewed fields** are
replayed: DIR32 renderer at offset `13`, REL32 marker at `1A` and six
REL32 free calls at `56/80/99/B2/CB/E4`. The public primary-TU
function owner, native complete boundary, all eight fields, and
source/support/tool/PE/MAP/PDB receipts pass the strict linked Oracle.
This is **canonical exact**, not merely a same-sized linked diagnostic.

The separately accepted `menu-mark-file-animations` unit uses its
actual menu-controller producer call context and independently
reproduces **78/78 bytes, zero relocations**.

Scoped evidence:
`.analysis/player-animation-document-release-review-20261111.txt`,
`.analysis/animation-document-destructor-final-check-compare.json`,
`.analysis/animation-document-destructor-contexts.log`, and
`build/probe-ltcg/AnimationDocumentDestructor-{simple,player}/`.
The next integration step is reconciling the still non-exact 415-byte
player storage destructor against the newly exact cleanup callee.
