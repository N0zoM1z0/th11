# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; the outer
folder is not Git. Read AGENTS.md and current source/ledgers; target bytes and
current receipts outrank historical notes. All dirty work belongs to this goal.

## Checked-in state

- Original Japanese TH11 v1.00a; config/target.toml pins identity.
- 2,073 candidates;327 reviewed boundaries;267 source mappings.
- 148 exact functions /21,463 bytes:8 normal COFF and140 linked PE.
- Origins:90 authored,228 reviewed indeterminate,7 exclusions,1,748 pending.
- Direct local Ghidra/scripts; no Factory MCP/REA. VC9 RTM21022/SDK6.0A.
- Commit/push authorized; prefix `gpt-6.1-sol: `.

Latest checkpoint adds22 complete secondary-script source mappings /9,160 target
bytes, including interpreter7,093 and frame transfer584. Nine new exact owners
add296 bytes. Twenty-two new origin reviews plus six earlier rechecks cover28
owners; the missing83-byte typed pop is recovered. All stay indeterminate. See
[SECONDARY_SCRIPT](SECONDARY_SCRIPT.md).

45B860 originally recorded6,759 code-only bytes. Correct owner is7,093 through
45D414:6,759 code+1 native NOP+244-byte jump table+89 selectors. All61 Ghidra
DATA edges agree with raw PE entries and are internal to corrected owner. Do
not restore the old span or classify a table cell45D2D0 as a function. Full
1,783-instruction export and independent tables are private evidence.

## Current gaps and next work

| Owner | Target bytes | Current source / diagnostic differences |
| --- | ---: | --- |
| RunSecondaryScript,45B860 | 7,093 | 6,845 /5,930; complete code/tables |
| EnterSecondaryScriptFrame,45B610 | 584 | 572 /546 |
| UpdateSecondaryScripts,45D420 | 142 | 155 /142 |
| ReadSecondaryFloatArgument,45D550 | 175 | 179 /162 |
| ReadSecondaryIntArgument,45D4B0 | 146 | 124 /125 |
| ReadSecondaryIntReference,45D600 | 100 | 94 /85 |
| ReadSecondaryFloatReference,45D670 | 145 | 142 /83 |
| PopSecondaryValue,45DC80 | 83 | 83 /27 |
| PushSecondaryValue,45DC30 | 71 | 70 /15 |
| EndSecondaryStackFrame,45DD20 | 41 | 41 /2; ADD-negative versus SUB |
| StartSecondaryThread,45D7A0 | 147 | 154 /61 |

1. Continue full interpreter codegen. Candidate has native61 jump cells/89
   selectors, but an EBP frame aligned64 versus native ESP/120. /Zp4 changes
   none and is rejected. A bounded next hypothesis is the explicit double
   temporaries: investigate generated spill/rounding/call order before changing
   their types. Do not force alignment, registers, padding or clip tables.
   Grouped arithmetic emitted3382; independent operation instances plus distinct
   branch/thread/unary arms are required for the native dispatch shape.
2. Pop83 preserves write-output, decrement/read-tag, then convert ordering;
   underflow leaves output unchanged. Its current source checks float first;
   native lays out non-float conversion first. A natural branch-order probe is
   concrete next work, with full83-byte comparison. End41's two encoding bytes
   and push71's missing pointer ADD remain real gaps, not acceptance masks.
3. Corrected physical stack fields: top1000/frame1004; VM stack starts8,
   top1008/frame100C, id1010, owner1014, extra1018, difficulty101C, flags1020.
   Old getters called frame offsets actually access top. Genuine Stack members
   RelativeAddress/SetTop close15/7; no invented calling convention is used.
4. Keep actual semantics: opcode85 integer-negates float bits, not floating
   value; raw/argument underflow sentinels differ. Failed frame lookup leaves
   new manager.current published and clears caller instruction. Geometry86
   rounds fetched coordinates, resolves output pointer, THEN computes. Trace
   int-to-double conversion must not acquire an extra float rounding. Never
   replace real failure paths with guards/defaults or paste decompiler casts.
5. Source-open neighbors with complete reviews: program install513, manager
   creation77, polar vector30 and math wrappers31/34/31. Program header SCPT/
   version1 and sorted name table are observed, not authorship evidence.
   Manager's five actual virtual slots cover host op/int/intptr/float/floatptr;
   45D600 tail slot4 is not a jump table. No guessed bodies supplied.

Earlier audio work remains in [MUSIC_FACTORIES](MUSIC_FACTORIES.md),
[AUDIO_DEVICE](AUDIO_DEVICE.md), [MUSIC_STREAM](MUSIC_STREAM.md). File/memory
factories713/665 retain675/590 and595/556 gaps; refill570 remains569/504.
Scalar-delete30/36 diagnostic0 still fail unique MAP ownership through ??_G/??_E
aliases. Only ordinary base133/derived11 are exact. Do not weaken that rule.
Broader VM44B4B0/11,640, renderer/EH, interpolation and geometry work remains open.

## Current proof and replay scope

`.analysis/secondary-script-nine-cold.log` passes exactly nine new complete unique
public primary-TU PDB contributions /296 bytes /zero address fields. Every byte
is raw-equal and current source/header/tool/producer/PE/MAP/PDB receipts pass.
No old exact source/context/dependency changed; no older family replayed. No
full cohort, broad behavior harness or infrastructure suite ran. Infrastructure
is unchanged. Source froze before cold proof; all tool jobs are terminal.

Final diagnostic: `.analysis/secondary-script-distinct-arms.log` and
`build/probe-ltcg/SecondaryScript/distinct-arms/diagnostic.json`. Topic doc lists
full-code/owner/support/math Ghidra attestations, missing-pop gap, independent
complete spans/tables and controlled probes. Private bytes/decompiler/tool/build
artifacts stay private; no database mutation or inventory regeneration.

```bash
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py --require-target
```

Replay changed exact owners/direct dependencies only. Editing this new primary
TU affects its nine canonical units, not every previous family. Refresh progress,
check staged diff including new-file EOF, commit/push meaningful progress. Whole
native product/runtime/portability remain later work; reconstruction stays active.
