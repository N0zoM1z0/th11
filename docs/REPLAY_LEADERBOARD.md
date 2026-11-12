# TH11 replay score and random-seed archives (2026-11-12)

Two substantial neighboring native owners create the CR and ST binary
archives. Source now lives in `src/ReplayLeaderboardInitialization.cpp`,
with measured `0x68D4` and `0x448` byte storage views. Their original nominal
class and translation-unit names remain unknown.

At `0x437550..0x437657` the **264-byte CR initializer** clears 0x68D4
bytes, publishes the `CR` header and size, fills five ten-entry score blocks
with descending one-million-to-one-hundred-thousand starting scores, and
populates 35 groups of five indexed character labels from the real source
tables. The full native span includes the seven-byte `LEA ESP,[ESP]` at
`0x437589..0x43758F`, independently reviewed rather than cropped from the
owner. Maintained natural C++ implements both record loops and all physical
fields. The first indexed loop structure expanded to 1,234 bytes under VC9;
subsequent native-shaped pointer loops now recover the **complete 264-byte**
owner, with only two bytewise addressing-order differences described below.

At `0x437660..0x437736` the **215-byte ST initializer** clears 0x448
bytes, publishes the `ST` header, size and nine blank name bytes, then writes
**512 separate 16-bit random samples** beginning at offset `0x46`. Each
sample uses the genuine text random seed at `0x4C2F00`, increments its call
counter and enters/leaves runtime critical-section slot 10 when locking is
enabled. An initial index-based loop emitted 205 bytes and changed lock
register lifetimes. A pointer-increment and decrementing 512-step loop
naturally restores the complete native loop. Linking one real dynamic-archive
caller under pinned VC9 `/GL` also recovers the private EAX receiver.

The canonical `initialize-replay-seed-archive` linked unit now has a complete
unique public **215/215-byte** PDB/MAP owner, 57 native instructions, three
calls and **16 independently reviewed address and call fields** (random
state, runtime lock/root offsets, imports, `memset`, blank name). Their
full replay produces zero byte differences; neither register hacks nor
synthetic instruction padding are needed. The CR initializer remains a
nonexact larger follow-up.

## 264-byte score initializer narrowed to two SIB bytes

The refined `InitializeReplayScoreArchive` no longer uses the nested
`category*10+rank` index expression, which VC9 expanded into long unrolled
code. A typed byte pointer, five groups of ten scores with a 100,000-step
countdown, and explicit 4+4+1 byte copies of the real nine-byte default
name naturally recreate the original first loop and its seven-byte alignment
`LEA ESP,[ESP]`. Reusing the exhausted five-group counter as the index for
the following 35 groups of five labels removes the last extra zeroing and
restores the complete second-loop layout.

In the pinned VC9 `/GL` family with a genuine dynamic archive caller, this
now has **264/264 bytes, 73/73 instructions**, all nine decoded address
fields and only **two non-address byte differences**. Both are SIB-byte
base/index selection in equivalent `MOVSX` reads at offsets `+0x9B` and
`+0xBD` (`ECX+EDI` vs `EDI+ECX`, and `EDX+EBX` vs `EBX+EDX`). Seven
semantically equivalent natural array/pointer spellings preserve the same
compiler choice. Therefore this boss is **source-present but not strict
exact**; the function is never shortened, padded or credited with a masked
match. Evidence is in `.analysis/probe-replay-score-last-sib-20261112.log`.

The neighboring canonical 215-byte seed initializer was cold-replayed after
this shared source-file change and retained **zero differences**.

## Full leaderboard runtime / resource chain

Three native owners immediately above the seed and score initializers are
now maintained in `ReplayLeaderboardRuntime.cpp`. The physical storage is
`0x2E21C` bytes: source buffer pointer and decoded pointer at +0/+4,
**seven 0x68D4-byte score archives beginning at +8**, and the **0x448-byte
seed archive at +0x2DDD4**. All three offsets and total size are asserted.

| Owner | Native length | Scoped VC9 /GL candidate | Function |
| --- | ---: | ---: | --- |
| `0x437740` | 138 | 151 | Construct full storage, load file, fill seed and seven score defaults, parse input |
| `0x437800` | 96 | **96/96 full codegen**, private EH-table field unproven | Allocate the record and publish its singleton |
| `0x4378F0` | 381 | 355 | Decode and validate persisted score/seed records; fall back to fresh header |

The 138-byte constructor's source preserves the native *pointer-difference*
index calculation in the seven-score loop (`(entry - scores) / 0x68D4`),
instead of replacing it with a simple number-of-entries loop. The underlying
primitive score/seed initialization functions remain independent and are
called for each archive. The 381-byte loader uses the existing true resource,
byte transform, decompression and exact 79-byte replay checksum sources:
it checks the `TH11` container header, `CR`/`ST` record signatures, version,
size and payload checksums, and copies valid records to their correct slots.
On missing or invalid data it allocates a new 0x18-byte header.

All three functions compile under pinned VC9 and have genuine primary-TU
PDB/MAP contributions. None has a strict full-byte match yet: the constructor
needs original private EBX receiver/quotient scheduling; the 96-byte allocator
has native EH register/handler differences; the 381-byte loader has different
allocation, decompression and loop ordering. The four bounded producer
variants and pointer-subtraction variants are recorded in
`.analysis/probe-replay-leaderboard-runtime-20261112.log` and
`.analysis/probe-leaderboard-ctor-pointer-20261112.log`. No full-game replay
was run, and no unproven exact credit was added.

## Scoped leaderboard producer follow-up (2026-11-12)

The complete `0x437740` source now uses an explicit byte cursor and the
`(cursor - base - 8) / 0x68D4` quotient observed in the target. This keeps
seven real score tables and their own initializer calls, plus the real
resource loader and 512-word seed table. A pinned VC9 `/GL` dynamic-owner
producer yields 151/138 bytes for the constructor: its private EBX receiver
and quotient control-flow scheduling remain open. The genuine `0x4378F0`
record loader remains 352–355/381 bytes, while preserving resource decode,
checksum calls and fallback file-header ownership.

The same pointer-driven constructor context is important for the separate
`0x437800` allocator: it restores the original EBX receiver allocation and
all **96/96 code bytes** (29 instructions, two calls). The original function
registers an exception handler at `0x48A30B` whose linked counterpart has
no unique public MAP symbol. Consequently this is a byte-complete *diagnostic*,
not a canonical exact unit. The constructor variant matrix and complete
PDB/MAP producer checks are retained in
`.analysis/probe-leaderboard-constructor-loops-20261112.log` and
`.analysis/probe-leaderboard-ctor-initial-byte-diff-20261112.log`.
