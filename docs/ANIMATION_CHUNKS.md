# Animation chunk loading and sprite construction

Direct Ghidra inspection of the hash-pinned Japanese v1.00a target recovers
four complete functions. This corrects the former handoff's role guesses:
0x004543E0 loads external texture source bytes; 0x004544C0 advances document
postloading. The actual texture/sprite/script decoder is 0x004545A0.

| Function | Complete native extent | Candidate bytes | State |
| --- | --- | ---: | --- |
| LoadAnimationChunkSource | 0x004543E0..0x004544BC, 221 bytes | 221 (producer context) | source, non-exact, two SIB code bytes differ |
| AdvanceAnimationDocument | 0x004544C0..0x00454593, 212 bytes | 211 | source, non-exact |
| DecodeAnimationChunk | 0x004545A0..0x004548CA, 811 bytes | 811 | **canonical linked-PE exact, 15 bindings** |
| StoreAnimationSprite | 0x00454B10..0x00454BA4, 149 bytes | 149 | canonical linked-PE exact |

All four entries, branches, return tails, incoming/interior references and
adjacent instructions/padding were reviewed. There are no external interior
references or embedded tables. The decoder owns nine internal alignment bytes
omitted from Ghidra's sparse 802-address body; preserve its complete extent.

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
`ANIMATION_TEXTURES.md`. The file path's transparent-pixel helper now also has
full source in `ANIMATION_TRANSPARENT_PIXELS.md`. The GetLevelDesc COM call uses slot+44,
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

## Exact proof and open differences

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
family. The previous constructor family produced209/147,211/118 and811/10
byte counts/unequal diagnostic positions; its known lower helper contracts,
typed records and observed field/table evaluation order restored most of the
decoder's native context. The indexed script loop reduced the last
figure to 811/5 nonfield differences; the final natural sprite
fileIndex/texture/textureIndex assignment order now removes all five
remaining instruction differences. The full 811-byte owner is strictly
**linked-PE exact**, with no artificial ABI, padding or executable patch.

D3DSURFACE_DESC is32 bytes/eight DWORDs, including MultiSampleQuality before
Width and Height. Earlier seven-DWORD fixtures do not establish dimensions;
strict byte proofs are independent of those fixtures.

Open animation work includes the 221-byte external source loader and
212-byte document advance function, plus the separately reviewed
135-byte document-slot scanner at 0x004548D0. That scanner now has
maintained C++ in `AnimationDocumentSlotsLifecycle.cpp`; the source
preserves the native unchecked slot reload after deletion. Its
scoped 135-byte candidate still differs by one EAX/EDI register-selector
byte, and is **non-exact**. See
[ANIMATION_DOCUMENT_SLOTS](ANIMATION_DOCUMENT_SLOTS.md).


## Indexed script records, 811-byte owner (2026-11-11)

The decoder's final script loop originally advanced a typed record pointer
in the `for` increment and accessed `script->offset`. That source was
semantically correct but made VC9 schedule the script-record initial pointer
LEA between two independent `ADD EAX,EAX` operations. In the original
executable both doubling operations precede the LEA at offsets
`0x2F2..0x2F8`.

The maintained `src/AnimationChunk.cpp` now uses the equally natural
indexed form `script[i].offset` and increments only the loop index. In a
focused six-support-TU VC9 `/GL` link with the lower texture
constructors and sprite helper, the complete primary-TU decoder is still
**811/811 bytes** with eight calls, three returns, and nine owned internal
alignment bytes. At this **intermediate** checkpoint, the five script-loop differences
disappeared and five sprite store bytes remained (offsets
`0x1E8/0x1F2/0x1F6/0x1FA/0x202`). The subsequent source change below
eliminated those too; the complete 811-byte function is now **strict
exact**. These historical masked diagnostics do not themselves
confer the accepted status.

The distinct strict `store-animation-sprite` unit was rebuilt and
recompared after the decoder edit: **149/149 exact**, no fields.
Private focused receipts:
`.analysis/animation-decoder-script-index-linked-build.log`,
`.analysis/decoder-script-script-int-index.cpp`, and
`build/probe-ltcg/AnimationDecoderFocused/`.


## Exact 811-byte decoder (2026-11-11)

