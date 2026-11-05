# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; outer folder
is not Git. Read AGENTS.md, this file, relevant source and ledger rows. Target
bytes/current receipts outrank older notes. All dirty work is ours.

## Current exact state

- Original Japanese TH11v1.00a, pinned by `config/target.toml`.
- 2,136 candidates;534 reviewed boundaries;415 source mappings.
- 232 exact functions/26,367 bytes:9 normal COFF and223 linked PE.
- Origins:90 authored,428 indeterminate,14 exclusions,1,604 pending.
- VC9 RTM21022/SDK6.0A; direct Ghidra/scripts, no Factory MCP/REA.
- Commit/push authorized, prefix `gpt-6.1-sol: `.
- No reconstruction job remains running at this checkpoint.

Operator priority is now large, difficult functions, not leaf accumulation.
Main continuation is the complete7,093-byte script interpreter at45B860 and its
missing16,556-byte host dispatcher at412E30.

The host's corrected boundary is15,780 code bytes plus
540 primary table,195 selectors, one alignment byte and two nested tables/40
bytes. Full4,003-instruction decode and4,001 reachable Ghidra instructions agree;
all145 formerly external DATA refs are internal. Native has465 direct calls,
two callbacks and one RET. Slot494074 reaches the host through412E20/state+103C.
It accounts for83 int/107 float argument reads absent from the current diagnostic.
No complete-host source mapping/exact credit is claimed. See [HOST](SECONDARY_HOST_DISPATCH.md).

All154 handled opcodes now have natural switch source: animation100..115,
motion118..133, control140..174 and records190..1C2. The previously suggested
134..13F/175..18F ranges are defaults. These cover all134 nondefault primary
targets/15,613 entry-interval bytes; prologue129/epilogue38 complete the code span.
No native wrapper/source mapping or exact credit is claimed.

Latest records fragment adds51 values/47 targets/5,830 entry bytes. It asserts
214-byte records,18 pattern entries,1E4 basic/204 advanced packets and object
fields. Preserve record-position ordered >0.899999976 gate,1B7's unordered-inclusive
not-less-than-990 marker, three-bin512/-512 versus five-bin600/200/-200/-600,
shared pair-reader tails, advanced counter fields40/44 versus44/48, late1B2 opcode
reread, argument/destination order and integer/float interpolation roundings.
Cold /GL records probe5568 has all186 direct/2 indirect physical calls, equal
callee counts/all51 acyclic call/index skeletons and29 targeted static checks.

Combined compile-only switch probe15776 covers all195 selector groups and all
call/index skeletons, projection roles and0/-1 results. Native464/candidate460
body calls differ at raw-int-value reader11/10, actor creator3/2 and ID lookup6/4;
2/2 indirect calls. Full native includes the additional GS call. It has local
allocation310/EH; candidate2D4/no EH. Diagnostic inputs still include instruction
and decoder workspace: original name-array capacity and native wrapper remain
open. Do not invent buffer padding or a throwing helper to force the frame.

Native FuncInfo49FE58 has one unwind entry49FE50 ->48AA90, which deletes the
saved mesh allocation; handler48AA9E performs cookie checks and passes FuncInfo
to484DAB. Natural1B8 new StageScriptMesh(17,17,0) retains the source obligation.
A second cold switch context with real StageMesh.cpp /GL gives15772 and the same
skeletons but still no EH. With only this caller, LTCG specializes the constructor
to213 bytes/no stack args; native has six call sites/five callers and generic389.
This is a missing-context finding, not a native ABI declaration. Next: original
host wrapper/name storage and real constructor/throwing-call context; complete
whole-owner field/byte diagnostics, without another leaf/probe-count campaign.

Prior source work corrects arithmetic operand/result storage and angle87 argument
publication inside that full owner. Actual-caller cold diagnostic is7013/5821
(previous7033/5779): full masked differences worsen, while reviewed local storage,
X-push-before-Y-read and per-opcode tail topology improve. Fixed120, all89 selector
bytes,60-arm physical order and both RET4 paths remain. No exact credit is added.
See [INTERPRETER](SECONDARY_INTERPRETER_CODEGEN.md).

That SecondaryScript.cpp change's27 affected old exact
units pass1,027 bytes/22 fields cold; all37 source/header/support hashes stay frozen.
Main VM ESI versus native EDI, unary84's merged negate/push paths and82/87's shared
float destination remain open. Ordinary member/accessor hypotheses did not solve
these gaps; keep the public interface and original /GL owners.
The records/header/probe additions affect none of86 canonical receipt contracts.
All37 earlier inputs and motion/animation/control hashes stay unchanged; no
prior cold replay is repeated.

## Immediate continuation

| Area | Concrete remaining work |
| --- | --- |
| Host dispatcher412E30/16556 | All154 handled values/134 nondefault targets implemented and jointly compiled; original wrapper/decoder stack capacity, missing EH and constructor context; native464/candidate460 body calls; full owner field/byte work, no native stub |
| Interpreter45B860/7093 | Main priority: complete7013/5821; fixed120/all89 selectors/60-case order retained; VM ESI versus EDI, unary84 merged push and82/87 shared destination |
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
- Arithmetic50 and52/58 extra sharing is removed; native comparison sharing
  remains. Logical73/74 each use one push/result slot18. Unary84 still has one
  push versus native two;82/87 still share float destination. Preserve the angle87
  X-push-before-Y-read order and math-before-destination lookup.
- Typed int-pointer pop leaves7013/5821 unchanged; memcpy bit transport expands
  frame1B0 and gives7381/6120 without fixing either missing call. Both rejected.
  Reconstruct the real large host context before another isolated probe lattice.
- Do not repeat rejected trace ablations/fixed-format/fp:fast, int advance,
  local/signed-pop or split trace-TU probes as untried. Current topic docs retain
  their measured results. Keep wrapped arithmetic and the established /GL owners.
- Preserve full extents/tables. No padding, forced alignment or private-register
  declarations to close the remaining frame/register/control-flow differences.

`.analysis/secondary-interpreter-tails-affected-proof.json` records27 current cold
results. Maintained whole-owner audit, per-opcode CFG audit and source freeze use
that prefix; diagnostic family is
build/probe-ltcg/SecondaryInterpreterTails/maintained. Clearing235 and
position-marker proofs remain unaffected. No full
cohort, broad behavior or unchanged-Oracle suite ran. Exact claims do not establish
original classes/TU partition, native product closure or runtime/port status.

Other findings are indexed in [KNOWLEDGE_BASE](KNOWLEDGE_BASE.md). Older narratives
are [historical](archive/KNOWLEDGE_BASE_2026-10-30.md). Artifacts are retained per
[ARTIFACTS](ARTIFACTS.md). Native product/runtime/port closure remains later work.
