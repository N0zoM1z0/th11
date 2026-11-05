# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; outer folder
is not Git. Read AGENTS.md, this file, workflow/Oracles and relevant source/rows.
Target/current receipts outrank historical notes. All dirty work is ours.

## Current exact state

- Original Japanese TH11v1.00a, pinned by config/target.toml; no substituted bytes.
- 2,136 candidates;540 reviewed boundaries;424 source mappings.
- 233 exact functions/26,378 bytes:9 normal COFF and224 linked PE.
- Origins:90 authored,434 indeterminate,14 exclusions,1,598 pending.
- VC9 RTM21022/SDK6.0A; direct scripts/ghidra.py, no Factory MCP/REA.
- Commit/push authorized, prefix `gpt-6.1-sol: `.
- No reconstruction job remains running at this checkpoint.

Operator priority is large, difficult functions, not leaf accumulation. Main work
is the complete16,556-byte host412E30 and7,093-byte interpreter45B860.

## Complete host and real creation/update cycle

The host owns15780 code bytes plus540 primary table,195 selectors,1 alignment
and40 nested-table bytes. Full4003 decoded/4001 reachable instructions, all145
owned DATA refs and all154 handled values/134 nondefault targets are reviewed.
Slot494074 enters through412E20/state+103C. Ordinary manager/state members enclose
the complete switch in SecondaryActorInstructions.cpp and four case fragments.
Only the complete11-byte manager adapter has strict exact credit; canonical unit
secondary-actor-instruction-adapter keeps /GL and all owned extents.

The family now includes real SecondaryScript/HostArguments and actor creation,
lifetime, animation and state-update support. New SecondaryActorCreation.cpp
implements the complete621-byte factory4108F0, including its20-byte table and53
selectors. Ordinary new2678/constructor, payload/timer/difficulty/flags, state
update and both intrusive-list paths remain; no invented allocation-failure guard.
Current factory625/RET8 stack arguments differs from native621/RET4 with EBX spawn
arguments and stack script; constructor706/native699. Earlier equal621 extent
still differs503 rebased bytes; current625 differs545. No creator exact credit.

12D/12F now selects motion/three tween pointers before reads, then stages each
target Y/X before its initial pair. All69 x87 operations/widths/register forms
agree after memory-address erasure, versus prior57. This does not prove stack
aliases/slots, predicates, original vector classes or full bytes. Current whole
host15904/frame328 has packetA4/shared spawn-name2AC/cookie330/EH338; native has
8C/294/318/320 and310 allocation. Cleanup EBP-328 versus native EBP-2F4 remains.
Do not drop these floating stages to preserve the older compatible310 frame.
The case-local128-byte decoder remains a capacity hypothesis;132-byte cookie
distance persists. Never invent padding, volatile, constructors or private ABI.

All195 selector groups/call-index skeletons/projection roles/0-and-minus1 returns
agree. Direct body sites464/455 still differ: float107/106, int83/82, supplied
int11/9, actor3/2, object4/3, missing-animation6/4 and emitter6/5. Indirect2/2 and
GS1/1 remain. Four native pointer-selection comparisons collapse to one; preset
reloads, register allocation, stack storage, FP/control and EH remain work.
The12D/12F757/native790 entry intervals are diagnostic only, never acceptance units.

Real state-update support restores Run's fixed120 frame: complete6989/native7093,
VM ESI versus EDI. All89 selectors, physical case groups and both RET4 agree.
Native32/source31 push sites, unary84 merged paths and82/87 shared destination
remain. State-update support4083/native4230 also remains non-exact. Neither gets
primary-TU acceptance from this family. Historical actual-caller7013/5821 is a
separate diagnostic context, not the current contribution.

Only one old exact unit is affected; fresh cold11-byte/one-field replay passes.
Headers/tools/Oracles and other exact units are unchanged. Current private proofs
use secondary-host-pairs-* and secondary-interpreter-tails-host-pairs-cfg.json.
The132-input union freeze changes the motion fragment and adds creation/update;
older28-unit reader and61-unit header proofs/freezes remain historical.

## Next substantial work

- Host12D/12F: retain the recovered69-operation FP stages; recover native pointer
  selection/storage/preset reloads without fake frames or ABI declarations.
- Other real factory callers:40D600/1212 and420390/1149 are unreviewed substantial
  owners; factory has seven native sites in four callers, three inside the host.
  Complete their target reviews before claiming original factory context.
- Full host: recover missing physical call sites and all byte/field/EH ownership.
- Interpreter: VM EDI, unary84's two push paths and82/87's separate destinations;
  preserve wrapped arithmetic, trace side effects and angle87 X-publication order.

Do not repeat full native-order case reordering as an untried fix: extra sharing
persists. Separate if/ternary pointer choices compile identically; two manually
shared float workers worsen frame330 and were rejected. Preserve actual owners
and /GL. Do not revive prior trace ablations, typed-pop/member/accessor probes,
fp:fast, synthetic mesh213 specialization or missing-throwing-helper theories as
established solutions. Original mesh context has generic389/six sites/five callers.

Other maintained large non-exact owners include movement2896, controller2888,
reset2508, damage1579, projectile896 and death703; topic docs retain their full
measurements. Follow [KNOWLEDGE_BASE](KNOWLEDGE_BASE.md) and
[HOST](SECONDARY_HOST_DISPATCH.md)/[INTERPRETER](SECONDARY_INTERPRETER_CODEGEN.md).
Cold replay only changed exact units and actual dependencies; broad behavior,
full-cohort replays and unchanged-Oracle tests are not routine requirements.
Native product/runtime/port closure and original TU/class identity remain separate.
