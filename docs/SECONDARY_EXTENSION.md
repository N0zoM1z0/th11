# Secondary extension parser, file entry and trace context

Five new complete source mappings and exact owners cover340 target bytes. The
full238-byte ANIM/ECLI callback,95-byte file entry,5-byte file getter and two
genuine RET-only trace functions pass complete strict cold proof. At this
extension checkpoint exact state was156 functions/21,872 bytes; boundary/origin state was2,082 candidates,
349 reviewed boundaries,277 mappings,90 authored,249 indeterminate,8 exclusions
and1,735 pending origins. Later lifetime/source/proof updates are in
[SECONDARY_LIFETIME](SECONDARY_LIFETIME.md).

| Owner | Address | Full bytes | Exhaustive address fields |
| --- | --- | ---: | ---: |
| ProcessSecondaryProgramExtensions | 410D50 | 238 | 5 |
| LoadSecondaryProgramFile | 4104D0 | 95 | 5 |
| SecondaryProgramFile | 40FFB0 | 5 | 0 |
| TraceAnimationLoad | 456AD0 | 1 | 0 |
| TraceSecondaryScript | 45B5B0 | 1 | 0 |

## Complete extension and file behavior

The corrected238-byte owner includes all three RET4 paths and native internal
alignment. Bad ANIM magic returns0. The unsigned count loop loads each name
through AcquireAnimationDocument(index+8,name), then publishes the returned
pointer at singleton+44+index*4 before testing failure. Failure calls the real
AppendLog on the runtime log prefix and returns-1; it does not erase the published
null or release earlier files. Success advances each name by strlen+1.

The cursor is aligned relative to the original extension start using signed
remainder4. ECLI magic enables a second unsigned count/name walk through actual
virtual slot8; those return values are ignored. The parser finally returns0.
No guessed extra header fields, capacity checks or allocation/null guards appear.
The singleton is the same address4A8D7C used by the mesh-effect event-count view;
its complete type and slot capacity remain unknown. The new byte view does not
create another native storage object.

The derived program view overrides slot4 and adds LoadFile at8. It describes the
observed virtual prefix. Its derived constructor and full1098-byte raw storage
view are now reconstructed in the lifetime record; original member identities
remain open. LoadFile clears shared filename byte0, uses strcat with its observed
source scan/destination scan/copy order, loads through LoadResourceFile with null
size and mode0, calls the base Install directly and translates index<0 to-1,
otherwise0. There is no added load-null guard or ownership cleanup. The filename
buffer at4C9928 has no invented public capacity.

Native file getter40FFB0 is files[index] at program+C. Its five-byte full extent
ends before eleven CC bytes. A normal program member reproduces it without
assigning the native private EAX/ECX convention in source.

## Compiler evidence and exact field meanings

Initial full source generated238/67 and91/31. Real animation/loader/log sources
are support TUs, not synthetic implementations. Independent variable filename,
size and mode inputs keep LoadResourceFile from specializing its two null/zero
arguments away, recovering both native pushes and all95 bytes.

The native animation trace is RET-only and preserves EAX/ECX across the cached
loader call. An opaque declaration falsely forces volatile-register saves.
Adding the real body recovers the native index-in-ECX caller contract. A fixed
ignored format parameter is removed by LTCG and loses native format pushes;
the source now declares all supplied values variadic. The shared declaration
changes no physical layouts. AnimationTrace.cpp contains the actual empty body,
not a success substitute for a missing operation. Its simple diagnostic caller
passes independent message/value arguments and is never executed.

The parser's last66 differences came from retaining a remainder temporary:
extra EDX/EAX movement shifted the alignment/error region by two bytes. Expressing
the repeated signed relative remainder directly in the condition and increment
lets VC9 recover the native register use and internal alignment. The complete
238-byte contribution then has zero differences. No instruction bytes, padding,
fake ABI, manufactured aliases or Oracle exceptions were introduced.

Independent raw PE decoding establishes every external address field:

