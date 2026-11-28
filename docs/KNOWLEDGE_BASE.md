# TH11 working knowledge

Use [RE_HANDOFF](RE_HANDOFF.md) for the next action and [PROGRESS](PROGRESS.md)
for generated ledger counts. Topic notes below retain the target findings,
compiler observations and useful failed approaches.

## Evidence and recovery

- Only original Japanese TH11v1.00a, SHA256
  `2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`.
- TH11-local PE/Rich evidence selects VC9 RTM21022/SDK6.0A. TH10/VC7.1 bytes or
  compiler choices do not establish TH11 facts. Original TU partition is open.
- Use direct `scripts/ghidra.py`; preserve full code, layout, tables and late
  blocks. Ghidra bounding spans and sparse bodies are not reviewed extents.
- Source presence, diagnostic equality, canonical exactness and runtime/product
  closure are separate. A support function replaying zero adds no exact credit.
- /GL stays with established owners. Public primary-TU acceptance requires cold
  receipts, full PE/MAP/PDB contribution identity, exhaustive fields and zero
  complete differences. Do not encode private registers or add inert storage.
- Rebuild affected exact contexts through `config/match-units.toml`. Cache
  retention is described in [ARTIFACTS](ARTIFACTS.md).
  Obsolete private experiment outputs were pruned on 2026-11-26. Historical
  paths below can identify the experiment without naming an available cache;
  use the retained findings/drivers and rebuild the relevant context if needed.
- **Updated 2026-11-17:** linked field replay authenticates
  `eh_static = true` for the owner-named compiler CIL EH handler.
  The spell-stage factory (206 B), spell-stage destructor (317 B),
  and HUD constructor/factory (208/150 B) have separate exact
  function-body proofs. The EH handler code is independently owned
  and is **not** made exact by an accepted address field. Historical
  notes below saying EH acceptance was still unimplemented describe
  earlier tool revisions; see [LINKED_ORACLE](LINKED_ORACLE.md).

## Relocation targets must match the named symbol (2026-11-22)

Both comparators bind the candidate side of a relocation through the MAP
symbol, but take the original side from the manifest's `target`. A source
call to the wrong function therefore passes if its target is set to the
original callee. This happened once: `DrawSecondaryProgramBrowser`
(0x438E70) called `AsciiManagerPrint` (0x4015A0) nine times while every native
call goes to `AsciiManagerPrintSmall` (0x401600). A second, harmless case
reused the `InputAcquireHook` symbol (0x459BC0) for the distinct one-byte RET at
0x41F6E0. Both sources were corrected and both owner units pass strict cold
replay with the native callee symbols.

`validate-tracking.py` now rejects the pattern for exact units: a symbol owned
by an exact unit must target that unit's address, one symbol may name only one
target, and an exact row's size must equal its `functions.csv` extent.
Symbols that resolve to diagnostic link anchors still rely on manual target
review, so check the callee's identity in the target, not only its address.

## Lessons from TH09 that apply to TH11 (2026-11-22)

TH09 (VC7.1 13.10.3077, no `/GL`) reached 1,036 exact functions mostly by
changing compile context rather than flags. The applicable lessons, with the
TH09 commit that demonstrated each:

- Hand-merged tails in one switch handler can move registers in other
  handlers of the same interpreter. Plain per-handler statements let the
  compiler do the merging (RunEcl, `5fc16212`). This is the first thing to
  test for `ExecuteAnimationVm`'s EBX-versus-EDI VM pointer: isolate handler
  groups in scratch to find which ones decide the allocation.
- A visible callee body changes caller scheduling and register choice far
  from the call, even when it is not inlined (`352d8577`, `e2c63478`,
  `eeab4cf1`). Under TH11's `/GL`, "visible" means compiled as `/GL` code in
  the same link. Opaque diagnostic anchors behave like declaration-only
  callees.
- Block-scoped locals, not function-wide temporaries, restored a native frame
  size and input registers (`c309298a`). Relevant to the 0xCC interpreter
  frame.
- Real headers and types beat locally invented helper views (`81b307f0`,
  `5be571a8`).
- A destructor's initial EH state of 1 rather than 0 usually means one more
  member or base with a non-trivial destructor. Decode the native FuncInfo and
  unwind map before editing the body (TH09 EH comparison, `33eb1939`).
  Confirmed for TH11 VC9: `~SecondaryEclDebugOwner` (0x40D320) became exact
  once the +0x2C8 `StageEmbeddedVm` from unwind state 1 was declared; its
  inline destructor was already visible as the body's final free/clear of
  +0x6C8, so that explicit cleanup had to go
  ([SECONDARY_DEBUG_LIFECYCLE](SECONDARY_DEBUG_LIFECYCLE.md)).

VC7.1 backend addresses, its allocator heuristics and `/Gr` defaults do not
transfer. TH09 traced `c2.dll` in a debugger to explain ties; for TH11's
LTCG, code generation runs in the linker-loaded `c2.dll`, which would need a
separate investigation.

## The target names its source files and classes (2026-11-28)

**Source-file markers give the TU link order.** `.rdata` keeps 110 strings of
the form `.\src\game\laser.cpp:115 LaserInf` (allocation/assert tags whose
code use compiled away). No code references them, but each stays in its own
TU's `.rdata` contribution, so their order is the link order: `game/` files
alphabetically (ascii, background, bomb, bomb_ma, bomb_rc, bullet, card,
eclview, effect, ending, enemy, fps, front, game, hint, item, laser, loading,
mother, pause, player, replay, score, smollscore, spttest, title), then
`pack/` (_ArcMngr, File, LzssUtil, ResFile), `core/` (main, screen, sound,
sprtanim, sprtfunc, sprtlib, zunlib, zwave) and `script/` (sptcmd, sptlib).
Files without tags (stage, ecl, ...) are absent from this list. The marker
spans run 0x4938A0..0x497E48; the pooled `__real@` float constants start at
0x497E48 and the CRT data follows near 0x498500.

`.text` follows the same order. Attributing each function's direct `.rdata`
operands to the marker spans gives a mostly monotone partition: ascii from
0x401000, card around 0x40C4A0, eclview 0x40D600, enemy 0x40FF10..0x418460,
fps 0x419EA0, game 0x420390, hint 0x421BD0, laser 0x424830, loading
0x4294B0, mother 0x429650, pause 0x42BFE0, replay 0x436330, title around
0x43AAD0, then pack 0x4415C0.., core main 0x445510, sound 0x449140,
sprtlib 0x454360, zunlib 0x4571A0, zwave 0x45A450. Span edges are approximate:
a TU's other string literals can precede its marker (`bullet.anm` at 0x493ABC
lies between the bomb_rc and bullet markers and is used by bullet, laser and
stage code), and pooled literals sit with the first TU that defines them.
Single-function outliers usually reference another TU's data, so treat one
vote as weak and a run as strong. This is TU evidence for grouping new source files, not an
exact claim; the reconstruction's own file split predates it.
Tools: `.analysis/claude-srcfile-map.py` (string list),
`.analysis/claude-tu-partition.py [--functions]`; outputs in
`.analysis/claude-tu-partition*.txt` and `claude-srcfile-strings.txt`.

**RTTI is present for virtual classes.** `.analysis/claude-rtti.py --slots`
lists every complete object locator with its vtable and slots
(`.analysis/claude-rtti.txt`). Game classes: `AsciiInf` and `TitleInf`
(both derive from `TaskInf`), `SptResourceInf`, `SptInf`, `EnemyInf : SptInf`,
`EclResourceInf : SptResourceInf`, `LaserDataInf` with `LaserLineInf` and
`LaserInfiniteInf`, `Pbg::IFile`, `Pbg::File`, `Pbg::MemFile`,
`Pbg::ResFile`, `ThreadInf`, `D3DThreadInf`, `CSound` and
`CStreamingSound`. Use these names for new source. Existing "Secondary*" views
often describe the same objects: `g_SecondaryClearManager` (0x4A8E94) is the
laser manager and `SecondaryClearObjectView` is `LaserDataInf`; see
[LASER](LASER.md).

## Memory xor/and/xor stores mean bit fields (2026-11-28)

The memory `xor/and/xor` store (and `shl 31; and 0x7fffffff; or` for the top
bit) identifies a bit-field assignment. A masked `flags ^= (v ^ flags) & mask`
gives a load/compute/store sequence instead. In `ExecuteAnimationVm`, bit-field
setters fix 12 case bodies whichever way the bit field is declared. The
whole-owner alignment still moves by about 160 instructions between
declarations, because x87 operand order in untouched blocks flips. Compare per
case body (`.analysis/claude-case-bodies.py`) as well as whole-owner numbers
before deciding. Details in ANIMATION_EXECUTION.md.

## Compare switch body order before anything else (2026-11-27)

VC9 RTM lays switch case bodies out in lexical order, so a source whose cases
are sorted by number collapses whole-function alignment when the original was
not. `ExecuteSecondaryActorInstruction` went from 1,184 to 3,350 of 4,003
register-normalized instructions by reordering its 135 bodies alone.
`.analysis/claude-case-order.py BUILD SYMBOL ADDR CODE_SIZE` decodes both jump
tables (with or without a byte index table) and prints the body order;
`.analysis/claude-case-cmp.sh` reports how many positions agree. Exception
seen in `ExecuteAnimationVm`: a body that returns from inside the interpreter
loop (`case -1/1/2`) is in lexical position in native but placed after all
other cases by our build.

## Address-exposed game state changes /GL alias decisions (2026-11-27)

Native 0x4A56E0..0x4A576F is one 0x90-byte global object: its constructor at
0x420E20 does `memset(this, 0, 0x90)`, sets +0x40 and +0x6C to 1 and returns
`this`, and several functions call members with `this = 0x4A56E0` (0x419790,
0x41F510, 0x420A70). Score, power, lives, bombs, character, stage variant,
replay flags and more are fields of it (several reconstruction names alias one
address). Because its address escapes, VC9 must assume any pointer store can
change a field and reloads it afterwards.

The reconstruction names these fields as separate extern globals that no /GL
code takes the address of, so whole-program alias analysis lets VC9 keep them
in registers across pointer stores. Exposing the field addresses in the link
context (`probes/secondary-game-state-context.cpp`, a `dllexport` root that
only stores the addresses) moved `InitializeSecondaryReplay` from 1,327 bytes /
327 aligned to 1,370 / 394 of 406 (native 1,380), with no change to its source.
Functions that only read these fields between calls (the HUD animation
initializer) are unaffected.

Use this probe in frontier contexts whose owners interleave pointer stores
with game-state reads; `g_MainRuntime` aliases such as `g_ReplayModeFlags`
(+0x564) belong in it too. Modeling the object as a real struct would be more
faithful but touches every user and many exact units; the probe records the
same program property without changing owner code.

## x87 commutative operand order is a scheduling outcome (2026-11-27)

Several near-exact owners differ only in which memory operand of a float add is
loaded first (`StepAnimationFloatPairTween` mode 17, `ResetStageMesh`, the
`CullStageObject` eye/center sums). Swapping the source operands almost never
changes the result. In VC9 RTM's `c2.dll` the binary op is first emitted as two
loads plus a popping op; the list scheduler (`SchSchedGraph`, which consults
`LfFPRegsPressure`) orders the two independent loads, x87 stack assignment
follows (`FpAssignStackLevelAndFxch`), and the `try_fld_fopp_peep` /
`try_rev_op_peep2` peepholes fold the later load into `fadd mem`. The integer
commutation in `LoFinalLower` (via `LoFinalTupIsCommutative`) and
`LfFPCanonicalForm` do not decide memory-operand order. The choice therefore
depends on the whole block's dependence graph and tie-breaks, which explains
why it flips between components and when nearby code changes: rewriting
`CullStageObject`'s sums with D3DX-style member operators made the native
asymmetric eye.y/eye.z order appear by itself, but flipped center.x.
Treat these bytes as evidence about surrounding code shape, not about the
operand spelling.

## Barriers and volatiles often stand in for value-object code (2026-11-27)

Several earlier fixes forced native store/reload order with
`_ReadWriteBarrier()` or `volatile`. In three owners the real cause was the
shape of ZUN's value code: the next-ID counter is an ID object (below), the
stage fog bytes are fields of the same object (not a `char *`), and vector
differences are by-value operators that return temporaries. When a frontier
leans on a barrier or volatile, first look for a value-type or field-access
form; see [STAGE_SCRIPT](STAGE_SCRIPT.md). Counter-example: replacing the
program browser's volatile count read with TH10's `Select(value)` form let VC9
constant-fold the count and was worse.

## The renderer next-ID counter is an ID object (2026-11-27)

Every native scene-VM publisher (0x4556E0, 0x455760, 0x4557E0, 0x455860) and
the inline copies in the HUD owners (0x41A96B, 0x41BD84 and two more) encode
the ID wrap as `add [nextId], 1; cmp [nextId], 0; jne; add [nextId], 1`. With
`unsigned int nextId; ++nextId; if (!nextId) ++nextId;` VC9 RTM folds it into
`add; jne; mov [nextId], 1` in every link context we have built. Declaring the
counter as an `AnimationVmId` object with an inline prefix `operator++` and
`operator==(unsigned)` (TH10's `AnmVmIdView` shape, here free functions in
`src/AnimationVmIdCounter.hpp`) reproduces the native sequence exactly. All
four 118/123-byte publishers became canonical exact with their single
`g_RenderBatch` field.

Failed spellings, measured on `PublishSceneAnimationVm` in its own context
(`.analysis/claude-nextid/`): a `volatile` counter adds a frame push and
reorders the list update (124 bytes); a separate `unsigned *` alias still
folds the second increment. A member postfix `operator++(int)` works as well
as the prefix form.

The HUD initializer previously stored the renderer tail through a local
`volatile` pointer to recover store order. With the ID counter, a plain tail
store already keeps the native order, and the `volatile` store was what kept
VC9 from sharing one stack slot among the four hidden-return ID temporaries
(frame 0x14 -> 8; native 0xC).

