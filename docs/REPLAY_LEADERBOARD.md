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
fields, but VC9's initial unlinked candidate expands the initialization to
1,234 bytes instead of the native 264. The typed code is source-present;
loop and register scheduling still require recovery.

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
