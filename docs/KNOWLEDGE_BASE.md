# TH11-local knowledge

## Target and tools (2026-10-23)

The operator archive's Japanese `th11.exe` is 688,128 bytes, SHA-256
`2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`.
Pinned thcrap versions.js commit `f08b582ce57bce800955dd371fc9a68dbad5b324`
classifies it as original TH11 v1.00a. Database SHA-256 is
`9ac6c0247b9c53129429f9522f66e035a7880d15082f8d519990ac7ed82c14e1`.
Target facts are parsed directly in `config/target.toml` and replayed by
`verify-target.py`.

PE linker version 9.0 and dominant Rich build 21022 select the VC9 RTM
candidate. Rich records include product/build/count `131/21022/137`,
`132/21022/50`, and `138/21022/59`; mixed older inputs also exist. Production
translation-unit partition, flags, libraries, and link order remain unknown.

Direct Ghidra 12.1.3 initial analysis exported 1,988 candidate bounding spans.
These can miss internal entries or include sparse bodies, data, and padding.
They are not an exhaustive authored-function denominator.

## ASCII text lifetime (2026-10-23)

Target appender `0x004014E0` caps the queue at 320 records, strides by `0x130`,
and starts storage at manager `+0x87C`. Count is at `+0x1847C` and per-record
lifetime at `+0x12C`. Appender stores identify 256-byte text, three-float
position, color, and two-float scale. Other fields remain unnamed.

`0x00401D50..0x00401DCB` decrements each lifetime and retains values >= 0,
compacting surviving records in order with complete 0x130-byte copies.
`0x004014A0..0x004014B5` invokes it, increments manager `+0x184A8`, and returns
1 through a receiver-in-ECX callback. Adjacent internal-ESI wrapper `0x00401480`
is not interchangeable with that callback. The partial source layout does not
claim that unknown bytes have identified owners. Two pinned VC9 cold builds
replay the complete update callback and its sole REL32 with zero differences.
The expiry source remains a 100-byte normal-COFF candidate against a 124-byte
target extent; it has no exact credit.

The RNG core `0x00458BC0` has conditional critical-section calls controlled by
`0x004C3810 & 0x8000`, in addition to the familiar 16-bit state recurrence.
TH10's lock-free RNG source cannot be imported as TH11 evidence. RNG source and
exactness remain open.


## Medium angle and input functions (2026-10-23)

AddAngles at 0x00458E30 is 157 bytes and NormalizeAngle at 0x00458ED0 is 145 bytes.
Both complete normal-COFF VC9 bodies are exact, including three independently
reviewed DIR32 bindings to float-rounded pi/two-pi constants. The shared
post-increment correction guard and float-rounding points are part of the
original behavior; see `ANGLE_MATH.md`.

UpdateInputRepeatState at 0x00459B50 is 105 bytes and raw-equal after cold VC9
compilation. It handles 32 hold counters, held-at8, repeat-at26/every8, and
pressed/released masks without publishing previous. See `INPUT_REPEAT.md`.

The input caller passes its keyboard mask in ECX to 0x00457270 and consumes its
EAX return. The actual successful cookie-check body at 0x0045FC2C preserves EAX.
Default Ghidra cookie return inference was removing live mask-building code;
temporary u32/fastcall and builtin cookie-fixup hypotheses recover it without
saving metadata or patching bytes. `INPUT_POLLING.md` records the two larger
source candidates and their remaining codegen differences.

The operator now prioritizes useful medium-size functions and focused replay
of only changed units/dependencies. This batch rebuilt the new AngleMath object
once for its two units and the new InputState object once; the earlier ASCII
unit was not replayed. Ghidra and Wine commands share one CPU affinity and the
same serialization lock.
