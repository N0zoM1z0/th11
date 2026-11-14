# Audio device setup and buffer recreation

This checkpoint adds five complete source mappings /719 target bytes and nine
complete exact owners /1,103 bytes. Four existing streaming source owners close
alongside all five new mappings. Twelve further boundary/origin reviews recover
seven omitted entries; all remain indeterminate. Full factory713/665 bodies were source-open at this checkpoint; the subsequent
[MUSIC_FACTORIES](MUSIC_FACTORIES.md) checkpoint supplies complete source. Their
codegen/EH ownership remains unresolved.

| Owner | Address | Full bytes | Current state / fields |
| --- | --- | ---: | --- |
| InitializeSoundDeviceOwner | 459D20 | 7 | Exact /0 |
| ReleaseSoundDevice | 459D30 | 21 | Exact /0 |
| InitializeSoundDevice | 459D50 | 66 | Exact /2 |
| ConfigurePrimarySoundFormat | 459DA0 | 272 | Exact /3 |
| RecreateMusicBuffers | 45A4F0 | 353 | Exact /6 |
| SelectAvailableMusicBuffer | 45A990 | 100 | Exact /1 |
| StopMusicStream | 45AB70 | 129 | Exact /1 |
| ResumeMusicStream | 45AC80 | 77 | Exact /1 |
| IsAnyMusicBufferPlaying | 45AD10 | 78 | Exact /0 |

## Revalidated target meaning

The earlier name OpenMusicDecoder for45A4F0 was wrong. The full353-byte owner
clears active, releases every nonnull buffer, deletes its pointer array, allocates
another array, creates each device buffer, queries the notification interface,
allocates16 offset/event records and installs them. It performs no decoder-file
open. Source, dispatcher declaration/call and maintained inventory now use
RecreateMusicBuffers. The historical queue note is superseded by this review.

The pointer-array allocation has no null-result guard in the native code. The
notification-array allocation separately returns E_OUTOFMEMORY on null. Creation
or QueryInterface failure returns E_FAIL without invented cleanup. Notification
registration failure releases its interface and deletes its positions before
E_FAIL. Successful registration has the same release/delete cleanup. Each offset
is notificationBytes*(index+1)-1, with the actual event handle at controller74.
No extra failure rollback is introduced. The compiler naturally emits the
native internal loop alignment; source adds no padding or instruction payload.

Controller38 contains its36-byte buffer description, not opaque padding;5C
points to the device owner and74 is the notification event. The primary setup
uses a zeroed36-byte description with primary flag1 and a zeroed18-byte packed
PCM format: stereo44100Hz,16 bits, blockAlign4 and176400 bytes/second. It checks
CreateSoundBuffer and SetFormat separately, retaining the native early-failure
buffer lifetime. Only successful SetFormat reaches Release/S_OK. Its complete
272-byte GS-protected contribution and both cookie-check exits are exact.

Device initialization releases/clears the old interface, calls original DSOUND
ordinal11 through thunk45F9B0/IAT48B008, propagates creation/cooperative-level2
failures, then ignores primary-format setup's HRESULT and returns S_OK. Local
installed export metadata identifies ordinal11 as DirectSoundCreate8; this is
protocol-name evidence, not an imported implementation or original source claim.
The external declaration retains C/stdcall linkage. The complete7-byte initializer
and21-byte release helper preserve their actual stores and void contract.

SoundDevice.hpp independently expresses the12-method device and4-method notify
COM protocols,36-byte descriptor,18-byte packed wave format and8-byte notification
position. Public signatures/order were checked against installed dsound.h.
Opaque unused pointees remain declarations. No library implementation, original
SDK header payload or compiler payload was imported. Static assertions check
physical sizes and controller offsets. These protocols do not prove provenance.

## Large-path corrections and compiler probes

A fresh read of the complete570-byte refill disproved two assumptions in the
previous source: data, byte count and read count must start at zero before the
buffer/status checks; a reset failure during repeated filling returns immediately.
The first reset's status is still ignored. Source now preserves this distinction.
These corrections take the current candidate from545 to569 bytes, with504
masked diagnostic differences. Its full native extent remains570; it is not exact.

Stop preserves a decoder snapshot across CloseHandle, rather than re-reading the
controller's decoder afterwards. Separate active/paused stores and initial status
placement reproduce its129-byte complete body. Resume snapshots the first buffer
before publishing active, and keeps separate null-array/paused failure exits.
Its77-byte owner is exact. Selector uses the observed available-return branch
before the random fallback; any-playing initializes its accumulator before the
null-array test. Their100/78-byte complete bodies also close. No calling convention
or register promises were added to force these outcomes.

The BYTE-pointer data experiment changed none of the major owner diagnostics and
was rejected. A genuine decoder member-Read experiment also left its160-byte
candidate and177 diagnostic differences unchanged, so the public free boundary
was retained. Expressing clipped bytes as base-cursor+size preserves native
arithmetic grouping. Splitting the two Resume gates reduced62 differences to22;
a buffer snapshot then closed the complete owner. Snapshotting priority/flags as
well expanded it to82 bytes and was rejected. Play's counter-store order is
corrected, but its132-byte candidate/18 differences still needs the real refill
calling context. Notification remains544/421 and position reset220/160.