| Owner | Field offset | Type | Native target |
| --- | ---: | --- | --- |
| Extension | 52 | REL32 | AcquireAnimationDocument454360 |
| Extension | 58 | DIR32 | singleton pointer4A8D7C |
| Extension | 156 | DIR32 | load-error text493874 |
| Extension | 161 | DIR32 | runtime log prefix4A5940 |
| Extension | 166 | REL32 | AppendLog458A10 |
| File entry | 11,26,60 | DIR32 | shared filename4C9928 |
| File entry | 67 | REL32 | LoadResourceFile458400 |
| File entry | 75 | REL32 | base Install45D900 |

All internal branch targets land on instruction boundaries. The94-instruction
parser and41-instruction file entry have no owned tables or foreign interior
entries. The real derived vtable494090 independently binds Install, extension
and file slots. The three tiny owners have no address fields. Full public
primary-TU PDB contributions and current receipts are required for all five;
support-TU loader/log bodies receive no exact credit in this family.

## Wider boundary and origin review

Thirteen complete spans were inspected, ten newly reviewed. Seven omitted
entries are recovered: file getter40FFB0/5, base constructor40FFC0/38, four
singleton helpers410530/11,410540/9,410550/10,410560/21, and derived constructor
410D20/38. Explicit read-only spans and independent full decoding partition
the groups at their real returns and exclude CC separators. They have no
foreign interior references or tables. The four helpers store indexed DWORDs
at40/1C, read field68 or replace flag3C's low bit; stronger meanings and types
remain unknown.

Base construction really writes49404C before memset1098, erasing its own vptr.
Derived construction clears1098 first and publishes494090 afterward. The former
matches the unusual browser initialization previously observed. Do not rewrite
either into an assumed safe constructor. Later source models the full observed
1098 allocation, closes the derived38 bytes and retains the base38/3 and owner
gap; see the lifetime record. Original raw-region member identities remain open.

New host reviews cover initialization410BE0/318 and state reset410440/126. The
initializer imports an existing animation file, constructs/publishes the derived
program, calls slot8 and registers two24-byte chain nodes at priorities18/20 with
callbacks4111A0/B0. Its original unchecked failures, timer writes and return0
remain observations. The reset clears nine flag bits, zeroes eight214-byte records
with208 sentinel-1, clears two final flags and returns self. Full host identity,
construction and source remain open. Trace456AD0 is also newly reviewed. All ten
new origins stay indeterminate; custom formats and generic RET/constructor
bodies alone do not prove original authorship.

## Large interpreter investigation and replay scope

The real script trace body is now maintained at its observed one-byte owner.
Full Run changes6989/5958 to6993/5880 and still owns61 dispatch cells/89 selectors;
its complete target is7093 bytes. Enter-frame remains572/546, pop83/2 and
end-frame41/2. No encoding gaps are masked.

A private trace-arm ablation gives6541/5989 and alignment8 instead of64. Removing
only the double-valued variadic trace call gives6965/5960 and alignment8. These
diagnostics localize the64-byte alignment to that call; they omit native behavior
and are rejected. The native caller really passes a QWORD trace value and needs
no dynamic64 alignment. Adding a fixed format parameter to the real trace body
does not fix alignment. A controlled /fp:fast probe also retains64 and gives
6961/5940; it is rejected. Canonical /O2 /Ob1 /GL and real double arguments remain.
The remaining EBP/stack/register/rounding context is open, not papered over by
changing types or declaring alignment.

All84 selected units passed cold replay:
five new complete unique public primary-TU contributions/340 bytes/ten fields,
and79 affected prior owners. Current source/header/support/tool/producer and
PE/MAP/PDB receipts, full ownership, exhaustive replay and every byte pass.
Actual program/trace files froze before their cold builds. Selection comes from actual prior receipt
source/header hashes, including transitive includes:79 prior units are affected
by the AnimationDocument.hpp trace declaration and script/program changes,
plus five new units. No unrelated unit or full cohort is selected. The compared
infrastructure is unchanged; broad behavior and infrastructure suites are not
checkpoint prerequisites.

