# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; the outer
folder is not Git. Read AGENTS.md and current source/ledgers. Target bytes and
current receipts outrank historical notes. All dirty work belongs to this goal.

## Checked-in state

- Original Japanese TH11 v1.00a; config/target.toml pins identity.
- 2,082 candidates;349 reviewed boundaries;277 source mappings.
- 156 exact functions /21,872 bytes:8 normal COFF and148 linked PE.
- Origins:90 authored,249 reviewed indeterminate,8 exclusions,1,735 pending.
- Direct local Ghidra/scripts; no Factory MCP/REA. VC9 RTM21022/SDK6.0A.
- Commit/push authorized; prefix `gpt-6.1-sol: `.

Latest checkpoint closes full ANIM/ECLI extension238 and file entry95, plus
file getter5 and two actual RET-only trace bodies. Five new complete source and
exact owners add340 bytes/ten fields. Ten new indeterminate origin reviews
recover seven omitted entries. See [SECONDARY_EXTENSION](SECONDARY_EXTENSION.md),
[SECONDARY_PROGRAM](SECONDARY_PROGRAM.md), [SECONDARY_SCRIPT](SECONDARY_SCRIPT.md).

## Corrected boundaries and contracts

- Program vptr49404C: Install45D900 at0, integer-result extension40FFA0 at4.
  Derived494090 overrides4 with410D50 and adds filename loading4104D0 at8.
  File getter40FFB0/5 is nonvirtual and does not change slot numbering.
- 410D50/25 and410D69/213 were one238-byte callback through410E3D. All three
  RET4 paths/internal padding and no foreign interior entries are reviewed.
  Full source is now exact. Never restore the obsolete second candidate.
- Base constructor40FFC0/38 writes49404C then memset1098 erases it. Derived
 410D20/38 memset1098 first then writes494090. Both are newly recovered and
  source-open. Browser's unusual base vptr/zeroing order was real. Do not repair
  it or mistake the current90-byte header view for the full1098 allocation.
- Missing singleton helpers410530/11,410540/9,410550/10,410560/21 only establish
  stores at40/1C, read68 or flag3C bit0 replacement. Full type/roles remain open.
  Host initialize410BE0/318 and reset410440/126 now have complete reviews.
- Interpreter45B860 owns7093 through45D414:6759 code+1 native NOP+244-byte
  jump table+89 selectors. Full1783-instruction review; all61 table DATA edges
  are internal. Never crop to6759 or create a function at table cell45D2D0.
- Context45DBE0/28 publishes program and returns0;45DC00/36 uses explicit program
  without publishing it and returns instruction==null;45DD50/28 uses stored
  program and returns0. All write through existing current/reset time. No guards
  or current initialization absent from native code.

## Current gaps and concrete next work

| Owner | Target bytes | Current source / diagnostic differences |
| --- | ---: | --- |
| InstallSecondaryProgram,45D900 | 513 | 513 /339; all branches retained |
| RestartSecondaryScript,45DD50 | 28 | 28 /10; argument/register scheduling |
| RunSecondaryScript,45B860 | 7093 | 6993 /5880; complete code/tables |
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

1. Continue the full installer/interpreter and source-open host/constructors.
   Installer's remaining registers, allocation expression, redundant old-pointer
   gate, call cleanup/alignment are real gaps. Keep file-slot publication,
   full-count insertion, ignored extension result and original failures.
2. Interpreter64-byte alignment is localized to the double variadic trace call.
   Full trace-arm ablation gives6541/5989 with alignment8; deleting only double
   trace gives6965/5960 and8. Both omit real behavior and are rejected. Native
   trace really receives a QWORD. Real/fixed-format trace bodies and /fp:fast
   retain64; fast gives6961/5940 and is rejected. Canonical keeps real doubles
   and /O2 /Ob1 /GL. Investigate remaining call/spill/rounding context, not forced
   alignment or fake ABI. Earlier /Zp4 and float-temporary probes also failed.
3. Real traces are RET-only, not placeholders. Animation trace needs all supplied
   values variadic: an ignored fixed format parameter is trimmed by LTCG and
   loses native pushes. Its body preserves EAX/ECX across cached acquire calls.
   New extension context supplies actual animation/resource/log code and variable
   resource size/mode to avoid specialization. Do not remove these real roots.
4. Typed pop's non-float-first branch leaves83/2; true Stack-member Pop/EndFrame
   probes change neither encoding gap. Keep bytes unmasked. Opcode85 negates float
   bits with integer NEG. Underflow sentinels differ. Failed frame lookup leaves
   new current published and clears caller instruction. Geometry86 rounds inputs,
   resolves destination THEN computes. Stack top/frame1000/1004; VM1008/100C.

Earlier audio gaps remain in [MUSIC_FACTORIES](MUSIC_FACTORIES.md),
[AUDIO_DEVICE](AUDIO_DEVICE.md), [MUSIC_STREAM](MUSIC_STREAM.md). Factories713/665
retain675/590 and595/556; refill570 remains569/504. Scalar-delete30/36 diagnostic0
still fail unique MAP ownership through ??_G/??_E aliases; no exemption. Broader
VM44B4B0/11640, renderer/EH, interpolation and geometry remain open.

## Current proof and affected scope

`.analysis/secondary-extension-affected-cold.log` passes five new plus79 affected
prior units across26 compilation contexts. Selection uses actual receipt input
hashes including transitive AnimationDocument.hpp includes, not just explicit
manifest inputs. No unrelated unit or full cohort replay. New340 bytes/ten
independently bound fields have complete unique public primary-TU PDB owners,
current source/header/support/tool/producer/PE/MAP/PDB receipts, exhaustive replay
and zero differences. Actual program/trace files froze before their cold builds;
all tool jobs terminal. Build/Oracle infrastructure unchanged; no broad behavior
or infrastructure suite.

Current diagnostics: secondary-extension-inline-offset, secondary-program-file-accessor,
secondary-script-trace-source, animation-trace-full-source logs and corresponding
build/probe-ltcg diagnostic.json. Current Run61/89 table shape independently
verified. Topic doc lists full Ghidra spans, source/ABI probes and raw field
review. Original bytes/decompiler/tool/build artifacts stay private.

Replay only changed exact units/direct dependencies. AnimationDocument.hpp is
transitively included by many contexts; inspect actual receipt dependencies.
Program/Script sources also support the new Extension family. Refresh progress,
check staged diff including new-file EOF, commit/push meaningful progress.
Native product/runtime/portability remain later work; goal stays active.
