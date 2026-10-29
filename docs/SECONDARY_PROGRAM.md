# Secondary program installation and context selection

Five complete source mappings add610 target bytes: program installation513,
the base extension callback5, and three previously omitted context entries
28/36/28. Three unique public primary-TU contributions pass69 exact bytes with
two independently reviewed REL32 fields. Installation remains non-exact.

| Owner | Address | Full bytes | Current source / diagnostic differences |
| --- | --- | ---: | --- |
| InstallSecondaryProgram | 45D900 | 513 | 513 /339; not exact |
| SecondaryProgramExtensionDefault | 40FFA0 | 5 | Exact; zero fields |
| InitializeSecondaryContext | 45DBE0 | 28 | Exact; one REL32 field at8 |
| SelectSecondaryScript | 45DC00 | 36 | Exact; one REL32 field at6 |
| RestartSecondaryScript | 45DD50 | 28 | 28 /10; not exact |

## Real virtual and file contracts

The base program vptr49404C points to Install45D900 in slot0 and the five-byte
extension callback40FFA0 in slot4. The derived table494090 retains Install,
overrides slot4 with410D50 and adds file loading4104D0 in slot8. The preceding
RTTI locator cells are separate from each vptr. Slot0 is not a destructor.
The extension callback returns an integer: the base really returns0/RET4;
the derived callback has observed0/-1 results. Install ignores that result.
These are normal C++ virtual declarations, with no guessed destructor slot.

The physical prefix remains fileCount4, subroutineCount8, file pointersC..8B
and the name/body table8C. The header view is only that prefix, not a full
object-size or constructor claim. One browser caller allocates1098 bytes;
the larger derived state remains open. Existing manager slots and field
offsets are unaffected by adding ordinary manager methods.

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
No source is supplied for this still-open238-byte callback.

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

Current source/header/support/tool/producer and PE/MAP/PDB receipts pass
`.analysis/secondary-program-affected-cold.log`: exactly three new program units
and nine affected prior secondary-script units. These are complete unique public
primary-TU contributions. All69 new bytes agree after exhaustive replay of the
two independently bound fields; the old296 exact bytes remain valid. No other
family or full cohort was replayed. Comparison/build infrastructure is unchanged.

The shared typed-pop conversion now follows native non-float-first branching.
Its full83-byte candidate drops27 differences to2, both the register SUB4 versus
ADD-negative encoding. A genuine Stack-member probe leaves those two differences
unchanged and is rejected. End-frame41 retains the same encoding gap. These
bytes stay unmasked. Shared inline changes also alter reference/interpreter
diagnostics: full Run is now6989/5958, with native61 dispatch cells/89 selectors
and an unresolved64-byte-aligned EBP frame. A private double-to-float-temporary
probe kept alignment64, produced6873/5934 against the preceding source and
could change rounding; it is rejected. Double temporaries remain in source.

Private evidence: secondary-program-effects-review, secondary-program-callers-review,
secondary-manager-create-caller-full, secondary-context-gaps-review,
secondary-restart-gap-review, secondary-program-extension-review and
secondary-extension-full-review (all mapped-text-attested); independent full-span,
vtable/import and exhaustive field reviews; current pop-order dispatch/selector
shape verification; native-contract, full-contract,
string-order and default-callback diagnostics; pop-order, member-pop-end and
float-temporary probes; current affected cold receipts. Target instruction bytes,
decompiler hypotheses, tool payloads and build artifacts remain private. Native
product, runtime and portability closure are still open.
