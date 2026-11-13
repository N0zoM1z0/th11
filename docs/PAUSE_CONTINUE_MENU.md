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
  record preparer42D850. The latter resets time and may enter record-name state18.
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
Close restores scale but does not restore pause's saved audio200. The score
pair is host-selection-state0/4 (best/current), not the unrelated actor-score
global; its HUD mirror is the field at HUD+43E4.

All254 prior frozen inputs are unchanged; four new files extend to258. All19
previously inspected menu/support contributions remain byte-identical after
full field replay. Shared child finder remains76/native70/41 with script stack/
RET4 versus native EBX/plain RET. No canonical exact unit is affected or added.

Private recovery: `.analysis/verify-pause-continue-checkpoint.py`,
`pause-continue-checkpoint-proof.json`, `pause-continue-source-freeze.json`,
`pause-continue-menu-proof.json`, target reviews and build/audit scripts/logs.
These were retained by cleanup. The 380-byte record preparer, its 365-byte
insertion callee and the 1856-byte writer have since gained source; all remain
non-exact (see REPLAY_WRITING.md). The large child-finder caller42E6B0 still
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
Future work: investigate /GL codegen and the still non-exact 1856-byte replay
writer436420 (see REPLAY_WRITING.md) without unrelated full-build validation.

## Case-13 shared factory codegen experiment (2026-11-11)

Native 42DA94..42DB55 has a common animation factory call at 42DB3F.
The practice branch jumps to 42DB36 after its score update; the normal
branch prepares the record and either exits early or falls through.
Both then create script 62 before initializing state 14. Previously
the source called the factory separately in those branches, with a
third call in state 18. Moving the state-13 call after the branch
retains the same ordering and early return while matching the native
total of **two** factory call sites.

Focused VC9 /GL compilation and complete public-primary-TU diagnostic:

| Draft | Source bytes | Fields | Full differing bytes | Returns | Frame | Missing call-count parity |
| --- | ---: | ---: | ---: | ---: | --- | --- |
| Pre-change (Nov 6) | 3048 | 158 | 2770 | 14 | 58 | four call families |
| Shared call (pre-keyboard) | 3112 | 160 | 2797 | 15 | 54 | **none** |
| Shared call, two explicit local IDs (discarded) | 3016 | 157 | 2799 | 14 | 58 | three call families |
| Shared call + signed keyboard quotient (current) | **3172** | **160** | **2767** | **15** | **54** | **none** |
| Native | 3192 | 162 | 0 | 15 | 58 | none |

The target selects distinct hidden-result storage at ESP+20 or ESP+14
before the common call; the simple shared expression recovers call/return
structure but drops one stack slot. Explicit local return IDs bring the
frame back to 58 yet worsen call alignment (missing one each of the
cookie-check, projectile-clear and selection-signal calls). Neither
candidate is exact; the larger raw difference count after changing the
control flow is recorded rather than suppressed. The shared factory form is retained
because its call graph and returns match the native owner, and the
second experiment demonstrates why blindly adding locals is not enough.
The next meaningful issue is reproducing the two branch-dependent result
lifetimes without sacrificing the common call.

## Keyboard signed remainder (2026-11-11)

Native 3192-byte continuation owner has 10 IMUL and zero IDIV instructions.
Four keyboard remainder tests compiled to IDIV with direct `% 13`, whereas
the mathematically equivalent quotient/remainder form generates native-style
IMUL constants. The normal-COFF candidate moves from 2988 to 3048 bytes,
with the same 158 relocations; non-field byte matches from 134/2560 to
139/2560. In a narrow /GL context with the real sound helper and record
dependencies, size moves from 3032 to 3096 against native3192. That smaller
context has 85 calls and 14 returns versus native 88/15; the prior 63-support
3112-byte/15-return result belongs to pre-edit source.

The original **63-support** linked context has now also been rebuilt only for
this owner. Its complete primary-TU PDB contribution is **3172 bytes**
with 160 address fields, **88/88 native calls**, **15/15 RET4 paths** and
ten IMUL/zero IDIV. After rebinding all reviewed fields, the complete
original-owner comparison has **2767 differing bytes** against 2797 in the
3112-byte previous candidate. The native owner remains 3192 bytes with
162 fields; the current local stack frame is 0x54 against native 0x58.

Evidence: `.analysis/continue-keyboard-focused.log`,
`.analysis/continue-keyboard-full-context-20261111.log`, and
`.analysis/pause-continue-keyboard-20261111-proof.json`.
This is a non-exact local improvement without a full-cohort cold replay.


## Exact 92-byte close helper under real signal/VM producer (2026-11-12)

The native owner `ClosePauseContinueMenu` at `0x42D7F0..0x42D84B` is now
**92/92-byte, 22/22-instruction strict exact**. It sets menu state 19,
advances the timer to frame -1, signals both backdrop and root animations,
and restores the saved animation time scale, but it does not restore audio.
The maintained natural C++ is in `src/PauseContinueMenuSupport.cpp`.

