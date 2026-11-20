# TH11 reconstruction handoff — 2026-11-20

## Start here

The source baseline was verified on clean `main` at **`9accb5a`**
(`gpt-web: express stage VM allocation with owned type size`). Subsequent
handoff commits correct stale documentation/ledger notes and prune **ignored
regenerable caches**, not game C/C++ source. Run `git status --short --branch`
and `git log -8 --oneline` before continuing; any new dirty changes are the
current agent's work to integrate, not a reason to skip a file.

Work against **Japanese TH11 1.00a only**: private `resources/th11.exe`,
688,128 bytes, SHA-256
`2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`.
The code-generation target is **VC9 RTM build 21022**; retain `/GL` in
LTCG owner contexts. Use the repository shell, `scripts/repo-python`, and
`scripts/ghidra.py` for direct Ghidra analysis.

## Canonical local progress

[PROGRESS](PROGRESS.md), generated from the ledgers, reports **2,175** tracked
function candidates, **1,610** reviewed boundaries, **565** needing boundary
review, **819** source mappings and **130** confirmed authored functions.
It records **487 canonical exact functions / 54,214 exact code bytes**; **68**
exact functions / **15,260 bytes** have confirmed authored origin. These
numbers are *local exact-codegen ledger claims*, not native game closure or
Factory-accepted Oracle coverage. Rerun `scripts/repo-python scripts/progress.py`
only when canonical ledger status actually changes.

Recently relevant selected exact owners include:

| Function (target address) | Exact extent and proof lane |
| --- | --- |
| `InitializeStageResource` (`0x4024C0`) | 630/630 B; strict linked-PE `/GL` owner and reviewed fields |
| `SeekStageScriptMarker` (`0x404F20`) | 113/113 B; normal COFF, two relocations |
| `BeginPrimaryStageTransition` (`0x4051D0`) | 103/103 B; normal COFF, three relocations |
| `StepStageVectorTween` (`0x405240`) | 883/883 B; linked PE, four relocations |
| `StepStageFogTween` (`0x4055D0`) | 737/737 B; linked PE, sixteen relocations |

The stage marker, transition and both tween owners passed a **selected four-unit
cold replay** in the retained `.analysis/` logs. The stage initializer also
passed an independent **one-unit** replay after the two natural C++ typed
layout refinements. The exact status of other owners is in
`config/matches.csv`; a source mapping or byte-length match is not a proof.

## Large functions still in progress

These are **nonexact**, measured on their focused natural-C++ VC9 frontiers.
Instruction-key alignment is diagnostic only:

| Owner | Native / candidate | Matched keys | Next concrete issue |
| --- | --- | ---: | --- |
| `ExecuteStageScript` (`0x404470`) | 2,736 / 2,696 B | 581/610 | Opcode 8 packed-color codegen, fog/tween return buffers, oscillator x87 scheduling, EH/funclet ownership |
| `CullStageObject` (`0x403950`) | 1,643 / 1,648 B | 361/504 | Private EAX camera argument, projected-point x87/local register schedule |
| `DrawStageLayer` (`0x404190`) | 727 / 731 B | 127/191 | X/Y quotient stack spill, private stage/culling call convention, four-byte extent gap |
| `InitializePrimaryStageScripts` (`0x405030`) | 249 / 246 B | 68/78 | Early callee-saved pushes, stack layout and final store/return scheduling |
| `LoadStageResource` (`0x403FC0`) | 297 / 297 B | 82/99 | Filename-call and size-register scheduling, section pointer and VM-count register assignment |
| `UpdateSecondarySpellStageState` (`0x40C0A0`) | 624 / 624 B | 161/167 | Signed modulus spill, event-3 loop entry and Z x87 load/add order |
| `InitializeSecondaryDebugHudAnimations` (`0x41A6D0`) | 1,619 / 1,619 B | 385/402 | Initial return buffer, frame reservation, zero-ID wrap and retained animation scheduling |
| `SecondaryEclDebugOwner::~SecondaryEclDebugOwner` (`0x40D320`) | 462 / 462 B | 126/127 | One compiler-generated EH state initializer (`1` native vs `0` candidate) |
| `InitializeSecondarySpellStage` (`0x40C650`) | 1,777 B native | diagnostic | Real caller uses EAX spell-name pointer and two stack integers, not three stack arguments |

Read [STAGE_SCRIPT](STAGE_SCRIPT.md), [STAGE_CULLING](STAGE_CULLING.md),
[STAGE_RESOURCE](STAGE_RESOURCE.md), [STAGE_TWEEN](STAGE_TWEEN.md),
[SECONDARY_SPELL_STAGE](SECONDARY_SPELL_STAGE.md) and
[SECONDARY_DEBUG_LIFECYCLE](SECONDARY_DEBUG_LIFECYCLE.md) for native
observations, original owner extents and failed code-generation trials.

**Important known traps:** `ExecuteStageScript` owns two internal switch tables
(100 B) within its complete 2,736-byte target extent. Do not crop either or
confuse compiler-generated EH handlers with the public function body. The
large initializer's linked 246-byte result is newer than its historical
237-byte normal-COFF diagnostic. The spell updater's `scaled.z + position.z`
C++ expression still emits the *wrong x87 operand load order*; multiple
mathematically equivalent C++ trials did not resolve that difference.
Do not promote these functions based on instruction alignment or size alone.

## Efficient continuation

Keep large Boss functions and exact-capable leaves in the same work stream.
Start with the full original owner plus call/reference review, change natural
C/C++, then build/compare **only the affected unit**, its owned extent and
relocation fields. Cold-replay only directly affected canonical exact units.
Do not run a full cold replay. Successful checkpoints should be saved as
`gpt-web: ...` commits; retain meaningful failed experiments in prose/logs.

Useful current scoped entry points:

```bash
scripts/repo-python scripts/build-match-unit.py --unit execute-stage-script-frontier
scripts/repo-python .analysis/compare-stage-script-frontier-20261118.py
scripts/repo-python scripts/build-match-unit.py --unit cull-stage-object-frontier
scripts/repo-python .analysis/compare-cull-stage-object-20261118.py
scripts/repo-python scripts/build-match-unit.py --unit draw-stage-layer-frontier
scripts/repo-python .analysis/compare-draw-stage-layer-20261118.py
scripts/repo-python scripts/build-match-unit.py --unit initialize-primary-stage-scripts-frontier
scripts/repo-python .analysis/compare-stage-init-current-20261119.py
```

These `.analysis/` helpers and review logs are **local ignored evidence**, not
tracked source. The retained logs include
`.analysis/stage-script-final-current-20261120.log`,
`.analysis/draw-stage-layer-final-current-20261120.log` and
`.analysis/cull-stage-final-current-20261120.log`. Verify any exact claim with
the strict unit replay command before updating `config/matches.csv`.

## Storage and current workspace

On 2026-11-20, **510 ignored cache files** were removed, saving about
**10.4 MiB** by before/after directory measurements. This included old trial
PE/PDB/MAP snapshots, source-sweep objects, one rotatable Ghidra application
log and Python bytecode. Trial `source.cpp`, `comparison.log`, referenced
machine-code snapshots, canonical exact output families, all active Boss
frontiers, target, tools, Wine prefix and Ghidra project were preserved.
See [ARTIFACTS](ARTIFACTS.md) and the ignored local inventory
`.analysis/handoff-storage-cleanup-20261120.txt`. Rebuild a missing historical
scratch binary before investigating it; do not use its old receipt as a current
exactness proof. No compilation or full replay is required just to edit docs.
