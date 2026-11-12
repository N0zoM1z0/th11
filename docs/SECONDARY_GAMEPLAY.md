# Complete gameplay flow and ECL debug callers

Two complete factory callers now have maintained natural C++ source: gameplay
420390/1149 and ECL debug40D600/1212, together2361 target bytes. Their independent
boundary/source reviews add no exact credit. Original classes, TU partition,
LTCG ownership, native product closure and runtime behavior remain separate.

| Owner | Target extent | Current complete candidate | Status |
| --- | --- | --- | --- |
| UpdateSecondaryGameplay | 420390/1149 | 1135;334 differing replayed bytes | Source, non-exact |
| UpdateSecondaryEclDebug | 40D600/1212 | 1153;785 differing replayed bytes | Source, non-exact |
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

## Real dependencies of the complete owners

SecondaryGameplaySupport.cpp supplies the guarded pause40CFA0/30 and runtime
mode40DEA0/30 bodies. Full direct Ghidra/PE reviews cover all10/6 instructions,
seven real callers each, all returns and neighbors, with no foreign interior
references or owned data. The pause helper ORs2 into nullable update/draw chain
flags; runtime flags590 bit2000 chooses forced mode2 or the requested mode558.
Ordinary conventional C++ declarations under /GL recover native EAX and EAX/ECX
inputs. Both complete support bodies have equal raw30-byte instruction sequences
in this context; neither receives independent exact acceptance or a leaf unit.

The same file implements complete gauge420A70/212:66 instructions, one RET, one
call459210, one real caller420390 and12 following CC bytes. It reads player Y at
880, compares ordered64/80/128 thresholds, raises value014 by5000 up to10000 or
by16/12/9 thereafter, and otherwise subtracts30 with a signed zero floor. When
timer.current20 is nonpositive it also decays value018, then decrements timer1C,
caps014 at13000 and018 at014. Wrapped arithmetic follows native32-bit operations.
These observed fields share the existing SecondaryHostSelectionState root4A56E0;
the duplicate gameplay-settings global/view is removed. The native gameplay stage
pointer is4A8D5C, so source uses g_StageSecondary. Initializer4024C0 publishes
g_StagePrimary at4A8D60 and destructor402740 clears60 then5C; those identities are
kept consistent across owners.

Its current full contribution is257 bytes with the actual timer/motion source. VC9 folds the unique constant settings
argument into global addresses; native still passes EDI. An ordinary member
hypothesis produces the same folding, so the maintained source keeps the simpler
free function. OffsetSecondaryTimer459210 is already implemented in SecondaryActorMotion.cpp;
reusing it restores native ESI timer input. FP operand widths/order remain
different. This is complete source presence, not exact codegen.

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

Every candidate is measured over its complete PDB contribution. The shared-state and broader dialogue contexts reproduced the same
gameplay/debug sizes and differences. Gameplay replays114
reviewed call/global/literal fields and debug78, with no cropped interval. All
62/41 call-site counts agree. Static acyclic call/return skeletons agree on183
and26 traces; loop reentry is recorded and omitted. These checks do not prove
predicates, memory aliases, iteration counts or byte equality. Full replay gives
1135/334 and1153/785, versus the prior1151/930 and1153/744. Debug's complete byte
comparison becomes worse in this context despite restored real helper inputs.

A natural reference to the factory's state subobject restores EBX spawn packet,
stack script and RET4 without a private ABI declaration. In the large gameplay
caller this restores immediate-zero staging and the common EBX-clearing
continuation. Real pause/mode bodies recover their native caller register setup.
After address/branch erasure, gameplay instruction differences now remain in
three regions: the missing second stage-null test, statistics register increment
and store instead of memory ADD, and the gauge's folded settings input instead
of native EDI setup/restoration. This erasure is diagnostic, never exact credit.
Direct-global deletion, an integer reference and a structured statistics row do
not recover the target sequence and are not adopted. Positive outer-guard
restructuring is also not retained.

