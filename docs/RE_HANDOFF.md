# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; the outer
folder is not Git. Read AGENTS.md and current source/ledgers; current target
bytes and receipts outrank historical notes. All dirty work is ours.

## Checked-in state

- Original Japanese TH11 v1.00a only; identity in config/target.toml.
- 2,067 candidates;297 reviewed boundaries;237 source mappings.
- 137 exact functions /21,023 bytes:8 normal COFF and129 linked PE.
- Origins:90 authored,198 reviewed indeterminate,7 exclusions,1,772 pending.
- Direct local Ghidra/scripts; no Factory MCP/REA. VC9 RTM21022/SDK6.0A.
- Commit/push authorized; prefix `gpt-6.1-sol: `.

Latest device checkpoint adds5 complete source mappings /719 bytes and9 complete
exact owners /1,103 bytes. Those include primary-format setup272 and buffer
recreation353. Twelve new boundary/origin reviews recover7 missing entries;
all indeterminate. Both factories713/665 reviewed; source/EH remains open.
See [AUDIO_DEVICE](AUDIO_DEVICE.md), [MUSIC_STREAM](MUSIC_STREAM.md).

45A4F0 is buffer/notification recreation, not decoder open. Earlier label
OpenMusicDecoder was wrong; current source/caller/inventory use
RecreateMusicBuffers. Record1C is dataSize, signed loopStart18. Controller38 is
36-byte buffer description,5C device-owner pointer,74 notification event.
Device66 ignores primary272 HRESULT after successful creation/cooperative setup.
RTTI names CSound/CStreamingSound and public protocols do not prove provenance.

## Current gaps and concrete next work

| Owner | Target | Current source / diagnostic differences |
| --- | ---: | --- |
| RefillMusicBuffer,45A6F0 | 570 | 569 /504 |
| UpdateMusicStream,45AEE0 | 543 | 544 /421 |
| ReadMusicDecoder,45B480 | 192 | 160 /177 |
| ResetMusicPosition,45B100 | 223 | 220 /160 |
| PlayMusicStream,45AA60 | 133 | 132 /18 |
| ProcessSoundQueue,44A340 | 1,460 | 1,524 /1,083; full code/table |
| FirstMusicBuffer,45AA00 | 19 | 19 /13 |
| UpdateAnimationRenderer,455480 | 221 | 213 /170; native8-byte alignment absent |
| UpdateOtherAnimationRenderer,455560 | 249 | 249 /195; same gap |
| StepAnimationIntVectorTween,44E4B0 | 768 | 768 /345; frame48/native40 |

1. Continue large streaming/read context. Native refill uses EBX receiver and
   buffer/loop on stack, RET8; candidate keeps three args on stack, RET12.
   Native read has ESI decoder/EAX bytes, destination/output on stack, RET8;
   candidate additionally uses EDX/EBX and RET. Genuine refill/member-read
   experiments did not change code. BYTE-pointer data probe also changed none.
   One concrete untried context probe: omit diagnostic entry's direct calls to
   Read/Refill while keeping their actual Play/Update/queue caller graph. Target
   Read has only its four refill/update sites; the current diagnostic adds one.
   Do not force registers, add dummy uses, crop extents or inject padding.
2. Refill re-review corrected actual missing semantics: zero data/bytes/read
   before guards, propagate repeated decoder-reset failure; first reset status
   is ignored. Keep both distinctions. Restore calls twice per retry iteration;
   sleep10 only for first BufferLost. Untyped Ghidra wrongly removed lost/loop
   and notification release branches. Raw full flow is authoritative.
3. Stop129/Resume77 now exact: decoder snapshot across CloseHandle and first
   buffer snapshot before active publication matter. Selector100/any-playing78
   also exact after branch/initialization order corrections. Full new COM
   protocol types retain real stdcall calls; no implementation was imported.
4. Read snapshots cursor before clearing optional count. File branch subtracts
   remaining before ReadFile and ignores BOOL. Binder43 really writes state
   then E_NOTIMPL. Keep archive/loop/seek formulas and actual failure lifetime.
5. Source lifetimes remain open: base ctor119/dtor133/scalar-delete30, streaming
   ctor48/scalar-delete36/tail11. Factories713/665 include10/8 internal alignment
   omitted from sparse Ghidra703/657. Preserve full intervals/EH ownership.
   Broader large work remains: VM interpreter44B4B0/11,640, interpolation332/673/
   331, renderer ctor2601/dtor225 EH, fan805/598, world1036, state572,
   submission1112, billboard976, rotation571, projected792, fog794/546 and
   dispatcher217/table. Review four VM helper tables44B080/B2A0/B380/B400 fully.

## Current evidence and replay scope

`.analysis/audio-device-twenty-nine-affected-cold.log` passes exactly9 new plus20
affected old MusicStream/SoundQueue owners:29 complete public primary-TU PDB
contributions /2,741 bytes /59 fields, full zero byte differences and current
source/header/tool/producer/PE/MAP/PDB receipts. New SoundDevice.hpp and existing
MusicStream.hpp declared in all affected contexts. No other family/full cohort,
broad behavior suite or infrastructure tests ran; infrastructure unchanged.

Final diagnostics: `.analysis/music-stream-refill-reviewed-outputs.log` and
`.analysis/sound-queue-device-recreate.log`. Topic doc lists fresh full mapped-
text Ghidra attestations, explicit missing-prefix/reference queries, independent
full extents/fields/protocol/import review and rejected probes. All jobs are
terminal; source froze before cold proof. Private inputs/artifacts stay private.

```bash
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py --require-target
```

Replay changed exact units and directly affected dependencies only. Refresh
tracking/progress, check staged diff (including new-file EOF), commit and push
meaningful progress. Native product/runtime/portability remain later work;
the full reconstruction goal remains active.
