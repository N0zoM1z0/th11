# Complete menu controller

UpdateMenuController at4399B0 has complete natural C++ source in
src/MenuController.cpp/.hpp. The reviewed owner is1,224 bytes, including all
sixteen switch cells. The isolated VC9 /GL context generated1,192 bytes and remains
non-exact. All36 physical call sites are present; no new exact unit is claimed.

## Ownership and real entry

Direct mapped-text-attested Ghidra review and an independent original-PE CFG
establish4399B0..439E77:300 reachable instructions/1,157 code bytes,3 internal
alignment bytes and64 bytes in the table439E38..439E77. All16 apparent foreign
interior entries come from those owned DATA cells. Eight CC separate439E80.
The menu arrives on the stack and stays in EBP; both returns use RET4/EAX1.
EBX is saved after the idle prefix, with EBP/ESI/EDI saved earlier. Ghidra's
thiscall/undefined8 hypothesis is contradicted by the complete native flow.

The real registered update callback439F10..439F16 was absent from the initial
function inventory. It is PUSH ECX/CALL4399B0/RET, with DATA registration at
4395E6 and four preceding/nine following CC. Its ordinary fastcall wrapper is
normal-COFF support so the controller retains its real stack argument/RET4.
No existing /GL owner was downgraded. The adjacent draw dispatcher439E80 owns
140 bytes:105 code/43 instructions,3 alignment and32 bytes in eight cells at
439EEC..439F0B. Its auto-analysis105-byte extent omitted the owned table/layout.
That draw owner has boundary review only; no draw source body is claimed.

The four marking calls use4564C0..45650D/78 bytes, with renderer ESI/file EDX.
The complete supporting source traverses both real renderer VM chains, caches
the next node before mutation, and ORs04000000 for matching file pointers.
There is no foreign interior reference or table, and two following CC.

## Complete operations

Page1 increments the idle counter, tests only the low16 input bits, and enters
the replay path after1,800 idle frames. It sets stage flags, copies the selected
filename, reads the replay, advances the reloaded signed demo index modulo4,
and scans eight24-byte stage records. The native unchecked pointer accesses
and possible index8 result are retained. Replay header18 supplies selections
at5C/60 and difficulty64; stage selection storage has40-byte stride. The helper
435C50 destroys replay contents before the separate free call; it does not apply
the replay. Demo strings and replay storage remain extern data with unresolved
allocation capacities. The menu entry global is4C3250, distinct from audio state
4C3E00.

Music flag1 starts a five-frame delay before enqueueing the title track, an
optional opcode4 and opcode2. Entry mode is reloaded after the commands, before
the statistics write. Page0 marks four cached renderer files, signals/clears the
cached ASCII overlay ID, restores selection/difficulty and routes entry modes
into the shared primary, extra or replay page blocks. All sixteen page selectors
and the shared audio-reset routes are implemented. The current graph supplies primary, audio, joystick and music-room bodies;
seven other page handlers remain source-open ordinary declarations; their observed private register inputs
are recorded separately and are not forced into the source ABI.

The timer copies current to previous, uses a fast increment only between the
promoted float0.99 double and float1.01 bounds, otherwise rounds the elapsed sum
to float before conversion. Both complete return paths and all64 table bytes
remain in every comparison. Original class identity, full object/replay allocation
sizes, TU partition, native-product closure and runtime behavior remain open.

## Compiler findings and comparison

The first complete controller generated1,172 bytes/288 reachable instructions,
with one page-set/page-update pair merged. Replacing the explicit selection
branches with a generic inline selection operation merged another pair and
generated1,144 bytes. That spelling is discarded. Explicit count<=1 instead of
count<2 preserves the separate paths under VC9, restoring all6 page-set and4
primary-page-update sites. The retained strcpy also produces the native
single-increment, destination-minus-source copy loop naturally.

Last isolated complete primary contribution:1,192 bytes/290 instructions/1,125 code,
3 alignment and64 table bytes. Its100 address fields are exhaustively replayed;
full comparison reports1,138 differing bytes, including the32-byte size gap.
Target has101 fields: its input-reset and demo-reset stores are physically
separate, whereas the current source shares the idle-zero store. The idle/demo
block placement, input zero-extension, EBX save/use, register scheduling and
source-open page ABIs remain concrete gaps. Equal call counts are structural
evidence and do not imply byte equality.

The 78-byte marker `MarkMenuFileAnimations` is strict cold
linked-PE **78/78 exact** (`menu-mark-file-animations`). The registered
seven-byte `MenuControllerUpdateCallback` at 439F10 is now also
**7/7 exact** under the independent normal-COFF unit
`menu-controller-update-callback`, replaying its sole REL32 field at
offset2 to the reviewed menu owner4399B0. Neither leaf proves
that the full 1224-byte menu owner is exact. Under this real controller context,
the audio apply owner remains3,836/110 fields/zero diagnostically; its992-byte
caller still has the same9 prologue differences after61 fields. The real upper
caller therefore does not by itself explain that EBP save placement. Canonical
audio acceptance remains its existing public primary-TU unit and receipts.

## Reproduction and limits

The public compile-only context is probes/menu-controller-context.json. It
selects19 supporting sources, with only the real callback compiled normally;
the controller and established owners retain /GL. All dependencies are public
source. Build through the existing cold producer from the repository root:

```sh
scripts/repo-python - <<'PY'
import importlib.util, json, sys
from pathlib import Path
sys.path.insert(0, 'scripts')
spec = importlib.util.spec_from_file_location('builder', 'scripts/build-match-unit.py')
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)
module.build(json.loads(Path('probes/menu-controller-context.json').read_text()))
PY
```

The receipt binds source, tools, producer and PE/MAP/PDB artifacts. Unresolved
interfaces are diagnostic data anchors, never runnable replacement functions.
This image is a compilation context, not a game product. The controller has no
acceptance manifest entry while full equality is open. This checkpoint originally changed no accepted source. Later typed-field
updates for the primary page supersede its old receipt/source freeze; see
[MENU_PRIMARY.md](MENU_PRIMARY.md) for the current real-page context and gaps.

The joystick context now supplies page04 as well as the primary page and restores
all36 physical parent calls, but its1188-byte contribution remains non-exact.
The previous isolated/primary-only receipts are historical after the typed fields
and shared coefficient correction. See [MENU_JOYSTICK.md](MENU_JOYSTICK.md).

The music-room context also supplies the complete2668-byte page0D owner at
4408F0, including its previously omitted16-byte table. Its three required
parser/tween bodies add no leaf acceptance units. See
[MENU_MUSIC_ROOM.md](MENU_MUSIC_ROOM.md) for current cold codegen gaps.

The player-data context supplies the full2448-byte page0A owner and750-byte
record refresher, alongside the current typed-selection music-room candidate.
See [MENU_PLAYER_DATA.md](MENU_PLAYER_DATA.md); canonical exactness is separate
from these broader linked diagnostics.

## Complete 140-byte menu draw owner (2026-11-12)

`src/MenuControllerDraw.cpp` now maintains the complete source of the
five-path `DrawMenuController` (`0x439E80..0x439F0B`), adjacent to the 1224-byte
update dispatcher and the independently exact callback. Original Japanese PE
review covers **105 code bytes/43 instructions**, the **three-byte internal
alignment**, and the **eight-cell, 32-byte jump table** at `0x439EEC`.
The page-index switch checks the original page value at menu +0x1C and
routes 8 to `0x43DAA0`, 10 to `0x43F410`, 11 to `0x43E2F0`, 14 to
`0x43FC50`, and 15 to `0x440520`; other selectors return 1 without a
page call. Cases 14/15 receive the controller as one stack argument.

The isolated and real-update/menu-producer pinned VC9 `/GL` primary-TU
comparisons both compile the complete five-route source: **108/140 bytes**,
**five calls and eight real jump cells**, with 14 current address fields.
The native aligned stack frame, saved EDI and original EAX receiver are
not yet recovered. The full original owner is retained without cropping its
table. This is source presence, not byte-exact acceptance. Local comparison:
`.analysis/probe-menu-controller-draw-20261112.py` and log.

## Full 427-byte practice-stage page draw (2026-11-12)

`src/MenuPracticeStageDraw.cpp` now contains the complete source of
`DrawMenuPage08` at `0x43DAA0..0x43DC4A`, a substantial original owner
which the 140-byte menu draw dispatcher calls directly. Full attested PE and
direct Ghidra review confirms **427 bytes / 108 instructions**, only
**two calls** to the genuine `AsciiManagerPrint` at `0x4015A0`, **two
return paths**, a sole real caller, no foreign interior entry or owned table.

Menu states 2/3 enable the practice screen. It sets the ASCII manager's
+0x18494 rendering flag, prepares a three-float text position (two native
x columns, 168/296, at y=216), and iterates the six stages. For each
stage it indexes the real statistics buffer using
`(selection0*3 + selection1)*0x68D4 + (stage + difficulty*6)*8`, reads
its +0x5A9 unlock byte and +0x5A4 high-score integer, and sets the
original white/grey/yellow/black colors for locked, unselected, selected,
or timer-modulo-four blinking entries. It formats the real stage name with
`"%s  ---------"` for locked stages or `"%s  %.8d0"` for an unlocked
score. Every line moves y by the original double constant 18.0; the function
finally restores white and clears the ASCII draw flag. Only state 2 with
elapsed time under 10 skips the list; states other than 2/3 return
immediately.

The naturally typed `AsciiManagerPrint` takes a manager and position explicitly;
the original private `/GL` call had ECX manager and EBX position with its
format arguments on the stack. The parent dispatcher now supplies the menu
object to page08, instead of declaring it as a no-argument stub.
Scoped pinned VC9 `/GL` linked comparisons give **399/427** with the
independent caller and real menu producer (104/108 instructions, both
native calls and both returns, 17 candidate fields), and **406/427**
with the actual ASCII formatter (107/108 instructions, both calls/returns,
18 candidate fields). The updated parent emits 112/140 bytes isolated,
120/140 with the page08 implementation, preserving all five calls and the
eight owned table entries. Original private ABI/stack/x87 scheduling remains
open; these are full-source, *non-exact* diagnostics, not partial owners.

Local reproduction:
`.analysis/probe-menu-practice-stage-draw-20261112.py` and
`.analysis/probe-menu-controller-draw-linked-practice-20261112.py` with their
logs, using only the changed page/dispatcher and direct producers.
