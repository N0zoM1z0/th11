# Audio command queue and streaming worker

Five complete source mappings cover1,932 target bytes: the1,460-byte audio
command/effect dispatcher,177-byte streaming thread,151-byte enqueue function,
19-byte first-buffer getter and125-byte music-volume function. Three new full
cold linked proofs add453 bytes. Dispatcher/getter codegen remains open.

Eighteen complete boundaries and origins are reviewed. All18 origins remain
indeterminate: application state-machine, Win32 thread and DirectSound utility
roles alone do not distinguish game authorship, templates or reused sample
code. No new candidate entries are invented in this checkpoint. One existing
boundary grows from1,428 to1,460 because its complete owned switch table was
missing from the original inventory.

| Owner | Address | Full bytes | State |
| --- | --- | ---: | --- |
| ProcessSoundQueue | 44A340 | 1,460 | Source; candidate1,556 /1,085 diagnostic differences |
| MusicStreamingThread | 44A900 | 177 | Exact;7 fields |
| EnqueueSoundCommand | 44A9C0 | 151 | Exact;8 fields |
| FirstMusicBuffer | 45AA00 | 19 | Source; candidate19 /13 |
| SetMusicBufferVolume | 45AAF0 | 125 | Exact;4 fields |

## Corrected owner and physical views

The dispatcher is audio code, despite its position near ANM binding functions.
It consumes no input parameters; Ghidra's inferred fastcall parameters are not
used as a source contract. Its1,428-byte code has394 independently decoded
instructions through RET44A8D3. The following32 bytes,44A8D4..44A8F3, are an
eight-entry jump table indexed by opcode-1 with bounds0..7. Each Ghidra DATA
reference independently agrees with its target:44A3CA,44A40F,44A584,44A525,
44A5A7,44A5E7,44A605 and44A3AD. These are internal switch arms, not separate
function entries. Twelve CC bytes separate the full owner from thread44A900.
The old eight external-interior references all originate in this now-owned
table. No unexplained external interior entries remain.

The partial manager at4C3E80 has enabled610, thread ID614, thread handle618,
effect queue620, track records pointer1F84,32 command slots at1F88 and cached
filenames4108. Each command is10C bytes: opcode/value/step and256 text bytes.
Enqueue considers only slots0..30; the shift can copy slot31 as a sentinel.
The effect queue has12 IDs,12 counts and12 blocks of128 pan samples. This is
the same physical storage already used by QueueMeshEffectSound, whose source
and exact context are unchanged. New view names do not establish data ownership. The610 cell is named for the
dispatcher's nonzero gate; its underlying device/interface type remains open.

Music controller prefix fields include buffer pointer array4, unsigned count10,
fade14/18/1C, active30, decoderC and cross-thread busy78. Decoder record90 has dataSize1C and loopStart18 within a34-byte record.
The earlier loop label at1C was corrected by full decoder review; see
[MUSIC_STREAM](MUSIC_STREAM.md). Size/offset assertions check the
observed layouts. Its slot0 scalar-deleting destructor is modeled by a real
virtual destructor and virtual delete call. The later
[MUSIC_FACTORIES](MUSIC_FACTORIES.md) checkpoint supplies actual base/derived
resource cleanup and ordinary public destructor proofs.

The pinned SDK payload lacks dsound.h. The initial `SoundBuffer.hpp` used a
physical function-table view for observed calls. It now declares the full COM
method order with accurate stdcall signatures and opaque unused pointees;
[MUSIC_STREAM](MUSIC_STREAM.md) records the protocol comparison and current
proofs. No library implementation or compiler payload was imported.

## Dispatcher state machine

Section11 is conditional on runtime flags8000, with byte depth at runtime93B.
Disabled manager returns0 after the matching conditional exit. Music mode
uses runtime byte200 bit10; pause/resume use byte1E3 equal1; effect processing
uses byte1E4 nonzero. These are observed physical gates, not inferred ANM flags.

- Command1 optionally closes prepared music at step0, loads/caches the requested
  name and removes the command regardless of helper result. Nonzero internal
  step merely advances. It sets a repeat flag after loading.
- Command2 has separate prepared and decoder paths. Prepared steps0/2/5/7
  start, reset position, refill and play; step20 consumes. Decoder steps0..4
  stop, wait for not-busy/open, select a named track/record, reset/refill and
  play; step7 consumes. Failures consume only at the observed stages. Value
  becomes the record's boolean loop flag at refill. No missing-controller,
  invalid-index or decoder failure recovery is invented.