## Interpreter layout: preserve real helper calls (2026-11-26)

TH11 VC9 RTM `/GL` confirms TH10's warning about manually expanded helpers.
The UV update in `ExecuteAnimationVm` calls the same `0x452010` quad-mode
helper as opcode 98. Expanding that call into a switch added two calls and a
16-byte jump table to the interpreter, concealing two missing float-reader
call sites elsewhere in its total call count. Restoring the call removes
144 owned bytes. Compare the complete owner, but count instructions only in
its code region; decoding trailing dispatch data distorted earlier metrics.

Linking the three real geometry TUs changes the helper from 100 to 108 bytes
and changes its private argument registers without changing interpreter size.
The helper remains non-exact. This is concrete evidence to inspect the real
callee graph before attempting local register or source-order adjustments.
See [ANIMATION_EXECUTION](ANIMATION_EXECUTION.md#current-status-2026-11-26)
for native call sites, result-buffer offsets and retained failed controls.

## VC9 stack packing traced in the actual linker backend (2026-11-26)

The pinned VC9 RTM `c2.dll` uses the same stack-color ordering principle found
in TH10: compare the color class, then the signed integer ratio
`weight * 1000 / size`. This was checked in the VC9 binary itself, not inferred
from VC7.1 addresses. Relevant RVAs are `StackPacking` at `0x1067D0`,
`StkColorCmp` at `0x1096D0` and `QSortColors` at `0x109750`. Runtime DLL bases
vary under Wine, so resolve the mapped `.text` before placing breakpoints.

The compiler component SHA-256 is
`215c21b66b7769e65fff3ca6a9af498c5a896695abdf69e2412f3c2cacd73c5b`.
The downloaded public PDB has the compiler's GUID
`53BF9F1F-671F-4485-AC1E-CA8A9B7FC280`; its stripped age is 12 while the DLL
records 11. Its four exported-function RVAs and section headers agree with
the DLL, and the named routines were checked against local machine code.
Use those names for compiler analysis, not as an Oracle identity receipt.

A read-only linker trace of the pre-child-local interpreter records 26 stack
colors. Four single-use, four-byte temporaries have weight 1 and ratio 250;
they occupy offsets -4, -8, -12 and -16 before the shared 48-byte quad group
at -64. That quad group combines two source arrays, with total weight 14
and ratio 291. The scale pair's eight-byte return shares a color with a
source target pair, with total weight 4 and ratio 500. The four word slots
come from copying factory return values into `scratch.childId`; the native
child paths pass the returned ID object directly to `ClearMissingAnimationId`.
This identifies a source-level return-object issue before changing stack-slot
or declaration order.

The group records are `0x1C` bytes. Unlike the TH10 trace's 32-bit sparse
vectors, this VC9 backend uses 64-bit nodes with base, low/high bit words and
a next pointer. Do not reuse TH10's vector decoder unchanged. Read groups
before and after sorting; they have already been recycled when
`StackPacking` returns.

The traced link and an ordinary relink of the same inputs/output paths have
identical 31,814-byte `.text` sections. Local evidence is in
`.analysis/compiler-vc9/execution-stack.baseline.txt`,
`.analysis/compiler-vc9/trace-stack-baseline-run.log` and
`.analysis/compiler-vc9/baseline.linked.receipt.json`. Reproduce the local
trace with `scripts/repo-python .analysis/compiler-vc9/trace-stack.py` after
building the intended focused context. The trace changes no IR, weights,
register assignments or generated instruction bytes.

## A conditional return preserves both opcode 97 readers (2026-11-26)

Changing only `ReadF` from two early returns to one conditional return restores
78 float reads and all 262 calls in the real-timer graph, with full-width script
IDs and the existing child-publication order. A TH10-style conditional macro
also restores them; the ordinary inline function suffices. A control using the same source and object paths keeps the exact reader symbol
and returns to 76 reads when the
old body is restored. The source path's anonymous-namespace hash is unchanged.

The maintained cold candidate owns 12,096 bytes: 11,680 code/alignment plus
416 table bytes, with 3,223 instructions and frame `0xE4`. Every direct call
resolves, and the complete 259-field REL32 destination inventory and
multiplicities agree with native. The complete inventory is 409 fields versus
415: the zero-vector components still have two reads each versus native four.
Conditional branches are 258/259 and jumps 142/147. VM/cursor remain EBX/EDI.
This restores a specific codegen property; the complete owner remains non-exact.

Both forms enter late flow with 78 reads and the child copy before the first
float load/test. `FgDoLateFlowOpts` leaves 76 reads for the old form and 78
for the conditional form. In the latter, the copy is still early at pre-encode
entry. The final `Schedule` call at RVA `0x125581` moves that same EAX→ESI
tuple after FLD/test, observed at its return `0x125586`. The earlier pointer-
placement difference is therefore a later scheduling result, not a sufficient
explanation of the merge choice. Both branch tags encode equality through
`TupCCEncode`; the raw tags cannot index the machine `CondCodeName` array
directly.

All paired debugger/ordinary text controls match: 33,446 bytes for the old
form, 33,542 for the macro and 33,590 for the conditional function. Four math
results retain immediate FSTP32 materialization, and all four strip windows
retain both three-component rounding stages. The complete 540-byte reader
and 104-byte integer destination have identical nonfield bytes and field
positions across the old/new control.

Evidence: `.analysis/gpt-6.1-sol-execution-readf-{macro,conditional}-control{.py,.log}`,
`gpt-6.1-sol-readf-same-path-control{.py,.log}`,
`gpt-6.1-sol-readf-conditional-main-{cold,code,fields}.log`,
`gpt-6.1-sol-readf-main-cold-rounding-review.txt`,
`gpt-6.1-sol-readf-expression-child-windows.txt` and
`.analysis/compiler-vc9/execution-child-{flow,flow-macro,conditions-baseline,conditions-conditional,schedule}.txt`,
with their `trace-child-*-run.log` controls.

The private constructed-result model also retains the restored readers. It
owns 12,176 bytes / 3,254 instructions, frame `0xCC`, all 415 fields, and the
native call/conditional/jump counts 262/259/147. Its direct-call inventory has
no missing or extra destinations. It remains non-exact and private: matching
these counts and the frame does not close the remaining allocation/copy gaps.
Evidence: `.analysis/gpt-6.1-sol-constructed-readf-conditional-{control,code,fields}.log`.
Replacing the sum value's three-argument constructor with a default constructor
and three component assignments leaves those size/count/frame results intact.
The copied control changes 22 nonfield bytes and retains every field position
and `ESP+24` vector-result slot. Evidence:
`.analysis/gpt-6.1-sol-constructed-sum-default-{control.log,body-comparison.txt}`.

## The extra animation-reader merge occurs in late flow optimization (2026-11-26)

A complete call-only VC9 trace localizes the opcode 96/97 merge to
`FgDoLateFlowOpts` (RVA `0x48BD0`), called from `dop2_FinalFlowOpts` at
`+0x125403`. On the 11,816-byte interpreter candidate, all 78 float-reader
calls survive lowering and remain present at global-register-allocation
entry. The late flow pass removes two of them; subsequent code motion does
not restore them.

| Candidate stage | Float reader | ID lookup | RNG next | All calls |
| --- | ---: | ---: | ---: | ---: |
| Before final late flow | 78 | 7 | 4 | 264 |
| After `FgDoLateFlowOpts` | 76 | 5 | 3 | 259 |
| Before encoding | 76 | 5 | 4 | 260 |

The two ID-lookup merges reproduce the native shared factory continuations.
An RNG call is merged and then duplicated again before encoding. The native
interpreter has 78 float reads, five ID lookups, four RNG calls and 262 calls
in total. This separates the unwanted reader merge from legitimate merges
and shows why a total call count alone is insufficient.

The three snapshots cover the complete function's region chain. The traced
link and normal relink have identical 31,798-byte `.text` sections. Evidence:
`.analysis/compiler-vc9/execution-phase-counts.txt` and
`.analysis/compiler-vc9/trace-phase-counts-run.log`; reproduce with
`scripts/repo-python .analysis/compiler-vc9/trace-phase-counts.py` against the
intended cold input family. This observation precedes the effect-local timer
constructor repair. A full operand dump reached the five-minute bound, so
retain the call-only snapshots for phase localization and trace only the
matched tails when investigating the register schedule.

## One common-tail deletion removes both opcode 97 readers (2026-11-26)

A focused trace inside the final late-flow pass catches one
`FgDoCommonTailMerge` deletion containing both opcode 97 float-reader calls,
their float spill/reloads, the child X/Y stores and the translation copy.
The kept sequence belongs to opcode 96. This is a common suffix merge, rather
than two independent call eliminations. All 241 observed deletion ranges
were fully traversed; only this range contained float-reader calls.

At `TupRemoveRange` entry (RVA `0x10F180`), EAX is the first removed tuple and
EBX is the exclusive end. The caller at `FgDoCommonTailMerge + 0x4B7`
(return RVA `0x4D1E7`) retains the kept range at `[EBP-0x4C]` and
`[EBP-0x58]`. Check these conventions against this pinned backend before
reusing the trace. Operand metadata is compiler IR, not game field offsets.

The removed prefix already moves the lookup result from EAX to ESI
(`scratch.child`) before the argument load and variable-bit test. Native
opcode 97 makes that copy after the load/test. Its case-local
`positionedChild` currently coalesces into the shared scratch pointer.
Later local-pointer/reference controls do not recover the calls. The
conditional-return and final-scheduler traces above separate the preserved
reader calls from the eventual child-copy placement.

Evidence: `.analysis/compiler-vc9/execution-merges.txt` and
`.analysis/compiler-vc9/trace-merges-run.log`; reproduce with
`scripts/repo-python .analysis/compiler-vc9/trace-merges.py`. The debugger
link and ordinary control have identical 31,798-byte `.text` sections.

## VC9 splits the interpreter pointers before choosing their registers (2026-11-26)

The 11,816-byte pre-strip-rounding interpreter's EBX-VM/EDI-cursor inversion
is already constrained before
`ChoosePhysReg` selects the final registers. In the integer allocation round,
the original pointer ranges change as follows:

| Stage | VM | Instruction cursor |
| --- | --- | --- |
| Before integer `Spillcosts` | Seven general registers | Seven general registers |
| After `Spillcosts` | EBP | EBX, EBP |
| After frame constraints | Empty | EBX |
| Main split range at `ChoosePhysReg` | EBX | EDI |

The earlier floating-register round leaves the integer masks untouched; keep
the two rounds separate when reading phase snapshots. The frame correctly
reserves EBP. Changing declaration order to break a final register-choice tie
therefore misses the earlier split planning that produced these singleton
sets.

Two hardware watchpoints on the original ranges' sparse-vector words capture
nine mask changes inside `Spillcosts` (RVA `0x32440`). Fixed EAX setup and
`SetStageTimer` first remove caller-clobbered registers. A child-ID result
address in ESI removes ESI; the float-destination argument moved into EDI
removes EDI; the vector-tween return buffer addressed through EBX removes EBX
from the VM range. These are constraints from different interpreter regions,
not only the handler currently being tuned. Inspect split costs and real
callee register use before changing another local's spelling.

Useful VC9 points: the pre-assignment selection at `ChoosePhysReg + 0x2A1`
(RVA `0x36C41`), `CmdSetConstraints` at `0x1CA060`, and the integer
`Spillcosts` call/return at `0x2F503/0x2F508`. The current tuple in that
`Spillcosts` frame is `[EBP-4]`; `[EBP+8]` is its operand cursor.
Evidence: `.analysis/compiler-vc9/execution-registers.txt`,
`execution-initial-splits.txt`, `execution-mask-writes.txt` and their
`trace-*-run.log` controls. Each completed debugger/control pair has identical
31,798-byte `.text` sections. The traces explain this candidate's decisions;
the native split graph is not available.

A later trace follows the competition directly. Before the first integer
selection, both the original cursor and the main VM split allow only EBX.
Their allocation priorities are 5,760 and 7,466 respectively. The VM wins;
`UpdateAffectedGlobregs` empties the cursor's mask, and a new cursor split
with priority 5,134 subsequently allows EDI. This explains the inversion as
a primary allocation followed by cursor splitting, rather than two unrelated
register preferences. The original VM receives 13 split-marker insertions,
mostly around tween-result stores. Range numbers are reused between floating
and integer rounds: qualify them by register class and source identity.

Evidence: `.analysis/compiler-vc9/execution-vm-split-markers.txt` and
`execution-cursor-deferral.txt`, with their independent control logs
`trace-split-markers-verified-run.log` and `trace-cursor-deferral-run.log`.
Both completed controls replay identical 31,798-byte `.text` sections.

Traces of the preceding 11,940-byte, 23-TU real-timer graph confirm the same
competition in that source epoch. VM split 462 has priority 7,466; original
cursor 37 has 5,778, and both allow only EBX. Assigning the VM empties the
cursor mask; cursor split 444 later allows only EDI at priority 5,032.
The two completed debugger/ordinary pairs each reproduce all 33,446 text
bytes. These range numbers apply before the conditional-return change.
Evidence: `.analysis/compiler-vc9/execution-registers-real-timers.txt`,
`execution-register-allocation-real-timers.txt`,
`trace-registers-real-timers-run.log` and `trace-real-timers-deferral-run.log`.

A case-local opcode 97 child-pointer control leaves its handler unchanged and
retains the 11,816-byte owner, 260 calls and `0xD4` frame. Its only code changes
swap x87 load/arithmetic operands in opcodes 9 and 13: addition improves while
multiplication loses native order. The copied-source baseline reproduces the
entire maintained interpreter byte-for-byte. Keep the local-pointer trial in
`.analysis/gpt-6.1-sol-opcode97-local-control.log` and
`gpt-6.1-sol-opcode97-local-diff.log`; it is not a maintained improvement.

## Geometry math options change the interpreter helper ABI (2026-11-26)

The rotated quad's two CRT trig calls force its VM into ESI in the interpreter
context. Compiling only `AnimationVmRotation.cpp` with `/fp:fast` replaces those
calls with FCOS/FSIN and restores EAX-VM. That change propagates into
`BuildModeQuad`: its inputs become native EAX-VM/ECX-output, and the complete
108-byte helper has zero diagnostic differences across all nonfield bytes,
including three alignment bytes and its four-entry table. The new worktree unit
`build-animation-vm-mode-quad` subsequently passes strict cold replay: all eight
fields, complete public primary-TU PDB ownership and all 108 bytes match.
Its receipt and comparison are in `.analysis/gpt-6.1-sol-mode-quad-cold.log`
and `gpt-6.1-sol-mode-quad-cold-compare.json`. The helper is now canonical
after the complete affected shared-input replay; the promotion replay is
`gpt-6.1-sol-mode-quad-canonical-checkpoint.json`.

The rotated body is then 482 bytes, not native 571, and still lacks the paired
FSINCOS. The option supplies a useful reproducible helper context; it does not
establish the original rotation TU's math settings. Keep the main interpreter
and rotated owner non-exact. Evidence:
`.analysis/gpt-6.1-sol-execution-rotation-fast-control.log`.

Repeating the isolated rotation-profile change in the preceding 23-TU real-timer
graph leaves the whole interpreter at 11,940 bytes / 3,183 instructions /
407 fields. Every field position stays the same; only four nonfield bytes
change, swapping the EAX/ECX inputs at its two quad-helper calls. Frame `0xE4`,
VM/cursor EBX/EDI and both missing readers remain. The downstream ABI repair
therefore does not resolve the whole-owner allocation gap in this graph.
Evidence: `.analysis/gpt-6.1-sol-real-timer-rotation-fast-control{.py,.log}` and
`gpt-6.1-sol-real-timer-rotation-fast-owner-delta.txt`.

Useful rejected controls: `/G6` leaves all 11,816 interpreter bytes unchanged.
Putting the same geometry bodies in the shared renderer TU, then retaining its
real drawing root in a merged 30-TU graph, leaves every interpreter nonfield
byte unchanged. An inline update helper also leaves those bytes unchanged and
adds an unused 2,286-byte helper copy. These controls are retained in
`.analysis/gpt-6.1-sol-execution-{g6,renderer-owner,live-renderer}-control.log`,
`gpt-6.1-sol-execution-update-helper.log` and
`gpt-6.1-sol-execution-context-controls-nonfields.log`. The maintained source
and original frontier profiles are unchanged.

## Strip coordinate sums require two float rounding stages (2026-11-26)

The native ring and arc loops store a complete three-float intermediate after
adding position and other-position, then add translation and store a second
three-float result. For the first ring vertex, `0x44DD98..0x44DDC6` forms the
first result and `0x44DDCA..0x44DDF0` forms the second. The same boundaries
appear after the other three polar calls. Scalar flattening in `06b6b93`
removed the first float rounding. With round-to-nearest, the exact integer
inputs `(2^24, 1, -2^24)` produce 0 through the two rounded additions and 1
through the flattened extended-precision chain.

`EmitStripPair` now uses nested `Sum` return objects for both vertices,
preserving each observed operand grouping. Before correcting the argument
and timer context, this source-only cold owner was
11,952 bytes (11,536 code/alignment + 416 table), with 3,187 instructions and
a `0xE4` frame. Native has 12,056 bytes, 3,203 instructions and a `0xCC` frame.
The candidate still has 260/262 calls and 407/415 complete address fields.
Keep the rounding correction even though its frame is larger. Evidence:
`.analysis/gpt-6.1-sol-strip-call-windows.txt`,
`gpt-6.1-sol-execution-strip-rounding-cold-code.log` and
`gpt-6.1-sol-execution-strip-rounding-cold-fields.log`.

A scoped result constructor inspired by TH10's `AnmFloat3View` retains the
rounding and gives 11,920 bytes / `0xD4`, but diagnostic register-normalized
alignment drops from 2,551 to 2,448 native keys. It is not maintained. Capturing
ReadF's literal before the flag test instead grows the owner to 12,424 bytes
without restoring the missing calls. Both controls preserve the frozen source;
see `.analysis/gpt-6.1-sol-execution-vector-result-constructor*.log` and
`gpt-6.1-sol-execution-readf-capture.log`.

The old consumer replay was deliberately stopped before the source update.
Its logs/receipts are preserved. The new replay carries a prior result only
when the complete current receipt still verifies. The quad helper's complete
108-byte/eight-field cold proof repeats after the correction in
`.analysis/gpt-6.1-sol-mode-quad-post-strip-compare.json`. The helper is now
canonical after the complete affected shared-source replay.

The six missing DIR32 fields are all reads of the global zero vector at
`0x4C326C`: native reads each component four times, candidate twice. Expanding
the two aggregate assignments in `ZeroControls` into six scalar assignments
does not restore those fields. That isolated control stays at 11,952 bytes,
260 calls and 407 fields; its full-owner copied-source baseline has zero byte
differences. Evidence: `.analysis/gpt-6.1-sol-execution-zero-components.log`.
Investigate the global's ownership and alias context before changing its type
or access semantics.

Direct Ghidra xrefs find 13 reads of each component and no direct write to
these three addresses (`.analysis/gpt-6.1-sol-zero-vector-xrefs.txt`). Native
copies interleave each integer load and destination store, then reload all
three components for the second control. TH10's corresponding two tangent
copies use `memcpy` in `AnmManager.cpp`; this motivates a bit-preserving copy
control, rather than adding volatile access to the global. TH10 supplies the
source hypothesis; the TH11 load/store sequence supplies target evidence.

That isolated `memcpy` control restores all six missing DIR32 fields. The
complete owner has 156 DIR32 and 257 REL32 fields versus native 156/259;
only the two known float-reader calls remain missing. It owns 11,984 bytes
with 3,193 instructions, retains the `0xE4` frame, and raises diagnostic
register-normalized alignment from 2,551 to 2,568 keys. Local windows confirm
that the second control reloads all three words. Evidence:
`.analysis/gpt-6.1-sol-execution-zero-memcpy{,-code}.log`,
`gpt-6.1-sol-execution-zero-memcpy-fields.json` and
`gpt-6.1-sol-zero-memcpy-windows.txt`. Keep it isolated until the current
shared-input checkpoint; field-count agreement alone does not close the owner.

Moving those copies directly into the two opcode cases gives 12,016 bytes
and 3,194 instructions, still 156 DIR32 / 257 REL32 fields. Its diagnostic
register-normalized alignment falls to 2,541 despite the closer size; the
helper form scores 2,568. Evidence:
`.analysis/gpt-6.1-sol-execution-zero-direct-memcpy{,-code}.log`.

The cold quad-helper context also permits a full interpreter-side control of
the corrected callee ABI. Its 11,952-byte interpreter differs from the ordinary
frontier in just four nonfield bytes: at `+0x1771/+0x1773` and
`+0x2480/+0x2482`, VM/output setup changes from ECX/EAX to EAX/ECX. Its frame,
pointer allocation and complete branch/call counts stay unchanged. Evidence:
`.analysis/gpt-6.1-sol-execution-mode-quad-context-{code.log,delta.txt}`.

Read-only C2 traces narrow the copy distinction further. Both variants reach
`PRE_LOWER` with 805 regions, 5,634 tuples and 81 `OPMBASSIGN` operations.
The four zero-vector copies have the same 12-byte block opcode and type word;
the destination indirect operand's byte at `+0x20` is 3 for typed assignment
and 4 for `memcpy`. This is alignment metadata: the pinned C2's
`TupMakeIndir` writes it at RVA `0x1101AB/0x1101D1/0x1101E0`, including paths
through `MscRestrictAlign`. Do not label it an alias flag. The two debugger
links equal their ordinary controls over all 31,958/32,006 `.text` bytes.
Evidence: `.analysis/compiler-vc9/execution-zero-{typed,byte-copy}-prelower.txt`,
`trace-zero-{typed,byte-copy}-prelower-run.log` and
`tup-make-indir-review.txt`. An earlier seven-stage trace exceeded its
300-second bound; its partial `execution-zero-typed.txt` is diagnostic only.

Native translation accumulation at `0x44D970..0x44D9A0` stores each float
sum directly to the VM. The maintained aggregate `Sum` introduces three stack
stores and integer reloads. A component `+=` control removes seven instructions
and 32 owned bytes, but leaves the `0xE4` frame and reverses all three x87
load/add operand pairs. Combined with the zero-vector byte copies it owns
11,952 bytes / 3,186 instructions / 413 fields; diagnostic normalized alignment
is 2,472, versus 2,568 for byte copies alone. Keep this control isolated while
checking operand order. Evidence:
`.analysis/gpt-6.1-sol-execution-translation-accumulate{,-code}.log`,
`gpt-6.1-sol-execution-zero-and-translation{,-code}.log` and
`gpt-6.1-sol-translation-accumulate-windows.txt`.
Writing explicit assignments with native operand grouping produces identical
nonfield bytes across the whole 11,920-byte control; it does not recover the
x87 order. See `.analysis/gpt-6.1-sol-execution-translation-direct-code.log`
and `gpt-6.1-sol-translation-direct-delta.txt`. Further local expression
spelling trials have no support from this control.

## VC9 promotes the zero-vector components during register allocation (2026-11-26)

These traces use the 11,952-byte strip-rounding context with the argument
support TU still at `/Ob0`. The typed and byte-copy variants both still have
twelve global-zero loads at
`dop2_GlobalRegAlloc` entry. By `dop2_PostCodegenFlowOpts`, typed assignment
has six loads whose destination symbols are the global's own three scalar
equivalents. The byte-copy variant retains twelve loads into temporary symbols.
This localizes the difference after global-allocation entry, rather than to
the original block-copy expansion. All equivalents here have type `0x1004`
and four-byte scalar accesses; scalar destination alignment is 3 in both
variants. The earlier block alignment difference alone does not explain the
later register promotion.

Scalar equivalents lose their direct names. The pinned C2's `SymNewEquiv`
copies the root link at `SYM+8`, clears the direct symbol pointer at `+0`,
and stores the byte offset at `+0x24`. Following the root recovers the zero
vector's `+0/+4/+8` names. A direct-name-only search incorrectly reports no
late references. Static review and corrected traces are in
`.analysis/compiler-vc9/sym-new-equiv-review.txt`,
`execution-zero-{typed,byte-copy}-equiv-lowering-verified.txt` and
`zero-global-promotion-summary.txt`. Both corrected debugger links match their
ordinary controls over all 31,958/32,006 `.text` bytes; see
`trace-zero-{typed,byte-copy}-equiv-lowering-verified-run.log`.

All twelve original typed-load tuples are freed from `SubsumePseudos` through
the call at C2 RVA `0x31C1E` to `TupFree` (stack return `0x31C23`). A trace
restricted to those twelve tuples completes and matches all 31,958 `.text`
bytes against its ordinary control. See
`.analysis/compiler-vc9/execution-zero-typed-free-fast.txt`,
`trace-zero-typed-free-fast-run.log` and `subsume-zero-loads-review.txt`.
The earlier unfiltered allocator breakpoint exceeded 300 seconds; its partial
trace is retained separately.

A local `const CameraVector *zero = &g_AnimationExecutionZero` followed by the
original aggregate assignments also restores all twelve reads, without calling
`memcpy`. It gives 11,984 bytes / 3,193 instructions / 413 fields, but diagnostic
normalized alignment is 2,462 rather than the byte-copy form's 2,568. This
isolates source addressability as a useful lead; the byte-copy form is still
the better complete-owner control. Evidence:
`.analysis/gpt-6.1-sol-execution-zero-address{,-code}.log`.

The third missing interpreter conditional belongs to opcode 17's `fmod`
assignment tail. Native `0x44D41B` tests operand-zero decoding and `0x44D44B`
separately tests destination decoding. The candidate's explicit jump to
`animation_float_destination` shares the second test with other math cases.
The other two missing conditionals accompany opcode 97's known merged readers.
The physical case comparison is in
`.analysis/gpt-6.1-sol-execution-case-predicates.txt`.

Replacing opcode 17's shared tail with a local result restores its destination
test but adds a sixteenth `AnimationFloatDestination` call; native has fifteen.
Caching its raw operand pointer while retaining `scratch.floatResult` still
adds that call. Both isolated owners are 12,016 bytes / 408 fields; the pointer
form has 3,205 instructions and 257 conditional branches. Native shares the
variable-destination slow path at `0x44D6E5` while keeping the literal store
local. Preserve that distinction in further lifetime and callee-context work.
Evidence: `.analysis/gpt-6.1-sol-opcode17-{local,pointer}-code.log` and
`gpt-6.1-sol-opcode17-pointer-fields.{log,json}`.

## The exact argument profile repairs its ABI inside the interpreter graph (2026-11-26)

Using the canonical float-destination leaf's `/O2 /Oy /Ob1 /GL` profile for
`AnimationExecutionArguments.cpp` in the main frontier restores its complete
124-byte body and ESI-VM/EDI-operand interface. The previous `/Ob0` support
profile emitted 140 bytes and received the VM in EAX. Only that support TU's
`/Ob0` changes to `/Ob1`; its source and the other TU profiles stay the same.

At this stage the cold interpreter owns 11,952 bytes / 3,187 instructions,
260 calls and 407 address fields, with frame `0xE4` and EBX-VM/EDI-cursor.
Diagnostic register-normalized alignment improves from 2,551 to 2,564 keys.
The complete cold support contribution also replays the existing fourteen
float-destination bindings with zero byte differences. The canonical leaf's
separate primary-TU proof remains its accepted exact row. Evidence:
`.analysis/gpt-6.1-sol-execution-argument-profile-cold{,-code,-fields}.log`
and `gpt-6.1-sol-argument-profile-leaf-cold-replay.log`.

The integer-destination and pending quad-helper receipts verify before and
after this frontier-only setting change. Their contexts, including their own
argument support profiles, remain unchanged. The complete earlier frontier
family is preserved in `build/probe-ltcg/ExecutionBeforeArgumentProfile`.

Relinking the zero-vector byte-copy primary object with the repaired argument
support gives 11,984 bytes / 3,193 instructions / 413 fields and raises
diagnostic normalized alignment to 2,581. Its only missing call fields remain
the two opcode 97 float readers. Relinking the opcode 17 pointer control still
gives sixteen float-destination calls, so correcting that callee interface
alone does not restore the native partial tail sharing. Both controls keep
frame `0xE4` and EBX-VM/EDI-cursor. Evidence:
`.analysis/gpt-6.1-sol-execution-argument-couplings.log`,
`gpt-6.1-sol-argument-coupled-{zero,opcode17}-code.log` and
`gpt-6.1-sol-argument-coupled-zero-fields.log`.

## RGB operand lifetimes affect distant interpreter layout (2026-11-26)

Native opcode 57/78 handlers retain blue and green reader results as full
32-bit values until red has been read, then pack the three bytes before
reading the duration. The maintained source truncates each channel immediately.
TH10's `src/AnmManager.cpp` uses full `finalBlue`, `finalGreen` and `finalRed`
locals followed by byte packing in both corresponding color-time cases
(lines 3553–3596). This supplies a useful source model after the TH11 observation;
it does not establish TH11's original nominal color type.

With the repaired argument profile, preserving full channel values produces a
12,000-byte interpreter, 3,202 instructions and 407 fields. Each physical RGB
handler aligns 45 of 47 native instruction keys; the native handler is 169 bytes
and the candidate 174. Whole-owner diagnostic register-normalized alignment
rises from 2,564 to 2,633. The frame stays `0xE4`. C2 stack traces explain why:
the four additional blue/green word locals join two existing stack colors,
raising their weights from 122/97 to 128/103 without adding colors. Both traces
retain 26 colors and 205 payload bytes, and their ordinary whole-text controls
have zero differences. Keep the existing three-byte color view; there is no
basis for adding a fourth padding byte. Evidence:
`.analysis/gpt-6.1-sol-execution-rgb-word-temporaries{.py,.log}`,
`gpt-6.1-sol-rgb-word-temporaries-handler-diff.txt`,
`.analysis/compiler-vc9/execution-stack-materialization-{baseline,rgb}.txt`
and `stack-materialization-summary.txt`.

## Opcode 17 shares only the variable-destination tail (2026-11-26)

Direct Ghidra references confirm the native opcode 17 trampoline at `44D474`
branches to `44D6E5`; another incoming branch at `44D6E1` reaches `44D6F3`.
The literal result stays local. Separating the raw operand's local role from
the pointer passed to the shared variable resolver restores the destination
test without adding a sixteenth float-destination call. This control owns
11,984 bytes, 3,194 instructions and 407 fields. It still misses the two opcode
97 float readers. Evidence:
`.analysis/gpt-6.1-sol-math-slow-tail-xrefs.txt` and
`gpt-6.1-sol-opcode17-split-roles{.py,.log}`.

Inlining all four math destinations in TH10's direct-expression style gives
12,096 bytes, 263 calls and 410 fields. Its call inventory has eighteen
float-destination calls versus native fifteen, while still missing two float
readers. Likewise expanding all child-creation tails reaches the native total
of 262 calls by adding an ID lookup and list insertion; it does not recover
the missing readers. Reject both controls. The aggregate call count can conceal
wrong callees. Evidence:
`.analysis/gpt-6.1-sol-execution-direct-math-family.log`,
`gpt-6.1-sol-execution-local-child-family.log` and
`gpt-6.1-sol-materialization-call-inventories.log`.

## Vector construction changes stack alignment without removing colors (2026-11-26)

A scoped three-float value constructor changes VC9's temporary alignment in
the interpreter. This was measured in a private context combining the full-word
RGB reads, zero-vector byte copies, opcode 17's partial shared tail and direct
component translation stores. The POD-sum variant owns 12,064 bytes with frame
`0xE4`; returning constructed vector values owns 12,032 with frame `0xD4`.
Both have 3,216 instructions, 413 fields and the same 26 stack colors with
205 payload bytes, identical sizes, members and weights. No color disappears.

Three twelve-byte colors change alignment from eight to four. Their allocations
move from `-152/-168/-184` to `-144/-156/-168`; the remaining eight-aligned
12-byte color moves from `-200` to `-184`. The quad slot stays 48 bytes at
`-48`, and the eight-byte pair stays at `-64`. The compiler's authenticated
`ColorStackElements+0x4A5` instruction (RVA `108F85`) loads color field `+0x14`,
then passes it with the size to `allotemp` at `1F68C0`; the latter's mask and
rounding operations confirm that this field is alignment. The 16-byte frame
reduction therefore comes from packing, not fewer source temporaries.

Both traced links reproduce their ordinary controls' complete 32,086-byte
text sections with zero differences. All four strip call windows still contain
two complete three-component `FSTP32` rounding stages. The constructed variant
has diagnostic normalized alignment 2,676, still receives VM/cursor in EBX/EDI,
and still lacks the two opcode 97 readers; native frame is `0xCC`. Naming the
five vector/color return results and passing their addresses to ordinary
inline copy helpers does not close the remaining eight-byte frame gap. The
five anonymous members of the last shared color require eight-byte alignment;
the other twelve-byte elements have alignment code 3 (four bytes). A complete
812-region / 6,058-tuple walk maps their address operands to three actual
`StepStageVectorTween` calls (elements 153, 154, 156) and two
`StepAnimationIntVectorTween` calls (155, 157). The call operand's source symbol
matches the corresponding previously observed compiler-function symbol.
These five return buffers impose the shared color's eight-byte alignment.
`colorElemental` takes the maximum member requirement at RVA `1094B3`; the
member encoding at `SYM+0x2C` is converted to `1 << (code - 1)`.

Constructing only opcode 59's rotation target does not change frame `0xD4`;
it adds four owned bytes and one instruction (12,036 / 3,217), with the same
413 fields and two missing reader calls. Evidence:
`.analysis/compiler-vc9/constructed-vector-return-buffer-map.txt`,
`execution-stack-materialization-constructed-vector-return-calls.txt`,
`trace-stack-constructed-vector-return-calls-run.log`,
`stack-color-alignment-formation.txt` and
`.analysis/gpt-6.1-sol-rotation-target-construction{.log,-fields.json}`.

Evidence: `.analysis/gpt-6.1-sol-execution-vector-value-context{.py,.log}`,
`gpt-6.1-sol-vector-value-strip-rounding.txt`,
`.analysis/compiler-vc9/execution-stack-materialization-{component-vector,constructed-vector}.txt`,
`stack-value-context-summary.txt`, `stack-color-alignment-review.txt` and
`.analysis/gpt-6.1-sol-execution-named-results.log`. These private source
controls remain separate from the maintained interpreter. Earlier isolated
constructor trials worsened alignment; the useful result depends on the
surrounding data lifetimes and branch graph.

## Tween result types account for the remaining frame alignment (2026-11-26)

Constructed three-component return values restore the native `0xCC` frame in
a private interpreter graph. The control gives `StepStageVectorTween` and
`StepAnimationIntVectorTween` ordinary derived value result classes with
converting constructors, updates their actual definitions and declarations
together, and recompiles the primary TU plus all twenty support TUs. Calling
conventions and the three-component payloads stay unchanged. These classes
are an experiment in value semantics; their original TH11 nominal identities
are unknown. TH10's `AnmInt3View` also has constructors, and its update path
keeps an evaluated color as a const value before publishing three bytes.

C2 still packs exactly 26 colors with the same sizes, members and weights.
The five hidden result buffers change alignment code 4→3 (eight→four bytes),
and their shared color changes alignment 8→4. Its eight members move from
`-184` to `-180`; the other twelve-byte colors keep their previous offsets.
The final frame shrinks from `0xD4` to `0xCC`, and all five tween calls use
`ESP+0x24`, matching the native result-buffer location. The complete traced
32,134-byte text section reproduces its ordinary relink with zero differences.
The primary owner is 12,016 bytes / 3,215 instructions / 413 address fields.
Only the two known opcode 97 reader fields are missing; all direct calls bind.

The three float-vector result copies now use the native interleaved
load/store sequence through EDX, ECX and EAX, instead of loading three words
before publishing them. Their complete seven/seven/eight-instruction
post-call publication sequences have identical instruction texts without
register or field-offset normalization; the enclosing owner still differs. The VM/cursor registers remain EBX/EDI. Whole-owner
diagnostic register-normalized alignment is 2,584 versus the previous 2,676;
this heuristic does not capture all the newly recovered local schedules.
The main owner remains non-exact. Keeping the color locals in the constructed
result type changes them to byte reads from a separate `ESP+0x20` slot and
owns 12,032 bytes; it does not reproduce native sharing. Keep both controls
private while investigating the actual shared vector type.

Evidence: `.analysis/gpt-6.1-sol-execution-constructed-return-context{.py,.log}`,
`gpt-6.1-sol-constructed-return-{code.log,fields.log,call-windows.txt}`,
`gpt-6.1-sol-constructed-return-float-copy-sequences.txt`,
`.analysis/compiler-vc9/execution-stack-materialization-constructed-return-types.txt`,
`trace-stack-constructed-return-types-run.log` and
`.analysis/gpt-6.1-sol-constructed-result-local-color-calls.txt`.
The separate 21-TU copied-family baseline restores the prior 12,032-byte
owner and `0xD4` frame. Its 32,086-byte text has one raw byte difference,
inside the PE DIR32 field at `408D4D`: the `AllocateSceneAnimationVm` compiler
EH handler's FuncInfo address moves from `409198` to `409180`. The FuncInfo
records have equal nonpointer fields and refer to byte-identical unwind maps
at `409190` / `409178`. Replaying this reviewed address field leaves zero
text differences. Keep the raw log unchanged. Evidence:
`.analysis/gpt-6.1-sol-execution-return-context-control{.py,.log}` and
`gpt-6.1-sol-return-context-control-eh-replay.txt`.

TH10's explicit infinite event scan and cursor-first local declaration order
were also tested in the earlier constructed-sum context. Both retain the
complete 12,032-byte owner, all 413 field locations and zero nonfield byte
differences. Neither changes the event-loop layout. Evidence:
`.analysis/gpt-6.1-sol-event-scan-body-controls.txt`.

## Real timer callees repair two interpreter call interfaces (2026-11-26)

The earlier main frontier omitted the actual timer TUs. Its declarations alone
made the compiler use ordinary stack calls for `RewindSecondaryTimer` and
`OffsetSecondaryTimer`; native uses ESI for the timer and EAX for rewind's
integer, with only the floating offset on the stack. Adding the real
`SecondaryActorMotion.cpp` and `SecondaryProjectileCallbacks.cpp` with the
established `/O2 /Ob1 /GL` profile recovers those private interfaces naturally.
The rewind support body is twenty bytes; offset is 96. Offset's known near-one
x87 operand-order gap remains open.

A paired private control uses the same two real TUs with either POD or
constructed tween results. The POD graph owns 12,032 bytes / 3,214 instructions
and frame `0xD4`; the constructed graph owns 12,016 / 3,213 and frame `0xCC`.
Both retain 413 fields and only the two known missing opcode 97 reader calls.
The type-dependent frame change survives the corrected timer interfaces.
Whole-owner diagnostic normalized alignment is 2,668 / 2,576 respectively.
These scores do not justify retaining a wrong callee interface.

Fresh allocation traces of the constructed-result graph before the timer
addition show VM split 461 (priority 5,507) winning EBX over original cursor
37 (4,282). `UpdateAffectedGlobregs` empties the cursor's EBX mask; its new split
443 receives EDI at priority 3,556. The earlier 449/431 IDs and 7,466/5,760/5,134
scores belong to a different input. Both fresh traces reproduce their ordinary
32,134-byte text controls with zero differences. The same mechanism persists
after fixing the frame; the register numbers are evidence tied to this graph.

Evidence: `.analysis/gpt-6.1-sol-execution-real-timer-contexts{.py,.log}`,
`gpt-6.1-sol-real-timer-{pod-results,constructed-results}-{code,fields}.log`,
`.analysis/compiler-vc9/execution-register-allocation-constructed-return.txt`,
`execution-register-allocation-constructed-return-deferral.txt` and the
corresponding `trace-constructed-return-{registers,deferral}-run.log` files.
Before the conditional-return change, the 23-TU cold family owned 11,940 bytes / 3,183
instructions / 407 fields, with frame `0xE4` and diagnostic normalized alignment
2,558. Its complete twenty-byte rewind / one-field support replay is exact,
as is the 124-byte / fourteen-field float-destination support replay. These
support diagnostics do not replace primary-TU acceptance. Evidence:
`.analysis/gpt-6.1-sol-execution-real-timers-cold{,-code,-fields}.log` and
`gpt-6.1-sol-real-timers-{rewind,float-destination}-support.log`.
The preceding family is preserved at
`build/probe-ltcg/ExecutionBeforeRealTimers`; keep the constructed-result and
RGB/source controls private while their whole-owner differences remain open.

## Dynamic renderer input and flag width do not close the wrapper (2026-11-26)

The full unrotated-draw cold replay produces 53 bytes / seven fields instead
of native 51 / six. Its renderer becomes a constant global under LTCG. An
existing generic `/GL` producer restores dynamic renderer input and RET4,
but the complete owner is 49 bytes and its submission flag uses BL. A real
32-bit member flag declaration restores stack flag passing without changing
that size; native additionally saves/restores a four-byte slot with ECX.
TH10's `DrawInner` takes an integer flag, but TH11's byte tests do not settle
its nominal width. Neither private control is promoted. The obsolete exact
claim is withdrawn, preserving the full extent and failed receipts. See
[the focused draw note](ANIMATION_VM_DRAW.md#unrotated-wrapper-exact-claim-withdrawn-2026-11-26)
for evidence and the remaining compiler-layout question.

## Execution-view identity does not recover the two reader calls (2026-11-26)

The type merge in `9abb56f` is not, by itself, the cause of opcode 97's lost
reader calls. A complete copied pre-conditional-return control reproduces the
11,940-byte / 3,183-instruction owner. A zero-storage derived execution view
produces 11,936 / 3,181, while a separate flat view of the same observed fields
returns to 11,940 / 3,183. All retain 407 fields, frame `0xE4`, and 76 float
reads. Routing the flat view's operand helpers through the original formal VM
pointer leaves all nonfield bytes and field locations identical. A case-local
child reference changes eighteen nonfield bytes but still leaves 76 reads.
These controls give no reason to replace the maintained VM view.

A later inline component-setter control also keeps 76 reads and the complete
11,940-byte owner. Its opcode 97 still jumps into opcode 96 immediately after
copying the child to ESI; passing the float by value does not delay that copy.
Evidence: `.analysis/gpt-6.1-sol-execution-child-setter-control{.py,.log}` and
`gpt-6.1-sol-child-setter-case-windows.txt`.

The copied control's complete 33,446-byte text differs at one raw byte in the
PE DIR32 field at `40929D`. Its owner-qualified compiler EH FuncInfo moves
`40A1DC`→`40A1D4`; equal nonpointer fields lead to byte-identical unwind maps.
Replaying this independently checked field leaves zero text differences.
Evidence: `.analysis/gpt-6.1-sol-execution-{view-copy-control,distinct-view-context,flat-view-context,flat-formal-input,child-reference-control}.py`,
`gpt-6.1-sol-view-and-reference-body-controls.txt` and
`gpt-6.1-sol-view-copy-control-eh-replay.txt`.

The retained 2026-11-24 child-result images really contain 78 reader calls to
the same 540-byte reader. They are not a clean comparison for the current
source: all still narrow script IDs to sixteen bits before the factory, and
two copy translation before the X/Y reads. The child-result variant has the
correct later translation publication but still has the narrowing. The actual
factory indexes `file->scripts` with the full integer; do not restore the old
narrowing to influence allocation. Evidence:
`.analysis/gpt-6.1-sol-historical-child-reader-inventories.txt` and
`gpt-6.1-sol-historical-child97-publication-review{,s-more}.txt`.

## Separating scratch values changes a distant RGB stack color (2026-11-26)

Replacing the active float/pointer scratch union with two ordinary locals
reduces the pre-conditional-return private owner from 11,940 to 11,924 bytes and
frame `0xE4`→`0xDC`, with the same 3,183 instructions, 260 calls and 407 fields.
It does not recover the two readers. C2 traces show 26→25 colors and
205→202 payload bytes. The disappearing color is a three-byte RGB target,
not the float result: that result remains a four-byte stack element and joins
an existing word color. The RGB target (trace element 2) joins the vertex
pointer's four-byte color, moving from `-116` to the shared `-180` slot.
The quad stays at `-48`.

A focused color-choice trace exposes the propagation. Among the first fourteen
nonparameter elements, the changed bitmap rows are 0/1/2/3/4/7; each loses
element 8. In particular, maximum 7 no longer excludes the float/scratch
element. The float can join that word color, displacing angle 11 into the
instruction/v color. Color 12 then joins RGB initial 3, and vertex 13 joins
RGB target 2. Vertex 13's entire neighbor bitmap is identical in both runs;
the previously chosen color members differ. `colorElemental` scans colors
backward, rejecting membership conflicts before `SbvAnyCommon`, so a changed
earlier exclusion propagates through otherwise unchanged later lifetimes.
Evidence: `.analysis/compiler-vc9/scratch-color-choice-comparison.txt`,
`execution-stack-materialization-scratch-choice-{baseline,separated}.txt` and
`trace-stack-scratch-choice-{baseline,separated}-run.log`. Both complete
debugger/ordinary text controls again have zero differences.

Both complete traced text sections reproduce their ordinary links with zero
differences: baseline 33,446 bytes, separated scratch 33,430. This explains a
nonlocal frame change without inventing padding or assuming that eliminating
a source union removes its float spill. At both `fmod` calls and the `acos`
and `atan` calls, both bodies immediately materialize the result with a
four-byte `FSTP` stack store. The alternative remains private pending the
next affected-source comparison.
Evidence: `.analysis/gpt-6.1-sol-execution-active-scratch-control{.py,.log}`,
`.analysis/compiler-vc9/execution-stack-materialization-scratch-{baseline,separated}.txt`,
`active-scratch-stack-comparison.txt`, `active-scratch-stack-offsets.txt` and
`trace-stack-scratch-{baseline,separated}-run.log`. The four rounding windows
are in `.analysis/gpt-6.1-sol-active-scratch-math-rounding.txt`.

The same scratch change is not additive with the constructed tween-result
model. In its real-timer graph, the owner changes 12,016→12,008 bytes and
3,213→3,212 instructions while frame `0xCC` stays unchanged. Paired C2 traces
still show 26→25 colors and 205→202 payload bytes, but the five hidden-result
buffers move `-180`→`-176`. All three vector calls use `ESP+0x28` instead of
the native-matching `ESP+0x24`; the interleaved publication instructions remain.
Both traced links reproduce their ordinary complete 33,622-byte text sections
with zero differences. Keep the scratch model private: its smaller body loses
an independently recovered native layout. Evidence:
`.analysis/gpt-6.1-sol-constructed-active-scratch-control{.py,.log}` and
`.analysis/compiler-vc9/constructed-active-scratch-stack-comparison.txt`, with
`trace-stack-constructed-scratch-{baseline,separated}-run.log`.

## Stage culling and auxiliary switch owners (2026-11-17)

- The stage layer's formerly source-less `CullStageObject` (403950, 1,643
  bytes) projects **12** vertices, not just eight cuboid corners. Four
  center-X-plane samples join the eight AABB corners before the D3DX projection.
  The caller `DrawStageLayer` (404190, 727 bytes) runs 12 layer entry points
  and updates VM Fog/Z render state; see [STAGE_CULLING](STAGE_CULLING.md).
- The auxiliary stage's `g_MeshEffectEnemy` singleton at 4A8D64 is the same
  physical 0x4A0-byte owner whose registration (4061B0) and destruction
  (406260) are now reconstructed. Its callback jumps into the uninventoried
  406620 update owner: 208 code bytes plus a **24-byte six-entry switch
  table**, which must be included when comparing the complete owner. The
  typed C++ uses the existing character/partner globals rather than
  duplicating the data owner; see [STAGE_AUXILIARY](STAGE_AUXILIARY.md).
- The selector at 405E80 publishes camera base `runtime+0x204` with a
  0x118-byte stride and then calls `ApplyCamera` and D3D SetViewport; its
  original ESI/EBX convention is private LTCG, not a source-level custom ABI.
  See [RENDER_CAMERA](RENDER_CAMERA.md).

## NaN-sensitive stage culling and native stack ownership (2026-11-17)

The 1,643-byte `CullStageObject` at 403950 stores all three
camera-eye coordinates as independently rounded floats before
subtracting them from the object center. Native screen-space min/max
comparisons are **ordered**: an unordered/NaN X/Y comparison updates
neither extreme. The earlier C++ `>= / else` structure incorrectly
wrote a NaN into the minimum. The new strict `< / else if >` branch
and two groups of six projected samples match the source-level
flow. In the focused VC9 /GL context, dropping the forced EBP frame
and retaining camera-eye snapshots improves normalized instruction
alignment to **297/504** (1,468-byte candidate vs 1,643-byte native);
see [STAGE_CULLING](STAGE_CULLING.md). Do not promote diagnostic
instruction alignment to exactness.

## CRT library code and withdrawn asm claims (2026-11-14)

`scripts/scan-library-code.py` matches every code section of the pinned VC9
`libcmt.lib` and `libcpmt.lib` against the image with relocated fields masked.
A section counts only with a single hit, or with several hits where a checked
REL32 picks the right member. Every checked REL32 must also land on the
callee's matched address. It classifies 889 inventory functions
(150,480 bytes) as library code. All of them lie at 0x45FC2C or above, so the
game's own code ends at 0x45FC2B. 773 rows equal their archive symbol's extent
and are now boundary-reviewed. The other 116 sit inside a matched section but
are split differently, mostly SEH `__finally` blocks and one-byte tail
differences. They keep library origin but still need boundary review. About 139
CRT-region rows (12.5 KB) remain unmatched: `entry`, tiny duplicated wrappers
and code from objects outside these two archives.

The scan showed that the "exact" 0x45FD49 `FreeHeapBlock` is libcmt
`operator delete` (`delete.obj`), and that its source was a naked `__asm` copy
of the target. A sweep found the same pattern behind 24 exact claims
(8,310 bytes). Those included the replay writer, compressor, replay
initializer/loader/stage, record factories, score insertion/preparation, draw
publishers, pair leaves and `DestroyTitleMenuRecord`, which is aliased through
`/alternatename` to a naked destructor that uses `_emit`. Two more claims forced
their reads through volatile casts: `ConstructReplayRecordStageLink` 437490 and
`MusicStreamingThread` 44A900. All 26 claims (29 after a symbol-level re-sweep) are withdrawn, and their
`functions.csv` notes say why. By the end of 2026-11-14 every one of these
sources was rewritten in plain C++ and no `__asm` remains. Their match units remain only as measuring
harnesses for real C++ rewrites. The continue, game-over and pause menus and
`RecordReplayInputFrame` still contain `__asm` (including the
`DeletePauseMenuReplayRecord` helper in `PauseMenu.hpp`), so measurements of
those owners are not evidence of a C++ shape.

## Link context size (2026-11-15)

- Narrow probe links create false gaps. LTCG inlines, constant-folds and
  picks register conventions for shared callees based on the callers it sees.
  `StepStageFogTween` 4055D0 matched only in a link with every `Secondary*.cpp`
  source added to the screen-effect context; its "inlined operator+" gap came
  from the narrow tween probe. Sweeping large existing links for masked matches
  (`.analysis/tool-dirsweep.py`, `tool-lsweep*`) found 24 more already-exact
  functions without units.
- Callees that are still non-exact (for example `UpdateStage`, `DrawSecondaryHud`)
  can make LTCG choose a different convention than native, so their callers'
  call sequences stay off until the callee bodies are right.

## Inline definitions and opaque COMDAT callees (2026-11-14)

- An inline-asm `call F` anywhere in the link makes LTCG keep `F` on the
  standard ABI. `AppendReplayWriteSegment` stayed on stack arguments until the
  asm writer was stubbed out; with C++ callers it gets native's EBX/EDI. So a
  private register convention can only be reproduced once every caller is C++.
  A probe that tail-calls `F` with its own stack arguments also pins it.
- LTCG can pass `this` on the stack. The replay record destructor 435C50 is an
  ordinary out-of-line destructor (`ret 4`, `this` at `[esp+4]`), and callers
  call it directly rather than through a wrapper.
- `UnregisterChainElement` 457080 is `__stdcall (manager, element)` in
  source. LTCG passes element in ECX and manager in EDX, and right-to-left
  argument evaluation makes callers set ECX first, as every native caller with
  both values in registers does. The former `__fastcall (element, manager)`
  reproduced the body as normal COFF but put EDX first at every call site. With
  the swap, the replay record destructor 435C50 matches byte for byte apart from
  relocated fields. The lock/unregister block also appears in the Ascii
  (401210) and stage (402740) destructors.
- `AsciiManagerObject`'s only virtual is a size method (vtable 0x4938C8 ->
  401DD0, `mov eax,0x184BC; ret`). The destructor is not virtual, which lets
  LTCG pass `this` on the stack as native does. `StageEmbeddedVm`'s destructor
  is an inline header member: native inlines it in the Ascii destructor and
  keeps the 402170 copy for array teardown. With both corrections the Ascii
  destructor matches 475/475 bytes apart from IAT fields.
