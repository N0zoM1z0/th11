# Local artifacts and cleanup

Maintained C++, probes, ledgers, tooling and current documentation are tracked.
Private targets, analysis state and generated binaries remain ignored. Start
continuation at [RE_HANDOFF.md](RE_HANDOFF.md).

| Path | Retention |
| --- | --- |
| `resources/th11.exe` | Keep the hash-bound original Japanese1.00a input |
| `.tools/` | Keep pinned compiler/SDK, Ghidra/JDK links, dependencies and Wine state |
| `ghidra-project/` | Keep the TH11 analysis database |
| `build/match/`, `build/match-linked/` | Keep every current canonical artifact and receipt declared by match-units |
| `build/toolchain-bootstrap/` | Keep historical compiler/linker smoke artifacts |
| `.analysis/` | Keep native reviews, useful compiler observations, cold logs and current source probes |
| `build/probe-ltcg/`, `build/probes/` | Experimental outputs; regenerate when pursuing a concrete byte gap |
| Python caches | Regenerable |

The2026-10-28 handoff cleanup removed825 files, about27.5 MiB: obsolete compiled
experiments, old Factory UX fixtures,39 retired behavior/mutation/helper scripts,
72 unused source snapshots, behavior reports, caches and35 unbound canonical
auxiliary objects/libraries. The cleanup helper itself was also retired.

All435 protected target/database/canonical files were hashed before and after
cleanup and remained identical. Current receipt-bound PE/MAP/PDB/object/anchor
files were preserved; installed tools were outside the deletion scope. The
private audit is `.analysis/handoff-cleanup-20261028.json`.

Current exact artifacts include the newly completed398-byte callback. Only it
and the affected91-byte status function were cold-replayed for this checkpoint.
Cleanup itself did not trigger a full-cohort replay.

The next unpromoted column experiment remains in `.analysis/column-sample-first.cpp`
with its log, full-owner diagnostic JSON and assembly. Its reusable compiler
driver is `.analysis/probe-effect-source.py`; it can regenerate the deleted
experimental binary using current canonical support objects. See
[MESH_EFFECTS.md](MESH_EFFECTS.md) for the concrete gap and command.

Earlier knowledge narratives moved to
[the historical archive](archive/KNOWLEDGE_BASE_2026-10-28.md). Old test counts and
private paths there are records, not requirements to recreate retired suites.
The concise [working knowledge](KNOWLEDGE_BASE.md) links active topic documents.

For later source changes, cold-replay affected exact units only, for example:

```bash
scripts/repo-python scripts/replay-exact-units.py --unit square-mesh-vm-callback --unit update-mesh-effect-spell-state
```
