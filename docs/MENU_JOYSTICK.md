# Complete joystick configuration owners

The joystick page has full natural source for its908-byte state machine,
2439-byte display updater and200-byte assignment helper. The display updater
is the substantial owner in this checkpoint. Source presence, byte equality,
original interfaces/TU ownership and runtime behavior remain separate claims.

## Complete target boundaries

Direct mapped-text-attested local Ghidra review, complete1200-limit code export,
reference/neighbor inspection and independent original-PE CFG establish:

| Owner | Complete bytes | Reachable code/instructions | Layout/table | Terminators |
| --- | ---: | --- | --- | --- |
| ApplyMenuJoystickBindings43BD60 |2439|2439 /752|None outside reachable code|One RET and tail JMP44AB40|
| UpdateMenuPage0443B9D0 |908|885 /216|3 alignment +five-cell20-byte table|Four RET/EAX1 paths|
| AssignMenuJoystickButton43C6F0 |200|200 /48|None|One shared RET|

The display owner includes all twenty late child-success blocks at43C62F..43C6E2
and the final missing-VM return at43C6E3..43C6E6. The earlier43C62A jump is an
external tail binding, not the end of the owner. Never crop off the late blocks.
Its59 ordinary calls comprise40 lookups and19 bindings, followed by one tail
binding. All61 address fields include that tail REL32. Nine CC precede43C6F0.
There is no foreign interior entry or table.

The page's table43BD48..43BD5B accounts for all five apparent foreign interior
DATA entries; four CC precede43BD60. Its initial885-byte Ghidra span omitted23
owned bytes. Assignment has no foreign interior entry/table and eight following
CC before43C7C0. Actual inputs are EDI menu for display, EBX menu for page04,
and EAX selection/ECX button/EDX menu for assignment. Conventional C++ signatures
leave private register allocation to /GL; no register ABI is forced.

## Observed storage and operations

The partial menu view now types the actual value ID at2CC and five staged signed
shorts59C4..59CC. Existing offsets and view size remain unchanged. The existing
JoystickBindingsView supplies fire/bomb/slow/pause plus skip at+10; its four
middle words are copied unchanged when saving the complete18-byte record to
MainRuntime+1CC. No duplicate binding globals or allocation-size claim is added.

Each staged binding has two digits in two script groups, all using sprite base2A:

| Staged binding | First tens/ones | Second tens/ones |
| --- | --- | --- |
| Fire |3B/3C|45/46|
| Bomb |3D/3E|47/48|
| Slow |3F/40|49/4A|
| Pause |41/42|4B/4C|
| Skip |43/44|4D/4E|

Every digit independently looks up the root, clears a missing root ID, traverses
the related draw nodes by16-bit script, and performs the second lookup even for
ID0. The native unchecked root traversal is retained; only the final sprite
write is VM-guarded. Reads of the staged binding occur after lookup. Direct
signed-short /10 uses the native multiply/sign correction, while %10 uses IDIV10.
There is no invented+1, negative clamp or leading-zero visibility operation.

The seven-selection page loads staged bindings on opening, starts script2/layer16,
and activates after frame6. Press/repeat navigation remains independent. Polling
uses the existing complete PollJoystickButtons implementation. The page scans
indices0..30, stops at the first pressed button, and assigns it only when the
selection is<=4; no additional lower-range guard is introduced. Selection5 reloads
current bindings and refreshes the display. Cancel on selection6 reloads/discards
and closes; confirm on selection6 commits before the shared close. After10 closing
frames it returns to page3 and pops selection history.

Assignment exits when the signed old binding equals the requested button. Five
ordered conflict tests copy the selected old value into any conflicting other
slot, rereading the selected slot as native. It then writes the new short,
refreshes all twenty digits and queues sound10. These are target/source operations;
no broad hardware, allocation or multiframe execution suite is implied.

## Shared sprite coefficient correction

Fresh full review of389-byte BindAnimationSprite44AB40 verifies the QWORD load
at44AC7A from497F50. The actual little-endian double there is1/256, not0.5.
The previous source multiplied width/height by0.5f. Both transform scales now use
`*(1.0 /256.0)`, retaining the observed double coefficient and float destinations.
Other binding operations and interfaces are unchanged. This is a concrete target
value/type correction; its caller matches never established callee correctness.

The binding body remains non-exact: in the canonical audio graph its complete
support contribution is388/native389 with293 differences after the one coefficient
field. Its input file is EDX there. The new joystick graph instead passes file
on the stack and reuses that slot for float scratch, changing binding/caller code.
No source API is misdeclared to hide that difference.

## Current codegen and experiments

The first complete display output is2592/native2439:778 reachable instructions,
2476 code bytes and116 internal alignment bytes,61 fields and2328 full differences.
It has40 lookups and20 ordinary bind calls; the native final tail jump is absent.
The file parameter is pushed at each bind. Explicit void-tail forwarding, a
computed final-binding status result, a combined audio/display TU, a normal file
member-method model, and an additional real ASCII caller do not recover the
native register/tail context. None is retained as a public interface or TU claim.

The page contribution is908/native908/833 full differences after62 fields;
assignment is199/native200/200 differences after2 fields. The new real caller restores all36
outer-controller call sites (formerly32 with only the primary page), but its
1188/native1224 contribution remains non-exact. Equal sizes or counts grant no
byte credit. The earlier1608/30-difference primary page result belongs to its
isolated context; inspect current diagnostics before using it as current state.

The current primary-page support is1604/native1608/1320 differences after116
fields. In this broader graph audio apply is3900/native3836/3525 differences
and its992-byte caller has823 differences. These do not replace canonical audio
acceptance: its own freshly cold-built context still has3836/110 fields/zero
and992/61 fields/9. The binding is394/native389/347 differences in the new graph
versus388/native389/293 in the canonical graph. No function borrows a support
owner's proof or treats a context change as integrated game closure.

Six focused primary-page probes are also negative: shared selection pointer or
reference gives1604 and removes a target address calculation; a separate ID local
gives536 differences, explicit scalar/member-address spellings stay30, and /GS-
stays30. The maintained primary source/profile are unchanged. Full comparisons
and field replay, not masked similarity or a shortened prefix, decide progress.

## Reproduction and scope

probes/menu-joystick-context.json is a public compile-only recipe with the display
as primary,28 real/probe supports and the actual menu/controller/input bodies.
Only the already observed callback and general selector are normal supports;
the new owners and established linked owners keep /GL. Use the existing cold
producer as in MENU_PRIMARY.md, selecting this JSON recipe. The receipt binds
all source, tools, producer and PE/MAP/PDB artifacts. Diagnostic unresolved data
anchors are never executable substitutes or a complete game product.

The coefficient change directly affects11 accepted units in8 compilation families.
Only those families are cold rebuilt and compared through the strict linked
Oracle. The new joystick family also receives a full cold build; no new exact
unit is claimed while register/tail and body differences remain. Tool/Oracle
implementations are unchanged, so public infrastructure tests and the complete
historical cohort are not checkpoint requirements.