- The record, Ascii and stage destructors are all SEH-framed. The linked
  oracle binds only public symbols and does not compare FuncInfo data, so
  these destructors cannot take exact credit until EH acceptance is designed.

- VC9 LTCG uses a callee's real register usage only for ordinary functions.
  The out-of-line COMDAT of an `inline` function whose inlining was declined is
  treated as a standard call. Native callers that keep `this` in ESI and
  reload ECX around a thiscall (`mov ecx,esi; call; mov eax,esi`) point to an
  inline callee. This made the three fog operators exact; see
  [STAGE_TWEEN](STAGE_TWEEN.md).
- A function inlined in one TU but called out of line everywhere else, even
  from small callers, is defined `inline` in that .cpp and only declared in the
  shared header (`SetStageTimer`, `Pack`).
- The image keeps unreferenced COMDAT copies of inline helpers. Small
  uninventoried islands next to their users, such as 405A90..405B6F and
  405C20, are such copies. When every call to an inline member is inlined
  under LTCG, its dead copy allocates EDX for the loaded field and keeps ECX;
  an address-taken or non-inline definition uses ECX. This reproduces the
  42F340/42F350 comparison islands byte for byte (see
  [PAUSE_GAME_OVER_MENU](PAUSE_GAME_OVER_MENU.md)) and the timer `>=` copy at
  405AC0 has the same shape.
