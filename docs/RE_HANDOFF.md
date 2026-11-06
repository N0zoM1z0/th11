# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; outer folder
is not Git. Read AGENTS.md, workflow/Oracles and relevant source/rows. Current
receipts outrank historical notes. All dirty work is ours.

## Current exact state

- Original Japanese TH11v1.00a, pinned by config/target.toml; no substituted bytes.
- 2,139 candidates;567 reviewed boundaries;450 source mappings.
- 233 exact functions/26,378 bytes:9 normal COFF and224 linked PE.
- Origins:90 authored,461 indeterminate,14 exclusions,1,574 pending.
- VC9 RTM21022/SDK6.0A; direct scripts/ghidra.py, no Factory MCP/REA.
- Commit/push authorized, prefix `gpt-6.1-sol: `; no heavy job remains running.

Operator priority is large/difficult owners, not leaf accumulation. Main owners:
host412E30/16,556, animation44B4B0/12,056, interpreter45B860/7,093,
dialogue41D380/6,980, HUD41B380/4,716 and gameplay420390/1,149. Native product,
broad runtime and portability are later phases. No new exact unit this checkpoint.

## New complete animation interpreter12056

AnimationExecution.cpp/header/Instructions/Update fragments cover all signed
opcodes-1..102, events/saved returns, nine tween updates, both strip generators,
callbacks and clocks. Full owner44B4B0..44E3C7=11637 code/3202 instructions+3
alignment+416 in104 cells. All104 foreign DATA refs are owned;8 CC before44E3D0.
The following44E3D0/44E430/44E460 are separate source-open code, not more table.
Native stack VM, frameCC/alignment8, three RET4 return0/0/1. Reject decompiler
phantom register/float inputs and eight-byte return. Callback428 nonzero returns1
without restoring the overridden global time scale; retain that path.

Cold build/probe-ltcg/AnimationExecutionOwners: complete public primary12292,
frameE0 without8 alignment, VM EBP versus nativeEDI,407 reviewed fields and11744
full replay differences. Native/source262/260 calls: RNG4/3 and float destination
15/14. All104 entries retain416 static call/loop/post-update/return skeletons;
these do not prove predicates, operands, aliases or loop reentry. Native returns
0/0/1 retained. Direct-if ReadF removes literal spill/reloads; reviewed native
case order with four distinct child cases recovers signed dispatch and lookup,
insertion,malloc counts. Split children alone adds112 bytes and is rejected.
Instruction41 was incorrectly text-only: flag40000000 selects scene4C2EF8 at
44D523 versus text4C2F00 at44D543. Corrected full arms restore78/78 float reads;
four targeted argument paths pass, and the old candidate fails scene selection.
Direct -- is byte-identical and leaves EDI=-1/ADD versus DEC; do not repeat.
Floating greater-than is ordered by raw AH masks, not a negated <=.

Ring9 must divide by float(count-1):44DCF9/44DD03 explicitly round before both
divisions. Corrected10-op ring and existing12-op arc x87 setups agree after only
stack/VM naming erasure. EmitStripPair advances each vertex, recovers two1C cursor steps per pair
and keeps color in a register; precision fix+pointer change saves28 bytes net.
Nested sums alone do not shrink the frame. /Oy- probe11688/frameF0/VMEBX/no8
alignment and264 calls is rejected. Native pair tween has EBX hidden result at
ESP88; current source returns its pair in EAX/EDX. Class/TU origin remains open.

Real direct dependencies have full natural source, without leaf acceptance:
read float540 (447+1+92table), int219 (145+3+48table+23selectors), float destination
124, int destination104, UV52; vector timer52, node insertion23, scale138,
primary/alternate RGB216/216 and alpha132/118. Alternate alpha and scale setup
leave controls untouched; scale receiver is whole VM. Source-open bodies include
polar44EF80, front-draw456020 and reviewed random/unary wrappers. Original nominal
classes/header/template/TU origins remain indeterminate. VM374/377 is packed
primary color/alpha; the older spriteIndex label is wrong, but that shared
header was not changed solely to rename it. See ANIMATION_EXECUTION.md.

