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

## Handoff cleanup: 2026-11-20

The worktree was clean before deletion. Only regenerable ignored files were
removed. A reference scan protected experimental binary snapshots named by
maintained documentation or analysis scripts; **93** such files remain.

| Removed cache | Files | File-block allocation reclaimed |
| --- | ---: | ---: |
| Trial-only `.analysis/<experiment>-20261119/20/source.{exe,pdb,map}` with retained `source.cpp` and `comparison.log` | 315 | 5,394,432 bytes |
| `.analysis/sweep/*.obj` from the tracked-source sweep recipe | 186 | 1,347,584 bytes |
| Rotatable Ghidra runtime `application.log` | 1 | 4,075,520 bytes |
| `scripts/__pycache__/` Python bytecode | 8 | 135,168 bytes |
| **Total** | **510** | **10,952,704 bytes (~10.45 MiB)** |

Independent `du -sk` measurements: `.analysis/` fell from **75,696 KiB** to
**65,192 KiB**; `scripts/__pycache__/` dropped from **136 KiB** to absent.
`build/` stayed at **549,476 KiB**. The combined measured directory reduction
is **10,640 KiB (~10.39 MiB)**. Directory allocation and the sum of individual
file blocks need not agree exactly.

The exact removed-path inventory and protected-snapshot list are preserved
locally in ignored `.analysis/handoff-storage-cleanup-20261120.txt`. They do
not need to be committed. None of the tracked source, exact receipts, target,
compiler, Wine prefix or Ghidra project was removed. The unconfigured
`StageScriptCombinedFrontier` linked output remains because two retained
analysis scripts depend on it; the other noncanonical **active** frontiers were
also left intact. Experiment source and comparison text survive even when an
unreferenced binary cache is gone. Rebuild a pruned binary before using any
historical receipt as a current exactness claim.

## Older cleanups and regeneration

Earlier local handoffs reclaimed roughly 55.4 MiB on November 17, 3.6 MiB
and 4.8 MiB in two November 18 passes, and 15.1 MiB on November 19. Their
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
