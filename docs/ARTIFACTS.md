# Private artifacts and disk hygiene

The **durable reconstruction** lives in tracked C/C++ sources, probes,
`config/` ledgers/match contracts, scripts and documentation. Generated
objects and links are **caches**; an old cache or matching size alone is
not proof of exactness. For current tasks, see [RE_HANDOFF](RE_HANDOFF.md)
and for current totals, see generated [PROGRESS](PROGRESS.md).

## What must be preserved

| Area | Retention rule |
| --- | --- |
| `resources/th11.exe` | **Keep.** Private Japanese v1.00a target, SHA-256 pinned in `config/target.toml` |
| `.tools/msvc900/`, `.tools/wineprefix-msvc900/` | **Keep.** Pinned VC9 compiler, headers/libraries and game-isolated Wine prefix |
| `ghidra-project/` | **Keep.** Private TH11 analysis database |
| `src/`, `probes/`, `config/`, `scripts/`, `docs/` | **Keep.** Tracked source, reusable experiments, proof contracts and research |
| `build/match/` and canonical `build/match-linked/` output directories | **Keep.** Exact-unit build families and receipts; they can be regenerated, but useful for handoff |
| `.analysis/` original target reviews, recipes, meaningful comparisons and proof logs | **Keep.** Historical evidence not stored in Git; do not indiscriminately delete this tree |

A canonical `linked-pe` output is identified by a unit referenced in
`config/matches.csv` and its `output_dir` in
`config/match-units.toml`. Several units share one output directory;
**do not remove a directory if any of its units is canonical exact**.
The original target, Ghidra project, toolchain, all canonical output
families and the known costly menu diagnostics were preserved during
the cleanup below.

## Handoff cleanup — 2026-11-17

Removed only **147 ignored, regenerable paths** after checking the
live Git worktree, match manifest, canonical exact-unit list and
references from retained analysis scripts/documentation.

| Removed category | Entries | Reason |
| --- | ---: | --- |
| Abandoned/orphaned `build/match-linked/` directories and unrelated noncanonical frontier caches | 31 | No canonical exact unit owns these outputs; active HUD, stage script, tween and rendering frontiers were retained |
| One-off `build/probe-ltcg/` objects and unretained small probe directories | 88 | Ordinary compile probes, replaceable from tracked source; 14 expensive historical diagnostics retained |
| Unreferenced duplicate full `match-units.toml` snapshots in `.analysis/` | 28 | No retained script/document imports their names; canonical manifests and their Git history remain available |

All removed paths were ignored. No Git-tracked files or canonical exact
receipt directories were deleted.

Measured allocated disk usage (KiB, from `du -sk`):

| Directory | Before | After | Saved |
| --- | ---: | ---: | ---: |
| `build/` | 560,152 | 521,340 | 38,812 |
| `.analysis/` | 72,456 | 54,560 | 17,896 |
| **Combined** | **632,608** | **575,900** | **56,708** |

That is approximately **55.4 MiB of allocated disk space**
(56,561,171 apparent file bytes, or **53.9 MiB**). The ignored
`.analysis/handoff-storage-cleanup-20261117b.txt` records the exact
removed paths and their individual sizes. The TH11 target,
`.tools/`, Ghidra project and every tracked file were unchanged.
No code was compiled or replayed merely for this documentation/storage
cleanup.

The actively relevant diagnostic link caches retained at handoff
include `StageScriptFrontier`, `StageTween`, `StageCullingFrontier`,
`StageLayerFrontier`, `SecondaryDebugHudDestructorFrontier`,
`SecondaryDebugHudPrecleanupFrontier`,
`SecondaryDebugHudVisualInitFrontier`,
`SecondaryDebugHudRegistrationFrontier` and
`SecondaryDebugSessionDestructorFrontier`. The spell-stage scoped
frontiers were also retained. Previously retained expensive
pause/continue, replay, loader and game-over probe contexts were
not removed.

## Rebuilding a deleted diagnostic

A removed noncanonical frontier or compile probe is **not a lost
source change**. Rebuild only the owner relevant to the next edit:

```bash
scripts/repo-python scripts/build-match-unit.py --unit secondary-debug-hud-visual-init-frontier
```

To refresh a canonical exact result **when its inputs actually
change**, run a selected cold build and strict replay, for example:

```bash
scripts/repo-python scripts/replay-exact-units.py --unit secondary-debug-hud-constructor-frontier
```

Do not run full cold replay or `git clean -fdx` just to reclaim
space. Avoid deleting exact output directories, the Wine prefix,
the private target, Ghidra databases or `.analysis/` evidence.
Earlier dated cleanup inventories and experiments remain available
through Git history and retained ignored cleanup receipts, rather
than in a sequence of outdated disk-usage tables here.