Next difficult gaps: frame/alignment, temporary lifetimes, VM register, pair
tween hidden result and remaining call merges: integer random40 and float assignment7 joining the
17/27/45/46 destination tail. Do not invent
padding/double locals/register ABI to force them. Preserve /GL and full tables.
HUD now calls canonical int ExecuteAnimationVm. The older void prototype/caller
name in player-controller files is still external there; changing that source
would affect six exact units, and its header eleven. It was not changed here.

## Dialogue, HUD and shared gameplay

The full new real-dependency family retains dialogue6948 but now frame140/native
and5357 full differences, improving earlier frame13C/5694. All454 fields,252 calls
and46 static per-op call/advance/return skeletons remain. No original-TU or exact
claim. Native owner6980=6780 code+14 alignment+2 pre-table+184 in27/7/7/5 cells;
1834 instructions,252 calls/28 targets, EAX0/-1/two RET4. All27 opcodes have source.
Color/hidden-result/store/register/layout gaps remain. Local color cache is byte-
identical; positive do-while6940/6483 rejected. Physical native order/current>=
stamp, shared annotation/continue-menu and actual sprite44AB40 are maintained.
Decoder41 has2 differences; child70 and sprite34/36 support remain zero, no leaf
acceptance. Scratch4C9A30 capacity remains unknown; retain unchecked accesses.

HUD4716=4647 code/1302 instructions+39 alignment+2 pre-table+28 stage cells. Full
source maintains9/2/4 VM loops, boss/gauges/title/ten badges, locks/publication,
dialogue/eight releases, indicator/alpha and timers. Callback41CFB0/7 has DATA
41A442, CC6/9; ordinary COFF supports real /GL stack owner, original TU open.
Current support4652/frameC/4101 and104 fields/all27 calls remain. Position/alpha49
forms match only after branch-address/distance-slot erasure (nativeESP10/source14).
Alpha FF guard is >=64, unordered reaches conversion; signed-char cast naturally
recovers CW/FISTP. Direct state reads/per-branch extraction4812/4264 rejected.
Dialogue elapsed4 is separate from18/2C; title/badges use portrait444C. Screen44
allocation has timer30, real null guard and initializer kind43. Initializer448A90
is source-open; automatic1129 excludes unreviewed table ownership.

Gameplay uses secondary4A8D5C, primary4A8D60; gauge4A56E0 is shared host-selection
root14/18/timer1C. New family keeps gameplay1135/334/114fields, debug1153/785/78fields.
Remaining: redundant stage-null guard, statistics memory ADD, gauge EDI setup,
debug count resets/store/GS. /Og-1629/2325 rejected. Real callback420840/7 preserves
stack owner; original TU split unknown.

## Host, script interpreter and creator

Canonical secondary-actor-instruction-adapter keeps /GL; only its11-byte adapter
is accepted exact. Host16556 has154 cases/195 selector checks/145 owned DATA refs.
Canonical15952/frame328 versus native310, sites464/457, all3 actors+6 emitters.
Remaining float107/106,int83/82,supplied-int11/9,object4/3,missing-animation6/4.
12D/12F69 x87 forms agree after memory erasure, but pointer selections4/1 and
packet/spawn-name/cookie/EH slots differ. Decoder128 is a capacity hypothesis.
Do not repeat physical reorder/ternary/shared-float330 or trace/pop/member/
accessor/fp:fast probes. Creator629/native621 has EBX packet/stack script/RET4;
478 full differences, state versus whole-actor base; ctor706/native699.
Interpreter6989/native7093 retains120/all89 selectors/two RET4, VM ESI/nativeEDI;
32/31 pushes, unary84 and82/87 shared destinations remain. Canonical actor update
4083/native4230; broader family4091 remains a separate context.

## Checkpoint evidence

Current receipt check: .analysis/verify-animation-execution-strip-checkpoint.py.
Private evidence is animation-execution-* and animation-vm-* direct
Ghidra exports, complete field/CFG/return/target proofs and receipt/source freeze.
This checkpoint changes only AnimationExecution.cpp and Update.inc against the
179-input freeze; no accepted exact unit depends on them. Cold replay covers
this changed owner only. Earlier HUD/dialogue/parent receipts remain historical
support contexts. No unrelated caller replay or unchanged Oracle tests required.
Tools/Oracle implementations unchanged. No new exact unit or source mapping.
