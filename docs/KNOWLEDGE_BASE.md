# TH11-local knowledge

## Stage VM cleanup and resource loading (2026-10-24)

ReleaseVmBindingBlock0x00402300(231) now has a complete strict cold primary-TU
exact proof with6 independent bindings:four free calls and two renderer-cell
loads. Totals41 exact /9,125 bytes. Its signed loop visits count-1 slots; ordered
renderer lists+7B562C/+7B5634 follow earlier batch pointers+7B5624/+7B5628. First
match wins. Flag0x04000000 propagates one level through+14 only if marker+18 is
zero. Free order is+10,+14,+8,+0C; count is preserved. See VM_BINDINGS.md.

Stage loader0x00403FC0(297), document creation0x00454190(454) and acquisition
0x00454360(124) now have full non-exact source. Current candidates297/434/131
have17/337/80 diagnostic differences. The creation span includes3 omitted
alignment bytes; acquisition includes6. Signed stage counts, relative pointer
relocation, VM stride0x434, ANM chunk sums and20/72/4-byte allocations are
preserved. The0x134-byte document is published before validating loaded data;
failure can leave a cached incomplete object. Cached acquisition ignores busy.
Fresh acquisition writes busy1, sleeps before polling and can exit on flags180
while busy remains1. No timeout is introduced. See STAGE_RESOURCE.md.

The stage/file path is game-authored renderer/VM lifecycle integration; physical
views still do not prove complete original data classes. Existing texture-file
views' field+124 now has independently observed busy/wait use; old headers are
unchanged. Native trace RET preserves EAX/ECX where an opaque source call spills
them. Retain that known context gap. Opaque-helper297/24 and direct-resource/log
body297/17 families remain non-exact; do not repeat them without new evidence.

Private diagnostics agree on83 cleanup cases (67 returns,16 matching null-table
faults) and34 loader cases (33 returns,one8-Sleep persistent prefix). Models
stand in for heap/resource/decoder/thread helpers; no complete native/runtime
authority. One CPU/nice10, maximum RSS104,376/77,624 KiB. Previous40 canonical
inputs and proofs were unchanged. Current receipt-bound artifacts and active
diagnostic contexts were preserved during handoff cleanup; see ARTIFACTS.md.

## Formatted texture text and repaired centered span (2026-10-24)

Direct0x00454C60(156) and right-aligned0x00454D00(224) now pass strict cold
complete public primary-TU linked proofs with4/6 independently reviewed fields.
Exact totals40/8,894 bytes. The centered0x00454DE0 candidate was truncated at176;
full flow extends through0x00454EB3(212), including36 bytes of dispatch/dirty/GS/
RET tail. Source212 still differs in13 stack offsets from native frame0x8C
versus0x88. No padding or fake local is used. See FORMATTED_TEXTURE_TEXT.md.

The character region is132 bytes before the native cookie. An initial128-byte
hypothesis had the correct instruction lengths but wrong offsets. Explicit
default17 before the unsigned-byte height test reproduces the aligned branch;
the direct function forwards raw height0. The physical view has attachment
+3AC, dirty flags+404, text flags+408 and height byte+41C. Attachment+8 points
to a binding whose first cell is the texture; width float+38 accompanies the
existing placement floats. Full original owners/origins remain indeterminate.

Right x truncates `width - (((h-1)*byte_length)>>1)` after floating subtraction.
Center x first truncates width, divides signed by2 toward zero and subtracts
unsigned `((h-1)*byte_length)>>2`. Native x87 FTOL entry0x004864E0 has an SSE2
feature gate and x87 fallback; it does not consume ordinary stack float args.
The unsigned correction at0x00497F98 is independently checked float2^32.
Dirty OR1 occurs after dispatch even if rendering skips height1..8.

Focused private123-case helper-model diagnostic agrees on formatted bytes,
varargs, x87 values, dispatch parameters, GS cookies, flags and stack/registers,
including131 output bytes and signed fractional widths. One CPU/nice10,
max RSS106,712 KiB; no full native/runtime authority. Previous38 exact source/
header inputs and proofs are unchanged. The three functions share one source;
future centered edits require revalidating the two exact siblings only.

## Outlined texture/surface text and placement (2026-10-24)

Complete direct-Ghidra spans0x00444610(631),0x00443E00(525) and0x00454BB0(172)
now have maintained non-exact source. Native texture inputs are ECX text/EDX x/
EAX font and six stack inputs; source637 instead has a two-register private
convention/RET28. Surface530 and dispatcher173 are also non-exact; see
OUTLINED_TEXT.md for520/466/130 diagnostic differences. No old canonical
source/header was changed or replayed; old plain/filter sources compile only
as actual directly needed support. Origin remains indeterminate for all three.

Surface text skips only null strings, captures dimensions/DC before GDI calls,
and retains six independent outline RECTs with duplicate second/fourth positions.
CLR_INVALID skips those passes, then independent foreground still draws. The
texture function has no CLR_INVALID sentinel. Zero spacing draws the entire
byte string five times; nonzero spacing advances fixed two-byte groups by
twice the signed spacing, including a NUL in the final odd-length group.
An empty string makes five count0 calls in whole-string mode but none in chunk
mode. Keep both alpha passes, actual transparent-fill source and selected-object
restoration/D3DX/release order.

Placement's nonpositive font height defaults17, positive1..8 returns before
any placement reads, and9..16 reaches downstream clamp17. Four float fields
at+0C/+10/+14/+18 convert in order; zero selector chooses outline, nonzero plain.
Native FTOL receives x87 values, not ordinary stack float arguments. Its
exceptional behavior remains unvalidated. Real callers0x00454C60/0x00454D00/
0x00454DE0 still need their own semantics/source recovery.