Later actual-caller and typed-trace work supersedes the interpreter alignment
blocker above: fixed120 and native dispatch layout are recovered in the current
7033/5779 context. See [SECONDARY_INTERPRETER_CODEGEN](SECONDARY_INTERPRETER_CODEGEN.md).

## Strict chain callbacks and initialization source work (2026-11-12)

Two original `InitializeSecondaryHost` (`0x410BE0`) chain callback pointers
were independently reviewed as **whole** native owners, not invented stubs.
`SecondaryMasterUpdateCallback` at `0x4111A0..0x4111A6` is exactly
`PUSH ECX; CALL 0x411120; RET`: it adapts the native ECX context to the
105-byte `UpdateSecondaryMasterActors` and retains that callee's return.
`SecondaryMasterDrawCallback` at `0x4111B0..0x4111B5` is exactly
`MOV EAX,1; RET`, acknowledging the draw chain. The native registrations
refer directly to each distinct callback start at `0x410C76/0x410CCB`,
with no foreign interior entries or tables; following `CC` bytes are not
part of either function.

The ordinary C++ in `src/SecondaryMasterCallbacks.cpp` naturally generates
both target functions with the pinned VC9 normal-COFF toolchain: **7/7 and
6/6 complete bytes**, one reviewed REL32 call field targeting `0x411120`
and no fields, respectively. Both have independent cold source/tool/object
receipts and **zero differences**; canonical units
`secondary-master-update-callback` and `secondary-master-draw-callback`.
The match is full-function COFF ownership, not an orphan instruction sequence
or a claim of original source provenance. Only these two small affected
units were cold replayed.

The separately scoped 318-byte native `InitializeSecondaryHost` main body
also has full natural source in `src/SecondaryMasterInitialization.cpp`;
its real program/new/chain construction paths preserve all seven calls.
Its current linked codegen is still non-exact and is tracked separately.

## Full 318-byte secondary host initialization boss (2026-11-12)

`src/SecondaryMasterInitialization.cpp` now reconstructs the complete reviewed
`InitializeSecondaryHost` (`0x410BE0..0x410D1D`) in ordinary C++.
It loads the active animation file from the existing secondary actor files,
constructs the native 0x1098-byte program-with-extensions and calls its
virtual script-loader, then allocates and initializes **two independent
0x24-byte chain nodes** for update and draw. These link into the actual
`AddToUpdateChain` and `AddToDrawChain` paths with priorities **18** and
**20**, using now-exact native callbacks at `0x4111A0` and `0x4111B0`.
It initializes the embedded stage timer fields at +0x50 through +0x60
with the native `SetFrame(-1)` path and returns zero. The allocation-failure
fall-throughs remain native unchecked paths; no unwarranted guards or
nominal class claims were introduced.

Three scoped pinned VC9 `/GL` PE/MAP/PDB context links were compared to the
entire **318-byte / 102-instruction / seven-call** original. A standalone
producer yields **306/318 bytes, 96 instructions, 7/7 calls**; linking the
real script-program constructors and both chain producers yields **242/318**
and **248/318** variants (still seven calls), because the compiler emits
separate program construction rather than the target's inlined sequence.
Six further natural C++ variations for program result, chain-field ordering,
flag expressions, and timer initialization retain or slightly alter the
same non-exact layout. No forced ABI, byte edits, or suppressed owner code
were accepted. The complete natural source is source-present **but not
strict exact**, with no credit in `config/matches.csv`.

The bounded reproducible diagnostic is
`.analysis/probe-secondary-master-initialization-20261112.py`, and the
variant study is `.analysis/try-secondary-master-init-20261112.py`.
Their public independent caller is
`probes/secondary-master-initialization-context.cpp`. All probes compile
only this primary source owner and direct producers, never a full cohort.

