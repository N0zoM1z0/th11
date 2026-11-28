# TH11 reconstruction handoff — 2026-11-28

## Start here

Work only against original Japanese TH11 1.00a (`resources/th11.exe`, SHA-256
`2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`). The
pinned code-generation target is VC9 RTM build 21022. Keep `/GL` for established
LTCG owners and use natural C/C++ only: no inline assembly, padding, byte stubs
or cropped extents. Use the repository shell, `scripts/repo-python` and direct
Ghidra through `scripts/ghidra.py`.

## Roadmap from 2026-11-28: breadth first, large owners later

The operator changed the plan. Build out the independent, smaller functions
first, so that the framework is broadly in place (sources, types, contexts and
callees). Then come back to the large owners. Do not start with the
interpreters or menu pages listed further down.

**Where the game stood when the roadmap was set** (`config/functions.csv`; 900 library rows excluded; current totals in PROGRESS):

| State | Functions | Bytes | Share of bytes |
| --- | ---: | ---: | ---: |
| Canonical exact | 497 | 57,168 | 16% |
| Source present, not exact | 327 | 192,572 | 55% |
| No source | 451 | 99,679 | 29% |

Exact work so far is almost all small: 447 of the 497 owners are under 256
bytes. Only 60 of the 327 source-present functions have a frontier unit, so
for the other 267 nobody knows how close they are. The PROGRESS chart's
"69/130" counts exact functions among the 130 whose authored origin is
confirmed. It is an origin-review figure, not reconstruction breadth.

**Batch 1 — independent functions with no source.** Game code lies in
0x401000..0x45FFFF; the 138 no-source rows at 0x460000 and above sit among the
library exclusions (CRT names such as `entry`, `write_multi_char`). Classify
those with `scripts/scan-library-code.py` or a Ghidra review instead of writing
source. In the game range there are 313 no-source functions, 223 of them under
256 bytes. The 0x420000 block is the thinnest: 151 non-exact against 21 exact
(36 KB has no source). Work in groups of 20-30 neighbouring functions. Adjacent
addresses usually share a TU, so a group can share one source file and one
context. Take leaves and functions whose callees are already exact first.
Prefer a plain normal-COFF unit (`artifact_kind = "coff"`, e.g.
`ascii-manager-on-update`) or a small linked unit with few supports (e.g.
`advance-stage-timer`). Use a wide context only when the native ABI needs LTCG
specialization from real callers. Small new units cost little to replay and do
not touch shared headers.

**Batch 2 — near-exact sweep of existing sources.** Give the 267
source-present functions without a unit a frontier unit in the smallest
context that compiles them. Compare all of them once and rank by difference.
153 of them are under 256 bytes; collect the ones that differ by a few bytes.
Record the remaining gap in the function's doc rather than tuning it at
length.

**Batch 3 — framework gaps found along the way.** Missing callees that force
data anchors, shared types still modeled as casts or views, and the
`StageTimerView` constructor blocker below. Fix these in batches, because each
shared-header change needs a wide replay (`AnimationVm.hpp`: 295 units in 160
contexts).

**Later — large owners.** `ExecuteSecondaryActorInstruction`,
`ExecuteAnimationVm`, `RunSecondaryScript`, `RunSecondaryDialogue`, the HUD,
pause and menu pages. Their remaining gaps depend on whole-function register
allocation, x87 scheduling and callee ABIs, and they get easier as their
callees gain real source. Their state and next steps are kept below for that
phase.

Origin review, which moves the 69/130 chart, is a separate classification lane
and does not block batches 1-3.

## State at handoff (2026-11-28)

Generated totals are in [PROGRESS](PROGRESS.md) (`config/matches.csv`). Every
source change is committed and its affected exact consumers passed cold
replay. Five earlier exact claims were withdrawn because they fail at HEAD
(blocker below).

Batch 1, laser.cpp, manager lifetimes and gameplay helpers (2026-11-28, later
session; 54 new exact owners, 5 stale claims withdrawn):

- **The target names its TUs and classes.** `.rdata` keeps
  `.\src\game\<file>.cpp:<line> <Type>` tags in link order, and RTTI names the
  virtual classes. `.text` follows the same alphabetical file order, which
  gives a TU partition for grouping new source (KNOWLEDGE_BASE, "The target
  names its source files and classes"; `.analysis/claude-tu-partition.py`,
  `claude-rtti.py`). Name new classes after RTTI.
