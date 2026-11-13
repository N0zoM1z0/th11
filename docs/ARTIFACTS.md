# Local artifacts and storage cleanup

Keep the private original `resources/th11.exe`, the pinned VC9 compiler and
its headers/libraries, the independent Wine prefix under `.tools/`, the TH11
Ghidra project, and useful analysis evidence. These are **not disposable
caches**, even when they occupy more disk than generated objects.

The source in `src/`, reusable `probes/`, match contracts and review ledgers
in `config/`, documentation, and repository scripts are the durable result.
`build/match/` and `build/match-linked/` hold **canonical exact-unit**
COFF/PE/MAP/PDB/object/receipt caches. Keep them for the next agent; their
contents can be recreated, but the receipt for a changed source must be
refreshed before relying on it. An old cached result alone is not acceptance.

`build/probe/`, `build/probe-coff/` and most of `build/probe-ltcg/` contain
ignored, regenerable exploratory objects and temporary diagnostic links.
Do not treat a historical `.analysis/` path as proof that its corresponding
old executable/PDB is still cached. Retain useful experimental recipes,
logs, target reviews and notes in the ignored `.analysis/` directory; these
are small relative to builds and can save expensive re-analysis.

## Latest cleanup — 2026-11-13

The cleanup inspected Git tracking before deleting anything; **no tracked
files** were in the removed build paths. All actions were restricted to
ignored diagnostic build output, without running `git clean -fdx` or touching
other private/work directories.

| Diagnostic cleanup | Before | After |
| --- | ---: | ---: |
| Entire `build/` | **293,240 KiB** (~286 MiB) | **142,600 KiB** (~139 MiB) |
| `build/probe-ltcg/` | 164,080 KiB | 15,356 KiB |
| Scratch `build/probe/` | 1,916 KiB | Removed |

**150,640 KiB (~147 MiB)** was reclaimed by deleting 364 of 369 old
exploratory link contexts and the scratch objects. At that first cleanup
checkpoint, five deliberately retained expensive diagnostic contexts remained
in `build/probe-ltcg/`:

- `PauseContinueMenuOwner` — 3,192-byte continuation menu with expensive
  63-support-TU context (~11 MiB);
- `LoaderCtorOrder20261112-explicit-before-clear` — browser construction;
- `BrowserState0-20261112-goto-layout` — 934-byte state dispatcher;
- `SecondaryReplayLoadRealHooks-real-all-hooks-20261112` — replay loader;
- `MenuReplayRecordsDraw-isolated-20261112` — replay record UI.

All canonical `build/match-linked/` (~124 MiB) and `build/match/` caches,
`.analysis/` evidence (~24 MiB), private original, `.tools/` (~716 MiB),
`ghidra-project/` (~14 MiB), source and Git history were preserved.
The local cleanup inventory is
`.analysis/handoff-storage-cleanup-20261113.txt` (ignored).

For context, an **earlier**, separate cleanup on 2026-11-12 reclaimed
831.9 MiB from 835 outdated diagnostic links and left `build/` at ~133 MiB
*at that time*. More probes were subsequently built, so those older sizes
and the old 330-unit inventory are **historical**, not today's checkpoint.

## How to rebuild after cleanup

```bash
# Build/verify only the exact context affected by a source change:
scripts/repo-python scripts/replay-exact-units.py --unit read-archived-file

# To examine disk usage again:
du -sh build/* .analysis
```

For exploratory work rerun its relevant `.analysis/*.py` recipe or compile
its tracked `probes/` context. **Do not replay all exact units merely because
old diagnostic caches were removed.** For current branch state, open tasks and
proof boundaries, see [RE_HANDOFF](RE_HANDOFF.md) and [ORACLES](ORACLES.md).

## Handoff cleanup — 2026-11-13 (second pass)

A second audit removed the remaining one-off experiment outputs from the
continuation-menu and score-register search. It inspected `git ls-files` first;
`build/probe-ltcg/` and `build/probes/` contained no tracked files. Only those
ignored build paths were touched. No canonical `build/match/` or
`build/match-linked/` output, target, toolchain, Ghidra data, source, probe
recipe or `.analysis/` evidence was deleted.

| Diagnostic cleanup | Before | After |
| --- | ---: | ---: |
| Entire `build/` | 318,428 KiB (~311 MiB) | 159,728 KiB (~156 MiB) |
| `build/probe-ltcg/` | 186,764 KiB (~182 MiB) | 30,092 KiB (~29 MiB) |
| Scratch `build/probes/` | 2,028 KiB | Removed |

**158,700 KiB (~155 MiB)** was reclaimed from `build/`: 156 exploratory
`build/probe-ltcg/` directories plus six loose diagnostic files were removed,
and the scratch `build/probes/` directory was deleted. The exact removal list
and per-context sizes are retained in
`.analysis/handoff-storage-cleanup-20261113b.txt`.

Nine probe contexts remain: the five historical contexts above plus
`PauseContinueSameTuScore-20261113`,
`PauseContinueAllRealCallersFinal-20261113`,
`PauseContinueDynamicModeSupport-20261113`, and
`PauseContinueDynamicFactorySupport-20261113`. The latter four preserve the
current best 63-support continuation baseline, the real-caller-expanded
comparison, and the two dynamic helper support objects needed to reproduce
those experiments. All other diagnostic objects can be regenerated from the
retained `.analysis/` recipes and tracked `probes/` sources.