- Command3 stops with close flag1 at step0, consumes at1 and otherwise advances.
- Command4 stops, posts WM_QUIT, waits256 milliseconds, closes handles/deletes
  the controller, then consumes at10. A nonzero wait result posts again and
  decrements step before the shared increment, staying at2. Successful wait
  clears the handle before stage3, retaining the subsequent native close call.
- Command5 calls the actual inert trace boundary, rounds the signed value to
  float, multiplies by double60 and truncates to fade counters14/18, setting1C.
- Commands6/7 wait without advancing while configured music is busy, otherwise
  pause/resume when configuration equals1, then consume.
- Command8 applies current music volume when a controller exists, then consumes.
  Other opcodes leave the command stage unchanged.

Removal shifts at most31 records while the current opcode is nonzero. The
cursor advances with every copy. The command1 repeat branch reuses that
advanced cursor; it does not reset to the queue head or drain all queued loads.
This behavior is retained even though a different queue algorithm might look
more conventional. The final return reads the global head opcode after the
critical-section exit, not the advanced cursor's opcode.

Effect processing stops at the first negative ID. It clears the selected ID
before work. Negative count stops only; nonnegative count averages signed pan
samples when positive, clears count, then stops/resets/sets pan/volume/plays an
existing buffer. Global buffer cells are re-read between actual calls as in
the source path. Counts are not clamped to128 here. Signed sum uses32-bit wrap
and division truncates toward zero. No upper pan or volume clamp is added.

Effect volume uses rounded percentage `p`, rounded `1-p`, rounded cube,
rounded `1-cube`, then truncates gain times signed-short base+5000 and subtracts
5000. Configuration0 emits-10000. Music helper uses a square instead of a cube,
with the same100.0 divisor and final attenuation conversion. Combining these
curves would change the target. Base/configuration meanings beyond the
observed arithmetic are not inferred.

## Worker, enqueue and compiler experiments

The thread waits on one music event plus a0x4BF message mask. Missing controller
sets completion but still processes that wake result. Event0 updates only an
active snapshot; it sets busy1, passes a freshly read global controller to the
real543-byte UpdateMusicStream boundary, then freshly reads the global to clear
busy. Message wake1 drains PeekMessageA/PM_REMOVE, setting completion on WM_QUIT
while continuing to drain. Other wait results retain the native loop behavior.

A scoped volatile view expresses the worker's three independently observed
shared-pointer reads; the global declaration remains ordinary. Busy is a
volatile physical field used by worker writes and dispatcher reads. This does
not prove original C++ qualifiers or broader thread safety. A globally volatile
pointer reproduced the worker but enlarged dispatcher to1,668 bytes/1,133
mismatches through extra reads; that broader qualification was rejected.
Scoped reads retain full worker177 and dispatcher1,556/1,085.

The actual CreateThread callback pattern, independently observed in449E80, is
present in the probe. An ordinary direct-only call let LTCG remove the unused
thread context and RET4 contract. Retaining the real callback escape restores
the contract naturally. A switch over wake reason restores the target's
message/event branch arrangement. The plain pointer/alias context still made
worker168 bytes; the distinct shared reads close all177, including its internal
NOP. No alignment padding, fake parameter use or false callback ABI is added.

The first enqueue source omitted its critical section; complete target review
caught and corrected that error before acceptance. It now enters/exits section11,
scans31 slots and copies the full pointed string before step0. Its151 bytes and
all8 fields match. A full queue stays unchanged. The19-byte getter's branch
layout still differs despite matching pointer/count behavior; no extra indexed
parameter or fabricated class method is added to force it.

Dispatcher attempts include a bool repeat flag and ordinary shared pointer
(candidate1,520/1,108), a DWORD repeat/busy view (1,552/1,081), the rejected
global-volatile experiment and the retained scoped worker-read context. All
complete target/candidate code and tables remain included. Private helper
calling contexts, stack frame and control-flow scheduling are still open;
source presence and lower diagnostic counts grant no exact credit.

## Reviewed helpers and proof scope

