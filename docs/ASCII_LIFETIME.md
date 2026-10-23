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
| `AsciiManagerAppendString` | `0x004014E0..0x00401595`, 182 bytes | ESI manager, EBX position, ECX text; RET; following CC padding; no external interior references |
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

`src/AsciiAppend.cpp` maintains the queue appender. Its signed count check
rejects counts at or above320; accepted records receive the null-terminated
text,12-byte position, color, two scalar float scales, four observed style
fields and lifetime. Record+0x118 is untouched. The style stores deliberately
copy manager+0x18498 before+0x18494, as observed. The two callers at0x004015A0
and0x00401600 format text before calling it; the appender itself has no calls.
This preserves the original copy behavior and does not add bounds checks.

## Code generation

Initial normal VC9 RTM `/TP /O2 /Gy /MT /Isrc` callback probing reproduces all
18 nonrelocated bytes, a complete 22-byte COFF function, and the expected sole
REL32. Two independent canonical receipt-bound cold replays pass all 22 bytes after replaying
the independently reviewed REL32, with zero differences. The exact context is
recorded in `config/match-units.toml` and `config/matches.csv`.

The appender passes a cold receipt-bound VC9 RTM linked-PE proof over its
complete182-byte public primary-TU PDB contribution. All182 bytes equal the
target directly, with no relocation/linkage fields. Ordinary stdcall source
and the independent two-call context in `probes/ascii-append-context.cpp`
naturally produce the observed private registers under
`/TP /O2 /Ob1 /Gy /MT /Isrc /GL`. The original production TU remains unknown.
Only `ascii-manager-append-string` was cold-replayed for this addition; the
unchanged callback and previous exact units were not rebuilt. Boundary review
is reproducible with `ghidra.py query ... review 0x004014E0`.

The expiry candidate is **non-exact**: its normal COFF extent is 100 bytes,
compared with the target's 124 bytes. A natural explicit pointer-compaction
probe also produces 100 bytes. No instruction padding, fake paths, target byte
arrays, or ABI changes were introduced to bridge that difference. Original
TU partition, optimizer visibility, and LTCG ownership remain unknown; the
next codegen investigation should compare declared compiler contexts and a
VC9 linked-image probe rather than claim byte equality from behavior alone.

These function proofs do not establish a complete Windows product or runtime validation.
