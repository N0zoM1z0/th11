# TH11 reconstruction handoff — 2026-11-28

## Start here

Work only against the original Japanese TH11 1.00a (`resources/th11.exe`,
SHA-256 `2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`).
The code-generation target is VC9 RTM build 21022. Keep `/GL` for established
LTCG owners and write natural C/C++ only: no inline assembly, padding, byte
stubs or cropped extents. Use the repository shell, `scripts/repo-python` and
direct Ghidra through `scripts/ghidra.py`. Use the commit prefix your operator
gives you (AGENTS.md says `gpt-6.1-sol:`; the last session used
`claude-opus-5.5:` without a trailer).

Generated totals are in [PROGRESS](PROGRESS.md). Every committed source change
passed cold replay of its affected exact units. Four older claims stay
withdrawn because they fail at HEAD: `secondary-actor-instruction-adapter`
(13 bytes instead of 11), `draw-fan-animation-vm`, `animation-distance-root`
and `reset-animation-vm` (249 vs 303 bytes, missing nine tween-duration zero
stores).

## Roadmap: breadth first, large owners later

The operator wants the small independent functions done first, so that
sources, types, contexts and callees exist broadly before the large owners.

1. **Functions with no source** in game code (0x401000..0x45FFFF). Work in
   groups of neighbouring functions; neighbours share a TU, so one group gets
   one source file and one small linked context. The `.rdata` tags
   `.\src\game\<file>.cpp:<line> <Type>` name the TUs and classes in link
   order (KNOWLEDGE_BASE, "The target names its source files and classes").
   The inventory misses unreferenced functions: 336 gaps with code are in
   `.analysis/claude-ledger-gaps.txt`.
2. **Near-exact sweep of existing sources**: give source-present functions
   without a unit a frontier unit in the smallest context that compiles them,
   compare once, and keep the ones that differ by a few bytes.
3. **Framework gaps found on the way**: missing callees that force data
   anchors, types still modeled as casts. Batch shared-header changes; each
   needs a wide replay.
4. **Later, large owners** (table below).

## Fast loop

A compile takes about 1 s; a small linked context about 5-15 s. Every Wine
launch is serialized by the shared Factory lock, so parallel jobs do not help.

1. Disassemble the target: `scripts/repo-python .analysis/claude-dis.py ADDR [ADDR...]`
   (ledger names inline); `.analysis/claude-xrefs.py` for references.
2. Write the source and a small probe in `probes/` whose exported entry calls
   the new functions and calls every shared callee with generic arguments
   (otherwise LTCG specializes their arguments). Link the real bodies of
   callees whose private convention matters (`ChainRegistration.cpp`,
   `ChainRemove.cpp`, `StageTimer.cpp`, `AnimationDocument.cpp` +
   `LogBuffer.cpp` + `probes/animation-document-load-context.cpp`, ...).
3. Append the units to `config/match-units.toml` with `frontier = true`,
   build one of them (`scripts/build-match-unit.py --unit NAME`) and compare:
   `scripts/repo-python .analysis/claude-symbol-diff.py BUILD_DIR SYMBOL ADDR SIZE --diff`.
4. For a match: review the extent (`.analysis/claude-extent-review.py
   ADDR:SIZE`), add missing ledger rows (`.analysis/claude-add-ledger-rows.py`),
   draft the field list (`.analysis/claude-draft-linkages.py UNIT`; a `?`
   inside `g_MainRuntime` at 0x4C3280 becomes that symbol with an addend),
   remove `frontier`, and register it with `.analysis/claude-add-coff-exact.py
   SPEC.json` (strict compare plus ledger rows).
5. `scripts/validate-tracking.py`, `scripts/replay-exact-units.py --source FILE`
   (or `--file PATH` after touching a shared file), `scripts/progress.py`,
   `git diff --check`, commit.

