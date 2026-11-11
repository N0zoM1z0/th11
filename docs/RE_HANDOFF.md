# TH11 reconstruction — agent handoff (2026-11-11)

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
  At this checkpoint: **277 canonical exact**, **567 source-present**,
  **652 reviewed boundaries**. Refresh with
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
- Player animation-pair reset `0x42F580` is **286/286 exact**;
  the player dialogue query `0x42F540` is **27/27 exact**;
  two array element constructors are **3/3 exact** each.
  See [player lifecycle](SECONDARY_PLAYER_INITIALIZATION.md).

## Highest-value next work — substantial functions and leaves

| Native owner / current source | Current scoped result | Next concrete question |
| --- | --- | --- |
| **934-byte** `UpdateSecondaryProgramBrowser` `0x438AC0` + **83-byte** cleanup `0x438570` — `SecondaryProgramBrowser.cpp` | Both have new natural C++ with exact physical state/view offsets; focused `/GL` **859/934** browser, native **22/22 calls**, cleanup **87/83** | Restore dynamic selection-count codegen and native cleanup ESI ABI. [Browser](SECONDARY_PROGRAM_BROWSER.md) |
| **976-byte** `BuildBillboardAnimationVmQuad` `0x450700` — `AnimationBillboardQuad.cpp` | Full C++ now source-present; linked real billboard producer **927/976**, normal COFF 939 | Paired x87 `FSINCOS` and float spill/rounding lifetime; non-exact. [Billboard](ANIMATION_PROJECTED_QUAD.md) |
| **313-byte** `CopyAnimationTextureSurface` `0x454EC0` + **48-byte** `FlushScreenshotResources` `0x445460` | Both now source-present; real parent recovers child private EAX/ECX, focused **312/313** child and **58/48** parent | Backbuffer-to-texture copy direction corrected; RECT stack slots and last texture reload remain non-exact. [Surface queue](ANIMATION_SURFACE_COPY.md) |
| **571-byte** `BuildRotatedAnimationVmQuad` `0x4503D0` — `AnimationVmRotation.cpp` | Natural source added; VC9 COFF **609/571** with separate sin/cos calls, `/GL` compiles | Restore native paired `FSINCOS` and owner/register context. [VM draw](ANIMATION_VM_DRAW.md) |
| **221-byte** `LoadAnimationChunkSource` `0x4543E0` — `AnimationChunk.cpp` | With real `BeginLoadAnimationDocument` producer: **221 bytes, six calls, two RET4**; 12 reviewed address fields; only two non-field SIB selector bytes at `0xB3/0xBD` differ | Natural ECX/EDX array-address codegen. Earlier pointer/reference trials regressed to 212 bytes; eight later ordinary array/offset variants preserve 221 but retain both SIB differences. [Chunks](ANIMATION_CHUNKS.md) |
| **212-byte** `AdvanceAnimationDocument` `0x4544C0` — `AnimationChunk.cpp` | Narrow decoder context: **211 bytes**; native file input uses EDI | Resolve native private register ABI; the dependent **135-byte** `UpdateAnimationDocumentSlots` at `0x4548D0` now links to **135 bytes, three calls, two returns**, with only **one non-field byte at 0x70** (EAX vs EDI). Byte-to-int decoded-state trials regressed. [Slots](ANIMATION_DOCUMENT_SLOTS.md) |
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
