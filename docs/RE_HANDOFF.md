# TH11 reconstruction — agent handoff (2026-11-12)

**Read this before resuming work.** Reconstruct only *original Japanese*
TH11 v1.00a, SHA-256
`2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`,
from private `resources/th11.exe`. The pinned codegen candidate is
**VC9 / MSVC 2008 RTM build 21022**, with scoped `/GL` compilation for
LTCG owners. The original TU/class partition and runnable product
remain open.

Start with `git status --short --branch`. Continue **all** dirty and untracked
work—do not reset or sidestep it. Use `gpt-web: ...` commits promptly,
and compare only changed functions and directly affected exact units;
**never run the full cold replay** as routine work.
Follow [AGENTS](../AGENTS.md), [workflow](RE_WORKFLOW.md) and
[Oracle requirements](ORACLES.md). Use direct `scripts/ghidra.py`, not REA.

## What is authoritative

- [Generated progress](PROGRESS.md) and
  `config/{functions,function-boundaries,function-origins,matches}.csv`
  determine names, origins, reviewed extents and exact status.
  At this checkpoint: **296 canonical exact**, **589 source-present**,
  **661 reviewed boundaries**. Refresh with
  `scripts/repo-python scripts/progress.py`, not manual totals.
- An exact owner requires the entire original function, source/tool
  receipts, exhaustive reviewed field rebinding and **zero differing
  bytes**. Similar length or field-masked similarity grants no credit.
- The large `DecodeAnimationChunk` at `0x4545A0` is **811/811 exact**
  in the canonical `decode-animation-chunk` linked unit, with 15
  fields and nine internal alignment bytes. Its C++ is
  `src/AnimationChunk.cpp`; see [chunk evidence](ANIMATION_CHUNKS.md).
- Independently exact related animation owners include sprite store
  **149 bytes**, embedded texture **287**, empty texture **67**,
  render target **69**, document cleanup **243** (eight fields),
  menu file VM marker **78**, and indexed slot release **51**.
  They retain separate `config/match-units.toml` contracts.
- All 27 registered animation renderer draw-priority callbacks now have **780/780
  exact bytes**. The 22 ordinary callbacks use 22 REL32 call fields; five
  stateful wrappers add 453 bytes and 43 reviewed DIR32/REL32 fields. All
  passed scoped cold PE/MAP/PDB proof under real constructor/camera source. See [renderer lifecycle](ANIMATION_RENDERER_LIFECYCLE.md).
- The two renderer **update** callback adapters at `0x455120` and
  `0x455130` now have independent **7/7 exact** linked proofs with one REL32
  tail jump each. See [renderer lifecycle](ANIMATION_RENDERER_LIFECYCLE.md).
- Two newly reconstructed front-secondary animation factories are **105/105**
  and **112/112 strict exact**, each with 11 reviewed linked fields and a
  real `AnimationVmFactory.cpp` initializer producer. Their shared 118-byte
  head-prepend publisher is source-present but remains 111/118 diagnostic;
  three existing exact shared-header consumers were re-cold-proved.
  See [emitter factory evidence](SECONDARY_EMITTER_RESET.md).
- The 275-byte `EnterSecondaryContinueMenu` and 336-byte
  `EnterSecondaryGameOverMenu` now have maintained full source and targeted
  272/275 and 336/336 diagnostics (non-exact). The real 123-byte
  `QueueMusicWaveRequest` callee at `0x42A0D0` is now **strict 123/123
  exact**, five reviewed fields; real generic sound producer needed to
  preserve EAX/EDI argument allocation. See [sound queue](SOUND_QUEUE.md)
  and [player controller](SECONDARY_PLAYER_CONTROLLER.md).
- The secondary-draw default factory at `0x455D80` is newly
  **105/105 strict exact** under its real initializer and append-publisher
  producer. Two positioned secondary-draw append factories at `0x455E60`
  and `0x455F00` now have full C++ with **151/151** and **167/167**
  diagnostics; each remains two stack-ABI selector bytes away from exact.
  See [emitter factory evidence](SECONDARY_EMITTER_RESET.md).
- Player animation-pair reset `0x42F580` is **286/286 exact**;
  the player dialogue query `0x42F540` is **27/27 exact**;
  two array element constructors are **3/3 exact** each.
  See [player lifecycle](SECONDARY_PLAYER_INITIALIZATION.md).

