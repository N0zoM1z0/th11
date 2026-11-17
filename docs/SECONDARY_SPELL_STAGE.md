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
