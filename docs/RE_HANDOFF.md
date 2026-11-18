# TH11 reconstruction — agent handoff (2026-11-18)

## Resume safely

Target: original Japanese TH11 v1.00a, private resources/th11.exe,
SHA-256 2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813.
Pinned compiler: VC9 / Visual C++ 2008 RTM build 21022. Use direct
scripts/ghidra.py and the repository's VC9 linked-PE Oracle. Never substitute
TH11c, a patched executable, or another game's compiler profile.

The source reconstruction baseline is fe8833e (gpt-web: hoist shared HUD
teardown manager and critical imports), which was equal to origin/main before
this documentation-only handoff checkpoint. There are exactly two unstaged
source modifications and no untracked or conflicted paths:
- src/SecondaryDebugHudLifetime.cpp
- src/SecondaryDebugHudPrecleanup.cpp

These are active reconstruction work, not disposable build output. Read their
diffs before editing. Do not reset, restore or overwrite them.

The generated PROGRESS.md currently reports 2,175 tracked candidates,
813 source mappings and 475 canonical exact functions (52,289 exact code
bytes). These are repository-local ledger totals. The Factory read-only
inventory currently reports zero imported Oracle results and an
inventory-import-only warning; do not interpret the local exact ledger as
Factory acceptance or whole-game production closure.

## Current HUD state

The physical HUD owner is SecondaryDebugHudOwned at singleton 0x4A8D84,
with a native allocation of 0x4458 and 16 real embedded VMs: arrays of
9/2/4 plus the VM at +0x3F1C. Its timers are at +0x43C8 and +0x4424.
Do not regress to the obsolete 15-VM / 0x4450 layout.

| Native owner | Current source state | Evidence |
| --- | --- | --- |
| 0x41A2F0 constructor, 208 B | exact | secondary-debug-hud-constructor-frontier |
| 0x41B2A0 factory, 150 B | exact | secondary-debug-hud-factory-frontier |
| 0x419F80 dialogue VM release, 101 B | exact | secondary-debug-hud-dialogue-frontier |
| 0x41A3C0 three-chain registration, 339 B | exact | secondary-debug-hud-registration; 15 fields |
| 0x41A520 visual/resource initialization, 336 B | exact | secondary-debug-hud-visual-init; 21 fields |
| 0x41C5F0 front HUD draw, 105 B | exact | secondary-hud-front-draw; 5 fields |
| 0x41CFC0 front draw callback, 10 B | exact | secondary-hud-front-draw-callback; 1 REL32 |
| 0x41AF30 resource finalizer, 129 B | exact | secondary-debug-hud-finalization; 8 fields |
| 0x41B360 global HUD destruction, 28 B | exact | secondary-debug-hud-global-destroy; 3 fields |
| 0x41AD30 precleanup, 468 B | nonexact; current candidate 477 B / 130 instructions | dirty src/SecondaryDebugHudPrecleanup.cpp |
| 0x41AFC0 owning destructor, 729 B | nonexact; current candidate 741 B / 195 instructions | dirty src/SecondaryDebugHudLifetime.cpp |
| 0x41A6D0 animation initialization, 1,619 B | nonexact; current candidate 1,619 B / 403 instructions (native 402) | secondary-debug-hud-animation-init-frontier |

The two dirty source changes were freshly built with the pinned VC9 /GL
toolchain. The precleanup revision caches the renderer pointer, makes the
first two precleanup animation-ID accesses volatile, and uses a volatile
eight-count do/while loop. The destructor revision expands the three chain
unregister paths in the owner, retaining a register-held LeaveCriticalSection
pointer while loading the manager in each non-null path. These are current
experiments, not exact results; preserve them and continue from their diffs.

After those dirty edits, focused cold replay of the dependent 150-byte factory
and 28-byte global-destroy owners passed: both exact, zero byte differences.
No full cold replay was run.

## Next work

1. Continue the 729-byte destructor. Current 741/195 output has the right
   decoded instruction count but is 12 bytes too large. Compare chain-lock
   scheduling, renderer-list searches, the shared found-VM path and the
   compiler-generated 9/2/4 member-array destruction. Keep the real C++ owner.
2. Continue the 468-byte precleanup. Current output is 477/130 versus
   468/128. Revisit renderer snapshot lifetime, volatile ID stores and the
   counted loop without changing ownership semantics.
3. Continue the 1,619-byte animation initializer. Current output is the
   native size but has one extra decoded instruction; compare complete
   branches and relocation fields, not size alone.
4. Continue other large owners: ExecuteStageScript (0x404470, 2,736 B),
   StepStageVectorTween (0x405240, 883 B), CullStageObject (0x403950,
   1,643 B), DrawStageLayer (0x404190, 727 B), and the 1,777-byte
   InitializeSecondarySpellStage (0x40C650). Their detailed notes live in
   STAGE_SCRIPT.md, STAGE_TWEEN.md, STAGE_CULLING.md and
   SECONDARY_SPELL_STAGE.md.

## Focused workflow

Read AGENTS.md and RE_WORKFLOW.md, inspect the live Git diff, and verify the
target/toolchain identity once when resuming. Build only the affected unit:

    scripts/repo-python scripts/build-match-unit.py --unit secondary-debug-hud-destructor-frontier
    scripts/repo-python scripts/build-match-unit.py --unit secondary-debug-hud-precleanup-frontier

Cold-replay only exact units whose inputs changed and their real dependent
owners. The last dependency check used:

    scripts/repo-python scripts/replay-exact-units.py --unit secondary-debug-hud-global-destroy --unit secondary-debug-hud-factory-frontier

Then run validate-tracking.py --require-target, progress.py and git diff
--check. Do not run the full cold replay. Commit meaningful work with the
prefix gpt-web: and never claim exactness without a complete cold Oracle
replay, full PDB owner extent and all relocation fields.

## Storage and evidence

See ARTIFACTS.md. Canonical linked output directories, active frontier
outputs, the Japanese target, VC9 toolchain, game-specific Wine prefix,
Ghidra project and useful .analysis evidence are retained. The 2026-11-18
handoff cleanup removed only an unconfigured orphan link experiment and
three reproducible temporary snapshots; no source, exact receipt or active
frontier was removed.
