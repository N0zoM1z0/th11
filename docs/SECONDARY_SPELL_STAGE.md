# Secondary spell stage update (TH11 1.00a)

The 624-byte native owner at 0x40C0A0..0x40C30F now has natural
C++ source in `src/SecondarySpellStageState.cpp`. Ghidra verified
the complete native body (621 instruction bytes and 3 in-function
alignment bytes), six direct calls, all returns and no foreign
interior references. This is a full source reconstruction,
**not a byte-exact producer**.

The physical `SecondarySpellStageView` has two animation VMs at
+0x10 and +0x444, three follower animation IDs at +0x878,
a tracked-position animation ID at +0x884 and the stage timer
at +0x888. Its flags live at +0x8E0, score fields at +0x8E4..8EC,
the active-frame counter at +0x8F4, and its interpolated position
at +0x910.

The original update returns 1 even when spell flag bit0 is clear.
When active, it increments the frame counter and, once the timer
reaches 60, clears the primary stage's +2FF0 bit0. At age 300
and without spell flag bit3, it reduces the score using signed
divisions and rounds downward *toward zero in tens* via the
signed C++ remainder. No divide-by-zero guard was invented.

It then executes the two embedded animation VMs, advances the real
stage timer, and tests the newly advanced age against 120.
Without flag bit2, player Y below 96 signals event3 to each live
follower VM and related children, then sets that bit. With bit2,
player Y above 128 signals event2 to those VMs and children,
then clears the bit. Each lookup uses the actual renderer VM list
and each child traversal is conditional on VM+18 being null.

The updater reads the real first scene actor at master+1C,
samples its world position at actor+1070 and computes a rounded
three-axis delta. It moves stage position +910 by 0.05 of that
delta, with the native double multiplication and float stores,
then publishes it to animation ID +884 through the existing
`PositionSecondaryAnimation` function. Finally it clears spell
flag bit5 if the auxiliary-stage singleton at 4A8D64 is inactive.

The original register ABI is a private VC9 /GL EDI receiver; the
natural C++ call boundary and EH ownership still need a wider
producer comparison. No ASM or padding was used.

## Shared state layout for the original initializer (2026-11-17)

The formerly local `SecondarySpellStageView` physical definition now lives
in `src/SecondarySpellStage.hpp`, while its 624-byte update code stays
in `src/SecondarySpellStageState.cpp`. This makes the same real state
view available to the source-open **1,777-byte initializer** at 40C650.
In that native initializer, the spell name is copied into the
0x40-byte field at +0x89C and the signed spell index is stored at
+0x8DC. Both have compile-time offset assertions in the shared header,
along with VM, timer, follower IDs, flag and position offsets.

Only the updater is mapped as source-complete. The header declaration
for `InitializeSecondarySpellStage` does **not** imply the 40C650
initializer has been reconstructed; its score-history writes, multiple
VM binding owners and seven-way stage-configuration switch still need
their own complete C++ recovery. No exact credit is added by this
layout extraction.

## Complete stage-spell initialization boss (0x40C650, 1,777 bytes)

The original initializer at `0x40C650..0x40CD40` now has natural C++
source in `src/SecondarySpellStageInitialization.cpp`, and the
624-byte update owner shares its typed physical layout through
`src/SecondarySpellStage.hpp`. The function extent has 1,769 Ghidra
body bytes and 8 internal alignment bytes. The original VC9 compiler
places its **seven-entry jump table** at `0x40CD44..0x40CD5F`,
immediately *outside* this function boundary; no table data or
instruction padding was inserted into the source.

It resets the timer at stage+0x888 to frame -1, stores the spell
index at +0x8DC, copies the NUL-terminated name at +0x89C, and
updates the +0x8E0 flags. Unless the record mode at global 4A8EB8+10
is one, it records the spell name and increments a capped 99,999
attempt count in two separate native records. The first record
uses `(selection0*3+selection1)*0x68D4 + index*0x90 + 0x66C`;
the second uses `index*0x90+0x27B64`. The native performs
unbounded string copies; no new truncation or sanitization is added.