- **The inventory misses unreferenced functions.** The link kept unreferenced
  code; 336 ledger gaps with code bytes (about 40 KB) are listed in
  `.analysis/claude-ledger-gaps.txt`. Add rows with
  `.analysis/claude-add-ledger-rows.py` when reconstructing a function.
- **Laser TU** (`src/Laser.{hpp,cpp}`, [LASER](LASER.md)): all 20
  `LaserDataInf` default slots, both derived slot-4 overrides, the three
  constructors, the manager's create/initialize/destructor, update and draw
  loops, both chain callbacks and the clear marker are exact. The spawn
  `CreateSecondaryHostObject` is a frontier: native passes the request in
  EDI and no probe made LTCG do so. Next: the derived slot bodies
  (0x4250B0..0x428600) and the unreferenced list helpers.
- **Same pattern elsewhere.** `SecondaryAuxiliaryOwner` (0x40E0A0/0x40E190/
  0x40E240) is exact after moving its flag and publication into a memset
  constructor and linking `ChainRemove.cpp`. The ascii initializer called the
  wrong log function (`AppendErrorLog` instead of `AppendLog`); with that fixed
  and `CreateAsciiManager` (0x4013F0) written, it is now 115/124 aligned, held
  back by LTCG's choice of EDI instead of EAX for `self`.
- **Sweep of existing images** (`.analysis/claude-sweep-20261128.txt`,
  `tool-dirsweep.py` over every build dir): stale images give false leads
  (`MusicStreamingThread` dropped from 177 to 167 bytes after a cold rebuild),
  so rebuild before trusting a hit. `BindAnimationSprite` and
  `ResetSecondaryHostEffectSnapshot` (0x410880, the `LaserInfiniteParams`
  constructor) became exact this way, and so did `CreateStageResource`
  (0x4029C0) once its `StageLifecycle` context was rebuilt. The other hits
  live in support TUs and need their own context.
- **Batch 2 lane that works.** For a source-present function without a unit,
  look for a symbol of the native size in a context whose primary source is
  the function's file (`.analysis/claude-batch2-candidates.txt`), clone that
  context with `.analysis/claude-try-unit.py FROM_UNIT NEW_UNIT SYMBOL ADDR`
  (it drafts the fields and runs the strict compare) and rebuild stale
  contexts first (`.analysis/claude-rebuild-and-try.py DIR...`). Exact this
  way: the RTTI virtual slots `AsciiManagerObject::StorageSize` (0x401DD0),
  the music scalar deleting destructors (0x45A4D0, 0x45AD90),
  `DeleteThreadInfVirtual` (0x4593E0) and the two pause selection
  comparisons (0x42F340/0x42F350, which need `/GL` even though their ABI is
  plain). A normal-COFF sweep of the small context-less files
  (`.analysis/claude-coff-sweep/results.txt`) found nothing else: those
  functions use LTCG private conventions and need linked contexts with real
  callers.
- **Gameplay helpers.** Matching `UpdateSecondaryGameplay`'s native call
  order to its declared-only callees gave seven small exact functions:
  music pause/resume/reset/start and three chain pauses
  (`src/SecondaryGameplayAudio.cpp`, `src/SecondaryGameplaySceneEffect.cpp`;
  SECONDARY_GAMEPLAY.md). The same mapping trick works for any wide owner:
  list its native calls in order and line them up with the source.
- **Identical bodies** (`.analysis/claude-dup-bodies.txt`, normalized
  disassembly grouped across the ledger): `SecondaryUnaryMathA` (0x405FB0,
  sine) is exact as the twin of the exact cosine wrapper. Open leads for
  Batch 3: `ChangeSecondaryAnimationScript` (0x456770) has the body of the
  exact `ChangeSecondaryDialogueSprite` once its id parameter is a
  `const AnimationVmId &` (needs `SecondaryActorRuntime.hpp` and
  `SecondaryActorUpdate.cpp`); 0x423240 is an out-of-line
  `StageEmbeddedVm` destructor; the unreferenced atan2 copies 0x419750 and
  0x433C80 take EAX. Static header helpers recur once per TU (KNOWLEDGE_BASE,
  FSINCOS helpers), which marks TU boundaries.
- **Inline assembly removed.** The exact `AdvanceStageTimer` used a naked asm
  body; `src/StageTimer.cpp` now reproduces it from natural C++.
- **Tools** (all in `.analysis/`): `claude-dis.py` (target disassembly with
  ledger names), `claude-xrefs.py`, `claude-extent-review.py` (padding,
  external and interior references), `claude-add-coff-exact.py` (registers
  COFF or linked exact units from a JSON spec after a strict compare),
  `claude-draft-linkages.py`.

