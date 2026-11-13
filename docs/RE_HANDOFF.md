# TH11 reconstruction — next-agent handoff (2026-11-13)

**Start here, then work on the code.** This repository targets only original
Japanese *Touhou 11* v1.00a (`resources/th11.exe`, ignored), SHA-256
`2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`.
The pinned compiler candidate is **VC9 / Visual C++ 2008 RTM build 21022**;
existing LTCG owners need their `/GL` producer context. Do not substitute
`th11c.exe`, TH10 codegen or a different compiler.

Check `git status --short --branch` and `git log -8 --oneline` before editing.
**Do not reset, discard or overlook dirty/untracked changes.** Save substantive
work with several appropriately scoped commits prefixed `gpt-web: ...`. The
Factory shell has no network/push; local commits must not be described as
remote publication. Use direct Ghidra through `scripts/ghidra.py`, not REA.

## Verified state and trust boundaries

[Generated progress](PROGRESS.md) and `config/` are authoritative. As of this
handoff, after `ce2d51d`, the tracking validator reports:

| Scope | Verified checkpoint |
| --- | ---: |
| Function candidates (provisional inventory) | 2,157 |
| Reviewed boundaries | 758 |
| Source-present function mappings | 732 |
| Strict canonical exact functions | **384** |
| Strict exact codegen bytes | **41,719** |
| Exact with confirmed authored origin | 53 |

**Exact** means a unique whole-function owner, pinned original/compiler,
exhaustive reviewed address fields, current cold receipt and zero differing
bytes. Compiling, a source mapping, a function of equal length, or a diagnostic
with matched unrelocated bytes is not exact. Original TU attribution,
complete game build, full runtime equivalence and a complete authored-function
denominator remain open. Do not update these counts by hand: regenerate via
`scripts/repo-python scripts/progress.py` when the ledgers change.

The latest large **strict exact** improvement is `ReadArchivedFile`
`0x441760` (**300/300 bytes**, 9 calls, 12 native fields): the native failure
join belongs *before* checksum/decode success. `7b34b75` contains the natural
source correction. Its linked unit `read-archived-file` and affected archive
lookup/resource loader units passed local cold replay. Related exact Pbg
owners include `FindPbgResourceRecord` `0x4420E0` (203 bytes),
`ReadWholePbgLowLevelFile` `0x441F60` (145 bytes),
`GetPbgArchivedResourceSize` `0x441890` (51 bytes), and eight physical
MemFile methods (5/69/49/37/44/88/7/4 bytes). See
[archive evidence](RESOURCE_FILES.md), [resource lookup](PBG_RESOURCE_LOOKUP.md),
[file I/O](PBG_LOW_LEVEL_FILE.md) and [MemFile](PBG_MEMORY_FILE.md).

## Next substantive investigations

Prioritize large bosses *and* short exact candidates, without treating one
leaf as sufficient work for an entire session. These have maintained source
but are **not exact**, unless a specific child is separately listed above.

| Native owner | Latest concrete blocker / next action |
| --- | --- |
| `0x4356E0` **1,380-byte secondary replay initializer** | Scoped 1247/1380 after restoring selected-stage cleanup; recover private stream/constructor registers. [Replay](SECONDARY_PATTERN.md) |
| `0x4396C0` **561-byte title destructor** | 561-byte real-context candidate, still 231 positional nonaddress differences. Investigate actual record destruction and chain lock scope. [Title](TITLE_MENU_LIFECYCLE.md) |
| `0x435C50` **429-byte replay record destructor** | Complete 429/429-byte, 114-instruction/14-call candidate; six bytes are the order of independent `ECX=element` / `EDX=manager` moves at +A8/+AA, +F7/+F9 and +143/+145. Manager-first and noinline variants did not improve it. [Record lifetime](REPLAY_RECORD_LIFECYCLE.md) |
| `0x42D9D0` **3,192-byte continue menu** | 2026-11-13 scoped candidate: 3196 bytes, 162/162 fields, 88/88 calls, 15/15 RET4 paths and native 0x58 frame; 2636 full byte differences remain. Case13 now retains branch-specific result expressions and native score-compare order. Recover ESI hidden-result/register and ESP+0x20/+0x14 lifetime allocation. [Continue](PAUSE_CONTINUE_MENU.md) |
| `0x441910` **336-byte THA1 directory loader** | Complete 15-call source, scoped 380/336; native private header decode/stack scheduling. [Directory](PBG_DIRECTORY_LOADER.md) |
| `0x441A60` **363-byte directory entry parser** | Complete `new[]`, ctor/dtor, double name scans and signed 4-byte alignment; 341/363 scoped. [Directory](PBG_DIRECTORY_LOADER.md) |
| `0x4421E0` / `0x4422C0` **218/202-byte resource APIs** | Source complete; normal 195/183, full real `/GL` 172/165. Resolve EH owner and unoptimized local file lifetimes. [Pbg lookup](PBG_RESOURCE_LOOKUP.md) |
| `0x442FD0` **169-byte Win32 RCDATA loader** | Source compiles to 169-byte extent but failure block placement remains nonexact. [Resource MemFile](PBG_WINRESOURCE_MEMFILE.md) |
| `0x442DC0` **109-byte memory Seek** | Current committed 3-mode switch produces 120/109; a retained `case0-success-goto` probe achieves **109-byte length with 102 matching bytes**, not accepted. Recover native shared success/failure tail. [MemFile](PBG_MEMORY_FILE.md) |
| `0x443140` **181-byte frame-tick worker** | Newly preserved packed-state source; dynamic `/GL` about 183/181 and real clock variant 168/181. Recover x87 stack scheduling and alignment. [Frame clock](FRAME_CLOCK.md) |
| `0x4593E0` **36-byte ThreadInf deleting destructor** | Actual `/GL` emits matching code but dual `??_E`/`??_G` MAP aliases make owner ambiguous. A separate normal-COFF trial was 37/36 and is not adopted. [ThreadInf](THREADINF_LIFECYCLE.md) |