The initializer clears stage flags bit5, then sets it only if
the auxiliary owner at 4A8D64 is active and the selection pair is
not mode five. It clears bit6, starts the active counter at one,
creates follower animation IDs for scripts 1, 0x4A and 2 from
their observed independent files, looks up the name VM, renders
the name with the real formatted-text helper and queues sound 14.
Another positioned ID uses scene script 0x8B and the original
scene actor's position. The function locates its two embedded child
VMs with signed script IDs 0x89 and 0x8A and publishes the user
duration at child+0x3BC, without inventing fallback VMs when missing.

The score is set from `(level+difficulty)*1,000,000`, with its
initial copy clamped only above 99,999,999. The function also
creates the native script 0x95 animation and dispatches seven
difficulty cases: case1/2 bind scripts 7/8; case3 binds 8/9;
case4 binds 12/13; case5 binds 16/17; case6 selects 21/22
or 18/17 depending on progress below 24; case7 selects 11/12
or 7/8 on the same threshold. Each branch creates its own
overlay script (10, 11, 15, 19, 20, 23 or 13) only under
the original spell-index and progression conditions.

The source preserves real VM reset/bind and allocation calls
and compiles under pinned VC9. Its public source signature is
provisional because the native EAX string and RET8 form a private
whole-program calling convention. No byte-exact claim is made;
actual linked /GL caller ownership and relocation comparison
remain to be resolved.

## Stage-spell HUD drawing (0x40C4A0, 370 bytes)

The native 370-byte `DrawSecondarySpellStage` owner now has a
complete source body in `src/SecondarySpellStageDrawing.cpp`,
sharing `SecondarySpellStageView` with the 1,777-byte initializer
and 624-byte updater. The source is natural C++ and compiles under
the fixed VC9 toolchain; this owner remains nonexact.

Only when stage spell flag bit0 is active does it draw the two
embedded VMs at +10/+444 and look up follower VM ID +880. An
absent VM zeros that ID and returns one. With a live VM, the
function prepares the **existing** AsciiManager at global 4A8D58:
sets style flags +1849C=1, +18498=2 and copies the VM byte at
+377 into manager alpha +18483. It then draws the score at
(265,51,0) with `%.8d` if spell flag bit1 is set, or the
currency marker `$` at (279,51,0) if not.

The historical counter line uses (360,51,0) and the native
format `%.2d/%.2d`. The two counters are read from
the shared record table at global 4A8EBC, using index
`(selection0*3+selection1)*0x68D4 + spellIndex*0x90`
and offsets +6EC/+6F0. The manager style fields are reset to
zero afterward, alpha becomes 0xFF, and all paths return one.
The original call sites and the three native callees were
reviewed; no extra null guards or fake render methods were added.

## Scoped native-compiler comparisons (2026-11-17)

Two new diagnostics build only their affected source owners and a
live dynamic caller under the pinned VC9 RTM `/O2 /GL` context:

| Unit | Native span | Current PDB contribution | Normalized native instruction keys |
| --- | ---: | ---: | ---: |
| `secondary-spell-stage-init-frontier` | 1,777 B | 1,560 B | 115 / 468 |
| `secondary-spell-stage-draw-frontier` | 370 B | 384 B | 65 / 89 |

Both are **nonexact frontiers**. The initialization owner's private
EAX spell-name argument, register allocation, seven-branch producer
partition and real animation/script callees have not yet been
recovered in a native-equivalent whole-caller link. The original
seven-entry jump table begins at 40CD44, outside the 1,777-byte
function extent; adding it to the source owner would corrupt the
comparison.

The drawing owner originally built a single three-float text
position before its branch. This produced 352 candidate bytes and
53/89 aligned native instruction keys. The source now initializes
the 265/279,51,0 text position separately on its two print paths,
then publishes 360,51,0 for the history counters. This matches the
original branch-dependent sequence of float stores; the local
linked result has 384 bytes and 65/89 aligned keys. A separate
`const int` spelling experiment changed no instructions and was
not retained. Both results need exact original register/argument
ownership before any strict byte claim.

The shared `SecondarySpellStageView` is now checked for its complete
0x91C-byte prefix size as well as its critical member offsets.
Neither test runner performs the full cold replay cohort.

## Natural signed-decimal result checksum (0x40BB50, 80 bytes)

`src/SecondarySpellRecordChecksum.cpp` reconstructs the complete
80-byte record-checking leaf. It reads the signed 32-bit result code
from spell-stage+0x90C and checks whether

