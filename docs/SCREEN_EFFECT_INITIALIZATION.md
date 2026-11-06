# Complete screen-effect initialization

InitializeSecondaryHudScreenEffect at448A90 now has complete natural C++ for all
nine modes, registration and state initialization. Its full1168-byte owner is
non-exact: the cold /GL contribution has65 byte differences after all54 reviewed
address fields replay. The actual80-byte factory at4489E0 is included as its real
unknown-mode caller; its zero-difference diagnostic receives no separate leaf
acceptance unit or exact credit. Both original class/TU origins are indeterminate.

## Full extents and interfaces

Fresh direct mapped-text-attested Ghidra reviews, complete code disassembly,
references and independent PE decoding establish:

| Owner | Reachable code | Alignment | Owned table | Full extent |
| --- | ---: | ---: | ---: | --- |
| Initializer448A90 | 1129 /342 instructions | 3 | 36 /9 cells | 448A90..448F1F /1168 |
| Factory4489E0 | 80 /32 instructions | 0 | 0 | 4489E0..448A2F /80 |

The automatic1129 span omitted the table at448EFC. All nine apparent foreign
DATA references into the code are its owned selector cells. The table has only
the initializer's jump reference; no foreign code interior entry was found.
Following448F20 begins separate code immediately after the last cell. The
factory's following448A30 is separate unreviewed code and is not absorbed.
The three alignment bytes before the table remain part of the full owner.

Initializer inputs are six stack words: self, mode, duration, color0, color1 and
draw priority. It returns void through one RET18; EAX's incidental color word
is not a return API. Its callers are405130,41B380 and4489E0. The factory takes
five stack words and returns its object pointer or0 through two RET14 paths.
The decompiler's five/four-parameter signatures omit a real input in each case.

The0x44 physical effect view retains flags0, update/draw pointers8/C, mode10,
unknown14, value18, duration1C, color words20/24, value28, unknown2C and timer30.
The constructor's timer member initialization, whole-object clear and flags2
remain separate from this initializer. These are physical field names, not a
recovered original class declaration.

## Complete mode and registration behavior

| Mode | Update callback | Draw callback |
| ---: | --- | --- |
| 0 | 447F10 /Update0 | 448390 /Draw0 |
| 1 | 448720 /Update1 | unchanged |
| 2 | 448420 /Update2 | 4484B0 /Draw2 |
| 3 | 447F10 /Update0 | 4484B0 /Draw2 |
| 4 | 448610 /Update4 | 4486C0 /Draw4 |
| 5 | 448420 /Update2 | 448390 /Draw0 |
| 6 | 448500 /Update6 | 448580 /Draw6 |
| 7 | 448500 /Update6 | 4485C0 /Draw7 |
| 8 | 448830 /Update8 | unchanged |

Each update is registered at priority14; each draw uses the last input. HUD's
clear-frame180 path supplies mode5/duration200/colors0/0/draw priority0x43. The
older description of0x43 as an initializer kind was wrong. Mode3 stores255 at18
before allocation. Other modes preserve18. Modes1/8 retain an existing draw
pointer, and the default path retains the existing update pointer. No mode
clamp or default-node allocation is added.

Every node is a0x24 allocation. Its actual constructor clears only ownership
bit0, clears the three callback slots and priority, installs its self link and
zeroes next/previous. Context20 is set when registering. The local constructor
adapter uses the shared physical ChainElement interface without changing that
shared header or claiming the original inheritance hierarchy. The existing raw
chain removal uses operator delete on the allocation address.

Binding sets the selected callback, clears0C/10, installs the effect context,
sets node flag bits0/1 and calls the real AddToUpdateChain/AddToDrawChain. The
node pointer is published to the effect after registration. Constructor guards
and the following unconditional writes preserve the observed unchecked paths.
After mode selection, only the update node receives cleanup callback4489A0 in
slot10. Callback bodies, their full extents and destructor448F20 remain open;
the declarations follow the observed ECX context and ChainCallback interface.

Timer30 is lazily initialized only when its flag bit0 is clear: fraction/current0,
previous-999999, scale4A7948 and flag bit0. The subsequent reset sets previous-1,
current/fraction0 while retaining an already installed scale pointer. It then
publishes mode, clears28 and copies duration/color0/color1 in the observed order.

## Whole-owner codegen evidence

The pinned VC9 RTM21022 /O2 /Ob1 /GL family uses the actual HUD caller and the
actual factory, with real ChainRegistration support. Full source and target
both have342 reachable instructions,1129 code bytes,3 alignment bytes and all
nine cells. Physical calls are identical:14 allocations,6 update registrations
and1 draw registration. The six-stack-word interface and RET18 are also restored.

An explicit post-allocation initialization helper first gave1036 bytes with an
EDI self argument and RET14. Expressing the observed initialization as a real
node constructor recovered1168 bytes, the stack self input and150 differences.
Reviewed native entry order places5 before4 and reduced differences to95.
Callback/priority clears before ownership-bit initialization reduced them to65.
Keeping a derived pointer through callback assignment or spelling ownership as
a bitfield is byte-identical; those variants are not retained.

All65 differences lie in five13-byte windows beginning at448AC0,448B02,448B44,
448CF2 and448D81. Target MOV ESI,EAX follows the four callback/priority clears;
source schedules it before them. The complete1168 bytes and all54 address fields
are compared, including alignment/table data. Localizing these windows grants
no masked or partial exact credit. The next work is this constructor scheduling,
with the whole owner and /GL retained.

The factory reproduces its complete80 bytes after its three call fields replay.
It still has no independent acceptance unit. HUD retains4652/frameC/4101 full
differences, all104 fields and all27 calls in this fuller dependency context.
No native product, original TU, callback-body or runtime closure follows.

A bounded static argument replay checks all nine selectors and both unsigned
default edges for callbacks, priorities, cleanup and copied/reset fields. It uses
symbolic successful allocations and an initially uninitialized timer. It is not
an execution, allocation-failure, concurrency or multiframe suite, and does not
prove the callback bodies. No unchanged Oracle infrastructure tests or unrelated
caller regressions were run. No accepted exact unit depends on the changed HUD
header; exact counts remain unchanged.

Private evidence is screen-initializer-* and screen-effect-factory-* Ghidra
exports, full target/PDB/field comparisons, mode arguments, mismatch windows and
cold receipt/source freeze. No executable bytes, decompiler output or databases
are committed.
