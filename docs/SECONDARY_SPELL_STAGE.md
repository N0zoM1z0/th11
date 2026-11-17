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
