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

[Generated progress](PROGRESS.md) and `config/` are authoritative. The
production-source baseline for this handoff is `e7e1f13`
(`gpt-web: recover pause replay deletion ABI`), which was at `origin/main`
before the documentation-only handoff commits. An unfinished local ABI
experiment had modified six production files and left three probe copies in
`src/`. It was measured, found to worsen the Game Over boss, and reverted to
the committed baseline; the useful ABI observations and focused source
experiments are retained under `.analysis/`. Do not restore the rejected
production edits as if they were accepted progress. Regenerated
`docs/PROGRESS.md` reports:

| Scope | Current checkpoint |
| --- | ---: |
| Function candidates (provisional inventory) | 2,157 |
| Reviewed boundaries | 760 |
| Source-present function mappings | 734 |
| Canonical exact functions | **389** |
| Canonical exact codegen bytes | **42,616** |
| Exact with confirmed authored origin | 53 |

Use `docs/PROGRESS.md` for the complete generated breakdown; do not copy these
counts into the SVG or ledgers by hand.

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
| `0x42BA80` **365-byte score insertion leaf** | **365/365 exact**, all17 DIR32/REL32 fields reviewed, canonical normal-COFF unit `insert-pause-continue-score`. [Continue](PAUSE_CONTINUE_MENU.md) |
| `0x42D850` **380-byte record preparer** | **380/380 exact**, all17 DIR32/REL32 fields reviewed, canonical normal-COFF unit `prepare-pause-continue-records`; private ESI receiver recovered and both callers updated. Its 3192-byte boss remains open. [Continue](PAUSE_CONTINUE_MENU.md) |
| `0x42D9D0` **3,192-byte continue menu** | Best focused 63-support `/GL` diagnostic: **3184/3192 bytes**, **162/162 fields**, **88/88 calls**, **15/15 RET4 paths**, 0x58-byte frame and **2008 non-address differences**. `/Oy` plus branch-local result slots improved the case-13 call ABI, but candidate still uses EBX for the menu receiver and an EBP frame; native uses EBP receiver with ESP-relative frame. Eight-byte owner extent and remaining codegen differences remain open. [Continue](PAUSE_CONTINUE_MENU.md) |
| `0x42E6B0` **3,208-byte game-over menu** | Best retained focused VC9 `/GL` diagnostic: **3212-byte PDB contribution**, **154/154 fields**, **88/88 calls**, **14/14 RET4 paths**, seven switch entries and **2040 non-address differences**. Its 0x58-byte ESP frame, GS cookie slot and EBP menu receiver match the native prologue shape, but the function is not exact. A separate state-26 variant measured 2039 differences, but its relink output was lost during a failed Wine link retry. [Game Over](PAUSE_GAME_OVER_MENU.md) |
| `0x441910` **336-byte THA1 directory loader** | Complete 15-call source, scoped 380/336; native private header decode/stack scheduling. [Directory](PBG_DIRECTORY_LOADER.md) |
| `0x441A60` **363-byte directory entry parser** | Complete `new[]`, ctor/dtor, double name scans and signed 4-byte alignment; 341/363 scoped. [Directory](PBG_DIRECTORY_LOADER.md) |
| `0x4421E0` / `0x4422C0` **218/202-byte resource APIs** | Source complete; normal 195/183, full real `/GL` 172/165. Resolve EH owner and unoptimized local file lifetimes. [Pbg lookup](PBG_RESOURCE_LOOKUP.md) |
| `0x442FD0` **169-byte Win32 RCDATA loader** | Source compiles to 169-byte extent but failure block placement remains nonexact. [Resource MemFile](PBG_WINRESOURCE_MEMFILE.md) |
| `0x442DC0` **109-byte memory Seek** | Current committed 3-mode switch produces 120/109; a retained `case0-success-goto` probe achieves **109-byte length with 102 matching bytes**, not accepted. Recover native shared success/failure tail. [MemFile](PBG_MEMORY_FILE.md) |
| `0x443140` **181-byte frame-tick worker** | Newly preserved packed-state source; dynamic `/GL` about 183/181 and real clock variant 168/181. Recover x87 stack scheduling and alignment. [Frame clock](FRAME_CLOCK.md) |
| `0x4593E0` **36-byte ThreadInf deleting destructor** | Actual `/GL` emits matching code but dual `??_E`/`??_G` MAP aliases make owner ambiguous. A separate normal-COFF trial was 37/36 and is not adopted. [ThreadInf](THREADINF_LIFECYCLE.md) |

The pause/secondary-draw work now has these strict canonical exact owners
(the origin of each native function remains indeterminate unless separately
classified):

| Owner | Native bytes | Canonical unit |
| --- | ---: | --- |
| `CommitPauseMenuScore` `0x42BA20` | 33/33 | `commit-pause-menu-score` |
| `SignalPauseMenuChild` `0x42F380` | 70/70 | `signal-pause-menu-child` |
| `ClosePauseContinueMenu` `0x42D7F0` | 92/92 | `close-pause-continue-menu` |
| `ShowPauseMenuAnimationTree` `0x456610` | 64/64 | `show-pause-menu-animation-tree` |
| `SetSecondaryGameplayRuntimeMode` `0x40DEA0` | 30/30 | `set-secondary-gameplay-runtime-mode` |
| `PublishSecondaryDrawAnimationVm` `0x4557E0` | 123/123 | `publish-secondary-draw-animation-vm` |
| `CreateSecondaryEmitterAnimationVmId` `0x455BC0` | 112/112 | `create-secondary-emitter-animation-vm-id` |
| `CreateDefaultSecondaryDrawAnimationVmId` `0x455D80` | 105/105 | `create-default-secondary-draw-animation-vm-id` |
| `CreateSecondaryDrawAnimationVmId` `0x455DF0` | 112/112 | `create-secondary-draw-animation-vm-id` |
| `ResetSecondaryPatternSlot` `0x4355F0` | 24/24 | `reset-secondary-pattern-slot` |

