# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; outer folder
is not Git. Read AGENTS.md, this file, relevant current source and ledger rows.
Target bytes/current receipts outrank older notes. All dirty work is ours.

## Current state

- Original Japanese TH11 1.00a, pinned by `config/target.toml`.
- 2,136 candidates;523 reviewed boundaries;394 source mappings.
- 217 exact functions/24,578 bytes:9 normal COFF and208 linked PE.
- Origins:90 authored,417 indeterminate,14 exclusions,1,615 pending.
- VC9 RTM21022/SDK6.0A; direct Ghidra/scripts, no Factory MCP/REA.
- Commit/push authorized, prefix `gpt-6.1-sol: `.
- Latest user task: clean code/docs and disposable outputs. No reconstruction
  job is left running at this checkpoint.

Latest reconstruction adds six mappings/3166 bytes: full controller2888, real
callbacks7/10 and helpers54/101/106. Three linked helpers close261 bytes/five
fields; update callback closes7 normal-COFF bytes/one relocation. Thirteen new
boundary/origin reviews cover5975 bytes/1585 instructions/23 returns, including
two omitted callbacks. All new origins remain indeterminate.

## Immediate continuation

| Area | Current evidence / next useful work |
| --- | --- |
| Controller431070/2888 |Complete source; current2800/2285. Real normal-COFF callback support restores stack owner/two RET4; frame/vector stores remain open |
| Movement430290/2896, death4327D0/703 |Full boundaries reviewed, source-open; genuine callees for further controller work |
| Orbit4336A0/255, aim4337B0/495 |Full boundaries reviewed, source-open; use real callback publications/ECX context |
| Reset432CC0/2508 |Complete source2444/1967; main factory sites1 versus native4 |
| Publishers455760/118,4557E0/123 |111/33 and116/47; ID zero-check/register allocation unresolved |
| Spawn433F90/853 |Complete source; real Sound846/733 and real-only/independent RNG856/690 both fail |
| Actor state411750/4230 |Complete source4161/3261 in full support context; frame/private allocation remains open |
| Damage4347F0/1579, projectile434440/896 |Complete sources; last1513/1414 and908/765 in their recorded contexts |
| Interpreter45B860/7093 |Complete source6993/5880; required variadic double trace still aligns candidate64 versus native ESP/120 |

Do not repeat the already completed Sound experiment as an untried next step.
Scoped reset temporaries give2556/1977 and six factory sites versus native four;
signed/wrapped counter variants leave publishers unchanged. Neither is adopted.
Avoid volatile reloads, fabricated ABI, constructors, padding or forced frames.

## Corrections that matter

- Projectile6C: acceleration64, parameters68. Descriptor0/1 are signed period/
  phase; negative period ends a list and zero period remains unguarded.
- Eight E4 emitter objects begin7570 and end7C90;75CC is coordinate prefix+5C.
  Reset branch3 focused base is148; branch4 writes seven pairs from configuration
  bases28/28/28/78/C8/118/168. Branch2 keeps prior integer offsets.
- Controller takes one actual stack owner. Aim101 takes owner AND a real vector;
  its caller constructs `(0, position.y-224, 0)`. Radius clears have actual
  position inputs beyond their three stack words. Drop allocation has six real
  logical inputs. Decompiler extraout values are not dummy source parameters.
- Callbacks431C50/7 and431C60/10 are published at42F962/42F9B7 to chain+8/context+20.
  Update is exact COFF; draw remains7 versus10. No established LTCG primary owner
  was changed to gain COFF credit; original TU identity remains unknown.
- Special406510 owns268 including six entries; drop424230 owns851 including two
  entries AND11 selector bytes; movement430290 owns2896 including two eight-entry
  tables and18 internal padding bytes beyond sparse2811. Their24 case refs are
  owned data. Death703 includes7 internal padding bytes.
- Reset2508 includes two switch tables; actor state4230 includes9 internal padding
  bytes; interpreter7093 includes its jump/selector data. Never crop these owners.

## Current proof and retained artifacts

`.analysis/secondary-player-controller-final-proof.json` records four exact new
units,268 bytes total. Linked helpers have unique complete primary PDB owners;
normal-COFF callback has a complete7-byte extent and one reviewed relocation.
Both float pi/2 payloads agree independently with497F90. Source freeze is intact.
Controller source/native call audit agrees on22 direct targets/38 call sites and
two RET4 in the real caller context; this does not prove controller exactness.

The final bound-operand cleanup changes only controller support source. Only its
three affected linked units were cold-replayed; unchanged callback COFF receipt
was revalidated directly. Earlier canonical families do not depend on these five
new files. No full cohort or broad behavior/Oracle suite was run.

Cleanup removed2047 compiled/cache files and716 unused code copies/drivers,
freeing219.0 MiB allocated space. Documents and all diagnostic/native review
records are retained. The deletion audits verify protected files remain identical;
installed tools and canonical receipts remain intact. Old code/driver paths in
historical records are not dependencies. See [ARTIFACTS](ARTIFACTS.md).

Start detailed work at [PLAYER_CONTROLLER](SECONDARY_PLAYER_CONTROLLER.md),
[EMITTER_RESET](SECONDARY_EMITTER_RESET.md),
[PATTERN_CALLBACKS](SECONDARY_PATTERN_CALLBACKS.md). Other concrete gaps and
corrected layouts are indexed in [KNOWLEDGE_BASE](KNOWLEDGE_BASE.md).
Older checkpoint narratives are explicitly [historical](archive/KNOWLEDGE_BASE_2026-10-30.md).
Product/runtime/port closure remains later work.
