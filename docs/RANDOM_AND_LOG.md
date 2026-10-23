# Random generation and log append

Direct Ghidra12.1.3 review of the attested Japanese TH11 v1.00a covers three
complete contiguous functions. Each has following CC padding, no embedded
data and no outside references to its interior.

| Source name | Target extent | Owned bytes |
| --- | --- | ---: |
| `AppendLog` | 0x00458A10..0x00458AE2 | 211 |
| `RandomNext16` | 0x00458BC0..0x00458C28 | 105 |
| `RandomNext32` | 0x00458C30..0x00458CC8 | 153 |

Replayable private review:

```bash
scripts/repo-python scripts/ghidra.py query .analysis/screenshot-rng-review.txt review \
  0x00429CA0 0x00445460 0x00458BC0 0x00458C30
scripts/repo-python scripts/ghidra.py query .analysis/screenshot-dependencies-review.txt review \
  0x00458A10 0x0042B8D0 0x00429BB0
```

## Random state and results

The receiver has a16-bit seed at+0 and32-bit call count at+4. Bytes+2/+3
remain unknown and untouched. Define a16-bit intermediate
`t = (seed ^ 0x9630) - 0x6553` and rotate it left by two bits to update the
seed. The addition/shift expression truncates back to16 bits.

`RandomNext16` increments the count once, advances the state once and returns
the stored rotated seed. When locking is enabled, it reloads that seed after
unlocking. `RandomNext32` increments the count by two, performs two advances
under one lock, and returns `(firstIntermediate << 16) | secondIntermediate`.
Its halves are the values before rotation; replacing it with two calls to
`RandomNext16` changes both output and synchronization.

Runtime flag0x8000 controls section10 at0x004C3B80 and depth byte10 at
0x004C3BBA. Source uses the established MainRuntime root plus+0x900/+0x93A.
The32-bit generator has19 reviewed game callers, including gameplay and
effect routines; its custom state/counter and shared game locking distinguish
it from an unclassified library routine. The16-bit generator is called by
the nearby bounded wrapper at0x00458D70.

Each canonical unit replays eight DIR32 fields. For the16-bit function the
offsets are2,13,19,25,70,81,87,93; for32-bit they are2,13,19,25,112,123,129,135.
The fields bind flags, section10, EnterCriticalSection IAT0x0048B084, depth10,
flags, section10, LeaveCriticalSection IAT0x0048B088 and depth10, respectively.

## Log buffer

`AppendLog` uses an8192-byte stack buffer and vsprintf. It computes the
formatted length, then appends only when `end + length < text + 8191`.
The owner's end pointer is at+0x2000; a successful append advances it by the
length and writes a null terminator. The original formatting buffer has no
additional size check. The function returns its original format pointer in
EAX, as established by the final stack-argument load.

Flag0x8000 controls section3 at0x004C3AD8 and depth3 at0x004C3BB3. Its custom
buffer ownership and21 reviewed game callers, including screenshot/error
paths, support authored-game attribution. Ordinary cdecl variadic source lets
VC9 /GL pass the receiver in ECX while leaving format and varargs on the
stack. No custom calling convention was imposed.

The twelve exhaustive field bindings are:

| Offset | Type | Meaning and target |
| ---: | --- | --- |
| 6 | REL32 | Stack probe0x00480360 |
| 11 | DIR32 | Security cookie0x004A2088 |
| 26,154 | DIR32 | MainRuntime flags, root0x004C3280+0x590 |
| 41,167 | DIR32 | Section3, root+0x858 |
| 47 | DIR32 | EnterCriticalSection IAT0x0048B084 |
| 53,179 | DIR32 | Depth3, root+0x933 |
| 79 | REL32 | vsprintf0x0045FDD2 |
| 173 | DIR32 | LeaveCriticalSection IAT0x0048B088 |
| 200 | REL32 | Security cookie check0x0045FC2C |

## Exact compilation

All three complete public primary-TU PDB contributions pass the cold linked-PE
Oracle with current source/header/tool/producer/PE/MAP/PDB receipts, exhaustive
field replay and zero byte differences. Profile:
`/TP /O2 /Ob1 /Gy /MT /Isrc /GL`, pinned VC9 RTM.

The two random units share one source and independent caller context, so their
cold replay builds that family once. The log unit has a separate independent
variadic caller. Only these three new canonical units were replayed; the
preceding thirteen canonical proofs were not replayed. Original production TU partitions and
full native game closure remain open.
