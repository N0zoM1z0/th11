# Reconstruction handoff

Follow `AGENTS.md`, `RE_WORKFLOW.md` and `ORACLES.md`. Inspect Git status/diff;
continue existing work. Use `scripts/repo-python`, direct `scripts/ghidra.py`
and the isolated TH11 state. Serialize Ghidra and Wine/compiler jobs.

## Current state

The target remains original Japanese TH11v1.00a, SHA256
`2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`.
VC9 RTM21022/SDK6.0A is the observed compiler candidate; original TU ownership
remains open. Exact counts come from [PROGRESS](PROGRESS.md); findings and topic
links are in [KNOWLEDGE_BASE](KNOWLEDGE_BASE.md).

The latest source draft is `src/PauseContinueMenu.cpp/.hpp`, reviewed at
42D9D0..42E647/3192 bytes. It covers states12..19, practice score, replay mode0,
record-name editing and runtime transitions. It remains non-exact: the
63-support public context produces3048 bytes/158 fields/2770 full differences.
Frame58/cookie54 and48 table bytes agree. Factory sites are3/native2, event3
sites1/native2, selection events4/native5 and GS/RET4 paths14/native15.
Investigate call sharing, object lifetimes and layout allocation. See
[PAUSE_CONTINUE_MENU](PAUSE_CONTINUE_MENU.md).

Rebuild with `probes/pause-continue-menu-context.json`; its output is
`build/probe-ltcg/PauseContinueMenuOwner`. The latest local build/audit helpers,
native reviews, comparison records and checkpoint verifier remain in
`.analysis/`. Older unrelated experiment outputs and verifiers are removed.
Canonical source, ledgers and build/comparison scripts are unchanged.

## Next reconstruction work

Prioritize substantial complete owners and their byte-level gaps. The pause
owner42C8D0/3212 remains3084/160 fields/3039 differences. Seven direct child-query
result objects recover frame68/cookie64; an inline wrapper reused one slot and
produced frame4C. Preserve independent input tests and metadata stage68 versus
difficulty64. The shared child finder stays76/native70/41 with an unresolved
register/stack ABI difference. See [PAUSE_MENU](PAUSE_MENU.md).

PreparePauseContinueRecords42D850/380 and InsertPauseContinueScore42BA80/365
now have VC9-compiled source in src/PauseContinueRecords.cpp and
src/PauseRecordInsertion.cpp. Full normal-COFF diagnostic sizes are387/232;
they are not exact. The insertion coefficient uses100.0, and the stage7/saved
run branch is rechecked after insertion. Writer436420/1856 remains source-open.
See PAUSE_CONTINUE_MENU.md. The other native child-finder
caller42E6B0 retains an unreviewed3179-byte default. Do not substitute unrelated
HUD/gameplay/ActorInstructions sources.

## Checks

Default `scripts/repo-python scripts/ci.py` checks the public checkout without
private inputs. Cold-build and fully compare only affected exact contexts.
Toolchain identity: `scripts/repo-python scripts/verify-toolchain.py --check`.
Canonical caches remain in `build/match/` and `build/match-linked/`; select a cold
rebuild before relying on a cache. See [ARTIFACTS](ARTIFACTS.md) for retention.
