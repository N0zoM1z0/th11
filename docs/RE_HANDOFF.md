# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; outer folder
is not Git. Read AGENTS.md, this file, relevant source and ledger rows. Target
bytes/current receipts outrank older notes. All dirty work is ours.

## Current exact state

- Original Japanese TH11v1.00a, pinned by `config/target.toml`.
- 2,136 candidates;526 reviewed boundaries;403 source mappings.
- 223 exact functions/25,597 bytes:9 normal COFF and214 linked PE.
- Origins:90 authored,420 indeterminate,14 exclusions,1,612 pending.
- VC9 RTM21022/SDK6.0A; direct Ghidra/scripts, no Factory MCP/REA.
- Commit/push authorized, prefix `gpt-6.1-sol: `.
- No reconstruction job remains running at this checkpoint.

Previous work added seven complete mappings/1,722 bytes. Six cold linked proofs
close1,019 bytes/35 fields: orbit255, aim495, callbacks12/12, angle difference101
and life HUD144. Death703 has complete source715/423 and remains non-exact.
Two additional boundary/origin reviews cover246 bytes/74 instructions/two RET4;
both origins remain indeterminate. See [EMITTER_MOTION](SECONDARY_EMITTER_MOTION.md).

Latest continuation adds complete effect allocator229/penalty102 source and one
boundary/origin review. Their current228/37 and102/14 diagnostics remain non-exact;
the real death caller gives the same results as independent manager/position
inputs. Earlier exact sources/headers/contexts are unchanged; no cold replay needed.

## Immediate continuation

| Area | Concrete remaining work |
| --- | --- |
| Movement430290/2896 | Full boundary reviewed, source-open; next substantial controller callee |
| Death4327D0/703 | Source715/423 in real controller/HUD/Bind context; callee-save placement, global stores and emitter-loop layout |
| Effect422560/229, penalty42F6F0/102 | Complete source228/37 and102/14; allocator position EDI versus native EBX and mode/lifetime stores |
| Controller431070/2888 | Source2800/2285 in previous context; actual normal-COFF callback support preserves stack owner/two RET4; frame/vector stores |
| Reset432CC0/2508 | Source2444/1967; main factory sites1 versus native4 |
| Publishers455760/118,4557E0/123 |111/33 and116/47; ID zero-check/register allocation |
| Spawn433F90/853 | Real Sound846/733 and real-only/independent RNG856/690 both fail |
| Actor state411750/4230 | Source4161/3261 in full support context; frame/private allocation |
| Damage4347F0/1579, projectile434440/896 | Source1513/1414 and908/765 in recorded contexts |
| Interpreter45B860/7093 | Source6993/5880; required variadic double trace aligns candidate64 versus native ESP/120 |

Do not repeat completed Sound, scoped-reset-temporary or signed-publisher-counter
experiments as untried work. Do not use volatile, fabricated ABI, constructors,
padding or forced frames. Full table/padding ownership remains mandatory.

## Current findings and proof

- Retain AngleMath's established normal-COFF context as normal support for motion.
  /GL support merges the native repeated angle-difference/normalization calls.
  New motion/callback/HUD primary owners remain /GL; original TU identity is open.
- Native motion caller locals reserve float3. Polar433CD0 writes only x/y with
  FSINCOS; its source implementation remains open. Separate signed conversion
  locals and component-level Player() reads recover the actual conversion/load
  order. Aim uses remembered movement8AC/8B0 after current8A0/8A4 gates.
- Life HUD41A060 owns144, not sparse132:12 internal alignment bytes belong to it.
  Independent real HUD/count/fragments inputs prevent global-only specialization.
  Independent file/script/VM inputs likewise preserve BindAnimationScript's ABI.
- Penalty42F6F0/102 has a real position stack input loaded into EBX before422560;
  Ghidra's no-argument hypothesis misses it. Complete allocator/penalty source now
  exists in SecondaryDeathEffect.cpp; the88-byte effect has position0/nodeC/text1C,
  lifetime60/mode68/duration6C/delay70/scale7C/color84 and selected manager heads78.
- Death pointer-count/end-pointer/field-iterator probes give721/434,712/481 and
  721/443; swapping score/lives order changes none. These are completed probes,
  not untried next steps. Maintained death715/423 and exact HUD144 remain intact.
- Movement2896, reset2508, controller2888, drop851, special268 and interpreter7093
  retain all reviewed internal alignment and owned dispatch/selector data.

`.analysis/secondary-emitter-motion-final-proof.json` records six current exact
results; `secondary-emitter-motion-fields-proof.json` independently checks all35
native fields and13 emitted literal uses/92 bytes. Three cold families require
complete unique primary PDB ownership and current PE/MAP/PDB/source/tool receipts.
Only these six new units were replayed: older families do not depend on the new
files. No full cohort, broad behavior suite or Oracle/infrastructure suite ran.
The complete death call contract agrees on three direct targets/three sites;
this grants no exact credit to its remaining715/423 gap.

Other corrected layouts and findings are indexed in [KNOWLEDGE_BASE](KNOWLEDGE_BASE.md).
Older narratives are explicitly [historical](archive/KNOWLEDGE_BASE_2026-10-30.md).
Artifact retention/earlier cleanup is documented in [ARTIFACTS](ARTIFACTS.md).
Native product/runtime/port closure remains later work.
