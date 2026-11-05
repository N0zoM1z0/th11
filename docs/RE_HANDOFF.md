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

Motion fragment118..133 covers28 handled values/16 physical
targets/4,517 native entry-interval bytes. The new .inc/header and compile-only
probe preserve12A/12B's real fallthrough, ordered sentinel/NaN gates, mirrored and
shortest-path angles, vector/pair/ellipse tweens, centers and timer resets.
Cold VC9 /GL probe4368 has the same opcode partition,89 physical call sites and
all per-opcode call/index trace sets. This is partial-owner structural evidence,
not a complete native function mapping or byte match.

Creation/animation fragment100..115 adds22 values/22 primary targets.
It retains guarded spawns, signed payload indexing, snapshots/repeated readers,
the shared80-byte packet, real camera0 projection addresses, ID value transport,
replacement/cache/event flows and111/115's angle join. Cold /GL probe2804 agrees
on all per-opcode call/index trace sets and projection argument roles. Native89
physical calls versus88 remain: raw integer reader11/10, lookup6/5, flag2-clear2/3.
New control/effect140..174 adds53 values/49 targets/2,663 entry-interval bytes:
effect/counter/HUD state, slot changes, spell decoder/transitions, blocking returns,
selection/difficulty and VM-time operations. Cold /GL probe3248 has all100 native
physical calls/callee counts, all53 acyclic call/index skeletons and24 targeted
static detail checks. Preserve shared low/middle reader tails; ten-ID loop bounds;
signed decoder length, byte77/7/+10 and late opcode reread; discarded argument2;
15A/15B/15F/162/170 rounding/division stages. Decoder workspace capacity remains
unproved; the compile-only context takes a workspace pointer, not a native ABI.
Important correction:153/154 returns -1 through416BB0 to pause the interpreter;
other paths clear EAX at416BAE. Ghidra's void inference lost this result.
Combined source covers103/154 handled values/87 of134 nondefault targets;
remaining51 values and EH source are open. No fragment is a complete native
mapping or exact comparison unit.

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
The control fragment/header/probe additions affect none of86 canonical receipt
contracts. All37 earlier inputs and motion/animation hashes stay unchanged; no
prior cold replay is repeated.

## Immediate continuation

| Area | Concrete remaining work |
| --- | --- |
| Host dispatcher412E30/16556 |103/154 values implemented/compiled/audited across animation100..115, motion118..133 and control140..174; next large record/effect134..13F and175..1C2 groups (51 values); original decoder storage and EH lifetimes; no partial/default native stub |
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