`code / 100000 - 22` differs from
`((code / 100) % 100 + 34) % 100 + (code % 100 + 67) % 100`.

All divisions and remainders retain the original **signed** C++
rules. The native function receives its physical stage pointer in
EAX (private VC9 register allocation), has one caller at 40C461
and contains no external calls or embedded tables. Ghidra reviewed
the full 80 bytes and their neighbors. A separate pinned VC9
normal-COFF diagnostic is 102 bytes with no relocations and does
**not** match; this leaf is not exact.

The shared `SecondarySpellStageView` now identifies the actual
+8F0 record index, +8F8 final frame count, +8FC/904 double clock
fields and +90C record code. A scoped `#pragma pack(push,4)` keeps
those double fields at their real x86 offsets, rather than allowing
the compiler to align the first double to +900. Compile probes of
the 1,777-byte initializer, 624-byte updater and 370-byte HUD
owner all pass with this shared physical layout.

## Complete spell result timing/settlement owner (0x40C310, 368 bytes)

The 368-byte native owner is now written as
`SettleSecondarySpellStageTime` in
`src/SecondarySpellStageTimeSettlement.cpp`. It uses the previously
source-present `GetFrameClockSeconds` at 446920 and the 80-byte
`CheckSecondarySpellRecordCode` leaf at 40BB50. This owner is
called from the three native frame/clock operations at 4463EC,
446640 and 44686B; its original Ghidra extent contains no
foreign interior references.

While spell flag bit0 is active, its first call stores the current
clock in the stage's double field +8FC and sets flag 0x40.
When the spell becomes inactive and the flag was set, it
captures the active frame count at +8F8, computes elapsed seconds,
and removes the residue modulo **0.0167**. If the residue is
ordered greater/equal to **0.00835**, one 0.0167 interval is
added back. The rounded result is stored as double at +904.
The integer seconds use `floor` and an upper cap of 999; the
fractional remainder is multiplied by 100 and converted to a
signed integer without inventing a nonnegative clamp.

The encoded record result uses native 32-bit wrapping arithmetic
with `(hundredths+seconds)*1000`, adds
`seconds+0x5632`, multiplies by 100, and adds signed
`(hundredths+33)%100`. The double at +904 is then reset to
zero, the active clock bit0x40 is cleared, and the result goes
to +90C. In a normal run it is also written to the native
difficulty-indexed record table at global 4A8EB8, offset
+3C plus index*4. In replay mode (global 4A8E88+74 nonzero),
the result is loaded from the alternate table and checked
with the 80-byte leaf; an invalid entry is replaced by
the original sentinel `0x06AE9C24`. Both paths advance the
record index at +8F0 once.

The packed physical source view retains original +8FC/+904
double offsets. The function and its dependent 80-byte leaf
compile with pinned VC9; their native private x87 scheduling
and full linked exactness are unresolved, so source coverage
does not increase the canonical exact count.

## Complete stage-spell termination (0x40CD60, 335 bytes)

The native 335-byte game owner is now reconstructed as
`FinishSecondarySpellStage` in `src/SecondarySpellStageFinish.cpp`.
The function exits immediately when stage flags bit0 is clear.
Otherwise it sets primary stage flag bit0 at +2FF0, signals events
through *all three* follower animation IDs at stage+878/87C/880,
clears the active and pending stage spell bits (mask `~0x21`),
releases the positioned animation ID at +884 and zeros that ID.

When the stage spell was a success (flags bit1 remains set), the
real global score receives signed stage score/10, with 32-bit wrap
and upper clamp at 999,999,999. The original 41F010 score-feedback
owner is called using the stage's unscaled score. Unless the
spell record status has mode 1, two different completion counters
at record +80 are incremented independently up to 99,999:
the character/mode-specific record at
`4A8EBC + (selection0*3+selection1)*0x68D4 + spellIndex*0x90 + 0x66C`
and the generic record at
`4A8EBC + spellIndex*0x90 + 0x27B64`.
The success path queues sound 0x2D; the failure path calls the
score-feedback owner with zero without incrementing either record.

The original target's complete contiguous 335 code bytes, one
incoming caller, four actual callees, returns and neighboring
alignment were checked independently. The 41F010 feedback routine
is its own code owner with a native jump dispatch; this source
does not replace it with fake bytecode. The pinned VC9 TU compiles.
Private register allocation and strict full-span equivalence are
not yet established, so this is source coverage rather than exact.

