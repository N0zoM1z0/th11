# Outlined text drawing and placement

Direct read-only Ghidra review supplies three complete target functions and
their real call chain. This checkpoint adds their maintained C++ source and
does not change existing canonical source or headers.

| Function | Native span | Native bytes | Linked bytes | Unequal diagnostic positions |
| --- | --- | ---: | ---: | ---: |
| `DrawOutlinedTextToTexture` | `0x00444610..0x00444886` | 631 | 637 | 520 |
| `DrawOutlinedSurfaceText` | `0x00443E00..0x0044400C` | 525 | 530 | 466 |
| `DrawPlacedTextureText` | `0x00454BB0..0x00454C5B` | 172 | 173 | 130 |

All three remain non-exact. Counts exclude candidate address fields and have
no acceptance authority. The texture function is a complete primary-TU PDB
contribution in the final diagnostic family; the other two are support-owned
there. No support-only result is promoted to an exact proof. New source
mappings cover 1,328 native bytes; exact totals remain 38 / 8,514 bytes.

Full explicit-span decoding covers all native bytes, branches and returns,
with zero external interior references and no embedded table. The texture
entry follows an adjacent RET; the surface and placement entries follow
RET/CC. All three end before CC. The surface caller `0x00443D20` was reviewed
through its font creation, AL-result forwarding and cleanup, but receives no
new source mapping or credit here. Its font/string ownership remains open.

## DIB surface outline

The surface routine receives the physical view in EAX, text in EDI and five
stack inputs: x, y, font, foreground and outline. It returns an AL boolean with
RET20. Null text returns false before reading the view. There is no bitmap/DC
availability test. Width/height/DC are captured from +104/+108/+114 before
SelectObject and SetBkMode(TRANSPARENT).

Outline color CLR_INVALID (`0xFFFFFFFF`) skips the outline passes. Otherwise
six DrawTextA calls use count-1 and flags0 with these rectangles in order,
where w/h are the captured dimensions:

1. `(x, y, x+w-2, y+h-2)`
2. `(x+2, y, x+w+2, y+h)`
3. `(x+1, y+2, x+w+1, y+h+2)`
4. `(x+2, y, x+w+2, y+h)`
5. `(x, y+2, x+w, y+h+2)`
6. `(x+2, y+2, x+w+2, y+h+2)`

The duplicate second/fourth positions are intentional source fidelity. Each
has its own RECT storage: a GDI write to one rectangle must not alter another.
Foreground then uses `(x+1,y+1,x+w+1,y+h+1)`. The previously selected object is
restored; background mode and text color are not restored. GDI results do not
change the true return for non-null text.

## Texture outline

Native inputs are text in ECX, x in EDX, font choice in EAX, followed by six
stack inputs: destination RECT, font-height, foreground, outline, texture and
spacing. It returns with RET24. Current source compiles with a different
private convention and RET28, so even the ABI remains non-exact.

Four font choices and the minimum font-height17 match the plain drawing path.
The entire workspace image byte count is cleared by memset. The routine
selects the font, inverts alpha over `height*2+6` rows, selects TRANSPARENT
background mode and counts the byte string.

Spacing zero draws the whole byte string four times in outline color at
`(2*x+2,4)`, `(2*x-2,4)`, `(2*x+2,0)`, `(2*x-2,0)`, then once in foreground
at `(2*x,2)`. These calls also occur for an empty string with count0.

Nonzero spacing processes fixed two-byte groups, without character decoding.
Each group has a third zero byte; TextOutA always receives count2, including
the final NUL when the original byte length is odd. Empty strings produce no
TextOutA calls in this branch. The same five positions advance horizontally
by `2*spacing` per group. Negative spacing remains negative. CLR_INVALID has
no sentinel meaning in this texture routine; it is passed as an ordinary
outline color.

Both branches restore the selected object, invert alpha, fill transparent
pixels and restore the object again. The source rectangle, upper right cap1024,
GetSurfaceLevel, D3DX filter4/color-key0 and conditional Release match the plain
path documented in `TEXT_DRAWING_COMPOSITE.md`. HRESULTs do not gate loading or
release. The surface output remains uninitialized if COM does not write it;
that case is indeterminate rather than a fabricated error path.

## Placement and real callers

The native placement function receives font-height in EAX, text in EBX and
font choice in EDI. Seven stack inputs supply texture, placement view, x,
foreground, outline, plain-mode selector and spacing. It ends in RET28.
Nonpositive font-height becomes17. Positive heights1 through8 return before
reading placement or calling any drawing path. Heights9 through16 reach the
selected drawing path, which separately clamps them to17.

Four observed floats at placement+0C/+10/+14/+18 are converted in order into a
local RECT using the native FTOL helper. Selector zero invokes the outlined
path; any nonzero value invokes the existing plain path. The 28-byte partial
view is not a recovered complete original class. Three real callers at
`0x00454C60`, `0x00454D00` and `0x00454DE0` reach this function. They now have
source, with direct/right-aligned exact proofs in `FORMATTED_TEXTURE_TEXT.md`;
their original data owner remains indeterminate.

## Focused evidence and remaining gaps

The cold diagnostic family uses pinned VC9 RTM `/O2 /Ob1 /Gy /MT /GL`, with
new outlined/placement sources and only directly needed existing plain-text
and pixel-filter source as support. A surface-primary family also produced
530 non-exact bytes. Changing foreground rectangle addition order to the
native decompiler hypothesis retained that result. Swapping text/font-choice
declaration order in the new placement interface changed diagnostic positions
but did not close its 173-byte extent. Do not repeat those contexts without
new evidence. Native frame/type and original TU context remain open.
An independent surface-primary `/Os` probe retained `/O2` for supports and
produced425 bytes/485 unequal diagnostic positions, also non-exact. It is not
the maintained profile and must not be mistaken for a shorter accepted extent.

The earlier plain drawing source is unchanged; in this new support context it
is403 bytes and still non-exact. This is not a replay of its old context or of
any previous canonical proof. No old exact unit was replayed.

Private `.analysis/outlined-text-differential.json` records123 bounded cases.
They execute all three new complete bodies plus actual directly called text
filters/plain drawing, and compare image buffers, unchanged workspace/views,
API order and arguments, rectangle values, AL result, cleanup, preserved
registers and stack balance. An independent rectangle reference checks the
surface outline, including duplicate positions and modeled writes to RECTs.
Cases cover all fonts, formats21/25/26, plain/outlined routing, both spacing
branches, signed spacing/coordinates, empty/odd/even/CP932 strings, height
gates, clipping, null text on the skipped path, null surface and ignored GDI/
COM/D3DX errors.

GDI rasterization and D3DX transfer are modeled. FTOL observes actual x87 inputs
and models truncation/pop behavior for finite bounded values; it does not
validate the original helper implementation or exceptional conversion cases.
These diagnostics grant no exact, full native or full runtime authority.
The run uses one CPU/nice10 with maximum RSS117,400 KiB. Ghidra and compiler
jobs retain the existing shared lock and resource limits. All three source
origins remain indeterminate: text/GDI use and real callers alone do not prove
original game versus bundled library ownership.
