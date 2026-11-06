# Local artifacts and cleanup

Maintained C++, probes, ledgers, tools and documentation are tracked. Targets,
analysis databases and generated binaries stay private. Start at
[RE_HANDOFF](RE_HANDOFF.md); ledger counts come from [PROGRESS](PROGRESS.md).

## Retention

| Path | Policy |
| --- | --- |
| `resources/th11.exe` | Keep the hash-pinned original Japanese1.00a input |
| `.tools/` | Keep compiler/SDK, Ghidra/JDK links, dependencies and Wine state |
| `ghidra-project/` | Keep the mutable TH11 analysis database |
| `build/match/`, `build/match-linked/` | Keep canonical artifacts/receipts declared by match-units |
| Current public diagnostic outputs | Keep the caches listed below; check receipts before use |
| `.analysis/` | Keep native reviews, decompiler evidence, current verifiers, result JSON/ASM/logs and useful experiment sources |
| Retired experiment binaries | Remove after recording results and preserving useful metadata |
| Python bytecode caches | Regenerable; not evidence |

## Cleanup:2026-11-06

Removed188 obsolete directories under `build/probe-ltcg`, totaling11218 generated
files and about727.5 MiB of allocated space. Before removal,6258 JSON/MAP/log/
text/ASM/listing/response files were retained under
`.analysis/retired-build-records-20261106/`, preserving their SHA256 hashes.
After that retained metadata, net recovered allocated space is666.4 MiB.
There are no binary backups of the discarded builds.

Before and after deletion,8936 protected file/symlink identities were checked
and remained identical. Protection included maintained source/probes/scripts/
config/docs/resources, target input, installed tools/Wine, database, canonical
builds and all public diagnostic output directories. Toolchain identity still
passes. No source was changed to improve codegen during cleanup.

The deletion plan and audit are private:

- `.analysis/cleanup-20261106-plan.json`
- `.analysis/cleanup-20261106-audit.json`

The audit includes removed paths, retained metadata hashes and protected-file
hashes. Historical receipts in the retired-records directory document removed
builds; their binaries no longer exist and they are not current acceptance
receipts. Rebuild a selected experiment before attempting a live comparison.
Older private verifier scripts may reference those retired paths or assert old
ledger counts. They are historical, not a checklist for the next agent.

The earlier2026-10-30 cleanup records are retained in
`.analysis/generated-trial-cleanup-20261030.json` and
`.analysis/unused-code-cleanup-audit.json`. Their old file counts and "three
remaining drivers" statement do not describe the current workspace.

## Public diagnostic cache status at handoff

Receipt checks were read-only; no full cohort was cold-replayed. A current
receipt proves the recorded build inputs/artifacts still agree, not byte-exact
output or runtime closure. The audio recipe shares its output with an accepted canonical context; the
other entries describe diagnostic caches. Individual support contributions
replaying zero do not establish independent exact units.

| Recipe under `probes/` | Cache status |
| --- | --- |
| configuration-audio-menu-context.json | Current; canonical audio output also retained |
| menu-character-selection-context.json | Current |
| menu-difficulty-selection-context.json | Current |
| menu-joystick-context.json | Current |
| menu-music-room-context.json | Current |
| menu-partner-selection-context.json | Current |
| menu-player-data-context.json | Current |
| menu-practice-stage-context.json | Current |
| pause-menu-context.json | Current |
| pause-continue-menu-context.json | Current; latest draft |
| menu-controller-context.json | Stale cache; cold-build before reuse |
| menu-primary-context.json | Stale cache; cold-build before reuse |

The latest retained diagnostic is `build/probe-ltcg/PauseContinueMenuOwner`.
Its current local verifier is `.analysis/verify-pause-continue-checkpoint.py`;
its build/audit drivers, full target reviews, frozen-input hashes and complete
comparison records remain in `.analysis/`. No Ghidra/compiler job is live.

Cleanup does not require rebuilding the game, replaying all exact units or
reinstating broad behavior/failure/random/caller suites. Source changes require
cold replay only of changed exact units and directly affected dependencies.
The current draft changed none of the254 prior frozen source inputs and affects
no canonical unit. Native product/runtime/portability closure remains later work.
