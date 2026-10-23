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

The complete182-byte appender now passes the pinned cold linked-PE Oracle
without linkages or byte normalization. Ordinary stdcall source plus two
independent calls under `/O2 /Ob1 /GL` naturally yields manager-in-ESI,
position-in-EBX and text-in-ECX. It leaves record+0x118 untouched; the style
copy order reads manager+0x1848C,+0x18498,+0x18494,+0x1849C. See
`ASCII_LIFETIME.md` and canonical unit `ascii-manager-append-string`.

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

## Keyboard polling exactness and joystick corrections (2026-10-23)

PollKeyboardInput at 0x004576B0 now has a complete 888-byte canonical VC9
normal-COFF cold proof, including thirteen independently reviewed relocations.
Separate input-lost and other-error branches are necessary for the target
layout. Both key maps, keyboard gate, joystick merge, mask publication and
repeat-state update are covered. See `INPUT_POLLING.md` for binding evidence.
Only this changed unit was cold-replayed in this batch.

The 812-byte joystick target uses separate JOYINFOEX and 8-byte-aligned
DirectInput buffers. Its input-lost retry call pushes the counter and
`"error : DIERR_INPUTLOST %d\r\n"`; these arguments were missing from the earlier
source view. The improved candidate is 815 bytes but remains non-exact. A
same-TU empty trace sink removes the observed call and was discarded; the
callee's production context remains open.

## File check and paired byte transforms (2026-10-23)

FileExists at 0x004585E0 has a complete 141-byte canonical normal-COFF cold
proof. All fourteen DIR32 fields, including four independently parsed KERNEL32
import slots, replay exactly. Its conditional critical section at 0x004C3AC0
and byte counter at 0x004C3BB2 are retained on both exit paths. This batch
cold-built this new unit and the changed keyboard unit only.

ScatterXorBlocks at 0x004581C0 and GatherXorBlocks at 0x004582E0 each have a
reviewed 284-byte span and maintained conventional source. The first span
includes a four-byte internal LEA ESP alignment instruction excluded from its
280-address Ghidra body. Each normal VC9 candidate is 276 bytes and remains
non-exact; target key-in-AL/five-stack-argument ABI and origin are unresolved.
Each candidate passed 692 bounded target-vs-VC9 Unicorn execution cases using
shared deterministic library shims. This is focused diagnostic evidence, not
native product or semantic phase acceptance. See `BYTE_CIPHER.md`.

## Callback chains and complete table ownership (2026-10-23)

ExecuteDrawChain at0x00456E10 is now exact across its complete284-byte owned
normal-COFF extent, including one alignment byte and the six-entry switch
table omitted by the initial259-byte Ghidra body. All24 relocations have
independently reviewed bindings. UnregisterChainElement at0x00457080 is exact
across114 bytes including its sole operator-delete call. Only these two new
units were cold-replayed; earlier exact units were not rebuilt.

ExecuteUpdateChain's reviewed owner is352 bytes, including its nine-entry
switch table and twelve alignment bytes. The ordinary member candidate is348
bytes and remains non-exact; target manager is an internal EBX argument.
New ordered update/draw registration source covers two147-byte target bodies;
conventional source159 remains non-exact because ESI/EBX arguments need real
linked context. Each registration passed560 focused target-vs-VC9 diagnostic
cases; dispatch diagnostics passed220 draw and440 update cases. See
`CALLBACK_CHAIN.md` for enabled/null counts, return codes, pause semantics,
callback context and lock behavior.

A private `/GL /Ob1` caller probe naturally transformed ByteCipher's standard
six-argument declarations into the observed key-in-AL/five-stack-argument
convention. No manual ABI was imposed. Both linked bodies remain293 versus
target284 bytes; this is compiler evidence only, not exact acceptance. Probe
source/results remain under `.analysis/cipher-caller/` and
`build/probe-ltcg/cipher-caller/`.

## Joystick button capture and VC9 linked acceptance (2026-10-23)

PollJoystickButtons at0x004575A0 is exact across267 bytes using a cold /GL
linked-PE context. Its returned array at0x004C98A0 is directly consumed by the
input-configuration caller0x0043BB39; default Ghidra void inference was wrong.
WinMM expands32 bits; DirectInput copies128 bytes after GetDeviceState even
on error and does not initialize that source buffer. Failure Poll performs
up to400 extra input-lost acquisitions without the gameplay trace call.

