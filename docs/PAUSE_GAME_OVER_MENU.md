# Game-over menu states 20–26

The Japanese TH11 1.00a game-over owner at `0x0042E6B0..0x0042F337`
now has maintained C++ source in `src/PauseGameOverMenu.cpp`. It is a
substantial **3208-byte** function, not the original Ghidra default 3179-byte
span. It is independent of the 3192-byte continuation function at
`42D9D0`, although both use the observed pause-view storage. Original
class/TU ownership and runtime behavior remain unknown.

## Complete original extent

Direct hash-attested local Ghidra code and reference review plus independent
native PE Capstone CFG traversal establishes:

| Owned component | Bytes |
| --- | ---: |
| 875 reachable instructions | 3163 |
| Four alignment slots at 42E737/42E73E/42EC29/42F31B | 17 |
| Seven four-byte state jump-table entries at 42F31C..42F337 | 28 |
| **Complete function** | **3208** |

The seven dispatch targets (states 20–26) are `42E6DC`, `42E774`,
`42E8E7`, `42EAB6`, `42ED14`, `42EFC9` and
`42F2B2`. All are internal code destinations; alleged foreign
interior refs to jump cells are DATA, not additional function entries.
There are **88 calls to 26 targets**, **14 GS/RET4 exits**, no
unresolved outgoing branch and eight CC immediately after the table,
before the independent owner at `42F340`.

Recovery: `.analysis/pause-gameover-review-20261111.txt`,
`.analysis/pause-gameover-decompile-20261111.c`, and the independent
`.analysis/gameover-cfg-review-20261111.py` with its log.
These private target artifacts are not committed.

## Reconstructed state transitions

State 20 waits ten frames, accepts the 0x80001 confirm mask, queues sound10,
sets root/related event2 and resets the timer on entry to21. State21 commits
the score: practice mode updates the per-character, difficulty and stage
record and creates the distinct `0x68 - (saveMode != 0)` animation
before entering22. Normal mode prepares record insertion, resets the timer,
enters record naming state25 when required, or creates script62 and sets
state14 as the native cross-mode continuation path. The selector has three
choices, wrap1 and root choice event7.

State22 supports three options. Up/down changes trigger selection animation
before sound12. Confirm signals child script `0x5E+current` and
commits the current score; choices0/2 close via state26, while choice1
opens the 25-slot replay selection in state23. Cancel selects0 if necessary.

State23 waits ten frames, navigates replay slots, and initializes the
91-key editor when entering24. It stamps the active replay header at
+0xC and sets stage at +0x68; the latter is **not** difficulty+0x64.
Remembered name and trimmed cursor are restored. Cancel instead restores
the root and history, destroys/deletes and clears all25 replay slots,
and returns to the three-choice selector at22.

State24 edits the eight-character replay name in a 13-column keyboard:
navigation uses -13/+13 and horizontal wrapping, keys88/89 are space/delete
and key90 writes the selected replay with **mode0**, reloads its slot,
stores the remembered name and returns to23. State25 is the separate
high-score name editor, conditional on unknown1F8; it can insert a name
into the character/partner/difficulty record and return through the
game-over script67 before restoring state22.

State26 waits12 frames, closes the menu, sets state27 and selects
runtime mode4 for choices0/1 or writes mode10 for choice2. No new
null, bounds or input guards are introduced.

## Narrow VC9 diagnostics

Fixed VC9 RTM with `/TP /O2 /Ob1 /Gy /MT /EHsc` compiles the complete
maintained source to **3068 bytes**, with154 COFF relocations. A scoped
six-TU `/GL` context using `probes/pause-game-over-context.cpp`,
the menu support functions and record insertion produces a **3060-byte**
public primary-TU PDB contribution. PE/MAP/PDB ownership is confirmed.
The target and both candidates have **88 calls in 26 categories**; target
and linked source both have **14 RET4 paths**, with all seven state
table cells retained. Codegen and full bytes remain non-exact, and
the original production TU/helper register ABI is unknown. This is source
presence plus useful compiler evidence, **not canonical exact credit**.

Build logs/proofs are under `.analysis/gameover-*` and
`build/probe-ltcg/PauseGameOverContext/`. No unrelated canonical
cold replay was run. Next useful work: recover the private register
allocations and local temporary lifetimes, not synthetic instruction
bytes or cropped function extents.
