# Animation chunk loading and sprite construction

Direct Ghidra inspection of the hash-pinned Japanese v1.00a target recovers
four complete functions. This corrects the former handoff's role guesses:
0x004543E0 loads external texture source bytes; 0x004544C0 advances document
postloading. The actual texture/sprite/script decoder is 0x004545A0.

| Function | Complete native extent | Candidate bytes | State |
| --- | --- | ---: | --- |
| LoadAnimationChunkSource | 0x004543E0..0x004544BC, 221 bytes | 209 | source, non-exact |
| AdvanceAnimationDocument | 0x004544C0..0x00454593, 212 bytes | 211 | source, non-exact |
| DecodeAnimationChunk | 0x004545A0..0x004548CA, 811 bytes | 811 | source, non-exact, 10 diagnostic differences |
| StoreAnimationSprite | 0x00454B10..0x00454BA4, 149 bytes | 149 | canonical linked-PE exact |

All four entries, branches, return tails, incoming/interior references and
adjacent instructions/padding were reviewed. There are no external interior
references or embedded tables. The decoder's full explicit span includes nine
internal alignment bytes omitted from Ghidra's sparse 802-address body. Its
complete extent is retained. Private review artifacts are
`.analysis/animation-chunk-review.txt`, `animation-decode-review.txt`,
`animation-decode-full-span.txt`, `animation-sprite-full-span.txt` and
`animation-texture-contracts.txt`.

## Physical views and loading order

The chunk has version DWORD+0, unsigned sprite/script counts+4/+6, unsigned
width/height/format WORDs+A/+C/+E, name offset+10, signed X/Y offsets+14/+16,
embedded-texture offset+1C, embedded-data byte+20 and next-chunk offset+24.
The sprite-offset array begins at+40. Script records follow it, eight bytes
each: their second DWORD is the chunk-relative script offset; their first
DWORD is ignored here. Original source-level class ownership remains open.

Document texture records are 20 bytes: COM texture pointer, loaded source
pointer, source size, bytes per pixel and flags. The document's+124 word is
a one-based postload cursor, not merely a boolean; +128 is a destruction
request checked by the worker, and +12C accumulates texture byte estimates.

LoadAnimationChunkSource rejects version !=7, then skips embedded chunks and
names beginning with '@'. Other names are formatted through the existing
260-byte local path and loaded with disk-mode1. Failure logs and returns-1;
success writes source size before source pointer, and returns1. There is no
extra null/index/bounds check, allocation, rollback or texture creation.
BeginLoadAnimationDocument now calls this accurately named function instead
of the former three-argument DecodeAnimationChunk declaration.

AdvanceAnimationDocument traces the document index and cursor, then rescans
from the first chunk to derive texture, sprite and script indices. It decodes
when the scanned index equals busy-1. A negative result clears busy and returns
null; reaching the last chunk clears busy and returns the document. After a
successful non-final decode it advances busy and returns. The original
additional index==busy exit is retained, including the behavior of zero,
negative and out-of-chain cursor values. Chunk links and cursor loads remain
in native order, without validation or timeouts.

## Complete decoder

DecodeAnimationChunk rejects null chunks and version !=7 before texture work.
Its four creation paths are:

| Chunk/name | Helper | Accounting/error behavior |
| --- | --- | --- |
| external, ordinary name | 0x00453D90, CreateAnimationFileTexture | signed offsets, unsigned dimensions/format; negative result logs filename and returns-1; otherwise add result to +12C |
| external, '@R' prefix | 0x004540F0, CreateAnimationRenderTarget | ignore result; no byte-accounting addition |
| external, other '@' name | 0x004540A0, CreateAnimationEmptyTexture | add result without checking failure |
| embedded data | 0x00453F80, CreateAnimationEmbeddedTexture | pass chunk+textureOffset; negative result logs and returns-1; otherwise add result |

These four lower texture helpers now have full source, three exact; see
`ANIMATION_TEXTURES.md`. The file path still calls an external transparent-pixel
helper. The GetLevelDesc COM call uses slot+44,
level0 and a D3DSURFACE_DESC. Its HRESULT is ignored. There is no synthesized
fallback for texture allocation/description failure.

