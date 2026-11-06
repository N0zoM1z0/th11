# Complete practice-stage selection page

`src/MenuPracticeStage.cpp` reconstructs the complete page08 state machine.
`src/MenuPracticeStageSupport.cpp` supplies its transition overlay and music-fade
operations. This work adds complete source coverage and compiler context for
large menu owners; it does not add accepted exact units or a runnable game.

## Reviewed ownership

Direct mapped-text-attested Ghidra code, references, neighbors and table review,
checked against an independent original-PE CFG, establish:

| Function | Complete extent | Bytes | Instructions/code | Layout/table |
| --- | --- | ---: | --- | --- |
| Practice-stage page08 |43D6A0..43DA9F|1024|285 /1003|1 alignment +five-cell20-byte table|
| Transition overlay |40FCA0..40FCF0|81|24 /81|None|
| Music fade command |42A270..42A2BF|80|28 /80|None|

The old1003-byte page inventory ended at its final RET. The complete owner also
contains NOP43DA8B and the entire switch table43DA8C..43DA9F. Its five apparent
foreign interior references are owned table DATA, and the next function begins
immediately at43DAA0. There are21 physical calls to12 distinct targets,79 address
fields and13 plain RET/EAX1 paths. The two required support operations have no
owned table or external interior entry; transition has15 following CC, while
fade ends directly before42A2C0. Their bodies have2/5 address fields.

## Selection and keyboard paths

A partial physical view types the existing selection object at24. Opening sets
count6, restores the saved selection using the native upper/lower clamp, creates
script68 and the selected character's69-based script, then enters state1.
Frame greater than10 activates state2. Up/down movement, sound12 and the
cancel-before-confirm ordering are retained. Cancel saves the selection and
starts state4; closing at frame6 stops both scripts, selects page7 and restores
selection history.

Confirmation indexes the existing statistics at
`(selection1 + selection0*3)*68D4 +5B1 +(current +difficulty*6)*8`.
An unlocked entry enters state3 and queues sound10; a locked entry queues37.
Both branches save selection, zero the shortcut and read256 raw keys. This page
tests the snapshot return as **zero/nonzero**: zero checks virtual digits31..39,
nonzero checks scan codes02..0A. The first high-bit key chooses shortcut1..9.
This differs from the player-data page's explicit return2 test; preserve the
observed branches rather than correcting the backend choice. Downstream meaning
of the numeric shortcut beyond its observed stores remains open.

This is the actual second caller of458100: at43D833 it loads ESI=4C9690 and
calls at43D847. Player-data loads ESI=4C9BC0 at43ECFC and calls at43ED01.
Completing this caller restores a genuinely variable output buffer under /GL,
without a synthetic caller or a declared private register convention.

## Transition signatures

At frame10 of state3, the page passes **480.0f,392.0f** to the transition overlay,
then calls the existing five-argument screen-effect factory4489E0 with
`(5,32,0,0,57)`. At frame40 it pushes selection history, selects page2,
sets the stage pointer and both variant globals, queues a fade of **6.0f**,
and writes7 to main-runtime558.

The fade argument is easy to misread:43DA1F pushes the calculated stage pointer,
but43DA20 immediately overwrites that stack slot with an x87 float store. The
call therefore receives one float, not a pointer. Complete42A270 ends with RET4.
It divides duration by the time scale only when the scale is nonzero and not
greater than1, including negative and unordered values; then it rounds through
its float parameter slot, converts to int and queues opcode5 with an empty
string. The units of that duration are not established here.

The overlay constructs `{x,y,0}` even when its stored VM ID is already present.
If absent, it calls455A70, returns an AnimationVmId value and stores the value
at ASCII-manager184B4. The complete153-byte factory boundary is reviewed but
its source is still open; its position input is native EAX, with four stack
slots including the hidden value-result pointer. Its direct script binding
must not be replaced by the zero-position initializer456910.

## Cold compilation

The public diagnostic recipe is `probes/menu-practice-stage-context.json`.
It retains VC9 RTM21022, /O2 /Ob1 /MT /EHsc /GL and the real controller callback
route, with48 support sources and the same two established normal supports.
Complete PE/MAP/PDB contributions and every address field are compared; no
prefix is cropped and no internal branch layout is normalized. Unresolved
callee/data anchors keep this executable diagnostic rather than runnable.

Current complete candidates in this broader context:

| Function | Native bytes | Source bytes | Full differences | Candidate fields |
| --- | ---: | ---: | ---: | ---: |
| Practice-stage page08 |1024|1044|970|76|
| Player-data page0A |2448|2488|2319|169|
| Record refresher |750|715|676|17|
| Music-room page0D |2668|2680|2416|94|
| Keyboard snapshot, support only |101|101|0|6|
| Transition overlay, support only |81|82|35|2|
| Music fade, support only |80|86|83|6|

All call counts agree. Page08 has283 instructions/1024 code bytes and all20
table bytes. Its menu remains on the stack, RET4 differs from native plain RET,
and the hidden ID result reuses the incoming parameter slot instead of native
four-byte scratch. Two missing fields are cached pressed-input reads; the third
is a hoisted saved-selection read. Register choice and digit-tail sharing remain
open. No masked match, normalized branch comparison or cropped extent is used.

The first page candidate was1060/1005 full differences. A positive state guard,
negative unlock test and single selection assignment give1060/988. Returning
the selected value from the clamp helper gives1044/970 and is retained. Modeling
the page as a real object member with a correspondingly changed real controller
call produces identical code/1044/970 and is discarded; member syntax does not
solve this private ABI. No artificial register or stack workaround is retained.

The original one-caller snapshot was109/99 differences with an embedded output
buffer. Completing page08 restores ESI output,101 bytes and six fields, initially
with73 differences from the disabled-path placement. A natural positive polling
guard restores the full native branch order and zero differences. This is a
support diagnostic, not a separately accepted leaf or original-TU claim.
Player-data now contains its formerly missing169th field, but the complete page
changes2472/2273 to2488/2319 in the more complete caller graph; that larger owner
remains open. Music stays2680/2416, and record formatting stays715/676.

Both public page08 and player-data recipes contain48 supports and independently
cold-reproduce this evidence. The accepted236-function manifest is unchanged;
only the snapshot source changes among239 previous source inputs, and it has
no accepted canonical dependents. Three new source files extend the freeze to242.
The checkpoint verifier is `.analysis/verify-menu-stage-checkpoint.py`.
