# TH11 reconstruction — next-agent handoff (2026-11-12)

**Target:** original Japanese *Touhou 11* v1.00a, private
`resources/th11.exe`, SHA-256
`2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`.
Do not use the localized `th11c.exe` or a TH10 target. The pinned historical
codegen candidate is **VC9 / Visual C++ 2008 RTM build 21022**; established
LTCG owners use `/GL`. The original translation-unit partition, a complete
native game binary, and runtime equivalence are **not** established.

Start with `git status --short --branch` and `git diff`. All dirty, staged
and untracked work is the project's work: **continue it, never reset or
silently discard it**. Commit coherent progress as `gpt-web: ...`; local
commits may be ahead of `origin/main`. Do not run the entire cold replay.
Direct Ghidra through `scripts/ghidra.py` is the approved target analyzer,
not REA. Read [AGENTS](../AGENTS.md), [workflow](RE_WORKFLOW.md),
[Oracle rules](ORACLES.md) and [storage rules](ARTIFACTS.md) as needed.

## Verified checkpoint and trust boundaries

[Generated progress](PROGRESS.md) is authoritative for counts. At this
checkpoint the ledgers report:

| Scope | Current count |
| --- | ---: |
| Function candidates | 2,150 (provisional inventory) |
| Reviewed boundaries | **678** |
| Maintained source mappings | **652** |
| Strict canonical exact functions | **330** |
| Strict exact code bytes | **37,980** |
| Exact with confirmed authored origin | 52 |

`config/functions.csv`, `function-boundaries.csv`,
`function-origins.csv`, `matches.csv`, and `match-units.toml` are the
per-function evidence/contracts. Source presence, size equality, and
field-masked similarity **do not count as exact**. An accepted linked owner
needs its **complete** primary-TU PDB contribution, MAP/PE identity,
exhaustive reviewed fields, current source/compiler/producer receipt, and
zero changed bytes. A normal-COFF match does **not** prove the target's
original TU or `/GL` ownership.

Recent strict successes to protect:

| Function / owner | Original size and scope | Canonical unit / detail |
| --- | --- | --- |
| `QuantizeSecondaryActorPosition` `0x410580` | **133/133**, 29 instructions, 2 calls, 5 fields | `quantize-secondary-actor-position`; shared float destination preserves x87/flag order, [lifetime](SECONDARY_LIFETIME.md) |
| `InitializeInputBindingDefaults` `0x459BD0` | **98/98**, 20 instructions, no relocations | `input-binding-defaults` **normal COFF**; natural `return this` restores entry MOV, but independent `/GL` stays 96/98, [audio](AUDIO_DEVICE.md) |
| Ten state readers/writers `0x410020..0x4100D0` | **35/35** across **10** separately owned functions, no fields | `secondary-state-accessor-*`; five setters need natural value-before-pointer arguments, [lifetime](SECONDARY_LIFETIME.md) |
| `ClearReplayStageSegments` `0x437420` | **93/93**, one delete field | `clear-replay-stage-segments`, [replay](SECONDARY_PATTERN.md) |
| `ReadSharedReplayBytes` / `CloseSharedReplayReader` `0x458980/0x4589D0` | **69/69** + **49/49**, 4 + 6 fields | `read-shared-replay-bytes`, `close-shared-replay-reader`; global-handle guard determines EBX/EDI codegen, [replay](SECONDARY_PATTERN.md) |
| `SetAudioAdjacentScratchMatrix` `0x459C70` | **168/168**, all 17 DIR32 fields | `audio-adjacent-scratch-matrix`, [audio](AUDIO_DEVICE.md) |
| `DrawSecondaryProgramBrowser` `0x438E70` | **548/548**; 130 instructions, 11 calls, 43 fields | [browser](SECONDARY_PROGRAM_BROWSER.md) |
| `DecodeAnimationChunk` `0x4545A0` | **811/811** including owned alignment/table bytes | `decode-animation-chunk`, [chunks](ANIMATION_CHUNKS.md) |
| `AdvanceAnimationDocument` `0x4544C0` | **212/212** | Independently exact; the related 135-byte slot *scanner is not*, [slots](ANIMATION_DOCUMENT_SLOTS.md) |

