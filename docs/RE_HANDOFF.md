# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; the outer
folder is not Git. Read AGENTS.md and current source/ledgers; target bytes and
current receipts outrank historical notes. All dirty work belongs to this goal.

## Checked-in state

- Original Japanese TH11 v1.00a; config/target.toml pins identity.
- 2,075 candidates;339 reviewed boundaries;272 source mappings.
- 151 exact functions /21,532 bytes:8 normal COFF and143 linked PE.
- Origins:90 authored,239 reviewed indeterminate,8 exclusions,1,738 pending.
- Direct local Ghidra/scripts; no Factory MCP/REA. VC9 RTM21022/SDK6.0A.
- Commit/push authorized; prefix `gpt-6.1-sol: `.

Latest checkpoint adds five complete program/context source mappings /610 target
bytes, including installer513. Three new exact owners add69 bytes. Twelve new
origin reviews add eleven indeterminate and one DirectInput import exclusion;
three missing entries are recovered and one false split is merged. See
[SECONDARY_PROGRAM](SECONDARY_PROGRAM.md), [SECONDARY_SCRIPT](SECONDARY_SCRIPT.md).

## Corrected boundaries and contracts

- Program vptr49404C: Install45D900 at0, integer-result extension40FFA0 at4.
  Derived494090 overrides4 with410D50 and adds file loading4104D0 at8. Do not
  invent a destructor at slot0. Base callback really returns0/RET4; derived has
 0/-1 paths. The physical header view is only the90-byte prefix, not full size.
- 410D50's old25-byte gate and410D69/213 are one238-byte ANIM/ECLI callback
  through410E3D. Gate JE shares the continuation's saved state and RET4 ABI.
  Full94-instruction span, all three returns/internal padding/neighbor CC and
  zero foreign interior references are independently reviewed. Only unmaintained
  auto rows were merged; private database unchanged. Derived source remains open.
- Missing45DBE0/28,45DC00/36,45DD50/28 are genuine entries separated by CC.
  Initialize publishes program and returns0 unconditionally; Select uses an
  explicit program without publishing it and returns instruction==null; Restart
  uses stored program and returns0. All write through existing current/reset
  time. Do not initialize current or add null guards absent from native code.
- Interpreter45B860 owns7,093 through45D414:6,759 code+1 native NOP+244-byte
  jump table+89 selectors. Full1,783-instruction review; all61 table DATA edges
  are internal. Never crop to6,759 or create a function at table cell45D2D0.
- Browser438AC0/934 requires the full267-instruction export, not its truncated
 250 view. Neighbor45DD70/188,45DE30/177,45DF10/84 are animation effects, not
  script lifecycle. JMP [48B000] at45DF64 is PE-verified DirectInput8Create.

## Current gaps and next work

| Owner | Target bytes | Current source / diagnostic differences |
| --- | ---: | --- |
| InstallSecondaryProgram,45D900 | 513 | 513 /339; all branches retained |
| RestartSecondaryScript,45DD50 | 28 | 28 /10; argument/register scheduling |
| RunSecondaryScript,45B860 | 7,093 | 6,989 /5,958; complete code/tables |
| EnterSecondaryScriptFrame,45B610 | 584 | 572 /546 |
| UpdateSecondaryScripts,45D420 | 142 | 155 /142 |
| ReadSecondaryFloatArgument,45D550 | 175 | 184 /168 |
| ReadSecondaryIntArgument,45D4B0 | 146 | 124 /125 |
| ReadSecondaryIntReference,45D600 | 100 | 94 /85 |
| ReadSecondaryFloatReference,45D670 | 145 | 147 /85 |
| PopSecondaryValue,45DC80 | 83 | 83 /2; ADD-negative versus SUB |
| PushSecondaryValue,45DC30 | 71 | 70 /15 |
| EndSecondaryStackFrame,45DD20 | 41 | 41 /2; ADD-negative versus SUB |
| StartSecondaryThread,45D7A0 | 147 | 154 /61 |

1. Continue substantial installer/interpreter codegen and genuine source-open
   neighbors. Installer now uses separate magic/version checks and strlen,
   reducing506/446 to513/339. Remaining register selection, allocation expression,
   redundant old-pointer gate, call cleanup/internal alignment are real gaps.
   Preserve publication, sorted insertion through the full allocated count,
   original failures and ignored extension result; do not add guards.
2. Typed pop's non-float-first branch closes27 differences to2. Genuine Stack
   member Pop/EndFrame changes neither encoding gap and is rejected. Source stays
   ordinary helpers; no register/ABI declarations, patching or padding. Shared
   inline changes affect larger diagnostics and require the nine family receipts.
3. Interpreter still has native61 jump cells/89 selectors but a64-byte-aligned
   EBP frame versus native ESP/120. /Zp4 changes none. Private double-to-float
   temporaries keep alignment64 and produce6873/5934 against the preceding source;
   rounding could change, so rejected. Doubles remain. Inspect actual spill and
   call/return contracts before changing types. No alignment-forcing tricks.
4. Keep established behavior: opcode85 integer-negates float bits; raw versus
   argument underflow sentinels differ. Failed frame lookup leaves new current
   published and clears caller instruction. Geometry86 rounds inputs, resolves
   destination pointer, THEN computes. Trace int-to-double adds no float rounding.
   Stack top/frame1000/1004; VM top/frame1008/100C. Old frame-offset getter names
   were wrong; genuine relative-address/setter members are already exact.

Earlier audio gaps remain in [MUSIC_FACTORIES](MUSIC_FACTORIES.md),
[AUDIO_DEVICE](AUDIO_DEVICE.md), [MUSIC_STREAM](MUSIC_STREAM.md). Factories713/665
retain675/590 and595/556; refill570 remains569/504. Scalar-delete30/36 diagnostic0
still fail unique MAP ownership through ??_G/??_E aliases. No exemption. Broader
VM44B4B0/11,640, renderer/EH, interpolation and geometry remain open.

## Current proof and replay scope

`.analysis/secondary-program-affected-cold.log` passes exactly three new program
plus nine directly affected prior script units. New69 bytes/two REL32 fields and
old296 bytes/zero fields are complete unique public primary-TU contributions;
current source/header/support/tool/producer/PE/MAP/PDB receipts and exhaustive
full-byte replay pass. Both new calls independently target45DB10. No other family,
full cohort, broad behavior harness or infrastructure suite ran. Infrastructure
unchanged; source frozen before cold proof; all tool jobs terminal.

Current diagnostics: secondary-program-default-callback and secondary-script-pop-order
logs; corresponding build/probe-ltcg diagnostic.json files. Topic docs enumerate
full-code/span Ghidra reviews, independent vtables/import/fields, rejected probes
and cold receipts. Private target/decompiler/tool/build artifacts stay private.

```bash
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py --require-target
```

Replay changed units/direct dependencies only. SecondaryProgram depends on
SecondaryScript.cpp as real support; changing that support affects its three
canonical units too. Refresh progress, check staged diff including new-file EOF,
commit/push meaningful progress. Native product/runtime/portability remain later
work; reconstruction goal stays active.
