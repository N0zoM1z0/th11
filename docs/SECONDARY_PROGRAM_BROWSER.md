# Secondary ECL program browser

The complete native browser-update owner at `0x438AC0..0x438E65` (**934
bytes, 267 instructions, 22 calls**) and the 83-byte filename cleanup at
`0x438570..0x4385C2` now have maintained C++ in
`src/SecondaryProgramBrowser.cpp/.hpp`. Both were mapped from the original
Japanese TH11 v1.00a executable and compiled with pinned VC9 RTM. These are
**source-present, non-exact** owners, not binary-exact claims.

## Physical browser view

The state is at +0x30, filename array +0x34, count +0x38. Three distinct
`SecondaryDebugSelectionView` objects begin at +0x3C, +0x114, and +0x1EC;
their `count` and `wrapEnabled` fields are kept separate. The display origin,
scale and offset are at +0x2C4/+0x2C8/+0x2CC, the timer +0x2D0, flags
+0x2D4, the ECL program +0x2D8 and the active script manager +0x2DC.
Compile-time layout checks cover these addresses. No original class name or
translation-unit partition is inferred from this view.

The 83-byte filename owner receives its browser in the native private ESI
register; it frees individually non-null names, clears their slots, then
frees and clears the pointer table without modifying the existing count.
Its two real native callers are at 0x438710 and 0x438AC0. Independent full
PE/Capstone decoding establishes 29 instructions, two `_free` calls, one
terminal RET, no embedded data and no foreign interior references. The
preceding CC bytes and following alignment are not part of its 83-byte span.

## Complete browser state machine

Mode **0** releases any old list, enumerates `../../data/*.ecl` via
`FindFirstFileA`/`FindNextFileA`, counts the first result, closes the handle
without adding a new invalid-handle test, allocates the filename table and
copies each discovered name. It initializes category, filename and subroutine
selections to counts 3, discovered-count and 1, with wrap enabled. It then
enters mode 1, sets the display timer to 1000, and initializes the observed
float display fields (0/32/0).

Mode **1** copies the category's current selection into its previous field,
then moves with the down/up 0x20/0x10 bits from pressed *or* repeat input.
Category 0 navigates filenames on right/left 0x80/0x40 and, upon confirm
press 0x80001, clears flags bit 2, starts the existing script-load thread
and enters mode 2. Category 1 navigates subroutines only when a program is
present; its **released** confirmation bit (rather than pressed) deletes any
prior manager and constructs one from the selected subroutine name, entering
mode 4. Category 2 requests runtime mode 3 on confirmation.

Mode **4** ticks the manager through `UpdateSecondaryScripts` with delta
1.0f. If the main script's instruction pointer remains non-null, the browser
stays in mode 4. Otherwise it invokes the manager's existing virtual
deleting-destructor path, clears the pointer and returns to mode 1. The
native calls the scheduler before the manager-null check. All modes return 1.
The source does not invent null checks for file allocation, filename copies
or manager objects, and leaves the actual `CreateSecondaryManager` owner at
0x45DB90 as an independent unresolved dependency.

## Focused codegen and next questions

Pinned VC9 `/O2 /Ob1 /Gy /MT /EHsc` normal COFF generates **855 bytes** for
the browser and 93 for cleanup. In a narrow genuine-source `/GL` diagnostic
with a compile-only independent callback, the browser is **859 bytes/249
instructions/22 calls**, versus native **934/267/22**; cleanup is **87
bytes/30 instructions**, versus native **83/29**. Including the real menu
selection and gameplay-support TUs changes browser codegen to 843 bytes,
with the same 22 call sites. No profile has earned exact credit.

The most concrete browser gap is the initialization of the three embedded
selections: native code dynamically reloads each saved count and branches
through the zero/negative/positive rule, even immediately after the count
stores, while the current source's generic helper lets VC9 constant-fold
some of those branches. The native cleanup's ESI register ownership also
remains to be restored under an appropriate producer. Further work should
restore those normal source dataflow boundaries and preserve the true manager
construction ABI, rather than padding byte spans or masking mismatches.

Private diagnostic recipe and result context:
`.analysis/probe-secondary-program-browser.py`,
`.analysis/browser-direct-producer.cpp`, and
`build/probe-ltcg/SecondaryProgramBrowser-*`. No full cold replay was run.
