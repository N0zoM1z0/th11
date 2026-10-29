# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; the outer
folder is not Git. Read AGENTS.md/current source/ledgers. Target bytes and current
receipts outrank historical notes. All dirty work belongs to this goal.

## Checked-in state

- Original Japanese TH11 v1.00a; config/target.toml pins identity.
- 2,097 candidates;385 reviewed boundaries;291 source mappings.
- 168 exact functions /22,118 bytes:8 normal COFF and160 linked PE.
- Origins:90 authored,283 indeterminate,10 exclusions,1,714 pending.
- Direct local Ghidra/scripts; no Factory MCP/REA. VC9 RTM21022/SDK6.0A.
- Commit/push authorized; prefix `gpt-6.1-sol: `.

Latest checkpoint adds14 complete source mappings/355 target bytes. Twelve
unique primary-TU owners close246 bytes/eight fields. Thirty-six new origin
reviews add34 indeterminate and two compiler exclusions;15 missing entries
recovered. Large actor update4230, constructor699 and destructor384 have full
reviews and remain source-open. See [SECONDARY_LIFETIME](SECONDARY_LIFETIME.md),
[SECONDARY_EXTENSION](SECONDARY_EXTENSION.md), [SECONDARY_PROGRAM](SECONDARY_PROGRAM.md).

## Corrected models and ownership

- Program storage is1098: known prefix through8C, raw90..108F and observed
  DWORD1090/1094. Unknown bytes are actual cleared storage, not instruction padding
  or guessed ownership. Inline base construction writes49404C then memset erases
  it. Empty derived construction naturally inlines zeroing and publishes494090
  afterward; derived38 is exact. Base38/3 remains support-owned/non-exact.
- Program destructor40FFF0/36 is nonvirtual and frees only the table. Master
  cleanup410E90/374 frees all32 file slots then program/table; do not move that
  ownership. Program vtable slots remain Install0/extension4, derived file8.
- Manager494058 has five real zero/null/FLDZ defaults and virtual deleting
  destructor14. The former pure-virtual-only model was incomplete. Five defaults
  and ordinary destructor40FF10/47 are exact. Scalar deleting40FF40/71 is
  source-backed but candidate30/62 calls ordinary47 and shares ??_G/??_E address;
  strict ownership/extent remains open. No custom deleting wrapper.
- Actor object is base103C+state163C=2678. Ordinary adapter411600 is12 bytes,
  not13 including adjacent CC. It calls complete state update411750 on object
  +103C and is exact. It is not an EH funclet: actual FuncInfo49FB78/unwind49FB70
  points48A730/8, which jumps40FF10. Handler48A738/27 selects FuncInfo after GS
  cookie check; SafeSEH49E960/56 independently includes it. Both compiler entries
  excluded; main EH-bearing actor destructor384 remains source/acceptance-open.
- Actor-state update411750..4127D5 owns4230, including9 alignment bytes omitted
  by sparse4221. Full1108 instructions/five returns, no tables or foreign interior
  entries; all flow reviewed. Initial250-instruction prefix was insufficient.
- Interpreter45B860 owns7093 through45D414:6759 code+1 NOP+244 jump+89 selectors.
  Full1783-instruction review;61 table DATA edges are internal. Never crop to6759,
  restore false callback410D69 or create a function at table cell45D2D0.

## Current gaps and concrete next work

| Owner | Target bytes | Current source / diagnostic differences |
| --- | ---: | --- |
| InstallSecondaryProgram,45D900 | 513 | 513 /339 |
| SecondaryProgramBaseConstructor,40FFC0 | 38 | 38 /3; support-owned |
| SecondaryManagerDeletingDestructor,40FF40 | 71 | 30 /62; ??_G/??_E aliases |
| RestartSecondaryScript,45DD50 | 28 | 28 /10 |
| RunSecondaryScript,45B860 | 7093 | 6993 /5880; full code/tables |
| EnterSecondaryScriptFrame,45B610 | 584 | 572 /546 |
| UpdateSecondaryScripts,45D420 | 142 | 155 /142 |
| ReadSecondaryFloatArgument,45D550 | 175 | 184 /168 |
| ReadSecondaryIntArgument,45D4B0 | 146 | 124 /125 |
| ReadSecondaryIntReference,45D600 | 100 | 94 /85 |
| ReadSecondaryFloatReference,45D670 | 145 | 147 /85 |
| PopSecondaryValue,45DC80 | 83 | 83 /2; ADD-negative vs SUB |
| PushSecondaryValue,45DC30 | 71 | 70 /15 |
| EndSecondaryStackFrame,45DD20 | 41 | 41 /2; ADD-negative vs SUB |
| StartSecondaryThread,45D7A0 | 147 | 154 /61 |

1. Continue full installer/interpreter and actor lifetimes/state update. Installer
   keeps full-count insertion, publication and ignored extension result; remaining
   registers, allocation expression, free gate and alignment are real gaps.
   Actor4230 flow covers motion/tweens, script/hooks, damage/counter, eight VM IDs,
   mesh deformation and timers/status. Source remains open; do not replace it with
   a bounded prefix or guessed simplified loop. Actor699/384 and master374 offer
   real construction/cleanup context; EH ownership is separate.
2. Interpreter64 alignment is localized to its double variadic trace call, whose
   native QWORD argument must remain. Ablations omit behavior and are rejected.
   Fixed-format/real trace bodies, /Zp4, /fp:fast and float temporaries failed.
   New split calls6993/5877, float conditional7001/5937 and format arms7141/5984
   all retain64; none adopted. Investigate real call/spill/rounding/TU context,
   not forced registers/alignment or changed precision. Canonical O2/Ob1/GL.
3. Keep exact semantics: opcode85 integer-negates float bits; reference/argument
   underflow differs; frame lookup failure leaves new current published and clears
   caller instruction. Geometry86 resolves destination before arithmetic. Counter
  61/13/16 retain wrapped7 arithmetic/truncated division and input low-bit semantics.
4. Existing audio scalar30/36 alias gaps stay open; no exemption. Factories713/665
   retain675/590 and595/556, refill570 remains569/504. Broader VM44B4B0/11640,
   renderer/EH/interpolation/geometry remain open.

## Current proof and affected scope

`.analysis/secondary-lifetime-affected-cold.log` passes12 new+16 affected prior
units acrossfive contexts. Actual source/header receipt dependencies select only
these; no full cohort or unrelated family replay. New246 bytes/eight independent
fields pass complete unique public primary-TU ownership, current source/header/
support/tool/producer/PE/MAP/PDB receipts and every byte. Source froze before cold
builds; all jobs terminal. Build/Oracle tools unchanged; no broad behavior or
infrastructure suite.

Current diagnostics: secondary-script-default-virtuals, secondary-program-frozen-lifetime,
secondary-extension-frozen-lifetime, secondary-counter-typed-prefix and
secondary-actor-full-adapter logs/diagnostic.json. Topic doc lists full Ghidra,
independent38-span/CFG/vtable/EH/field evidence and rejected probes. Private
bytes/decompiler/tool/build artifacts stay private; DB unchanged.

Replay changed exact owners/direct dependencies only. Script/Program sources
support the Extension family; inspect actual receipts including transitive headers.
Refresh progress, check staged diff including new-file EOF, commit/push meaningful
progress. Native product/runtime/portability remain later work; goal stays active.