The broader exact catalog is in the canonical manifest and topic notes;
do not repeat all 330 units as hand-edited prose. Historical diagnostic
numbers in topic documents describe their **then-current** source and
producer, not a newer receipt for altered source.

## Substantial work ready for continuation

Both big bosses and useful leaves matter. These are real, full-source
owners; **none in this table is exact**:

| Original owner | Current scoped comparison | Concrete next step |
| --- | --- | --- |
| `LoadSecondaryProgramSelection` `0x438910` (277 bytes) | **277/277**, 81/81 instructions, 8 calls, **18 nonfield byte differences** | Native vptr write precedes `memset` argument pushes at +0x9F..+0xA2; count uses EBX/ECX and cookie store differs at +0xDB..+0xF8. Multiple natural constructor and selection rewrites did **not** help. [Browser](SECONDARY_PROGRAM_BROWSER.md), `.analysis/try-loader-new-selection-20261112.log` |
| `UpdateSecondaryProgramBrowser` `0x438AC0` (934 bytes) | **928/934**, 262/267 instructions, 22/22 calls, 173 nonfield differences | State-0 filename/selection count branch lifetime and register scheduling; keep its exact 83-byte cleanup and callbacks. [Browser](SECONDARY_PROGRAM_BROWSER.md) |
| `DestroySecondaryProgramBrowser` `0x438710` (439 bytes) | **448/439**, all 16 native calls | Original resource/ANM destruction inlining, SEH and registers. [Browser](SECONDARY_PROGRAM_BROWSER.md) |
| `InitializeSecondaryReplay` `0x4356E0` (1,380 bytes) | **1247/1380** with real segment cleanup/writer; **22/22 calls and 4/4 returns** | Native mode-0 first clears the **selected stage** via exact `ClearReplayStageSegments`, then appends to that stage, publishing a fixed +0x9C tail. Earlier **1236/1380** was *before this correction*. Restore private callback/constructor/stream register lifetimes. [Replay](SECONDARY_PATTERN.md), `.analysis/probe-secondary-replay-init-with-true-cleanup-20261112.log` |
| `LoadReplayStageData` `0x436B60` (441 bytes) | **387/441** with real file/codec producers and opaque reader, **402/441** when the two exact shared-file helpers are also sourced; 14/14 calls, 1 return | Recover private read/codec helper inputs, stack/cookie schedule; [replay](SECONDARY_PATTERN.md), `.analysis/probe-secondary-replay-load-with-helpers-20261112.log` |
| `WritePauseMenuReplay` `0x436420` (1,856 bytes) | ~**1765/1856**, 46 calls | Writer/segment ownership and original compressor ABI. [Replay writing](REPLAY_WRITING.md) |
| `DrawMenuController` `0x439E80` (140 bytes) | **112/140** with typed page calls; 8 jump cells and all 5 calls present | Recover original EAX menu receiver and aligned EDI frame. Both page 08 and 0A below have real source. [Menu](MENU_CONTROLLER.md) |
| `DrawMenuPage08` `0x43DAA0` (427 bytes) | **399/427** isolated / **406/427** with real ASCII, 2/2 calls and returns | Six practice stages, scores and color blinking implemented; preserve EBX/ECX ASCII ABI. [Menu](MENU_CONTROLLER.md) |
| `DrawMenuPage0A` `0x43F410` (670 bytes) | **627/670** isolated / **623/670** with real ASCII, all 6 calls, 1 return | Ten replay record rows with `_localtime64`, dates, high scores and 3 summaries; private EDI/vararg/x87 schedule. [Menu](MENU_CONTROLLER.md) |
| `DestroySecondaryMaster` `0x410E90` (374 bytes) | **340/374**, 12/12 calls | Program destructor inlining/renderer slots, with exact 201-byte precleanup. [Lifetime](SECONDARY_LIFETIME.md) |
| Special effect lifecycle `0x45DD70/0x45DE30/0x45DF10` (188/177/84 bytes) | Latest scoped **184/188**, **180/177**, **82/84**, preserving 5/7/2 calls | Preserve second spell-mask reload on draw; original register/VM/audio/float scheduling remains open. Earlier 178/177 and 73/84 trials are historical. [Player controller](SECONDARY_PLAYER_CONTROLLER.md) |
| `ChangeSecondaryAnimationScript` `0x456770` (36 bytes) | **30/36** with exact VM finder and real binder, 2 calls / RET4 | Its two actual callers are in the **4230-byte** `UpdateSecondaryActorState` boss; private EAX/ECX/EDX binder ABI remains open. [Actor](SECONDARY_ACTOR.md) |
| `UpdatePauseContinueMenu` `0x42D9D0` (3,192 bytes) | **3172/3192** in 63-support producer; 88/88 calls, 15/15 returns | Restore native 0x58-byte versus 0x54 stack and state-13 result lifetime; keep expensive retained diagnostic PE/PDB. [Continue menu](PAUSE_CONTINUE_MENU.md) |