The score leaf is 8/8 instructions with four reviewed DIR32 fields; its small
inline-assembly body preserves the native EAX/ECX order and the HUD mirror at
`+0x43E4`. `SignalPauseMenuChild` has two reviewed fields. The 123-byte
secondary append publisher uses the private EBX=VM / EAX=hidden-result-slot
ABI. The two secondary append factories use the same private publisher; the
default factory remains in `PauseContinueMenuSupport.cpp`, while the explicit-
layer factory and publisher are in `SecondaryDrawPublisher.cpp`.

### Shared selection helper ABI (provisional, not canonical exact)

The shared helper at `0x459160`, currently named
`MoveSecondaryDebugSelection`, does **not** use the ordinary two-stack-
argument ABI implied by its C++ `__stdcall` declaration. The native entry
uses EAX for the selection pointer, one signed delta on the stack, returns the
current selection in EAX, and exits with `RET 4`. A throwaway naked body in
`.analysis/ConfigurationMenuSelection-native-abi-20261113.cpp` reproduced
**121/121 bytes** in a normal-COFF structural comparison with zero
relocations. That result has no accepted canonical receipt and must not be
called strict exact. The existing production C++ implementation and its
callers were not migrated.

A Game Over-only thiscall facade was also tested. It generated a
**3236/3208-byte** boss with **2116 non-address differences**, worse than the
retained candidate, and was reverted. The native helper has many shared
callers; do not change its global declaration or rewrite its implementation
until the full caller set and all call-site ABIs are accounted for. The
throwaway raw source is retained for analysis, not as an adopted implementation.

The five-unit selected replay run for the new secondary append path passed:
`publish-secondary-draw-animation-vm`,
`create-secondary-emitter-animation-vm-id`,
`create-secondary-draw-animation-vm-id`,
`create-default-secondary-draw-animation-vm-id`, and
`reset-secondary-pattern-slot`. Earlier selected runs also covered the score,
child signal, close/tree helpers and runtime-mode leaf. No full-cohort cold
replay was run. The 365-byte score-insertion leaf, 380-byte record preparer,
18-byte pause replay delete helper and 11-byte heap-free thunk are exact. The
3192-byte continue boss has a focused 63-support `/GL` candidate at
3184/3192 bytes, 162/162 fields, 88/88 calls, 15/15 RET4 exits and 2008
non-address differences. The 3208-byte Game Over boss has a retained
3212-byte linked candidate with 154/154 fields, 88 calls, 14 RET4 exits and
2040 non-address differences. Both bosses remain non-exact.

The native original TU/class/library authorship of these recovered owners is
not established. A local commit is a source checkpoint, not an Oracle receipt.
The Factory accepted-snapshot adapter currently rejects this repo's VC9 family
(`unsupported Windows toolchain family: Microsoft Visual C++ 2008 RTM (VC9)`);
use the repository's pinned-toolchain validators and selected
`scripts/replay-exact-units.py --unit NAME` workflow, and do not treat that
adapter limitation as a codegen result.

Additional substantial, separately documented work includes replay playback
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

## Local storage and cleanup

Two targeted ignored-cache cleanups were performed on 2026-11-13. The first
removed 364 old probe contexts (about 147 MiB). The handoff cleanup then
removed 156 additional exploratory `build/probe-ltcg/` directories, six loose
files and 2,028 KiB of scratch `build/probes/` objects. `build/` fell from
318,428 KiB to 159,728 KiB, reclaiming **158,700 KiB (~155 MiB)**. Exact
before/after sizes and the full removal list are recorded in
`.analysis/handoff-storage-cleanup-20261113b.txt` and summarized in
[ARTIFACTS.md](ARTIFACTS.md).

Eleven diagnostic contexts are retained: the five earlier history caches,
the four pause contexts above, plus `PauseContinueBossTrackedBestOy-20261113`
(the best retained 3184/3192 continuation candidate) and
`PauseGameOverFpoAdapters-20261113` (the best retained Game Over candidate).
The latter cache is a diagnostic PE/MAP/PDB snapshot, not an Oracle receipt.
The state-26 variant source and adapter recipe remain in `.analysis/`; its
latest relink attempt hit a Wine linker page fault while opening the cached
Archive support object, so its 2039-difference result is historical until
reproduced.

A third targeted cleanup removed 67 non-retained `build/probe-ltcg/`
directories/loose files (92,079,610 bytes) and 49 one-off `.analysis/`
experiment files (417,326 bytes). The complete build now occupies about
163 MiB; the 11 retained diagnostic contexts occupy about 36 MiB.
All canonical `build/match-linked/` and `build/match/` receipts, the private
original, `.tools/` compiler/Wine state, `ghidra-project/`, source, Git
history and substantive Ghidra/review evidence were preserved. The exact
cleanup summary is `.analysis/handoff-storage-cleanup-20261113c.txt`.
Deleted probe binaries are regenerable; a historical note is not proof its
old ignored PE/PDB directory still exists. Do not erase canonical receipts,
the original, toolchain or Ghidra project for space.

The production tree is restored to the committed `e7e1f13` baseline; the
worktree was clean before these documentation-only handoff commits. The private
`.analysis/handoff-threadinf-deletingview-20261113/` keeps a rejected
duplicate deleting-owner experiment out of active `src/`. Commits preserve
authoritative source, reusable probes, documentation and ledgers; ignored
build outputs remain local and disposable.
