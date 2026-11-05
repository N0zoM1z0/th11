# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; outer folder
is not Git. Read AGENTS.md, this file, relevant source and ledger rows. Target
bytes/current receipts outrank older notes. All dirty work is ours.

## Current exact state

- Original Japanese TH11v1.00a, pinned by `config/target.toml`.
- 2,136 candidates;527 reviewed boundaries;405 source mappings.
- 224 exact functions/25,664 bytes:9 normal COFF and215 linked PE.
- Origins:90 authored,421 indeterminate,14 exclusions,1,611 pending.
- VC9 RTM21022/SDK6.0A; direct Ghidra/scripts, no Factory MCP/REA.
- Commit/push authorized, prefix `gpt-6.1-sol: `.
- No reconstruction job remains running at this checkpoint.

Latest continuation adds complete movement2896 and variable event setter67 source.
The setter closes67 linked bytes/two fields; movement remains2836/2434. Source and
native movement agree on12 direct targets/22 sites, one indirect callback and one
RET4, with18 hex stack bytes. Its full two-table owner is retained.

Movement review corrects a support-source error: reset publishes orbit/aim at DC,
called by movement; draw uses a separate E0 slot. The shared view and both reset
publications now agree with independent target address arithmetic. Correcting
this and movement's int return declaration cold-replays23 actually affected old
units. Those and the new setter pass24 units/1,992 bytes/84 fields/relocations.
One new67-byte boundary/origin review remains indeterminate. See
[PLAYER_MOVEMENT](SECONDARY_PLAYER_MOVEMENT.md).

Previous motion/HUD six proofs retain1,019 bytes/35 fields and all13 literal uses/
92 bytes after the correction. Death703 source715/423 and effect allocator229/
penalty102 sources228/37 and102/14 remain open.

## Immediate continuation

| Area | Concrete remaining work |
| --- | --- |
| Movement430290/2896 | Complete source2836/2434; native call/return/frame contracts agree; register/branch/vector stores remain open |
| Horizontal wrap430E50/540, item attraction4245E0/63 | Fresh decompilation exists; real movement callees remain source-open |
| Death4327D0/703 | Source715/423 in real controller/HUD/Bind context; callee-save placement, global stores and emitter-loop layout |
| Effect422560/229, penalty42F6F0/102 | Complete source228/37 and102/14; allocator position EDI versus native EBX and mode/lifetime stores |
| Controller431070/2888 | Source2800/2285 in previous context; actual normal-COFF callback support preserves stack owner/two RET4; frame/vector stores |
| Reset432CC0/2508 | Source2412/1970 in corrected movement context; main factory grouping remains open |
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
- Reset orbit/aim publications are DC, not E0; movement calls DC and draw calls
  E0. Keep the two callbacks distinct.
- Event456230/67 uses the existing ID value wrapper to preserve stack ID and SI
  event. Scalar-ID source emits53/57; independent renderer input is required.
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

`.analysis/secondary-movement-final-proof.json` records24 current strict cold
results; `secondary-movement-call-audit.json` records the complete movement call
contract. The earlier six motion/HUD result and literal-proof files are refreshed
against current receipts. Source freeze is recorded separately. No full cohort,
broad behavior suite or Oracle/infrastructure suite ran. All native origin/class/
TU identities remain separate from these exact-codegen claims.

Other corrected layouts and findings are indexed in [KNOWLEDGE_BASE](KNOWLEDGE_BASE.md).
Older narratives are explicitly [historical](archive/KNOWLEDGE_BASE_2026-10-30.md).
Artifact retention/earlier cleanup is documented in [ARTIFACTS](ARTIFACTS.md).
Native product/runtime/port closure remains later work.
