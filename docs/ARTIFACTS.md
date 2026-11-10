# Local artifacts

Keep the original `resources/th11.exe`, the locked compiler components and their
used headers/libraries/runtime dependencies, the TH11 Ghidra project and the
current independent Wine prefix. Reuse matching Factory tools through links;
each game keeps its own writable state.

Source, public probe recipes, exact ledgers and full comparison scripts are the
reproducible result. Canonical artifacts in `build/match/` and
`build/match-linked/` are useful caches, not irreplaceable inputs. Rebuild an
affected context before relying on its byte equality.

The unresolved continuation-menu context is retained at
`build/probe-ltcg/PauseContinueMenuOwner`, with its current build/audit helpers,
native review and result records in `.analysis/`. See [RE_HANDOFF](RE_HANDOFF.md).
Other experimental outputs can be regenerated when needed.

Keep useful findings and failed approaches in the topic notes. A historical
reference does not justify keeping its output. Remove obsolete outputs and
their references together; do not archive them or create cleanup reports.
