# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; the outer
folder is not Git. Read AGENTS.md and current source/ledgers. Current target
bytes and receipts outrank historical notes. All dirty work belongs to this goal.

## Checked-in state

- Original Japanese TH11 v1.00a only; config/target.toml pins identity.
- 2,072 candidates;305 reviewed boundaries;245 source mappings.
- 139 exact functions /21,167 bytes:8 normal COFF and131 linked PE.
- Origins:90 authored,206 reviewed indeterminate,7 exclusions,1,769 pending.
- Direct local Ghidra/scripts; no Factory MCP/REA. VC9 RTM21022/SDK6.0A.
- Commit/push authorized; prefix `gpt-6.1-sol: `.

Latest checkpoint adds8 complete source mappings /1,755 bytes, including file713
and memory665 factories and six real/generated lifetime boundaries. Ordinary
base destructor133 and derived tail11 gain cold exact proof. Scalar-delete30/36
have zero diagnostics but fail unique MAP ownership; NO exact credit. Eight new
origin reviews recover five omitted secondary-script accessors. See
[MUSIC_FACTORIES](MUSIC_FACTORIES.md), [AUDIO_DEVICE](AUDIO_DEVICE.md),
[MUSIC_STREAM](MUSIC_STREAM.md).

## Current gaps and next work

| Owner | Target | Current source / diagnostic differences |
| --- | ---: | --- |
| CreateFileMusicStream,459EB0 | 713 | 675 /590; full EH closure open |
| CreateMemoryMusicStream,45A1B0 | 665 | 595 /556; full EH closure open |
| MusicSoundBaseConstructor,45A450 | 119 | 116 /83 |
| MusicStreamingConstructor,45AD60 | 48 | 48 /23 |
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

1. Real base0x38 /derived0x7C /decoder0x9C lifetimes are now implemented. Native
   refill uses EBX receiver plus two stack args/RET8; candidate keeps three on
   stack/RET12. Its public source now accepts MusicSoundBaseView. Base ctor's
   three-byte gap also depends on this private call. A genuine base-member
   refill probe after the new hierarchy remains untried; an older controller-
   member probe did not help. Do not force register ABI, dummy uses or padding.
2. Removing diagnostic entry's direct Read/Refill roots changes nothing, both
   before and after actual factory/lifetime context; rejected hypothesis.
   BYTE-pointer data and member-read probes were also unchanged. Canonical
   roots remain intact. Native read has ESI decoder/EAX bytes, two stack args/
   RET8; candidate uses additional EDX/EBX and RET.
3. Strict first cold attempt rejected scalar-delete30: MAP scalar/vector public
   aliases ??_G/??_E share its entry. Derived36 has the same ambiguity. Both
   source bodies are compiler-generated, not hand-written flag wrappers.
   Ordinary base133 and derived11 have unique full primary PDB/MAP owners and
   zero proofs; do not weaken the ambiguity Oracle to accept the scalar aliases.
4. Factories preserve prefix-only decoder initialization (record90, unknown0,
   size2C, memoryMode7C). File rejects ANY nonzero open HRESULT and deletes its
   decoder; memory ignores binder E_NOTIMPL after mutation. Other failure paths
   preserve native leaks/lifetimes. Source member methods have native32/36 stack
   cleanup. Sparse factory703/657 omit10/8 alignment; keep713/665 intervals.
5. Adjacent secondary-script frame helper45B610/584 now has complete boundary/
   origin review; sparse583 omits one alignment byte. Its6759-byte interpreter
   caller45B860 remains fully unreviewed and is a concrete next large target:
   inspect full flow/table/extents, not review's default250-instruction prefix.
   Five missing accessors45B5C0/D0/E0/F0/600 are maintained; trace45B5A0/B0 are
   actual RET-only entries, source-open. Do not invent formatting or authorship.
6. Keep earlier audio corrections: record1C=dataSize, loopStart18; buffer
   description38/device-owner5C/event74. Refill zeroes data/bytes/read before
   guards and propagates repeated-reset failure; first reset status ignored.
   Restore calls twice per retry, sleeps10 only for first BufferLost. Untyped
   Ghidra dropped real lost/loop/release paths; raw target flow wins. RTTI names
   and public protocols establish role, not original authored provenance.

Broader large work is still open: VM44B4B0/11,640, interpolation332/673/331,
renderer ctor2601/dtor225 EH, fan805/598, world1036, state572, submission1112,
billboard976, rotation571, projected792, fog794/546 and dispatcher217/table.
Four VM helper tables44B080/B2A0/B380/B400 need complete owner review.

## Evidence and replay scope

`.analysis/music-factory-thirty-one-affected-cold.log` passes exactly2 new plus29
affected old MusicStream/SoundQueue owners:31 complete public primary-TU PDB
contributions /2,885 bytes /65 fields, current source/header/tool/producer/PE/
MAP/PDB receipts and every byte zero-difference. Earlier33-owner attempt failed
as expected at the ambiguous scalar entry; the two invalid units were removed.
No unrelated family/full cohort, broad behavior or infrastructure tests ran.
Infrastructure unchanged. Source froze before final cold proof.

Final diagnostics: `.analysis/music-stream-explicit-factory-locals.log`,
`.analysis/music-lifetime-explicit-factory-locals.log` and
`.analysis/sound-queue-factory-lifetimes.log`. Topic doc lists fresh mapped-text
Ghidra attestations, missing-gap/reference queries, independent full extents/
fields and rejected probes. All jobs terminal; private inputs/artifacts stay private.

```bash
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py --require-target
```

Replay only changed exact owners/direct dependencies. Refresh progress, check
staged diff including new-file EOF, commit/push meaningful progress. Full native
product/runtime/portability remain later work; reconstruction goal stays active.
