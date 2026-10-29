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
fade14/18/1C, active30, decoderC and cross-thread busy78. Decoder record90 has
loop information at1C within a34-byte record. Size/offset assertions check the
observed layouts. Its slot0 scalar-deleting destructor is modeled by a real
virtual destructor declaration and virtual delete call, without an invented
implementation.

The pinned SDK payload lacks dsound.h. `SoundBuffer.hpp` therefore models the
observed COM vtable directly: Play30, SetCurrentPosition34, SetVolume3C,
SetPan40 and Stop48, with correct stack receiver/arguments and HRESULT returns.
Unobserved slots stay opaque and are never invoked. It embeds no target bytes
and does not invent signatures for those slots. The125-byte volume proof
independently covers the resulting real indirect call sequence.

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
decoder open45A4F0/353, refill45A6F0/570, first buffer45AA00/19,
play45AA60/133, volume45AAF0/125, stop45AB70/129, pause45AC00/117,
resume45AC80/77, worker update45AEE0/543, reset position45B100/223 and
record configure45B2D0/130. Only getter and volume have body source here.
The other declarations denote these real reviewed boundaries, with no fake
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

Private evidence: `anm-binding-large-full-review.txt` (the initial location-based
name predates audio identification), `anm-binding-neighbors-review.txt`,
`audio-queue-helper-review.txt`, `audio-queue-table-references.txt` and their full
mapped-text attestations; `sound-queue-independent-review.json` and
`sound-queue-independent-bindings.json`; baseline/locked-callback/shared-busy/
shared-pointer/scoped-shared-read compiler logs; `renderer-dispatch-real-reclaim.log`;
`sound-queue-and-renderer-affected-cold.log`. Target bytes, decompiler hypotheses,
tools, build artifacts and mutable Ghidra data remain private.
