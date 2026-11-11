# Secondary program installation and context selection

Five complete source mappings add610 target bytes: program installation513,
the base extension callback5, and three previously omitted context entries
28/36/28. Three unique public primary-TU contributions pass69 exact bytes with
two independently reviewed REL32 fields. Installation remains non-exact.
Later complete extension/file source and current proof are recorded in
[SECONDARY_EXTENSION](SECONDARY_EXTENSION.md).

| Owner | Address | Full bytes | Current source / diagnostic differences |
| --- | --- | ---: | --- |
| InstallSecondaryProgram | 45D900 | 513 | 513 /339; not exact |
| SecondaryProgramExtensionDefault | 40FFA0 | 5 | Exact; zero fields |
| InitializeSecondaryContext | 45DBE0 | 28 | Exact; one REL32 field at8 |
| SelectSecondaryScript | 45DC00 | 36 | Exact; one REL32 field at6 |
| RestartSecondaryScript | 45DD50 | 28 | 28 /10; not exact |

Current construction/destruction and full raw storage views are recorded in
[SECONDARY_LIFETIME](SECONDARY_LIFETIME.md).

## Real virtual and file contracts

The base program vptr49404C points to Install45D900 in slot0 and the five-byte
extension callback40FFA0 in slot4. The derived table494090 retains Install,
overrides slot4 with410D50 and adds file loading4104D0 in slot8. The preceding
RTTI locator cells are separate from each vptr. Slot0 is not a destructor.
The extension callback returns an integer: the base really returns0/RET4;
the derived callback has observed0/-1 results. Install ignores that result.
These are normal C++ virtual declarations, with no guessed destructor slot.

The physical prefix remains fileCount4, subroutineCount8, file pointersC..8B
and the name/body table8C. The initial header described only that prefix.
Constructor/allocation evidence now supplies a1098-byte raw storage view,
without guessing its remaining member identities or ownership; see the lifetime
record. Adding ordinary manager methods does not change existing field offsets.

The36-byte file header contains SCPT magic0, version WORD4, extension extent
WORD6 and subroutine-count WORD10. Offsets begin24+extensionBytes; names follow
the DWORD offset array. Bodies are relative to their owning file allocation.
Unknown header bytes remain unknown. The old description of slot4 as an
include-only callback was too narrow: its derived implementation first reads
ANIM and only then optionally ECLI.

## Full installation behavior

Install publishes files[fileCount] before checking magic/version1. Rejection
clears that slot and returns-1 without freeing the input. It snapshots the old
table, adds the new count and allocates the full new name/body table. No invented
file-capacity, allocation-failure or null checks are added.

With no old table, it publishes each relative body and name using the native
total-count loop. With an old table it copies the old prefix, frees the previous
allocation, finds each insertion position by strcmp and shifts entries backward.
The shift starts at totalCount-1, including slots not yet populated by the new
file. Changing that bound to usedCount-1 would change the native behavior.
Each name advances by strlen+1. It then increments fileCount, optionally invokes
slot4 on file+24, ignores the callback's result and returns the prior file index.

Separate magic/version checks and ordinary strlen calls reduce the diagnostic
from506/446 to513/339. The complete owner is513 bytes; matching its length does
not close differing registers, call cleanup, allocation-size computation or
internal alignment. No bytes, padding, register declarations or failure paths
were injected to force equality.

## Context entries recovered from gaps

Three missing entries lie between already reviewed functions and CC separators.
Independent complete PE decoding and read-only Ghidra span exports establish
45DBE0..45DBFB/28,45DC00..45DC23/36 and45DD50..45DD6B/28. No interior entries,
embedded tables or adjacent CC bytes belong to these owners.

Initialize publishes its explicit program, looks up the name, writes the result
through manager.current and resets its time. It returns0 even for a missing
name. Select uses an explicit program without replacing manager.program, writes
instruction/time and returns whether the instruction is null. Restart uses the
stored program and returns0 unconditionally. None initializes current or adds a
null guard. Restart's remaining10 differences are argument/register scheduling,
not a different status contract.

Initialize's field8 and Select's field6 independently decode as CALL REL32 to
FindSecondarySubroutine45DB10. Exhaustive candidate decoding finds exactly
these fields. The base callback consists only of XOR EAX,EAX and RET4 and has
no address fields. All three canonical units require full strict comparisons.

## Corrected callback boundary and expanded origins

The default inventory split410D50 into a25-byte rejection gate and a213-byte
function at410D69. That second address is reached by the first entry's JE,
uses its saved self/input/stack and rejoins the same RET4 contract. The true
full interval is410D50..410E3D/238 bytes, including native internal alignment.
A full explicit-span export and independent94-instruction decode agree; no
foreign interior references or tables occur, and the next entry is410E40 after
two CC bytes. The three returns cover bad magic, loading failure and success.
Only the two never-maintained auto candidates are merged; no maintained source
or exact owner is removed. The private database is unchanged.