The `.analysis/claude-*.py` helpers are private and untracked; keep them.
`src/Item.cpp`, `src/Bullet.cpp`, `src/Hint.cpp` and `src/PauseSession.cpp`
are templates for manager lifetimes (KNOWLEDGE_BASE, "Manager lifetimes share
one template").

## Recent work (2026-11-28)

New TUs with exact functions: laser.cpp ([LASER](LASER.md)), item.cpp
([ITEM](ITEM.md)), bullet.cpp ([BULLET](BULLET.md)), hint.cpp
([HINT](HINT.md)), the pause session ([PAUSE_SESSION](PAUSE_SESSION.md)) and
front.cpp's two document helpers (`src/Front.cpp`). Speed work:
`scripts/winpath.py` replaces `winepath`, and redundant per-compile checks were
removed.

`AnimationVmId` now has an empty default constructor (needed by the pause
session constructor). It is an input of about 300 exact units, so any further
edit of `src/AnimationVmId.hpp` needs `--file src/AnimationVmId.hpp`.

## Near frontiers (small, concrete gaps)

| Unit | Gap |
| --- | --- |
| `hint-manager-constructor` (0x420E90) | Same size; only the order of the sixteen list-head stores differs |
| `update-pause-session` (0x42BF30) + its callback | Native passes the session in EAX and keeps it in EBX; ours uses EDI |
| `InitializeAsciiManager` (0x401080) | 115/124; native receives `self` in EAX and copies it to EDI, ours receives it in EDI ([ASCII_LIFECYCLE](ASCII_LIFECYCLE.md)) |
| `create-laser` (`CreateSecondaryHostObject`, 0x424DF0) | Native passes the request in EDI |
| `laser-line-acceleration` (0x425BD0) | 237 vs 235 bytes |
| `draw-item-manager` (0x423F40) | 334/336 bytes; `g_SecondaryActorFiles` in EAX instead of EDX |
| `convert-items-to-drops` (0x4240F0) | Callee conventions of `SpawnSecondaryPlayerDrop`/`CreatePositionedVmId` |

The register-choice gaps share one open question: how VC9 LTCG decides
whether a private parameter arrives in EAX or in a callee-saved register.
Adding visible callee bodies, member versus free function and probe variants
did not change it for the pause update.

## Next manager groups

- **front.cpp `FrontInf`** (global 0x4A8E98, 0x478 bytes, the object
  `TitleMenuThread` waits on): initializer 0x428B40 (two chains, priority 3
  and 53, then starts the loading thread 0x4289E0 through its `ThreadInf` at
  +0x10), destructor 0x428C30 (465 bytes, EH frame and GS cookie; releases
  slots 0/1/5/6, deletes the ascii manager and the 0x4A8EBC object, frees
  +0x430), create 0x428E10 (inline constructor: `ThreadInf`,
  `StageEmbeddedVm` at +0x30, memset), update/draw 0x428ED0/0x428F30.
- The bullet update/draw callbacks; game.cpp `GameInf` (0x4202D0 publishes
  0x4A8E88).
- `ChangeSecondaryAnimationScript` (0x456770) has the body of the exact
  `ChangeSecondaryDialogueSprite` once its id parameter is a
  `const AnimationVmId &`.

## Large owners (deferred)

| Owner | State | Notes |
| --- | --- | --- |
| `ExecuteAnimationVm` (`44B4B0`, 12,056 B) | Frontier | Flag-setter opcodes are bit fields (finding not applied, needs a 160-context replay); register, frame and shared tails remain. Full state in [ANIMATION_EXECUTION](ANIMATION_EXECUTION.md) |
| `ExecuteSecondaryActorInstruction` (`412E30`, 16,556 B) | 3,353 of 4,003 register-normalized | Case bodies in native order (switch bodies are laid out in lexical order); about 45 callees are still data anchors (`.analysis/claude-anchor-callees.py`) |
| `RunSecondaryScript` (`45B860`, 7,093 B) | 1,768 of 1,834 register-normalized | VM register, a tail merge, callee ABIs ([SECONDARY_SCRIPT](SECONDARY_SCRIPT.md)) |
| `RunSecondaryDialogue` (`41D380`, 6,980 B) | 1,805 of 1,848 register-normalized | Register choices, one goto block layout ([SECONDARY_DIALOGUE](SECONDARY_DIALOGUE.md)) |
| `UpdateMenuController` (`4399B0`, 1,224 B) | 214 aligned | Page owners' LTCG conventions differ ([MENU_CONTROLLER](MENU_CONTROLLER.md)) |
| `ExecuteStageScript` (`404470`, 2,736 B) | 603 aligned | Frame 0x90 vs 0x98, vector temporaries ([STAGE_SCRIPT](STAGE_SCRIPT.md)) |
| `UpdateStage` (`402AA0`, 1,677 B) | 395 aligned | Mode-2 mesh reload and color scheduling ([STAGE_UPDATE](STAGE_UPDATE.md)) |
| `CullStageObject` (`403950`, 1,643 B) | 492 aligned | Vector copies, two add orders ([STAGE_CULLING](STAGE_CULLING.md)) |
| `InitializeSecondaryReplay` (`4356E0`, 1,380 B) | 394 of 406 | Store scheduling |
| `InitializeSecondaryDebugHudAnimations` (`41A6D0`, 1,619 B) | 1,635 B | Frame 8 vs 0xC, register choices |
| `~SecondaryDebugHudOwned` (`41AFC0`, 729 B) | Frontier | Native caches both critical-section imports, `this` on the stack |
| `ResetStageMesh` (`40E6C0`, 325 B) | Frontier | Two x87 add operand orders (scheduler; operand swaps do not help) |
| `DrawStageLayer` (`404190`, 727 B) | 155 aligned | Stage-pointer register |

Before reusing an old frontier number, rebuild its context: stale images give
false leads.

## Storage

`build/` (about 540 MB) holds the exact families that receipts verify; do not
prune them. Cleanup on 2026-11-28 removed eight unused Wine prefix copies from
a parallel-build experiment (`.tools/wineprefix-msvc900-slot0..7`, 4.6 GB),
two unconfigured build directories, the old private
`build/probe-ltcg/Execution*` interpreter models (their findings and logs stay
in `.analysis/` and ANIMATION_EXECUTION.md) and my scratch objects and
backups in `.analysis/`. Retention rules: [ARTIFACTS](ARTIFACTS.md).
