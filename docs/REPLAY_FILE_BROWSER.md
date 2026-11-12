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
