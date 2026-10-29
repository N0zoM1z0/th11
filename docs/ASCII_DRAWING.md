# ASCII formatting and full glyph drawing

Five further owners have complete source, covering2,007 target bytes. Direct
local Ghidra reviews attest the full mapped TH11 text and inspect callers,
references, instructions and adjacent bytes. All origins were reviewed: the
four ASCII owners are authored-game code; the shared ANM UV setter remains
indeterminate. AsciiInf class/string evidence and its already reviewed
callbacks connect the formatting/drawing paths to the same game object.

| Function | Address | Complete target bytes | Source |
| --- | --- | ---: | --- |
| AsciiManagerPrint | 4015A0 | 83 | AsciiFormatting.cpp |
| AsciiManagerPrintSmall | 401600 | 106 | AsciiFormatting.cpp |
| DrawAsciiManager | 401670 | 1,636 | AsciiDrawing.cpp |
| DrawAsciiOverlay | 401CE0 | 101 | AsciiDrawing.cpp |
| SetAnimationSpriteUv | 401E40 | 81 | AnimationSpriteBinding.cpp |

## Corrected complete owner

The old DrawAsciiManager inventory stopped at RET8 at401CC1: a1,618-byte
bounding code span with six skipped alignment bytes at4016BA..4016BF. Its
switch at4018D5 indexes a table at401CC4. All four table entries point back
into the body, to4018DC/401953/4019CA/401AB5. The complete owner therefore
includes two alignment bytes401CC2..401CC3 and all sixteen table bytes through
401CD3:1,636 bytes total. The next twelve bytes are CC padding, followed by the
separate401CE0 function. The four apparent external interior references in
the initial review originate inside this owned table; no other external
interior reference was found.

The complete1,618 code bytes were independently decoded with pinned Capstone,
and also reviewed by Ghidra's explicit code-span query. Table values and
adjacent bytes were independently read from the verified PE. The first attempt
to decode code and table together as instructions failed at the data tail;
it is not used as a successful extent proof. Do not crop the owned table or
repeat the old1,618-byte extent in an exact claim.

## Formatted append and a field correction

Both formatters use an unbounded `vsprintf` into a256-byte local buffer and
then call the maintained queue appender. Keep the original GS-cookie codegen;
no truncation or new capacity handling is introduced. The small-font wrapper
subsequently writes1 to the last record's style at+120. Native displacement
`count*130 + 86C` is `strings[count-1].unknown120`, not the+128 draw-channel
field. An initial overlay interpretation was disproved by that displacement.
It marks the existing last record even when an already-full320-record queue
rejects the append; no guard is invented.

## Drawing flow and physical views

The manager's text VM begins at+14. Drawing starts by replacing flag bits280000
and setting140001. It filters records by their+128 channel. Each selected
record supplies position, scale, color, font style+120, camera selector+11C
and shadow flag+124. Position is VM+3DC, scales+3C/+40, dimensions+4C/+50,
selected sprite+3AC, flags+404, and color+374. The latter is diffuse ARGB:
resetting it toFFFFFFFF means white. The default glyph advance reads drawMode184A4 as a signed integer, matching
the native FILD; the lifecycle view's unsigned storage label must not trigger
an unsigned-to-float correction. The drawing source explicitly reads its
signed value. The historical `AnimationVmView` name
`spriteIndex` for374 is misleading; the signed16 sprite marker is at39C.
The drawing source uses a separate accurately named partial view.

The runtime camera view uses alternate camera+204, normal camera+31C, current
pointer+54C and camera mode+550, rooted at the existing4C3280 runtime object.
Changing the record camera flushes the current renderer, applies the chosen
perspective camera and its viewport, then records normal/alternate state.
When the last logical camera selector is nonzero, the function flushes and
restores normal camera state. Empty/no-selected-record cases also follow this
restoration path. The overlay function deliberately flushes, draws channel1,
flushes again, and restores normal state again before returning1.

Both draw functions return1, not a glyph count. Renderer quad construction
44FE30 and VM draw44F880 remain reviewed external call boundaries. The former
receives the VM and four independently observed positions in the shared
four-vertex buffer4C91D8; the latter receives renderer, VM and flag1. Their
full reconstruction is still open.

