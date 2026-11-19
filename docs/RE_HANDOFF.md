# TH11 reconstruction handoff — 2026-11-19

## Target and checkpoint

Reconstruct only the original Japanese TH11 v1.00a executable:
`resources/th11.exe`, 688,128 bytes, SHA-256
`2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`.
The pinned compiler is Visual C++ 2008 RTM / VC9 build 21022. Preserve `/GL`
for owners whose native code generation is LTCG-dependent.

This handoff was prepared from a clean `main` worktree at `d832f18`
(`gpt-web: restore HUD renderer tail publication order`). The source checkpoint
contains the recent exact-function work and the two large debug-HUD frontiers
below. Documentation and storage cleanup are recorded separately in
[ARTIFACTS](ARTIFACTS.md).

## Current local inventory

`scripts/progress.py` generates [PROGRESS](PROGRESS.md). The current local
ledger reports:

- 2,175 tracked candidates; 1,610 reviewed boundaries; 565 still need review.
- 516 origins pending; 130 confirmed authored functions.
- 819 source mappings.
- 486 canonical exact functions covering 53,331 code bytes.
- 67 canonical exact functions with authored origin, covering 14,377 bytes.

These are repository-local ledger counts, not whole-game closure. The Factory
inspection still reports 2,175 inventory subjects, zero imported Oracle
results, and an `inventory-import-only` warning. Do not describe local exact
rows as Factory-accepted facts or as a complete native product.

## Recent strict exact owners

The recent focused work added or reconfirmed these complete natural-C++ owners:

- `LoadReplayStageData` at `0x436B60`: 441/441 bytes, 137 instructions,
  18 reviewed relocation fields; selected strict cold replay passed.
- `SecondaryAngleMath` at `0x40B900`: 34/34 bytes, 12 instructions and one
  `REL32` to `__CIatan2`; selected strict cold replay passed.
- `SecondaryUnaryMathB` at `0x419660`: 31/31 bytes, 10 instructions and one
  `REL32` to `__CIcos`; selected strict cold replay passed.
- `SecondaryAngleDifference40B830` and `SecondaryWrappedAngleDifference`:
  separate 101-byte owners, each with 34 instructions and three reviewed
  pi/tau fields; both passed selected strict replay.
- `SecondaryAngleDifference4287A0`: 101 bytes, 34 instructions and three
  reviewed pi/tau fields; strict replay passed.
- `SecondaryViewportPoint428850::IsOutside`: 93 bytes, four ordered rectangle
  tests and four reviewed float-constant fields; strict replay passed.
- `SetSecondaryIntegerPlayerPosition` at `0x435630`: 110/110 bytes,
  22 instructions and two reviewed address fields; strict replay passed.
- `SecondarySpellUpdateCallback` (10 bytes) and
  `SecondarySpellDrawCallback` (7 bytes) remain independently exact; their
  updater/draw owners have separate proof states.

See [SECONDARY_PAIR_MATH](SECONDARY_PAIR_MATH.md),
[SECONDARY_SCRIPT](SECONDARY_SCRIPT.md), [SECONDARY_REPLAY_LIFECYCLE](SECONDARY_REPLAY_LIFECYCLE.md)
and [SECONDARY_SPELL_STAGE](SECONDARY_SPELL_STAGE.md) for the individual
contracts and evidence.

## Active large-function frontiers