The last five unequal instruction bytes were caused by the declaration
order of three ordinary sprite fields. In `src/AnimationChunk.cpp`
the reconstructed file index is now assigned **before** its texture
entry pointer, followed by the texture index. Combined with the
indexed script-record loop, this restores every code byte without
hardcoded instructions, private calling-convention tricks or object
padding.

Cold linked Oracle `decode-animation-chunk` independently recompiles
the full six-support-TU VC9 `/GL` context and checks the original
811-byte extent, including its nine owned alignment bytes, every
function return and the unique primary-TU PE/MAP/PDB code
contribution. It binds all **15** independently reviewed fields:
**eight DIR32** entries (error strings, log global, two identical
constant-pool references) and **seven REL32** calls (two error
logger sites, four texture helpers and one sprite-store helper).
Complete replay returns **811/811 exact with zero differences**.
The separately accepted 149-byte sprite store remains an independent
strict exact owner.

This does not establish original C++ nominal classes or the exact
commercial translation-unit partition. The 212-byte advance owner
and file-source loader remain non-exact, and the focused image is a
non-runnable diagnostic, not a reconstructed full executable.
Evidence is under `.analysis/decode-animation-chunk-strict-*.json`,
`.analysis/decode-animation-chunk-strict-cold-build.log` and
`build/match-linked/AnimationChunkDecode/`.


## Scoped dependency rechecks after the 811-byte exact owner

After the final sprite-header field-order change, only directly affected
strict units were cold-rebuilt and compared, not the whole cold cohort.
The following maintain their independent exact byte proofs:
`create-animation-embedded-texture` **287/287, 10 fields**,
`create-animation-empty-texture` **67/67, 5 fields**,
`create-animation-render-target` **69/69, 5 fields**, and
`store-animation-sprite` **149/149, no fields**.
Private receipts:
`.analysis/create-animation-*-decoder-exact-proof.json` and
`.analysis/store-animation-sprite-final-boss-proof.json`.

The remaining 212-byte `AdvanceAnimationDocument` was also exercised
in its sole native scan caller context. Changing its decoded-state
`bool` to a 32-bit integer produced a **215-byte** private /GL
contribution instead of the previous 212, and did not change the
scanning caller's private EAX receiver (native EDI). Source variable
reordering and extra natural dynamic callers gave the same result.
Those variants are kept only under `.analysis/advance-document-*`;
the maintained advance source was not changed.


## Real producer context for 221-byte source loader (2026-11-11)

The source-present `BeginLoadAnimationDocument` at 0x454190 is the genuine
producer of `LoadAnimationChunkSource` (0x4543E0). A compile-only dynamic
source caller in `probes/animation-document-load-context.cpp` retains
this parent-child relationship in a narrowly linked VC9 `/GL` family.
The source loader naturally recovers the native private arguments:
ECX document, EBX texture index and a single stack chunk parameter,
with `RET4`. The complete PDB contribution is **221/221 bytes**,
**six calls and two RET4s**, unlike the earlier texture-only family
with 209 bytes, five calls and one RET8.

All twelve address/call fields were independently compared with the
hash-pinned original: six DIR32 values (security cookie, error strings
and runtime log) and six REL32 calls (logging, security check,
formatting and resource loading). Only **two non-field bytes remain**:
the SIB selectors at offsets 0xB3 and 0xBD encode commutatively
equivalent ECX/EDX base/index addressing for the two stores into the
texture entry. The native uses byte 0x11 and the linked candidate
0x0A. **This is not exact**: these instruction bytes must match,
not be ignored or rewritten by the comparator.

Nine normal C++ alternatives using a pointer, reference, precomputed
offset or explicit member addresses were compiled with pinned VC9. The
first five bounded real-producer `/GL` links all regressed to **212
bytes** and did not recover the native SIB register selection, so the
maintained source remains untouched. Private evidence:
`.analysis/animation-load-real-producer.log`,
`.analysis/animation-load-store-link-a.log`,
and `build/probe-ltcg/AnimationRealProducer-producer/`.

The linked producer context also changes the decoder to 827 bytes,
so it is **not** substituted for the already cold-attested independent
811/811 exact `decode-animation-chunk` unit. The production TU
partition remains unknown.