Fifteen called helpers were independently reviewed in full: filename lookup
4496B0/215, prepared load449C70/515, start449E80/291, close449FE0/178,
buffer recreation45A4F0/353 (earlier decoder-open label corrected), refill45A6F0/570, first buffer45AA00/19,
play45AA60/133, volume45AAF0/125, stop45AB70/129, pause45AC00/117,
resume45AC80/77, worker update45AEE0/543, reset position45B100/223 and
record configure45B2D0/130. At this queue checkpoint only getter and volume had body source. The later
[MUSIC_STREAM](MUSIC_STREAM.md) checkpoint implements most streaming/decoder
helpers and supersedes those source-open states. [AUDIO_DEVICE](AUDIO_DEVICE.md)
then reconstructs the full353-byte buffer/notification recreation owner and
corrects its earlier OpenMusicDecoder name; it performs no decoder-file open.
Remaining declarations denote real reviewed boundaries, with no fake
success implementations. All have full RET/reference/neighbor reviews and
no embedded tables or external interior entries.

Canonical new family: `build/match-linked/SoundQueue`. Three complete public
primary-TU contributions replay453 bytes and19 independently reviewed fields:
thread7, enqueue8, volume4. Fields bind runtime4C3280, section11/IAT cells,
music root4C9088, event4C908C, worker45AEE0, music volume4C9168,
double100 at497E50 and real conversion4864E0. Complete current source/header/
tool/producer/PE/MAP/PDB receipts and every byte pass with zero differences.

Separately, adding real AnimationRendererLifecycle support to the existing
renderer-dispatch family improves update candidates from231/180 and232/193
to213/170 and249/195. Native8-byte alignment is still absent; equal secondary
length is not exactness. Both existing exact owners, draw127 and allocator177,
retain full zero replay after this context change. Only these two affected old
owners plus the three new sound units were cold-replayed:5 units total,757
bytes/33 fields. No full cohort, broad behavior suite or infrastructure tests
were run; infrastructure is unchanged.

## Exact 123-byte WAV request producer (2026-11-12)

`QueueMusicWaveRequest` at `0x42A0D0..0x42A14A` now has complete maintained
C++ in `MusicWaveRequest.cpp`. The direct hash-attested Ghidra review and
independent mapped-PE decode agree on all **123 bytes, 34 instructions,
3 calls and a RET8**, with ten original call sites in eight containing
functions and no interior foreign entry or owned table. Its five following
CC bytes belong outside the owner.

The original copies the supplied pathname into a **256-byte stack array**
without extra bounds or null checks, finds the last dot with `strrchr`,
changes the next three bytes to `wav`, calls the queue with opcode 1 and
returns integer 1. It does not force a null terminator after those three
writes. The source retains that exact behavior, including the assumption
that the input contains a dot.

A restricted `/GL` compile with the queue treated as opaque produced
120/123 bytes; including real `SoundQueue.cpp` produced 116/123 because VC9
specialized the fixed opcode and global queue pointer. Linking the **actual
independent dynamic queue producer** `probes/sound-queue-context.cpp` and its
real `MusicStream.cpp` companion preserves the generic audio argument ABI:
**123/123 bytes, 34/34 instructions, 3/3 calls**, and zero nonfield
differences. The complete unique primary-TU PDB contribution has five
independently reviewed original fields: DIR32 security cookie at +7,
REL32 `strrchr` at +0x33, DIR32 queue root at +0x55, REL32
`EnqueueSoundCommand` at +0x5A and REL32 security-check helper at +0x6E.
All five rebind to the original target, leaving **zero differing bytes**.

Canonical linked unit: `queue-music-wave-request`; full cold PE/MAP/PDB
receipt and strict replay log:
`.analysis/music-wave-request-strict-20261112.log`. Focused diagnostic
recipe: `.analysis/probe-music-wave-request-20261112.py`; public producer:
`probes/music-wave-request-context.cpp`. The game-over menu entrance now
uses this real C++ function instead of an unresolved name-only dependency.
This success does not imply the 275/336-byte menu bosses are exact.

## Prepared music streaming startup boss (2026-11-12)

The complete 291-byte `StartPreparedMusic` at 449E80 now has natural
maintained source in `src/PreparedMusicStart.cpp`. Its authentic 92-instruction
owner checks the queue/device readiness, runtime byte +1E3, and prepared
mode at +200. File mode calls the separate 65-byte 449C20 entry using the
indexed cached filename; prepared mode requires the indexed wave buffer,
traces the requested index and uses the saved `SoundTrackRecord` block align
and sample rate to derive the notification chunk size. The source retains
original 32-bit overflow/truncation before rounding down to block alignment.

