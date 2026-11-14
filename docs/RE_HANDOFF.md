# TH11 reconstruction — next-agent handoff (2026-11-14)

**Start here, then continue the code.** This repository reconstructs only the
original Japanese TH11 v1.00a executable at `resources/th11.exe` (ignored),
SHA-256 `2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`,
with VC9 / Visual C++ 2008 RTM build 21022. Preserve `/GL` in established
LTCG owners; all units use `/O2 /Ob1` (a global `/Ob2` is ruled out, see the
knowledge base). Do not substitute `th11c.exe`, TH10 codegen or another
compiler.

```bash
git status --short --branch
git log -8 --oneline
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
```

Commits use the `claude-opus-5.5-high: ...` prefix (earlier history uses
`gpt-web: ...`). `docs/PROGRESS.md` is the generated inventory authority;
regenerate it with `scripts/repo-python scripts/progress.py` instead of copying
counts here.

## How recent gains were found

Read the knowledge-base section "Inline definitions and opaque COMDAT callees"
before editing. In short:

- A callee that native callers treat as an opaque thiscall (they keep `this`
  in ESI and reload ECX) is an `inline` function whose inlining was declined.
- Uncalled, unreferenced leaves are usually dead copies of inline helpers.
  They reproduce only when the helper is inline in the real owning TU and that
  TU's code uses it. Find the users by searching the native image for the
  copy's store shape or for calls to its callee.
- Native writes switch cases out in full and lets VC9 cross-jump identical
  tails; source `goto`s into shared blocks are usually wrong.

## Roadmap

Measured backlog (2026-11-14, after the CRT scan): 624 functions still have
unreviewed boundaries and origin is pending for 546. The game's own code ends
at 0x45FC2B; everything matched above it is VC9 library code. Exact credit
requires a reviewed boundary, so extend review, but tie it to reconstruction:

1. **Library classification is done** (`scripts/scan-library-code.py`;
   [KNOWLEDGE_BASE](KNOWLEDGE_BASE.md) "CRT library code"). 889 functions are
   library. About 139 CRT-region rows (12.5 KB) remain unmatched, plus 116
   library rows with split extents (mostly SEH funclets).
1a. **Rewrite the withdrawn claims in C++.** 29 exact claims had `__asm`
   bodies, asm-symbol units or volatile casts. Exact again from natural C++:
   the five LZSS tree helpers, the stage-link constructor, both draw-VM
   creators (455D80/455DF0), score insertion 42BA80 and record preparation
   42D850. Plain C++ but non-exact: compressor 166/240, writer 446/564,
   segment append 38/40, frame recorder 94/108, music thread 54/63, draw
   publisher 4557E0 (id re-test), header storage 420D80 30/40, integer
   position 435630 19/22, pair leaves, score commit. Byte-identical apart from
   fields but SEH-framed (no EH acceptance yet): record destructor 435C50,
   Ascii destructor 401210. Still asm: replay initializer/loader/stage lifecycle
   (4356E0, 436B60, 436DA0), record factory owner units (435E00/435EF0; C++
   exists in `ReplayRecordFactories.cpp`), statistics writer 437A70, pause
   deletion 435FC0, plus `__asm` blocks in the three pause menus. Private
   register conventions only appear when every caller is C++, and a probe must
   give shared callees live arguments, or LTCG folds constants into them.
2. **Inventory the uncovered gaps.** Classify each as dead inline copy,
   padding, data or thunk. Dead copies are quick exact wins with the method
   above (store-shape search for users, inline helper in the owning TU).
3. **Review and reconstruct by TU region.** Take a contiguous range next to
   existing exact units (tween, script, chain, pattern areas), review boundary
   and origin together, write its small leaves and prove them in that
   region's family. Native layout follows source order, so neighbours share
   headers, inline helpers and ownership.
4. **Close the near-misses below**, each of which has one known gap.
5. **Large owners**: the dispatcher (techniques known, 1,102 B interval
   error), then the secondary interpreter and the pause/continue menus.

Avoid a review-only sweep: it yields no exact bytes, and boundaries reviewed
far from current work tend to need revisiting once the source shape is known.

## Near-exact work to finish

