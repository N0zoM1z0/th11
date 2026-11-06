# Complete character-selection menu

`src/MenuCharacterSelection.cpp` reconstructs page06, the character selector
before the partner page. This maintains the complete state machine, four child
animation event trees and selection history. Original class/TU identity, exact
code generation and native-product/runtime closure remain separate claims.

## Reviewed ownership

Mapped-text-attested direct Ghidra code/reference/span/table review and an
independent complete original-PE CFG establish43CBE0..43D0AB/1228 bytes:

| Component | Count |
| --- | ---: |
| Reachable instructions |338|
| Reachable code bytes |1180|
| Skipped loop alignment |27|
| Final NOP before table |1|
| Five-cell owned table43D098..43D0AB |20|
| Direct physical calls/distinct targets |32 /13|
| Address fields |60|
| RET4/EAX1 paths |8|

The default1207-byte bounding extent stopped at43D096; Ghidra's sparse body has
only1180 code addresses. The six-byte hole before43CE40, ten-byte hole before
43CEF0 and eleven-byte hole before43CF50 are reviewed alignment, not missing
handlers. NOP43D097 and all table cells belong to the complete owner. The five
apparent foreign interior references are owned table DATA. Preceding page05's
five-cell table43CBCC..43CBDF meets this entry directly; no preceding CC gap
exists. Four CC follow this page's table before partner page43D0B0.

Native input is one stack menu pointer, despite the decompiler's extra unused
this parameter. All eight returns are RET4/EAX1 and frame size is zero. The only reviewed
caller is controller4399B0 at439D86. The EAX value live across selection-history
calls does not prove a nominal C++ return type and does not justify adding a
fabricated return.

## Opening and active input

Opening sets count2. At difficulty4, a character is excluded only when all three
of its partner unlock bits are clear: statistics2DDF0..2 and2DDF3..5, bit10.
A locked current character is moved to the other through the observed count
clamp, then appended to the existing exclusion list at selection90 using countD4.
No exclusion reset, all-locked fallback or capacity guard is added.

The page creates heading script61, stops/recreates character-root7B, executes
root event3, sends short event11+current and enters state1. Time greater than6
activates state2. Left/right uses pressed/repeat masks40/80. A change queues12,
executes root event3 and sends event7+current. Cancel102 precedes confirm80001,
starts state4 and queues11.

Confirmation preserves four separate child lookups and VM lookups, in order:
`77+current` event6, `79+current` event6, `78-current` event1 and
`7A-current` event1. The parent value ID is the persistent root at4B0. Each
found VM receives the short event; when marker18 is zero, its related-node list
receives the same event. The selection is re-read for each script expression.
Only afterward does the page queue10, enter state3 and return1. These event
operations are not interchangeable with clearing visibility flag2.

## Transition and saved values

At state3 time14 or later, stop61 and select page7. Capture new and previous
character, publish the new character and its saved value4C325C, push selection
history, then set the next selector count3. If the character changed, select0
through the native clamp and set saved partner4C3260 to0; **do not write** the
current partner global4A5714 in that branch. If unchanged, call the existing
fastcall SelectMenuItem on the saved partner, then write the captured raw saved
value to the current partner global. That store does not use the clamped return.

At state4 time6 or later, stop7B/61, select page5, store the current character
global, pop history, then copy that global to the saved character. No new saved
partner normalization or pointer/null matrix is introduced. The history push
remains its existing void C++ operation; natural /GL already preserves its EAX
selection pointer across the call in the compiled candidate.

## Cold compilation

The public recipe is `probes/menu-character-selection-context.json`. It keeps
VC9 RTM21022 /O2 /Ob1 /MT /EHsc /GL, the real controller callback and53 support
sources, with the same two established normal menu supports. It includes the
complete partner/practice/player-data/music owners and the existing dialogue
caller. Unresolved callee/data anchors keep this a diagnostic link, not a game.
Complete PE/MAP/PDB contributions and all fields are compared; no prefix is
cropped, no branch is normalized and no private register convention is forced.

The initial whole candidate is1224 bytes,340 instructions/1183 code,21 layout
and all20 table bytes, with59 fields and707 full differences. All32 calls and
eight RET4 paths agree, and frame0 already agrees. Menu receiver EBX and selection
EBP are reversed from native EBP/EBX; script remains stack input to the shared
child finder instead of native EBX. Close caches one native character-global
reload across Pop, explaining the missing field. The source reuses its incoming
parameter slot for ID temporaries naturally, without a new frame or fake return.

Controlled natural-source variants are kept as private negative evidence:

| Model | Bytes | Full differences | Fields | Concrete result |
| --- | ---: | ---: | ---: | --- |
| Initial case-local pointers/indexed IDs |1224|707|59|Frame0 retained|
| Explicit physical ID fields448/4B0 |1256|1197|59|Introduces frame8; discarded|
| One selection pointer before guard |1200|1140|59|EBP menu/EBX selection, but pointer hoisted before guard|
| One pointer assigned per case |1224|707|59|All13 inspected owners byte-identical to initial|
| Four child expressions read through owner |1224|707|59|All13 inspected owners byte-identical to initial|
| Shared pointer plus owner reads |1200|1140|59|All13 byte-identical to shared-pointer model|

All variants preserve32 calls, all20 table bytes and eight RET4s. The12 sibling
owners remain byte-identical across these probes after exhaustive address-field
replay, and match the previous52-support partner context. Constant-size/difference
statistics alone are not used as proof of equivalence. No model is retained just
to select registers, add stack storage or imitate the target's physical ABI.

A69-support experiment adds the existing real HUD owner and callback together
with the dialogue/gameplay/debug cycle, preserving all four established normal
supports. This supplies the dialogue interpreter's actual HUD caller omitted
from the earlier gameplay-only union. All13 inspected contributions are still
byte-identical to the53-support baseline after exhaustive field replay. This
larger context is not retained, and the result does not prove original TU or
nominal class ownership. The shared child finder stays76/41 with script stack
input. At this checkpoint page05 had no maintained body. The actual native
42C8D0/42D9D0/42E6B0 callers also remain source-open; linking the existing
ActorInstructions/HUD/gameplay/debug sources does not implement those owners.

All245 previous source inputs remain unchanged; the two new files extend the
freeze to247. No accepted canonical unit is affected and no exact unit is added.
The older52/48-support menu receipts remain valid. Checkpoint verifier:
`.analysis/verify-menu-character-checkpoint.py`. Tracking is610 reviewed
boundaries,492 mappings and236 exact/31881 bytes. Source coverage and complete
comparison evidence do not claim byte equality or native-product closure.