The new public linked Oracle requires source/tool/producer/artifact receipts,
matching complete PE/MAP/PDB identity and primary-TU owner, exhaustive fields
and zero differences. Thirteen fields bind this function. Twenty-four synthetic
controls exercise its acceptance boundary. Specialized static/EH/split contexts
remain open; original TU ownership and native product closure are not proved.
Only the new267-byte unit was cold-built/replayed. Earlier eight exact units
were not rebuilt. Details are in `LINKED_ORACLE.md` and `INPUT_POLLING.md`.

Bounded new compiler probes found that an empty trace sink in a separate
normal-COFF support TU retains its call but leaves MergeJoystickInput at815
bytes; it does not reproduce the target's preservation/scheduling context.
Natural byte-cipher loop/source-style probes also remain non-exact. Private
results stay under `.analysis/joystick-native-trace/` and
`.analysis/cipher-shapes/`; no exact credit was added for those experiments.

## Exact ordered callback registration (2026-10-23)

AddToUpdateChain0x00456B70 and AddToDrawChain0x00456C10 now each have complete
147-byte /GL linked proofs. One shared cold context builds the primary source
and a separate ordinary caller with independent unknown arguments. VC9
naturally chooses element-in-ESI and priority-in-EBX; all nine DIR32 bindings
per function replay exactly. No prior exact unit was rebuilt.

Startup evidence identifies physical runtime base0x004C3280: instance storage
and window-setup argument, D3D cells+4/+8, flags+0x590, twelve24-byte critical
sections+0x810, and depth bytes+0x930. `MainRuntime.hpp` is a partial0x93C view,
not a claim about the complete original class. Expressing the shared fields
through that view preserves the target's flag-read ordering after the final
list-link writes. Independent scalar views, volatility and profile variants
were insufficient. Source remains ordinary C/C++ without imposed registers.

The linked Oracle now has25 synthetic controls, including member-addend
binding that cannot be bypassed by compensating the target base. Details and
reproduction commands are in `CALLBACK_CHAIN.md`. Legacy standalone runtime
views in other units remain pending data-owner integration.

Bounded update-dispatch member/free-pointer/global and loop-caller probes
remain non-exact; no count volatility or artificial ABI was introduced. The
actual358-byte frame caller at0x00446080 loads the chain manager into EBX
immediately before update dispatch and clears EBX immediately afterwards.
Its complete direct-Ghidra review/decompile remain private under
`.analysis/frame-dispatch-review.txt` and `.analysis/frame-dispatch-caller.c`.

## Frame clock exactness (2026-10-23)

`GetFrameClockSeconds` at0x00446920..0x00446A23 is a reviewed260-byte game
clock with QPC and unsigned timeGetTime paths. Runtime lock/depth index5
corresponds to0x004C3B08/0x004C3BB5. Frequency is signed64-bit storage at
0x004C3DA0, counter base at0x004C3DA8 and origin double at0x004C3DE0; complete
state ownership remains open. The fallback arithmetic is exactly
`(milliseconds - origin * 1000.0) / 1000.0` and must not be simplified by
assuming a different origin unit. The high-resolution path unlocks before
reading/subtracting the origin.

A shared double local declared before LARGE_INTEGER and the comparison
`origin > seconds` naturally produce the observed floating-point flow.
Normal COFF then differs only in three stack allocation immediates16/20;
ordinary independent calls under VC9 `/O2 /Ob1 /GL` generate the target's20.
Canonical linked unit `get-frame-clock-seconds` passes a cold complete PDB
owner proof and all26 DIR32 bindings with zero differences. The appender and
clock additions replayed only their own new units. See `FRAME_CLOCK.md`.

## Random and log exactness; screenshot source (2026-10-23)

`RandomNext16`0x00458BC0 (105 bytes) and `RandomNext32`0x00458C30 (153)
pass cold complete linked-PE proofs. A16-bit seed is at receiver+0, count at+4;
locking uses MainRuntime section/depth index10. The32-bit result concatenates
two intermediate values before their two-bit rotations; stored state is the
second rotated value. It is not equivalent to two16-bit calls. Reading the
first seed before incrementing count reproduces the target's scheduling.
Each function has eight exhaustive DIR32 bindings.