Earlier work of 2026-11-28 (no new exact owners):

- **Switch body order.** VC9 RTM lays switch case bodies out in lexical order.
  `ExecuteSecondaryActorInstruction` was written almost in opcode order while
  native is not. Reordering its 135 bodies into one
  `SecondaryActorInstructionCases.inc` raised it from 1,184 to 3,353 of 4,003
  register-normalized instructions. The spawn cases share a block scope again,
  which restores the native 0x310 frame. The menu controller's 15 bodies are
  also in native order now (205 -> 214 aligned). All configured switch
  frontiers with a build now match native body order except
  `ExecuteAnimationVm`'s return block. Check this first on any switch owner:
  `.analysis/claude-case-order.py` and `.analysis/claude-case-cmp.sh`.
- **Per-body comparison.** `.analysis/claude-case-bodies.py` compares each case
  body's size and mnemonic shape, which separates local codegen differences
  from global register/layout noise in large interpreters.
- **ExecuteAnimationVm flag setters are bit fields** (finding, not applied;
  see below and ANIMATION_EXECUTION.md).

Three systematic findings from 2026-11-27 still drive the work (details in
KNOWLEDGE_BASE):

1. Barriers, `volatile` and `register` in frontier sources usually stand in
   for ZUN's value-object or field-access code.
2. Native 0x4A56E0..0x4A576F is one address-exposed game-state object. Link
   `probes/secondary-game-state-context.cpp` into frontiers whose owners mix
   pointer stores with game-state reads.
3. x87 commutative operand order is decided by the VC9 scheduler and flips
   when unrelated code in the owner changes; do not spend builds swapping
   operands.

**Blocker: the missing timer constructor (claims withdrawn 2026-11-28).**
`9abb56f` removed `StageTimerView() { flags &= ~1u; }` from
`StageResource.hpp`. Native code clears bit 0 of every embedded timer's flag
word on construction, so ZUN's timer type has that constructor. Three exact
claims fail at HEAD because of it and were withdrawn (their units are now
frontiers): `stage-embedded-vm-constructor` (0x402270, links as a 33-byte
memset body), `secondary-actor-state-constructor` (0x410440, a 14-byte
vector-constructor call) and `secondary-actor-instruction-adapter` (13 bytes
instead of 11). `StageResource.hpp` was not a declared input, so their
receipts never went stale. Restoring the constructor is the fix, but nearly
every unit includes the header: it needs one deliberate wide replay, ideally
together with other shared-header changes (Batch 3). New code works around
it locally with `struct LaserTimer : StageTimerView { LaserTimer() { flags &=
~1u; } }`, as `SecondaryHud.hpp` does; drop those once the base type has its
constructor. Other exact units whose objects embed timers may be affected
too; a cold replay of the `StageLifecycle` context family is the first check.

Two more claims were withdrawn after cold rebuilds of their contexts at HEAD
(`.analysis/claude-rebuild-and-try.log`): `draw-fan-animation-vm` (0x452680,
LTCG now specializes `DrawAnimationVertexFan` and drops the `push 0x21`) and
`animation-distance-root` (0x4196E0, loses its 8-byte aligned frame). Both
sources are unchanged; something else in their contexts moved. Stale
receipts hide such regressions, so rebuild a context before trusting its
old exact rows.

**For the later large-owner phase: give their callees source.** In the wide
gameplay context `ExecuteSecondaryActorInstruction` (16,556 B) calls about 45
symbols that exist only as declarations and are bound to data anchors
(`FindSecondaryHostObject`, `RecomposeSecondaryHostMotion` at 0x459670,
`SecondaryHostRandomAngle`, `CreateSecondaryHostObject`, the `ResetSecondaryHost*`
timers, ...). LTCG can only call an anchor with the plain stack convention,
while native uses each callee's private register interface, so the caller's
code differs far from the calls. This is the same effect that `0x41F510`
showed for the dialogue interpreter. List them with
`scripts/repo-python .analysis/claude-anchor-callees.py BUILD SYMBOL`; native
addresses partly come from `.analysis/claude-map-callees.py`.

