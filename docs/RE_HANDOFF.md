# TH11 reconstruction handoff — 2026-11-28

## Start here

Work only against original Japanese TH11 1.00a (`resources/th11.exe`, SHA-256
`2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`). The
pinned code-generation target is VC9 RTM build 21022. Keep `/GL` for established
LTCG owners and use natural C/C++ only: no inline assembly, padding, byte stubs
or cropped extents. Use the repository shell, `scripts/repo-python` and direct
Ghidra through `scripts/ghidra.py`.

## State at handoff (2026-11-28)

The canonical ledger has **497 exact owners** (`config/matches.csv`; generated
totals in [PROGRESS](PROGRESS.md)). The working tree is clean apart from this
handoff; every source change is committed and its affected exact consumers
passed cold replay, except for the two stale rows in the blocker below.

Work of 2026-11-28 (no new exact owners):

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

**Blocker found 2026-11-27: two canonical exact units fail at HEAD.**
`secondary-actor-state-constructor` (0x410440, 126 B) now links as a 14-byte
call to the vector-constructor iterator, and `secondary-actor-instruction-adapter`
links at 13 B instead of 11. Both reproduce with the committed
`SecondaryScript.cpp`, so they predate this session. Cause of the first:
`9abb56f` removed `StageTimerView() { flags &= ~1u; }` from
`StageResource.hpp`. The native constructor clears bit 0 of every timer
flag word (frame, both vector tweens, six pair tweens, two timers), so ZUN's
timer type does have that constructor. `StageResource.hpp` is not a declared
input of these units, so their receipts never noticed. Restoring the
constructor is the faithful fix, but nearly every unit includes the header;
it needs a deliberate wide replay. Treat both rows as unverified until then.

**Next for the largest owners: give their callees source.** In the wide
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

## Current focus: ExecuteAnimationVm (0x44B4B0, 12,056 B)

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
| `BindAnimationSprite` (389 B) | Non-exact | [ANIMATION_EXECUTION](ANIMATION_EXECUTION.md) |

## Other large owners (state at handoff)

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

Keep the interpreter and real dependencies in one work stream. Compile and
compare the complete affected owners and relocation fields; replay only the
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
