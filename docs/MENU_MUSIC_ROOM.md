# Complete music-room menu owner

`UpdateMenuPage0D` at `0x004408F0` now has complete natural C++ source in
`src/MenuMusicRoom.cpp`, with three required parsing/position operations in
`src/MenuMusicRoomSupport.cpp`. Source presence does not establish exact codegen,
original class/TU identity, native product closure or runtime behavior.

## Target ownership

Direct mapped-text-attested local Ghidra review, full instruction export,
incoming references, neighboring bytes and independent original-PE CFG establish
`0x004408F0..0x0044135B`: **2668 bytes**. This contains708 reachable instructions /
2641 code bytes,11 layout bytes and the four-cell16-byte switch table at44134C.
The old2650-byte inventory stopped at the last RET and omitted the owned terminal
alignment/table. The four apparent exterior interior entries are table DATA
references. Six plain RET/EAX0 paths belong to the function; four CC separate the
following441360 owner. Actual menu input is EAX, not Ghidra's ECX hypothesis.

There are42 physical calls to20 destinations. The94 address fields include the
switch operand and all four table cells. RGB values00FFFFFF and008080FF are
numeric constants, not DIR32 addresses; six such operands were excluded from
the initial100-field heuristic. Internal branch displacements remain bytes in
the complete comparison. No target instruction dump is public.

| Required support | Reviewed full span | Bytes | Observed private inputs/return |
| --- | --- | ---: | --- |
| SkipMenuMusicLine |441360..44139A|59|EAX cursor/EDX count; EAX cursor/plain RET|
| CopyMenuMusicLine |4413A0..4413F8|89|ECX cursor/EDI count/stack destination; EAX cursor/RET4|
| TweenMenuMusicPosition |441400..4414DE|223|EAX VM/ECX target/EDX initial; plain RET|

These full bodies have no foreign interior reference or table. Their following
CC gaps are5/7/1 bytes. They support the large owner and have no separate exact
acceptance units. Source uses ordinary stdcall signatures and keeps /GL.

## Operations retained from the target

On opening frame1, the page selects the first item, starts the menu/music
animations, conditionally creates the ASCII footer, and loads `musiccmt.txt`.
A record begins with an at-sign, then supplies a filename, title and eight
comment lines. Other lines, including hash comments, are skipped. Both readers
share the loader's remaining-byte cell, consume CR/LF runs and return the cursor.
Copying replaces the delimiter with NUL and uses the original unbounded strcpy.
The marker's advance does not separately decrement the count. EOF access order,
missing-file cleanup and unchecked array indexing are preserved.

The additional menu view records20 physical track-ID cells at60C, eight comment
IDs at65C, count/comment state at680..68C,32 filename slots of40 bytes at690,
32 title slots of42 bytes atE90 and32 groups of eight42-byte comment slots at16D0.
These physical spans do not establish safe parser bounds. Scroll index58D0 and
owned file-buffer pointer5B6C remain independent. Existing menu/header layouts
and accepted source files are unchanged.

Frames below10 create two track entries at a time, without a new lower-frame
clamp. The visible window contains ten rows. Coordinates use float 64.0 for x and
double96/20/4 arithmetic, with explicit float destination rounding. The selected
row shifts x by4. Movement snaps when absolute rounded y difference is at least
float40, otherwise it starts the real four-frame/mode0 tween at VM90. Its two
control vectors read the mutable global at4C326C; they are not hard-coded zeros.
The displayed position is VM3DC, distinct from the existing secondary-actor
position view at3E8. Visibility bit2 and short events2/3 follow the selected row.

Both opening and active states refresh one of eight comment lines on even
frames. Unheard tracks first display the eight warning strings from4A7D80 in
color008080FF; the next confirmation permits playback. Cursor movement clears
the pending warning only after all eight lines have been refreshed. Playback
retains optional sound command4, command2 and the native write to statistics
byte2DDFA itself, rather than inventing a selected-track index there. Confirm
returns before testing cancel. Cancel frees the text buffer, pops selection,
signals all track IDs and comment trees, and starts the closing timer. After10
frames, closing stops both animations, signals the footer, returns to page1,
loads title music, calls42A150 with both arguments0, and pops history again.
The reviewed42A150 boundary is stdcall(position, track), returns0 and marks the
indexed heard byte after enqueueing playback; its full body is not mapped here.

## Compilation and open gaps

The public compile-only recipe is `probes/menu-music-room-context.json`.
It uses the pinned VC9 RTM21022 profile and real controller callback route,
with43 support sources and the existing two normal supports; established linked owners retain /GL.
Cold source/tool/producer/PE/MAP/PDB receipts bind the diagnostic image. Existing
accepted source inputs remain byte-for-byte unchanged, so no canonical family
requires rebuilding solely for this added diagnostic context.

