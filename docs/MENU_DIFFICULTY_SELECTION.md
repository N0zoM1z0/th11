# Complete difficulty-selection menu

`src/MenuDifficultySelection.cpp` reconstructs page05. Its required event3
operation is in `src/MenuDifficultySelectionSupport.cpp`. This maintains a
complete source owner and its compiler context, with original class/TU,
exactness and native-product/runtime closure still separate unknowns.

## Reviewed complete owners

Mapped-text-attested direct Ghidra full code/span/reference/table/support review
and an independent original-PE CFG establish:

| Function | Complete extent | Bytes | Instructions/code | Layout/table |
| --- | --- | ---: | --- | --- |
| Difficulty page05 |43C7C0..43CBDF|1056|298 /1026|10 layout +five-cell20-byte table|
| Execute indexed menu event3 |439230..439244|21|7 /21|None|

The old1034-byte page bound stopped at43CBC9. Sparse Ghidra body1026 also skips
eight alignment bytes before43CA40. Two-byte MOV EDI,EDI at43CBCA..43CBCB and
all table cells43CBCC..43CBDF are owned. Five apparent foreign interior entries
are table DATA; the next character page43CBE0 follows the table immediately.
Eight CC precede this entry after43C7B7. The page has34 direct physical calls
to16 distinct targets,61 address fields, frame4 and five RET4/EAX1 paths.
Keep the late child-result block43CAE8..43CAEC after the earlier common return.
The21-byte dependency has one call/one field, no external interior entry/table
and11 following CC before439250. It loads an indexed value ID and delegates to
the existing execute-event helper with event3, preserving EDI. Its return is
unused; the source introduces no fabricated result or leaf acceptance unit.

## Snapshot, opening and input

Each invocation captures `75 +(currentDifficulty >=4)` **before** the state
guard. Opening then checks heading5C; when absent it starts5C and creates footer
script12/layer16 from the ASCII animation file, storing its ID at5C8. Count is4
when the preferred difficulty is below4, otherwise1; the current difficulty is
then assigned the preferred value. Stop/recreate uses the previously captured
75/76, even when initialization changes the global difficulty. The page sends
root execute-event3 and short event11+current, creates script60 and enters
state1. Time greater than6 activates state2.

Only current difficulty below4 permits up/down pressed/repeat10/20. A change
queues12, executes indexed event3 and sends short event7+current. Navigation does
not publish the selected difficulty immediately. Cancel102 precedes confirm80001,
sets state4, queues11 and immediately stops the captured75/76 animation.

Confirmation captures the renderer and root-ID reference and signals root event6.
The normal branch calls the shared child finder for `6B +selection.current`,
then looks up its returned value using the global renderer and sends event2.
The Extra branch directly searches signed script6F through drawNode10 and uses
the previously captured renderer for both lookups. Missing parent IDs are cleared,
then the observed unchecked continuation remains; no null-return repair is added.
All found-VM events propagate through related nodes when marker18 is zero.
State3 is set before queuing10. The normal out-of-line finder and constant Extra
inline search are distinct target observations, not interchangeable call counts.

## Delayed transitions and saved values

At state3 time14 or later, stop60 and select page6. Normal difficulty is taken
from the selector; Extra keeps the current value. The preferred difficulty is
published, history is pushed, and the next selector receives wrapEnabled1 and
count2. The saved character4C325C is captured before the existing fastcall clamp;
its raw saved value is then published to the character global, not the clamped
return value.

Closing at state4 time6 or later stops60/5C, signals the footer through the
existing4070E0 operation, and selects page1. Normal difficulty comes from current
selection; Extra reads the observed menu field58E8, whose wider semantic role
remains unproved. Both current/preferred globals are stored before history pop.
The footer ID is not cleared. No allocation/input-failure or caller runtime
harness is introduced for this exact-codegen work.

## Compiler context and gaps

`probes/menu-difficulty-selection-context.json` retains VC9 RTM21022,
/O2 /Ob1 /MT /EHsc /GL, the real controller callback and55 supports with the same
two normal menu supports. Missing callee/data anchors keep it diagnostic. Full
public PDB contributions and all fields are replayed without cropping, normalized
branches, target instruction bytes or a declared private register convention.

The initial candidate is1044 bytes/299 instructions/1015 code, nine layout bytes
and all20 table bytes, with61 fields and967 full differences. All34 physical
calls/five RET4s and frame4 agree. Field count equality is not field-meaning
identity: native has one additional renderer reload, while the source has one
additional difficulty reload. The source caches the renderer in the normal
branch and keeps a difficulty value live longer. Menu/index/result-register
allocation and layout remain open. The required21-byte support has one SIB
byte difference: source ECX menu/EAX script versus native EAX menu/ECX script.
It is not accepted independently.

The71-support HUD/gameplay/debug experiment still does not supply every native
child-finder caller:42C8D0/42D9D0/42E6B0 have no maintained source or public PDB
contribution. Their default3154/3141/3179-byte boundaries remain unreviewed.
Linking ActorInstructions.cpp does not establish those owners. Existing source
covers the dialogue and page05/06/07 callers; genuine missing source remains a
next step, not a reason to force the shared helper ABI.

Full field replay establishes all15 inspected bodies in the71-support and
ternary-script probes are byte-identical to the retained55-support context.
The previous13 sibling bodies also match the character checkpoint in full.
A natural captured transition value eliminates the extra difficulty load but
gives1036 bytes/60 fields/977 differences; it changes only page05 and is discarded.
No synthetic register/volatile/ABI workaround is adopted.

All247 prior frozen inputs are unchanged; three new files extend the freeze to250.
No canonical exact unit is affected. The older53/52/48-support receipts remain
valid. `.analysis/verify-menu-difficulty-checkpoint.py` verifies target identity,
Ghidra spans and tables, source/receipt contracts, exhaustive fields and full
comparison/equivalence. Tracking:612 reviewed boundaries,494 mappings,
236 exact/31881 bytes. No exact credit or native-product closure is added.