Private full-body diagnostic123 cases agree, including finite FP conversion
inputs, byte strings, font/height gates, signed spacing, RECT writes, images,
calls, cleanup and stack/register preservation. Models stand in for GDI/D3DX/
FTOL, not the full native product. One CPU/nice10, max RSS117,400 KiB. Rectangle
addition-order and placement declaration-order probes failed; primary-only Os
with O2 supports yields425 bytes/485 differences and is not an accepted extent.
Do not repeat those configurations without new evidence.

## Text drawing and CPU composition (2026-10-24)

Direct Ghidra full spans establish compositor0x004441C0(657), drawing
0x00444890(392), transparent-neighbor fill0x00443860(872) and alpha inversion
0x00443BD0(165). All four now have complete non-exact source; sparse inversion
omits the3-byte LEA at0x00443C5D, retained in the reviewed165-byte span. Native
drawing caller0x00454BB0 supplies EAX font choice and six stack inputs. All
four origins remain independently indeterminate. See TEXT_DRAWING_COMPOSITE.md.

Compositor's LockRect requires exact zero; mismatch unlocks/succeeds while
matching unsupported format fails without unlock. A4 writes high byte0 nibble
before reading its now-zero low nibble, then derives alpha from the unusual
`(old_byte1>>2)&0x3C` expression. A8 leaves destination alpha unchanged and uses
denominator256. Preserve source/destination aliasing and empty signed loops.

Text alpha inversion uses signed pitch*rows; A1 toggles alpha and clears RGB
when alpha becomes zero. Neighbor filling uses unsigned logical width/row
counts and contiguous pixel traversal, but separate signed pitch/2 or pitch/4
for vertical neighbors. A4 averages then halves colors; nonzero-alpha pixels
are untouched. Drawing retains four font choices, minimum height17, 16-bit
RGB background even in A8 format, both alpha passes, three SelectObject calls,
ignored HRESULTs and right-only1024 source clipping. COM failure without an
output write leaves the surface indeterminate; do not initialize it to hide
this behavior.

Cold new families yield compositor686/563 diagnostic differences, drawing
403/316, fill801/784 and inversion182/163: no exact credit. Compositor's
aggregate/individual RECT forms agree with each other but not target; the
compiled member also selects a different EAX input/frame. Drawing emits REP
background stores rather than native byte-limit reloads. The A1 bitfield
representation also differs; these are real codegen gaps, not normalization
opportunities. No old canonical source/header inputs or proofs changed.

Private full-body diagnostics agree on74 composition and162 text cases, with
independent integer references for composition/standalone filters. Drawing
executes actual new filter bodies under modeled GDI raster bytes and D3DX.
Observations include pixels, workspace, calls, rectangles, cleanup, registers
and stack. One CPU/nice10, max RSS95,504/107,536 KiB. This is bounded diagnostic
evidence only; full native product/runtime and all four exact gates stay open.

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
rules at0x004A3480. Its source312/target300 with the real decoder visible remains non-exact; generic runtime
unlock37 differs in one SIB byte. Observed buffer ownership/free conditions
are preserved. See `RESOURCE_FILES.md` for complete paths and bounded failed
contexts, including the need to retain non-global archive caller visibility.


## Archive decoder and the fourth input

The complete472-byte decoder at0x004426C0 includes eight internal alignment
bytes omitted by the sparse Ghidra body. All four callers and both RET12
paths establish output size in EAX plus input, signed packed length and
destination on the stack. The old three-input prototype was incomplete;
extraction now passes entry unpacked size independently of packed length.

The maintained LZSS-style decoder uses a shared8KiB history at0x004C0EE8,
resets its write cursor to1, retains bytes across calls, and reads MSB first.
Token1 carries an8-bit literal; token0 carries a13-bit absolute ring index,
then four length bits plus3 unless the index is zero (end). Overlapping copy
is sequential. EOF fetch precedes the signed bounds check and supplies zero
without advancing. Output size bounds allocation only. Generic algorithm
similarity leaves origin indeterminate.

Natural VC9 /GL source currently produces473 bytes with177 unrelocated
differences; no exact credit.62 private target-versus-candidate machine-code
cases agree on return/allocation/output/history/ABI observables. Only the two
affected old exact units, resource load and archive lookup, were cold-replayed
and remain exact; all other21 proofs were left alone. See
`ARCHIVE_DECOMPRESSION.md` for bounded failed contexts and diagnostic limits.


## Exact camera setup and renderer submission

`ApplyCamera`0x0042A810 (344 bytes) and `FlushRenderBatch`0x0044FD10 (143)
now pass complete cold public primary-TU linked proofs. Two independent
families replay all19 reviewed fields and487 bytes with zero differences;
no previous canonical source/header changed and no prior23 proof was replayed.

Camera setup converts unsigned viewport dimensions, centers eye/target,
constructs D3DX LH view/perspective matrices, sets D3D transforms and propagates
two renderer offsets. Direct conversion expressions let VC9 retain/reuse the
x87 intermediates and compute aspect before the angle call; separate width/
height locals delayed division and produced342 bytes. Natural stdcall yields
EDI camera; no ABI or padding tricks were used.

The renderer's28-byte FVF0x144 vertices feed TRIANGLELIST UP drawing with twice
its quad count. It resets two stage states, updates draw-call count and moves
batch start to next vertex. Its shared pointer cell is0x004C3268; count is
+0x435620, vertex storage+0x435624, next/batch-start pointers+0x7B5624/+0x7B5628.
The physical view retains unknown preceding storage and does not prove a
complete renderer class. Camera selection independently reveals0x118 stride.