The two larger factory owners allocate/init a decoder and create/register the
streaming buffer before constructing the controller. File713 calls the real file
open and handles its failure; memory665 ignores the memory binder's E_NOTIMPL
return after its state mutation. Both install16 notification events and copy the
complete description to controller38. All native branches, resource lifetime
asymmetries and EH setup remained review evidence at this checkpoint. The later
factory/lifetime checkpoint supplies actual bodies without ordinary EH credit. Ghidra's sparse703/657 bodies omit10/8 internal
alignment bytes; maintained713/665 intervals remain intact.

## Boundary/origin expansion and cold proof

The independently bounded prefix entries are: RET-only input entry459BC0/1;
WORD binding defaults459BD0/98; null-ended node search459C40/18;
three-field counter initialization459C60/12; sixteen-float scratch-matrix
publication459C70/168; device clear459D20/7; release459D30/21; initialization
459D50/66. Seven were missing from inventory. Terminal RET/RET64/RET4 and CC
neighbors partition them. Generic descriptions of the non-audio helpers remain
provisional, with no source mappings or authorship inference. The4 additional
new reviews cover primary272, both factories713/665 and base scalar-delete30.
Recreation353 is one earlier review rechecked, not a thirteenth new origin.

Each of the nine exact candidates has an independently checked exhaustive binding
list: rand46190D; cookie4A2088/check45FC2C; new45FCE4/delete45FD49; notification
GUID48D15C; original device thunk45F9B0; primary459DA0; CloseHandle48B0A0 and
configure45B2D0. All14 fields are decoded from original target operands separately
from candidate normalization. Complete public primary-TU PDB contributions,
current source/header/tool/producer/PE/MAP/PDB receipts and every byte pass
with zero differences.

Cold scope is exactly9 new plus20 affected old MusicStream/SoundQueue units:
29 complete owners /2,741 bytes /59 fields; all passed. All current transitive headers are
declared, including the new SoundDevice.hpp. No unrelated family, full cohort,
broad behavior suite or infrastructure tests are required; infrastructure is
unchanged. Whole-game linking/runtime and original TU/data ownership remain open.

## Exact 168-byte matrix scratch owner (2026-11-12)

`src/AudioAdjacentHelpers.cpp` now implements all three independently
reviewed audio-adjacent owners: `FindAudioAdjacentListNode` at `0x459C40`
(18 bytes), `ChainElementSetCallback` at `0x459C60` (12 bytes; see CALLBACK_CHAIN, previously misnamed InitializeAudioAdjacentTween), and
`SetAudioAdjacentScratchMatrix` at `0x459C70` (168 bytes). The first two
compile to full 18/18 and 12/12 lengths but remain non-exact because the
original uses private EAX/ECX inputs rather than the ordinary fastcall
ECX/EDX assignment. No false register-level ABI is asserted.

The matrix scratch owner is **strict exact, all 168/168 bytes**. Its complete
34-instruction original loads each of 16 float arguments from the stack with
x87 `FLD`, stores them to sixteen consecutive floats at `0x4A5900..0x4A593C`
with `FSTP`, returns the scratch base in EAX and cleans 64 stack bytes.
Ordinary VC9 RTM `/O2 /Ob1 /Gy` C++ naturally reproduces this entire owner.
Every one of the **17** DIR32 fields was independently reviewed: the EAX base
immediate and sixteen stores, all referring to the same native base plus
addends `0, 4, ..., 0x3C`. The pinned complete single-function normal-COFF
COMDAT, current source/tool/object receipt and all 17 field replays give
zero differing bytes. Canonical unit `audio-adjacent-scratch-matrix`; focused
cold proof `.analysis/audio-adjacent-matrix-strict-20261112.log`.

## Nine-key input defaults (2026-11-12)

`src/InputBindingDefaults.cpp` implements the complete reviewed
`InitializeInputBindingDefaults` at `0x459BD0..0x459C31`, an input-binding
receiver with nine `short` defaults at +0x134 through +0x144. Slot order is
`0, 1, 2, 4, -1, -1, -1, -1, 3`; the assignment to slot +0x140 is **-1**:
the target's `OR EDX,EDX` is a flags operation and does not clear the
register, which has already been set to -1. Stores are represented as normal
member field writes in original observed instruction order.

The initial source declared a void member and omitted the native return-
receiver behavior: VC9 RTM `/O2` compiled 96/98 bytes. The native first
instruction `MOV EAX,ECX` is meaningful: the initializer returns its own
receiver. Declaring `InputBindingDefaultsView *` and returning `this` yields
**98/98 strict exact bytes** in a complete normal-COFF COMDAT, 20 native
instructions, zero relocations, zero differences. Focused cold replay:
`.analysis/input-binding-defaults-strict-20261112.log` and canonical unit
`input-binding-defaults`. The current separate `/GL` producer still emits
96/98 bytes due to interprocedural argument allocation; the original TU
partition and normal-COFF versus LTCG ownership remain unknown. The accepted
result is a strict complete ordinary-COFF comparison, not an original-TU
claim. Local /GL diagnostic source:
`.analysis/probe-input-binding-defaults-20261112.py`.
