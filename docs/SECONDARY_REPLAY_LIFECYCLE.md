# Secondary replay stage lifecycle

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

## Stage-lifecycle owner (`0x436DA0`, 388 bytes)

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

The native body is 388 bytes / 108 instructions / five calls / two returns.
The focused VC9 RTM `/GL` candidate is 377 bytes / 106 instructions with the
same five calls and two returns. It is a complete semantic source reconstruction
but remains non-exact; the remaining 11-byte gap is code generation, not a
missing mode branch. The canonical target boundary is reviewed and the
original class/TU provenance remains unknown.

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

## Main initializer (`0x4356E0`, 1380 bytes)

`InitializeSecondaryReplay` remains a distinct three-mode owner (record,
playback, validation). The current focused `/GL` candidates are 1257 bytes in
the independent context, 1245 with the real chain-registration dependency,
and 1249 with the real writer dependency, against 1380 native bytes. Each
retains 22 calls and four returns. The source and producer contexts remain
non-exact; the typed stage-link restore and shared replay-state work now close
important data-flow gaps without treating the initializer as an isolated
function.