## Highest-value next work — substantial functions and leaves

| Native owner / current source | Current scoped result | Next concrete question |
| --- | --- | --- |
| **77-byte** `CreateSecondaryManager` `0x45DB90` — `SecondaryProgramManagerFactory.cpp` | **77/77 strict exact**; 3 reviewed fields, native VM stack-zero order | Accepted cold PE/MAP/PDB proof; **28** affected shared-header/browser exact units replayed with zero differences. [Browser](SECONDARY_PROGRAM_BROWSER.md) |
| **277-byte** `LoadSecondaryProgramSelection` `0x438910` and **8-byte** callback `0x438900` | Full real-resource `/GL` **277/277 bytes, 81/81 instructions, 8 calls**, 18 nonfield differences at `+0x9F..+0xA2` (vptr/push ordering) and `+0xDB..+0xF8` (index/cookie registers); thread **8/8 strict exact** | Six constructor spelling variants failed; fix genuine constructor scheduling and count ABI without touching the real resource loader. [Browser](SECONDARY_PROGRAM_BROWSER.md) |
| **5-byte** `SecondaryProgramBrowserUpdateCallback` `0x4390A0` | Newly inventoried real registered update-chain entry; **5/5 strict exact**, one REL32 tail JMP to 934-byte browser update | Protected by cold PE/MAP/PDB owner proof; related six-byte getter also rechecked exact. [Browser](SECONDARY_PROGRAM_BROWSER.md) |
| **548-byte** `DrawSecondaryProgramBrowser` `0x438E70`, **10-byte** adapter `0x4390B0`, **6-byte** getter `0x4390D0` | **548/548 strict exact boss**, 130/130 instructions and 11/11 calls, with real ASCII support and scoped XYZ locals; all 43 original fields reviewed. The 10-byte adapter and six-byte getter are separately exact. | Protect all three exact units; continue the 934-byte update and 439-byte destruction bosses. [Browser](SECONDARY_PROGRAM_BROWSER.md) |
| **439-byte** `DestroySecondaryProgramBrowser` `0x438710` | Complete source with scoped worker destructor; escaping-stdcall `/GL` caller **448/439 bytes, 118/116 instructions, 16/16 calls**; SEH entry and RET4 now restored, NON-EXACT | Reconstruct native program/ANM cleanup inlining and register scheduling; 142/173-byte neighbors remain exact. [Browser](SECONDARY_PROGRAM_BROWSER.md) |
| **142-byte** `ConstructSecondaryProgramBrowserStorage` `0x4385D0` | **142/142 strict exact**, 32 instructions, one memset call, three reviewed fields; sets worker/selection fields, clears 0x2E4 browser and publishes singleton | Keep original worker-vtable pointer as observed external data. [Browser](SECONDARY_PROGRAM_BROWSER.md) |
| **173-byte** `RegisterSecondaryProgramBrowserChains` `0x438660` | **173/173 strict exact**, 60 instructions, four calls, six reviewed fields. Dynamic-priority generic chain caller restores natural EBP/EBX/ESI owner ABI and two ordered node-field zero stores. | Keep proof with the independently exact 10-byte draw callback; pursue larger 548/934-byte owners. [Browser](SECONDARY_PROGRAM_BROWSER.md) |
| **56-byte** `StartSecondaryDebugThread` `0x4594A0` | **56/56 strict exact**, 19 instructions, two reviewed calls, recovered private EAX/EDI ABI via true 101-byte stop-thread source and dynamic callback | Link native start and mode producers to improve 934-byte browser; preserve exact worker stop. [Browser](SECONDARY_PROGRAM_BROWSER.md) |
| **30-byte** `SetSecondaryGameplayRuntimeMode` `0x40DEA0` | **30/30 strict exact**, six instructions, two RET, zero relocations. Generic dynamic runtime/mode caller preserves original EAX/ECX ABI | Real dependency of ECL browser state-2 mode transition; use tracked generic producer, no ABI shim. [Browser](SECONDARY_PROGRAM_BROWSER.md) |
| **105/112-byte** front-secondary draw factories `0x455FB0/0x456020` + **118-byte** publisher `0x455860` | Both factories **strict exact, 217/217 bytes** and 22 reviewed fields; publisher still 111/118; volatile counter regressed to 119 bytes in eight scoped probes | Preserve factory proofs; recover native separate counter CMP after ADD and original source register lifetime. [Emitter factories](SECONDARY_EMITTER_RESET.md) |
| **105-byte** `CreateDefaultSecondaryDrawAnimationVmId` `0x455D80`, **151/167-byte** positioned append pair `0x455E60/0x455F00` | Default factory **105/105 strict exact** with 11 fields; both positioned pairs complete source with 2 stack-slot bytes different, non-exact | Protect default 105-byte proof; recover genuine hidden result/file argument order before promoting positioned pair. [Emitter factories](SECONDARY_EMITTER_RESET.md) |
| **934-byte** `UpdateSecondaryProgramBrowser` `0x438AC0` + **83-byte** cleanup `0x438570` | Native selection count reload; state-4 independent manager deletion test recovered. Scoped true mode/worker source `/GL` **928/934 bytes, 262/267 instructions, 22/22 calls, 173 nonfield mismatches**, still non-exact; exact cleanup 83/83 | Resolve input/register scheduling and state0 initialization. [Browser](SECONDARY_PROGRAM_BROWSER.md) |
| **976-byte** `BuildBillboardAnimationVmQuad` `0x450700` — `AnimationBillboardQuad.cpp` | Full C++ now source-present; linked real billboard producer **927/976**, normal COFF 939 | Paired x87 `FSINCOS` and float spill/rounding lifetime; non-exact. [Billboard](ANIMATION_PROJECTED_QUAD.md) |
| **313-byte** `CopyAnimationTextureSurface` `0x454EC0` + **48-byte** `FlushScreenshotResources` `0x445460` | Both source-present; real parent preserves native private ECX/EAX. New short-lived texture-table reload **311/313 bytes, 112/113 instructions, 8/8 calls, 46 positional nonfield differences** (previous 55); parent **58/48** | Restore final dirty-texture index register/codegen and parent scan. Use `scripts/probe-animation-surface-copy.py`, not full replay. [Surface queue](ANIMATION_SURFACE_COPY.md) |
| **571-byte** `BuildRotatedAnimationVmQuad` `0x4503D0` — `AnimationVmRotation.cpp` | Natural source added; VC9 COFF **609/571** with separate sin/cos calls, `/GL` compiles | Restore native paired `FSINCOS` and owner/register context. [VM draw](ANIMATION_VM_DRAW.md) |
| **221-byte** `LoadAnimationChunkSource` `0x4543E0` — `AnimationChunk.cpp` | With real `BeginLoadAnimationDocument` producer: **221 bytes, six calls, two RET4**; 12 reviewed address fields; only two non-field SIB selector bytes at `0xB3/0xBD` differ | Natural ECX/EDX array-address codegen. Earlier pointer/reference trials regressed to 212 bytes; eight later ordinary array/offset variants preserve 221 but retain both SIB differences. [Chunks](ANIMATION_CHUNKS.md) |
| **212-byte** `AdvanceAnimationDocument` `0x4544C0` — `AnimationChunk.cpp` | **212/212 strict exact**, 74 instructions, 2 calls, 3 reviewed fields. Real 811-byte decoder TU and 135-byte slot scan caller recover native EDI ABI; 32-bit decoded flag, sprite-before-script order and ordinary busy comparison restore every byte | Protect this exact boss, 135-byte exact scan, 811-byte exact decoder and 51-byte exact release together. [Chunks](ANIMATION_CHUNKS.md) |
| **92-byte** `ClosePauseContinueMenu` `0x42D7F0` | **92/92 strict exact**, 22 instructions, two calls, four reviewed fields. Real signal+VM finder source preserves native ESI menu receiver | Independently accepted cold PE/MAP/PDB proof; 3192-byte parent and 64-byte animation-tree leaf remain separately open. [Continue](PAUSE_CONTINUE_MENU.md) |
| **64-byte** `ShowPauseMenuAnimationTree` `0x456610` | **64/64 strict exact**, 20 instructions and two reviewed fields; generic StageMesh caller restores native EAX/EDX ABI and the owned six-byte loop-alignment LEA | Independently cold accepted with 92-byte menu close; preserve exact leaves while improving 3192-byte caller. [Continue](PAUSE_CONTINUE_MENU.md) |
| **3192-byte** `UpdatePauseContinueMenu` `0x42D9D0` — `PauseContinueMenu.cpp` | 63-support `/GL` **3172/3192**; 88/88 calls, 15/15 returns; stack `0x54` vs native `0x58` | State 13 factory-result lifetime and private ABI. Private helper `.analysis/build-pause-continue-menu.py`. [Continue](PAUSE_CONTINUE_MENU.md) |
| **3208-byte** `UpdatePauseGameOverMenu` `0x42E6B0` — `PauseGameOverMenu.cpp` | Sound-support `/GL` **3228/3208**, 88 calls, 14 returns, seven table cells | State 21 factory/result and state 25 record-name editor. [Game over](PAUSE_GAME_OVER_MENU.md) |
| **3212-byte** `UpdatePauseMenu` `0x42C8D0` — `PauseMenu.cpp` | Older `/GL` ~3084 bytes; seven child-result slots differ | Full owner/stack/child finder. [Pause](PAUSE_MENU.md) |
| **1557-byte** `InitializeSecondaryPlayerController` `0x42F8A0` — `SecondaryPlayerInitialization.cpp` | Narrow `/GL` **1479 bytes**, 13 calls, two returns, 11/12 native FADD orders | Factory/float/EH register allocation. [Player](SECONDARY_PLAYER_INITIALIZATION.md) |
| **415-byte** player destructor `0x430010` and **272-byte** constructor `0x42F760` — `SecondaryPlayerStorageLifecycle.cpp` | Destructor **435/415**, 12 native calls, one RET4, three direct frees; constructor **278/272**, five calls, one RET4 | Destructor EH/locks; constructor array/VM ABI. 33-byte vector iterator has source but is **non-exact**. [Player](SECONDARY_PLAYER_INITIALIZATION.md) |
| Replay writer **1856 bytes** `0x436420` / compressor **795 bytes** `0x4423A0` | Writer focused `/GL` **1765**, 46 calls; compressor focused `/GL` **760** | Original file/segment ownership and dictionary ABI. [Replay](REPLAY_WRITING.md) |

