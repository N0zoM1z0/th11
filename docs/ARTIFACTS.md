# TH11 private artifacts and storage hygiene

The durable reconstruction is tracked in `src/`, `probes/`, `config/`,
`scripts/` and `docs/`. Start with [RE_HANDOFF](RE_HANDOFF.md) for active work
and [PROGRESS](PROGRESS.md) for generated ledger totals.

## Assets to retain

| Location | Purpose |
| --- | --- |
| `resources/th11.exe` | Original Japanese 1.00a target, pinned by `config/target.toml` |
| `.tools/` and the configured TH11 Wine prefix | Pinned VC9 RTM toolchain and isolated shared runtime |
| `ghidra-project/` and `.analysis/factory-runtime/` | Mutable TH11 database and active analyzer configuration |
| Configured exact `build/match/` and `build/match-linked/` families | Current complete objects, PE/MAP/PDB owners and receipts |
| Current private models, control images and their input objects | Unfinished interpreter/compiler work identified in the handoff |
| Useful `.analysis/` sources, drivers, focused traces and reviews | Reusable findings and reproducible experiments |

Check `output_dir`, source/input paths and exact ledger rows in
`config/match-units.toml` and `config/matches.csv` before removing build
products. Several units can share a family. Protect active jobs and private
models that reuse support objects from an older context. A historical path
reference alone is not a reason to keep obsolete generated output.

## Cleanup on 2026-11-26

The second cleanup removed **5,282 obsolete files / 300,048,206 bytes (286.15 MiB)**
(excluding the two exact caches rebuilt below):

| Removed products | MiB |
| --- | ---: |
| Superseded, unconfigured linked build experiments | 141.56 |
| Historical non-exact frontier build families | 5.65 |
| Stale, non-exact frontier object intermediates | 79.31 |
| Historical experimental object archives | 19.83 |
| Superseded broad compiler dumps, including the partial archive | 8.80 |
| Old Factory decompiler query cache | 7.88 |
| Historical `.analysis/` PE/MAP/PDB output | 17.45 |
| Generated debugger session files and old compiler-wrapper traces | 5.67 |

The path inventory is `.analysis/storage-prune-20261126.tsv`. At cleanup,
`build/` fell from about 802 to 571 MiB and `.analysis/` from 165 to 102 MiB;
active builds can change those figures. There was no `.build/` directory.
The target, toolchain, game-specific prefix, Ghidra database and running ReadF
consumer replay remain. Configured exact families were retained or recovered
as described below. Seven private
model/control families remain under `build/probe-ltcg/`, alongside configured
probe families; the current model's source and timer/support objects remain.

Five stale frontier contexts lost only their object intermediates; their
linked images, receipts and recipes remain: `update-stage-real-interpreter-animation-oy`,
`update-stage-real-interpreter-graph`, `update-stage-real-script-mesh-callees`,
`draw-secondary-player-callback` and `draw-unrotated-animation-vm`.
They require a cold rebuild before further comparison. The withdrawn draw
claims were not restored. Old non-exact linked caches for `animation-extent-magnitude`,
`draw-animation-vm-by-mode` and `build-rotated-animation-vm-quad` were also
removed; their source and recipes remain. Legacy top-level renderer output
was pruned from `build/match-linked/AnimationVmRendererOwner`; its two
configured exact child directories remain.

The first deletion pass missed configured children below that legacy parent
and removed two exact caches. `scale-animation-color-channel` and
`build-animation-vm-quad` were recovered through cold builds and complete
linked comparisons, both with zero differences. Recovery logs are
`.analysis/storage-recovery-{scale-animation-color-channel,build-animation-vm-quad}.log`.
Their rebuilt caches are excluded from the removed-file total above and marked
separately in the deletion inventory. Future cleanup must protect configured
descendants as well as directly configured directories.

The earlier same-day compression pass reclaimed 61.56 MiB. Its 547 object
archives and partial trace archive were subsequently deleted in this pass;
the old inventory `.analysis/storage-cleanup-20261126.tsv` describes that
intermediate state, not available backups. Together the two passes removed
**347.71 MiB** of file content. Historical notes may still name discarded
private outputs: retain their findings and rebuild the relevant experiment
when needed, rather than expecting every old cache to exist.

## Earlier records and rebuilding

November 22/23 inventories remain at `.analysis/storage-cleanup-20261122.txt`
and `.analysis/storage-cleanup-20261123.txt`; earlier documentation is in Git
(`4fe024e`, `9accb5a`). Inspect current usage with:

```bash
du -sh .analysis build .tools ghidra-project
```

Rebuild only the context needed for the task. Missing intermediates or stale
receipts require that context's cold build and affected exact comparisons;
cleanup does not require a full cohort or broad runtime suite. Never edit a
historical receipt to make a pruned or changed family appear current.

## Cleanup on 2026-11-28

Removed about 170 MiB: 23 unconfigured `build/probe-ltcg/Claude*` and
`claude-nextid` experiment directories, three private copies of the
`AnimationExecutionTweenContext` build and the source backups in `.analysis/`,
and 731 `.obj` intermediates (100 MiB) of non-exact frontier families.
Frontier `source.exe/.map/.pdb` files and receipts remain; rebuild a frontier
before relinking it. Not touched: every exact family,
`AnimationExecutionTweenContext` (its objects are reused by the private
`build/probe-ltcg/Execution*` models, which also remain), the target, the
toolchain, the Wine prefix and the Ghidra project. `build/` went from 687 to
530 MiB.

## Second cleanup on 2026-11-28 (handoff)

Removed about 4.6 GB: eight copies of the compiler Wine prefix
(`.tools/wineprefix-msvc900-slot0..7`) left by a parallel-build experiment
that was reverted (the shared Factory lock serializes all Wine work anyway),
the private interpreter models `build/probe-ltcg/Execution*` (about 10 MiB;
their findings and logs remain in `.analysis/` and ANIMATION_EXECUTION.md, so
the statement above that they remain is no longer true), two unconfigured
`build/match-linked` directories (`AnimationVmRendererOwner`,
`BindAnimationSpriteFresh`), and 33 private scratch objects, diffs and
backups in `.analysis/claude-*`. Files that ledgers or docs reference were
kept. Afterwards `.tools/` was 764 MiB, `build/` 539 MiB and `.analysis/`
105 MiB. The game prefix `.tools/wineprefix-msvc900`, every configured build
family, the target and the Ghidra project are untouched.