Small standalone callback contexts over-specialized `SignalSecondaryAnimation`
(94/92, two extra stack pushes); the genuine `SecondaryActorAnimation.cpp`
signal and `StageMesh.cpp` VM finder implementations restore the original
private ESI menu receiver. An independent dynamic menu/VM-ID caller is
tracked as `probes/pause-continue-support-context.cpp`, so the complete
producer needs only these three support translation units, not the full
63-support continuation menu. The 92-byte body has two calls, one plain
return and **four independently reviewed address fields**: DIR32 sites at
`+0x25/+0x57` to `g_AnimationTimeScale` (`0x4A7948`), and REL32 calls at
`+0x40/+0x4B` to the independently exact `SignalSecondaryAnimation`
(`0x4070E0`). The unique source TU has full PE/MAP/PDB ownership; all
original bytes match after binding the four fields, with **zero differences**.
Acceptance unit: `close-pause-continue-menu` in `config/match-units.toml`,
receipt `.analysis/close-pause-continue-menu-strict-20261112.log`.

The neighboring 64-byte `ShowPauseMenuAnimationTree` has a 64/64 candidate
in the full continuation-game producer, but with this smaller set of real
callees the compiler selects a 52-byte specialized function. It is still
**non-exact**, and remains independent of this accepted 92-byte owner.
The 3192-byte continuation state machine is also **non-exact**; experiments
with branch-specific animation factory result variables restored its stack
frame but reduced native calls from 88 to 85, and were not adopted.


## Exact 64-byte animation-tree visibility helper (2026-11-12)

`ShowPauseMenuAnimationTree` at `0x456610..0x45664F` is now independently
**64/64-byte, 20/20-instruction strict exact** under pinned VC9 `/GL`.
The owner obtains a VM by its ID, marks root bit 2, and traverses the
related VM list when no special bound marker is present. The original
six-byte loop-alignment `LEA EBX,[EBX]` remains naturally emitted; no
machine-code patch, artificial padding or fake register shim was used.

The 52-byte candidate from only the concrete `StageMesh.cpp` VM finder
wrongly specialized the global renderer pointer away. Linking the
independent generic renderer/VM-ID caller from
`probes/stage-mesh-context.cpp` with `StageMesh.cpp`, the actual
`SecondaryActorAnimation.cpp` signal source, and tracked
`probes/pause-continue-support-context.cpp` restores the original
private EAX ID reference and EDX renderer register conventions.
The primary `PauseContinueMenuSupport.cpp` PE/MAP/PDB contribution
then contains the complete target with **zero code-byte differences**.
Both original relocation fields were independently bound: DIR32 `+0x04`
to `g_RenderBatch` `0x4C3268`, and REL32 `+0x0A` to the independently
accepted `FindAnimationVmById` at `0x4561E0`. The selected-only cold
Oracle `show-pause-menu-animation-tree` accepted the full 64 bytes;
receipt `.analysis/show-pause-menu-animation-tree-strict-20261112.log`.
This exact leaf is separate from the 92-byte close helper and the
still-non-exact 3192-byte continuation owner.


## 33-byte pause score commit field view (2026-11-13)

Direct original-PE review of `CommitPauseMenuScore` at `0x42BA20` confirms
this exact sequence: load the HUD pointer from `0x4A8D84`, load the current
score from `0x4A56E4`, store it at HUD `+0x43E4`, copy the score into EAX,
compare against the best score at `0x4A56E0`, and update the best score only
when the current score is greater. The native owner is 33 bytes with four
address fields and four call sites across the continuation and game-over menus.

`PauseContinueMenuSupport.cpp` now represents the HUD destination with a
local `PauseHudScoreView` and a compile-time assertion for `score43e4` at
`+0x43E4`; this replaces the untyped pointer arithmetic without changing the
observed layout. The current VC9 `/GL` diagnostic remains **31/33 bytes** with
seven non-address differences and four decoded fields. It loads the current
score before the HUD pointer, unlike the native pointer-first sequence. A
volatile HUD-pointer experiment restores the first three native instructions
but reloads the current score after the store (36 bytes, six non-address
differences); neither candidate is exact.

The source change was followed by selected cold replays of all exact owners in
the same source file: `close-pause-continue-menu` (92/92),
`show-pause-menu-animation-tree` (64/64), and
`create-default-secondary-draw-animation-vm-id` (105/105). All three retained
unique public primary-TU ownership, exhaustive reviewed fields and zero byte
differences. The score commit itself remains source-present and non-exact.

## Branch-specific hidden-result storage experiments (2026-11-13)

The native state-13 factory call at `0x42DB3F` is shared, but the practice and
normal branches select different hidden-result storage before reaching it:
`ESP+0x20` and `ESP+0x14`, respectively. The current single-result source keeps
all **88/88 calls** and **15/15 RET4 paths**, but its best scoped 63-support
link remains **3172/3192 bytes** with a `0x54` frame rather than the native
`0x58` frame.

Two branch-specific `AnimationVmId` locals and an equivalent two-element
array both restore the `0x58` frame, but VC9 produces the same **3076/3192-byte**
owner with only **85/88 calls** and **14/15 RET4 paths**. The missing call
parity includes one `ClearSecondaryProjectileHit` and one
`SignalSecondaryAnimationEvent`; the missing return also loses one security
cookie check. These are worse complete-owner candidates and were not adopted.
The maintained shared-call source remains the best call/return-preserving
version. The next step is to recover the two native result lifetimes without
sacrificing those call sites and epilogues.
