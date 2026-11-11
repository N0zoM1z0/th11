# Animation renderer surface-copy queue

The four pending surface-copy records stored at the start of the renderer
are each 40 bytes. `FlushScreenshotResources` at `0x00445460..0x0044548F`
(48 bytes) walks all four records and forwards each nonnegative document ID
to the 313-byte `CopyAnimationTextureSurface` at
`0x00454EC0..0x00454FF8`. After each call it sets the record's document ID
to `-1`, with no error-status check. The real queue is also invoked by the
screenshot/hotkey handler at `0x00446880`; that parent is the sole entry
reference to the scanner, and the scanner is the sole caller of the copy.

## Native operations and ownership

The copy function receives the renderer in ECX and the 40-byte record in EAX
under the original private LTCG convention. Its record begins with document
and texture indices. The destination rectangle consists of x, y, width and
height at record +8..+17; the source rectangle uses x, y, width and height at
+18..+27. The native first checks the indexed document texture's COM pointer.
If absent it returns; otherwise it flushes the renderer and obtains
backbuffer 0 by `IDirect3DDevice9::GetBackBuffer(0,0,MONO)`.

If that call fails it returns. Otherwise it gets texture surface level 0,
releasing the backbuffer and returning if that fails. On success the code
builds two signed `RECT` values, and calls `D3DXLoadSurfaceFromSurface`
with filter 2, color key 0 and null palettes. A successful load invokes
the texture's `AddDirtyRect(NULL)` via vtable offset `+0x54`. It always
releases source surface then backbuffer after the load. The target does not
add document-index bounds, rectangle clipping, source-null checks or extra
HRESULT recovery. The maintained C++ preserves this flow and independent
reloads after opaque D3D/COM calls, rather than keeping a texture pointer
alive across them.

Native owner review: the 48-byte producer has 19 instructions, one call,
one plain return, and no embedded data. Its preceding 12 CC bytes are
outside the owner; `0x445490` immediately begins a distinct three-byte
function. The 313-byte copy has 113 instructions, eight calls, two returns
and no embedded tables or interior foreign entries. Its final RET is at
`0x454FF8`; seven following CC bytes separate the owner at `0x455000`.
Direct Ghidra caller/xref review and a separate full mapped PE/Capstone
instruction decode agree on both boundaries. Their original translation-unit
partition and class names remain unknown.

## Focused VC9 codegen work

`AnimationSurfaceCopy.cpp/.hpp` and `AnimationSurfaceDispatch.cpp` compile
with pinned VC9 in both normal COFF and `/GL` profiles. The real
`FlushScreenshotResources` producer establishes the child private EAX/ECX
register contract. A first linked copy was **234 bytes**; removing a
long-lived local texture cache and reloading the entry after COM calls
restores **312 candidate bytes / 113 instructions**, compared with native
**313 / 113**. Its main remaining differences are two separate `RECT` stack
placements and the final indexed texture pointer reload. The compile-only
producer context is under `.analysis/animation-surface-copy-context-20261111.cpp`;
linked artifacts and variant results are in
`build/probe-ltcg/AnimationSurfaceCopy-*` and `.analysis/try-surface-copy-*`.

The parent callback itself is normal-COFF **46/48** and focused `/GL`
**58/48** bytes in the maintained-child producer context, still non-exact. Tests of different natural loop forms
preserve this difference. These are diagnostic results, not canonical exact
match units. No executable byte patches, artificial padding or fake ABI
were used. Strict exactness awaits complete instruction/field agreement.