## Owning stage spell registration and callback pair

The original separate owner behind the physical 0x91C-byte spell stage
view has now been modeled with natural C++ `SecondarySpellStageOwned`
in `src/SecondarySpellStageOwned.hpp`. It has two genuinely
constructible/destructible `StageEmbeddedVm` members (0x434 bytes
each) starting at +0x10, followed by the same follower IDs (+0x878)
and timer (+0x888) that the updater, initializer and HUD sources
already use. The complete owning allocation is 0x924 bytes; no new
singleton storage is introduced.

`RegisterSecondarySpellStageChains` at 40BD00..40BDFF (256 bytes)
is restored in `src/SecondarySpellStageChains.cpp`. It allocates
two native-size 0x24 chain elements, sets the observed flags and
callbacks, registers them in the update and draw chains at priority
**22** and **9**, stores the two nodes at owner+8 and +C, then
sets the timer at owner+888 to frame -1. It always returns zero;
the target's lack of allocation-error checks is retained. The
C++ source uses the existing `ChainElement` constructor fields
and real `AddToUpdateChain`/`AddToDrawChain` boundaries.

The registration code points to two distinct callbacks. The
10-byte native `SecondarySpellUpdateCallback` at 40C630 saves EDI,
forwards the context through EDI to the 624-byte updater and returns.
The 7-byte `SecondarySpellDrawCallback` at 40C640 forwards
ECX into the 370-byte draw owner by a tail jump. Both now have
natural `__fastcall` source adapters; their **original native
register-specialized machine code** is recorded as evidence, not
reproduced using ASM, an injected JMP or alignment bytes.
The 256-byte registrar is now strict exact; the two small callbacks
remain independent, nonexact owners.

## Owning constructor, destructor and lifetime wrappers

The 0x924-byte owning stage spell C++ object now has real lifecycle
source in `src/SecondarySpellStageLifecycle.cpp`. This **does not**
allocate another singleton; it publishes/clears the same 4A8D6C
object that the initializer, updater and HUD treat as
`SecondarySpellStageView`.

`CreateSecondarySpellStage` (40BF90, 206 bytes) uses
`new SecondarySpellStageOwned`. Its **two genuine
StageEmbeddedVm members** are constructed as normal C++ array
members before its constructor clears the entire 0x924 bytes,
sets bit1 at +0 and publishes the singleton. The target
similarly resets VM constructor state after the vector constructor
runs; the original behavior is intentionally preserved, not
"fixed" by avoiding the initialization.

It then calls the real two-chain registrar (40BD00) even if
allocation returned null. On a nonzero registrar result, it
destroys and deletes the owner and returns null. The target's
compiler-generated SEH/vector-constructor handler code and original class
identity are separate from the now-exact 206-byte public factory body.

`SecondarySpellStageOwned::~SecondarySpellStageOwned`
(40BE50, 317 bytes) releases each of the three follower VM
IDs at +878/+87C/+880 in order, zeroing each; it independently
unregisters chain pointers +8 and +C. Each non-null chain
removal checks runtime flags bit0x8000, enters the true runtime
critical section0 and increments its depth, unregisters,
**rechecks the runtime flag** and then leaves/decrements.
The original destructor clears global 4A8D6C before the
compiler-generated array destructor frees the two embedded VMs'
native storage400 pointers. This is naturally expressed by
the owning C++ destructor body and the member-array destructor,
not handwritten EH instructions or padding.

The source also restores two distinct, previously unmapped
native entry points. `DeleteSecondarySpellStageReturningPointer`
(40C060, 18 bytes) deletes the given stage object and returns
the previous pointer value as the native wrapper does.
`DestroySecondarySpellStage` (40C080, 28 bytes) captures the
global pointer once, skips null, otherwise destroys and deletes
it. Both source bodies are ordinary C++ and maintain independent
function boundaries. The factory is now canonical byte-exact;
the 317-byte destructor and both delete wrappers remain nonexact
pending their own complete proof.

### Strict exact chain registration (0x40BD00, 256 bytes)

