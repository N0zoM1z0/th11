# Local artifacts and cleanup

Maintained C++, probes, ledgers, tooling and current documentation are tracked.
Private targets, analysis state and generated binaries remain ignored. Start at
[RE_HANDOFF.md](RE_HANDOFF.md); counts come from [PROGRESS.md](PROGRESS.md).

| Path | Retention |
| --- | --- |
| `resources/th11.exe` | Keep the hash-bound original Japanese1.00a input |
| `.tools/` | Keep installed compiler/SDK, Ghidra/JDK links, dependencies and Wine state |
| `ghidra-project/` | Keep the TH11 analysis database |
| `build/match/`, `build/match-linked/` | Keep current canonical artifacts and receipts declared by match-units |
| `.analysis/` | Keep native/decompiler reviews, result records, current proof logs and cleanup audits; remove unused code variants/drivers |
| `build/probe-ltcg/`, `build/probes/` | Disposable experimental binaries; retain useful diagnostic JSON/ASM/MAP/logs and regenerate binaries when needed |
| Python bytecode caches | Regenerable |

## Latest cleanup:2026-10-30

Removed2047 ignored compiled/cache files and716 unused experiment-code files,
freeing about219.0 MiB allocated space. The latter are private Python drivers,
C/C++/header variants and generated trial anchor source. Three small current
analysis drivers remain; maintained `src`, `probes` and public tools are retained.
No code backups or compressed copies are created.

All documents, logs, diagnostic JSON/ASM/MAP, native reviews, decompiler records
and historical notes are retained. The binary cleanup verified1114 protected
target/database/canonical files; code cleanup verified3154 protected document,
record, target/database/canonical files. All hashes stayed identical during the
respective deletion operations; installed toolchain verification passes.

Current canonical files remain in `build/match`/`build/match-linked`. Trial
binaries below `build/probe-ltcg` are removed while result records remain.
Private audits are `.analysis/generated-trial-cleanup-20261030.json` and
`.analysis/unused-code-cleanup-audit.json`. The earlier cleanup audit and
[historical notes](archive/KNOWLEDGE_BASE_2026-10-30.md) are records, not build
requirements or old code dependencies.

## Reproduction and checks

Rebuild a concrete experiment from its retained `.analysis` driver/source when
pursuing its byte gap. A diagnostic JSON or old binary does not establish current
exactness; canonical acceptance requires current source/artifact receipts.

Cleanup itself requires checking the protected artifacts' identities. Source
changes require cold-replaying only new/changed exact units and actual receipt
dependencies. Neither operation calls for a full-cohort replay or reinstating
retired broad behavior suites. For example, a controller support change affects
the three linked controller-helper units; its unchanged standalone COFF callback
receipt can be checked directly.
