# Replay file browser lifetime (2026-11-12)

The original TH11 v1.00a replay file browser is distinct from the leaderboard
resource manager. It owns the singleton at **0x4A8EC0**; the leaderboard's
0x2E21C-byte resource manager uses **0x4A8EBC**. Its physical storage has
update/draw callback pointers at `+8/+0xC` and an optional allocation at
`+0x418`. Neither its original class name nor its complete data layout is
established.

The **273-byte** destructor at `0x437E40..0x437F50` is implemented in
`ReplayFileBrowserLifetime.cpp`. It independently tests and unregisters the
two callback chains, entering/leaving the observed runtime critical section
around each removal when enabled. It then clears the singleton, frees the
optional data allocation, and zeroes the pointer. Direct Ghidra review
established the entire EH-bearing original range and its calls. The first
natural VC9 normal-COFF source is 223 bytes, with 21 relocation fields,
so this owner remains nonexact. Its distinct EH frame, stack parameter and
private register scheduling need follow-up.

The smaller but complete **28-byte** `DeleteReplayFileBrowser` at
`0x438000..0x43801B` loads the singleton, conditionally invokes the entire
273-byte destructor and calls scalar `operator delete`. The destructor (not
the leaf) clears the global. The pinned VC9 RTM normal-COFF unit
`replay-file-browser-delete-singleton` reproduces all **28/28 bytes**. Its
three original code fields are independently grounded: DIR32 +3 to the
browser singleton `0x4A8EC0`, REL32 +0xD to the full destructor `0x437E40`,
and REL32 +0x13 to scalar delete `0x45FD49`. Full cold replay confirms zero
differences. This exact claim does not extend to the 273-byte callee.

The neighboring original `0x437D50` (230 bytes) registers both update/draw
chains, and `0x437F60` (117 bytes) constructs and starts the browser. Their
native boundaries are reviewed and remain separate larger owners to recover.

## Complete native registration and startup owners (2026-11-12)

Both larger browser entry points are now maintained, rather than leaving the
singleton release leaf as the only source in the lifecycle family:

- **230-byte registration `0x437D50..0x437E35`:** Reads the active ASCII
  animation document, creates two complete 0x24-byte callback chain nodes,
  registers update priority 15 and draw priority 41, resets the embedded VM at
  `+0x18`, publishes the document pointer at `+0x3C8`, and binds sprite
  `0xC4`. Both callback pointers belong to the browser, not the unrelated
  secondary program browser. Original complete six-call boundary, next CC
  padding and lack of foreign interior entries are independently reviewed.
- **117-byte startup `0x437F60..0x437FD4`:** Allocates 0x78C bytes, calls
  the independently exact VM constructor `0x402270` at owner `+0x18`, resets
  13 stride-0x40 flag words beginning `+0x478`, zeros the complete owner,
  publishes the singleton, registers the actual callback chains, and on
  failure calls the 273-byte destructor and scalar delete. Original six-call
  boundary, both returns and following CC are reviewed.

Current natural C++ implementations in `ReplayFileBrowserSetup.cpp` and
`ReplayFileBrowserStartup.cpp` compile with pinned VC9. Normal COFF emits
227/230 and 204/117 bytes; scoped genuine `/GL` experiments with separate
chain-registration, VM/sprite-binding and VM-constructor producers yield
225/230 and 119/117 bytes respectively. The extra startup code contains a
null guard on placement construction and differing EDI/ESI register selection;
its native callback owner's private EAX receiver is not established. Neither
body is exact; the exact singleton leaf remains a distinct 28-byte owner.
Scoped comparison evidence: `.analysis/probe-replay-file-browser-scopes-20261112.log`.