| Target | State | Remaining gap | Note |
| --- | --- | --- | --- |
| Replay record destructor 435C50 | byte-identical apart from fields | SEH handler DIR32 cannot be bound by the public-symbol oracle (EH contexts open) | [KNOWLEDGE_BASE](KNOWLEDGE_BASE.md) |
| `AppendReplayWriteSegment` 437300 | 38/40 with C++ callers | segment constructor store schedule | [REPLAY_WRITING](REPLAY_WRITING.md) |
| `RecordReplayInputFrame` 435FE0 | 94/108, no unit | native loads `heldFrames[0]` into ECX instead of sharing the constant 5 | [REPLAY_WRITING](REPLAY_WRITING.md) |
| `PushSecondaryValue` 45DC30 | 20/21 instructions | native forms the slot address before the store | [SECONDARY_SCRIPT](SECONDARY_SCRIPT.md) |
| `SecondaryManager::Restart` 45DD50 | 28/28 bytes as an inline copy | name arrives in EAX, native ECX; change reverted | [SECONDARY_SCRIPT](SECONDARY_SCRIPT.md) |
| `StepStageFogTween` 4055D0 | 794/737 bytes | mode 17's second add is inlined; native calls `operator+` | [STAGE_TWEEN](STAGE_TWEEN.md) |
| `StepStageVectorTween` 405240 | 887/883 bytes, 267/280 aligned | mode-7/17 store order, add operand order | [STAGE_TWEEN](STAGE_TWEEN.md) |
| `StepAnimationIntScalarTween` 44E7C0 | 97/120 aligned | native spills `1-t` before loading 3.0 | [ANIMATION_TWEEN_DISPATCH](ANIMATION_TWEEN_DISPATCH.md) |
| `StepAnimationFloatPairTween` 44E910 | 217/221 aligned | one mode-8 multiply schedule | [ANIMATION_TWEEN_DISPATCH](ANIMATION_TWEEN_DISPATCH.md) |
| 42F340/42F350 comparisons | byte-identical as inline copies in a probe | need their real inlined uses | [PAUSE_GAME_OVER_MENU](PAUSE_GAME_OVER_MENU.md) |
| Other dead leaves (410530, 44ED70, 435400/30/60, ...) | open | find real users; list in the knowledge base | [KNOWLEDGE_BASE](KNOWLEDGE_BASE.md) |

## Actor dispatcher 412E30 (16,556 bytes)

Not exact. Candidate PDB contribution 16,276 bytes, frame `0x328` (native
`0x310`), masked alignment 1,053 instructions. Total per-opcode interval error
is 1,102 bytes (it was 1,831 at the start of 2026-11-14). Largest remaining
intervals, native → candidate:

| Opcode | Native | Candidate | Gap |
| --- | ---: | ---: | ---: |
| `0x1AA` | 118 | 171 | +53 |
| `0x1A6` | 112 | 140 | +28 |
| `0x10E` | 233 | 253 | +20 |
| `0x119`/`0x11B` | 315 | 271 | -44 |
| `0x101` | 133 | 93 | -40 |
| `0x121`/`0x123` | 509 | 475 | -34 |
| `0x199` | 181 | 149 | -32 |

Open causes: native keeps repeated identical opcode tests (motion/tween
selection) that VC9 threads into one branch here; separate `if`s spill
`opcode` for the whole function. 0x1AA's branches should cross-jump into
0x1A9's store block. Details and ruled-out variants are in
[SECONDARY_HOST_DISPATCH](SECONDARY_HOST_DISPATCH.md). Measure with:

```bash
scripts/repo-python scripts/build-match-unit.py --unit secondary-actor-instruction-adapter
scripts/repo-python scripts/replay-exact-units.py --unit secondary-actor-instruction-adapter \
  --unit initialize-secondary-hud-screen-effect
```

## Other large open owners

| Native owner | Current checkpoint | Detail |
| --- | --- | --- |
| `0x45B860` secondary interpreter (7,093 B) | 7,193-byte candidate with the inline pop; VM receiver ESI vs native EDI | [SECONDARY_SCRIPT](SECONDARY_SCRIPT.md) |
| `0x430290` player movement (2,896 B) | 2,920 bytes, 85 aligned instructions after the pair types | [SECONDARY_PATTERN](SECONDARY_PATTERN.md) |
| `0x4356E0` secondary replay initializer (1,380 B) | still an `__asm` body (exact claim withdrawn) | [SECONDARY_PATTERN](SECONDARY_PATTERN.md) |
| `0x42D9D0` continue menu (3,192 B) | 3,184/3,192, 2,008 non-address differences | [PAUSE_CONTINUE_MENU](PAUSE_CONTINUE_MENU.md) |
| `0x42E6B0` game-over menu (3,208 B) | 3,212-byte candidate, 2,040 non-address differences | [PAUSE_GAME_OVER_MENU](PAUSE_GAME_OVER_MENU.md) |
| `0x4396C0` title-menu destructor (561 B) | 561 bytes, 231 non-address differences | [TITLE_MENU_LIFECYCLE](TITLE_MENU_LIFECYCLE.md) |
| `0x436420` replay writer (1,856 B) | natural C++, 446/564 aligned; raw-buffer reload placement in the copy loop | [REPLAY_WRITING](REPLAY_WRITING.md) |

Keep source natural and ABI evidence-based. Never add target bytes, padding,
fake returns, guessed register declarations, volatile accesses or invented
callers to force emission.

## Verification and storage

After a source or header change, cold-replay every unit whose source/support
closure includes the changed files (shared headers such as `Chain.hpp`,
`SecondaryPattern.hpp` or `SecondaryScript.hpp` reach 30–220 units). A comment
edit in a support source also invalidates receipts.

`scripts/validate-tracking.py` already fails on clean HEAD at 0x00435630
(indeterminate origin without an individual reason). Local diagnostic helpers
(disassembly, masked diff, quick LTCG link) are in ignored
`.analysis/tool-*-20261114.py`; the probes cited by topic notes are kept beside
them. Preserve `.tools/`, `ghidra-project/` and canonical `build/match*/`
receipts. See [ARTIFACTS](ARTIFACTS.md) for earlier storage passes.
