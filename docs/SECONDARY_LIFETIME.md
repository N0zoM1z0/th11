# Secondary lifetime, counters and actor ownership

Fourteen new complete source mappings cover355 target bytes. Twelve complete
unique public primary-TU owners pass246 exact bytes/eight address fields. The
base program constructor and generated deleting entry remain non-exact. Total
state at this lifetime checkpoint was2,097 candidates,385 reviewed boundaries,291 mappings and168 exact /
22,118 bytes. Thirty-six new origin reviews add34 indeterminate and two verified
compiler exclusions; origins are90 authored,283 indeterminate,10 excluded and
1,714 pending.

Later full actor lifecycle/pair source, refined member construction and six
additional exact owners are recorded in [SECONDARY_ACTOR](SECONDARY_ACTOR.md).
Its current source/gap and progress counts supersede the source-open lifecycle
status below.

| Owner | Address | Full bytes | Current result |
| --- | --- | ---: | --- |
| SecondaryProgramBaseConstructor | 40FFC0 | 38 | Source;38/3, support-TU emission |
| SecondaryProgramBaseDestructor | 40FFF0 | 36 | Exact;2 fields |
| SecondaryProgramDerivedConstructor | 410D20 | 38 | Exact;2 fields |
| SecondaryManagerDestructor | 40FF10 | 47 | Exact;3 fields |
| SecondaryManagerDeletingDestructor | 40FF40 | 71 | Source-generated30/62; public aliases, not exact |
| SecondaryDefaultExecute | 40FD40 | 3 | Exact;0 fields |
| SecondaryDefaultInt | 40FE30 | 5 | Exact;0 fields |
| SecondaryDefaultIntPointer | 40FE40 | 5 | Exact;0 fields |
| SecondaryDefaultFloat | 40FE50 | 5 | Exact;0 fields |
| SecondaryDefaultFloatPointer | 40FE60 | 5 | Exact;0 fields |
| SetSecondaryCounterScaled | 410060 | 13 | Exact;0 fields |
| SetSecondaryCounterFlag | 410070 | 16 | Exact;0 fields |
| SubtractSecondaryCounter | 4100E0 | 61 | Exact;0 fields |
| UpdateSecondaryActor | 411600 | 12 | Exact;1 field |

## Real construction and destruction

Both program constructors independently clear1098 bytes and first initialize
DWORD1090/1094. The physical view now includes the actual raw4096-byte region
90..108F and those two DWORDs; known fields through8C are unchanged. Contents,
original member identities and ownership of the raw region remain unknown.
This storage bound comes from native allocation/clear/tail writes, not padding
introduced to match instructions. Static assertions check size1098 and tail1090.

Base construction genuinely writes49404C then erases it with memset(this,0,...).
A normal inline base constructor preserves that behavior. The empty derived
constructor naturally inlines the base initialization, removes the overwritten
base-vptr store and publishes494090 after clearing. All38 native bytes agree.
No vptr instruction is supplied by hand. The emitted base38 still schedules
PUSH self before its vptr write, unlike native, and belongs to a support TU in
the current context. Its three differences and ownership gap grant no credit.

The program destructor is nonvirtual: the actual program table has Install at0
and extension at4, with derived LoadFile at8. Its36-byte body resets49404C,
frees a non-null subroutine table and clears that pointer. It does not free the
loaded file buffers. The newly reviewed374-byte master cleanup frees all32 file
slots, independent of fileCount, before table cleanup/program deletion. Do not
move file freeing into the base destructor or add a destructor virtual slot.

The manager is different. Its real494058 vtable has five implemented defaults
followed by the deleting destructor at slot14. Derived494074 has corresponding
host overrides and deleting entry410420. The earlier pure-virtual-only header
was an incomplete interface view. It now declares the actual non-pure methods
and sixth virtual destructor without changing object offsets or size103C.
The five default bodies are observed native zero/null/FLDZ returns, not invented
fallbacks for missing derived behavior. Each has its own unique primary owner.

Ordinary manager destruction47 resets494058, snapshots each next auxiliary node,
deletes its VM and node, then continues. It does not clear the head, delete the
embedded main VM or free program data. Natural C++ reproduces the complete body.
The compiler-generated deleting entry instead emits30 bytes calling ordinary47,
versus native71 with the loop inlined. Its scalar/vector ??_G/??_E publics alias
one candidate address. It is source-backed but remains outside strict exact
acceptance. No custom deleting wrapper or alias exemption is introduced.

## Counter prefix and ordinary actor adapter

