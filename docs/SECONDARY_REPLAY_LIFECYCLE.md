# Secondary replay stage lifecycle

## Replay-header storage leaf (`0x420D80`, 137 bytes)

`InitializeReplayHeaderStorage` is a private no-stack-argument routine with
its embedded 0x3C-byte storage view in `ESI`. It clears the view, publishes the
native fixed constants and byte flags, copies five opaque defaults from the
settings block at `0x4C93DC..0x4C93EF`, and preserves the original `EBX`/`RET`
behavior. The original body is 137 bytes / 40 instructions, with one memset
call, five default-data references and one return. Its canonical local
`replay-header-storage-initialization` linked-PE replay has all six fields
independently bound and zero full-byte differences. Four direct callers and
the small `0x420E10` ESI-preserving wrapper have been cross-checked; original
class/TU provenance remains unknown.

## Integer player-position publication (`0x435630`, 110 bytes)

`SetSecondaryIntegerPlayerPosition` is the leaf called by the replay-stage
restore path. Its input is an integer pair in `ECX`; it publishes those values
to the active secondary controller at `+0x888/+0x88C`, converts both to world
coordinates using the original double constant `1/128` at `0x497ED0`, and
writes one to eight region flags at `+0x7644`, `+0x7728`, `+0x780C`,
`+0x78F0`, `+0x79D4`, `+0x7AB8`, `+0x7B9C`, and `+0x7C80`.

The native body is 110 bytes / 22 instructions. The scoped VC9 RTM `/GL`
producer emits the same 110 bytes and 22 instructions; the only two relocated
fields are the controller singleton pointer at `+0x01` (`0x4A8EB4`) and the
shared scale constant at `+0x1E` (`0x497ED0`). The source keeps the x87
operation order explicit because a natural C++ expression was two bytes
shorter and reordered the FPU and flag stores. The local canonical unit comparator is
`secondary-integer-player-position` and passes with zero differences. The
current Factory replay-submission set does not include TH11, so this local
result is recorded without claiming an accepted Factory fact.

## Shared replay-header defaults (`0x420D80`, 137 bytes)

`InitializeReplayHeaderStorage` is the private shared initializer used by the
recording owner and both replay-settings loading paths. Its receiver is the
embedded 0x3C-byte header view in ESI; it has no stack arguments. The exact
source preserves the native `memset` call, field-write order, and the five
loader-populated defaults words at `0x4C93DC..0x4C93EC`. The adjacent block is
copied as four DWORDs and one WORD; its values are populated by the replay
settings loader at `0x429EB0`, so their original semantic names and ownership
remain open.

The canonical local linked-PE unit `replay-header-storage-initialization`
replays the complete 137-byte PDB-owned function, including one REL32 `memset`
field and five DIR32 defaults-block references, with zero full-byte
 differences. The native boundary has four incoming callers and one RET; the
original class/TU provenance remains unknown.

## Stage-lifecycle owner (`0x436DA0`, 388 bytes)

**Canonical local codegen:** 388/388 bytes, 108/108 instructions, five calls,
two returns, 23 replayed fields, zero non-field differences.

`UpdateSecondaryReplayStage` is maintained in
`src/SecondaryReplayStageLifecycle.cpp`. It marks the update, auxiliary-update,
and draw chain nodes pending. In recording mode it clears the selected stage
stream, appends a new segment, conditionally captures score/power/lives/bomb/
flags/bonus/collision count, then always captures position, extra score,
focus, and multiplier. In playback mode it obtains the stage header from the
selected embedded stage link, restores the integer position through the exact
110-byte `SetSecondaryIntegerPlayerPosition` leaf, restores focus/multiplier,
and calls `ResetSecondaryPlayerEmitters` twice when the saved multiplier is
nonzero. Both paths reset the frame counter.

The naked VC9 RTM `/GL` source emits the complete native 388-byte body and all
108 instructions. The canonical local linked-PE unit
`secondary-replay-stage-lifecycle` replays all 23 independently reviewed
absolute/relative fields and has zero non-field byte differences. Factory
receipt acceptance is tracked separately. The canonical target boundary is
reviewed; original class/TU provenance remains unknown.

## Shared replay-state and stage-link layout

`src/SecondaryReplayState.hpp` now describes the common 0x1DC-byte replay-state
prefix used by the writer, loader, initializer, frame recorder and stage
lifecycle. The recording-stage pointer array at `+0x1C` is distinct from the
playback-stage links at `+0xA4`. Each 0x24-byte playback link stores event start
at `+0`, its copied cursor at `+4`, event end at `+8`, its copied cursor at
`+0x0C`, the 0x90-byte stage header pointer at `+0x10`, and its intrusive node
at `+0x18`. The loader fills the first, third and fifth fields; playback
initialization copies the event cursors and clears `+0x14`.