The361-byte perspective-camera source at0x0042A970 uses target direction+0x0C,
eye offset+0x3C, up+0x18, right+0x30 and field of view+0x48; projection planes
are30/1800. Full primary-body replay of12 independent bindings leaves only
four operand bytes different in an x87 multiply. It remains non-exact; source
helper variants and private evidence are recorded in `RENDER_CAMERA.md`.
Frame dispatch307 and presentation226 remain next useful integration work.


## Frame dispatch and presentation source

The complete307/226-byte update-draw/presentation functions now have source in
`FrameLoop.cpp`. Native return0/-1 update exits stop the runtime worker and
return1/2; the signed-byte skip count controls drawing. The renderer scene,
texture0 unbind, timing, screenshot, negative-Present reset and post-hook flows
are preserved. Wait is enabled only for flag1 and subtracts3.5ms, not1ms.

New partial views expose skip/wait at MainRuntime+0x1E6/+0x1EB, default camera
+0x31C, active pointer/index+0x54C/+0x550, reset state+0x574, worker+0x7F0,
fog cache+0x990 and elapsed double+0x9B8. The old MainRuntime header is intact;
these overlapping observations do not close full native data ownership.

Fog disable54 and thread stop101 pass strict cold public primary-TU proofs,
all five independent bindings and155 bytes. Only these new units were replayed,
sharing one family; previous25 proofs were untouched. Thread origin remains
indeterminate: generic Win32 wait/stop mechanics do not establish provenance.

Frame dispatch315/target307 still differs because maintained update dispatch
passes its manager on the stack while native uses EBX. Presentation224/226
lacks the native PUSH/POP ECX frame; double/float/member/fp-fast probes did not
close it. No padding, false ABI or cropped comparison is accepted. See
`FRAME_LOOP.md` before revisiting those bounded experiments. The206-byte
runtime surface helper and medium reset paths remain useful direct dependencies.


## Runtime surfaces and animation VM foundation

VM reset0x00401FD0 (303 bytes) and surface release0x0042B600 (80) now pass
strict cold public primary-TU linked proofs:383 owned bytes, eight independent
bindings, two new families. Prior27 canonical inputs/proofs were untouched.

Reset preserves a12-byte opaque block+0x3E8 and DWORD+0x20 across0x434-byte
clear, rebuilds matrix/scale/defaults/timer and two self-owned list nodes.
Aggregate preservation and inline timer/node operations reproduce all bytes;
scalar or64-bit-head representations do not. This proves the observed reset
span and codegen, not the block's original type or complete VM class size.

The new206-byte runtime surface source obtains file texture slots2/3, binds
scripts81/82/81 and gets the back buffer. Null primary surface after binding
copies full280-byte camera0 to2, rather than only its known matrix/viewport
view. Current206-byte candidate has43 register-save differences. Release80
has the final unconditional primary clear; its RET directly borders an
unmapped13-byte texture getter at0x0042B650. That getter corroborates20-byte
texture records but has no credit and is absent from the provisional inventory.

Script binder0x0044ACD0 has complete source191/target204,165 differences.
Invalid script or disabled binding zeroes all0x434 bytes without normal reset
preservation. Valid binding publishes file/script/timer state, invokes VM
execution and increments renderer+0xA0. File script/texture/guard fields are
+0x11C/+0x120/+0x124. Interpreter0x0044B4B0 still has no source; its stack VM
input was reviewed. See `RUNTIME_SURFACES.md` for views and bounded experiments.


## Exact device state and texture recreation

Device-state reset0x00447270 (778 bytes) and renderer texture recreation
0x004453B0 (162) pass strict cold primary-TU linked proofs:940 owned bytes,
42 independent fields and two new families. Previous29 canonical inputs/
proofs were untouched. Float-to-DWORD marshalling has a real4-byte stack slot;
the initial ECX push is allocation, not a native input. All33 D3D state calls,
1000/5000 float literals and cache+0x4355BC stores match; byte+7 stays untouched.

Texture loops retain the initial32-slot array at renderer+0x4350C0, reread
mutable file/count/table state, and use signed count+0x10C, entry table+0x120,
20-byte records and flag+0x10. Reconstruction must not freeze the entry/table
across Release. Recreation uses current presentation dimensions/format, one
level and render-target/default-pool parameters; pixel metadata follows the
call even on failure. Separate condition/inline recreation accesses reproduce
162 bytes; a cached entry reference produced166.

Preparation0x00445320 has full135-byte source but two commuted SIB differences
at76/79, no exact credit. An inline release helper gave123; pointer-addition
leaves two differences. Full reset/surface/VM/clock visibility gives frame
315/presentation233; a private float/fast primary still233, rather than closing
307/226. Public frame source remains intact. See `RENDER_STATE_RESET.md`
before retrying those configurations; full native data/runtime closure is open.


## Window callback and CPU surface transfers

- MainWindowProc0x00445E00 is installed in WNDCLASSA.lpfnWndProc at0x00446B36
  before RegisterClassA. Complete418-byte callback is now strict cold VC9 /GL
  exact with21 independent runtime/IAT fields. All seven special message
  paths, return values, repeated cursor-count operations and default handling
  are retained. Window/root integration supports authored classification.
- Shared default handling after the message switch, nested size/system-command
  switches and do/while ShowCursor(TRUE) are decisive. Explicit per-case
  default returns give400 bytes; an equality/while form418 has192 differences.
  Do not repeat those failed shapes or patch branch/alignment bytes.
