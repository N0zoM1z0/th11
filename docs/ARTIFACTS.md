# Local artifacts

Keep the original `resources/th11.exe`, the locked compiler components and their
used headers/libraries/runtime dependencies, the TH11 Ghidra project and the
current independent Wine prefix. Reuse matching Factory tools through links;
each game keeps its own writable state.

Source, public probe recipes, exact ledgers and full comparison scripts are the
reproducible result. Canonical artifacts in `build/match/` and
`build/match-linked/` are useful caches, not irreplaceable inputs. Rebuild an
affected context before relying on its byte equality.

`build/probe-ltcg/` is a disposable, ignored diagnostic cache; preserve
only expensive current owner contexts with a concrete handoff/debugging
purpose (not every loop/register variant). In particular retain
`PauseContinueMenuOwner` for the 63-support-TU menu investigation.
Historical diagnostic images can be recreated from the source in `src/`,
probe recipes in `probes/` and useful local `.analysis/` scripts and logs.
A directory mentioned as *historical* evidence need not remain cached.

Do **not** clean `build/match-linked/` or `build/match/` merely for space:
those are the current accepted-unit PE/PDB/object/receipt caches, though
the official match is always the freshly verified unit. Similarly,
`.tools/msvc900` and `.tools/wineprefix-msvc900` are not throwaway
artifact directories; removing them would break or force reinstallation
of the pinned compiler. The private original, Ghidra database and Wine
prefix are protected even if they are the largest users of disk space.

Keep compact useful failed-experiment findings in topic notes. Do not
commit diagnostic binaries, stale dump copies or screenshots of original
target bytes. For the current policy and retained investigation context
see [RE_HANDOFF](RE_HANDOFF.md).

## Handoff storage cleanup (2026-11-12)

Before cleanup, `build/` occupied approximately **965 MiB**, including
**844 MiB** of noncanonical `build/probe-ltcg/` test links. An explicit
manifest check verified that **all 330 canonical unit outputs** reside in
`build/match-linked/` or `build/match/`, outside the removable diagnostic
paths. We deleted **835 old diagnostic context directories** and the small
ignored `build/probe/`/`build/probe-coff/` scratch-object trees, reclaiming
**851,908 KiB (831.9 MiB)**. The post-cleanup `build/` occupies about
**133 MiB**.

Exactly five representative ignored diagnostic contexts were retained:

- `build/probe-ltcg/PauseContinueMenuOwner` — expensive 63-support-TU
  continuation owner, approximately 11 MiB;
- `build/probe-ltcg/LoaderCtorOrder20261112-explicit-before-clear` — 277-byte
  browser loader vtable/push scheduling comparison;
- `build/probe-ltcg/BrowserState0-20261112-goto-layout` — 934-byte browser
  state-0 dispatch comparison;
- `build/probe-ltcg/SecondaryReplayLoadRealHooks-real-all-hooks-20261112`
  — 441-byte replay loader with real file/codec helper bodies;
- `build/probe-ltcg/MenuReplayRecordsDraw-isolated-20261112` — 670-byte
  menu replay-records owner.

The `.analysis/` recipes, logs and original review evidence, all canonical
`build/match-linked/` / `build/match/` object/PE/MAP/PDB families and
receipts, `resources/th11.exe`, `.tools/`, and `ghidra-project/` were
preserved. After cleanup a targeted linked-PE comparison of
`quantize-secondary-actor-position` still reported `difference_count=0`,
and the normal-COFF `input-binding-defaults` comparison still reported
**98/98 exact**. These cache comparisons check preservation only; they do
not replace a new cold build after future source/tool changes.

To recreate a missing historical diagnostic, rerun its matching local
`.analysis/*.py` script or existing public `probes/` context. To refresh a
canonical exact owner after an edit, rebuild only its affected unit with
`scripts/repo-python scripts/replay-exact-units.py --unit UNIT_NAME`;
**do not run the full cohort** just because diagnostic caches were removed.