The source-level `SpellStageChainElement` constructor was corrected to
write its `priority` field *after* the three callback pointers, matching
the original's observed member initialization order. It still uses
ordinary C++ and the existing chain registration APIs; no bytecode,
padding, naked function or ASM wrapper was added.

In the scoped, real-caller VC9 /GL context, the complete
`RegisterSecondarySpellStageChains` PDB contribution is **256 bytes**
and has **all 76 native instructions aligned**. The selected,
independently cold-built strict linked-PE replay proves all
**256/256 bytes** equal to original Japanese TH11 v1.00a, with
seven individually reviewed native relocation fields:

| Target field offset | Relocation | Native destination |
| --- | --- | --- |
| +05 | REL32 | operator new at 45FCE4 |
| +42 | DIR32 | update callback at 40C630 |
| +53 | REL32 | AddToUpdateChain at 456B70 |
| +5D | REL32 | operator new at 45FCE4 |
| +97 | DIR32 | draw callback at 40C640 |
| +A8 | REL32 | AddToDrawChain at 456C10 |
| +DB | DIR32 | animation time scale at 4A7948 |

Strict result: `exact`, difference_count=0, with no mismatched
relocation. Run only this unit with
`scripts/repo-python scripts/replay-exact-units.py --unit register-secondary-spell-stage-chains`.
This is a canonical exact unit. Its two callback owners and
the 317-byte destructor remain nonexact; the separate 206-byte
factory now has its own full strict exact receipt below.

## Scoped constructor and destructor codegen (2026-11-17)

The native 0x40BF90 spell-stage factory is **206 bytes / 65 decoded
instructions**. In the existing `secondary-spell-stage-factory-frontier`,
an out-of-line natural C++ `SecondarySpellStageOwned` constructor emitted
only 153 bytes and aligned 40/65 native instruction keys. Declaring and
defining that **real C++ constructor** `__forceinline` makes the compiler
inline its member-array construction, full-object memset and singleton
publication in the original function. The focused VC9 /GL result now
has **206 bytes and all 65/65 normalized instructions aligned**.

The reconstructed natural C++ now has **strict full-owner exactness**.
The candidate's eleven decoded 32-bit address operands were individually
verified; the original handler pointer at +03 targets native 0x48A9DB
and corresponds to the VC9 compiler-generated `Static symbols` entry
`__ehhandler$?CreateSecondarySpellStage@@YAPAUSecondarySpellStageOwned@@XZ`.
This compiler-generated CIL EH static entry is checked against the
actual MAP section and current source owner, not inferred or masked.
The independent selected cold replay proves 206/206 bytes with zero
differences; the handler function's own code remains a separate owner.

The complete 317-byte destructor continues to be emitted as
**317 bytes**. Preserving the three individually accessed VM IDs
through a `volatile unsigned int *` physical view keeps each
original zero store between releases and improves its normalized
instruction alignment from **74/79 to 75/79**. Only two remaining
load/push pairs use EAX where the native chooses ECX and EDX.
The original C++ destructor still owns genuine destruction of
both `StageEmbeddedVm` array members. Normal compiler probes
retain this behavior; no ASM, naked functions or padding were used.

The adjacent 256-byte `RegisterSecondarySpellStageChains` remains
**strict exact** after these changes: a separate selected cold replay
still reports zero differences in 256 bytes with all seven known
relocation fields reviewed. The factory has now passed its own strict proof;
the destructor is the remaining nonexact large lifecycle owner.


### Strict exact spell-stage factory (0x40BF90, 206 bytes)

The dedicated `secondary-spell-stage-factory-frontier` unit has been
promoted from diagnostic to canonical, after successful selected cold
replay: **206/206 bytes exact, 0 differences, 65/65 native instruction
keys**. The real `new SecondarySpellStageOwned` C++ expression generates
a compiler-managed two-element VM constructor iterator, zero-initializes
the 0x924-byte object, sets the singleton and calls the exact registrar.

The original function body has **11 complete linked relocation fields**:
the owner-specific compiler EH handler at +03, security cookie +12,
operator new +29, embedded VM destructor +43 and constructor +48,
vector-constructor iterator +58, memset +6C, global singleton +78,
registrar +8B, owning destructor +99 and operator delete +9F.
The EH field is accepted only if the MAP `Static symbols` section
contains precisely one matching compiler-generated owner handler
from a CIL module. Regression tests in
`scripts/test-linked-eh-static.py` reject a false name, non-DIR32
field, wrong native target, falsified MAP provenance or duplicate
handler. Thus neither a hand-authored address override nor a
relocation wildcard can establish exactness.

