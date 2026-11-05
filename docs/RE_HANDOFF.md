# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; outer folder
is not Git. Read AGENTS.md, this file, relevant source and ledger rows. Target
bytes/current receipts outrank older notes. All dirty work is ours.

## Current exact state

- Original Japanese TH11v1.00a, pinned by `config/target.toml`.
- 2,136 candidates;540 reviewed boundaries;423 source mappings.
- 233 exact functions/26,378 bytes:9 normal COFF and224 linked PE.
- Origins:90 authored,434 indeterminate,14 exclusions,1,598 pending.
- VC9 RTM21022/SDK6.0A; direct Ghidra/scripts, no Factory MCP/REA.
- Commit/push authorized, prefix `gpt-6.1-sol: `.
- No reconstruction job remains running at this checkpoint.

Operator priority is now large, difficult functions, not leaf accumulation.
Main continuation is the complete7,093-byte script interpreter at45B860 and its
non-exact16,556-byte host dispatcher at412E30.

The host's corrected boundary is15,780 code bytes plus
540 primary table,195 selectors, one alignment byte and two nested tables/40
bytes. Full4,003-instruction decode and4,001 reachable Ghidra instructions agree;
all145 formerly external DATA refs are internal. Native has465 direct calls,
two callbacks and one RET. Slot494074 reaches the host through412E20/state+103C.
It contributes83 int/107 float argument reads, now represented in the full source.
Complete source mappings now exist; the16556-byte host still has no exact credit. See [HOST](SECONDARY_HOST_DISPATCH.md).

All154 cases/134 nondefault targets now have a complete ordinary member body
in src/SecondaryActorInstructions.cpp. Manager ExecuteInstruction calls the state
member, which obtains the captured instruction itself; no synthetic instruction
or workspace inputs remain in the actual owner. Both have source mappings.

A case-local128-byte decoder array is a capacity hypothesis. Cold VC9 /GL yields
the observed132-byte buffer-to-cookie distance; original declared type/bound is
still unknown. Ending the shared spawn packet's lexical scope before other
families reduces the initial member frame368 to native310. Packet8C, shared
spawn/name294 and cookie318 match. Dual GS cookies, EH registration320, one-state
FuncInfo and allocation-delete cleanup are now present naturally under /EHsc /GL.
Do not repeat the prior missing-throwing-helper theory as established: the real
member enclosure/local lifetimes recover EH without such a helper/profile change.

The complete11-byte adapter has strict cold primary-TU/one-field zero-difference
proof: secondary-actor-instruction-adapter. Its canonical family includes real
SecondaryScript.cpp and SecondaryHostArguments.cpp /GL support. Current complete
host15840/frame310 restores packets8C/shared spawn-name294/cookie318/EH320 in the
real VM context. All195 selector/call-index skeletons/projection roles/returns
agree; direct body calls464/456 still differ: float107/106, int83/82, supplied int
11/9, actor creator3/2, object creator4/3 and missing-animation clear6/4. Integer
argument index is now stack and float index ECX as native. Host integer adapter
state/index ECX/EAX remains swapped versus native EAX/ECX; cleanup EBP-310 versus
native EBP-2F4, FP/control and sharing remain. Run support7057/ESI/frame134 is not
primary-TU acceptance. Six real argument adapters have complete boundary/source
reviews, not leaf exact credit. Native pop paths reuse incoming parameters;
float argument retains its index-bit union. Keep natural public ABI and /GL.
Whole-host/specialized EH exactness and original decoder capacity remain open.
The28 affected old exact units/five families pass1,038 bytes cold; current private
proofs/freezes use secondary-host-arguments-* while prior header proof remains.

Earlier scoped member diagnostic15552/frame310 reproduces native packets8C,
scratch294/cookie318, but lacks the real VM support context.12D/12F790/663 and
spawn-join entry intervals are diagnostic attribution, never cropped acceptance
units. Synthetic-switch15776/mesh15772 probes and their absent EH are historical.
Do not retain spilled byte keys/length reload as current gaps or force the earlier
specialized213-byte constructor ABI onto native generic389/six sites/five callers.

Prior source work corrects arithmetic operand/result storage and angle87 argument
publication inside that full owner. Actual-caller cold diagnostic is7013/5821
(previous7033/5779): full masked differences worsen, while reviewed local storage,
X-push-before-Y-read and per-opcode tail topology improve. Fixed120, all89 selector
bytes,60-arm physical order and both RET4 paths remain. No exact credit is added.
See [INTERPRETER](SECONDARY_INTERPRETER_CODEGEN.md).

That SecondaryScript.cpp change's27 affected old exact
units pass1,027 bytes/22 fields cold; the current declaration change affects its shared state header only.
Main VM ESI versus native EDI, unary84's merged negate/push paths and82/87's shared
float destination remain open. Ordinary member/accessor hypotheses did not solve
these gaps; keep the public interface and original /GL owners.
The real nonvirtual state-member declaration changes only SecondaryActorState.hpp
among the prior37 frozen inputs. All61 old exact units in22 directly affected families
pass4,197 bytes cold; other36 prior inputs and tool/Oracle sources stay unchanged.
Retain that129-input freeze as history; current arguments freeze refreshes the changed reader and adds its new support source.

## Immediate continuation

| Area | Concrete remaining work |
| --- | --- |
| Host dispatcher412E30/16556 | Complete real member body; scoped15552/frame310, current real-VM/arguments family15840/frame310; adapter11 exact; calls464/456; host-int state/index swap, FP/control/sharing gaps; full-owner fields/bytes and EH; original array bound unknown |
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
  Extend the real host/argument-reader context before any isolated interpreter probe.
- Do not repeat rejected trace ablations/fixed-format/fp:fast, int advance,
  local/signed-pop or split trace-TU probes as untried. Current topic docs retain
  their measured results. Keep wrapped arithmetic and the established /GL owners.
- Preserve full extents/tables. No padding, forced alignment or private-register
  declarations to close the remaining frame/register/control-flow differences.

`.analysis/secondary-interpreter-tails-affected-proof.json` records the earlier27-unit
checkpoint; its interpreter audit/freeze keep that older prefix. New declaration
dependencies use secondary-host-owner-affected-proof.json and the129-input owner
freeze. Historical interpreter diagnostic family is
build/probe-ltcg/SecondaryInterpreterTails/maintained. Clearing235 and
position-marker proofs remain unaffected. No full
cohort, broad behavior or unchanged-Oracle suite ran. Exact claims do not establish
original classes/TU partition, native product closure or runtime/port status.

Other findings are indexed in [KNOWLEDGE_BASE](KNOWLEDGE_BASE.md). Older narratives
are [historical](archive/KNOWLEDGE_BASE_2026-10-30.md). Artifacts are retained per
[ARTIFACTS](ARTIFACTS.md). Native product/runtime/port closure remains later work.
