# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; the outer
folder is not Git. Read AGENTS.md and relevant source/ledger rows. Current
source, receipts and target bytes outrank historical notes.

## Checked-in state

- Original Japanese TH11 v1.00a only; target identity: `config/target.toml`.
- 1,995 candidates;149 reviewed boundaries,147 source-present mappings.
- 78 exact functions /14,988 bytes:8 normal COFF and70 linked PE.
- Direct local scripts/Ghidra; no Factory MCP or REA. Pinned VC9 RTM/SDK6.0A.
- Commit prefix: `gpt-6.1-sol: `. All dirty work belongs to this effort.

This checkpoint supplies complete1,112-byte VM submission source, the537-byte
unrounded builder and three dispatch wrappers: five mappings/1,869 target
bytes. Six further origins/boundaries were reviewed: five engine utilities
remain indeterminate; the actual CRT `_frnd`20-byte member is a verified
library exclusion. See [ANIMATION_VM_DRAW.md](ANIMATION_VM_DRAW.md).

Four new full cold proofs add1,263 bytes: coordinate632, floor40, unrounded
coordinate537, and unrounded dispatch54. Real submission caller context closes
the former53 stack-only coordinate differences and naturally produces both
its16-byte frame and floor's64-byte-aligned frame. No helper source, padding,
alignment declaration or ABI trick was needed. Color25/appender142 were
replayed because their caller context changed; both remain exact.

## Current gaps

| Function | Target bytes | Current source / diagnostic differences |
| --- | ---: | --- |
| DrawAnimationVm,44F880 | 1,112 | 1,190 /858 |
| DrawUnrotatedAnimationVm,4500B0 | 51 | 49 /27 |
| DrawAdaptiveAnimationVm,450610 | 115 | 117 /60 |
| BuildRotatedAnimationVmQuad,4503D0 | 571 | Source open; complete target review |
| ApplyAnimationVmDrawState,44F710 | 368 | 348 /216 |
| DrawAsciiManager,401670 | 1,636 | 1,208 /1,358; older opaque render context |
| InitializeAsciiManager,401080 | 391 | 391 /30 |
| AsciiManagerDestructor,401210 | 475 | 433 /290; EH |
| BindAnimationSprite,44AB40 | 389 | 388 /289 |
| InitializeColumnMeshEffect,406920 | 998 | 998 /269; EH |
| ExecuteStageScript,404470 | 2,736 | 2,728; EH open |

## Concrete next work

1. Continue submission's full1,112-byte codegen. Native pixel alignment inlines
   FRNDINT; current C++ calls the existing CRT `_frnd` primitive. The primitive
   is real and ABI-reviewed: all20 bytes match locked VC9 libcmt.lib's member;
   four-mode +1.5/-1.5 checks retain RC. No floor/fixed-nearest approximation,
   invented helper or target instruction sequence was added. Exact inline
   codegen and submission frame/scheduling remain open.
2. Complete571-byte rotated source: native FSINCOS and anchored local corners,
   with selector3 leaving a local axis unwritten. Do not substitute separate
   approximate sin/cos calls or initialized fallback corners. Its private EAX
   preservation also matters to the115-byte adaptive wrapper. The51-byte
   wrapper still has a4-byte frame/scheduling gap; no inert local was added.
3. Unrounded537 needs float-rounded half sizes, with half-height computed
   before X dispatch; the initial direct-double expression was not native.
   Both exact coordinate builders retain alias-sensitive re-reads and shared
   corner ordering. Native viewport rejection uses ordered separating tests;
   positive <= chains change NaN paths. Keep the unsigned viewport additions.
4. State368 remains348/216 (shared blend-call blocks/filter/register scheduling).
   Continue larger script/tween/VM interpreter work and extend origin review.
   Original TU/data ownership, EH/static/split-owner acceptance remain open.
   Older ASCII canonical families still have opaque renderer calls; add real
   support only to answer a concrete codegen question and replay affected units.

## Evidence and checkpoints

Canonical outputs/receipts: `build/match` and `build/match-linked`.
Target, pinned tools and `ghidra-project/TH11` remain private. Latest proof:
`.analysis/vm-draw-six-cold.log` (four new/two affected units). Full current
non-exact diagnostics: `vm-draw-final-diagnostic.json`. CRT identity and ABI
check: `.analysis/frnd-library/identity.json` and `abi-check.log`. Topic docs
link full Ghidra/reference/neighbor reviews and compiler experiments. No
reconstruction/compiler job is live.

```bash
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py --require-target
```

Cold-replay changed exact units and directly affected dependencies only.
Update tracking/progress, check the diff, and save meaningful progress.
Broad behavior/failure/random/caller suites are retired. Full native product,
runtime and portability work remains later work.
