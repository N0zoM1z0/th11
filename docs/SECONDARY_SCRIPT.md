# Secondary script interpreter and typed stack

Twenty-two complete source mappings cover9,160 target bytes, including the full
interpreter7,093 and frame-transfer584. Nine new unique public primary-TU owners
add296 raw-equal exact bytes. Twenty-two further boundary/origin reviews plus
six earlier owners rechecked cover28 complete target spans. The omitted83-byte
typed pop entry is recovered; all reviewed origins remain indeterminate.
Subsequent program/selector work and current affected cold proof are recorded
in [SECONDARY_PROGRAM](SECONDARY_PROGRAM.md).
Current extension/trace source, diagnostics and affected proof are in
[SECONDARY_EXTENSION](SECONDARY_EXTENSION.md).

| Owner | Address | Full bytes | Current source / diagnostic differences |
| --- | --- | ---: | --- |
| RunSecondaryScript | 45B860 | 7,093 | 6,993 /5,880; not exact |
| EnterSecondaryScriptFrame | 45B610 | 584 | 572 /546 |
| UpdateSecondaryScripts | 45D420 | 142 | 155 /142 |
| ReadSecondaryIntArgument | 45D4B0 | 146 | 124 /125 |
| ReadSecondaryFloatArgument | 45D550 | 175 | 184 /168 |
| ReadSecondaryIntReference | 45D600 | 100 | 94 /85 |
| ReadSecondaryFloatReference | 45D670 | 145 | 147 /85 |
| SecondaryIntDestination | 45D710 | 48 | 47 /37 |
| SecondaryFloatDestination | 45D740 | 89 | 105 /97 |
| StartSecondaryThread | 45D7A0 | 147 | 154 /61 |
| FindSecondaryThread | 45D840 | 36 | Exact; no fields |
| StopSecondaryThreads | 45D870 | 31 | Exact; no fields |
| FindSecondarySubroutine | 45DB10 | 126 | Exact; no fields |
| PushSecondaryValue | 45DC30 | 71 | 70 /15 |
| PopSecondaryValue | 45DC80 | 83 | 83 /2; ADD-negative versus SUB |
| BeginSecondaryStackFrame | 45DCE0 | 64 | Exact; no fields |
| EndSecondaryStackFrame | 45DD20 | 41 | 41 /2 |
| RelativeAddress | 45B5C0 | 15 | Exact; no fields |
| SecondaryStackAbsoluteAddress | 45B5D0 | 3 | Exact; no fields |
| SecondaryStackTop | 45B5E0 | 7 | Exact; no fields |
| SetTop | 45B5F0 | 7 | Exact; no fields |
| SecondaryScriptTable | 45B600 | 7 | Exact; no fields |

The true six-slot manager interface and current lifetime proof are recorded in
[SECONDARY_LIFETIME](SECONDARY_LIFETIME.md).

## Corrected complete interpreter owner

The initial6,759-byte interval was code-only. Independent PE decoding covers all
1,783 instructions through45D2C6. A native NOP at45D2C7 precedes61 DWORD dispatch
entries at45D2C8..45D3BB. The89-byte selector at45D3BC..45D414 maps signed opcodes
0..88 to those entries. Eleven CC bytes then separate the next owner45D420.
The complete owned interval is45B860..45D414,7,093 bytes:6,759 code+1 alignment+
244 jump table+89 selectors. Maintained size/span now includes both tables.

Every jump target lies within the reviewed code and every selector is0..60.
All61 Ghidra references previously external to its code-only body originate in
these owned cells and independently agree with raw PE decoding. There are no
foreign interior entries after correcting ownership. The default250-instruction
review output is supplemented by a full2,200-limit export ending at the actual
RET4; no bounded prefix or cropped extent is accepted as a full review.

The source contains normal switches/templates and no table byte payload. Its
current candidate also owns61 jump cells and89 selectors, but6,993 bytes and5,880
masked diagnostic differences remain. Matching table counts is not exactness.