It creates the auto-reset music event and streaming callback thread with
real `MusicStreamingThread` 44A900 and the runtime context at +58. Then it
uses the existing `SoundDeviceOwnerView::CreateMemoryMusicStream` 45A1B0
with the source buffer, track record, 16-byte native stream GUID, rounded
chunk size and event. Only successful nonnegative HRESULT publishes the
active slot at manager+1F80. The source deliberately does not add a failure
rollback or pointer/index guard missing in the original.

Scoped pinned VC9 `/GL` with real `SoundQueue.cpp`, `MusicStream.cpp` and
independent dynamic queue producer yields the entire **291/291-byte public
primary-TU PDB contribution**, **five calls** and 15 candidate linked
fields. It currently emits 89 vs the original 92 decoded instructions and
224 positional nonfield differences. The alternative queue-only family is
300/291 with 91 instructions. These remain diagnostics; no strict byte
credit is taken. Private producer comparison:
`.analysis/probe-prepared-music-start-20261112.py` and
`.analysis/prepared-music-start-20261112.log`; public independent caller:
`probes/prepared-music-start-context.cpp`. The larger 515-byte prepared load
owner remains separate work.

## Prepared BGM archive load boss (2026-11-12)

The reviewed complete **515-byte** `LoadPreparedMusic` owner at 449C70 now
has maintained source in `src/PreparedMusicLoad.cpp`. This is the actual
load/cache dependency called by `ProcessSoundQueue`, rather than a stub. It
first checks whether a prepared sample exists for the requested index and
whether the incoming name equals the cached one; identical requests return
immediately. Otherwise it copies the path to the indexed 256-byte cache,
then only performs disk loading when runtime byte +200 bit 0x10 and the
manager's prepared-audio gate are both set.

The source retains **four distinct 16-entry tables** at manager offsets
+1E80 (record), +1EC0 (allocated wave), +1F00 (playback wave) and +1F40
(stored bytes). After freeing an old sample, the native logs the requested
index, conditionally enters runtime critical section 2 and opens the
separate archive pathname at +5108 with read/share-read, OPEN_EXISTING and
sequential-scan attributes. Failed open logs the archive path, conditionally
leaves the section and returns -1. The normal path resolves a track through
the real 215-byte `FindSoundTrack`, seeks to that track's data offset,
allocates exactly its stored-byte count, reads it and closes the handle.
Allocation failure closes the file, logs the path and invokes the actual
37-byte `LeaveRuntimeCriticalSection` wrapper. Successful read also invokes
that wrapper and only then publishes the indexed track, two data pointers
and byte count. No unobserved bounds validation, allocation fallback or
ReadFile-status rejection is introduced. The two logger format literals are
independently confirmed at native 497140/49715C.

The full 515-byte target contains **153 instructions, 15 calls and three
RET8 exits**, with no owned table or interior foreign entry. Scoped pinned
VC9 `/GL` builds of the source compile to **489/515 bytes** with an opaque
producer; the authentic `SoundQueue.cpp`, `MusicStream.cpp`,
`RuntimeCritical.cpp` and independent sound-queue caller produce a unique
primary-TU **485/515-byte** contribution. This latter family preserves
**153/153 instructions and all 15/15 calls**, with 27 candidate linked
fields and 336 positional non-address-field differences. It is **non-exact**
and grants no strict byte credit. Local bounded comparison recipe:
`.analysis/probe-prepared-music-load-20261112.py`, public dynamic caller:
`probes/prepared-music-load-context.cpp`. The 291-byte startup owner remains
separate source-present work, and previously accepted queue/streaming exact
functions were not replayed unnecessarily.

## Strict 215-byte soundtrack basename/record lookup (2026-11-12)

The complete target `FindSoundTrack` at `0x4496B0..0x449786` is now
maintained as `src/SoundTrackLookup.cpp`. The original Ghidra direct
function/caller/xref review, independent PE decode and strict linked proof
cover all **215 bytes, 81 instructions, three calls** and its RET4, without
including adjacent padding. The original 3 callers include the prepared
515-byte loader and the 65-byte file-backed path.

The source extracts the final filename after the last `/`, or failing that
tries the last `\\`, copying the suffix (or entire name) into an unchecked
**128-byte local**. It walks 0x34-byte `SoundTrackRecord` names until a match
or the first empty name, returning the matched index or zero when exhausted.
It does not impose a new record-count bound or alter case sensitivity. Ghidra's
inferred `void` return was incorrect; the original callers consume the integer.

