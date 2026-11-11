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

Pinned VC9 `/O2 /Ob1 /Gy /MT /EHsc` normal COFF initially generated
**855 bytes** for the browser and 93 for cleanup. In a narrow genuine-source `/GL` diagnostic
with a compile-only independent callback, the browser is **859 bytes/249
instructions/22 calls**, versus native **934/267/22**; cleanup is **87
bytes/30 instructions**, versus native **83/29**. Including the real menu
selection and gameplay-support TUs changes browser codegen to 843 bytes,
with the same 22 call sites. These browser candidates remain non-exact.

For the 83-byte cleanup, introducing the naturally reused local
`char *filename = self->filenames[i]` before the null test changes the
original linked 87-byte form to the native **83 bytes/29 instructions**.
Strict cold Oracle `secondary-program-browser-clear` independently compiles
that source, verifies complete primary-TU PE/MAP/PDB ownership and current
compiler/producer receipts, exhaustively binds the two `_free` REL32 calls
at native offsets `0x1C` and `0x3D` to `0x45FC3B`, and replays **zero byte
differences**. See `.analysis/secondary-browser-clear-strict-proof-20261111.log`
and `build/match-linked/SecondaryProgramBrowserClear/`. No full cohort was
cold-replayed.

The selection-count initialization previously folded away branches even
though native code reloads object fields after writing them. The scoped
codegen restoration below addresses this gap, while the overall browser
and manager ABI remain non-exact.

Private diagnostic recipe and result context:
`.analysis/probe-secondary-program-browser.py`,
`.analysis/browser-direct-producer.cpp`, and
`build/probe-ltcg/SecondaryProgramBrowser-*`. No full cold replay was run.

## Real loader thread and 277-byte file selection (2026-11-11)

The source-open browser's state-1 file confirmation originally registered
`0x438900` as its thread callback (a DATA xref at `0x438CBE`), not the
separate generic debug loader. Its complete eight-byte native body is
`CALL 0x438910; XOR EAX,EAX; RET`, with one preceding CC byte, eight
following CC bytes, and no other references into its interior. Ghidra
originally labeled it `LAB_00438900` instead of including it as a function;
complete PE/Capstone review now establishes a separate function candidate.
`SecondaryProgramBrowserLoadThread` in
`src/SecondaryProgramBrowserThread.cpp` is the exact C++ callback selected
by the maintained browser state machine.

`LoadSecondaryProgramSelection` at `0x438910..0x438A24` is the real
**277-byte** loader. Its new natural C++ in
`src/SecondaryProgramSelection.cpp` formats the selected filename in a `MAX_PATH` buffer through
`"%s"`, deletes the previous manager, frees the old browser input blob
at +0x2E0, loads a fresh resource in disk mode 0, constructs a 0x1098-byte
`SecondaryProgramView`, calls its virtual Install slot, publishes the
new subroutine count and resets the selected subroutine and browser state.
The source retains the original two manager-null assignments and the lack
of new allocation-failure guards. The base program's vptr/memset order and
actual original TU partition remain open, so the source is not treated as
an executable runtime guarantee.

The loader now uses the observed Windows `MAX_PATH` (260) local buffer,
which restores the original `0x108` stack allocation instead of `0x104`.
A natural three-way zero/positive/negative selection rule preserves the
original signed zero checks without inventing additional error paths.
Compiling its genuine `ResourceFile.cpp` dependency with the existing archive,
cipher and critical-section support produces **277/277 bytes, 81/81 decoded
instructions, and all 8 native calls**. Of the full native span, **18
non-address-field bytes still differ**; the remaining discrepancies cluster
around the constructor vptr-store order and final selected-index register
allocation. This is improved source codegen, *not* an exact claim.

