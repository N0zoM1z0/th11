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

## Agent handoff cleanup — 2026-11-18

Removed four ignored, unreferenced temporary artifacts after checking
the current match-unit manifest, exact-unit ownership and maintained source
references:

| Removed path | Allocated size | Reason |
| --- | ---: | --- |
| build/match-linked/SecondaryDebugHudOwnerBodyExperiment | 1,176 KiB | orphan link experiment; no configured unit owns this output directory |
| .analysis/hud-destructor-unit-before-no-normal.toml | 680 KiB | one-off full-manifest snapshot; no maintained references |
| .analysis/hud-precleanup-unit-before-menu-support.toml | 680 KiB | one-off full-manifest snapshot; no maintained references |
| .analysis/functions.before-eax-local-noarg.csv | 1,148 KiB | exact historical copy of config/functions.csv at commit 852535c1; recoverable from Git |

The cleanup reclaimed **3,684 KiB (about 3.60 MiB)** of allocated space:
build/ fell from 531,148 to 529,972 KiB, and .analysis/ fell from
56,536 to 54,028 KiB.

No canonical exact output directory, active frontier, probe-ltcg diagnostic,
source, target, Ghidra project, toolchain, Wine prefix, replay receipt or
retained analysis comparison was removed. The HUD source changes present at
that earlier checkpoint were left untouched.

## Follow-up handoff cleanup — 2026-11-18

A second storage pass removed only ignored, unreferenced scratch copies from
the same day's compiler experiments:

| Removed category | Files | Apparent bytes | Allocated bytes |
| --- | ---: | ---: | ---: |
| Alternate C++ source snapshots and duplicate full match-unit manifests | **190** | **4,563,509** | **5,009,408** |

The allocated `.analysis/` usage fell from 68,380 KiB to 63,488 KiB,
reclaiming **4,892 KiB (about 4.78 MiB)**. The selected comparison scripts,
final diagnostic logs and the 1,670-byte spell-view method experiment patch
were retained. That patch records the tested but unhelpful member-method
experiment; the tracked source was restored because it did not recover the
native EDI callback convention.

No `build/match-linked/` directory was removed: every existing output
folder is referenced by a configured match unit, including nonexact active
frontiers. The build tree therefore remains at 535,916 KiB. The target,
toolchain, Wine prefix, Ghidra project, canonical receipts and tracked source
were not touched by storage pruning.

## Agent handoff cleanup — 2026-11-19

Two conservative storage passes removed only unreferenced scratch files and
byte-identical duplicate copies from the 2026-11-19 exploration. Candidate
paths were checked against tracked source/config/docs and the complete
`.analysis/` tree. For duplicate groups, at least one copy of each content hash
was retained, along with every referenced copy.

| Removed | Count | Measured allocated space |
| --- | ---: | ---: |
| Unreferenced C++/header snapshots and full manifest/profile backups in `.analysis/` | 86 files | 4,912 KiB |
| Unreferenced byte-identical duplicate files at `.analysis/` top level | 1,157 files | 9,012 KiB directory-usage reduction |
| Orphaned `build/match-linked/SecondarySpellVmLookup` output | 1 directory | 1,324 KiB |
| Generated `scripts/__pycache__/` | 1 directory | 212 KiB |
| **Total measured reduction** | | **15,460 KiB (about 15.1 MiB)** |

The 86 scratch snapshots occupied 4,855,865 apparent bytes (5,029,888
allocated bytes). The duplicate copies occupied 6,385,297 apparent bytes
(9,330,688 allocated bytes by file-block accounting). The exact removed path
list and both pass summaries are retained in
`.analysis/handoff-storage-cleanup-20261119.txt`.

| Directory | Before both passes | After | Saved |
| --- | ---: | ---: | ---: |
| `.analysis/` | 71,852 KiB | 57,928 KiB | 13,924 KiB |
| `build/` | 544,960 KiB | 543,636 KiB | 1,324 KiB |
| `scripts/__pycache__/` | 212 KiB | absent | 212 KiB |

No tracked source, proof receipt, canonical exact output, configured active
frontier, target, compiler/toolchain, Wine prefix or Ghidra database was
removed. The 218 remaining `build/match-linked/` directories are all
referenced by configured units; retained historical `probe-ltcg` contexts
remain available. No build or replay was run solely for this cleanup.