Debug's3/1-count reset branches remain in native code while VC9 folds them in the
current natural source. Helper registers, field/store scheduling and GS layout
still differ. Do not add volatile, dummy calls, padding, fabricated constructors
or false ABI declarations to preserve those bytes.

Factory source covers six native sites across three callers;418460/189 remains
source-open. Its current full contribution is629/native621 with478 differing
replayed bytes, versus prior625/545. Native and source now agree on input/return
cleanup, but source retains a state base versus native whole-actor base. All
three host creator sites and all six emitter sites are now physically retained.
Canonical host15952/frame328 has457/native464 body direct sites; the separate
mixed family has host15936 and state update4079, versus canonical4083. Interpreter
6989/frame120 remains unchanged. None of these support owners receives primary-TU
acceptance in the family.

The shared control view and gameplay source/header changes, followed by the
real dialogue context, affected only the existing adapter exact unit. Its fresh
canonical11-byte/one-REL32 proof still had zero differences. See
[SECONDARY_DIALOGUE](SECONDARY_DIALOGUE.md) for the substantial caller.
## Strict exact runtime mode setter (2026-11-11)

`SetSecondaryGameplayRuntimeMode` at `0x40DEA0` now has an independent
pinned VC9 `/GL` cold linked-PE **30/30 strict exact** proof. The entire
six-instruction function chooses either the caller's requested mode at
runtime `+0x558` or forced mode 2 when flags `+0x590` contain `0x2000`.
Its natural source remains in `src/SecondaryGameplaySupport.cpp` and the
new tracked `probes/secondary-gameplay-mode-dynamic-context.cpp` preserves
native EAX/ECX arguments without fabricated register casts. PDB/MAP/PE
proof has **zero address fields and zero differences**. See the
`set-secondary-gameplay-runtime-mode` unit and
`.analysis/secondary-gameplay-mode-strict-20261111.log`.
This same dynamic producer restores the correct literal-mode call lowering
in the 934-byte ECL browser owner, which is still non-exact.

## Debug session loader and thread entry reconstructed (2026-11-12)

`SecondaryDebugScriptLoad.cpp` now covers the complete **111-byte**
`LoadSecondaryDebugSessionBody` at `0x40D500..0x40D56E`, adjacent to the
separate **10-byte** `LoadSecondaryDebugScript` callback at `0x40D4F0`.
The loader invokes the real secondary-player factory and four separately
reviewed setup owners, creates a secondary master from the selected ECL
filename, invokes its final stage setup helper, reads the new program's
subroutine count into the third selection record at debug session +0x1F4,
steps that selection by +1 and -1, and sets debug state +0x30 to 1.
The four opaque startup callees are kept as distinct function declarations;
no speculative implementation or fake class identity is added. The complete
111-byte original has 32 instructions and nine calls, with no foreign
interior entry. Pinned VC9 `/GL` with real selection and master producers
emits **109/111 bytes, 30 instructions and 9/9 calls**. Its 10-byte
callback is **8/10** in both normal COFF and real-loader `/GL`: the
original saves/restores ECX around one forwarded call, whereas VC9 omits
that scratch preservation. Both owners remain **source-present, non-exact**.
The dedicated bounded producer is
`probes/secondary-debug-script-load-context.cpp`; the scoped comparison
and log are `.analysis/probe-secondary-debug-script-load-20261112.py`
and `.analysis/probe-secondary-debug-script-load-20261112.log`.

The nearby `StoreSecondaryHostSlot040` at `0x410530..0x41053A` also now
has full natural C++ in `src/SecondaryMasterSlot040.cpp`. It writes an
animation-file pointer through the unguarded indexed master table at +0x40.
Native code uses private EAX payload / EDX index, while an ordinary
fastcall compilation emits **10/11** bytes with ECX payload and EDX
index. Several calling-convention/source variants were compared in
`.analysis/try-master-slot040-20261112.py`; none is strictly exact, so
the source remains uncredited in the exact ledger.