The unchanged resource loader itself reproduces its full **466/466** body
with 36 reviewed fields and no nonfield differences in this targeted context.
`secondary-program-browser-load-thread` was separately cold-replayed after
the loader edit and remains strictly exact **8/8**, with its one REL32 call.
Proof logs: `.analysis/secondary-loader-full-resource-current-20261111.log`
and `.analysis/secondary-program-load-thread-after-loader-change-20261111.log`.
The rejected natural variants remain under `.analysis/probe-loader-*20261111.py`.


The new eight-byte callback has separately passed strict cold-linked Oracle
`secondary-program-browser-load-thread`: unique primary-TU PE/MAP/PDB owner,
one independently reviewed REL32 call field at offset +1 to `0x438910`,
current source/producer/tool receipts, and **8/8 exact with zero differences**.
The pre-existing `secondary-program-browser-clear` 83-byte exact owner was
cold-rechecked in the same scoped run after the shared header/caller edits
and remains **83/83 exact**. Evidence:
`.analysis/secondary-loader-two-scoped-exact-20261111.log` and
`build/match-linked/SecondaryProgramLoadThread/`. No full cohort replay.

## Selection-count control-flow restoration (2026-11-11)

The complete 934-byte `UpdateSecondaryProgramBrowser` owner now retains the
native post-store **count reload** in its three menu-selection initializers.
The existing helper had passed the literal counts `3`, `count`, `1`, allowing
VC9 /GL to fold the zero/positive/negative branches. The maintained C++
reads the just-stored physical field through a volatile-qualified view
(once per initializer) and spells distinct zero, positive, and negative
paths. The negative DEC is expressed through defined unsigned subtraction
so the original 32-bit wrap does not become a signed-overflow assumption.
These are actual selection state operations, not padding or ABI shims.

Scoped pinned VC9 `/GL` against the real native extent now yields **912/934
bytes**, **267/267 decoded instructions**, and **22/22 call sites**. Before
this change the same narrow producer yielded 859 bytes, 249 instructions
and 22 calls. The 53 additional bytes and 18 restored instructions recover
previously lost codegen paths. The owner remains strictly **non-exact**:
659 non-address-field positional byte differences remain, as do branch-layout
and register-selection differences. The real menu selection support context
produces a separate 896-byte diagnostic and must not be mistaken for proof.

The affected-only exact cold replay of `secondary-program-browser-clear` also
passed **83/83**, two CRT free bindings and **zero byte differences** after
the shared source change; no unrelated or full-cohort cold build ran.
Diagnostics: `.analysis/secondary-program-browser-restored-count-20261111.log`,
`.analysis/try-browser-selection-count-codegen-20261111.py`,
`.analysis/try-browser-branch-early-return-20261111.py`,
and `.analysis/secondary-program-browser-leaf-recheck-20261111.log`.

## Exact 77-byte manager factory (2026-11-11)

The browser's factory at `0x45DB90..0x45DBDC` is now natural C++ in
`SecondaryProgramManagerFactory.cpp`. It allocates the real 0x103C-byte
`SecondaryManager`, initializes the embedded VM stack fields, publishes the
program at `+0x102C`, resolves the selected subroutine and stores the
initial instruction and time through `current`. Under the real browser and
`SecondaryScript.cpp` `/GL` producer the complete function is **77/77
bytes exact**. Its three fields are `operator new` REL32 at `+7` to
`0x45FCE4`, manager vtable DIR32 at `+22` to `0x494058`, and
`FindSecondarySubroutine` REL32 at `+54` to `0x45DB10`. The cold-linked
PE/MAP/PDB primary owner and all three bindings replay with zero differences;
see `create-secondary-manager` and
`.analysis/secondary-manager-factory-strict-20261111.log`.

The shared `SecondaryVm` constructor now writes `stack.top` then
`stack.frame` as two normal assignments, matching the native ordered zero
stores. **All 28 affected exact units** that consume the changed shared header
or browser callback/factory source have passed selected-only cold replay with
**zero final byte differences**. The exactness log is
`.analysis/secondary-manager-shared-header-exact-20261111.log`. The neighboring 934-byte
browser and 277-byte deferred loader retain their own non-exact status.