- 20 source-present non-exact leaves have no direct calls and no stored
  address, so they are probably dead inline copies whose register conventions
  LTCG chose freely: 40FFC0, 410530, 42F340, 42F350, 433DD0, 433E10, 433E30,
  433E50, 433EA0, 433F10, 4353A0, 4353E0, 435430, 435460, 441650, 44ED70,
  453440, 459C60, 45DC80, 45DD50 (scan: E8/E9 targets plus little-endian
  address search over the attested image). The integer-pair group is already
  reproduced this way; see [SECONDARY_PATTERN](SECONDARY_PATTERN.md).
- 459C60 is `ChainElement::SetCallback` (callback=value, +0C=+10=0), not an
  audio helper: the same three stores are inlined in about two dozen chain
  registrations. As an inline member used by the ASCII lifecycle's chain
  helper it is exact (unit `chain-element-set-callback`). Searching the image
  for a dead copy's store shape is an effective way to find its real users.
- VC9 emits `add r,-4` (rather than `sub r,4`) for `if (x - 4 < 0) ...; x -= 4;`
  on a member: the test is formed on the unmodified member. Locals,
  `x += -4` and helper calls all canonicalize to `sub`.
- The length/angle copies 435400/435430/435460 do not reproduce as plain
  inline-copy probes. Native realigns their frame (`push ebp; and esp,-8`)
  around `_CIsqrt`/`_CIatan2`; probes with `sqrt(double)`, the float `sqrt`/
  `atan2` overloads, `sqrtf` or explicit double temporaries all give the
  unaligned 33/34/19-byte form. The alignment probably comes from LTCG's
  stack-alignment choice for the real (inlined) callers, so find those first.
