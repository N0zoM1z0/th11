# TH11 reconstruction handoff — 2026-11-26

## Start here

Work only against original Japanese TH11 1.00a (`resources/th11.exe`, SHA-256
`2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`). The
pinned code-generation target is VC9 RTM build 21022. Keep `/GL` for established
LTCG owners and use natural C/C++ only: no inline assembly, padding, byte stubs
or cropped extents. Use the repository shell, `scripts/repo-python` and direct
Ghidra through `scripts/ghidra.py`.

## Current focus: ExecuteAnimationVm (0x44B4B0, 12,056 B)

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
`gpt-6.1-sol-real-timers-{float-destination,rewind}-support.log`. The canonical
ledger has 492 exact owners. Only the quad helper's own context uses rotation
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

Next: finish the existing replay and checkpoint the conditional reader, then
investigate those moves, address calculations and shared tails before changing
shared result types. Preserve the recovered rounding and full-width IDs.

The main cold family is already current for the frozen source. If a later
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

## Secondary debug/HUD owners (2026-11-27)

| Owner | State | Notes |
| --- | --- | --- |
| `~SecondaryEclDebugOwner` (`40D320`, 462 B) | Canonical exact | +0x2C8 `StageEmbeddedVm` member supplied the second EH state |
| `InitializeSecondaryDebugHudAnimations` (`41A6D0`, 1,619 B) | Frontier | See SECONDARY_DEBUG_LIFECYCLE.md; frame, lives-display ABI and ID-wrap gaps |
| `~SecondaryDebugHudOwned` (`41AFC0`, 729 B) | Frontier | Native caches both critical-section imports and keeps `this` on the stack |
| `ResetStageMesh` (`40E6C0`, 325 B) | Frontier | Only two x87 add operand orders differ; `StageMesh.cpp` feeds ~40 exact contexts, so batch its replay |

Every VC9 build of the scene-VM publishers folds the ID wrap into
`add; jne; mov [nextId], 1`, while every native copy (0x4556E0, 0x455760,
0x455860, HUD 0x41A96B and 0x41BD84) re-reads: `add; cmp [nextId], 0; jne;
add`. This is one systematic source-form gap across all publishers;
`.analysis/claude-nextid/` holds candidate spellings to test.

## Continuation and other frontiers

Keep the interpreter and real dependencies in one work stream. Compile and
compare the complete affected owners and relocation fields; replay only the
exact consumers of changed executable inputs. Use `gpt-6.1-sol: ...` commits.
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

The second 2026-11-26 cleanup removed 5,282 obsolete files (286.15 MiB),
including old object archives, unconfigured experiment builds, broad compiler
dumps and the old Factory query cache. Combined with the earlier compression
pass, 347.71 MiB of file content was removed. `build/` was about 571 MiB and
`.analysis/` 102 MiB after cleanup. Inventory:
`.analysis/storage-prune-20261126.tsv`; retention details: [ARTIFACTS](ARTIFACTS.md).
There is no `.build/` directory. Exact families, current model/control inputs,
target, compiler, Wine prefix, Ghidra project and the active replay remain.
Historical private output paths are not all available; the old object archives
were deleted, so rebuild an old experiment if needed. Two nested exact caches
were mistakenly removed, then cold-built and compared with zero differences:
`scale-animation-color-channel` and `build-animation-vm-quad`. Their recovery
logs are listed in ARTIFACTS; they are excluded from the removed-file totals.

The 11,552-byte UpdateStageRealInterpreterAnimationOy receipt was already stale
after AnimationTween.cpp changed; its object intermediates are now pruned.
Cold-build that frontier before using its metrics.
Receipts can become stale when any hashed input changes, even when generated
code does not. A stale receipt is not a lost proof; rebuild the affected unit
and cold-replay only affected exact units. No full cold replay was run for this
handoff cleanup.