For each sprite, the decoder reads four floats after the record's ignored ID:
X, Y, width and height. It computes float scale factors from actual unsigned
texture dimensions divided by nominal unsigned16-bit dimensions, scales both
rectangle endpoints, and passes a 72-byte sprite record to StoreAnimationSprite.
The physical record stores actual height at+1C and actual width at+20. Scale
factors are at+3C/+40. Zero dimensions are not specially handled. The subsequent
script loop publishes chunk-relative pointers at the cumulative script index.

The stack sprite is intentionally not zero-initialized: the native function
copies all72 bytes, including fields it did not initialize. StoreAnimationSprite
then overwrites UVs at+24/+28/+2C/+30 and recovered sizes at+34/+38; the DWORD
at+44 remains indeterminate. Initializing that field would change native
behavior. UVs divide horizontal coordinates by actual width and vertical
coordinates by actual height; recovered sizes divide endpoint differences by
the original X/Y scales. The source also preserves aliasing when the supplied
sprite already occupies its destination cell.

## Exact proof and bounded diagnostics

```bash
scripts/repo-python scripts/replay-exact-units.py --unit store-animation-sprite
```

The cold pinned VC9 /GL family uses AnimationSprite.cpp as the primary TU,
AnimationChunk.cpp and an independent-input probe as support. The complete
public149-byte MAP/PDB contribution agrees with the target with zero byte
differences. Exhaustive linkage discovery finds no address fields. The proof
checks source/header/tool/producer/PE/MAP/PDB receipts and primary-TU ownership.
Original TU partition and whole-product ownership remain separate unknowns.

The original diagnostic family, with AnimationChunk.cpp primary, produced239,
212 and629 bytes for the three non-exact functions, respectively. Masked
address-field diagnostics find156,161 and735 unequal positions, including
length differences; these are not exactness claims. The opaque lower helper
and error logger contracts leave private-register/frame context unresolved.
StoreAnimationSprite independently produced the same149 raw bytes in that
family. The current constructor family produces209/147,211/118 and811/10
byte counts/unequal diagnostic positions; its known lower helper contracts,
typed records and observed field/table evaluation order restore most of the
decoder's native context. No artificial ABI, padding or executable patch was used.

Private `.analysis/check-animation-chunk.py` executes full original and
candidate bodies independently under Unicorn. Its58 cases agree, including
version/load/creation failures, all four texture paths, signed offsets,
unsigned high-bit texture dimensions, ignored description HRESULT, empty and
multiple sprite/script tables, byte-accounting wraparound, seven cursor values,
decode failures, cursor mutation, five integrated advance/decode cases and
aliased sprite input. Semantic snapshots omit only the decoder's indeterminate
sprite DWORD+44; standalone sprite tests compare the full72-byte record.
Neighbors, pointer tables, defined sprite bytes, calls, results, nonvolatile
registers, GS checks and stack balance are compared. Fixture helper contracts
model I/O, texture constructors, logging and COM description behavior. They do
not establish real D3D behavior, full-game runtime or semantic-phase acceptance.
Maximum diagnostic RSS is41,080 KiB after timely Unicorn fixture collection.

The current pass adds a second58-case run in the real constructor family,
recorded in `.analysis/animation-postload-differential.json`. Its actual private
ABIs were re-read from that artifact: the specialized resource loader receives
EBX filename/stack size with disk-mode1 implicit. The full original and candidate
function bodies remain independently executed. The stage/document family
retains297/434/131-byte sizes and17/337/80 differences, with all34 cases agreeing.
The sprite exact context was cold-replayed after its source/header dependencies
changed; unrelated exact contexts were not replayed.

Next useful work is full recovery of the file-texture path's transparent-pixel
helper, followed by the medium stage initializer and EH destruction. The
background dispatcher at0x004548D0 also needs its own
source and review: its destruction path writes through a slot after clearing
it, an observed failure path that must not be silently repaired.
