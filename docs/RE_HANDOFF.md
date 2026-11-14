# TH11 reconstruction — next-agent handoff (2026-11-14)

**Start here, then continue the code.** This repository reconstructs only the
original Japanese TH11 v1.00a executable at `resources/th11.exe` (ignored),
SHA-256 `2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`.
The compiler candidate is VC9 / Visual C++ 2008 RTM build 21022. Preserve
`/GL` in established LTCG owners. Do not substitute `th11c.exe`, TH10 codegen,
or a different compiler.

## Checkpoint and current inventory

Start with:

```bash
git status --short --branch
git log -8 --oneline
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
```

The current branch is `main`; recent local commits use the required
`gpt-web: ...` prefix. The latest source/config checkpoint is `17ebdf8`; this
handoff and artifact-policy refresh is the documentation checkpoint immediately
above it. Recent code checkpoints include:

- `17ebdf8` — include the real pair timer in the host LTCG context;
- `1257ea0` — stage the native three-pointer selection for pair tween `0x12D/0x12F`;
- `0e2ba3d` — reconstruct eight exact secondary pair-access leaves;
- `fb5d414` — tighten secondary-host motion staging;
- `210431b` — reconstruct exact replay record factories;
- `654f5fc` — reconstruct the secondary statistics writer.

`docs/PROGRESS.md` is the generated inventory authority. At this checkpoint it
reports 2,157 tracked candidates, 761 reviewed boundaries, 735 source-present
mappings, 389 canonical exact functions/42,616 exact bytes, and 90 confirmed
authored functions. The denominator remains provisional; do not edit generated
counts by hand.

## Current priority: complete secondary actor dispatcher

The native host owner starts at `0x412E30`, spans 16,556 bytes including dispatch
data, and contains15,780 bytes of executable code. `src/SecondaryActorInstructions.cpp`
now provides the complete state member and manager adapter; the four included
fragments cover all154 handled opcode values (`0x100..0x1C2`). The complete
source and current compiler measurements are documented in
[SECONDARY_HOST_DISPATCH](SECONDARY_HOST_DISPATCH.md).

The canonical VC9 `/GL` family includes the real VM readers, `SecondaryScript.cpp`,
host argument adapters, actor creation/lifetime/animation/update, and
`SecondaryPairAccess.cpp`. The last TU is important: native
`ResetSecondaryPairTimer` (`0x419520`, 52 bytes) receives the pair pointer in EAX.
Including the real body restores the no-push register call at all pair-tween call
sites. The eight pair-access leaves total 130 bytes and are strictly exact; see
[SECONDARY_PAIR_MATH](SECONDARY_PAIR_MATH.md).

Current canonical owner measurement:

| Measure | Native | Candidate |
| --- | ---: | ---: |
| Executable code | 15,780 B | 15,272 B |
| PDB contribution including dispatch data | 16,556 B | 16,048 B |
| Stack allocation | `0x310` | `0x328` |
| Direct calls | 465 | 459 |
| Indirect callbacks | 2 | 2 |

The complete host is **not exact**. The separate11-byte manager adapter at
`0x412E20` is exact; its fresh selected cold replay passed its sole REL32 field
with zero differences. Do not promote the full host from that adapter result.

Largest current entry-interval differences (native -> candidate):

| Opcode | Native | Candidate | Delta |
| --- | ---: | ---: | ---: |
| `0x105` | 82 B | 196 B | +114 B |
| `0x1A7` | 136 B | 216 B | +80 B |
| `0x1AA` | 118 B | 171 B | +53 B |
| `0x101` | 133 B | 185 B | +52 B |
| `0x15E`, `0x161` (each) | 121 B | 170 B | +49 B |
| `0x12D` / `0x12F` | 790 B | 812 B | +22 B |
| `0x100` | 392 B | 268 B | -124 B |
| `0x104` | 222 B | 100 B | -122 B |

Start with the large positive deltas and explain the unexpectedly short `0x100`
and `0x104` intervals. For `0x12D/0x12F`, motion and all three tween pointers
are now selected before the six real reads; the interval is212/216 instructions
with 13 calls. `0x121/0x123` is 473/509 bytes, 127/138 instructions and 9 calls.
The pair reset calls now use the native no-push EAX form, but field scheduling,
x87 predicates, register preservation and the0x328 frame remain open.

The current `RunSecondaryScript` context is 6,989/7,093 bytes with the native
0x120 frame, but its VM receiver is ESI rather than native EDI; it is also
non-exact. Do not infer interpreter exactness from the host's source coverage.

Useful focused commands (no full cold replay):

```bash
scripts/repo-python scripts/build-match-unit.py --check
scripts/repo-python scripts/build-match-unit.py --unit secondary-actor-instruction-adapter
scripts/repo-python scripts/replay-exact-units.py --unit secondary-actor-instruction-adapter
```

After source changes, build and replay only the changed exact unit and direct
exact consumers. Compare complete affected owners/fields; never crop an extent.
The host diagnostic link is useful for register/codegen analysis, not an exact
acceptance lane.

## Other substantial open owners

These are still non-exact; consult each linked note for its latest complete
measurements and retained experiments before editing:

| Native owner | Current checkpoint | Detail |
| --- | --- | --- |
| `0x4356E0` secondary replay initializer (1,380 B) | 1,247/1,380 diagnostic; 22 calls/four returns preserved | [SECONDARY_PATTERN](SECONDARY_PATTERN.md) |
| `0x42D9D0` continue menu (3,192 B) | 3,184/3,192; 162 fields, 88 calls, 15 RET4 paths, 2,008 non-address differences | [PAUSE_CONTINUE_MENU](PAUSE_CONTINUE_MENU.md) |
| `0x42E6B0` game-over menu (3,208 B) | retained 3,212-byte candidate; 154 fields, 88 calls, 14 RET4 paths, 2,040 non-address differences | [PAUSE_GAME_OVER_MENU](PAUSE_GAME_OVER_MENU.md) |
| `0x4396C0` title-menu destructor (561 B) | 561-byte candidate; 231 non-address differences | [TITLE_MENU_LIFECYCLE](TITLE_MENU_LIFECYCLE.md) |
| `0x436420` replay writer (1,856 B) | source-present, non-exact | [REPLAY_WRITING](REPLAY_WRITING.md) |

Large owners and their important leaves both deserve real work; do not spend a
whole session only on small helpers. Keep source natural and ABI evidence-based.
Never add target bytes, padding, fake returns, guessed register declarations,
or volatile accesses just to move a diff count.

## Evidence, tools and storage

Use direct Ghidra through `scripts/ghidra.py`, not REA. Preserve the original,
`.tools/` compiler/Wine state, `ghidra-project/`, canonical `build/match/` and
`build/match-linked/` receipts, source and useful `.analysis/` Ghidra reviews.
The 2026-11-14 storage pass removed only disposable `SecondaryActorHost-*`
probe links and obsolete host-switch scratch variants; it kept the canonical
owner build and the reproducible `.analysis/probe-secondary-host-with-pair-access-ltcg.py`
recipe. The remaining unrelated diagnostic contexts are not a full replay
requirement. See [ARTIFACTS](ARTIFACTS.md) for the cleanup inventory.

Native product closure, broad runtime comparisons and portability remain later
work. A successful build or local commit is a checkpoint, not an Oracle receipt.
The Factory shell has no network/push; describe commits as local checkpoints.
