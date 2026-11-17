# TH11 reconstruction — agent handoff (2026-11-17)

## Resume safely

Work only against the original Japanese **TH11 v1.00a**, private
`resources/th11.exe`, SHA-256
`2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`.
The pinned toolchain is **VC9 / Visual C++ 2008 RTM build 21022**. Use
direct `scripts/ghidra.py` and the repository's VC9 build/oracle scripts,
not TH10 codegen or another game binary. Read `AGENTS.md`, inspect
`git status --short --branch`, and continue any dirty changes rather
than discarding them. Use natural C/C++, complete functions, bounded
source-unit builds and `gpt-web: ...` commits; no ASM or padding.

**Canonical state** is in [PROGRESS](PROGRESS.md), generated from
`config/functions.csv`, `config/matches.csv` and the other ledgers.
At this handoff it reports **2,175 tracked candidates, 808 source mappings,
469 canonical exact functions**. These figures are a snapshot; **do not
maintain another count here**. A source mapping, size match or diagnostic
instruction alignment does not confer exactness.

## Current priority — HUD lifecycle and animation owners

The active work is around `SecondaryDebugHudOwned` (singleton at
**0x4A8D84**, native allocation **0x4458**). Its physical view in
`src/SecondaryDebugHudLifetime.hpp` owns **16 real
`StageEmbeddedVm` members**: arrays of 9/2/4 and a separate VM
at +0x3F1C, plus the real HUD timers at +0x43C8 and +0x4424.
Do not return to the obsolete 15-VM / 0x4450 view.

| Native owner | Source / current state | Useful evidence |
| --- | --- | --- |
| `41A2F0` constructor, 208 B | **exact** | `secondary-debug-hud-constructor-frontier`, 208/208 |
| `41B2A0` factory, 150 B | **exact** | `secondary-debug-hud-factory-frontier`, 150/150 |
| `419F80` dialogue VM release, 101 B | **exact** | `secondary-debug-hud-dialogue-frontier`, eight checked REL32 |
| `41A3C0` three-chain registration, 339 B | **exact** | `secondary-debug-hud-registration`; 339/339 and 15 verified fields |
| `41A520` visual/resource initialization, 336 B | natural source, nonexact | `src/SecondaryDebugHudVisualInitialization.cpp`; 336-byte scoped candidate, timer scheduling gap |
| `41AD30` pre-destruction cleanup, 468 B | natural source, nonexact | `src/SecondaryDebugHudPrecleanup.cpp`; candidate 445 B / 62 of 128 |
| `41AFC0` destructor, 729 B | natural source, nonexact | `src/SecondaryDebugHudLifetime.cpp`; 741-byte /GL candidate, 195 decoded instructions |
| `41AF30` cleanup helper, 129 B | **exact** | `secondary-debug-hud-finalization`; 129/129 and eight audited fields |
| `41B360` global destruction, 28 B | **exact** | `secondary-debug-hud-global-destroy`; 28/28 and three audited fields |
| `41A6D0` HUD initialization dispatch, 1,619 B | natural source, nonexact | `src/SecondaryDebugHudAnimationInitialization.cpp`; 1,619-byte /GL candidate / 403 instructions (native 402) |

The candidate figures are **historical scoped /GL diagnostics**, not
freshly reverified exact claims. Build only the relevant frontier unit
from `config/match-units.toml` to renew one; see
[secondary debug lifecycle](SECONDARY_DEBUG_LIFECYCLE.md).
Registration is now exact: a genuine independent generic-priority
and document-slot C++ consumer recovered the native private registers.
The 729-byte destructor and 336-byte resource initializer remain open. The 729-byte destructor still needs exact codegen: countdown ID cleanup,
one captured marker and a generic precleanup caller brought its candidate
from 772/200 to 741/195 (bytes/instructions), versus native 729/195.
Its genuine 16-VM member destruction remains intact.

**Next useful work:** refine the full 1,619-byte `41A6D0` HUD animation
initialization against its now same-length 1,619-byte /GL candidate
(403 versus 402 instructions), then close
the 729-byte destructor and 336/339-byte resource/registration owners.
The 129-byte independent cleanup and 28-byte global destroy entries
are now strictly exact. Both preserve their original natural C++
object/resource ownership and complete function extents. Build only affected units.
The 101/150/208-byte exact owners provide direct regression checks
when their common headers/producer inputs change.

## Other large open frontiers

- `ExecuteStageScript` at **404470**, 2,736 native bytes:
  `execute-stage-script-frontier` previously generated 2,728 B /
  531 of 610 normalized native instruction keys. Its generated
  switch tables and EH/callsite scheduling remain open; see
  [STAGE_SCRIPT](STAGE_SCRIPT.md).
- `StepStageVectorTween` at **405240**, 883 B:
  `step-stage-vector-tween` generated 883 B / 272 of 280 keys.
  The Hermite x87 multiplication order remains open; see
  [STAGE_TWEEN](STAGE_TWEEN.md).
- `CullStageObject` **403950** (1,643 B) and `DrawStageLayer`
  **404190** (727 B) have natural source with 12 projected points,
  local record offset snapshots and Fog/Z transitions, but are
  nonexact; see [STAGE_CULLING](STAGE_CULLING.md).
- `InitializeSecondarySpellStage` **40C650** (1,777 B),
  `UpdateSecondarySpellStageState` **40C0A0** (624 B),
  and their timing/HUD owners have maintained source, mostly nonexact.
  The **317-byte destructor** at 40BE50, **206-byte factory**
  at 40BF90 and **256-byte registrar** at 40BD00 have their own
  canonical exact proofs; see [SECONDARY_SPELL_STAGE](SECONDARY_SPELL_STAGE.md).
- The auxiliary stage singleton is **`g_MeshEffectEnemy` at
  0x4A8D64**, not the player pointer **`g_MeshEffectActor` at
  0x4A8EB4**. Preserve that corrected owner distinction; see
  [STAGE_AUXILIARY](STAGE_AUXILIARY.md).

## Proof and storage boundaries

For source changes, run a focused build and then cold-replay **only
the affected canonical units and real dependent owners**:

```bash
scripts/repo-python scripts/build-match-unit.py --unit secondary-debug-hud-destructor-frontier
scripts/repo-python scripts/replay-exact-units.py --unit secondary-debug-hud-constructor-frontier
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/progress.py
git diff --check
```

The linked Oracle checks **complete public primary-TU PDB function
contributions** and exhaustive relocations. It **does support**
an owner-qualified, compiler-generated `eh_static = true` DIR32
handler symbol from the MAP's Static symbols section; this proves
the referencing function body, **not the handler code as a separate
function**. Standalone static owners, ambiguous/split PDB owners and
whole-game closure remain open. See [LINKED_ORACLE](LINKED_ORACLE.md).

The private original target, pinned VC9 files, Wine prefix, TH11
Ghidra project, source/probes, match manifests and canonical exact
caches are retained. Some ignored **regenerable diagnostic** outputs
were pruned at handoff; do not cite a removed old PE/PDB as a current
comparison. Rebuild the unit to obtain a fresh receipt. See
[ARTIFACTS](ARTIFACTS.md) for the current retention/cleanup policy.