Run just this selected unit:
`scripts/repo-python scripts/replay-exact-units.py --unit secondary-spell-stage-factory-frontier`.
The public owner's *body* is exact; neither the external EH handler
code nor the nonexact destructor and callbacks inherit that status.

### Exact natural scalar deleting destructor and global destroy (2026-11-17)

The 18-byte native owner at 0x40C060 is **not** an authored wrapper
that takes an ordinary C++ parameter and returns it. It is the native
VC9 scalar-deleting destructor generated automatically from
`SecondarySpellStageOwned::~SecondarySpellStageOwned`. The artificial
`DeleteSecondarySpellStageReturningPointer` source was removed; the
owning class and real `delete` expression now produce symbol
`??_GSecondarySpellStageOwned@@QAEPAXI@Z` through the compiler.
Selected pinned VC9 /GL cold replay proves **18/18 bytes exact**, with
the two complete native REL32 fields to the destructor at 0x40BE50 and
`operator delete` at 0x45FD49. The original class identity remains
unverified even though this complete generated function matches.

The independent 28-byte `DestroySecondarySpellStage` owner at
0x40C080 now also has selected **28/28-byte strict exact** proof.
It reads singleton 0x4A8D6C once, conditionally deletes it, and
returns; all three decoded fields resolve to the actual native
global, destructor and `operator delete` at offsets +03,+0D,+13.
Neither owner was reproduced with ASM or a hand-coded jump.

These two new exact receipts are separate from the 206-byte factory
and 256-byte registration receipts, which were rechecked after
the source change. The 317-byte destructor itself is still
nonexact: exact calls to a function do not establish the callee's
exactness. Use only the affected units with
`scripts/repo-python scripts/replay-exact-units.py --unit secondary-spell-stage-scalar-deleting-destructor`
and
`scripts/repo-python scripts/replay-exact-units.py --unit destroy-secondary-spell-stage`.

### Exact draw callback from the real downstream LTCG producer

A new bounded `secondary-spell-draw-callback` compilation includes
the genuine natural C++ 624-byte stage updater and 370-byte
`DrawSecondarySpellStage` in the callback's VC9 /GL producer.
This makes the ordinary C++ `__fastcall SecondarySpellDrawCallback`
compile into the original **seven-byte** tail transfer:
`MOV EAX, ECX; JMP DrawSecondarySpellStage`.

The selected strict cold linked-PE comparison proves **7/7 bytes
exact**, including the sole original REL32 field at +03 targeting
0x40C4A0, with zero differences. No user-authored machine-code
JMP, inline ASM, naked function, NOP or padding was introduced.
In the same wide producer the update callback remains nonexact:
it selects ESI where the native update call wants EDI.
The existing independently exact 256-byte chain registrar is
also reverified after this work. These are distinct owners and
only the seven-byte draw callback receives new canonical
exact credit.


### Strict exact owning destructor (0x40BE50, 317 bytes)

Using existing `AnimationVmId` value objects, ordinary C++ references
to the three follower IDs, and **no compiler barriers**, the pinned
VC9 /GL producer emits the native 317-byte destructor with all
**79/79 instructions** in native order. The previously outstanding
load/push versus ID-zero-store scheduling differences disappear when
the three values are explicitly represented as typed references.
All original 317 bytes have been verified by selected **cold** strict
linked-PE replay; there are **zero differences** after exhaustive
relocation replay. The compiler-generated EH handler at +03 is
authenticated from the MAP Static symbols section against this
destructor's mangled name, not ignored or wildcarded.

The function is now canonical exact as
`secondary-spell-stage-destructor`. The original 0x924-byte
object's three animation IDs are each released and zeroed, the
two chain registrations are independently removed with the proper
critical-section checks, global 4A8D6C is cleared, and the
compiler itself invokes two `StageEmbeddedVm` member destructors.
This also preserves separate exact receipts for the 206-byte
factory, 256-byte registrar, 18-byte scalar deleting destructor
and 28-byte global destroy. Compiler-generated EH *handler code*
is still its own unproven function extent, not included in the
317-byte owner.
