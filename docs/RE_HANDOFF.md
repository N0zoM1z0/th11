# Agent handoff —2026-11-06

The operator ended the reconstruction goal and requested cleanup for handoff.
The latest draft is saved below; no compiler or Ghidra job remains running.
When reconstruction is resumed, prioritize complete large/difficult functions.

## Read first

Read AGENTS.md, RE_WORKFLOW.md, ORACLES.md and the relevant topic/source/ledger
rows. Inspect Git status/diff; existing dirty work belongs to this project.
Use scripts/repo-python and direct scripts/ghidra.py, never REA/Factory MCP.
Keep Ghidra/Wine serialized with existing resource defaults and established /GL.
Commits/pushes are authorized; prefix `gpt-6.1-sol: `.

## Authoritative state

- JP TH11v1.00a only; canonical ignored input resources/th11.exe. SHA256:
  `2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`.
- VC9 RTM21022/SDK6.0A from TH11-local evidence; original TU partition remains open.
- 2140 candidates;621 reviewed boundaries;503 source mappings.
- 236 canonical exact functions/31881 bytes:9 COFF,227 linked.
- Origins:90 authored,515 indeterminate,14 excluded,1521 pending.
- Full Windows i386 product/runtime/portability remain open. No new exact credit
  was added by the continuation-menu draft or cleanup.

Generated counts: [PROGRESS](PROGRESS.md). Current facts/topic index:
[KNOWLEDGE_BASE](KNOWLEDGE_BASE.md). Detailed older notes were moved to the
[historical snapshot](archive/KNOWLEDGE_BASE_2026-11-06.md), with links repaired.
Old "current" measurements there belong to individual historical contexts.

## Last draft: continuation menu3192

`src/PauseContinueMenu.cpp/.hpp` owns42D9D0..42E647/3192, not old3141:
872 instructions/3122 reachable code+22 layout+48 table,88 calls/162 fields/
15 RET4 GS paths/frame58. Complete source covers states12..19, practice score,
replay mode0 save, separate high-score-name editing and runtime transitions.
`PauseContinueMenuSupport.cpp` supplies required score/close/show/default-factory
operations; none has an independent acceptance unit.

Current63-support public-path cold result:3048 bytes/827 instructions/158 fields/
2770 full differences, correct local frame58/cookie54 and48 table bytes.
Native/source sites:factory2/3, event3 helper2/1, selection event5/4, GS/RET4 paths
15/14. Call-site sharing and lifetime/layout allocation remain open. Further
codegen experiments were stopped for handoff. See [PAUSE_CONTINUE_MENU](PAUSE_CONTINUE_MENU.md).

Close92/show64 replay zero in support only; score33 is31/21 differences, default
factory105 is107/69. No leaf exact credit. PreparePauseContinueRecords42D850/380
is boundary-reviewed, still source-open; insertion42BA80 and writer436420/1856
are also open. The only still-missing native child-finder caller42E6B0 retains
its unreviewed3179-byte default. Do not claim old HUD/gameplay/ActorInstructions
sources implement it.

All254 prior source inputs are unchanged; four new files extend the freeze to258.
All19 previously inspected menu/support contributions remain byte-identical.
No canonical exact dependency changed. Public recipe:
`probes/pause-continue-menu-context.json`, output build/probe-ltcg/PauseContinueMenuOwner.

Current private recovery files, retained by cleanup:

- `.analysis/verify-pause-continue-checkpoint.py` — latest source/receipt, full
  target, PDB-owner, exhaustive-field, complete-byte and ledger verification.
- `.analysis/pause-continue-checkpoint-proof.json`, `pause-continue-source-freeze.json`
  and `pause-continue-menu-proof.json` — current results and hashes.
- `.analysis/build-pause-continue-menu.py`, `audit-pause-continue-menu.py`, native
  inventory, Ghidra reviews and logs — reproducible local continuation evidence.

Older private checkpoint scripts assert old counts and may reference retired
experimental binaries. They are historical, not requirements to replay.

## Prior pause owner and durable cautions

42C8D0/3212 remains source-present:59-support candidate3084/160 fields/3039 full
differences. Seven direct child-query return objects restore frame68/cookie64;
the inline wrapper reused one result slot and gave frame4C. Preserve natural
object lifetimes, independent input tests and metadata stage68/difficulty64
separation. Shared child finder stays76/native70/41, stack script/RET4 versus
native EBX/plain RET. See [PAUSE_MENU](PAUSE_MENU.md).

Audio43AAD0/3836 is canonical exact; its992-byte caller still has nine prologue
differences in the audio context. Sprite binding44AB40 uses1/256, not0.5.
Void history push leaving EAX live does not prove a return-this API. Full owned
switch tables/late blocks must remain; never crop, mask, pad or force a private
register ABI to obtain exactness.

## Cache and cleanup state

[ARTIFACTS](ARTIFACTS.md) lists retained caches.188 obsolete experiment directories
were removed, net recovering about666.4 MiB after preserving their metadata.
Target, tools/Wine, database, canonical artifacts and public diagnostic outputs
were retained;8936 protected file identities were unchanged during deletion.
MenuController and MenuPrimary caches are stale; cold-build before reuse. The
other10 public diagnostic receipts passed the cleanup check. Receipt validity
is not byte equality or whole-game closure.

For a quick local recovery check:

```bash
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python .analysis/verify-pause-continue-checkpoint.py
scripts/repo-python scripts/validate-tracking.py
```
