# Animation texture creation

The four constructors called by DecodeAnimationChunk now have complete maintained
source in `AnimationTexture.cpp`. Direct hash-attested Ghidra review covers all
control flow, caller arguments, COM slots, incoming/interior references, RET tails
and adjacent padding. Each target body is contiguous, with no embedded tables or
external interior references. The private artifacts are
`.analysis/animation-texture-create-review.txt`,
`animation-file-texture-full-span.txt` and `animation-texture-create.c`.
Default decompilation has incorrect D3DX prototypes and removes real blocks;
the instruction review and ABI-correct SDK declarations take precedence.

| Function | Complete target span | Current source bytes | State |
| --- | --- | ---: | --- |
| CreateAnimationFileTexture | 0x00453D90..0x00453F73, 484 bytes | 483 | non-exact, 70 diagnostic differences |
| CreateAnimationEmbeddedTexture | 0x00453F80..0x0045409E, 287 bytes | 287 | canonical linked-PE exact, 10 bindings |
| CreateAnimationEmptyTexture | 0x004540A0..0x004540E2, 67 bytes | 67 | canonical linked-PE exact, 5 bindings |
| CreateAnimationRenderTarget | 0x004540F0..0x00454134, 69 bytes | 69 | canonical linked-PE exact, 5 bindings |

## Format and creation behavior

Independently reviewed target tables at0x00496548/0x00496560 map six indices to
D3D formats UNKNOWN, A8R8G8B8, A1R5G5B5, R5G6B5, R8G8B8 and A4R4G4B4, with
byte counts4,4,2,2,3,2. They remain external data owners, declared in
`AnimationTexture.hpp`; no array/index validation is added. When runtime+200
bit0 is set, the file and embedded constructors remap UNKNOWN/A8R8G8B8 to index5
and R8G8B8 to index3. Empty textures do not perform this remapping.

The file constructor clears flag bit0 before D3DXCreateTextureFromFileInMemoryEx.
It supplies the previously loaded source pointer/size, zero requested width,
height, mip levels and usage, the selected format, MANAGED pool, filter1,
mip-filter0xFFFFFFFF, color-key0 and null image-info/palette outputs. Every
nonzero HRESULT, including positive values, returns-1 without cleanup.

On success it gets surface0 and its description, ignoring both HRESULTs. If
actual dimensions equal the requested dimensions, it adopts the loaded texture
and releases the source surface; X/Y offsets are ignored in this branch. A size
mismatch creates a requested-size texture with levels0, usage0 and MANAGED pool,
gets its surface0, and copies from a source RECT. The rectangle uses signed
X/Y offsets and signed comparisons against actual dimensions; each far edge
is offset+min(requested size, actual size-offset). There is no lower clamp.
Creation, surface and copy results are ignored, including the native null-fault
paths. Source surface, destination surface and temporary texture are released
in that order when non-null, with local pointers then cleared.

File success calls the still-external transparent-pixel helper0x004534C0 before
publishing bytesPerPixel; the return is requested width*height*selected byte count,
not the copied rectangle size. The helper's provisional1,381-byte candidate has
only preliminary decompilation here: its full ownership and implementation
remain open. It is not a source-buffer free operation. Native passes the entry
in EAX; the current opaque ordinary source contract uses a stack argument.
Together with local-frame layout this remains part of the483/484-byte gap.

The embedded constructor reads a16-byte texture header: signed format+6 and
signed width/height+8/+A; pixel data starts at+10. Its zero-origin source and
destination RECT uses these signed source dimensions. Destination creation uses
the caller's dimensions, one level, usage0, selected destination format and
MANAGED pool. A nonzero creation HRESULT returns-1. The later surface acquisition
and D3DXLoadSurfaceFromMemory results are ignored. Source pitch is signed source
width times the original source-format byte count; destination fallback does
not change that source format. The same RECT is passed for source/destination,
filter1 and color-key0. It writes bytesPerPixel before conditionally releasing
the acquired surface, then returns selected bytesPerPixel*requested width*height.
No transparent-pixel pass runs on this path.

Empty creation clears flag bit0, requests one level/usage0/MANAGED pool and
ignores the creation HRESULT. Metadata and byte-accounting arithmetic still
happen after failure. Its ordinary source parameters are width,height,format;
VC9 naturally assigns the target's ESI entry/EDI format/EBX height/stack width.
Render-target creation sets bit0 first, uses runtime backbuffer dimensions and
format, one level, RENDERTARGET usage and DEFAULT pool. Its COM result is ignored;
bytesPerPixel is4 only for X8R8G8B8 and2 otherwise, and it returns0.

## Decoder progress and exact evidence

The new family contains the real chunk decoder and known resource/error helpers.
Its probe calls loading and postloading through their actual relationship: the
decoder has no extra synthetic direct caller. Removing that extra call restores
the native decoder's112-byte frame without stack realignment. Recovering the
real20-byte sprite and eight-byte script record types, restoring per-branch
texture-entry evaluation and the later table reload, and retaining the observed
sprite assignment order reduce the decoder from the former629-byte candidate
to the complete811 bytes with10 unequal diagnostic positions. Five positions
come from register/store scheduling in sprite setup and five from reordered
independent script-loop entry instructions. It remains non-exact: no register,
instruction-order or byte normalization grants credit.

Bounded alternate source contexts are retained privately: opaque helpers gave
629 bytes; real constructors with an extra decoder caller gave663; restricting
the caller gave667; restoring native table evaluation gave811/171 differences;
typed sprite records gave811/39; texture-first sprite assignment gave811/10.
The pointer-temporary assignment variant returned to39 and was not retained.
No inline bytes, padding, code patches or false ABI declarations were used.

```bash
scripts/repo-python scripts/replay-exact-units.py \
  --unit create-animation-embedded-texture \
  --unit create-animation-empty-texture \
  --unit create-animation-render-target \
  --unit store-animation-sprite
```

The three new strict proofs share one cold VC9 /GL family with AnimationTexture.cpp
as primary TU. All423 bytes and all20 independently reviewed linked fields agree
after exhaustive replay. Receipts cover source/header/tool/producer/PE/MAP/PDB
identity and complete public primary-TU code ownership. Runtime member bindings
use the independently reviewed MainRuntime base plus offsets; D3DX thunks and
format tables bind separately. The affected existing149-byte sprite exact unit
was also cold-replayed. Earlier41 exact contexts were not modified or replayed.
Original TU partition and full native product closure remain open.

Private `.analysis/check-animation-texture.py` and `animation_texture_fixture.py`
compare complete original/candidate constructors in92 cases, including all six
formats with/without fallback, crop branches and signed edge cases, positive/
negative HRESULTs, ignored failures, surface releases, accounting wraparound,
signed embedded dimensions and four matching native/candidate faults. D3DX,
COM and transparent-pixel processing are modeled contracts. A current additional
58-case chunk/sprite run uses the new family, retaining five integrated
advance/decode cases; the affected old stage/document family is independently
rebuilt and its34 cases still agree. These bounded diagnostics do not establish
actual D3D pixel results, full-game runtime or semantic-phase acceptance.