A genuine bounded VC9 `/GL` initially yielded **216/215 bytes**. Eight
source variants identified two essential C++ lifetimes: initialize `index`
before the `strrchr` calls, and keep the no-slash filename-copy path before
the suffixed path, preserving the native EDI index and conditional copy loop.
The resulting source was **213 bytes** but still optimized its final ternary
into a branchless sign/bit mask. Expressing the native fallback as
`if (!records[index].name[0]) index = 0; return index;` restored the final
CMP/JNE/XOR/JMP/MOV sequence naturally without forced registers or no-op
instructions. **All 215 bytes and 81 instructions now match exactly**.

The independent strict VC9 primary-TU proof replays all **four** native fields:
DIR32 security cookie `0x4A2088` and REL32 calls to CRT `strrchr` (twice,
`0x460560`) and `__security_check_cookie` (`0x45FC2C`). All original code
bytes and each field match with **zero differences**, current source/tool and
producer receipts. Canonical unit: `find-sound-track`; strict log:
`.analysis/find-sound-track-strict-20261112.log`; public dynamic producer:
`probes/sound-track-lookup-context.cpp`. No unrelated full cold replay was run.

## Original single-byte release tracing leaf (2026-11-12)

The independently reviewed original `TraceSoundQueue` at `0x44AB00` is
**one `RET` byte**, with callers cleaning their own variadic trace arguments.
The complete native function boundary and callers were checked directly;
following `CC` bytes are outside the owner. `SoundQueueTrace.cpp` uses an
ordinary empty variadic C++ body—no inline assembly or injected opcodes.
Its own pinned VC9 **normal-COFF** strict replay accepts the complete
one-byte auxiliary/COMDAT contribution, with **no relocation fields** and
zero differing bytes. Canonical unit: `trace-sound-queue`. This does not
establish original authorship or permit inlining away its parent calls.

## File-backed music startup dependency (2026-11-12)

`StartPreparedMusicFile` at 449C20 now has full **65-byte** source in
`PreparedMusicFile.cpp` with an independent caller in
`probes/prepared-music-file-context.cpp`. The native code tests the active
music controller at queue+5208 and returns -1 if missing. Otherwise it
calls the newly strict-exact 215-byte `FindSoundTrack`, obtains the decoder
at controller+C, passes the selected 0x34-stride `SoundTrackRecord` to
`ConfigureMusicDecoder` with offset zero, traces the index using the true
one-byte `TraceSoundQueue` and returns zero. All **21 target instructions,
three calls and two returns** belong to this complete owner.

The historical opaque-finder/generic-caller VC9 `/GL` diagnostic produced
**74 candidate / 65 native bytes**, 23/21 instructions, 3/3 calls and
four candidate address fields. This result was **non-exact and is now superseded
by the strict real-parent proof below**; its original private ESI manager and ECX
filename scheduling is not faked with incorrect calling conventions.
Private comparison: `.analysis/probe-prepared-music-file-20261112.py`.

The 291-byte `StartPreparedMusic` now calls this actual two-argument C++
boundary with the manager and cached path. Its latest full producer diagnostic
is 292/291 bytes (previously 291/291 using an opaque filename-only callee),
so it also remains non-exact. These C++ changes do not affect the independently
accepted 215-byte finder, 123-byte WAV request or one-byte logger; those two
new exact units were re-cold-replayed together with zero differences.

## Strict exact 65-byte file-backed startup (2026-11-12)

The complete `StartPreparedMusicFile` at `0x449C20..0x449C60` now passes
its own strict linked replay: **65/65 native bytes, 21/21 instructions,
3/3 calls and all four original linkage fields**. The native reads the
manager's controller pointer at `+0x5208`, returns -1 immediately when it
is absent, uses the now-exact `FindSoundTrack` to select a 52-byte WAV
record, passes its decoder at controller `+0x0C` to `ConfigureMusicDecoder`
(`0x45B2D0`), calls the genuine variadic `TraceSoundQueue` (`0x44AB00`),
and returns zero. The normal native return is a plain `RET`, not a `RET4`.