## Fonts, whitespace and shadows

| Style | Initial advance | Line height | Sprite selection |
| --- | --- | --- | --- |
| 0 | manager drawMode times scale X | 14 | byte minus32 |
| 1 | 7 times scale X | 9 | byte plus66 |
| 2 | 7 times scale X | 10 | byte plus179, with substitutions below |
| 3 | 12 times scale X | 16 | byte plus195, with substitutions below |

Style2 substitutes `/ + - * % $` with237/238/239/240/241/260. A period uses242
and changes the current advance to4 times scale X; following spaces retain
that advance until another non-whitespace glyph resets it. Style3 maps
slash/period/lowercase-s to253/254/255. Newline advances Y by scale Y times
line height and resets X to the record's original X. Space advances X without
binding/drawing a sprite. Unrecognized styles preserve the previous sprite;
no fallback glyph or bounds behavior is added.

After binding, dimensions come from the selected sprite. Optional shadows use
black with half the record alpha, draw at X/Y+2, and then restore position by
subtracting2. The normal glyph uses the complete record color and advances X.
The UV setter updates only the sprite pointer and all four UV corners. It
does not change dimensions, matrices or the signed16 marker.

## Private reproduction evidence

Target review files are `.analysis/ascii-drawing-review-next.txt`,
`ascii-draw-code-review.txt`, `ascii-draw-native.asm`,
`ascii-draw-table-review.json` and their direct-Ghidra attestation logs.
Renderer dependency hypotheses are private `ascii-render-deps-next.c`.
Compiler driver: `.analysis/probe-ascii-drawing.py`; initial/full independent
contexts are retained under `build/probe-ltcg/ascii-drawing`.

`probes/ascii-drawing-context.cpp` supplies independent manager/mode, sprite,
format/variadic, renderer and camera inputs. Real appender, UV setter, flush
and camera sources remain support code. The image is a diagnostic link
context, not a runnable game. Profiles stay VC9 RTM
`/TP /O2 /Ob1 /Gy /MT /Isrc /EHsc /GL`. Complete source and diagnostic byte
similarity grant no exact credit; only complete primary-TU cold proofs count.

## Exact checkpoint and open codegen

Four complete public primary-TU contributions cold-replay with zero byte
differences: Print83, PrintSmall106, Overlay101 and SetUv81, adding371 bytes.
The formatters each replay four reviewed fields: cookie4A2088,
vsprintf45FDD2, appender4014E0 and cookie check45FC2C. Overlay replays eleven
reviewed renderer/call/runtime-field bindings, including runtime+31C/+54C/
+8/+550. The UV setter has no relocation/control-transfer fields. Complete
extents, source/tool/artifact receipts and PE/MAP/PDB ownership all agree.

The canonical families are `AsciiFormatting`, `AsciiDrawing` and
`AnimationSpriteBinding`. Initial seven-unit cold proof also refreshes the
three directly affected lifecycle units; the final four-unit cold proof uses
the signed drawMode conversion. No full-cohort replay was run.

Main glyph drawing remains non-exact. Its final maintained contribution is
1,208 bytes with1,358 diagnostic differences against the full1,636-byte target.
Current source calls the public UV setter for all styles; the original
inlines UV stores in styles0/1 and retains calls on special-font paths. The
opaque renderer call boundaries also retain different argument scheduling.
Splitting normal/alternate camera helpers improved the candidate body from
1,192 to1,228 bytes before correcting the signed advance conversion. No bytes
were padded, cropped or patched to obtain these diagnostics.

An explicit cached sprite pointer and a SetSprite member form failed to close
the existing389-byte binder; it remains388/289. An out-of-line definition
marked inline was not emitted by VC9 and failed linking, so it was rejected.
Independent camera/renderer parameters, unlike global-only support, recover
the native101-byte overlay owner. Native renderer helpers44FE30 (632 bytes)
and44F880 (1,112 bytes) are concrete next larger reconstruction targets.

Proof logs: `.analysis/ascii-drawing-seven-cold.log` and
`ascii-drawing-final-four-cold.log`. Complete current main-function diagnostics:
`.analysis/ascii-draw-final-diagnostic.json`. Source presence and masked
similarity never grant the main function exact credit.