Newly accepted renderer-priority callbacks: 27 exact owners, 780 bytes
(22 simple callbacks + 5 stateful camera/device callbacks). Both linked
families were cold-replayed individually, not the whole cohort. See
`AnimationRendererPriority.cpp` and `AnimationRendererStatePriority.cpp`.

The newly mapped billboard (976) and renderer surface-copy pair (313+48)
are **source-only** progress and add no exact credit. Their producer and
diagnostic evidence are in the linked subsystem notes.

Other protected exact leaves: replay dictionary reset `0x4428D0`
(52 bytes), predecessor `0x442BE0` (49 bytes), pause overlay queue
`0x42F400` (103 bytes), registered menu update callback
`0x439F10` (seven bytes). Their callers are not automatically exact.

**Keep diagnostic contexts separate.** The real producer context recovers
the 221-byte loader but changes the neighboring decoder to a distinct
**827-byte** diagnostic; this does not invalidate the separately
cold-attested **811-byte** unit. Likewise the 135-byte scanner is
**non-exact** despite complete length/call/return agreement.

## Focused verification commands

```bash
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check

# Strict, receipt-bound linked Oracle on one affected unit:
scripts/repo-python scripts/build-match-unit.py --unit decode-animation-chunk
scripts/repo-python scripts/compare-linked-function.py --unit decode-animation-chunk

# COFF exact leaves: compare-coff-function.py --unit NAME --json
# Non-exact diagnostics: use compile-probe.sh and compare-coff-function.py.
```

Use direct `scripts/repo-python scripts/ghidra.py query ... review ADDRESS`
when additional native evidence is required. Serialize Ghidra and Wine.
For prose-only cleanup, the source/Oracle cohort need not be replayed.

## Artifact and storage notes

`build/probe-ltcg/` consists of **regenerable diagnostic experiments**.
Retain the costly continuation-menu build and a small set of useful
current owner contexts; historical variant images can be recreated
from `.analysis/` sources/logs and `probes/` recipes. The accepted
`build/match-linked/` and `build/match/` receipt-bearing caches are
preserved (rebuild an affected unit before trusting its cache).

Never delete `resources/th11.exe`, `.tools/msvc900`,
`.tools/wineprefix-msvc900`, the Ghidra project, unfinished work, or
useful `.analysis/` evidence. Do not commit private binaries or
generated compiler outputs. Local commits may be ahead of
`origin/main`; pushing is not part of this handoff.
