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

The continue menu owner at42D9D0..42E647/3192 bytes now merges its case13
factory call after practice/record branches, as the native control flow does.
The focused63-support VC9 /GL replay yields3112 bytes/160 fields/15 returns
versus native3192/162/15, with **no call-count differences**; its frame54
still differs from native58 and2797 full bytes differ. The earlier3048-byte
draft had158 fields,14 returns and four call-count gaps. A second layout with
two explicit result objects recovered frame58 but regressed call/return parity,
so it was rejected. See [PAUSE_CONTINUE_MENU](PAUSE_CONTINUE_MENU.md).

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
run branch is rechecked after insertion. Writer436420/1856 and stream
allocator437300/150 have compiled source in src/ReplayWriter.cpp; normal-COFF
sizes1684/160, neither exact. A focused eight-TU /GL link gives writer
1765/native1856 and 46/46 call sites (15 sprintf, two memset), while the
allocator remains160/native150. Original private ABIs still differ.
See REPLAY_WRITING.md. Compression4423A0 now has compiled source; normal-COFF candidate738 bytes versus native795, non-exact. See
REPLAY_WRITING.md and PAUSE_CONTINUE_MENU.md. Compression tree reset
4428D0/52 has a cold-pinned exact normal-COFF unit with four replayed
relocations. Tree search442910/416 also has compiled source in
src/ReplayCompressionSearch.cpp; normal COFF416/19 fields/270 of340
comparable bytes, nonexact. Continue narrowing the native-vs-VC9 codegen on compressor main
4423A0, then test a focused linked /GL context with the non-exact
node helpers in src/ReplayCompressionTreeOps.cpp (owners79/80/134/49).
The predecessor442BE0/49 is now **cold-pinned linked exact** with three
independent DIR32 field replays; the other three tree edits remain nonexact.
A focused five-TU VC9 /GL link gives encoder760/native795,
tree matcher414/native416, replacement134/native134 (nonexact), deletion
87/native79 and reset52/native52. See REPLAY_WRITING.md for field gaps;
this is diagnostic, not exact. No unrelated cold cohort needed.
The other child-finder caller42E6B0 now has a **reviewed complete3208-byte**
owner with the seven-entry jump table and VC9-compiled source in
src/PauseGameOverMenu.cpp. Branch-local selection reset in state21 improves
normal COFF3068->3116 and a focused /GL link with MeshEffects.cpp gives
3180/native3208 bytes,88/88 calls and14/14 returns (seven table cells).
It remains non-exact; next align factory result and name-editor layouts. See
[PAUSE_GAME_OVER_MENU](PAUSE_GAME_OVER_MENU.md). Neighboring 42F340/350
14-byte selection comparisons now have natural C++ source, 12/14 bytes
each and no confirmed xrefs. Overlay queue42F400/103 now has a strict cold linked-PE **103/103
exact** unit (no relocation fields, full PE/MAP/PDB receipt). Adjacent
VM capture42F480/101 has corrected sprite height/width parameter order
but remains non-exact (linked100). An early volatile-index experiment gets
linked101 but swaps native EBP/EBX and was not retained; a float-temp
variant regressed to111. See PAUSE_OVERLAY_RECTANGLES.md. See
[PAUSE_OVERLAY_RECTANGLES](PAUSE_OVERLAY_RECTANGLES.md). Do not rebuild
unrelated gameplay or canonical units.

## Checks

Default `scripts/repo-python scripts/ci.py` checks the public checkout without
private inputs. Cold-build and fully compare only affected exact contexts.
Toolchain identity: `scripts/repo-python scripts/verify-toolchain.py --check`.
Canonical caches remain in `build/match/` and `build/match-linked/`; select a cold
rebuild before relying on a cache. See [ARTIFACTS](ARTIFACTS.md) for retention.
