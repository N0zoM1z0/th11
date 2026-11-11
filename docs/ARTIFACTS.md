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
