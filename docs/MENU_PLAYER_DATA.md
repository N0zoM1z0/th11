# Complete player-data menu and record list

`UpdateMenuPage0A` now has full natural C++ source in `src/MenuPlayerData.cpp`.
The complete record refresher is in `src/MenuPlayerDataRecords.cpp`; three
required operations are in `src/MenuPlayerDataSupport.cpp`. These add source
coverage, not accepted exact units or native-product/runtime closure.

## Reviewed ownership

Direct mapped-text-attested Ghidra review, full code export, incoming references,
neighbor inspection and independent original-PE CFG establish:

| Function | Complete native extent | Bytes | Reachable instructions/code | Layout/table |
| --- | --- | ---: | --- | --- |
| Player-data page |43E790..43F11F|2448|620 /2422|10 alignment +four-cell16-byte table|
| Record refresher |43F120..43F40D|750|233 /747|3 alignment|
| Unlock operation |4393B0..439412|99|27 /92|7 alignment|
| Difficulty count |40BC70..40BCBD|78|28 /78|None|
| Raw keyboard snapshot |458100..458164|101|32 /101|None|

The old2432-byte page inventory stopped at the last RET and omitted the whole
switch table43F110..43F11F. Its four apparent foreign interior entries are owned
DATA references. The next750-byte function begins immediately at43F120; no
following CC separates the table and next owner. The page has three plain
RET/EAX1 paths,64 physical calls and169 address fields. The refresher includes
all late VM-search blocks, GS check and plain RET/EAX0, not a cropped text prefix.
It has six physical calls and17 fields; the B8 frame contains a168-byte text
region. No support body has an external interior entry or owned table. The
refresher/unlock/count/snapshot have2/13/2/11 following CC respectively.

Indexed absolute fields require review even when the x86 memory operand has a
base register. Native count78 has five table address fields, not zero. Keyboard
loops contain additional indexed global addresses, including pressed-buffer
bias-2 after advancing a cursor by3. Original PE relocations are stripped;
these native fields are classified from reviewed operands and object addresses.
Candidate fields are independently checked against PE relocations and the
complete control-flow enumerator. RGB constants are numeric values, not pointers.

## Physical state and operations

A new overlay types three adjacent existing selection objects at24/FC/1D4,
plus displayed-row count2AC and ten value IDs5CC. It does not change the shared
menu layout. Character count is6, difficulty count5 with initial selection1,
and record-page count is `(difficultyCount+9)/10+1`, initially selected0.
Difficulty and page wrapping are enabled. Original class and allocation identity
remain unknown; no new count clamp or failure matrix is introduced.

Opening conditionally starts script5C and the ASCII footer, creates script64,
then three selection-dependent animations and A6/A7/A8/A9/A3/A4/A5/AA. At frame7
it becomes active. Up/down changes difficulty and signals A8/A9; left/right
changes character and signals A6/A7. A difficulty change clamps the record page
to1 using its **old** count, refreshes if the previous page was positive, and
only then computes the new page count. Preserve that order. Character changes
replace both the group (`selection/3+96`) and character (`selection+98`)
animations as applicable and refresh a positive record page.

Confirmation creates ten row VMs only when leaving page0, advances the wrapped
record page, then refreshes or signals all ten IDs when returning to page0.
Cancel closes all selection animations and signals the ten row VM trees.
At closing frame6 it stops scripts64/5C and the footer, returns to page1 and
pops character selection history. The native cached renderer/two-list search,
unchecked accesses and short event1 propagation are retained.

