# Local artifacts and cleanup

The repository tracks maintained C++, probes, ledgers, tooling and documentation.
Game inputs, analysis state and generated binaries stay private under ignored
paths. [RE_HANDOFF.md](RE_HANDOFF.md) is the current recovery entry; historical
checkpoint notes are under `docs/archive/`.

| Path | Purpose | Retention |
| --- | --- | --- |
| `resources/th11.exe` | Hash-bound Japanese1.00a target | Keep; never commit |
| `.tools/` | Pinned compiler/SDK, dependencies, Wine prefix, Ghidra/JDK links | Keep; verify with tool declarations/checks |
| `ghidra-project/` | Mutable private Ghidra database | Keep; direct read-only queries by default |
| `build/match/`, `build/match-linked/` | Current canonical artifacts and receipts | Keep; rebuild only affected units when inputs change |
| `build/probe-ltcg/` | Noncanonical experiment binaries | Keep current diagnostic contexts; obsolete variants are regenerable |
| `build/probes/` | COFF fixtures used by private check scripts | Keep while those scripts reference them |
| `.analysis/` | Review dumps, probe/check scripts, assembly, logs and diagnostic reports | Keep useful evidence; never treat masked reports as exact proofs |
| Python caches | Import acceleration | Regenerable |

The2026-10-24 cleanup removed superseded compiled probe variants and Python
caches. All canonical artifacts, the target and Ghidra files were hashed before
and after deletion;384 protected files were unchanged. The private audit at
`.analysis/handoff-cleanup-20261024.json` lists removed paths, sizes and protected
hashes. One-off private ledger mutation scripts were also discarded after their
results were reconciled into maintained CSV/TOML files.

These current linked diagnostic contexts were retained for existing check scripts:

- `archive-decompress-final`
- `device-back-buffers`
- `formatted-text`
- `outlined-outlinedtexturetext`
- `stage-resource`
- `startup-path`
- `surface-composite`
- `text-drawing-textdrawing`
- `vm-bindings`
- `window-initialization`

Older reports and failed-context source probes remain available even when their
compiled variants have been removed. Recreate a specific experiment with its
private probe script only when new evidence warrants it; the producer rebuilds
its own output directory. Read the topic document first to avoid repeating a
known failed configuration.

For accepted units, use the public focused entry point, for example:

```bash
scripts/repo-python scripts/replay-exact-units.py --unit release-vm-binding-block
```

A cleanup is not a reason to replay the full cohort. Keep source/header/context
changes separate from deletion of obsolete diagnostics, and preserve the current
receipt-bound family when cleaning. The retained executables are diagnostic link
contexts, not a runnable reconstructed game.