- A global `/Ob2` is rejected: it inlines `ResetMusicDecoder` into the exact
  `ConfigureMusicDecoder`. Keep `/Ob1`; prefer source `inline` over
  `__forceinline` when testing why native inlined a helper.

## Latest continuation menu3192

42D9D0 owns3192 bytes through42E647, not its old3141 default. Full flow has872
instructions/3122 code+22 layout+48 table,88 calls/162 fields/15 RET4 GS paths.
Source covers states12..19, practice-score updates, replay mode0 save, separate
record-name writes and delayed runtime transition. The record-name and practice
score formulas differ; timestamp0C/stage68 must not alias difficulty64.

Historical63-support draft:3048 bytes/158 fields/2770 differences, frame58,
14/15 returns and four call-site gaps. After merging the case13 factory the
targeted63-support owner is now3172 bytes/160 fields/2767 differences, with15/15
returns and no call-count gaps. Frame54 now differs from native58. Keeping
two explicit result objects restored frame58 but regressed to14 returns and
three call-count gaps (3016 bytes/157 fields/2799 differences), so it was
reverted. The factory-merging source is retained for its closer call graph. Old19
inspected contributions stay byte-identical. No exact unit was added. Required
close92/show64 replay zero only in support; score33 and factory105 remain open.
See [PAUSE_CONTINUE_MENU](PAUSE_CONTINUE_MENU.md).

