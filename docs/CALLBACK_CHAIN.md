# Callback chain reconstruction

Five maintained functions now cover ordered registration, frame dispatch and
removal. The two dispatchers have reviewed switch tables in addition to the
instruction ranges initially reported by Ghidra.

| Function | Complete target owner span | Source/codegen state |
| --- | --- | --- |
| AddToUpdateChain | 0x00456B70..0x00456C02, 147 bytes | conventional source159; non-exact |
| AddToDrawChain | 0x00456C10..0x00456CA2, 147 bytes | conventional source159; non-exact |
| ExecuteUpdateChain | 0x00456CB0..0x00456E0F, 352 bytes | conventional member source348; non-exact |
| ExecuteDrawChain | 0x00456E10..0x00456F2B, 284 bytes | canonical normal-COFF exact |
| UnregisterChainElement | 0x00457080..0x004570F1, 114 bytes | canonical normal-COFF exact |

`Chain.hpp` models the target's 0x24-byte element: priority +0, flags +4,
fastcall main callback +8, initialization callback +0xC, auxiliary callback
+0x10, embedded three-pointer node +0x14, and callback context +0x20. The
manager has update and draw sentinels whose next pointers are +0x18 and +0x3C;
the update dispatcher also reads a pause field at +0x48. These observations do
not close the manager's complete lifetime or data ownership.

TH10's MIT-licensed Chain implementation supplied source-structure reference.
TH11 boundaries, flags, callbacks, registers, globals and return-code paths
were reviewed independently in the pinned TH11 target. TH11 conditionally
locks on shared flag bit0x8000; its update dispatcher also has pause handling
and return code8. TH10 is not an exactness Oracle for these functions.

## Dispatch behavior

The main callback receives element +0x20 in ECX and returns a code in EAX.
Dispatch caches the next node before invoking the callback, permitting removal
of the current element. A null main callback skips the element entirely. A
nonnull but disabled callback still contributes one to the count. Bit1 of the
element flags enables callback execution.

| Return code | Draw | Update |
| --- | --- | --- |
| 0 | unregister current element, count it | unregister current element, count it |
| 1 or other unhandled value | count current element | count current element |
| 2 | retry after rechecking enabled bit | retry after rechecking enabled bit |
| 3 | finish with1 | finish with1 |
| 4 | finish with0 | finish with0 |
| 5 | finish with-1 | finish with-1 |
| 6 | count current element | restart update list and reset count |
| 7 | count current element | call auxiliary callback if present, then count |
| 8 | count current element | finish with0 |

Enabled update elements also use the auxiliary path while manager +0x48 is
nonzero. The conditional chain lock is released around the main callback and
reacquired afterwards. Auxiliary callbacks execute with the lock held. Each
lock operation retests the shared flags and adjusts the byte at0x004C3BB0;
the critical section is at0x004C3A90. The manager pointer is at0x004C3234.

## Registration and removal

Registration snapshots the global manager, calls a nonnull initialization
callback before locking, then clears that callback. It returns the callback's
result even when nonzero; the result does not suppress insertion. Nodes are
inserted before the first element with equal or higher signed priority. Tail
insertion leaves the new node's existing next field unchanged, so callers
must supply a cleared node. No extra validation was added to the source.

Removal searches both sentinel lists. A match with no previous node is left
alone. Otherwise it repairs neighboring links, clears both links and disables
the main callback. Ownership bit0 additionally clears all three callbacks and
calls operator delete. The independently reviewed delete entry at0x0045FD49
is a cdecl tail jump to CRT free at0x0045FC3B.

## Complete exact extents

Draw's initial259-byte instruction range ends at0x00456F12. Its owned COMDAT
also contains one NOP at0x00456F13 and six table entries at0x00456F14..2B,
for284 bytes total. An explicit byte-by-byte padding/table reference scan
found only the local indexed jump; the six table-to-case references stay
inside this owner. Following four CC bytes precede the next function.

The cold VC9 RTM `/TP /O2 /Gy /MT /Isrc` proof covers every owned byte and all24
relocations. These bind shared flags, manager, critical section, depth byte,
KERNEL32 Enter/LeaveCriticalSection slots, UnregisterChainElement, the local
table base, and each of its six reviewed case labels. The import slots were
independently parsed from the original PE. The 114-byte removal proof includes
its sole reviewed REL32 to operator delete. Both receipts bind their source,
shared header, profile, target, tools and object.

Update's352-byte reviewed span contains304 Ghidra instruction addresses,
twelve alignment bytes and a36-byte nine-entry table. Its normal member
candidate is348 bytes. The actual target manager arrives in EBX; source uses
an ordinary member declaration. Registration targets receive element in ESI
and priority in EBX; their sources use conventional two-argument stdcall
views. These three calling/codegen contexts remain open.

## Focused verification

Only the two new exact units were cold-replayed:

```bash
scripts/repo-python scripts/replay-exact-units.py \
  --unit execute-draw-chain --unit unregister-chain-element
```

Private Unicorn1.0.2 target-vs-VC9 diagnostics passed220 draw and440 update
cases, comparing return values, element/manager memory, lock-counter wrap,
callback/auxiliary/delete events and lock events. They cover enabled/disabled
and null callbacks, all return codes, retry with flag changes, restart, pause,
and owned/non-owned removal. Registration passed560 cases per function,
including empty/middle/tail/equal-priority insertion, signed priorities,
initializer errors and lock-counter wrap. List ordering and back-links were
also checked independently. Library/callback shims are deterministic.

Reproduction and results remain in private `.analysis/check-chain.py`,
`.analysis/check-chain-registration.py`, `.analysis/chain-behavior.json` and
`.analysis/chain-registration-behavior.json`. These bounded diagnostics do
not establish the complete native product or close the later semantic phase.