## Physical views and call contracts

The16-byte instruction has signed time0/opcode4, unsigned byte extent6,
reference mask8, difficulty byteA, argument countB and unknown DWORDC. Payload
starts10. The stack has4,096 data bytes, top1000 and frame base1004. Earlier
frame-offset names on getters45B5E0/F0 were wrong: they access top, while relative
address45B5C0 uses frame1004. Names and ledger notes are corrected.

VM time/instruction are0/4, stack8, top/frame1008/100C, id1010, owner1014,
unknown1018, difficulty101C and flags1020; size1024. The manager's current VM is4,
embedded main8, program pointer102C and sentinel/list1030; static assertions check
these observed offsets. The program prefix has file count4, subroutine count8,
32 file pointersC..8B and sorted name/body table8C. These are physical views,
not claims of original source spelling or full data ownership.

Five observed manager virtual methods execute a host instruction, read an integer,
return integer address, read a float and return float address. Receiver, stack
arguments and return contracts are typed as ordinary C++ virtual methods. In
particular45D600's negative-reference tail jump is slot4, not the unknown jump
table suggested by Ghidra. Actual polar and math wrapper boundaries remain
external declarations; no placeholder implementations or guessed intrinsic names
are supplied.

The manager's actual sixth slot is its virtual deleting destructor. Its five
real default bodies and ordinary destructor47 are now reconstructed; the former
pure-virtual-only view is superseded. The scalar deleting body remains non-exact
and has ambiguous candidate publics; see the lifetime record.

## Stack, references and frame transfer

Typed pushes reserve a4-byte tag cell and a DWORD value; raw pushes omit the tag.
Native capacity checks and0/-1 statuses are preserved. Missing pop83 writes its
output before decreasing top for the tag, then performs only observed i/f
conversion. It returns-1 without writing output on underflow. Unknown tags keep
their bits. This ordering matters when the output aliases stack state and is
retained in the shared inline implementation and public owner. Natural
non-float-first conversion now reduces83/27 to83/2; a genuine Stack-member probe
does not change the remaining encoding gap. No exact credit is granted.

Reference values>=0 address frame-relative bytes. Integer-1 and float-1 request
a typed pop; other negatives call real manager slots. Raw integer reference
underflow retains-1; integer argument underflow instead retains its argument
index; float argument underflow retains that index's bit pattern. There is no
invented common zero fallback. Destination lookup has no stack-pop special case.

Frame entry copies source f/g versus integer operands and destination f versus
integer cells, resolving masked references in the caller's VM. It writes argument
values above reserved return metadata, preserves the actual saved-frame/time/
instruction layout and performs the native subroutine name lookup. It temporarily
changes manager.current. Success restores its previous value; lookup failure
leaves the new current VM published and clears the caller instruction before-1.
No cleanup guard is added to erase this failure asymmetry.

Begin-frame64 retains signed size and capacity behavior, optional prior-frame
push and unconditional frame publication. End-frame41 optionally restores the
previous frame then restores top. Its two residual bytes are ADD-negative versus
SUB encoding; they are not masked, patched or padded into an exact claim.

## Interpreter and scheduler behavior

Execution checks ordered instruction-time versus VM-time and difficulty masks.
It preserves stop, return, frame-call, relative/conditional branch, auxiliary
thread creation/search/flags/clear and host-default operations. Host result-1
yields0 without advancing time. Other processed instructions advance by their
actual unsigned extent; time increases by delta only at the native exit.

Every arithmetic/comparison/logical operation has its own template-instantiated
operand temporaries. Wrapped integer arithmetic, signed division/remainder,
ordered floating comparisons and raw typed-pop behavior are explicit. No new
divide-by-zero, empty-stack, destination-null or instruction-readiness guards are
introduced. Opcode85 is particularly easy to misread: native `NEG EDX` negates
the DWORD representation of a converted float and pushes it as type f. It is
not FCHS or floating subtraction. Source preserves that integer bit operation.