The formerly missing game-over child-finder caller42E6B0 has a fully
reviewed3208-byte owner including a seven-entry switch table, and VC9-compiled
state20..26 source. Targeted linked candidate3060/native3208 has88/88 calls
and14/14 returns; still nonexact. See
[PAUSE_GAME_OVER_MENU](PAUSE_GAME_OVER_MENU.md).
The native overlay queue42F400/103 now has a cold linked-PE **exact**
primary-TU proof with all103 bytes equal and zero fields. Its caller42F480/101
also has compiled C++ source but remains **non-exact**; the width/height
argument order was corrected using the established sprite layout. The neighboring 42F340/350
14-byte comparison candidates are 12/14 with ordinary C++ thiscall
source and unproved incoming references. See
[PAUSE_OVERLAY_RECTANGLES](PAUSE_OVERLAY_RECTANGLES.md) and
[PAUSE_GAME_OVER_MENU](PAUSE_GAME_OVER_MENU.md).
PreparePauseContinueRecords42D850/380 and its insertion callee42BA80/365
now have maintained VC9-compiled source; the normal-COFF diagnostic sizes are
387/232 respectively, neither exact. The insertion percentage uses target
double100.0 at497E50; the preparer rechecks stage7 after insertion.
Replay compression main4423A0/795 now has compiled source (normal COFF738/795,
non-exact), in addition to search442910/416 and four tree edits.
Replay writer436420/1856 and allocator437300/150 have VC9-compiled source,
but neither is exact. The scoped writer link now generates1765 bytes with
46/46 native call sites; the chunk constructor uses operator new and the
unconditional generic doubly linked-list insertion. ABI remains open. The adjacent compression dictionary reset4428D0/52 is
full normal-COFF exact with four reviewed fields, and predecessor442BE0/49
is now cold linked-PE exact with three reviewed node fields. The adjacent
416-byte tree matcher442910 has compiled source (same-length but nonexact
normal-COFF candidate); its four node splice/delete/replace/predecessor
helpers now also have compiled but nonexact source. Focused five-TU
VC9 /GL gives encoder760/795 and tree matcher414/416; same-length
node replacement still differs in 65 unmasked bytes. See
[REPLAY_WRITING](REPLAY_WRITING.md).
Existing ActorInstructions/HUD/gameplay sources do not implement these owners.

## Pause3212 and returned-ID lifetimes

42C8D0 owns3212 bytes, including both tables. All101 calls,164 native fields,
14 GS/RET4 paths and native frame68 were reviewed. The59-support candidate
3084/160 fields/3039 differences has frame68/cookie64. Seven direct child-query
expressions recover seven real hidden result slots; an inline wrapper reused
one slot and produced frame4C. Account for the actual source script push when
reasoning about stack addresses; never normalize byte comparison for credit.

Confirm/shortcut/cancel tests are independent. Replay mode1, transient state14,
two cancel timer resets, sound44, name persistence and raw metadata stores are
retained. Four native address-field occurrences are absent: session4A8E88,
active replay4A8EB8, name-string base4951E8 and runtime base4C3280. The shared
child finder is still76/native70/41; incomplete callers do not prove a forced
register ABI. See [PAUSE_MENU](PAUSE_MENU.md).

## Menu and shared-code findings to preserve

- Audio43AAD0/3836 is canonical linked exact. Caller43A6F0/992 still has nine
  prologue differences in its audio context. See [CONFIGURATION_AUDIO_MENU](CONFIGURATION_AUDIO_MENU.md).
- Difficulty1056, character1228, partner1516, practice1024, player-data2448/750,
  music2668 and joystick2439 are complete source owners but remain non-exact.
  Each topic records its own context. Smaller early extents omitted owned tables
  or late blocks; do not restore them.
- BindAnimationSprite44AB40 uses the QWORD1/256 coefficient, not0.5. The shared
  correction was cold-replayed over11 affected exact units; do not revert it.
- Void history push can naturally leave EAX live under /GL; this does not prove
  an original return-this API. Saved character/partner values may be published
  raw after clamp calls. The target's ordering must survive tidy-up.
- Normal/practice transitions use480/392 and a five-argument screen effect.
  Fade is float6.0; its FSTP replaces a pushed pointer slot. Partner writes
  runtime558=7 before fade; practice writes it afterward.
- MenuController and MenuPrimary measurements, including the isolated primary30
  differences, belong to historical contexts. Cold-build their public recipes
  before a new measurement.

## Trimmed VC9 environment

VC9 RTM21022 works without its installation Git database, ATL/MFC, CRT sources,
64-bit tools/libraries, managed/debug CRT libraries or unused SDK tools. The
retained x86 compiler, SDK headers/libraries and runtime passed normal C/C++, /GL,
resource and MAP/PDB link smoke. Temporary cold builds fully matched FileExists141,
sprite UV81 and screen-effect initialization1168, including all68 fields.
Factory's NTLM helper and complete dependency tree match the existing lock and
can be shared; keep the TH11 Wine prefix independent.

## Other large source owners

These native reviewed extents are source-present, not current byte-equality
claims. Rebuild a selected context before quoting a new candidate metric.

| Owner | Native bytes | Detailed evidence |
| --- | ---: | --- |
| ExecuteSecondaryActorInstruction412E30 |16556|[host dispatch](SECONDARY_HOST_DISPATCH.md)|
| ExecuteAnimationVm44B4B0 |12056|[animation](ANIMATION_EXECUTION.md)|
| RunSecondaryScript45B860 |7093|[script](SECONDARY_SCRIPT.md)|
| RunSecondaryDialogue41D380 |6980|[dialogue](SECONDARY_DIALOGUE.md)|
| UpdateSecondaryHud41B380 |4716|[HUD](SECONDARY_HUD.md)|

## Topic index

| Work | Evidence and remaining differences |
| --- | --- |
| Continuation/game-over menu3192 and replay/record names | [PAUSE_CONTINUE_MENU](PAUSE_CONTINUE_MENU.md) |
| Pause3212, replay slots and returned-ID lifetimes | [PAUSE_MENU](PAUSE_MENU.md) |
| Difficulty, character and partner selection | [difficulty](MENU_DIFFICULTY_SELECTION.md), [character](MENU_CHARACTER_SELECTION.md), [partner](MENU_PARTNER_SELECTION.md) |
| Practice, player data, music and joystick | [practice](MENU_PRACTICE_STAGE.md), [data](MENU_PLAYER_DATA.md), [music](MENU_MUSIC_ROOM.md), [joystick](MENU_JOYSTICK.md) |
| Complete animation interpreter12056, events and all tween/strip paths | [ANIMATION_EXECUTION](ANIMATION_EXECUTION.md) |
| HUD4716, real dialogue caller and full boss/animation lifecycle | [SECONDARY_HUD](SECONDARY_HUD.md) |
| Dialogue6980 and complete27-opcode script source | [SECONDARY_DIALOGUE](SECONDARY_DIALOGUE.md) |
| Gameplay and ECL debug factory callers | [SECONDARY_GAMEPLAY](SECONDARY_GAMEPLAY.md) |
| Mesh and effects | [STAGE_MESH](STAGE_MESH.md), [MESH_EFFECTS](MESH_EFFECTS.md), [VM factory](ANIMATION_VM_FACTORY.md) |
| Stage interpreter and interpolation | [STAGE_SCRIPT](STAGE_SCRIPT.md), [STAGE_TWEEN](STAGE_TWEEN.md), [STAGE_EASING](STAGE_EASING.md) |
| Stage lifecycle, updates and drawing | [STAGE_LIFECYCLE](STAGE_LIFECYCLE.md), [STAGE_UPDATE](STAGE_UPDATE.md), [STAGE_DRAWING](STAGE_DRAWING.md) |
| Stage/ANM resources | [STAGE_RESOURCE](STAGE_RESOURCE.md), [ANIMATION_CHUNKS](ANIMATION_CHUNKS.md), [ANIMATION_TEXTURES](ANIMATION_TEXTURES.md), [pixel processing](ANIMATION_TRANSPARENT_PIXELS.md) |
| Renderer quad construction/submission | [ANIMATION_QUAD](ANIMATION_QUAD.md), [ANIMATION_VM_DRAW](ANIMATION_VM_DRAW.md), [ANIMATION_PROJECTED_QUAD](ANIMATION_PROJECTED_QUAD.md), [ANIMATION_IMMEDIATE_DRAW](ANIMATION_IMMEDIATE_DRAW.md) |
| Secondary script interpreter, typed stack and threads | [SECONDARY_SCRIPT](SECONDARY_SCRIPT.md) |
| Secondary program installation and context selection | [SECONDARY_PROGRAM](SECONDARY_PROGRAM.md) |
| Secondary extension parser and trace context | [SECONDARY_EXTENSION](SECONDARY_EXTENSION.md) |
| Secondary lifetime, counters and large actor owners | [SECONDARY_LIFETIME](SECONDARY_LIFETIME.md) |
| Actor construction/destruction, pair interpolation and source contexts | [SECONDARY_ACTOR](SECONDARY_ACTOR.md) |
| Complete actor state, transitions, motion and VM return contracts | [SECONDARY_ACTOR_STATE](SECONDARY_ACTOR_STATE.md) |
| Pair arithmetic, omitted vector leaves and vendor floor | [SECONDARY_PAIR_MATH](SECONDARY_PAIR_MATH.md) |
| Complete damage/collision, projectile records and region allocation | [SECONDARY_DAMAGE](SECONDARY_DAMAGE.md) |
| Complete projectile update/homing, iterator/literal math and atan2 | [SECONDARY_PROJECTILES](SECONDARY_PROJECTILES.md) |
| Complete spawn, corrected record layout and omitted integer/emitter leaves | [SECONDARY_PATTERN](SECONDARY_PATTERN.md) |
| Real pattern file/callers, complete callbacks and containing emitter array | [SECONDARY_PATTERN_CALLBACKS](SECONDARY_PATTERN_CALLBACKS.md) |
| Complete emitter reset, list directions and factory ownership | [SECONDARY_EMITTER_RESET](SECONDARY_EMITTER_RESET.md) |
| Complete player controller, real callback contracts and full movement/drop tables | [SECONDARY_PLAYER_CONTROLLER](SECONDARY_PLAYER_CONTROLLER.md) |
| Exact orbit/aim callbacks, player death and life HUD | [SECONDARY_EMITTER_MOTION](SECONDARY_EMITTER_MOTION.md) |
| Exact VM release/position publication, ID values and marker read timing | [SECONDARY_ANIMATION_VALUES](SECONDARY_ANIMATION_VALUES.md) |
| Large interpreter caller context, trace types and native fixed frame | [SECONDARY_INTERPRETER_CODEGEN](SECONDARY_INTERPRETER_CODEGEN.md) |
| Large actor host dispatcher, full tables and missing interpreter caller context | [SECONDARY_HOST_DISPATCH](SECONDARY_HOST_DISPATCH.md) |
| Complete scene/actor/object clearing and floating comparison distinctions | [SECONDARY_CLEARING](SECONDARY_CLEARING.md) |
| Complete collision transition, primary plus32 VMs and timer/spell gates | [SECONDARY_COLLISION](SECONDARY_COLLISION.md) |
| Complete player movement, callback-slot correction and event setter | [SECONDARY_PLAYER_MOVEMENT](SECONDARY_PLAYER_MOVEMENT.md) |
| Complete horizontal wrap and exact item attraction | [SECONDARY_PLAYER_WRAP_ITEMS](SECONDARY_PLAYER_WRAP_ITEMS.md) |
| Streaming factories and lifetime ownership | [MUSIC_FACTORIES](MUSIC_FACTORIES.md) |
| Device setup and buffer recreation | [AUDIO_DEVICE](AUDIO_DEVICE.md) |
| Streaming buffers, decoder state and fades | [MUSIC_STREAM](MUSIC_STREAM.md) |
| Audio command queue and streaming worker | [SOUND_QUEUE](SOUND_QUEUE.md) |
| Animation interpolation, dispatch and pool allocation | [ANIMATION_TWEEN_DISPATCH](ANIMATION_TWEEN_DISPATCH.md) |
| Renderer lifecycle and pixel helpers | [ANIMATION_RENDERER_LIFECYCLE](ANIMATION_RENDERER_LIFECYCLE.md) |
| Buffered drawing and randomized fan | [ANIMATION_FAN](ANIMATION_FAN.md) |
| Renderer and device | [RENDER_CAMERA](RENDER_CAMERA.md), [DEVICE_INITIALIZATION](DEVICE_INITIALIZATION.md), [RENDER_STATE_RESET](RENDER_STATE_RESET.md), [RUNTIME_SURFACES](RUNTIME_SURFACES.md) |
| ASCII lifecycle, glyph drawing and formatting | [ASCII_LIFECYCLE](ASCII_LIFECYCLE.md), [ASCII_DRAWING](ASCII_DRAWING.md) |
| Text | [TEXT_RESOURCES](TEXT_RESOURCES.md), [TEXT_DRAWING_COMPOSITE](TEXT_DRAWING_COMPOSITE.md), [OUTLINED_TEXT](OUTLINED_TEXT.md), [FORMATTED_TEXTURE_TEXT](FORMATTED_TEXTURE_TEXT.md) |
| Title-menu and replay object lifecycles | [TITLE_MENU_LIFECYCLE](TITLE_MENU_LIFECYCLE.md), [REPLAY_RECORD_LIFECYCLE](REPLAY_RECORD_LIFECYCLE.md), [REPLAY_INPUT_RECORDING](REPLAY_INPUT_RECORDING.md), [REPLAY_FILE_BROWSER](REPLAY_FILE_BROWSER.md) |
| Pbg archives, disk paths and resource lookup | [PBG_RESOURCE_SESSION](PBG_RESOURCE_SESSION.md), [PBG_DIRECTORY_LOADER](PBG_DIRECTORY_LOADER.md), [PBG_RESOURCE_LOOKUP](PBG_RESOURCE_LOOKUP.md), [PBG_LOW_LEVEL_FILE](PBG_LOW_LEVEL_FILE.md) |
| Pbg memory streams and Win32 resource/process APIs | [PBG_MEMORY_FILE](PBG_MEMORY_FILE.md), [PBG_WINRESOURCE_MEMFILE](PBG_WINRESOURCE_MEMFILE.md), [PBG_PROCESS_LAUNCHER](PBG_PROCESS_LAUNCHER.md) |
| File/archive | [RESOURCE_FILES](RESOURCE_FILES.md), [SHARED_FILE](SHARED_FILE.md), [ARCHIVE_DECOMPRESSION](ARCHIVE_DECOMPRESSION.md) |
| Frame timing and thread lifetime | [FRAME_CLOCK](FRAME_CLOCK.md), [THREADINF_LIFECYCLE](THREADINF_LIFECYCLE.md) |
| Callbacks, input, frame and startup | [CALLBACK_CHAIN](CALLBACK_CHAIN.md), [INPUT_POLLING](INPUT_POLLING.md), [FRAME_LOOP](FRAME_LOOP.md), [STARTUP_WINDOW](STARTUP_WINDOW.md) |