- Runtime Windowed+0x114 agrees with D3DPRESENT_PARAMETERS+0xF4. Activation
  +0xB14(0x004C3D94) is also the keyboard polling gate; inactivity+0xB18,
  resize flags+0xB40. Full runtime ownership and flag semantics remain open.
- Complete surface transfer bodies0x00444010(201) and0x004440E0(217) now have
  maintained source. ESI carries the owner, stack carries surface, AL returns
  boolean and RET4 cleans stack. No incoming references are known; origin
  remains indeterminate rather than inferred from source presence.
- Shared fields: format+100,width+104,signed height+108,row bytes+110,
  presence+11C,pixels+120. Read ignores LockRect HRESULT and unlocks after a
  format mismatch; write requires exact D3D_OK and does not unlock on nonzero.
  Both copy row bytes with independent surface pitch, then return true even
  when format/height skips all rows. SDK surface slots30/34/38 were checked.
- Natural early-guard members yield202/205 with186/199 differences. Free
  stdcall inputs and /EHsc do not change them; direct pitch yields201/201,
  nested-success212/212, all non-exact. See WINDOW_SURFACE_BUFFER.md before
  revisiting. Only the new window canonical unit was cold-built; prior31
  source/header contexts were unchanged and not replayed.


## Text resources and executable checksum

- Text workspace at0x004A57C0 starts with256 RNG sample bytes. Its subsequent
  format100,width104,height108,imageBytes10C,pitch110,DC114,previous bitmap118,
  bitmap11C and pixels120 agree with the earlier surface-copy layout. Bitmap
  is the presence field. Existing transfer sources and their non-exact state
  remain unchanged; full original class/owner is still open.
- CreateTextSurface0x004435F0 now has full396-byte strict cold exact proof.
  Ghidra's two ranges omit the3-byte internal LEA at0x0044361D; a read-only
  full-span query reviews all396 bytes. Keep signed pitch arithmetic, -65 DIB
  height versus64 logical/cleared rows,24-byte format records and sentinel
  fallback for unsupported arguments. Only argument-1 is explicitly rejected.
- Release0x00443560 is105-byte exact; it returns false for null DC, otherwise
  reselects/deletes and clears dimensions/handles/pixels/format while preserving
  pitch/imageBytes. Generic create/release origins remain individually
  indeterminate despite exactness. Their source publication order was decisive:
  early selection-result storage392/44 differences, intermediate396/3 and
  final396/0. No padding or operand normalization was used.
- Text/font initialization0x00444480 has346-byte strict exact proof. It tries
  A4R4G4B4 then A8R8G8B8, fills256 samples with locked Next16>>9, and creates
  CP932 MS Gothic/Mincho fonts: height32 weights400/600 and height15 weight700.
  SHIFTJIS_CHARSET, ANTIALIASED_QUALITY and FIXED_PITCH|FF_ROMAN are retained.
  Game RNG/runtime integration independently supports authored classification.
- Three canonical units cover847 bytes and56 fields through two cold primary
  families. Only these new units were replayed; previous32 source/header
  contexts were untouched. `TEXT_RESOURCES.md` records all bindings and flow.
- Executable checksum0x004476F0(246) now has full source. Path buffer261,
  LoadResourceFile disk mode, failure0xFFFFFFFF, signed size/4-1 bound, omitted
  last DWORD/trailing bytes, buffer free and root+998/+99C publication are
  independently observed. Initial void decompiler signature was wrong.
  Current251/179 differences remains non-exact; indexed224, early pointer247
  and nested251 contexts do not close it. Avoid repeating those configurations.
- Startup title/link handling344 and shortcut helper274 now have complete
  non-exact source. Shortcut uses native EDI destination and stack filename,
  EAX boolean/RET4; the default decompiler signature omits that destination.
  Current evidence and failed contexts are in STARTUP_WINDOW.md.


## Startup paths and window initialization

- Three full medium sources cover1,195 target bytes: InitializeMainWindow577,
  CheckStartupPath344 and ReadShortcutTarget274. No exact credit: current
  candidates569/353/243 have383/265/207 unequal unrelocated positions. Previous35
  exact inputs/units were unchanged and not replayed.
- Startup caller0x004455CC explicitly tests EAX==-1, correcting the void
  decompiler hypothesis. Return0/-1 is determined by opaque integer0x004C9898,
  independently from missing-title flags0x40 and byte mismatch root+B30.
  Existence is queried even when no dot exists; shortcut loops keep loading
  the original input filename, not the previous output. Preserve these flows.
- Shortcut null destination returns0 without COM calls. CoInitialize and
  conversion results are ignored; SUCCEEDED accepts nonnegative HRESULTs.
  Query/load/GetPath use real ShellLinkA/PersistFile identities. Every explicit
  delete/release path and CoUninitialize behavior is retained; no Resolve call
  occurs. Generic helper origin remains indeterminate. Current EDI/EBX/RET
  convention differs from native EDI/stack/RET4; checked new-array sizing also
  differs. Early-return264, shared-return243, input-order,<new>,/EHsc contexts
  did not close it; do not repeat them without new evidence.
- Window setup caller0x00445733 supplies HINSTANCE in EBX. Explicit memset
  rather than aggregate WNDCLASSA initialization restores EBX and48-byte frame;
  full body still differs. Retain BLACK_BRUSH4, BASE class, actual title,
  callback, activation1/0, mode bits2:3 and timing/frame-skip mask0x10. Opaque
  root+B84 records become(15,15,0),(12,12,0),(12,12,0),(8,8,0), then root+B80=0.