Replay helper for changed shared inputs:
`scripts/repo-python .analysis/claude-replay-affected.py FILE...` replays every
exact unit whose context contains one of the files and resumes from current
receipts. Receipts hash transitive includes, so a header change reaches far:
`AnimationVm.hpp` alone affects 295 exact units in 160 contexts. A `.cpp`
used by the wide gameplay context typically needs 18-24 units in about 20
contexts (roughly 90 minutes). Wide contexts take 5-25 minutes each under
load. Never edit a source while a replay is running; batch changes instead.
The failure `secondary-actor-instruction-adapter` in every replay is the known
blocker above, not a regression.

## Deferred large owner: ExecuteAnimationVm (0x44B4B0, 12,056 B)

2026-11-28 finding, not applied: the flag-setter opcodes are bit-field
assignments in native. A bit-field member in `AnimationVm.hpp` raises
alignment to 2,953 and native-shape case bodies from 53 to 65 of 102. Applying
it needs a 160-context replay; see ANIMATION_EXECUTION.md, "Flag opcodes are
bit fields".

The current cold owner is 12,096 bytes: 11,680 code/alignment plus the complete
416-byte opcode table, against native 11,640 + 416. It has 3,223 instructions
versus native 3,203. Calls now agree at 262; every direct destination and its
multiplicity agrees. Conditional branches are 258/259 and jumps 142/147
(candidate/native). The exhaustive inventory has 409 fields versus native 415:
150/156 DIR32 and 259/259 REL32. Each zero-vector component still has two reads
versus native four. Frame `0xE4`, EBX-VM and EDI-cursor remain wrong. Diagnostic
alignment is 1,445/3,203, or 2,579 after a register-label swap. No exact credit.

`ReadF` now uses one conditional return. It restores both opcode 97 readers
with full-width script IDs and correct child-publication order. A
same-source/object-path control restores the old 76 reads when the old body is
reinstated, with the same reader symbol; a macro control also recovers 78
reads. Four math stores and all four strip windows retain FSTP32 rounding; the
complete 540-byte reader and 104-byte integer destination keep all nonfield
bytes and field positions. Evidence:
`.analysis/gpt-6.1-sol-readf-conditional-main-{cold,code,fields}.log`,
`gpt-6.1-sol-readf-main-cold-rounding-review.txt` and
`gpt-6.1-sol-readf-same-path-control.log`.

All 24 exact consumers in 23 contexts passed cold replay for this source
epoch. The first run stopped at context 15 when the machine restarted; the
resumed run reused the 14 current receipts and finished with
`TERMINAL ... "passed_units": 24, "failures": []`. Logs:
`.analysis/gpt-6.1-sol-readf-conditional-consumer-replays.log` and
`.analysis/readf-conditional-consumer-replays-resume.log`.

The preceding checkpoint is `0095348`: shared quad calls, direct child-ID
forwarding, two strip rounding stages, the effect-local timer constructor and
VM `+41C/+41D` initializer spelling. Its 74 affected units / 41 contexts passed;
those older proofs do not cover the new conditional-return source epoch.
The main graph retains 23 TUs, the argument TU's canonical `/Ob1` profile and
both actual timer TUs. The 124-byte float-destination / fourteen-field and
20-byte rewind / one-field support proofs remain recorded in
`gpt-6.1-sol-real-timers-{float-destination,rewind}-support.log`. Only the quad helper's own context uses rotation
`/fp:fast`; rotation and the enclosing interpreter remain non-exact.

The compiler traces correct the child-copy hypothesis. Both source forms
enter late flow with the copy early. The conditional form keeps its readers
through `FgDoLateFlowOpts`, and the copy is still early at pre-encode entry.
The final `Schedule` call at RVA `0x125581` moves it after FLD/test, observed
at return `0x125586`. Do not treat final copy placement as the cause of the
prior merge. Paired complete debugger/ordinary text controls match: 33,446
bytes for the old form, 33,542 for the macro and 33,590 for the conditional
function. See `KNOWLEDGE_BASE.md` and
`.analysis/compiler-vc9/execution-child-{flow,flow-macro,conditions-baseline,conditions-conditional,schedule}.txt`.
The former register-priority traces describe the preceding 11,940-byte graph;
recheck its ranges before reusing numeric identities with the new source.

The private constructed-result model with the conditional reader reaches
12,176 bytes / 3,254 instructions, frame `0xCC`, all 415 fields and the native
call/conditional/jump counts 262/259/147. All direct calls bind with correct
multiplicities, zero-vector reads agree, and vector return buffers retain
`ESP+24`. It remains 120 bytes / 51 instructions larger and EBX-VM/EDI-cursor.
The default-constructor sum control has the same size/counts and changes 22
nonfield bytes. Keep both private. Evidence:
`.analysis/gpt-6.1-sol-constructed-readf-conditional-{control,code,fields}.log`
and `gpt-6.1-sol-constructed-sum-default-body-comparison.txt`.