The player controller's eight-pair VM reset42F580/286 now has
complete cold linked-PE exactness (286/286, one reviewed global DIR32)
from a naturally compiled 304-byte controller reset caller.
Parent42FEC0/304 also has compiled source but remains non-exact
(319-byte linked draft); see [SECONDARY_PLAYER_ANIMATION_RESET](SECONDARY_PLAYER_ANIMATION_RESET.md).

Controller initializer42F8A0/1557 now has full VC9-compiled source,
13 native calls/2 returns in the narrow linked context; both narrow1479
and helper-linked1520 remain non-exact. Refer to
[SECONDARY_PLAYER_INITIALIZATION](SECONDARY_PLAYER_INITIALIZATION.md)
for caller and codegen gaps.


The player storage constructor's native vector-loop callbacks
at4021D0 and433F00 are now distinct **3/3-byte cold normal-COFF exact**
owners with zero fields; each is an identity-only ECX-to-EAX constructor.
The initializer42F8A0/1557 also has a source-level data-flow correction:
the second collision extent reads table4A7AB0, not focus table4A7AC0.
The large initializer and storage constructor remain non-exact;
see [SECONDARY_PLAYER_INITIALIZATION](SECONDARY_PLAYER_INITIALIZATION.md).


The TH11 player allocation factory4301B0/152 now has a natural C++
RAII source whose 0x8D24-byte typed storage restores VC9 exception
handling: normal COFF150, focused initializer /GL149, and
initializer+storage lifecycle /GL **152/native152** with five calls
and two returns. It remains **non-exact** (121 raw byte differences
and distinct optimized helper ABIs); see
[SECONDARY_PLAYER_INITIALIZATION](SECONDARY_PLAYER_INITIALIZATION.md).


The complete 33-byte original compiler-style element iterator4021A0
has reviewed ownership and maintained source in
`SecondaryPlayerElementIterator.cpp`. It is called twice per
player emitter in constructor42F760. Generic VC9 /GL gives 33 bytes
with29/33 identical but with the count/array registers swapped;
the actual seven-element callsite specializes it to31 bytes,
so **not exact**. Restoring explicit caller-level helper invocations
in the 272-byte storage constructor recovers its original five calls,
one RET4, and a focused linked **278/native272** contribution
(previously236/three calls). See
[SECONDARY_PLAYER_INITIALIZATION](SECONDARY_PLAYER_INITIALIZATION.md).


The player storage constructor42F760/272 now uses a physical inline
two-embedded-VM prefix (members at14/448) instead of two unrelated
placement-new statements. Pinned normal COFF improves
287/27-of-232 comparable bytes to **271/46-of-232**.
Scoped /GL remains **278/native272**, with all five native calls
and one RET4. This is still non-exact; see
[SECONDARY_PLAYER_INITIALIZATION](SECONDARY_PLAYER_INITIALIZATION.md).


The player destructor430010/415 now models two scope-guarded
chain unregister operations and **automatic reverse C++ destruction**
of embedded VM members. Their `storage400` fields are precisely
controller offsets848 and414, matching the native cleanup order.
In a limited VC9 /GL context including StageLifecycle.cpp the
owner improves from357/12 calls/2 returns to **400 bytes/
12 calls/1 return**, versus native415/12/1. It is still non-exact;
see [SECONDARY_PLAYER_INITIALIZATION](SECONDARY_PLAYER_INITIALIZATION.md).


Animation document cleanup4549E0/243 is now **strict cold-linked
exact 243/243**, with eight reviewed native fields and the full
six-byte internal alignment. Maintained C++ in
AnimationDocumentDestructor.cpp frees texture COM objects, source
buffers and five document allocations in observed order. The
separate menu marker4564C0/78 is also **cold-linked exact 78/78**
with zero fields, when a real generic renderer/file caller
prevents LTCG over-specialization. Both strict units retain
complete source/support/tool/PE/MAP/PDB receipts. See
[ANIMATION_DOCUMENT_DESTROY](ANIMATION_DOCUMENT_DESTROY.md).


The player storage destructor430010/415 now directly inlines the
observable two StageEmbeddedVm `storage400` cleanup operations after
its configuration free. With exact animation-document/marker callees,
the scoped /GL parent has all **three native direct free calls**
and the full **12-call/one-RET** graph, producing435/native415 bytes.
It remains non-exact; the storage constructor42F760 is unaffected.
See [SECONDARY_PLAYER_INITIALIZATION](SECONDARY_PLAYER_INITIALIZATION.md).


Animation document slot free4549A0/51 now has strict cold-linked
**exact 51/51** from natural C++ in AnimationDocumentSlotsLifecycle.cpp,
replaying two REL32 calls to exact document cleanup4549E0 and operator
delete45FD49. Its neighboring 32-slot scanner4548D0/135 has source
but is non-exact (115-byte scoped /GL), with the original unchecked
pending-destroy reload retained. See
[ANIMATION_DOCUMENT_SLOTS](ANIMATION_DOCUMENT_SLOTS.md).


The animation-document slot scanner4548D0/135 is now a focused
VC9 /GL **135/135-byte**, 50-instruction, 3-call/2-return complete
candidate. Its ordinary C++ restores the signed lower index check,
nonvolatile busy memory comparison and renderer-index reload; after
one DIR32 and three REL32 native fields only **one instruction byte**
differs (EDI vs EAX in the **then-non-exact** AdvanceAnimationDocument4544C0/212 context).
This scanner remains **non-exact**; the neighboring 51-byte slot
release remains strict canonical exact. See
[ANIMATION_DOCUMENT_SLOTS](ANIMATION_DOCUMENT_SLOTS.md).


Animation chunk decoding at4545A0/811 is now a **cold linked-PE
exact 811/811-byte boss function**, with all 15 independently reviewed
DIR32/REL32 fields replayed and nine owned alignment bytes.
The natural `script[i].offset` loop and the native sprite
fileIndex/texture/textureIndex assignment order eliminate all ten
previous code differences. The 149-byte StoreAnimationSprite is
separately exact. The later AdvanceAnimationDocument4544C0/212 cold
proof supersedes this historical non-exact observation; see the later
entry below and [ANIMATION_CHUNKS](ANIMATION_CHUNKS.md). See [ANIMATION_CHUNKS](ANIMATION_CHUNKS.md).


The 221-byte LoadAnimationChunkSource real parent context now includes
BeginLoadAnimationDocument: the dedicated load-animation-chunk-source
unit reproduces all 221 bytes, six calls and two RET4 exits. Its twelve
DIR32/REL32 fields replay with zero differences. A natural register-offset
local restores both native SIB selectors at B3 and BD. It is strict
linked-PE exact and remains independently built from the 811-byte decoder.
See [ANIMATION_CHUNKS](ANIMATION_CHUNKS.md).

Renderer priority callbacks 455140..45531E: the real
`AnimationRenderer` constructor provides the natural /GL producer context
for all 22 formerly source-open wrappers. They carry renderer in EDI,
selected dense layer in EAX and call dispatcher455660. Ordinary COFF
collapses the wrappers to 9 bytes, but the real producer reconstructs
12/15 bytes exactly, with one independently reviewed REL32 field per
callback and zero remaining byte differences under strict cold replay.
The five stateful priorities are now independently exact (see below).
See [ANIMATION_RENDERER_LIFECYCLE](ANIMATION_RENDERER_LIFECYCLE.md).

The 221-byte animation source loader4543E0 is now strict linked-PE exact.
The remaining SIB0A/SIB11 mismatch was resolved with the natural
register int offset = index * sizeof(*file->textures); source expression;
the dedicated producer unit replays all twelve fields and all 221 bytes
with zero differences. The earlier rejected variants remain isolated in
.analysis/try-animation-load-sib-20261111.py; the separately exact
811-byte decoder remains an independent unit.

Five renderer-priority stateful callbacks 455170/126, 455320/46,
455350/74, 4553A0/74 and 4553F0/133 become strict linked exact under
the real constructor, existing exact ApplyCamera and batch-flush code.
All 43 DIR32/REL32 fields bind the runtime's three cameras at root
+204/+31C/+434, active pointer+54C, index+550, fog cache+990,
renderer offsets, and the existing camera/layer dispatcher functions.
The 133-byte priority-62 owner restores screen camera after drawing
the alternate layer and retains the first draw result. No full-cohort
cold replay was needed. See
[ANIMATION_RENDERER_LIFECYCLE](ANIMATION_RENDERER_LIFECYCLE.md).

A real 48-byte four-slot animation surface-copy producer at445460 calls the
313-byte D3D9 texture blitter at454EC0 through a private EAX record/ECX
renderer ABI. The slots are the existing renderer four40-byte prefix entries.
After a managed-texture check and batch flush, the child acquires backbuffer
and texture level0, **copies from backbuffer into texture surface** with
source rectangle fields at command +8 and destination fields at +18 (filter2),
marks the texture dirty after success, and releases both surfaces. The 313-byte
native has113 instructions; reloading document/texture entries after each
opaque COM call rather than keeping the texture pointer cached recovers a
312-byte /113-instruction focused VC9 /GL candidate. Two RECT stack slots and
last reload codegen remain non-exact. See `ANIMATION_SURFACE_COPY.md`.

The source-open 976-byte billboard boss at450700 now has C++ that projects a
translated origin and camera-right vector, bounds-checks normalized depth,
derives a projected half-size and rotates an anchored four-corner screen
quad. The first real-parent /GL build yields927 bytes versus native976;
native paired FSINCOS and float evaluation lifetimes remain open. See
`ANIMATION_PROJECTED_QUAD.md`.

The two outstanding animation renderer **update-chain** callbacks `455120`
and `455130` also reconstruct exactly under the genuine constructor producer.
Each is a 7-byte `MOV EAX,ECX; JMP` adapter to the larger update routines
`455480`/`455560`; independently strict linked-PE proof replays one REL32 field
per owner. They were absent from the earlier set of 27 draw callbacks.
See `ANIMATION_RENDERER_LIFECYCLE.md`.

The secondary ECL program browser at438AC0/934 and its 438570/83 filename
cleanup now have source and reviewed physical selection fields. The native
934-byte state machine has267 instructions/22 calls; a focused VC9 /GL
independent-caller family gives859/249/22. That selection-init gap is now partially closed: a native-aligned
post-store count reload and three-way split reconstruct the full267 native
instructions in a scoped912/934-byte VC9 /GL candidate. The large browser
still has hundreds of byte differences and remains NON-EXACT. See
[SECONDARY_PROGRAM_BROWSER](SECONDARY_PROGRAM_BROWSER.md).


The 83-byte filename cleanup `ClearSecondaryProgramBrowserFiles` at438570 is
now strict VC9 /GL exact. A saved `char *filename` local before the null test
restores native MOV/TEST/PUSH rather than CMP-memory/LEA/MOV; both original
CRT free calls at 45FC3B are independently bound. The larger934-byte ECL
browser retains its 859-byte /22-call diagnostic and remains non-exact.
See [SECONDARY_PROGRAM_BROWSER](SECONDARY_PROGRAM_BROWSER.md).