The first source contribution was2440 bytes/661 instructions, with93 replayed
fields and2519 full differences. It generated44 calls because two native inline
state/timer transitions were emitted as extra calls. The retained source now
spells those observed inline transitions and shares the real file-size cell.
That restores42 calls/six returns and the complete four-cell table, producing
2584 bytes/677 instructions/93 fields and2506 full differences. The missing
native field is a second renderer-global read in the inlined two-list lookup;
source caches that pointer. Filename still used a stack argument because the
loader body was absent. The initial43-support context includes its existing real source and canonical
supporting callers. This restores filename-in-EAX at the loader call, while
full size/difference/field counts remain2584/2506/93. Thus the specific loader
argument choice is resolved, but it does not explain the whole-owner gap.

| Complete contribution | Native bytes | Source bytes | Full differing bytes | Replayed fields |
| --- | ---: | ---: | ---: | ---: |
| Music-room primary, current typed-selection candidate |2668|2680|2416|94|
| Required skip reader, support only |59|59|0|0|
| Required copy reader, support only |89|90|79|0|
| Required position tween, support only |223|213|193|4|

A used `double distance = fabs(delta)` now retains the library result type,
while the input delta remains float. Pinned VC9 emits the same native sequence
of DWORD delta store/reload, FABS, DWORD absolute-value store/reload, float40
comparison and parity branch. Nevertheless the nominal double local changes
stack alignment: that earlier primary had698 instructions/2624 reachable bytes,
8 layout bytes and all16 table bytes, with `AND ESP,-8` and frame2C. Native has
708/2641,11 layout and frame34. The length gap narrows84 to20 bytes, but the full
byte difference count increases2506 to2511: this is structural evidence, not an
overall byte improvement or exact acceptance.

That allocation restores the second renderer-global load. All94 field
destinations have the native external destination multiplicities, including
all42 calls. Six RET4 paths and a stack menu argument remain instead of the
native plain RET/EAX argument. The initial count clamp was still folded in
that version.

The player-data page supplied a useful independent comparison: its typed,
owner-based selection stores naturally preserve the native count reload.
MenuMusicRoomView now types the already observed selection at24, leaving every
subsequent offset and total view size unchanged. The pointer comes from this
view and both count writes go through its owner. Pinned VC9 now preserves the
complete count reload/zero/negative/positive clamp without volatile fields or
artificial accesses. Current whole owner is2680 bytes/711 instructions/2650
reachable bytes,14 layout bytes and all16 table bytes, with94 replayed fields
and2416 full differences. The size gap is12 bytes, down from20; full differences
drop95 from2511. This remains non-exact. Frame2C/native34, stack/native EAX menu,
RET4/plain RET, early pointer placement and private callee arguments remain open.
The original nominal class/TU still cannot be inferred from this physical view.

A scoped-pointer variant with the same typed selection gives2648/2508/94 and is
discarded. Generalizing the old SelectFirst helper alone leaves2648/2511/94;
that operation alone did not explain the count folding. The isolated43-support family and the earlier46-support player-data family
cold-reproduced the retained field projection. The current48-support player-data
and practice-stage contexts retain the same2680/2416/94 result; see
[MENU_PRACTICE_STAGE](MENU_PRACTICE_STAGE.md).

A named double y expression also produces2648/2511/94 and alignment8, differing
from the retained candidate in one SIB byte. This does not prove which original
local had double type. Direct `fabs(delta)` comparison restores the old unaligned
2584/2506/93 result. Ordinary inline, definition-after-use and a genuine member
owner produce the same2648 replay as that earlier double-local candidate; scoped
selection instead gives2632/2539. A real controller/page unity context and a five-source menu unity context
(controller, primary page/support, music page/support) also replay identically
to that earlier candidate. These variants are discarded. An attempted
cross-TU inline-only split failed to emit its helper and did not link; it is not
a valid codegen comparison. Scalar/control-pointer and memcpy support variants
do not change the large owner and are not retained. Scalar copies introduce
x87 loads/stores instead of native raw word copies; memcpy restores six global
reads but gives229/native223 and214 differences in that support body.

MenuMusicRoom.cpp and MenuMusicRoom.hpp change among the235 frozen inputs at
740cce1; no accepted unit depends on them. The two affected diagnostic families
are cold rebuilt with current receipts. Checkpoint proof is now
`.analysis/verify-menu-player-data-checkpoint.py`; the previous alignment
checkpoint's type hashes and2648 measurement are historical.

The copy reader chooses EAX cursor/ECX count instead of ECX/EDI; the tween keeps
its VM on the stack and coalesces six native global component reads into three.
These concrete allocation/aliasing gaps remain open. All source fields agree
with the independent PE relocation/control-flow field enumerator. The support
reader's zero result is a diagnostic, not an accepted exact unit.

This family is a compilation diagnostic with unresolved data anchors, not a
runnable game. There is no canonical music-room manifest entry and no exact
credit. New-context output from already accepted dependencies does not inherit
those dependencies' canonical exactness.
