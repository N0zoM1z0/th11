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
shorter and reordered the FPU and flag stores. The canonical replay unit is
`secondary-integer-player-position`; exactness is not claimed until its fresh
Factory receipt is accepted.

## Stage-lifecycle owner (`0x436DA0`, 388 bytes)

The adjacent owner is a separate stage-boundary operation, not part of the
1380-byte initializer. It marks the update, auxiliary-update, and draw chain
nodes pending; for recording mode it clears the selected stage stream, appends
a write segment, captures the controller's score/power/lives/bomb/flag state,
sets the recording-stage index, and resets the frame counter. For playback
mode it obtains the selected stage payload from the embedded stage-link slot,
restores the integer position through `SetSecondaryIntegerPlayerPosition`,
restores the controller's saved fields, conditionally clears the emitter slot,
and resets the frame counter. Its original boundary and data references have
been inspected; source reconstruction remains in progress.

## Main initializer (`0x4356E0`, 1380 bytes)

`InitializeSecondaryReplay` remains a distinct three-mode owner (record,
playback, validation). Its best existing scoped `/GL` candidate is 1247 bytes
against 1380 native bytes, retaining 22 calls and four returns. The source and
producer context are still non-exact; the new leaf and stage-lifecycle work
are intended to close the shared replay-state and stream-call paths rather
than treating the initializer as an isolated function.
