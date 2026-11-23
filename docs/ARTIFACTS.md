# TH11 private artifacts and storage hygiene

The durable reconstruction is tracked in `src/`, `probes/`, `config/`,
`scripts/` and `docs/`. Ignore-only compiler output is a cache, not proof of
exactness. For current work and source status, start at [RE_HANDOFF](RE_HANDOFF.md)
and the generated [PROGRESS](PROGRESS.md).

## Keep these assets

| Location | Why it must remain |
| --- | --- |
| `resources/th11.exe` | Original Japanese 1.00a target; its SHA-256 is pinned in `config/target.toml` |
| `.tools/msvc900/`, `.tools/wineprefix-msvc900/` | VC9 RTM compiler and TH11-specific Wine environment |
| `ghidra-project/` | Private TH11 analysis database |
| Tracked source, probes, contracts, scripts and documents | Reproducible natural C/C++ work and research |
| Canonical exact `build/match/` and `build/match-linked/` families | Selected exact-unit receipts, COFF or linked PE/MAP/PDB owners and reviewed relocations |
| Useful `.analysis/` scripts, target reviews, comparison logs and candidate source snapshots | Reusable diagnostics not committed to Git |

A canonical linked family is referenced by an exact row in
`config/matches.csv` and the corresponding `output_dir` in
`config/match-units.toml`. Multiple exact units can share one output directory;
do not discard a directory before checking **all** its units. Preserve active
nonexact frontiers such as `StageScriptFrontier`, `StageCullingFrontier`,
`StageLayerFrontier`, `StageScriptInitializationFrontier`, the stage tween
family and the secondary-spell/HUD diagnostics. Avoid `git clean -fdx` or
blanket deletion of ignored trees.

## Storage cleanup: 2026-11-22

Only regenerable intermediates were removed; the worktree was clean and no
build was running. Configured output directories (exact and frontier) were
classified with the unit manifest; directory names were searched in tracked
files and `.analysis/` scripts.

| Removed | Files | Bytes |
| --- | ---: | ---: |
| `*.obj` under `build/match-linked/*/experiments/` (trial `.cpp`/`.inc` snapshots, `source.exe/map/pdb` kept) | 1,435 | 255,223,638 |
| `*.obj` in 15 unconfigured directories that docs or scripts name | 179 | 26,154,653 |
| `exe/pdb/map/obj` in 120 unconfigured directories named nowhere (text snapshots and logs kept) | 1,755 | 206,835,939 |
| **Total** | **3,369** | **488,214,230** |

`build/` fell from 1,176,368 KiB to 692,692 KiB. `git gc` packed 9,156
loose objects: `.git` fell from 176,116 KiB to 5,812 KiB. The full removed-path
list is in ignored `.analysis/storage-cleanup-20261122.txt`. Canonical exact
families, active frontiers, the target, compiler, Wine prefix, Ghidra project
and `.analysis/` were not touched; spot checks of exact linked and COFF units
still compare with zero differences.

A receipt goes stale whenever any header it hashed changes, even if codegen
does not. For example, about 200 receipts include `AnimationDocument.hpp`
through `StageResource.hpp`. A stale receipt makes the comparator refuse;
cold-replay the unit before relying on it. That is expected, not a lost proof.

## Storage cleanup: 2026-11-23

A conservative second pass removed only unconfigured and unreferenced linked
build output directories. The classifier compared every directory with all
current output_dir entries and searched tracked files plus retained .analysis
scripts/receipts before deletion. Configured exact/frontier outputs and
referenced diagnostic outputs were retained.

| Removed | Directories | Files | Bytes |
| --- | ---: | ---: | ---: |
| Unconfigured, unreferenced build/match-linked outputs | 37 | 281 | 4,853,458 |

The build tree fell from approximately 694 MiB to 689 MiB. The 63.28 MiB
AnimationVmRendererOwner output was retained because it is still referenced
by configuration and renderer documentation. build/match, build/probe-ltcg,
.analysis, the target executable, compiler/toolchain, Wine prefix and Ghidra
project were not touched. The exact removed directory list and byte count are
preserved in ignored .analysis/storage-cleanup-20261123.txt.

## Older cleanups and regeneration

Earlier local handoffs reclaimed roughly 55.4 MiB on November 17, 3.6 MiB
and 4.8 MiB in two November 18 passes, 15.1 MiB on November 19 and 10.4 MiB on
November 20 (details in the Git version of this file at `4fe024e`). Their
original path-level details remain recoverable in the Git version of this
file at `9accb5a`; local ignored inventories include
`.analysis/handoff-storage-cleanup-20261119.txt`.

Compile only the owner being resumed, for example:

```bash
scripts/repo-python scripts/build-match-unit.py --unit execute-stage-script-frontier
```

To refresh strict exactness **only if relevant inputs change**, use the unit's
selected cold replay (for example `--unit step-stage-vector-tween`), not the
entire cohort. `.analysis/tool-sweep-20261114.py` is the source of the deleted
COFF sweep products; Ghidra regenerates its own application log as needed.