Geometry and clock operations retain actual argument evaluation/publication
order. The squared-length operation rounds both fetched coordinates to float,
resolves the output pointer, then computes/stores the result; it does not compute
before the pointer callback. Relative-angle inputs and delay preserve native
intermediate handling. The trace operation keeps the actual1024-byte allocation,
string copy/search loop, reference resolution/conversion and free, even though
its called trace entry is RET-only. Integer-to-double trace promotion avoids an
extra float rounding. Original bounds/failure behavior is not replaced.

The scheduler handles embedded main first, snapshots each next node before
execution, and publishes current VM. Main failure returns-1. Auxiliary failure
deletes the actual current VM, unlinks/deletes its node, then continues. Success
restores embedded main. Stop-all scans auxiliary nodes only and snapshots next
before clearing instruction; FindThread includes the sentinel/main node.
Subroutine search126 keeps native signed binary search and unsigned byte string
ordering, returning body+16 or null.

## Compiler evidence, origin scope and cold proof

A grouped-operation baseline emitted3,382 interpreter bytes. Per-operation
instantiation brought a first model to7,125. Recovering the real pop implementation
corrected output/tag alias order and changed code generation; separating the
remaining branch/thread/unary groups restored all61 dispatch entries. Current
real-trace candidate6,993/5,880 remains open, including a64-byte-aligned EBP
frame versus native ESP frame120. A controlled `/Zp4` probe changed none of the diagnostics and was
rejected; canonical `/O2 /Ob1 /GL` remains. No alignment/register/byte injection
was introduced. A later private double-to-float-temporary probe also retains
alignment64, produces6873/5934 against the preceding source and risks a changed
rounding point; it is rejected. Real Stack members for relative address and
SetTop closed their five/one differences without assigning a fake calling convention.

New reviews cover16 newly reviewed source owners and six still-source-open
neighbors: program installation513, manager creation77, polar vector30 and three
math wrappers31/34/31. Six previously reviewed stack/frame owners were rechecked.
Program installation validates SCPT/version1 and merges sorted named subroutines;
its full513-byte source and real virtual contract are now in
[SECONDARY_PROGRAM](SECONDARY_PROGRAM.md); the derived callback is now fully
reconstructed in [SECONDARY_EXTENSION](SECONDARY_EXTENSION.md), while constructors
remain source-open. The new83-byte pop is
bounded by RET and CC neighbors with no interior entry. Custom script roles,
format magic and generic math/stack wrappers do not establish original game,
header or library authorship. All28 origins remain indeterminate.

Canonical new family `build/match-linked/SecondaryScript` accepts exactly nine
complete unique public primary-TU contributions /296 bytes. Every body has zero
address fields; exhaustive decoding confirms an empty field list, and every
actual byte agrees. Current source/header/tool/producer/PE/MAP/PDB receipts and
full ownership initially passed `.analysis/secondary-script-nine-cold.log`.
After the current header/inline changes all nine pass again in
`.analysis/secondary-program-affected-cold.log`, together with three new program
units. The current trace declaration/body changes have a wider affected receipt
set, recorded in [SECONDARY_EXTENSION](SECONDARY_EXTENSION.md). No full cohort, broad behavior
harness or infrastructure tests ran;
comparison/build infrastructure is unchanged. Whole-game runtime and port stages
remain open.

Private evidence includes interpreter owner/full-code exports and complete
mapped-text attestations, full support/math reviews, missing-pop span,
secondary-vm-independent-review.json, secondary-vm-independent-tables.json,
full argument-reference export (the initial80-reference view was truncated),
typed/grouped/per-operation/member/distinct/profile probes and current cold
receipts. Original instruction/table bytes, decompiler hypotheses, tool payloads
and PE/MAP/PDB artifacts remain private.