| Function | Native owner | Current candidate | Current state |
| --- | ---: | ---: | --- |
| `ExecuteStageScript` (`0x404470`) | 2,736 B | 2,696 B; 581/610 native instruction keys aligned | Nonexact; fog return-buffer adapter improved call-site layout; EH/funclet and remaining codegen differences open |
| `CullStageObject` (`0x403950`) | 1,643 B | 1,648 B; 328/504 aligned | Nonexact; native EAX camera input and local/x87 scheduling remain open |
| `InitializePrimaryStageScripts` (`0x405030`) | 249 B | 246 B in focused linked frontier | Nonexact; private-call ABI is recovered; three-byte extent and stack layout remain |
| `DrawStageLayer` (`0x404190`) | 727 B | 744 B; 112/191 aligned | Nonexact; stage/culling ABI and register allocation remain open |
| `StepStageVectorTween` (`0x405240`) | 883 B | 883 B; 4 reviewed linked fields | Exact linked-PE owner; zero code-byte and relocation differences |
| `InitializeSecondarySpellStage` (`0x40C650`) | 1,777 B | Diagnostic linked frontier | Nonexact; native caller uses EAX for the spell-name pointer plus two stack integers, while the candidate still uses three stack arguments |
| `UpdateSecondarySpellStageState` (`0x40C0A0`) | 624 B | 624 B; 161/167 aligned | Nonexact; signed `idiv` and follower-cursor initialization order restored; divisor spill, event-3 branch layout and Z x87 order remain |
| `InitializeSecondaryDebugHudAnimations` (`0x41A6D0`) | 1,619 B | 1,619 B; 385/402 aligned | Nonexact; renderer-tail publication order restored; return-buffer, stack reservation, zero-ID wrap and retained-animation scheduling remain |
| `SecondaryEclDebugOwner::~SecondaryEclDebugOwner` (`0x40D320`) | 462 B | 462 B; 126/127 aligned | Nonexact; one compiler-generated EH-state initializer differs |

Instruction-key alignment is a navigation aid, not exactness. Every row above
still needs complete byte/relocation equality before it can be called exact.
See the corresponding subsystem documents for the full native observations.

## Recommended next work

1. Continue `ExecuteStageScript` as the main Boss. Work from the current
   2,696-byte / 581-of-610 frontier; keep EH/funclet ownership separate from
   the public body and do not crop the owner to fit a comparison.
2. Continue `InitializePrimaryStageScripts` (249-byte owner, 246-byte focused
   linked candidate); the private call ABI is already recovered, so focus on
   natural C++ lifetime/loop scheduling and the three-byte extent gap.
3. Continue `CullStageObject`, then rebuild `DrawStageLayer` only when a shared
   culling/header change affects it. Preserve the native strict depth and
   unordered-coordinate behavior documented in [STAGE_CULLING](STAGE_CULLING.md).
4. Continue the 1,777-byte spell-stage initializer through its real caller,
   then the 624-byte updater. The callback leaves are already exact; do not
   transfer their exactness to the large updater.
5. Continue the HUD initializer and debug-session destructor. The current
   improvements are useful natural-C++ frontiers, not exact proofs.

## Focused workflow

Use only the affected unit and its direct exact dependencies:

    scripts/repo-python scripts/build-match-unit.py --unit execute-stage-script-frontier
    scripts/repo-python .analysis/compare-stage-script-frontier-20261118.py
    scripts/repo-python scripts/build-match-unit.py --unit cull-stage-object-frontier
    scripts/repo-python .analysis/compare-cull-stage-object-20261118.py
    scripts/repo-python scripts/validate-tracking.py --require-target
    scripts/repo-python scripts/progress.py
    git diff --check

For a source edit, compare the complete owner and all declared fields, then
cold-replay only changed canonical exact units and directly affected exact
dependencies. Do not run the full cold replay. Keep C++ natural: no inline
assembly, embedded instruction bytes, inert padding, fake returns or ABI
misdeclarations. Commit meaningful checkpoints with the `gpt-web: ` prefix.

## Retained workspace

Keep `resources/th11.exe`, `.tools/` and its TH11 Wine prefix,
`ghidra-project/`, canonical `build/match-linked/` output families, active
diagnostic frontiers, and the selected `.analysis/` comparisons/proof logs.
The handoff cleanup removed only unreferenced scratch snapshots and one
orphaned linked-output directory; details and measured savings are in
[ARTIFACTS](ARTIFACTS.md).