`AppendLog`0x00458A10 (211 bytes) passes twelve bindings including stack probe,
vsprintf and security cookie handling. Ordinary cdecl variadic source under
VC9 /GL naturally moves the receiver to ECX while retaining stack format and
varargs. It returns the original format pointer. It uses an8192-byte temporary,
owner end pointer+0x2000 and strict capacity comparison against owner+0x1FFF;
locking uses index3. See `RANDOM_AND_LOG.md`. Only these three new canonical
units were cold-replayed for this checkpoint.

Screenshot capture0x00429CA0 (524 target bytes) and hotkey0x00446880 (156) now
have maintained source and independently reviewed authored-game boundaries.
They remain non-exact: linked candidates540/156, with23 unrelocated hotkey
differences. Screenshot views reuse MainRuntime's opaque storage at+0xF4
(presentation parameters) and+0x6D0 (thread/header/buffers/filename); unrelated
headers/proofs are unchanged. Preserve observed default-format early return,
allocation paths, bottom-up BGR conversion and async writer launch. See
`SCREENSHOT.md` for exact offsets, limits and bounded failed contexts.

## Shared file protocol and recovered worker (2026-10-23)

The229-byte `_beginthread` callback at0x00429BB0 was missing from initial
Ghidra function inventory. A bounded read-only PseudoDisassembler span query
reviewed complete flow, callback DATA reference, one RET, padding and absence
of existing owners/interior references. The curated inventory now has1989
candidates; the Ghidra database itself is unchanged. `review_span` labels the
requested range as a hypothesis, not automatic extent acceptance.

`ScreenshotWriteThread` writes14/40/0xE1000 bytes, closes, frees and nulls
info/pixel buffers and clears thread state. It preserves all later writes
after a header short-write cleanup. It and `OpenSharedFileForWrite`180,
`OpenSharedFileForRead`180 and `WriteSharedFile`88 pass complete primary-TU
cold linked proofs. Four new units/two families replay68 fields and677 bytes
with zero differences; previous sixteen canonical proofs were not replayed.

Handle0x004A34E0 starts invalid. Section/depth index2 is retained across
successful opens and released on close/failure. Read mode uses sequential
scan and still emits the observed write-error message. Write checks byte
count rather than BOOL, returns-1/-2/0 and does not reset the stored handle.
Source `size != written` preserves target operand order. The ordinary source
naturally generates ESI filenames and EDX buffer/ESI size. See `SHARED_FILE.md`.

## Resource load and complete-file write exactness (2026-10-23)

`LoadResourceFile`0x00458400 (466 bytes), `WriteWholeFile`0x00458670 (297)
and required `FindArchiveEntry`0x004418D0 (53) pass complete cold public
primary-TU linked proofs. Three new units replay66 independent fields and816
bytes with zero differences; preceding twenty proofs were not replayed.

Zero load mode uses archive0x004C9680; nonzero mode uses disk. Archive miss
sets a supplied size to zero, disk-open failure leaves it unchanged. The
second delimiter search restores the original path on failure rather than
retaining the first basename. Extraction and ReadFile status are ignored as
observed. Assigning lookup zero only after failure and placing the common
failure joins before disk success recover the target's register/block layout.

Complete-file writing uses a local handle and three status paths. Ordinary
stdcall source produces filename-in-EDI, size-in-EBX and buffer on stack.
The loader naturally has filename-in-EAX and two stack arguments. The lookup
has archive-in-EAX and name-in-EBX; no custom register ABI was imposed.

Maintained archive extraction uses16-byte records, adjacent-entry packed
length, a partial virtual stream with Read+8/Seek+0x18 and eight12-byte cipher
rules at0x004A3480. Its source310/target300 remains non-exact; generic runtime
unlock37 differs in one SIB byte. Observed buffer ownership/free conditions
are preserved. See `RESOURCE_FILES.md` for complete paths and bounded failed
contexts, including the need to retain non-global archive caller visibility.