The continuation walks counted ANIM names, loads/stores handles, reports a failed
load and returns-1. It aligns the relative cursor to four bytes and, if ECLI is
present, walks counted names through the actual derived slot8. The strings and
format roles establish behavior, not original game/header/library authorship.
The subsequent full source and exact comparison are in
[SECONDARY_EXTENSION](SECONDARY_EXTENSION.md).

Thirteen complete owners were reviewed, twelve for the first time. Besides the
five mapped owners and merged callback, reviews cover file loading95, browser
loading277, browser update934, adjacent effect initialization/tick/draw188/177/84
and the DirectInput import thunk6. The full browser export reaches its final
RET after267 instructions; the initial250-instruction view was insufficient.

The browser handles selection/input, file enumeration/copies and manager
creation/ticking/deletion. Its observed vptr/zeroing/allocation order is not
rewritten into an assumed safe constructor. The effect neighbors use animation
scripts23/24, sound/frame gates and position/magnitude fields; they are not
script-manager lifecycle functions merely because their addresses are adjacent.
All eleven newly reviewed non-import origins remain indeterminate.

45DF64 is exactly JMP [48B000]. The original PE import descriptor independently
identifies DINPUT8.dll!DirectInput8Create, agreeing with its direct caller and
Ghidra thunk review. This is one new high-confidence import exclusion, with no
source/exact credit. Net inventory change is+3 missing entries-1 false split:
2,075 candidates,339 reviewed boundaries,272 source mappings; origins90 authored,
239 indeterminate,8 excluded and1,738 pending.

## Affected proof and related compiler findings

The installation checkpoint replayed three new program units and nine affected
secondary-script units. All69 new bytes agree after exhaustive replay of two
fields; the prior296 exact bytes also passed. No unrelated family was rebuilt.

The shared typed-pop conversion now follows native non-float-first branching.
Its full83-byte candidate drops27 differences to2, both the register SUB4 versus
ADD-negative encoding. A genuine Stack-member probe leaves those two differences
unchanged and is rejected. End-frame41 retains the same encoding gap. These
bytes stay unmasked. Shared inline changes also alter reference/interpreter
diagnostics: full Run is now6989/5958, with native61 dispatch cells/89 selectors
and an unresolved64-byte-aligned EBP frame. A private double-to-float-temporary
probe kept alignment64, produced6873/5934 against the preceding source and
could change rounding; it is rejected. Double temporaries remain in source.
The real-trace source now gives6993/5880. Current trace-context experiments and
the updated affected cold receipt supersede those earlier diagnostics; see
[SECONDARY_EXTENSION](SECONDARY_EXTENSION.md).

## ECL program browser source checkpoint (2026-11-11)

The previously source-open **934-byte** `UpdateSecondaryProgramBrowser`
(`0x438AC0`) and its **83-byte** file-list destructor (`0x438570`) now have
maintained C++ in `SecondaryProgramBrowser.cpp`, with an independently checked
physical state/selection layout. Both compile in VC9 COFF and `/GL`; the scoped `/GL` browser retains all
22 native calls but remains non-exact. The separate filename cleanup is now
**83/83 strict linked-PE exact**, with both native CRT free calls bound. The complete state branches,
filename allocation, input-bit semantics and script-manager dependency are
detailed in [SECONDARY_PROGRAM_BROWSER](SECONDARY_PROGRAM_BROWSER.md).

## Source for deferred ECL browser loading (2026-11-11)

The exact thread callback at `0x438900` is now recognized as an independent
8-byte function and has source plus a strict linked-PE proof. It calls the
newly source-present but non-exact **277-byte** `LoadSecondaryProgramSelection`
(`0x438910`). The state-1 selection branch in the larger 934-byte browser now
uses the actual registered callback rather than the unrelated debug-loader
function. See [SECONDARY_PROGRAM_BROWSER](SECONDARY_PROGRAM_BROWSER.md) for
physical state fields, compiler comparisons and unresolved loader ABI.


The current browser source now re-reads the three selection `count` fields
after stores and keeps separate zero/positive/negative paths. Focused VC9
`/GL` improves **859/934 (249/267 instructions)** to **912/934
(267/267 instructions)** with all22 native calls retained. It remains
non-exact; the separately exact83-byte cleanup was cold-replayed after
this local edit. See [SECONDARY_PROGRAM_BROWSER](SECONDARY_PROGRAM_BROWSER.md).
