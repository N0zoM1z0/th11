# ASCII queue lifetime and update callback

Reviewed from the original TH11 target on 2026-10-23 with direct Ghidra 12.1.3.
The database passed complete mapped `.text` SHA-256 attestation
`6e5cb58c1434ef85c1022c1e4629160d0e942910660fa4adbc179c4ea9077f99`
for 564,757 bytes. Private replayable query:

```bash
scripts/repo-python scripts/ghidra.py query .analysis/ascii-review.txt review \
  0x004014A0 0x00401D50 0x00401080
```

## Boundaries and bindings

| Source name | Target extent | Boundary/ABI observations |
| --- | --- | --- |
| `AsciiManagerOnUpdate` | `0x004014A0..0x004014B5`, 22 bytes | One contiguous body; receiver ECX, return EAX=1, RET; following CC padding |
| `AsciiManagerExpireStrings` | `0x00401D50..0x00401DCB`, 124 bytes | One contiguous body; one stack receiver, RET 4; following CC padding |

Ghidra's reference manager reports no outside references to either body's
interior. The update entry is referenced as data by the callback registration
in initialization at `0x004010F4`. The update's direct CALL at `0x004014A4`
resolves to `0x00401D50`. This independently identifies the meaning and target
of the callback object's sole REL32 field at function offset 5.

Default Ghidra analysis misses the separate internal-ESI wrapper at
`0x00401480`; it is not silently merged into the ECX callback's extent.
The current inventory remains provisional for this reason and others.

## Behavior and source

Appender `0x004014E0` establishes 320 0x130-byte records at manager +0x87C and
count +0x1847C. The expiry function decrements the signed lifetime at record
+0x12C and stably compacts nonnegative survivors with whole-record copies.
A starting lifetime of zero is removed on the next update. The callback then
increments the manager frame counter at +0x184A8 and returns 1.

`src/AsciiManager.hpp` is a partial layout view with explicit unknown storage;
it does not identify all data owners. `src/AsciiStrings.cpp` maintains the
expiry behavior, and `src/AsciiUpdate.cpp` maintains the callback.

## Code generation

Initial normal VC9 RTM `/TP /O2 /Gy /MT /Isrc` callback probing reproduces all
18 nonrelocated bytes, a complete 22-byte COFF function, and the expected sole
REL32. Two independent canonical receipt-bound cold replays pass all 22 bytes after replaying
the independently reviewed REL32, with zero differences. The exact context is
recorded in `config/match-units.toml` and `config/matches.csv`.

The expiry candidate is **non-exact**: its normal COFF extent is 100 bytes,
compared with the target's 124 bytes. A natural explicit pointer-compaction
probe also produces 100 bytes. No instruction padding, fake paths, target byte
arrays, or ABI changes were introduced to bridge that difference. Original
TU partition, optimizer visibility, and LTCG ownership remain unknown; the
next codegen investigation should compare declared compiler contexts and a
VC9 linked-image probe rather than claim byte equality from behavior alone.

Neither function establishes a complete Windows product or runtime validation.