Earlier view/reference/setter controls did not recover the readers. Separate
scratch locals reduce one RGB stack color but lose the constructed model's
native tween-result slot; the causal chain is in
`.analysis/compiler-vc9/scratch-color-choice-comparison.txt`. Historical
78-reader child images narrow script IDs or publish translation early. Retain
their findings as failed controls; obsolete experiment binaries have been pruned. Unrotated draw and player draw callback exact claims
remain withdrawn after complete cold failures; see `ANIMATION_VM_DRAW.md`
and `SECONDARY_PLAYER_CONTROLLER.md`.

The complete private model's mnemonic deltas include +35 MOV, +13 LEA,
+6 MOVZX and +6 XOR, with four fewer FLD and FSTP each. The excess is concentrated in moves and address calculations. Physical opcode partitions move the update tail between the
case-2 and default intervals; use
`.analysis/gpt-6.1-sol-private-owner-partitions.txt` as a diagnostic partition,
not as semantic handler boundaries.

The conditional reader is committed (`9130dd3`, all 24 consumers passed).
Next: decide on the flag bit-field member (it needs the 160-context replay),
then investigate the moves, address calculations and shared tails (the 96/97/91
translation tail, the case -1/1/2 return block that our build places after the
loop). Preserve the recovered rounding and full-width IDs.

The main cold family was rebuilt from HEAD at handoff. If a later
source change needs a rebuild, inspect only that owner and use the code/table
aware comparison:

```bash
scripts/repo-python scripts/build-match-unit.py --unit animation-execution-tween-context
scripts/repo-python .analysis/compare-execute-animation-vm-code-only.py build/match-linked/AnimationExecutionTweenContext
```

## Related leaf state

| Leaf | State | Evidence |
| --- | --- | --- |
| `AnimationFloatDestination` (`44B380`, 124 B) | Canonical exact, all fourteen fields | `animation-float-destination` |
| `AnimationIntegerDestination` (`44B400`, 104 B) | Canonical exact, all eleven fields | `animation-integer-destination` |
| `StepAnimationFloatPairTween` (673 B) | Non-exact; two mode-17 Y-component operand bytes remain | [ANIMATION_TWEEN](ANIMATION_TWEEN.md), [ANIMATION_TWEEN_DISPATCH](ANIMATION_TWEEN_DISPATCH.md) |
| `BindAnimationSprite` (`44AB40`, 389 B) | Canonical exact as the `AnimationVmView` member body (`bind-animation-sprite-member`, standard profile); the stdcall wrapper is a non-native jump thunk | [ANIMATION_EXECUTION](ANIMATION_EXECUTION.md) |

## Other large owners (deferred; state at handoff)