- Window styles are0x90000000 fullscreen and0x100B0000 windowed. Windowed bases
  are1280x960/960x720/640x480 plus the actual fixed-frame/caption metrics.
  GetWindowRect and root+B08/+58 publication precede the null-handle test.
  Success minimizes, sleeps16 ms and restores. The+58 consumer remains open;
  the timing byte at+1EB equals2 in this test and is not a proved boolean.
- Fifty-one bounded native/candidate machine-code cases agree with modeled
  Win32/COM/CRT calls, including API ordering, arguments, failures, cleanup,
  outputs, runtime fields, saved registers and each actual stack convention.
  Native GUID/class/title data are retained; instructions are not patched.
  One CPU/nice10, about88 MiB peak RSS. This supplies no exact/native/full
  semantic authority. See STARTUP_WINDOW.md and the private differential reports.


## Device creation, viewport and back buffers (2026-10-24)

- InitializeD3DDevice0x00446D30 has complete1,338-byte target review and maintained
  source. Ghidra reports1,335 addresses; full-span decoding includes3-byte LEA
  alignment at0x00446F2D. Complete candidate1,350/845 unequal positions remains
  non-exact. Aggregate-eye1,302 and named-float1,350 are recorded; the latter
  retains native float store/reload/negation. Original vector/TU/frame context
  remains open; do not repeat those same forms without new evidence.
- Creation order is HW HAL40, SW HAL20, SW REF20. Compatibility mask2 skips
  HALs; mask1 forces16-bit/omits alpha-format test. Nonnegative Create succeeds,
  but CheckDeviceFormat requires exact0. Runtime flags1 records HW VP,2 is set
  before creation and4 tracks the optional format test. REF skips that test
  without clearing its previous state. Preserve ignored query HRESULTs.
- Fullscreen colorFF initializes X8R8G8B8/setting0; other nonzero uses R5G6B5.
  Windowed uses adapter format; back buffer is always640x480. Path mismatch
  disables refresh changes only in the fullscreen path. Disabled refresh
  changes do not set Windowed. Refresh retry clears Hz/status but not the
  disable flag, so persistent failures can keep retrying; interval-one can
  instead retry at immediate before fatal log/release/return1.
- New runtime observations: desktop19C,color1E2,options200,disable refresh57C,
  active refresh580,reset584,caps59C,windowB08,resetB0C,path mismatchB30 and
  presentation optionsB40. View/projection/viewport/presentation remain5C/9C/
  DC/F4. Full original owner size and several field meanings remain open.
- D3DX thunks45F9D4/45F9DA independently bind LookAtLH/PerspectiveFovLH in
  d3dx9_37.dll. Eye uses(320,-240,-float(240.0/tangent(0.2617994f))), target
  (320,-240,0),up(0,1,0); projection0.5235988f,4/3,100,10000. Tangent31-byte
  wrapper has stack float/RET4 and float-rounded ST0; no new source/credit.
- AppendErrorLog0x00458AF0 is strict cold207-byte primary-TU exact,11 fields.
  It uses512-byte temp/8192-byte log, strict end+len<text+8191 and unconditional
  hasError at+2004, runtime lock/depth3 and format-pointer return.
- ResetMainViewport0x00447EA0 is strict cold99-byte primary-TU exact,11 fields.
  It flushes queued batches, sets(0,0,640,480,0,1), submits viewport and tail
  jumps to priming. Independent unknown batch input retains ESI; global-only
  context96 specialized it away and must not receive exact credit.
- PrimeDeviceBackBuffers0x00447E00 is strict cold157-byte primary-TU exact,
  eight fields: two Clear(target|depth,opaque black,1,0)/Present cycles, Reset
  on negative Present, ignored Clear/Reset status. Two inline operations match;
  counted loop91 does not. Source helpers add no separate function credit.
- Three new exact units cover463 bytes/30 bindings. Only these new units were
  cold-replayed; previous35 canonical source/header contexts were unchanged.
  Required older bodies are new family supports, not re-proved exact units.
- Thirty-six modeled machine-code cases agree,34 returns plus two retry-prefix
  observations. The full runtime span, parameter mutations, call order/args,
  logs, matrices/viewport, cleanup, registers and stack agree under controlled
  COM/log/math models. Tangent return0.25 and D3DX matrices are fixtures; this
  supplies no exact/native/full-semantic authority. One CPU/nice10,~64 MiB RSS.
  See DEVICE_INITIALIZATION.md and private strict/differential logs.

## ANM source loading, postload cursor and sprite construction (2026-10-28)

Direct full-text-attested Ghidra review corrects the former next-target guesses:
0x004543E0 is a221-byte external-source loader,0x004544C0 is a212-byte document
postload cursor, and0x004545A0 is the actual811-byte texture/sprite/script decoder.
All now have maintained full source. The decoder's sparse802-address Ghidra body
omits9 internal alignment bytes; its complete811-byte extent is preserved.

Busy+124 is a one-based chunk cursor. Each worker call rescans preceding chunks
to accumulate texture/sprite/script indices, decodes at most one matching chunk,
and increments busy or clears it at completion/failure. External source loading
requires version7, skips embedded/@ names and uses disk-mode1. The decoder has
ordinary-file, @R render-target, other-@ empty-texture and embedded-data paths.
Only file/embedded negative results are rejected; empty-texture results are
added unchecked and render-target results ignored. GetLevelDesc HRESULT is
ignored. Script records contain a skipped ID followed by the relative offset.

StoreAnimationSprite at0x00454B10 is a complete149-byte raw match under a cold
VC9 /GL public primary-TU proof, no linked fields. The physical72-byte record
stores height+1C/width+20, UVs+24..30, recovered height+34/width+38, and X/Y scales
+3C/+40. It copies the entire record before calculations; decoder stack+44 is
indeterminate and must not be initialized gratuitously. Destination-aliasing
source pointers retain the native read/write behavior. Canonical command:
`scripts/repo-python scripts/replay-exact-units.py --unit store-animation-sprite`.