The counter view describes observed fields0,C,10,14 only, not a recovered full
original type. SetScaled stores wrapped7*amount atC. SetFlag replaces only bit0
of flags14 with the low input bit; treating input as C++ bool would change input2.
Subtract uses flag0 to choose scaled decrement and
`(scaled - 7*baseline)/7 + baseline`, or direct value decrement. Signed division
truncates toward zero; explicit32-bit wrapping preserves native overflow behavior.
No clamps, guards or inferred timer semantics are added. All90 bytes agree.

Actor update411600 adds the actual base-manager size103C and calls the full state
update411750. Independent RET decoding ends at41160B: the first13-byte explicit
span included an adjacent CC, so the real owner is12 bytes, established before
the compiler comparison. This is an ordinary adapter, not an EH funclet. Source
declares the real, still-source-open state boundary and does not supply a stub.

Independent exhaustive fields are program destructor DIR32/vptr at8 and REL32/
free at18; manager destructor vptr9 and operator-delete calls25/31; derived
constructor memset22 and derived vptr31; actor adapter REL32 at7 to411750.
The targets are49404C/45FC3B,494058/45FD49,46A720/494090 and411750 respectively.
Every other new exact owner has zero address fields. Entire primary-TU PDB
contributions, current receipts and every byte are checked.

## Complete large-owner and EH review

Full actor constructor699 establishes base103C plus appended state163C, total
2678. Its699 bytes initialize the embedded VM/manager, publish494074, reset/clear
state, set vectors/timers/nodes/sentinels and perform script lookup. It remains
source-open. Main actor destruction384 covers unlinking, master slot release,
ten animation-id detach paths, actor references, mesh deletion and base thread
cleanup. EH-bearing lifecycle acceptance remains open; its main body is not
cropped into a non-EH exact claim.

Actor-state update411750..4127D5 owns4230 bytes, including6+3 internal alignment
bytes omitted from Ghidra's sparse4221-byte body. Full Ghidra export and an
independent1108-instruction decode cover all five returns; every direct branch
lands on an instruction boundary. No embedded table or foreign interior entry
occurs, and ten CC bytes separate the next owner. The initial250-instruction
review was insufficient and is supplemented by the full1800-limit export.

Its complete flow snapshots13 DWORDs of motion state, applies interpolation,
position/boundary logic, script stepping and hooks, damage/scaled-counter logic,
eight animation IDs, mesh vertex deformation, timers and frame/status updates.
The processed4000 flag is set by the state update and cleared by the master
scheduler on successful stepping. Returns include0,1 and-1. No source or stronger
original-authorship claim is supplied for this4230-byte owner.

Actual GS/EH handler48A738/27 checks the registration cookie, selects
FuncInfo49FB78 and jumps to CxxFrameHandler3. The info magic19930522 and one-state
unwind map49FB70 point to48A730/8, which loads the saved receiver and tail-jumps
base destructor40FF10. The PE SafeSEH table49E960 has56 entries; its49E9F4 entry
independently identifies handler RVA8A738. These two mechanical compiler entries
are exclusions, with no authored/source/exact credit. The two references inside
the combined35-byte review enter the second owner's start, not an owner interior.
The adjacent ordinary adapter411600 is not the unwind action.

Thirty-eight full spans were inspected:36 new reviews and two constructor
rechecks. Fifteen missing entries include program destructor36, twelve primitive
accessors/counter helpers, actor adapter12 and handler27. Ten repeated DWORD
accessors remain source-open; identical instructions do not establish one class.
Other new complete reviews include position133, effect/reset58, unlink82,
master creation97/deletion29/cleanup374/scheduler105, actor position309 and
the ordinary/default/deleting entries. All34 new non-compiler origins remain
indeterminate. No private database mutation or inventory regeneration occurred.

## Compiler experiments and current proof

Full interpreter7093 remains6993/5880. Splitting real/integer trace calls gives
6993/5877 but retains64-byte alignment. An implicit float expression gives
7001/5937 and risks extra float rounding; separate per-format arms give7141/5984
and still64. None is adopted. Canonical trace arguments, source/profile and full
61-cell/89-selector shape remain; no alignment/type trick closes the large owner.

Cold replay passed12 new and16 directly affected prior units across five contexts. Actual receipt dependencies select
those prior units; no unrelated unit or full cohort replay. All246 new bytes
and eight independently bound fields pass complete unique public primary-TU
ownership, current source/header/support/tool/producer/PE/MAP/PDB receipts and
zero full-byte differences. Source froze before cold builds. Build/Oracle tools
are unchanged; no broad behavior or infrastructure suite ran.

Later actual-caller and typed-trace work supersedes the interpreter alignment
blocker above: fixed120 and native dispatch layout are recovered in the current
7033/5779 context. See [SECONDARY_INTERPRETER_CODEGEN](SECONDARY_INTERPRETER_CODEGEN.md).

## Full 374-byte master destruction boss (2026-11-12)