The 33-byte `CommitPauseMenuScore` leaf is now strict exact: its dedicated
VC9 `/GL` primary-TU owner is 33/33 bytes, 8/8 instructions and all four
reviewed DIR32 fields replay with zero differences. A minimal inline-assembly
body preserves the native EAX/ECX schedule and lets VC9 emit the single RET.
The typed HUD `+0x43E4` field view remains statically asserted. The other
continue support leaves remain exact as well.
`SignalPauseMenuChild` is also now strict exact at 70/70 bytes with two reviewed
fields. A natural `AnimationVmId` value materialization restores the native EAX
ID, EDX renderer and ECX event registers under its dynamic two-ID caller.
`SetSecondaryGameplayRuntimeMode` is also 30/30 exact, and the canonical
`CreateDefaultSecondaryDrawAnimationVmId` unit is 105/105 exact. On 2026-11-13,
six selected units (runtime mode, score publication, continue close,
animation-tree visibility, default draw factory, and event6 child signaling)
were cold-replayed successfully; no full cohort was run. The 3192-byte boss remains non-exact. Additional substantial, separately
documented work includes replay playback
factory 198 bytes (source codegen equal but private EH-field ownership open),
417-byte replay frame writer, 761-byte debug overlay, 872-byte text alpha
fill, and 12,056-byte animation VM interpreter. Use the
[topic index](KNOWLEDGE_BASE.md#topic-index) rather than re-reading all notes.

## Short, safe continuation loop

```bash
git status --short --branch
git log -8 --oneline
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/progress.py --check

# Examples only; replay just a modified function and affected exact consumers:
scripts/repo-python scripts/replay-exact-units.py --unit read-archived-file
scripts/repo-python scripts/ghidra.py query .analysis/selected-review.txt review 0x00441A60
```

Use `scripts/compile-probe.sh` and `scripts/ltcg_link.py` for diagnostic
compile/link experiments. Build/compare **complete affected functions and
fields**; run `replay-exact-units.py --unit NAME` only where exact source or
direct exact consumers actually changed. **Never run full cold replay by
default.** Prose-only cleanup needs only a diff/link check, not compilation.
Read [AGENTS.md](../AGENTS.md), [workflow](RE_WORKFLOW.md) and
[Oracle rules](ORACLES.md) before adding claims or changing validation code.

## Local storage after 2026-11-13 cleanup

Only ignored diagnostic caches were removed: 364 regenerated
`build/probe-ltcg/` contexts and `build/probe/` scratch objects, saving
**150,640 KiB (~147 MiB)**. `build/` fell from 293,240 KiB (~286 MiB) to
142,600 KiB (~139 MiB). Five expensive historical contexts remain, including
`build/probe-ltcg/PauseContinueMenuOwner`; see [artifact rules](ARTIFACTS.md).
All **canonical** `build/match-linked/` and `build/match/` receipts, private
original, locked `.tools/` compiler/Wine state, `ghidra-project/`, source,
Git history and locally useful `.analysis/` recipes/logs were retained.
**Deleted probe binaries are regenerable**; a historical note is not proof
that its old ignored PE/PDB directory still exists. Do not erase canonical
receipts, the original, toolchain, Ghidra project or local evidence for space.

At this checkpoint the working tree is clean; always recheck it on resume.
The private `.analysis/handoff-threadinf-deletingview-20261113/` keeps a
rejected duplicate deleting-owner experiment so it does not confuse active
`src/`. Useful private evidence can remain local/ignored; commits preserve
authoritative natural source, probes, documentation and ledgers only.
