# Complete gameplay flow and ECL debug callers

Two complete factory callers now have maintained natural C++ source: gameplay
420390/1149 and ECL debug40D600/1212, together2361 target bytes. Their independent
boundary/source reviews add no exact credit. Original classes, TU partition,
LTCG ownership, native product closure and runtime behavior remain separate.

| Owner | Target extent | Current complete candidate | Status |
| --- | --- | --- | --- |
| UpdateSecondaryGameplay | 420390/1149 | 1151;930 differing replayed bytes | Source, non-exact |
| UpdateSecondaryEclDebug | 40D600/1212 | 1153;744 differing replayed bytes | Source, non-exact |
| SecondaryGameplayUpdateCallback | 420840/7 | Normal callback has7-byte shape | Source; no leaf acceptance unit |
| LoadSecondaryDebugScript entry | 40D4F0/10 | Prototype only | Reviewed boundary; source still open |

## Full boundaries and physical call contracts

Fresh direct Ghidra exports attest the entire mapped text. Gameplay has305
instructions/1149 bytes,62 direct sites/33 targets and four RET4 paths. It has no
owned tables, internal alignment, escaping branches or foreign interior entry;
three CC bytes separate42080D..0F from the next owner.

Debug has331 linear instructions over1212 bytes:330 reachable instructions/1209
bytes plus an internal3-byte LEA at40D93D. It has41 call sites, one normal RET,
no tables/escaping branches or foreign interior references; four CC bytes follow.
Decompiler removal warnings for count-reset blocks are hypotheses, not authority
to crop the complete boundary. Independent PE flow/byte decoding agrees with the
full Ghidra instruction exports, including the sparse internal alignment.

Ghidra missed callback420840 entirely. Independent reviewed span7 is PUSH ECX,
call420390, RET, with DATA reference41FB99,13 preceding and9 following CC bytes.
This establishes the gameplay body's physical stack owner despite the decompiler's
unused-this hypothesis. The normal COFF callback plus /GL main body reproduces
that7-byte entry, frame54 and four RET4 paths. All-/GL callback/body compilation
instead yields1135/frame50 with a register owner and5-byte tail callback. The
split is a measured compiler context, not proof of original TU partition or
original /GL ownership. No established /GL owner was converted to normal COFF.

A second missed entry40D4F0/10 is referenced at40D907: scratch push, call40D500,
EAX0, scratch restore and RET. It has no explicit local argument reads, no foreign
interior references and CC gaps2/6. Do not invent a stack parameter/RET4 from the
thread-start name alone. The original nominal callback API remains unresolved;
only a zero-input external declaration is maintained, not an implementation.

## Complete gameplay source

SecondaryGameplay.cpp preserves both original initialization paths and both
factory calls, rather than replacing them with one simplified initialization.
The0-frame path handles immediate mode return, the existing stage transition or
fresh initialization; the30-frame/800-flag path resets and starts after transition.
Both retain ordered object resets, the265E50-byte item payload clear, counter
resets, zero80-byte spawn packet, main script, HUD setup and chain suspension.
The two required chain pairs are dereferenced without invented null guards;
the shared suspension helper has its own separately observed guarded contract.

The HUD200 flag/120-frame gate,5-frame audio state, guarded stage destruction,
4->80 terminal flag, demo input/mode changes and3540/3600 events remain. Stage6
has distinct initial, delayed and300-frame audio calls. Flags10/20/40 return3.
Statistics retain signed215999999 saturation and wrapped32-bit row arithmetic
`(selection0*3+selection1)*68D4+594`. Dialogue and90-frame gating precede gauge
updates, two global counters and the actual stage timer advance.

Additional physical views preserve gameplay timer10/flags60 and HUD signal4370,
flags4420, transition frame4428 and dialogue4438. Runtime mode558/flags590 and
worker7F0 reuse the existing MainRuntime root. Other existing global views are
reused where their target identities agree; new unknown chain/selection views
retain observed offsets without claiming original classes or capacities.

## Complete ECL debug source

SecondaryEclDebug.cpp retains states0/1/4 and the other states' normal return1.
Initialization performs two ANSI FindFirst/FindNext scans over the original ECL
mask, allocates/copies byte filenames and preserves the unconditional first
FindClose and unchecked second scan/allocation behavior. No failure guard, count
repair, bounds check or cleanup policy is added. The same WIN32_FIND_DATAA is
reused. Natural compilation reproduces frame19C, GS and eight-byte alignment.

The three D8 selection records begin3C/114/1EC, with current/previous/count at0/4/8,
wrap flagD0 and exclusion countD4. Reset preserves the observed zero/negative-count
rule. The source keeps top and nested input moves in order; file/menu confirmation
uses pressed bits, while subroutine confirmation uses released bits. File mode
retains seven ordered deletions, flag clear, thread start/context and state2.
Subroutine mode retains resets and the zero spawn packet with explicit zero
position, life10000 and selected program name before state4.

Running mode aliases backup fields118/11C/124 inside the existing InputState view;
it does not introduce a separate input global. It copies current/previous input,
updates debug input and pressed backup, then conditionally resumes nullable
chains and resets the player/projectiles/actors/items before state1. State-local
and packet lifetimes remain ordinary C++; no inert storage or forced frame is used.

## Complete comparisons and remaining work

The cold mixed context includes both real callers, the full host/VM/actor cycle,
and existing stage lifecycle/tween/thread implementations under /GL. Only the
real callback and the synthetic diagnostic entry use normal COFF. This is a
non-runnable compiler context, not the game or a behavioral harness.

Every candidate is measured over its complete PDB contribution. Gameplay replays
115 reviewed call/global/literal fields and debug78, with no cropped interval.
All62/41 call-site counts agree. Static acyclic call/return skeletons agree on
183 gameplay and26 debug traces; loop reentry is recorded and omitted. These
checks do not prove predicates, memory aliases, iteration counts or byte equality.
Size proximity is not exactness: complete replay still differs930/744 bytes.

Concrete gameplay gaps include the factory's EBX packet versus source stack
argument. Native EBX then holds the packet pointer, so later zero arguments use
immediates and common continuation clears EBX; source keeps EBX0. Shared pause,
runtime-mode and gauge helpers also still have incomplete source/ABI context.
The redundant stage-null test, shared return1 placement, statistics memory ADD
versus register increment/store and register scheduling remain different.

Debug's3/1-count reset branches remain in native code while VC9 folds them in the
current natural source. Private helper registers, field/store scheduling and GS
layout still differ. Do not add volatile, dummy calls, padding, fabricated
constructors or false ABI declarations to preserve those bytes.

Factory source now covers six native sites across three callers, including the
three host paths and these two complete owners;418460/189 remains source-open.
Optimized host sharing still reduces its physical site count. The factory remains
625/stack arguments/RET8 versus native621/EBX packet/stack script/RET4 in this
context. Host15904/frame328 and interpreter6989/frame120 remain unchanged support
observations with no acceptance in this family.

The current union freeze covers143 inputs. The prior132 frozen inputs and all
existing canonical contexts are unchanged;
no old exact unit needs cold replay. Only the new whole-owner compiler context
is built and reviewed. No Oracle implementation or broad runtime suite changed.
Private evidence uses secondary-gameplay-{boundary-proof,owners-functions,
owners-function-proof,owners-cfg-proof,owners-bindings,source-freeze}.json,
secondary-gameplay-full-disassembly.txt and the two explicit entry-span reviews.