An opaque-callee build produced **78/65**, whereas the true parent
`PreparedMusicStart.cpp` together with exact `SoundTrackLookup.cpp` and
`MusicStream.cpp` causes VC9 `/GL` to allocate the receiver in ESI and the
filename in ECX, restoring the complete native owner naturally without
redeclaring an artificial register ABI. All four independently reviewed
fields replay: the three REL32 calls to the exact track finder, decoder
configuration and trace leaf, and the native trace-format DIR32 at
`0x497124`. The full owner is uniquely public in the source TU's PDB/MAP,
with current compiler/source/producer receipt and zero replay differences.
Canonical unit: `start-prepared-music-file`; log under
`.analysis/start-prepared-music-file-strict-20261112.log`. Only this scoped
producer was cold compiled; the unrelated exact cohort was not replayed.

## Exact 178-byte prepared music shutdown boss (2026-11-12)

`ClosePreparedMusic` at `0x449FE0..0x44A091` now has a complete, naturally
compiled C++ owner in `src/PreparedMusicClose.cpp`. Its source checks the
manager's extended music controller at `+0x5208`, stops playback through the
true `StopMusicStream` (`0x45AB70`, `closeFile=1`), then, if a worker exists,
posts `WM_QUIT` and repeats the original 256-ms `WaitForSingleObject` loop
with another post on each nonzero result. It closes the worker and event
handles at `+0x618/+0x520C`, zeros only the worker field, re-reads the
controller independently and invokes the native virtual scalar-deleting
destructor if present. No additional worker cancellation, wait bound,
exception cleanup or null guard was invented.

Pinned VC9 `/GL` with real `SoundQueue.cpp`, `MusicStream.cpp` and a dynamic
sound queue producer restores the original private ESI manager receiver,
Win32 import pointer loads and virtual deletion. The cold linked full
primary-TU PDB/MAP/PE owner matches all **178/178 bytes, 55/55 instructions,
8/8 call sites**, plus all four independently checked address fields:
`StopMusicStream` REL32, `PostThreadMessageA`, `WaitForSingleObject`,
`CloseHandle` DIR32/IAT roots. The source does not patch code bytes or force
an ABI declaration. The complete matched function has one RET and no
embedded table. Canonical strict unit: `close-prepared-music`; receipts and
comparison are in `.analysis/close-prepared-music-strict-20261112.log`.

The immediately related 515-byte load and 291-byte start bosses retain
complete, independently compiled sources but are **not** declared exact.
Only the closing function's local cold replay was needed here.

## Exact 178-byte prepared music shutdown boss (2026-11-12)

`ClosePreparedMusic` at `0x449FE0..0x44A091` now has maintained, complete
C++ in `src/PreparedMusicClose.cpp`. Its **178 bytes, 55 decoded instructions,
eight calls and ordinary RET** cover the *entire* native owner. It first
checks the manager's current controller at +0x5208, invokes the actual
`StopMusicStream(controller, 1)`, then performs the original background-worker
shutdown if `thread` is nonnull. It sends `WM_QUIT`, calls
`WaitForSingleObject(thread, 256)` and repeats the message/wait cycle until
`WAIT_OBJECT_0`. It closes both the worker handle and the music event at
+0x520C, clears the thread slot, re-reads the controller and invokes the
real virtual scalar-deleting destructor before clearing +0x5208. Neither
invented early termination on wait failure nor extra cleanup is introduced.

An isolated source link emitted 178 bytes but 56 instructions and 47
positional nonfield differences. Adding actual `MusicStream.cpp` restored the
55-instruction shape, while linking its true parent `SoundQueue.cpp` with the
independent dynamic queue caller recovered the native ESI manager register and
all remaining instructions. The complete real-producer contribution is now
**strict 178/178 exact** after pinned VC9 `/GL` cold compilation, unique
primary-TU PE/MAP/PDB ownership and exhaustive independent field replay.
The four original linkages are REL32 `StopMusicStream` (45AB70) and DIR32
IAT entries for `PostThreadMessageA` (48B27C), `WaitForSingleObject`
(48B0F0), and `CloseHandle` (48B0A0). All **four fields and every code byte**
match exactly, with current source/compiler/producer receipts. Canonical unit
`close-prepared-music`; strict evidence:
`.analysis/close-prepared-music-strict-20261112.log`, bounded experiment
`.analysis/probe-prepared-music-close-20261112.py` and public dynamic producer
`probes/prepared-music-close-context.cpp`. No full cold replay was run.