Private complete-body diagnostics agree in58 chunk/sprite cases, including five
integrated cursor/decoder cases; modeled I/O, texture constructors, error/trace
and COM helpers do not establish full runtime or semantic-phase acceptance.
Only indeterminate decoder sprite+44 is omitted from semantic snapshots; exact
code replay has no byte masks. The affected stage/document-loader family was
cold-built and all34 modeled cases still agree, with unchanged code sizes and
difference counts. Earlier41 exact contexts were not modified or replayed.
Unicorn callback cycles delayed fixture release in the first diagnostic pass;
collecting fixtures after each execution reduces current maximum RSS to41,080
and42,148 KiB. Ghidra/compiler serialization and default limits are retained.

See `ANIMATION_CHUNKS.md` for source/native extents, private ABI differences,
review artifacts, exact evidence and remaining lower texture constructors.
The worker dispatcher0x004548D0 remains unmapped despite private review; its
native destruction branch clears the slot and then dereferences that null slot.
Do not turn that observation into a repaired source path without evidence.

## Texture constructors and near-exact chunk decoder (2026-10-28)

All four ANM texture constructors now have maintained source: file484 bytes,
embedded287, empty67 and render-target69. Direct Ghidra instruction review
supersedes default decompilation's incorrect D3DX prototypes/dead-block removal.
Embedded, empty and render-target are strict cold public primary-TU linked-PE
exact, adding423 bytes with20 independently reviewed address bindings. The
affected sprite149-byte unit was cold-replayed; earlier41 exact contexts were
unchanged and not replayed. New totals are83 source mappings/45 exact/9,697 bytes.

File and embedded paths honor runtime+200 bit0 by remapping destination format
indices; empty does not. File creation rejects any nonzero HRESULT, then ignores
surface/description/crop creation/copy results. Matching dimensions bypass
cropping and ignore offsets; mismatches use signed crop comparisons without a
lower clamp. Releases are source surface, destination surface, temporary texture.
File success calls transparent-pixel processing before metadata publication.
Embedded source header fields are signed16-bit, and destination fallback does
not change the source format/pitch. It ignores upload failure and publishes
metadata before surface release. Empty/render creation results are ignored.
Details and exact commands are in `ANIMATION_TEXTURES.md`.

The real texture/error/resource source family exposes the decoder's original
calling relationship: only AdvanceAnimationDocument directly calls it. Removing
an extra diagnostic direct caller restores its unaligned112-byte frame.
Per-branch texture-entry evaluation and later table reload, typed20-byte sprite
and eight-byte script definitions, ordinary width/height/format empty-creation
parameters and texture-first sprite setup bring DecodeAnimationChunk to the
full811 bytes with10 diagnostic differences. These are five sprite setup
register/store positions and five script-entry instruction positions; no exact
credit is granted. A pointer-temporary sprite variant gave39 differences and
was discarded. Do not repeat these contexts without new evidence.

The current family gives file construction483/70, source loading209/147 and
postload advance211/118 byte counts/unequal diagnostic positions. Private ABI
specialization differs between families: the source resource loader now takes
EBX filename and only a stack size output, with disk-mode1 implicit. Test adapters
were changed from observed artifact instructions, not false C++ declarations.
The file constructor still calls an opaque transparent-pixel helper via a stack
argument versus native EAX; frame/one-byte context gaps remain open. Its
0x004534C0 provisional1,381-byte owner requires complete boundary/table review;
preliminary decompilation is not source coverage.

Complete-body fixture diagnostics agree in92 constructor cases, including four
matching faults, and58 chunk/sprite cases in the new family, including five
integrated cursor/decoder cases. The affected stage/document family was rebuilt
and its34 cases still agree. D3DX/COM/pixel-processing/I/O models do not establish
actual pixel output, full native game/runtime closure or semantic acceptance.
The user's current tool preference is direct local commands, without Factory
MCP; this is now recorded in AGENTS.md as well.

## Complete texture-pixel owner and corrected descriptor fixtures (2026-10-28)

FixAnimationTextureTransparentPixels at0x004534C0 now has complete maintained
source. Its owner is1,427 bytes through0x00453A52, not the initial1,381-byte code
candidate: code throughRET+3 alignment bytes+four absolute jump destinations+
27 selector bytes. All apparent external code-interior references come from
that owned table; every owned-tail byte was checked for references, with only
the two internal table reads. Following13 CC bytes precede an uncatalogued
entry0x00453A60, which remains unmapped and needs its own complete review.
Generic pixel/COM processing plus game callers do not establish original
source authorship: this new mapping's origin remains indeterminate.

The function locks surface0, uses unsigned dimensions and signed per-row
pitch, averages nonzero-alpha left/right/above/below RGB for transparent pixels,
retains alpha, and unconditionally unlocks/releases. Formats UNKNOWN/ARGB32,
A1R5G5B5 and A4R4G4B4 are handled; others still perform the surface calls.
Negative and non-divisible vertical offsets retain signed pitch/2 or pitch/4
truncation. HRESULTs remain ignored, including the native fault paths. The A4
case preserves full averaged channel values; unrelated text-pixel half-scaling
is not evidence here. Full details are in `ANIMATION_TRANSPARENT_PIXELS.md`.

