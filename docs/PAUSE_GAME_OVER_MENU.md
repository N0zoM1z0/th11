# TH11 game-over pause/replay menu (0x0042E6B0)

The original Japanese TH11v1.00a game-over menu is maintained in
`src/PauseGameOverMenu.cpp` under the descriptive name
`UpdatePauseGameOverMenu`. Its owner and nominal class remain unproven.
This function is **source-present, non-exact**. It is distinct from the
normal pause owner at 42C8D0/3212 and continue owner at 42D9D0/3192.

## Complete native boundary

An original-image-hash-attested Ghidra review and independent PE/Capstone
CFG walk covered the entire owner `0x42E6B0..0x42F337` (3208 bytes):

| Measure | Count |
| --- | ---: |
| Reachable instructions | 875 |
| Instruction bytes | 3163 |
| Alignment/layout bytes | 17 |
| Switch table bytes | 28 |
| Calls, distinct native targets | 88, 26 |
| Returns | 14 RET4 |
| Foreign interior entries/tail transfers | 0 |

The original Ghidra default span ended at `0x42F31A`, before one alignment
NOP at `0x42F31B` and the seven four-byte switch entries at
`0x42F31C..0x42F337`. The table maps menu states 20..26 to entries
`42E6DC, 42E774, 42E8E7, 42EAB6, 42ED14, 42EFC9, 42F2B2`.
There are 7+2 alignment bytes near 42E737, 7 bytes near 42EC29 and the
final one-byte NOP. CC bytes begin immediately after the table.

## Maintained state-machine behavior

- State 20 waits for a confirmed game-over screen, plays sound 10,
  sets state 21, propagates animation event 2 and resets its timer
- State 21 commits the current score, updates practice records or
  prepares a high-score name, distinguishes the game-over save script
  0x68/0x67 from normal continuation and initializes three menu choices
- State 22 navigates the three choices, dispatches confirm/cancel,
  requests replay slot selection when choice 1 is confirmed, and handles
  the delayed closing path for choices 0/2
- State 23 selects among 25 replay files; confirmation initializes the
  91-key name editor and timestamps the replay header at offset 0x0C,
  while cancellation disposes all 25 replay entries
- State 24 edits an eight-character replay name and writes a selected
  `th11_%.2d.rpy` replay through the maintained writer at 436420
- State 25 edits the inserted score record's eight-character name,
  persists it to the per-character/per-difficulty score array and
  remembered name, and returns to the game-over selector
- State 26 waits twelve frames then restores runtime state or requests
  runtime mode 10, depending on the selected exit path

The native code uses the same PauseMenu storage offsets as existing owners:
state04, timer10, selector38, keyboard110, root1E8, cursor1F0,
save-mode1F4, replay array204 and name2CC. The code preserves
individual input tests, animation ID side effects and early returns.

## Focused compiler diagnostics

The **initial** pinned VC9 normal-COFF `/O2 /Ob1 /Gy /MT /EHsc`
source generated 3068 bytes with 154 relocation fields. An earlier,
non-runnable six-TU VC9 `/GL` linked context compiled this owner,
score/record preparation, pause/continue support and the game-over probe.
That historical primary PDB contribution was **3060 bytes** against
native **3208 bytes**; later state-21 and keyboard-codegen changes
superseded this baseline (see the measurements below). Independent instruction inventory confirms exact
**88/88 call-site counts**, **26/26 callee identities**, **14/14 RET4
sites**, the 7-entry switch-table shape and a 0x58 stack frame.

This is **not an exact match**: synthetic helper ABIs, register allocation,
instruction ordering, layout offsets and unbound original data still differ.
A matching call graph is not a zero-byte-difference proof, and this
local linked context is not an authentic whole-game build.

Private evidence: `.analysis/gameover-cfg-review-20261111.py`,
`.analysis/gameover-cfg-review-20261111.log`,
`.analysis/pause-gameover-review-20261111.txt`,
`.analysis/pause-gameover-decompile-20261111.c`,
`.analysis/pause-game-over-ltcg-build.log`,
`.analysis/gameover-coff-compare-20261111.json` and
`build/probe-ltcg/PauseGameOverContext/`.

### State 21 branch-local selection reset (2026-11-11)

The original states 21 practice and regular routes each set the three-option
selection count, wrap flag, and initial cursor before their common root event.
Keeping these operations *inside each branch*, rather than hoisting them
after the join, produces more faithful VC9 codegen without changing the
state machine or call targets.

The complete normal-COFF body improves **3068 -> 3116 bytes**, with
relocation-masked diagnostic comparable matches **171/2592 -> 254/2592**
and unchanged 154 relocations. A focused `/GL` link with the real
`MeshEffects.cpp` sound helper and existing score/record support produces
**3180/3208 bytes**, versus 3116 with that same support context before
the source change and 3060 in the earlier six-TU context. It retains
**88/88 call sites, 14/14 returns and 7/7 owned dispatch entries**.
A wider `/GL` context adding timer and selection helpers instead reduced
the function and removed call/return sites; it was not adopted.

