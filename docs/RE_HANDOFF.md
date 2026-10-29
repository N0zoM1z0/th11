# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; the outer
folder is not Git. Read AGENTS.md and relevant source/ledger rows. Current
source, receipts and target bytes outrank historical notes.

## Checked-in state

- Original Japanese TH11 v1.00a only; target identity: `config/target.toml`.
- 2,060 candidates;285 reviewed boundaries,232 source-present mappings.
- 128 exact functions /19,920 bytes:8 normal COFF and120 linked PE.
- Origins:90 authored,186 reviewed indeterminate,7 exclusions,1,777 pending.
- Direct local scripts/Ghidra; no Factory MCP or REA. Pinned VC9 RTM/SDK6.0A.
- Commit prefix: `gpt-6.1-sol: `. All dirty work belongs to this effort.

Latest checkpoint adds26 complete source mappings/3,230 target bytes, including
refill570 and streaming update543. Seventeen complete cold exact proofs add1,185
bytes, including decoder reset223/open164/configure130/pause117/restore93 and
four fades70/51/51/57. Thirteen missing entries recovered;23 new boundary/origin
reviews plus eight earlier owners rechecked. All remain indeterminate. See
[MUSIC_STREAM.md](MUSIC_STREAM.md).

Preserve all maintained missing-entry additions and boundary corrections. Audio
dispatch44A340 owns1,460 including its32-byte table; initial inventory1,428 was
wrong. No DB mutation or regeneration over CSVs. New RTTI inspection confirms
CSound/CStreamingSound names, not original unmodified library/sample ownership.

## Current gaps and concrete next work

| Function | Target bytes | Current source / diagnostic differences |
| --- | ---: | --- |
| RefillMusicBuffer,45A6F0 | 570 | 545 /500; receiver/private read arguments |
| UpdateMusicStream,45AEE0 | 543 | 544 /421 |
| ReadMusicDecoder,45B480 | 192 | 158 /179 |
| ResetMusicPosition,45B100 | 223 | 220 /160 |
| SelectAvailableMusicBuffer,45A990 | 100 | 100 /21 |
| PlayMusicStream,45AA60 | 133 | 132 /20 |
| StopMusicStream,45AB70 | 129 | 135 /129 |
| ResumeMusicStream,45AC80 | 77 | 77 /62 |
| IsAnyMusicBufferPlaying,45AD10 | 78 | 78 /13 |
| ProcessSoundQueue,44A340 | 1,460 | 1,540 /1,077; full code/table |
| FirstMusicBuffer,45AA00 | 19 | 19 /13 |
| UpdateAnimationRenderer,455480 | 221 | 213 /170; native8-byte alignment absent |
| UpdateOtherAnimationRenderer,455560 | 249 | 249 /195; same alignment gap |
| StepAnimationIntVectorTween,44E4B0 | 768 | 768 /345; frame48/native40 |

1. Continue the two larger streaming bodies and read context. Native refill has
   EBX receiver, two stack arguments/RET8; candidate keeps all three on stack.
   Native read has ESI decoder/EAX bytes, destination/output on stack/RET8;
   candidate additionally uses EDX/EBX and RET. A member-refill experiment gave
   unchanged code and was rejected. A concrete next type probe is BYTE pointers
   for locked/read data instead of void pointers, consistently in real callers;
   never force register ABI or add dummy uses. Keep full contributions.
2. Restore calls twice per iteration, with Sleep10 only on the first BufferLost
   result. Refill/notification must retain all lost/loop/silence paths that
   untyped Ghidra COM calls incorrectly dropped as unreachable. Early failure
   lock lifetimes, ignored first position result, and zero-progress loops remain
   native behavior; do not add cleanup, guards or fallback success.
3. Corrected record1C is dataSize, not loop. Signed loopStart is18. Decoder8 is
   remaining and2C total size; base/cursor/extent80/84/88, file8C, record90,
   filename94 and saved98. Memory read snapshots cursor before clearing output.
   File read decrements remaining before ReadFile, ignores BOOL and returnsS_OK.
   Memory binder43 really publishes state then returnsE_NOTIMPL; it is exact.
4. All21 COM methods are declared in normal C++ interface order; old explicit
   function-table view is superseded. Public signatures were checked against
   installed protocol declarations; no implementation/tool payload imported.
   /Ob2 did not improve the big gaps and enlarged configure130 to265, so canonical
   /Ob1 /GL remains. Real MusicStream support now belongs to SoundQueue context;
   worker177/enqueue151/volume125 retain full zero proof.
5. Lifetimes remain source-open: base ctor119/destructor133, streaming ctor48,
   scalar-deleting destructor36 and plain destructor tail11. Keep ownership/EH
   acceptance separate. Continue broader large paths too: interpreter44B4B0
   /11,640 source-open; interpolation332/673/331, renderer ctor2601/dtor225 EH,
   fan805/598, world1036, state572, submission1112, billboard976, rotation571,
   projected792, fog794/546 and dispatcher217/table. Four VM operand helper
   tables near44B080/B2A0/B380/B400 need complete owner review before credit.

## Evidence and replay scope

New family: `build/match-linked/MusicStream`. Latest proof:
`.analysis/music-stream-twenty-affected-cold.log`:17 new owners/1,185 bytes/26
fields plus exactly3 affected SoundQueue owners/453 bytes/19 fields. Complete
public primary-TU PDB extents and current source/header/tool/producer/PE/MAP/PDB
receipts pass with all bytes zero-difference. No other exact family, full cohort,
broad behavior suite or infrastructure tests ran. Infrastructure is unchanged.

Current diagnostics: `.analysis/music-stream-virtual-buffer.log` and
`.analysis/sound-queue-stream-owner.log`. Topic doc links full mapped-text
Ghidra attestations, explicit missing-entry spans, independent fields/extents,
RTTI evidence, protocol-reference hash and rejected profile/member probes.
Source froze before cold verification. Target, decompiler output, compiler
payloads and build artifacts stay private. No tool job is live at checkpoint.

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