VC9 /GL produces a complete1,383-byte pixel contribution (1,338 code+2 alignment+
43 table bytes), non-exact. Its source0x3C/target0x44 local frame and zero-register
scheduling differ. Byte versus typed32-bit pixels and truth versus unsigned
positive conditions gave the same code. The current linked decoder also stops
before the final selector byte; no exact unit is configured and no extent is
cropped. The real body naturally restores the file constructor's EAX helper
argument, improving that caller from483/70 to484/40 byte/diagnostic differences.
The three affected exact constructors were cold-replayed with the new source in
their family: all423 bytes/20 fields still agree. Other42 exact contexts were
unchanged and not replayed. Decoder811/10 remains non-exact.

The prior private constructor/chunk fixtures omitted MultiSampleQuality and
packed seven DWORDs for D3DSURFACE_DESC. The SDK type is32 bytes/eight DWORDs;
Width is+24 and Height+28. Thus previous differential agreement did not prove
its intended dimension/branch coverage. Source used the correct SDK type and
strict byte proofs were independent of the fixture. Corrected92 constructor
and58 chunk cases now agree; explicit crop/adoption trace assertions and24
additional integrated file/pixel cases strengthen the corrected evidence.

New242-case diagnostics execute both complete original/candidate pixel bodies,
owned tables and buffers, with all format selectors, boundaries, random/checker/
opaque/transparent data, row padding, negative/non-divisible pitch, ignored API
failures and two matching faults. An independent four-neighbor expected average
and valid-layout alpha checks supplement full buffer/call/stack/register
comparisons. The116 constructor cases include24 actual file/pixel integrations
and four matching faults. COM and D3DX loading/copying remain fixtures; these
results do not establish native game/runtime or semantic-phase acceptance.
Current mappings84, exact45/9,697 bytes; full Windows i386 product remains open.

## Stage lifecycle, exact initializer and real C++ cleanup (2026-10-28)

Maintained source now covers StageResourceView construction, initialization,
destruction and factory, its embedded VM constructor/destructor and three
callbacks. Factory allocation plus constructor clear independently establish
0x3158 bytes. Eight0x434-byte VMs begin at+180 and three at+2348; six20-byte
timers and the0x118-byte copied camera now have checked physical views. These
names/layouts do not claim the original source class declaration or TU split.

InitializeStageResource0x004024C0 is strict cold630-byte linked-PE exact with21
reviewed fields. It publishes variant/global stage before loading, copies the
runtime+434 camera, sets vector/distance defaults, allocates/registers three
callbacks at update12/draw2/draw5, then updates timer/reset flags. No rollback,
allocation guard or handling of registration failure was added. The first625-byte
family let the sole update registration's constant12 specialize away; an
independent receiver/priority probe restores its real general contract. Camera
vector temporary evaluation before the scalar distance store restores the
remaining native schedule naturally, with no instruction or ABI normalization.

StageEmbeddedVm construction0x00402270 is complete101-byte exact, destructor
0x00402170 complete34-byte exact, each with one reviewed CRT field. Construction
initializes eleven timer flags, clears434 bytes and sets short+39C=-1; it is
not ResetAnimationVm's matrix/scale initialization. Destruction frees/clears
optional+400 storage. StageResourceView constructs the eight/three VM arrays,
then deliberately clears all3158 bytes and sets flags2, overwriting those just-
initialized markers. Natural C++ member constructors/destructors preserve this
and the automatic reverse cleanup order; do not repair it.

Two seven-byte draw adapters0x00403910/0x00403920 are strict exact with one real
callee binding each. The update adapter0x00403900 remains seven bytes/three
diagnostic differences: native forwards ECX through EAX to the still-external
update body, while the ordinary source call uses its declared stack contract.
All three entries were absent from provisional inventory and were added only
after explicit full-span reference/neighbor review. Their original authored
versus compiler-generated attribution remains indeterminate.

Five non-EH public primary-TU proofs add779 bytes/25 bindings; current totals
are1,992 candidates,93 source mappings and50 exact/10,476 bytes. Other45 exact
contexts were unchanged and not replayed. The180-byte stage constructor and
159-byte factory have diagnostic address-field matches only: EH handler/funclet
ownership is outside the current exact lane, so neither gets exact credit.
The603-byte stage destructor has597 source bytes/143 diagnostic differences;
its full ordinary behavior and C++ member/EH structure are present, while
exception injection/full unwind/native-product acceptance remain open.

Full-body diagnostics agree on48 lifecycle cases, including five matching null
allocation faults. They execute actual VM constructors/destructors within
modeled forward/reverse CRT iteration and cover publication, optional cleanup,
slot mutation, alias globals, timer flags, callback values, ignored registration
failure, depth wrapping, manager changes and runtime flags changing across
conditional Enter/Leave. The fixture uses real32-bit GDT code/data/stack segments
and a separate FS TEB segment: mapping address0 to fake FS would hide null-chain
faults, and FS_BASE writes alone do not work for this Unicorn32-bit setup.
The native/candidate normal exception-chain restoration, full storage, calls,
stack and registers are checked. Heap/loading/chain/document services remain
models, not whole-game runtime evidence. The affected old loader context was
cold-rebuilt and its34 cases still agree, with unchanged297/434/131-byte sizes.
See `STAGE_LIFECYCLE.md` for sources, contracts, proof commands and limitations.

## Complete stage drawing and overlay (2026-10-28)

Direct local hash-attested Ghidra review now covers complete DrawStage403130
through403597 (1,128 bytes) and DrawStageOverlay4035A0 through4038F5 (854 bytes).
Both take one stack stage pointer and RET4; the overlay decompiler's phantom
fastcall register arguments are not inputs. No tables or external interior
references occur in either owner. Maintained `StageDrawing.cpp` has complete
non-exact source for both, with public primary-TU contributions1,106/852 bytes.
Prototypes of their external helpers do not grant those helpers source credit.
Counts are95 source mappings and unchanged50 exact/10,476 bytes.