| Owner | State | Notes |
| --- | --- | --- |
| `~SecondaryEclDebugOwner` (`40D320`, 462 B) | Canonical exact | +0x2C8 `StageEmbeddedVm` member supplied the second EH state |
| `InitializeSecondaryDebugHudAnimations` (`41A6D0`, 1,619 B) | Frontier, 1,635 B / 406 ins | ID wrap, tail order and lives-display ABI fixed; frame is 8 vs native 0xC (native has an extra never-accessed high-weight stack color), plus register choices |
| `~SecondaryDebugHudOwned` (`41AFC0`, 729 B) | Frontier | Native caches both critical-section imports and keeps `this` on the stack |
| `ResetStageMesh` (`40E6C0`, 325 B) | Frontier | Only two x87 add operand orders differ; swapping the source operands had no effect. `StageMesh.cpp` feeds ~40 exact contexts |
| `ExecuteStageScript` (`404470`, 2,736 B) | Frontier, 2,724 B / 603 aligned | Barrier-free; frame 0x90 vs 0x98 and vector-temporary packing remain (STAGE_SCRIPT.md) |
| `UpdateStage` (`402AA0`, 1,677 B) | Frontier, 1,672 B / 395 aligned | Mode 1 aligns; mode-2 mesh reload, copy and color scheduling remain (STAGE_UPDATE.md) |
| `CullStageObject` (`403950`, 1,643 B) | Frontier, 1,646 B / 492 aligned | Natural bounds/corner/loop source; start-of-function vector copies and two corner add orders remain (STAGE_CULLING.md) |
| `InitializeSecondaryReplay` (`4356E0`, 1,380 B) | Frontier, 1,370 B / 394 of 406 | Game-state field addresses now exposed in its context (KNOWLEDGE_BASE 2026-11-27); store scheduling remains |
| `ExecuteSecondaryActorInstruction` (`412E30`, 16,556 B) | Frontier, 16,172 B / 2,712 aligned, 3,353 of 4,003 register-normalized | Case bodies in native order, native 0x310 frame; entry register assignment and anchored callees remain (SECONDARY_HOST_DISPATCH.md) |
| `RunSecondaryScript` (`45B860`, 7,093 B) | Frontier, 7,085 B / 1,310 aligned, 1,768 of 1,834 register-normalized | Case shapes restored; VM register, a tail merge and callee ABIs remain (SECONDARY_SCRIPT.md) |
| `RunSecondaryDialogue` (`41D380`, 6,980 B) | Frontier, 7,008 B / 1,313 aligned, 1,805 of 1,848 register-normalized | Native switches, sprite ternaries, flag bit field, structured clear statistics, real 0x41F510 helper; register choices and one goto block layout remain (SECONDARY_DIALOGUE.md) |
| `UpdateMenuController` (`4399B0`, 1,224 B) | Frontier `menu-controller`, 1,196 B / 214 aligned, 218 of 300 register-normalized | Native case order; page owners linked but their LTCG register conventions differ from native (MENU_CONTROLLER.md) |
| `DrawStageLayer` (`404190`, 727 B) | Frontier, 722 B / 155 aligned | No volatile/register; stage-pointer register choice remains. Its four exact consumers pass (`.analysis/claude-stagelayer-replay.log`) |

The four scene-VM publishers (0x4556E0, 0x455760, 0x4557E0, 0x455860) are
now canonical exact: the next-ID counter is an `AnimationVmId` object (see the
KNOWLEDGE_BASE entry of 2026-11-27). Replay log of their 83 exact consumers:
`.analysis/claude-counter-replay.log`.

## Continuation and other frontiers

Follow the roadmap at the top. Compile and compare the complete affected owners and relocation fields; replay only the
exact consumers of changed executable inputs. Use the commit prefix the
operator specifies (AGENTS.md says `gpt-6.1-sol:`; this session's operator
asked for `claude-opus-5.5:` with no trailer).
Retain useful failed controls in prose/logs and keep private models separate.

Other unfinished work is recorded in [STAGE_SCRIPT](STAGE_SCRIPT.md),
[STAGE_CULLING](STAGE_CULLING.md), [STAGE_RESOURCE](STAGE_RESOURCE.md),
[STAGE_TWEEN](STAGE_TWEEN.md), [SECONDARY_SPELL_STAGE](SECONDARY_SPELL_STAGE.md)
and [SECONDARY_DEBUG_LIFECYCLE](SECONDARY_DEBUG_LIFECYCLE.md). Their old
candidate sizes/key counts are historical; use their selected contexts before
relying on new metrics. `ExecuteStageScript`'s complete 2,736-byte target owns
100 bytes of tables. The acquisition `__thiscall` filename-view attempt broke
four exact callers and was reverted; its failure is in STAGE_RESOURCE.

`animation-execution-isolated` is a historical register/frame experiment.
`update-stage-real-interpreter-animation-oy` has a stale linked receipt;
rebuild that context before using it. The retained stage comparison logs are
`.analysis/{stage-script,draw-stage-layer,cull-stage}-final-current-20261120.log`.

## Storage and current workspace

Cleanup on 2026-11-28 removed 170 MiB: the `Claude*` experiment directories
under `build/probe-ltcg/` (none was configured), my private build copies and
source backups in `.analysis/`, and 731 `.obj` intermediates of non-exact
frontier families. Afterwards `build/` was 530 MiB and `.analysis/` 103 MiB.
Frontier linked images (`source.exe/.map/.pdb`) remain, so the comparison
tools still work, but rebuild a frontier before relinking it or verifying its
receipt. Exact families were not touched. `AnimationExecutionTweenContext`
keeps its objects because the private `build/probe-ltcg/Execution*` models
reuse them. Those private models remain.

The 2026-11-26 cleanup and retention rules are in [ARTIFACTS](ARTIFACTS.md)
(inventory `.analysis/storage-prune-20261126.tsv`).
Receipts can become stale when any hashed input changes, even when generated
code does not. A stale receipt is not a lost proof; rebuild the affected unit
and cold-replay only affected exact units.
