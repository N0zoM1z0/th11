# Text drawing and CPU pixel composition

Direct, read-only Ghidra review of the original Japanese TH11 v1.00a establishes
four complete spans. All have maintained C++ source, but none has an exact
proof. Existing canonical source and header inputs are unchanged.

| Target | Function | Native bytes | Current linked bytes | Unequal diagnostic positions |
| --- | --- | ---: | ---: | ---: |
| `0x004441C0..0x00444450` | `SurfaceCompositeView::BlendToSurface` | 657 | 686 | 563 |
| `0x00444890..0x00444A17` | `DrawTextToTexture` | 392 | 403 | 316 |
| `0x00443860..0x00443BC7` | `FillTransparentTextPixels` | 872 | 801 | 784 |
| `0x00443BD0..0x00443C74` | `InvertTextAlpha` | 165 | 182 | 163 |

These are complete compiler-owned linked contributions under pinned VC9 RTM
`/O2 /Ob1 /Gy /MT /GL`, not selected slices. Differences are diagnostic counts
outside candidate address fields; they grant no exact credit. The drawing TU
is primary and the two processing functions are support-owned in its probe.
No processing primary-TU exact claim is made. Source mappings add 2,086 native
bytes of coverage while exact totals stay at 38 functions / 8,514 bytes.

## Boundaries and calling conventions

Full explicit-span Ghidra decoding covers all four extents, preceding RET/CC,
following CC and zero external interior references. No embedded tables were
found. `InvertTextAlpha` has a 162-address sparse Ghidra body: its omitted
three-byte LEA at `0x00443C5D` belongs to the full 165-byte span.

The compositor receives its physical view in ECX, destination y in EAX, and
surface/x/copy-width/copy-height in four stack slots. It returns an AL boolean
with RET16. No incoming reference currently identifies its original owner.
The natural member source currently uses EAX for copy-height and an aligned
frame, so its private ABI and bytes remain different. Replacing RECT aggregate
initialization with individual field stores did not change the 686-byte result.
Do not repeat these two contexts without new evidence.

The two processing functions take one stack row count and return an AL boolean
with RET4; alpha inversion uses signed byte limits, neighbor filling unsigned
row/column comparisons. Drawing receives font choice in EAX and six stack
inputs: destination RECT, x, font-height, color, string and texture. It ends in
RET24. The real caller `0x00454BB0` prepares these at `0x00454C3B..0x00454C50`.

These operations and their callers establish their use, but do not independently
identify original game versus bundled library ownership. All four origin
classifications remain indeterminate.

## Surface composition

`SurfaceCompositeView` is a separate partial physical view, retaining format
at +100, logical dimensions at +104/+108, source row bytes at +110, presence at
+11C and pixels at +120. This checkpoint does not change earlier surface-copy
or text-creation headers and makes no complete original class-size claim.

Presence zero returns false without COM calls. GetDesc status is ignored;
LockRect must return exactly D3D_OK, including rejection of positive nonzero
HRESULTs. Its rectangle is always the entire logical source dimensions at
(0,0), independently of the destination offset and copy dimensions. A format
mismatch unlocks and returns true. Matching unsupported formats return false
without unlocking. UnlockRect status is ignored.

- A8R8G8B8 blends B/G/R with signed `(source-destination)*alpha >> 8`, leaves
  destination alpha unchanged, and advances by four bytes. Alpha255 therefore
  still uses denominator256.
- A1R5G5B5 copies a complete word only when the source alpha bit is set.
- A4R4G4B4 first replaces the high nibble of destination byte0. The low-nibble
  blend reads that newly written byte, whose low nibble is zero. Preserve this
  operation order even when source and destination overlap. Its output alpha
  is `min(((old_destination_byte1 >> 2) & 0x3C) + source_alpha, 15)`, followed
  by the red-nibble blend; it is not ordinary alpha accumulation.

Pointers advance by source and locked destination pitches after each row.
Nonpositive copy dimensions retain the native empty-loop behavior; no clipping
or validation is introduced.

## Text processing and drawing

The existing workspace at `0x004A57C0` supplies format/dimensions/image bytes/
pitch/DC/pixels. Font cells are large Gothic `0x004C2EEC`, large Mincho
`0x004C2EE8`, small Gothic `0x004C0EE4` and small Mincho `0x004A8ED4`.

Alpha inversion processes signed `pitch*rows` bytes. A8 complements every
fourth alpha byte; A4 XORs the high nibble of every second byte. A1 toggles bit15
and clears the complete pixel when the resulting alpha is zero. An odd positive
byte count includes the last complete word. Unsupported formats return false.
The maintained A1 bitfield representation is faithful in bounded diagnostics,
but does not reproduce the native bitfield instruction sequence.

Transparent-pixel filling supports A8 and A4; all other formats return true
unchanged. For a pixel with zero alpha it averages color channels from opaque
left/right/up/down neighbors; zero or one neighbor needs no division. A4 halves
the averaged components before storing, while preserving alpha. Current opaque
pixels remain unchanged. Traversal is contiguous by logical width, with no row
padding skip. Vertical neighbor displacement separately uses signed pitch/4 or
pitch/2 rounded toward zero. The lower-neighbor bound uses stored height,
independently of the requested number of processed rows.

Drawing selects fonts 0/1/2/other, clamps signed font-height to at least17 and
seeds image storage with 16-bit RGB nibbles extracted from color bits20/12/4,
regardless of the current format. The image-byte loop uses signed comparisons
and writes the final word for odd positive counts. It then selects the font,
inverts alpha, selects TRANSPARENT background mode, measures the byte string,
sets the unmodified color and calls TextOutA at `(x*2,0)`.

After text output it restores the selected object, inverts alpha, fills
transparent pixels and restores the same object again. Processing rows are
`font_height*2+6`. D3DX source bounds are `(0,0,(right-left)*2+22,
font_height*2+2)`, with only the right bound capped at1024. GetSurfaceLevel and
D3DXLoadSurfaceFromMemory results are ignored. Loading uses filter4, color key0,
the original destination RECT and the current workspace format/pitch. A non-null
surface is released regardless of HRESULT. The surface output is not initialized
before the COM call, matching native behavior; failure without writing it remains
indeterminate, and is not turned into a clean error path.

## Focused diagnostics and remaining work

Two cold private families compile only the new compositor and text paths.
The compositor's 74 machine-code cases agree with both the original full body
and an independent integer reference, including all formats, signed dimensions,
negative pitches, overlapping buffers, COM failures and ignored status values.
The text family's 162 cases execute all three complete bodies and compare
pixels, unchanged workspace, AL results where meaningful, API order/arguments,
source rectangles, font choices, release behavior, stack balance and preserved
registers. Standalone processing cases also agree with an independent pixel
reference. Integrated drawing uses modeled GDI raster bytes and D3DX transfer;
it does not establish real font rendering or GPU behavior.

Private reports are `.analysis/surface-composite-differential.json` and
`.analysis/text-drawing-differential.json`. Each runs at nice10 on one CPU;
measured maximum RSS was95,504 and107,536 KiB respectively. Compiler/Ghidra
jobs retain the shared lock and existing resource limits. These bounded
diagnostics are neither exact proofs nor complete native/runtime acceptance.
No existing canonical unit was replayed.

Original type/TU/frame context remains open. Drawing currently turns its
background word loop into REP stores, while native reloads the byte limit.
Do not force volatility, artificial padding or altered return/ABI declarations
to fit the Oracle. The sibling text path at `0x00444610` (631 bytes) and the
related pixel routine at `0x00443E00` (525 bytes) are useful next targets.
