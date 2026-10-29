# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; the outer
folder is not Git. Read AGENTS.md and relevant source/ledger rows. Current
source, receipts and target bytes outrank historical notes.

## Checked-in state

- Original Japanese TH11 v1.00a only; target identity: `config/target.toml`.
- 2,047 candidates;262 reviewed boundaries,206 source-present mappings.
- 111 exact functions /18,735 bytes:8 normal COFF and103 linked PE.
- Origins:90 authored,163 reviewed indeterminate,7 exclusions,1,787 pending.
- Direct local scripts/Ghidra; no Factory MCP or REA. Pinned VC9 RTM/SDK6.0A.
- Commit prefix: `gpt-6.1-sol: `. All dirty work belongs to this effort.

Latest checkpoint adds five complete source mappings/1,932 target bytes:
audio dispatcher1,460, streaming worker177, enqueue151, getter19 and volume125.
Three complete cold exact proofs add453 bytes (177/151/125). Eighteen further
boundary/origin reviews remain indeterminate. See [SOUND_QUEUE.md](SOUND_QUEUE.md).

44A340 is audio code, not ANM despite neighboring bind functions. Its old1,428
inventory omitted32 bytes of owned switch table44A8D4..44A8F3; full owner now
1,460. Eight prior external-interior references originate in that table and
are internal arms. Twelve CC bytes precede thread44A900. Ghidra's inferred two
fastcall inputs are unused. Maintain this correction and all prior missing
entry additions; never regenerate the old inventory over the CSVs. DB unchanged.

## Current gaps and next work

| Function | Target bytes | Current source / diagnostic differences |
| --- | ---: | --- |
| ProcessSoundQueue,44A340 | 1,460 | 1,556 /1,085; includes full code/table |
| FirstMusicBuffer,45AA00 | 19 | 19 /13; return-block layout |
| UpdateAnimationRenderer,455480 | 221 | 213 /170; native8-byte alignment absent |
| UpdateOtherAnimationRenderer,455560 | 249 | 249 /195; same frame issue |
| StepAnimationIntVectorTween,44E4B0 | 768 | 768 /345; frame48 versus native40 |
| StepAnimationIntScalarTween,44E7C0 | 332 | 321 /263 |
| StepAnimationFloatPairTween,44E910 | 673 | 615 /581 |
| StepAnimationFloatScalarTween,44EBD0 | 331 | 326 /269 |
| AnimationRendererConstructor,4526F0 | 2,601 | 2,601 /153; EH acceptance open |
| AnimationRendererDestructor,453130 | 225 | 202 /73; EH/default-member inlining open |
| InitializeFanAnimationVm,4520F0 | 805 | 805 /417 |
| UpdateFanAnimationVm,452420 | 598 | 614 /436 |
| DrawWorldAnimationVm,4513A0 | 1,036 | 1,038 /750 |

1. Continue audio dispatcher and its real helpers. Full reviews now cover
   lookup215, prepared load515/start291/close178, decoder open353, refill570,
   play133/stop129/pause117/resume77, streaming update543, reset223 and configure130.
   Only getter19 and volume125 have source among these15 helpers. Manager610
   is currently a physical nonzero gate; its concrete interface type is open. Opaque helper
   calling contexts, frame and scheduling still differ; never crop the switch
   table or substitute fake successes. Command cursor advances during shift,
   load repeat does not reset it, and shutdown wait retry cancels step increment.
2. Worker177 needs actual CreateThread callback escape to retain unused context
   and RET4. Its per-wake snapshot and two subsequent shared-root reads are
   independently observed. A scoped volatile view expresses those worker reads;
   making the whole global pointer volatile added unwanted dispatcher reads and
   was rejected. Busy is a volatile physical view, not proof of original source
   qualifiers. Effect volume uses cube; music volume uses square. Do not merge.
3. Renderer-dispatch canonical support now includes real AnimationRendererLifecycle.
   It improves update213/170 and249/195; draw127 and allocator177 stay exact.
   Interpreter44B4B0/11,640 bytes is still source-open; actual8-byte alignment
   context and full interpreter remain substantial work. Four operand helpers
   near44B080/B2A0/B380/B400 also have following tables omitted by auto extents;
   they were inspected but not yet granted reviewed ledger states or source.
4. Continue the four interpolation bodies. Separate completion returns/value
   snapshots and a four-float coefficient aggregate improve integer XYZ from
   723/647 to768/345. Keep vector per-component truncation versus scalar final
   truncation, mode7 velocity and17 position/velocity updates, and real float
   coefficient/product rounding. Equal size gives no exact credit.
5. Continue broader big paths: constructor/destructor EH with3/6 alignment bytes,
   fan805/598, world1036, state572, submission1112, billboard976, rotation571,
   projected792, fog794/546 and dispatcher217/table. Paired FSINCOS4526A0 and
   its EDX preservation remain open; no separate sin/cos implementation is used.
   See prior topic records for actual fan and UV/storage behavior.

## Evidence and replay scope

New family: `build/match-linked/SoundQueue`. Latest proof:
`.analysis/sound-queue-and-renderer-affected-cold.log`: three new owners/453
bytes/19 fields, plus exactly two affected old renderer owners/304 bytes/14
fields. Complete public primary-TU PDB extents, current source/header/tool/
producer/PE/MAP/PDB receipts and all bytes pass. No full cohort or broad
behavior suite ran. Oracle/build infrastructure is unchanged.

New physical `SoundBuffer.hpp` models only observed COM calls because the pinned
SDK payload lacks dsound.h; opaque slots are never invoked. It introduces no
compiler payload or target bytes. Existing mesh-effect queue source/context
and other canonical families did not change. Source froze before cold proof.

Current diagnostics: `.analysis/sound-queue-scoped-shared-read.log` and
`.analysis/renderer-dispatch-real-reclaim.log`; the audio topic links full
Ghidra attestations, table references, independent bindings/extents and rejected
probes. Trace44AB00 is a verified single RET; only the real boundary is declared,
with no new stub source or exact credit. No tool job is live at checkpoint.

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
