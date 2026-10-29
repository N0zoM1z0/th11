# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; the outer
folder is not Git. Read AGENTS.md and relevant source/ledger rows. Current
source, receipts and target bytes outrank historical notes.

## Checked-in state

- Original Japanese TH11 v1.00a only; target identity: `config/target.toml`.
- 2,047 candidates;244 reviewed boundaries,201 source-present mappings.
- 108 exact functions /18,282 bytes:8 normal COFF and100 linked PE.
- Origins:90 authored,145 reviewed indeterminate,7 exclusions,1,805 pending.
- Direct local scripts/Ghidra; no Factory MCP or REA. Pinned VC9 RTM/SDK6.0A.
- Commit prefix: `gpt-6.1-sol: `. All dirty work belongs to this effort.

Latest checkpoint adds22 complete source mappings/3,129 target bytes, including
integer XYZ interpolation768, integer scalar332, float pair673, float scalar331,
renderer updates221/249 and allocation177. Fifteen full cold exact proofs add551
bytes: layer draw127, allocator177, four factors39 each, timer reset52 and eight
small accessors39 total. Twenty-two further origins/boundaries reviewed, all
indeterminate. See [ANIMATION_TWEEN_DISPATCH.md](ANIMATION_TWEEN_DISPATCH.md).

Nine missed tiny entries bring inventory2,038 to2,047. Getters/setters are
separately partitioned at final RETs and CC padding; integer-vector getter
44E7B0 immediately follows the full768-byte owner without a gap. Preserve CSV
additions; never regenerate old inventory over them. Ghidra DB is unchanged.

## Current gaps and next work

| Function | Target bytes | Current source / diagnostic differences |
| --- | ---: | --- |
| StepAnimationIntVectorTween,44E4B0 | 768 | 768 /345; frame48 versus native40 |
| StepAnimationIntScalarTween,44E7C0 | 332 | 321 /263 |
| StepAnimationFloatPairTween,44E910 | 673 | 615 /581 |
| StepAnimationFloatScalarTween,44EBD0 | 331 | 326 /269 |
| UpdateAnimationRenderer,455480 | 221 | 231 /180; native8-byte alignment absent |
| UpdateOtherAnimationRenderer,455560 | 249 | 232 /193; same frame issue |
| SetAnimationFloatScalarTweenDuration,44ED70 | 4 | 4 /1; private input registers |
| AnimationRendererConstructor,4526F0 | 2,601 | 2,601 /153; EH acceptance open |
| AnimationRendererDestructor,453130 | 225 | 202 /73; EH/default-member inlining open |
| AccumulateAnimationArgbPixel,453440 | 30 | 30 /6; green/blue load schedule |
| InitializeFanAnimationVm,4520F0 | 805 | 805 /417 |
| UpdateFanAnimationVm,452420 | 598 | 614 /436 |
| DrawWorldAnimationVm,4513A0 | 1,036 | 1,038 /750 |

1. Continue the four interpolation bodies. Separate completion returns and
   value snapshots improve integer XYZ from723/647 to768/464; a real four-float
   coefficient aggregate reaches768/345. Remaining completion stores, aggregate
   copies/epilogues, frame and x87 scheduling are open. Keep actual per-component
   integer truncation versus scalar sum-then-truncate, mode7 velocity and17
   position/velocity updates, and coefficient/product rounding phases.
2. Renderer updates save update-node next before callbacks/reclaim and rebuild
   per-layer nextDraw at VM+1C. Secondary update resets processedB8 and forces
   layers other than29/30 to29; use two tail slots indexed layer-29. Draw reads
   nextDraw after callback/draw. Both callbacks' return values are ignored.
   Private reclaim source is still opaque in this diagnostic family: adding
   real AnimationRendererLifecycle support is a concrete next context probe.
   Interpreter44B4B0/11,640 bytes remains source-open; its real8-byte alignment
   context and full interpreter are substantial future work, not a fake stub.
3. Four39-byte factors need real StageEasing support to recover native private
   EAX curve. Layer127 needs actual AnimationImmediateDraw support. Both are
   already canonical exact. Allocation177 checks only two signed pool slots,
   then real new434/VM construction/reset, and advances slot once more. Do not
   turn it into a full-pool scan or add an allocation-null recovery path.
4. Renderer constructor uses real4096/default/31 VM lifetimes before clearing
   allocation7BD894 and resetting the main pool. Keep3/6 internal alignment
   bytes in constructor/destructor. Default member's inline cleanup and EH
   states differ; specialized EH acceptance is still open.
5. Continue broader big paths: fan805/598, world1036, state572, submission1112,
   billboard976, rotation571, projected792, fog794/546 and dispatcher217/table.
   Polar4526A0 uses paired FSINCOS and preserves EDX; no separate sin/cos or
   invented implementation is substituted. Fan drift4A4 drives both UV axes,
   edge Z accumulates, closing vertex32 copies1. See existing topic docs.

## Evidence and checkpoint scope

New canonical families: `build/match-linked/AnimationTween` and
`build/match-linked/AnimationRendererDispatch`. Latest proof:
`.analysis/animation-tween-dispatch-affected-cold.log`:15 new units/551 bytes/19
independent fields, plus exactly two affected old units (buffer487/reclaim273,
760 bytes/29 fields). Every complete public primary-TU contribution and byte
passes with current source/header/tool/producer/PE/MAP/PDB receipts.

Renderer physical header now names processedVmCountB8, signed nextPoolSlot4350BC
and known publication counter7BD888, with occupancy/slot offset assertions.
This header change is why those two old units were replayed. No other old unit,
full cohort or broad behavioral suite was run. Infrastructure is unchanged.

Current probes: `.analysis/animation-tween-coefficient-aggregate.log` and
`.analysis/renderer-dispatch-real-draw-owner.log`; topic doc links baselines,
rejected probes, complete target attestations and independent binding/extent
receipts. Source files froze before cold verification. Target, decompiler
output, tool payloads and Ghidra data remain private. No tool job is live.

```bash
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py --require-target
```

Cold-replay changed exact units and directly affected dependencies only.
Update tracking/progress, check the diff, and save meaningful progress.
Full native product, runtime and portability work remains later work;
the reconstruction goal is active.
