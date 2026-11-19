# Secondary replay stage lifecycle

All functions below are plain C++ (rewritten 2026-11-14 from withdrawn
`__asm` copies). The integer-position helper now has strict exact proof;
the larger replay lifecycle functions remain nonexact; `config/functions.csv` notes hold the
current measurements.

| Function | Address | Bytes | Aligned | Remaining gap |
| --- | --- | ---: | --- | --- |
| `ReplayRecordRegistryView::InitializeRecord` | 4356E0 | 1380 | 305/406 | stage-pointer reload after the store; snapshot store scheduling |
| `LoadReplayStageData` | 436B60 | 441 | 131/137 | one-step temp-register rotation around the `malloc` argument |
| `UpdateSecondaryReplayStage` | 436DA0 | 388 | 47/108 | native keeps the state in ESI (copying to EBX for the append) |
| `InitializeReplayHeaderStorage` | 420D80 | 137 | 30/40 | zero-byte and version stores scheduled after the default copy |
| `SetSecondaryIntegerPlayerPosition` | 435630 | 110 | 22/22 | Exact (cold strict linked-PE zero byte differences, 2 DIR32 fields) |

## Initializer 4356E0

A thiscall member of the registry at 0x4A8D88. LTCG passes the mode in EAX;
in modes 1 and 2 `this` (whose first member is the replay filename) is the
loader's source argument. Mode 0 clears and appends the current stage's
segment list, creates the file header (memset constructor: magic `t11r`,
version 4, flags 0x100), the 0x70-byte header (its settings member constructor
runs `InitializeReplayHeaderStorage`, then the header body memsets the whole
object), and the 0x90-byte stage record. It then snapshots the stage
state, fills 20 key words with `i * 0xDEADDEAD`, and registers update,
auxiliary and draw chains at priorities 0x0B/0x1B/0x38. Mode 1 loads,
restores the settings block into `g_SecondaryDialogueSession`, copies the
stage-link cursors and restores the player state; mode 2 only loads.

## Loader 436B60

Copies the filename to `state+0x1DC`. Without status bit 0x20 it reads
`replay/<name>` through the shared reader, checks `t11r`/version 4 with two
separate failure exits, and reads the packed payload. With the bit set it loads
the archived copy through `LoadResourceFile(source, &size, 0)`. It then
decrypts twice, decompresses and indexes each stage link. The link must include
the real file, resource, cipher and archive sources so LTCG gives those helpers
their native ESI/EDI/EAX conventions.

## Stage lifecycle 436DA0

Marks the three replay chains, then either clears and appends a recording
segment and snapshots the stage record, or restores the integer position, the
focus and special fields and resets the emitters. Both paths zero the frame
counter.

## Layout

`src/SecondaryReplayState.hpp` describes the shared replay state. Recording
stage records are at +0x1C, segment list heads `{segment, next, previous}` at
0x3C + 12 * stage, the current tail node at +0x9C, the segment count at +0xA0
and the playback stage links at +0xA4. `g_SecondaryReplayCurrent` (0x4A8EB8)
is the replay state; `g_SecondaryControllerMode` (0x4A8EB4) is the player
controller.

## Exact integer player-position helper (2026-11-19)

The complete original 110-byte `SetSecondaryIntegerPlayerPosition` owner at
0x435630 now matches the pinned VC9 `/GL` primary-TU contribution: 22 native
instructions, all 110 bytes, and both reviewed absolute fields at +1
(`g_SecondaryControllerMode`, 0x4A8EB4) and +0x1E (double 1/128, 0x497ED0).
The C++ publishes two distinct controller integer coordinates through typed
volatile member references (to preserve publication order), then reads them for x87 conversion and marks all
eight 0xE4-stride item slots active. Expressing the publication as one ordinary
aggregate assignment lets VC9 start the X conversion before the Y store,
which is different from the target. The ordered member writes recover the
native two-store boundary, while keeping the actual type, 1/128 arithmetic,
and item loop. This is natural C++ with no assembly or padding.

Replay only `secondary-integer-player-position` for this owner; its original
C++ class identity and actual translation-unit partition remain unknown.