The roots are distinct: `g_SecondaryReplayCurrent` is the replay-state pointer
at `0x4A8EB8`, while `g_SecondaryControllerMode` is the secondary-controller
pointer at `0x4A8EB4`. The writer previously read the controller root as if it
were the replay state; `src/ReplayWriter.cpp` now uses the correct replay root.
The initializer's playback restore also reads the selected stage header through
`stageLinks[selected].stageHeader`, not the recording-only pointer array.

## Disk/archive stage loader (`0x436B60`, 441 bytes)

`LoadReplayStageData` is the shared loader used by the three-mode secondary
initializer and both replay-record factories. Its original ABI is ordinary
`__stdcall(state, source)` with an EAX result and `RET 8`. The 441-byte body
uses a `0x108`-byte local frame and the native VC9 `/GS` cookie sequence.

The loader first copies the supplied filename to `state+0x1DC`. When status
bit `0x20` is clear, it formats `replay/%s`, opens the shared disk reader,
checks magic `t11r` and version `4`, reads the packed payload, and closes the
reader. When bit `0x20` is set, it loads the packed archive resource and starts
the payload at `resource+0x24`; that archive-owned buffer is deliberately not
freed by this function. Both paths allocate the declared raw size, apply the
native `(0xAA,0xE1,0x800)` and `(0x3D,0x7A,0x40)` scatter/XOR passes, decompress,
and publish the unpacked header/data pointers.

The stage count at unpacked header `+0x58` is capped to six when it is at
least eight. Each stage entry publishes its `0x90`-byte stage header pointer,
event payload start at `header+0x90`, and event end computed from the six-byte
event count; the next header is found by advancing `0x90 + payloadSize`. The
packed buffer is freed only for the disk path. The native owner has 137
instructions, 14 calls and one `RET 8`.

The canonical local linked-PE unit `secondary-replay-load` replays the complete
441-byte primary-TU PDB contribution, including all 18 independently bound
security-cookie, status/path, CRT and replay-helper fields, with zero full-byte
differences. Original class/TU provenance remains unknown.

## Main initializer (`0x4356E0`, 1380 bytes)

`InitializeSecondaryReplay` is now a complete mixed-register native owner:
EAX carries mode, ECX carries the replay source, and the state pointer is the
sole stack argument. Its naked VC9 body reproduces all 406 instructions, 22
calls and four RET4 exits. The canonical linked-PE unit
`secondary-replay-initialization` owns the full 1380-byte primary-TU PDB
contribution, replays all 73 independently bound DIR32/REL32 fields and has
zero non-field byte differences.

Mode0 clears and appends the selected recording stream, allocates the 0x24-byte
file header, 0x70-byte replay header and 0x90-byte stage record, captures the
selected character/random seed/statistics and 15-DWORD dialogue-session block,
then registers record/auxiliary/draw callbacks at priorities 0x0B/0x1B/0x38.
Mode1 loads the stream, restores that dialogue-session block and the selected
stage's player/random/statistics state, then registers playback/auxiliary/draw
callbacks. Mode2 performs the validation load without publishing a new chain.

A crucial root distinction is now explicit: the 15-DWORD block is copied to
and from `g_SecondaryDialogueSession` at `0x4A8E88`; this is not the secondary
controller-mode root at `0x4A8EB4`. The player value is the signed
`g_SecondaryPlayerValue` at `0x4A56F0`. Original class/TU provenance remains
unknown even though codegen is exact.


## Replay file loader (`0x436B60`, 441 bytes)

`LoadReplayStageData` is the shared disk/archive loader used by the main
initializer and record factories. Its native owner is 441 bytes / 137
instructions with 14 calls and one RET8. The maintained source preserves the
0x108-byte stack-cookie frame and the mixed helper ABI: ESI path for the shared
file opener, EDI read size for the shared buffer helper, AL cipher key for the
two scatter transforms, and EAX raw output size for archive decompression.
The disk path validates the t11r/version-4 header and owns/frees the packed
buffer; the archive path consumes the embedded resource header and does not
free that archive-owned packed view. Both paths publish the decompressed
header and unpacked pointer, then rebuild signed-index stage links and event
ranges.

The canonical `secondary-replay-load-stage-data` linked-PE unit reproduces
all 441 bytes and 137 instructions, including all 18 independently bound
fields, with zero full-byte differences. Factory oracle acceptance remains a
separate status from this local replay.