Other challenging large sources: `SpawnSecondaryPlayerDrop` **851 bytes**,
`ActivateSecondaryPlayerSpecial` **268**, `EmitSecondaryActorEffects`
**457**, `BuildBillboardAnimationVmQuad` **976**, and
`LoadAnimationChunkSource` **221** (only two SIB selector bytes left).
For each, consult [player](SECONDARY_PLAYER_CONTROLLER.md),
[actor state](SECONDARY_ACTOR_STATE.md),
[billboard](ANIMATION_PROJECTED_QUAD.md) or
[chunks](ANIMATION_CHUNKS.md) before changing an established owner.

## Short continuation loop

```bash
git status --short --branch
git log -8 --oneline
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/progress.py --check
scripts/repo-python scripts/validate-tracking.py --require-target

# Only for changed exact units or their proven dependencies:
scripts/repo-python scripts/replay-exact-units.py --unit quantize-secondary-actor-position
scripts/repo-python scripts/replay-exact-units.py --unit input-binding-defaults

# Direct original evidence for a selected address:
scripts/repo-python scripts/ghidra.py query .analysis/selected-review.txt review 0x00410580
```

Use pinned `scripts/compile-probe.sh` and `scripts/ltcg_link.py` for scoped
non-exact diagnostics. The public source/probe code and locally retained
`.analysis/*.py` recipes are reusable. `build-match-unit.py` rebuilds a
selected canonical unit with current source/tool hashes. Source edits require
**full affected-function bytes/fields**, plus directly affected exact units;
prose-only edits require neither compiler nor cold replay. Never run a
routine all-unit cold replay or broad behavioral/fault-injection suite.

## Local artifact/storage handoff

`build/match-linked/` and `build/match/` contain useful **canonical receipt
families**. Keep them and the pinned original/tools/Wine/Ghidra projects.
`build/probe-ltcg/` and `build/probe/` are ignored, **regenerable diagnostic
build caches**, not authoritative source or exact proofs. On 2026-11-12,
**835 old diagnostic links and the scratch-object trees were deleted**,
reclaiming **831.9 MiB**; `build/` fell from ~965 MiB to ~133 MiB.
Five representative contexts remain: `PauseContinueMenuOwner`,
`LoaderCtorOrder20261112-explicit-before-clear`,
`BrowserState0-20261112-goto-layout`,
`SecondaryReplayLoadRealHooks-real-all-hooks-20261112` and
`MenuReplayRecordsDraw-isolated-20261112` (under `build/probe-ltcg/`).
Scripts/logs in `.analysis/` retain the important decisions even when
historical binary images are gone. Do not expect deleted probe binaries
to exist; rerun their recorded recipes when a specific context is needed. Never delete the
original `resources/th11.exe`, `.tools/msvc900`,
`.tools/wineprefix-msvc900`, `ghidra-project/`, useful `.analysis/` evidence,
or existing uncommitted work. See [artifacts](ARTIFACTS.md).

**Git state is provenance, not acceptance.** Existing upstream divergence is
normal; commit cleanup and meaningful reconstruction with `gpt-web: ...`.
Do not commit binary targets, compiled artifacts or private decompiler dumps.