`src/SecondaryMasterDestruction.cpp` now reconstructs the whole reviewed
`DestroySecondaryMaster` function at `0x410E90..0x411005` (**374 native
bytes, 108 decoded instructions, 12 calls, two returns**, no embedded
jump table or foreign interior entry). The first call is now the independent **201/201 exact** maintained
precleanup owner at 40D0C0; its nominal original class is still unresolved. The master pointer
receives ordinary source-level C++ parameters; native compiler privately
passes it through EAX, not a hard-coded caller ABI shim.

The function unregisters the two independent update/draw chain pointers
at master `+8/+C`, each under its own conditional runtime critical
section 0, using the real, already exact `UnregisterChainElement` 457080.
It then walks all **32** installed script-file pointers in the master's
program table (offsets `0x0C..0x88`), freeing each nonnull allocation
without a new program-null guard. The ordinary program destructor frees
its subroutine table before the program allocation is released. The master
program pointer at `+0x64` is then cleared. Only when resource flag bits
`0x09` are both absent does the original check renderer document slots
**8 through 11**: each live file is passed to the independently exact
243-byte `DestroyAnimationDocument` at 4549E0, deleted and its indexed
pointer cleared. Both exit paths clear the original singleton. This is
not a blanket or reordered destruction of all renderer slots.

A scoped pinned VC9 `/GL` diagnostic links the real chain removal,
script-program destructor, and animation-document implementations. The
first natural indexed-array source emitted **321/374 bytes, 95/108
instructions, all 12 calls and two returns**, with 191 positional
nonfield mismatches. Six independent C++ loop-shape probes tested the
native byte-offset traversal for `program+0x0C..0x88`, the four document
indices, and a fresh renderer slot read each iteration. Keeping the two
natural byte-offset loops improves the retained source to **340/374
bytes, 99/108 instructions, all 12 calls, two returns, 29 linked fields,
97 positional nonfield differences**. The exact original 374-byte owner
is still open: VC9's program destructor inlining, index bounds codegen and
private chain-register allocation differ. No fake vtable, padding or
register declarations were inserted to force an apparent match.

The focused recipe is `.analysis/probe-secondary-master-destruction-20261112.py`,
with variants/results under `.analysis/try-secondary-master-loops-20261112.py`
and `.analysis/secondary-master-loops-20261112.log`; the public live caller is
`probes/secondary-master-destruction-context.cpp`. This checkpoint grants
**374 target bytes of complete source coverage but zero new exact bytes**.
No unrelated full cold replay was executed.

## Strict 201-byte master precleanup dependency (2026-11-12)

`PrepareSecondaryMasterDestruction` at `0x40D0C0..0x40D188`, the real
precleanup called by the 374-byte master teardown, now has complete natural
C++ in `src/SecondaryMasterPrecleanup.cpp`. The independently reviewed TH11
1.00a original has **201 bytes, 68 instructions, one virtual deleting call,
one RET4**, no embedded table, and no foreign interior entry. The operation
scans renderer document slots **8–11** and both separate linked VM lists:
for each VM referring to the current file, it sets flag `0x04000000` at
VM+0x404 without removing the node. It then follows the master actor chain
from +0x68, snapshots each next link before invoking the actor's virtual
scalar-deleting destructor, clears fields +0x74 and +0x1C, and removes flag
bit 2 from the independently nullable master update/draw chains at +8/+C.
No invented owner deletion or null check is added.

Initial VC9 `/GL` compiled **200/201** bytes with 67 instructions because
its `for` loops loaded VM data before the next node; expressing native-style
`next` snapshots and advancing at the end of each body restored the first
`0x9B` bytes and **201/201 bytes, 68/68 instructions**. The remaining 14
positional differences came from the compiler hoisting a chain pointer load
ahead of two state stores. Using the existing typed `self->Slot(0) = 0`
source expression restores the native exact ordering, instead of volatile
aliases, artificial ABIs or code-byte patches. The final strict VC9 RTM
`/GL` unique primary-TU **PE/MAP/PDB** replay matches **all 201 bytes**,
including the sole independently reviewed `g_RenderBatch` DIR32 field at
+0x06 targeting `0x4C3268`, with current tool/source/producer receipts and
zero differences. Canonical unit: `prepare-secondary-master-destruction`;
receipt/replay `.analysis/master-precleanup-strict-20261112.log`.

The bounded exploration is reproducible via
`.analysis/probe-secondary-master-precleanup-20261112.py`,
`.analysis/try-master-precleanup-order-20261112.py`,
`.analysis/try-master-precleanup-tail-20261112.py` and matching logs. The
public independent caller is
`probes/secondary-master-precleanup-context.cpp`. The directly linked
374-byte master is still **340/374 source-only, non-exact**: closing its
201-byte callee does not magically grant exactness to the caller. No full
cold replay was run.
