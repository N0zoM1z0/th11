# TH11 private artifacts and storage hygiene

The durable reconstruction is tracked in `src/`, `probes/`, `config/`,
`scripts/` and `docs/`. Compiler output supports an exact claim only with its
current receipt and complete comparison. Start with [RE_HANDOFF](RE_HANDOFF.md)
for active work and [PROGRESS](PROGRESS.md) for generated ledger totals.

## Assets to retain

| Location | Purpose |
| --- | --- |
| `resources/th11.exe` | Original Japanese 1.00a target, pinned by `config/target.toml` |
| `.tools/` and the configured TH11 Wine prefix | Pinned VC9 RTM toolchain and isolated shared-runtime configuration |
| `ghidra-project/` | Mutable private TH11 analysis database |
| Configured `build/match/` and `build/match-linked/` families | Exact/frontier objects, PE/MAP/PDB owners and receipts |
| Current private probe inputs and linked images | Compiler comparisons and unfinished reconstruction work |
| Useful `.analysis/` source snapshots, scripts, reviews and logs | Reusable target/compiler evidence |

Check every `output_dir` in `config/match-units.toml` before removing build
products. Several units can share a family. Preserve the active replay's
outputs and inputs; its current process and log are in the handoff.

## Cleanup on 2026-11-26

Old experimental object files under `.analysis/` were archived with gzip.
Their source snapshots, linked PE/MAP/PDB files, logs and historical receipts
remain. Each archive was decompressed and checked against the original SHA-256
before its uncompressed object was removed. One identical duplicate of the
compiler phase dump was removed after comparing both complete files.

| Operation | Files | Original bytes | Retained bytes |
| --- | ---: | ---: | ---: |
| Archive historical experimental `.obj` files | 547 | 72,506,778 | 20,792,811 |
| Archive the partial first compiler-phase dump | 1 | 5,315,121 | 854,315 |
| Remove identical `execution-phases.pre-late-timeout.txt`; keep `execution-phases.txt` | 1 | 8,375,515 | 0 |
| **Total** | **549** | **86,197,414** | **21,647,126** |

This reclaimed **64,550,288 bytes (61.56 MiB)**. The path-level inventory is
`.analysis/storage-cleanup-20261126.tsv`. Configured build families and current
probe inputs were retained while the ReadF consumer replay continued.

Restore an old experimental object only when it is needed:

```bash
gzip -dk .analysis/EXPERIMENT/build/source.ltcg.obj.gz
```

Use the actual path from the inventory. Restoring preserves the original bytes;
the usual receipt checks still determine whether its inputs are current. Read
an archived text dump with `gzip -dc PATH.txt.gz`.

## Earlier cleanup records

The November 22 and 23 path inventories remain at
`.analysis/storage-cleanup-20261122.txt` and
`.analysis/storage-cleanup-20261123.txt`. Older cleanup details are retained in
Git (`4fe024e`, `9accb5a`). The previously reported 689 MiB build size was a
November 23 measurement. Inspect current usage with:

```bash
du -sh .analysis build .tools ghidra-project
```

Rebuild only the context needed for the task. A stale receipt requires that
context's cold build and affected exact comparisons; artifact cleanup does not
require a full cohort or broad runtime suite.
