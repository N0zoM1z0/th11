# Continue/game-over follow-up menu

This is the last reconstruction draft before the2026-11-06 handoff cleanup.
It is source-present and cold-compiled, not byte-exact. Further codegen work was
stopped when the operator requested handoff preparation.

## Target and source scope

Direct hash-attested Ghidra/full-span/reference/table review plus independent PE
flow establishes42D9D0..42E647/3192, replacing the default3141-byte span:
872 instructions/3122 reachable code bytes,22 layout bytes and48 table bytes.
The state12..19 table has eight cells42E618..42E637; the nested selector table
has four cells42E638..42E647, with entries1/2 shared. All12 apparent foreign
interior entries are owned DATA. There are88 calls to26 targets,162 address
fields and15 RET4/GS paths. Four preceding CC and eight following CC separate
this owner from its neighbors.42E650 is a separate uninventoried entry.

`PauseContinueMenuView` contains the observed pause view followed by the file
pointer2DC. It does not claim an original class or inheritance relationship.
The complete C++ states preserve:

- 12:wait10, confirm, sound10, state13, root/related event2 and timer reset.
- 13:commit score, then either update the practice stage/difficulty score or call
  source-open42D850. The latter resets time and may enter record-name state18.
  Other paths create default-layer script62, enter14, set count4/wrap1, clamp0,
  send event3 and then7+current.
- 14:copy previous before navigation, signal before sound12, confirm child
  `5E+current`, then sound10/state19/reset/score commit. Choices1/3 repeat
  state/reset/sound;2 enters16 and loads25 replay slots. Cancel is independently
  tested after confirm, queues11 and selects1 if needed.
- 15:no action, including the target's table entry to the common epilogue.
- 16:replay-slot selection. Confirm enters17 and initializes the91-key editor;
  timestamp0C and stage68 are written through the captured replay-header
  pointer. Cancel enters14, resets, shows root, pops history, signals, directly
  destroys/deletes/nulls25 replays, writes14 again, resets again and queues11.
- 17:replay name editing. Save queues44 and calls436420 with filename/name/mode0,
  unlike pause's mode1, then reloads the slot and remembers the name. Delete at
  cursor0 returns before cancel; cancel at cursor0 returns to16.
- 18:record-name editing, distinct from replay save. Navigation depends on1F8.
  Key90 copies into the character/partner/difficulty/row record and remembered
  name, then recreates62 and returns to14. Nonzero1F8 bypasses editing on
  confirmation. Cancel finishes directly only for a negative selected row;
  otherwise it deletes one character with sound11 when the cursor is nonzero.
- 19:wait12, close, overwrite state with27, then request runtime mode14/10 for
  choice0 according to saveMode, mode4 for1/2, or10 for3.

The record-name address is statistics + `(character*3+partner)*68D4` +
`(difficulty*10+row)*1C` +1E. The practice score address instead uses
`(difficulty*6+stage)*8+5A4`. No new null/index/input guards are added.

## Cold diagnostic result

`probes/pause-continue-menu-context.json` retains VC9 RTM21022, /GL and63
supports, with the same two normal menu supports. The full public primary-TU
PDB contribution is3048 bytes:827 instructions/2979 code,21 layout and48 table.
All158 address fields are replayed;2770 full byte differences remain. Local
frame58/cookie54 agree with native, but physical codegen still differs:

| Operation | Native sites | Source sites |
| --- | ---: | ---: |
| GS check/RET4 paths |15|14|
| Default draw factory |2|3|
| Event3 helper |2|1|
| Selection event |5|4|

The two state13 factory expressions deliberately retain separate returned-ID
objects, with another factory expression at the record-name finish. Call-site
sharing, lifetime allocation and the four missing native address-field
occurrences remain to be investigated. Do not infer exactness from frame/table
agreement. This draft has not received a runtime-behavior claim.

Required support observations, without independent exact units:

| Operation | Native/source bytes | Full differences | Fields |
| --- | ---: | ---: | ---: |
| CommitPauseMenuScore42BA20 |33/31|21|4|
| ClosePauseContinueMenu42D7F0 |92/92|0|4|
| ShowPauseMenuAnimationTree456610 |64/64|0|2|
| CreateDefaultSecondaryDrawAnimationVmId455D80 |105/107|69|11|

The factory preserves the default layer and uses secondary-list append4557E0;
it must not substitute the primary-list publisher or add a layer20 store.
Close restores scale but does not restore pause's saved audio200. Score fields
are host-selection-state0/4, not an unrelated actor-score global.

All254 prior frozen inputs are unchanged; four new files extend to258. All19
previously inspected menu/support contributions remain byte-identical after
full field replay. Shared child finder remains76/native70/41 with script stack/
RET4 versus native EBX/plain RET. No canonical exact unit is affected or added.

Private recovery: `.analysis/verify-pause-continue-checkpoint.py`,
`pause-continue-checkpoint-proof.json`, `pause-continue-source-freeze.json`,
`pause-continue-menu-proof.json`, target reviews and build/audit scripts/logs.
These were retained by cleanup. `PreparePauseContinueRecords42D850/380` is
boundary-reviewed but source-open; its insertion callee42BA80 and writer436420/
1856 also remain open. The last missing large child-finder caller42E6B0 still
has its unreviewed default3179-byte boundary. Original class/TU, whole product,
runtime and portability are unproved.

## Record preparation and high-score insertion (2026-11-11)

The previously open owners at 42D850..42D9CB (380 bytes) and
42BA80..42BBEC (365 bytes) now have maintained source in
src/PauseContinueRecords.cpp and src/PauseRecordInsertion.cpp. Both
compile with the pinned VC9 compiler. Full normal-COFF diagnostics
produce 387 and 232 bytes respectively, versus the native 380 and 365.
Neither is exact. Their original translation-unit/LTCG ownership is
unproven.

The preparer has two distinct stage-7/save-mode tests, before **and**
after calling score insertion. The first temporarily sets the current
and remembered stage to 8 and the selected stage-data pointer to
g_MenuStageSelections + 0x40. The second does **not** unconditionally
reverse that change: only if stage is again 7 does it select the table
base and store both stage values as 7. A negative insertion rank marks
unknown1f8 and returns. Success sets up a 25-item selector and a 91-key
editor, copies the remembered name, steps the keyboard left unless that
name is eight spaces and finds the last nonspace cursor position.

Insertion compares scores as signed integers and puts the new score
before the first entry whose score is no greater. It moves the suffix
toward the end in **28-byte** records and returns -1 if the score does
not qualify. New entries store the current score, stage and shot type,
a 64-bit wall-clock timestamp, nine bytes of the default eight-space
name (including NUL), and the completion percentage calculated from
two doubles at offsets 24 and 2C of the live timing object. Original
target bytes at 497E50 encode double **100.0**; the formula is
100.0 minus the ratio of those times multiplied by 100.0. The shot
type and timing pointer are kept as descriptively named but still
nominally unknown globals at 4A573C and 4A8D80.

The preparer is called from the 3192-byte continuation owner at 42D9D0
and from 42E6B0. The insertion callee is also used by 43F802.
Future work: investigate /GL codegen and the 1856-byte replay writer
436420 without expanding to unrelated full-build validation.