A second change makes the two 13-column keyboard navigation tests
compute the signed remainder as `current - (current / 13) * 13`.
This is equivalent to `current % 13`, but VC9 lowers the direct modulo
tests to IDIV. The quotient form restores the original multiply sequence:
native uses **10 IMUL and zero IDIV**, and the focused linked candidate
now also uses **10 IMUL and zero IDIV** (rather than 2 IMUL and four IDIV).
The full linked owner becomes **3228/3208 bytes**, with **88 calls,
14 RET4 and seven table entries** still intact. The updated normal-COFF
body is 3172 bytes with 154 relocation fields.

The remaining **20-byte size difference** still requires complete code and
data-field reconciliation; size proximity is not exactness.
Local diagnostics: `.analysis/gameover-state21-source-linked.log`,
`.analysis/gameover-main-after-branches-coff-compare.json`, and
`build/probe-ltcg/GameOverSoundOnly/`. Next focus: case 21's private
factory result/lifetime and case 25's keyboard/record-name path.

Next focus: preserve the full state20..26 graph while reducing register
allocation/stack/result-identity differences against the 3208-byte owner.
Do not run unrelated cold replay cohorts to compare this function.

## Latest focused boss and shared-selection ABI checkpoint (2026-11-13)

The best **retained** focused VC9 `/GL` candidate is
`build/probe-ltcg/PauseGameOverFpoAdapters-20261113/`. Its primary PDB
contribution is **3212 bytes** against the native **3208-byte** extent. The
candidate has **154/154 decoded address fields**, **88 calls**, **14 RET4
exits**, the seven-entry state dispatch table and **2040 non-address byte
differences** after masking those fields. Its initial frame shape is now
aligned with the target: `sub esp,58h`, the GS cookie at ESP+54h, and EBP
holding the menu receiver rather than a conventional EBP frame. The first
48 bytes contain **41 equal raw bytes**; the seven differing bytes are the
cookie/branch/table address operands. This is a substantial layout
improvement, not exactness; the remaining body has over two thousand
non-address differences.

A separate state-26-decrement source variant measured **3212 bytes and 2039
non-address differences** with the same 154 fields, 88 calls and 14 RET4
exits. Its output PE/MAP/PDB was removed by a later relink attempt that hit a
Wine linker page fault while opening the cached Archive support object. The
source snapshot and focused driver remain at
`.analysis/PauseGameOverMenu-state26-decrement-20261113.cpp`,
`.analysis/PauseGameOverMenu-abi-adapters-20261113.cpp` and
`.analysis/probe-pause-gameover-state26-decrement-20261113.py`. Treat the
2039 figure as a historical diagnostic until the link is reproduced. The
retained 3212/2040 snapshot is the current handoff reference.

The shared `MoveSecondaryDebugSelection` helper at `0x459160` is another
important blocker. Its native machine ABI is EAX selection pointer, one
stack-passed signed delta, EAX result and `RET 4`; the existing C++
`__stdcall` declaration does not describe that register-level interface.
A temporary naked implementation reproduced **121/121 bytes** in normal
COFF with no relocations, but only as a structural comparison with no
accepted canonical receipt. A Game Over-only thiscall facade worsened the
boss to **3236/3208 bytes** and **2116 non-address differences**, so those
production-source changes were reverted. The existing production helper and
its callers remain unchanged. Do not promote the 121-byte experiment or
change the shared declaration globally without reviewing all callers.

The production source tree remains at the committed `e7e1f13` baseline.
No Game Over boss candidate has a canonical exact receipt or runtime-
equivalence claim. Continue by improving the complete state20..26 owner and
its private helper ABIs, using only the focused context; do not run a full
cold replay for this diagnostic work.

## Neighboring previously uninventoried comparison helpers

The independent original PE also contains two complete fourteen-byte
comparison bodies at `42F340..42F34D` and `42F350..42F35D`,
separated by two CC bytes. Each takes an ECX object pointer and one
stack integer, compares that value with the object's first integer, returns
the normalized integer predicate (`!=` or signed `<`) and
ends in RET4. These bodies begin **after** the game-over function's table,
not within its 3208-byte extent.

Ghidra has no function definitions or incoming references for either
candidate. The original source/class identity remains unknown. A natural
C++ member-view draft in `src/PauseSelectionComparisons.cpp` compiles
with the pinned VC9 into complete 14-byte non-COMDAT function extents
with no relocations; independent full comparisons reproduce **12/14 bytes**
for each. The two remaining differences are ECX-vs-EDX register choices
in the load and compare, not address fields. Neither is exact and no
vendor/game attribution is claimed. The adjacent 42F360/42F370 wrappers
are still unclassified.

**2026-11-14: the EDX form is reproduced.** Defining both members inline in
the class and calling them only from code where LTCG inlines every call leaves
dead out-of-line COMDAT copies (the link keeps unreferenced COMDATs). Those
copies are byte-identical to native: `8B11 33C0 3B542404 0F95C0/0F9CC0
C20400`. A non-inline or address-taken definition uses ECX and stays 12/14.
So these islands are leftover copies of inline helpers whose calls were all
inlined, most likely in the game-over menu TU that precedes them. Probe:
`.analysis/PauseSelectionComparisons-inlined-20261114.cpp`.

To make them canonical exact units, the inline class must live in the header
used by the real owner, and the owner's source must call the helpers where
native has inlined comparisons; the copies would then be emitted from that
primary TU. Do not add an invented caller to force emission.