## Full 318-byte master initialization boss (2026-11-12)

`src/SecondaryMasterInitialization.cpp/.hpp` now maintains the **entire**
`InitializeSecondaryHost` at `0x410BE0..0x410D1D` (**318 native bytes,
102 decoded instructions, seven calls, one RET4**, no embedded table or
foreign interior entry). The source saves the actual HUD/ANM file pointer
to master+0x40, allocates the real `SecondaryProgramWithExtensions` (0x1098
bytes), stores it at master+0x64 and invokes the third virtual file-loading
slot. It then creates **two distinct 0x24-byte chain elements** with native
constructor flag/node initialization and independently exact callbacks
`0x4111A0` and `0x4111B0`, registers update at **priority 18** and draw at
**priority 20**, and installs the update/draw fields at master+8/+C. The
embedded stage timer at master+0x50 is reset to frame -1, including the
initialization branch guarded by timer flag 1. Native null allocation and
unconditional-use behavior is retained rather than inventing extra recovery.

A dedicated physical 0x24-byte chain element constructor preserves the
native flag-clear, null callback/node initialization and owner pointer
publication, instead of using a zero-fill POD value-init that would change
the original uninitialized flag semantics. The original derived script
program constructor, virtual file loader, two exact 147-byte chain-list
registration owners and the two exact callback leaves are compiled in
bounded **VC9 RTM `/GL`** PE/MAP/PDB contexts. The independent producer
family is 348/318 bytes, while the real program and chain producer context
produces a **290/318-byte complete code contribution, 95/102 instructions,
all seven native calls**, ten candidate code fields and 230 positional
nonfield byte differences. The parent remains **non-exact** because of the
program constructor inlining, chain construction and private register
allocation. It is not cropped or falsely marked byte-exact.

Scoped reproduction: `.analysis/probe-secondary-master-initialization-20261112.py`,
`.analysis/try-secondary-master-init-constructors-20261112.py` with logs,
plus public dynamic caller `probes/secondary-master-initialization-context.cpp`.
The unchanged exact callbacks retain their own cold normal-COFF receipts;
no unrelated full cold replay was performed.

## Three strict secondary-master accessors (2026-11-12)

Three previously source-open complete owners adjacent to the actor master
lifecycle now have exact C++ in `src/SecondaryMasterAccessors.cpp`:

- `ReadSecondaryHost068` (`0x410540`, **9/9 bytes**) returns the first actor
  list node at master+0x68, preserving the original `MOV EAX,[master];
  MOV EAX,[EAX+68]; RET`.
- `StoreSecondaryHostSlot01C` (`0x410550`, **10/10 bytes**) writes an
  actor pointer to the unguarded indexed master slot at +0x1C. Its natural
  fastcall C++ produces native ECX payload/EDX index and plain `RET`.
- `SetSecondaryHostFlag03C` (`0x410560`, **21/21 bytes**) updates only bit
  zero at master+0x3C, preserves all higher bits and uses native `RET4`.
  Writing the natural equivalent as `(flags & ~1u) | (bit & 1u)` restores
  the exact memory XOR/AND/XOR codegen. Direct XOR-assign and `volatile`
  alternatives produced 24–35 bytes instead; no artificial instructions
  or fake register ABI were adopted.

All three targets have independently reviewed complete original boundaries
with no foreign interior references or embedded tables. The pinned VC9
normal-COFF functions are unique complete single-function COMDAT owners;
each has exactly **one independently reviewed DIR32** at +1 targeting
`g_SecondaryMaster = 0x4A8D7C`, and **zero byte differences** after full
relocation replay and source/tool/object receipt checks. Canonical units:
`read-secondary-host-068`, `store-secondary-host-slot-01c`, and
`set-secondary-host-flag-03c`. Scoped cold replay evidence is in
`.analysis/secondary-master-accessors-strict-20261112.log`. All three
builds were selected by changed unit, not by full cold replay.