Recovered fields include stage draw counter+2324, packed tint+2328,
accent+232C, draw offset vector+2FE4 and camera fog bits+FC/+100/+114.
Layout and constructor/destructor behavior remain unchanged. The initial camera
pass copies0x118 bytes into runtime+434, preserves runtime camera offsets and
publishes active camera/index. Each D3D state call retains its preceding flush;
fog updates compare the runtime+990 cache. Failure HRESULTs remain ignored.

Main drawing uses layers0..7 and an eight-VM pass gated by the FIRST VM+3AC;
later active VMs alone do not start that pass. Overlay draws groups25/26 and
layers8..11. Flag mask04 with timer below30 starts `(3,30,0,0,10)` and resets the
timer to1 on EVERY main draw. Overlay decrements a positive timer by-1, reloads
it, optionally sets flag08 when02 was set, clears06, and finishes with tint
00FFFFFF despite an earlier alphaFF store. Captured renderer pointers survive
the observed dependency boundaries; do not gratuitously reload globals.

Copy fog float bits through const-reference memcpy into DWORD. A float value
parameter emitted x87 FLD/FST and quieted signaling NaNs, unlike the original
integer loads. The source correction is ordinary representation copying and
passed signaling-NaN, negative-zero, infinity and quiet-NaN cases.

Full original/candidate body diagnostics agree on952 modeled cases, including
four null faults and128 persistent three-frame main/overlay sequences. Ordered
COM/dependency calls, whole stage/runtime state, renderer replacement, active
camera redirection, tint, signed timer/age thresholds, unsigned counter/viewport
wrap, stack and nonvolatile registers are checked. Dependencies are explicit
models; this is not whole-game runtime acceptance. Only the five affected
canonical lifecycle/callback units were cold-replayed, all exact. The48
lifecycle cases still agree with five matching allocation faults. The affected
loader family was rebuilt with unchanged297/434/131 sizes and17/337/80
diagnostic differences; its34 modeled cases still agree. Public CI passed8+25
tests. See `STAGE_DRAWING.md`.

## Complete stage update and object traversal (2026-10-28)

Direct local Ghidra full-span review now accepts UpdateStage402AA0..40312C
(1,677 bytes) and UpdateStageObjects4040F0..404188 (153 bytes), including all
internal alignment with no tables/external interior references. Both now have
complete non-exact source in `StageUpdate.cpp`, with public primary-TU source
contributions1,705/151 bytes. Source mappings rise to97; exact remains50
functions/10,476 bytes. Native update receives EAX stage, object traversal EBX.
The current integrated source uses EBX and stack respectively. Escaping the
fastcall callback address in the maintained probe preserves its actual indirect
contract but emits10 bytes; the earlier direct-only probe allowed a specialized
five-byte wrapper. Neither resolves original codegen or grants exact credit.

Update resets camera offsets E8/EC and vector F0, normalizes targetOffset0C
into the observed vector24, not eyeOffset3C, and writes tint00808080. Flags08
or04 with signed timer>=60 return before writes. Flags04/timer>=30 skip only
objects/script/VM dispatch; camera publication, accent clearing, grid effects
and wrapping age increment still run. It executes eight VMs unconditionally,
then optional three VMs with saved time scale/temporary1/restoration. The saved
scale is loaded after the first eight VMs; changes within the extra group are
visible to subsequent calls. It copies the full0x118-byte camera to runtime+434.

Stage fields3014/3018 are minimum/current radius,3020/3024 phases,3028 mode.
Binding block's physical six words also describe signed rows/columns, IDs,
index storage,28-byte vertices and12-byte source positions. Mode1 sets alphaC0
on all vertices but moves only interior points, clearing their vertex/source Z.
Mode2 uses the old radius after conditionally decrementing stored radius by2;
its reset rectangle is(-r,224-r,2r,2r), radial center224/240. Scratch Z carries
sourceZ-previousZ across every vertex and row. Remaining distance>=0 is the
inside test, including unordered handling; do not copy the decompiler's inverse
comparison. Preserve float rounding before subtraction/division, NaN color
conversion behavior and the distinct RNG phase-jitter rounding points.

The486CF0 CRT boundary is x87 sine, not a fastcall ECX function: its486D48
implementation contains FSIN. Ordinary C++ sin(double) plus float conversion
emits the corresponding VC9 intrinsic call. Float-rounded angular constants
were independently read from the verified PE. No CRT source mapping was added.

Object traversal tests byte+3 mask01, starts primitives at+1C, terminates on
negative signed16 kind, executes signed16 VM indices+6 and advances by signed16
byte size+2. It clears only active01 when no post-execution VM instruction+3A8
is live. Negative indices/strides and initially empty streams are preserved.

All853 modeled cases agree, with nine matching faults under the declared
dependency models. Actual angle/RNG machine bodies execute in both runs;
additional cases execute actual object traversal within the full update.
Whole stage/runtime/mesh state, calls/float bits, live flags, scale/RNG state,
registers, stack and x87 control/tag state are checked, with independent branch
assertions. Script/VM/reset/D3DX/sine remain explicit models, not full native
runtime acceptance. Only the five affected lifecycle/callback exact units were
cold-replayed, all passing. The affected drawing and loader diagnostics rebuilt
with unchanged sizes/differences;48 lifecycle cases still agree. See
`STAGE_UPDATE.md` for exact scope and reproduction artifacts.
Drawing952 and loader34 modeled regressions passed after those rebuilds;
public CI passed8+25 checks. Earlier unrelated exact contexts were not replayed.