The record refresher filters175 signed difficulty bytes. Its skip loop counts
matching entries without a new175-bound guard; the display loop then handles
up to ten entries and blanks unused rows. Global record stride is90, with text
at statistics+27B64 and an observed field at+84 gating visibility. A character's
two displayed counters are at `character*68D4 + spell*90 +6EC/+6F0`.
The original record text capacity and the two counters' semantic names remain
unproved; the view names them first/second rather than assigning game meanings.
Known names use the original unbounded strcpy, signed strlen result and space
padding to42 bytes in a168-byte stack region. Known/hidden/blank branches all
clear missing IDs and still call the centered formatter, without invented null
returns. Known format is `No.%3d %s %4d/%4d`; hidden text remains external data.
High color is **00FFFF80**, independently derived from native `EFEFEF+100F91`,
not0100FF80. Low/hidden/blank colors are EFEFEF/808080/FFFFFFFF.

## Complete hidden input path

Only character3/difficulty4 runs the raw keyboard path. Input mask80103 resets
sequence position/time before sampling. A256-byte previous buffer is copied,
and the snapshot helper clears its output before checking polling enable.
Return0 means disabled; return2 uses GetKeyboardState and ignores its BOOL;
return1 uses DirectInput GetDeviceState and reacquires on every nonzero HRESULT,
including8007001E. Existing InputBackend globals and COM method types are reused.

For return2, all26 virtual letter keys are mapped to scan codes through a zeroed
temporary buffer, then copied back to current. Both successful paths compute
`(previous ^ current) & current` across256 bytes. The12 expected scan-code
indices remain external data. Correct keys advance and reset time; another key
among indices0..56 resets progress. A completed position triggers the unlock
operation on the next successful sample, even without a new edge, queues sound44
and resets position. Time increments after processing (including disabled polls)
and resets position/time when it exceeds300. Ordinary menu cancel is still
processed afterward. No new modifier filtering or backend normalization is added.

Unlock writes four11111111 words at statistics2DDEA..2DDF6, then six character
groups, each containing4x6 flag cells with stride8. These are maintained target
operations; the diagnostic has not executed them against game data.

## Compilation and remaining work

Public `probes/menu-player-data-context.json` keeps VC9 RTM21022 and the pinned
/O2 /Ob1 /MT /EHsc /GL profile, real controller callback and two established
normal supports. It now includes the complete practice-stage caller and its
required operations, for48 supports. Cold receipts bind sources, tools,
producer and PE/MAP/PDB identity; unresolved anchors remain diagnostic.

| Function | Native bytes | Source bytes | Full differences | Candidate fields |
| --- | ---: | ---: | ---: | ---: |
| Player-data page |2448|2488|2319|169|
| Record refresher |750|715|676|17|
| Difficulty count, support only |78|78|0|5|
| Unlock, support only |99|99|14|1|
| Snapshot, support only |101|101|0|6|

The large page has all64 calls,626 instructions/2440 code bytes,32 layout bytes
and all16 table bytes. Its stack menu/RET4 and frame8 differ from native EAX/
plain RET/frameC. Original class/TU and register/branch allocation remain open.
The record refresher keeps native ECX input, B8 frame, all six calls and17 fields;
its text/lookup placement and counter addressing remain open. The previous
separate-record-offset expression731/680 was discarded, not accepted.

The initial46-support context had only one real snapshot caller, so /GL embedded
the current buffer and removed its argument: page2472/2273/168 fields and
snapshot109/99/8. Completing the actual second caller page08 restores generic
ESI output and the page's169th field. Direct attested sites are43D833 loading
ESI=4C9690 before call43D847 and43ECFC loadingESI=4C9BC0 before call43ED01.
With both callers, the snapshot first gives101/73/6; replacing its early disabled
return by the natural positive enabled branch restores101/zero/6. The full
player-data candidate becomes2488/2319; restored ABI evidence does not imply
that the large owner is exact. No synthetic caller or private ABI declaration
is used. See [MENU_PRACTICE_STAGE.md](MENU_PRACTICE_STAGE.md).

Count78 and snapshot101 replaying zero in support add no independent exact
credit. Music measured2680/2416/94 in the broader context. No accepted unit
depends on the changed MenuPlayerDataSupport.cpp.
