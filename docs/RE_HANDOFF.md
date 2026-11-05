# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; outer folder
is not Git. Read AGENTS.md, this file, relevant source and ledger rows. Target
bytes/current receipts outrank older notes. All dirty work is ours.

## Current exact state

- Original Japanese TH11v1.00a, pinned by `config/target.toml`.
- 2,136 candidates;532 reviewed boundaries;415 source mappings.
- 232 exact functions/26,367 bytes:9 normal COFF and223 linked PE.
- Origins:90 authored,426 indeterminate,14 exclusions,1,606 pending.
- VC9 RTM21022/SDK6.0A; direct Ghidra/scripts, no Factory MCP/REA.
- Commit/push authorized, prefix `gpt-6.1-sol: `.
- No reconstruction job remains running at this checkpoint.

Operator priority is now large, difficult functions, not leaf accumulation.
Main continuation is the complete7,093-byte script interpreter at45B860.

Latest work restores the interpreter's physical60-case order and typed trace
format branches. All60 opcode bodies stay unchanged. Through its sole actual
updater caller, complete7033/5779 now has fixed120, all89 selector bytes and both
RET4 paths matching the target. Trace/reference call multiplicities agree. Main
VM ESI versus native EDI and extra push/destination tail sharing remain open.
See [INTERPRETER](SECONDARY_INTERPRETER_CODEGEN.md).

Only SecondaryScript.cpp plus a new actual-caller probe changed. Its27 affected
old exact units pass1,027 bytes/22 fields cold; no unrelated replay. The large
owner receives no exact credit. Independent-helper context size7093 still aligns
64 and is not an acceptance result. Source/probe hashes remain frozen.

## Immediate continuation

| Area | Concrete remaining work |
| --- | --- |
| Interpreter45B860/7093 | Main priority: complete7033/5779; fixed120/all89 selectors/60-case order recovered; VM ESI versus EDI, extra PushValue and FloatDestination tail sharing |
| Collision432A90/546 | Complete source543/257; frame18 versus1C, EBX save/restore placement, sound argument EDX versus native ESI |
| Movement430290/2896 | Complete source2928/2447 after animation value correction; native call/return/frame contracts agree; register/branch/vector stores remain open |
| Horizontal wrap430E50/540 | Complete source499/415; preserve inclusive6800 boundaries, counter8/45 and real sound47/48/reset calls |
| Death4327D0/703 | Source715/423 in real controller/HUD/Bind context; callee-save placement, global stores and emitter-loop layout |
| Effect422560/229, penalty42F6F0/102 | Complete source228/37 and102/14; allocator position EDI versus native EBX and mode/lifetime stores |
| Controller431070/2888 | Source2800/2285 in previous context; actual normal-COFF callback support preserves stack owner/two RET4; frame/vector stores |
| Reset432CC0/2508 | Source2428/1965 after early position-marker correction; main factory grouping remains open |
| Publishers455760/118,4557E0/123 |111/33 and116/47; ID zero-check/register allocation |
| Spawn433F90/853 | Real Sound846/733 and real-only/independent RNG856/690 both fail |
| Actor state411750/4230 | Prior4161/3261 full-support measurement; not rebuilt for this API change; frame/private allocation |
| Damage4347F0/1579, projectile434440/896 | Source1513/1414 and908/765 in recorded contexts |

Do not repeat completed Sound, scoped-reset-temporary or signed-publisher-counter
experiments as untried work. Do not use volatile, fabricated ABI, constructors,
padding or forced frames. Full table/padding ownership remains mandatory.

## Findings to retain

- Native Run has one direct caller45D443 in UpdateSecondaryScripts. Direct
  synthetic Run input changes LTCG alignment; do not treat64 as a double ABI rule.
- Keep real trace double args and all side effects. Float/int formats resolve
  their values separately; native seven trace calls and reference pairs remain.
- Extra candidate tails:50 joins integer-comparison push;52/58 share another;
  82/87 share float destination. Native itself shares comparison tails.
- Do not repeat rejected trace ablations/fixed-format/fp:fast, int advance,
  local/signed-pop or split trace-TU probes as untried. Current topic docs retain
  their measured results. Keep wrapped arithmetic and the established /GL owners.
- Preserve full extents/tables. No padding, forced alignment or private-register
  declarations to close the remaining frame/register/control-flow differences.

`.analysis/secondary-interpreter-hard-affected-proof.json` records27 current cold
results. Maintained audit/diagnostic, opcode-body hashes and source freeze share
that prefix. Clearing235 and position-marker proofs remain unaffected. No full
cohort, broad behavior or unchanged-Oracle suite ran. Exact claims do not establish
original classes/TU partition, native product closure or runtime/port status.

Other findings are indexed in [KNOWLEDGE_BASE](KNOWLEDGE_BASE.md). Older narratives
are [historical](archive/KNOWLEDGE_BASE_2026-10-30.md). Artifacts are retained per
[ARTIFACTS](ARTIFACTS.md). Native product/runtime/port closure remains later work.