Original callback at438900 was missed by Ghidra's initial function inventory.
The browser's actual registration at438CBE takes its address, and the complete
8 bytes are CALL438910, XOR EAX,EAX, RET. Natural VC9 /GL linked code is
exact 8/8 with one reviewed REL32, independent of the still non-exact 277-byte
`LoadSecondaryProgramSelection` body. The bigger934-byte browser has been
corrected to register this callback. The separate83-byte filename cleanup
remains exact after a scoped cold recheck. See
[SECONDARY_PROGRAM_BROWSER](SECONDARY_PROGRAM_BROWSER.md).


The 934-byte ECL browser now uses the actual post-store selection count
reload and separate zero/positive/negative initializer branches. Its real
VC9 /GL narrow body grows from859/249 instructions to912/267, equal to
all267 native decoded instructions while retaining22/22 calls. It is not
byte exact (659 nonfield diagnostic differences). The shared83-byte file
cleanup stays strict exact after a scoped cold replay. See
[SECONDARY_PROGRAM_BROWSER](SECONDARY_PROGRAM_BROWSER.md).


The ECL browser deferred loader438910 now compiles to full native277 bytes
and81 instructions under the real `ResourceFile` archive/critical dependency
producer, with8/8 calls. Changing its filename storage from256 to MAX_PATH260
restores native `SUB ESP,108`; expressing an explicit 0/positive/negative
selection rule restores original code length. Only18 nonfield bytes remain,
mostly the ordering of constructor vptr write and the final EBX/ECX index
register allocation. Resource loader466/466 remains exact and the separately
accepted 8-byte thread caller still passes a scoped strict cold replay.
See [SECONDARY_PROGRAM_BROWSER](SECONDARY_PROGRAM_BROWSER.md).

The `SecondaryManager` 77-byte factory at45DB90 now reproduces every native
instruction and three reviewed fields in a real browser+`SecondaryScript`
VC9 /GL producer. Its two embedded `SecondaryVm` stack-zero stores require
`stack.top=0; stack.frame=0;` in that order; the previous chain assignment
reversed the two writes. The full distinct factory is cold linked exact;
see `SECONDARY_PROGRAM_BROWSER.md`.


Mode4 of the 934-byte browser has a second native manager-null check after
script completion, not merely a delete nested under the earlier guard. A
natural local snapshot plus independently reread manager field restores it
under the real browser/debug caller+manager producer: 915/259 instructions
becomes928/262 with22 original calls, and positional nonfield differences
drop629 to412. Both exact browser dependents, the83-byte cleanup and77-byte
manager factory, cold replay unchanged. See `SECONDARY_PROGRAM_BROWSER.md`.

The 548-byte secondary ECL browser ASCII overlay at438E70 and adjacent
10-byte draw-chain adapter at4390B0 now have natural C++ sources. The overlay
contains130 complete instructions, 11 calls and three returns, rendering
file/ECL/quit/position state via the existing AsciiManager. Narrow VC9 /GL
produces504 bytes/11 calls; compiling actual ASCII print dependencies changes
that context to445 bytes/10 calls. Neither is exact. The distinct size getter
at4390D0 returns0x5B94 and is **6/6 strict exact** with no fields. The
header-dependent83-byte cleanup, 8-byte loader callback and77-byte manager
factory all rechecked exact. See `SECONDARY_PROGRAM_BROWSER.md`.


The 548-byte ECL ASCII draw owner at438E70 must reload g_AsciiManager
after opaque AsciiManagerPrint calls. Retaining its pointer throughout the
function causes VC9 /GL to keep it in ESI and use EDI for the browser, losing
one of the native11 calls when real ASCII support is compiled. Removing the
long-lived cache naturally recovers native browser-in-ESI, the complete x87
opening position setup, and11/11 calls: the real-ASCII candidate improves
from445/548 (10 calls,298 nonfield differences) to526/548 (11 calls,262
nonfield differences). The related six-byte size getter remains strict exact.
See `SECONDARY_PROGRAM_BROWSER.md`.

Another ECL browser owner missed by initial Ghidra function discovery:
4390A0..4390A4, a complete five-byte update-chain tail JMP to438AC0.
The real registration stores its address at4386A2. With the maintained
934-byte update source visible to VC9 /GL, the natural five-byte forwarding
body is strict linked-PE exact with the sole REL32 at+1; the neighboring
six-byte size getter independently rechecks exact. See
`SECONDARY_PROGRAM_BROWSER.md`.

The browser draw callback4390B0 is now fully **10-byte exact**. Natural
`PUSH ESI/MOV ESI,ECX/CALL 438E70/POP ESI/RET` appears only when the real
update/draw-chain registration producer at438660 and the large ASCII drawing
owner are visible together to VC9 /GL. The full 173-byte registration is **now strict exact**, all60 instructions,
four calls and six reviewed fields. Its initial159/153-byte diagnostics were
caused by over-specialization when no independent dynamic-priority chain
caller was present; linking that caller naturally restores native EBP/EBX/ESI
ABI. Splitting `link.next=0; link.previous=0;` repairs the two remaining
constructor store-order differences. It binds priority5 callback4390A0 and
priority44 callback4390B0. See `SECONDARY_PROGRAM_BROWSER.md`.

The Ghidra-missed ECL browser storage constructor4385D0..43865D is now
**142/142 strict exact** in natural VC9 /GL C++. Its 32 instructions preserve
the worker initializers, three selector initializers and final 0x2E4-byte
memset/global publication order. All three fields (worker table4978D0,
memset46A720, browser singleton4A8EC4) replay exactly. It directly precedes
the independently exact 173-byte chain registration. See
`SECONDARY_PROGRAM_BROWSER.md`.

The complete 439-byte `DestroySecondaryProgramBrowser` at438710 now has
natural C++ source. Full original PE/Ghidra review shows116 instructions,
16 calls, one RET4 and an SEH frame. Cleanup is files -> chain update/draw
with separate optional locks -> raw ECL -> program -> manager -> ANM slots6/5
-> singleton -> worker thread. Scoped VC9 /GL has455/439 (112 instructions,
16 calls) with external dependencies or372/439 (91 instructions, 16 calls)
with real helper TUs; EH and ESI receiver ABI remain open. See
`SECONDARY_PROGRAM_BROWSER.md`.


The browser destructor438710/439 now expresses the nontrivial worker-member
lifetime as a scoped C++ destructor, not a manual final call. A separate
independent call probe that lets its stdcall function address escape restores
original SEH entry and RET4 rather than producing a specialized ECX/RET.
The real 16-call producer now yields448/439 bytes and118/116 instructions,
still non-exact (233 non-address-field positional differences). See
`SECONDARY_PROGRAM_BROWSER.md`.


The previously open 548-byte ECL browser ASCII overlay at438E70 is now
**strict full exact** with all130 native instructions and11 call sites. A
fresh distinct local `float position[3]` in each case1/2/3 restores
the target's x87 redundant XYZ initialization/stack lifetimes; earlier
526/548 and 123/130 candidate was non-exact. The full primary-TU source
passes independent pinned VC9 `/GL` cold-linked PE/MAP/PDB replay, with
all43 fields verified: 11 calls, 12 AsciiManager globals, nine native
string literals and 11 byte-exact float/double literals; zero differences.
The 10-byte draw callback and 6-byte size getter retain separate exact units.
See `SECONDARY_PROGRAM_BROWSER.md`.

`StartSecondaryDebugThread` at4594A0 is **56/56 strict exact** under a
natural `/GL` dynamic-callback producer and genuine exact stop-thread source.
Its actual ABI passes worker in EAX, callback in EDI, context on stack, and
returns with RET4; two reviewed REL32 calls go to StopRuntimeThread459430
and CRT __beginthreadex46184B. It is the real thread-start dependency of
934-byte ECL browser update. See `SECONDARY_PROGRAM_BROWSER.md`.

The 30-byte six-instruction `SetSecondaryGameplayRuntimeMode` helper at40DEA0
is now **strict exact** with no relocations. Its actual generic EAX runtime,
ECX mode ABI requires an independent dynamic-mode user; literal mode3 in
the 934-byte ECL browser would otherwise be specialized away by VC9 /GL.
When combined with real 56-byte worker-start and 101-byte stop sources,
the frontier unit `secondary-program-browser` reproduces the
large browser at928/934 bytes and262/267 instructions (all22 calls), with
nonfield mismatches reduced from412 to173. That browser remains non-exact.
See `SECONDARY_PROGRAM_BROWSER.md`.

The 313-byte `CopyAnimationTextureSurface` owner now uses a short-lived
texture-table lookup only after a successful backbuffer-to-texture D3DX
copy, preserving every COM/RECT direction and release. Its real parent plus
screenshot producer (frontier unit `animation-surface-copy`) creates a
311/313-byte 112/113-instruction candidate with all8 native calls and46
positional nonfield differences, vs the old312-byte/55-difference candidate.
The missing instruction is in the final indexed texture AddDirtyRect setup;
this remains non-exact. See `ANIMATION_SURFACE_COPY.md`.


The 135-byte `UpdateAnimationDocumentSlots` scan at4548D0 is now **strict
exact**. Its formerly lone ModRM byte +70 (EAX versus target EDI) is
naturally recovered when the real 811-byte decoder dependency TUs are
linked alongside the scanner and `AdvanceAnimationDocument`. All135 bytes,
50 instructions, three calls, two returns, and four native DIR32/REL32
fields now match, independently cold accepted. This proves the non-exact
212-byte advance callee's private EDI input without asserting that callee
is exact. See `ANIMATION_DOCUMENT_SLOTS.md`.


`AdvanceAnimationDocument` at4544C0 is now **212/212 strict exact** under
real 811-byte decoder + 135-byte exact document slot scanner production.
Its original EDI document receiver, five argument decoder call, first four
DWORD stack locals, 32-bit decoded flag, sprite/script prefix order and
non-volatile direct CMP of busy+124 restore all74 instructions. Three native
DIR32/REL32 fields are independently reviewed and the full primary-TU VC9
/GL PE/MAP/PDB cold replay reports zero differing bytes. The exact 135-byte
scanner, 811-byte decoder and51-byte slot release remain compatible in the
same linked diagnostic context. LoadAnimationChunkSource is also now strict linked-PE exact.
See `ANIMATION_CHUNKS.md` and `ANIMATION_DOCUMENT_SLOTS.md`.


`ClosePauseContinueMenu` at42D7F0 is now **92/92 strict exact** (22
instructions, two calls, four reviewed native fields). A dynamic caller
and actual `SecondaryActorAnimation.cpp`/`StageMesh.cpp` TUs recover the
native ESI receiver and two direct signal calls; standalone source without
the real callee was94 bytes due over-specialized stack argument pushes.
Full unique PE/MAP/PDB owner cold replays with zero differences. The helper
restores animation time scale, not audio. Parent3192 continuation owner is
still non-exact. See `PAUSE_CONTINUE_MENU.md`.


The 64-byte `ShowPauseMenuAnimationTree` helper at456610 is **strict exact**:
20 native instructions including a reached six-byte loop-alignment LEA,
one call and two independently reviewed DIR32/REL32 fields. The genuine
`StageMesh.cpp` finder alone specializes away the renderer and yields only
52 bytes; adding `probes/stage-mesh-context.cpp` with a dynamic renderer
restores the original EAX ID reference / EDX renderer private ABI. The
linked primary owner is byte-exact with zero differences; the 3192-byte
parent remains non-exact. See `PAUSE_CONTINUE_MENU.md`.


The title-menu thread at `0x4394F0` is now **165/165 strict linked exact**.
Its native callback has no consumed argument and uses plain `RET` even though
the caller passes its address to `_beginthreadex`. After the real animation
document destructor, the original re-reads renderer slot 1 before scalar
delete; preserving that pointer reload naturally restores ESI/EDI register
allocation and all 47 instructions. The genuine title/document `/GL`
producer and 12 reviewed address/call fields replay with zero differences.
See [TITLE_MENU_LIFECYCLE](TITLE_MENU_LIFECYCLE.md).


`TitleMenuView` construction at `0x439420` is now a strict **201/201-byte**
public primary-TU `/GL` match: 39 native instructions, one memset call and
four reviewed target fields. VC9's dynamic title-lifecycle producer recovers
the original private ESI-this/EAX-return convention. Surprisingly, the
original initializes selection records and ThreadInf before zeroing the
complete 0x5B94-byte object, then publishes flags and its singleton. This
exact match does not establish original TU partition. See
[TITLE_MENU_LIFECYCLE](TITLE_MENU_LIFECYCLE.md).


The complete title resource initializer at `0x4395A0` is now **275/275
strict exact**; its important producer context is the separate generic
priority/slot caller together with the real chain, document, log and document
load sources. A mere real-document link left two instruction-byte differences,
but constraining both dynamic priority and dynamic file slot restored all
90 native instructions/7 calls and 15 reviewed fields. The same real producer
also gives the 28-byte singleton deletion exact. The title startup at
`0x439900` is **106/106 strict exact** under the true runtime-thread stop
producer. The ThreadInf constructor + non-deleting destructor are **21/21 +
16/16**; its 36-byte deleting destructor is **non-exact owner identity**
despite identical bytes because MAP aliases the `??_E` and `??_G` entries.
See [TITLE_MENU_LIFECYCLE](TITLE_MENU_LIFECYCLE.md),
[THREADINF_LIFECYCLE](THREADINF_LIFECYCLE.md).

`DestroyTitleMenuStorage` 561 still differs in 231 position-wise nonfield
bytes under its 561-byte/15-call/RET4 real-worker context. A true C++ virtual
title class generated 559 bytes and ordinary RET, and direct record deletion
spelling or real chain removal did not help. The related 429-byte replay
record destructor still differs in exactly three ECX/EDX load pairs, 6
nonfield bytes. No unsupported match credit is assigned; probe evidence is
retained in `.analysis/probe-title-*20261112.log`.
