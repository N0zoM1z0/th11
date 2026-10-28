# Animation transparent-pixel processing

FixAnimationTextureTransparentPixels at0x004534C0 now has complete maintained
source in `AnimationTransparentPixels.cpp`. It is a non-exact reconstruction.
It fills transparent texels' RGB from opaque horizontal/vertical neighbors,
leaves alpha intact, and performs the original surface acquisition/lock/unlock/
release sequence without new validation or cleanup paths.
Its original game-versus-library source attribution is indeterminate; generic
surface/pixel operations and game callers alone do not establish authorship.

## Complete target ownership

The old1,381-byte Ghidra code candidate is not the complete owner. Direct
full-text-attested Ghidra instruction, flow, incoming/interior-reference and
owned-tail reference review establishes this complete1,427-byte range:

| Owned region | Inclusive addresses | Bytes |
| --- | --- | ---: |
| Contiguous code, through RET | 0x004534C0..0x00453A24 | 1,381 |
| Compiler alignment | 0x00453A25..0x00453A27 | 3 |
| Four absolute switch destinations | 0x00453A28..0x00453A37 | 16 |
| Format-to-destination selector | 0x00453A38..0x00453A52 | 27 |

The selector maps formats0/21 to the32-bit case,25 to A1R5G5B5,26 to A4R4G4B4,
and all other indices0..26 to the default exit. Unsigned values above26 also
take the default path. The four apparent external code-interior references
come from this owner's jump table; they are internal to the complete span.
Every owned-tail byte was checked for references: only the two internal table
reads enter it. Three reviewed callers enter the function itself. Preceding
RET/CC, following thirteen CC bytes and the next code at0x00453A60 were checked.
That next entry is absent from the provisional inventory and has not been
mapped or credited; the padding/gap must not be absorbed into this function.

Private evidence is retained in `.analysis/animation-transparent-review.txt`,
`animation-transparent-disassembly.txt`, `animation-transparent-code-span.txt`,
`animation-transparent-table-refs.txt` and
`animation-transparent-owned-tail-refs.txt`. Ghidra's default decompilation has
incorrect COM prototypes/stack recovery; its vertical-neighbor expressions
must not override the signed pitch/division instructions.

## Surface and pixel behavior

Native input is the20-byte texture entry in EAX. The function initializes a
surface pointer to null, obtains level0, gets a32-byte D3DSURFACE_DESC and locks
the entire surface with flags0. It ignores every HRESULT. Unsupported formats
still acquire, describe and lock the surface, then unlock and release it.
UnlockRect and Release are unconditional; null acquisition and null locked
pixel pointers retain the observed fault behavior.

Each row starts at locked.pBits + row*Pitch, so padding is respected instead
of treating all rows as one tightly packed array. Width/height/row/column are
unsigned; Pitch is signed. Left/right neighbors are one pixel away. Vertical
neighbors use signed Pitch/4 or Pitch/2, truncated toward zero and then scaled
by the pixel stride, matching even negative and non-divisible pitch fixtures.

Only nonzero-alpha neighbors contribute. The order is left, right, above,
below; RGB channels sum as unsigned integers. More than one contributor is
averaged with unsigned integer division; one is copied, and no contributors
clear RGB. Alpha is never filled. UNKNOWN and A8R8G8B8 use byte RGB/alpha;
A1R5G5B5 and A4R4G4B4 use their real packed fields, retaining the original alpha
bits. The A4 case does not halve the averaged components; an unrelated text
surface routine has different behavior and is not evidence for this function.

## Code generation and validation

The current VC9 /GL family produces a complete1,383-byte public contribution:
1,338 code bytes, two alignment bytes and the same43 bytes of switch-table
structure. Target ownership remains1,427 bytes. Diagnostic address-field
masking leaves1,339 unequal positions including the size difference. Stack
layout differs (source0x3C versus target0x44), and immediate-zero/register
scheduling context remains open. Byte-pointer versus typed32-bit pixel source
and truth-test versus unsigned-positive condition syntax produced the same
candidate; do not repeat those probes unchanged. No padding, fake ABI or
instruction bytes were added.

The linked Oracle's current linear decoder also leaves a final selector byte
unaccounted for in this contribution. No exact unit is configured for it. Its
tables and full extent must remain intact; cropping to the RET or treating a
masked comparison as exact would be invalid.

Resolving the real helper through ordinary source declarations naturally gives
the file constructor its original EAX helper argument. The constructor now has
484 bytes with40 diagnostic differences, improved from483/70; its own0x44/0x4C
local-frame gap remains open. The three existing exact constructors were
cold-replayed with the real pixel helper in their declared family; all423 bytes
and20 linked fields still agree. The other42 exact contexts were unchanged
and were not replayed. Decoder codegen remains811 bytes/10 diagnostic differences.

## Descriptor layout correction

D3DSURFACE_DESC contains eight DWORDs: MultiSampleQuality precedes Width at+24
and Height at+28. Earlier seven-DWORD fixtures misplaced the dimensions. The
source uses the SDK type; strict byte proofs are independent of those fixtures.
