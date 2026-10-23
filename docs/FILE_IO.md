# File existence check

`FileExists` covers `0x004585E0..0x0045866C`, 141 bytes. Direct Ghidra review
found one contiguous body, four incoming calls, no external interior entry,
two complete RET 4 exits, and following CC padding. The path is a stack
argument and EAX returns 0 or 1.

The function tests shared flags at 0x004C3810 for bit 0x8000. When enabled it
enters the critical section at 0x004C3AC0 and increments the byte at
0x004C3BB2. It opens the path with GENERIC_READ, FILE_SHARE_READ,
OPEN_EXISTING, and flags 0x08000080. A valid handle is closed before returning
1; an invalid handle returns 0. Each exit separately retests the shared flag,
leaves the critical section when enabled, then decrements the byte counter.
This is an openability check; it does not enumerate directory entries.

The complete cold-built VC9 RTM normal-COFF body is exact. All fourteen DIR32
relocations were reviewed and replayed; every byte agrees. The original PE
import table independently identifies KERNEL32 slots EnterCriticalSection at
0x0048B084, LeaveCriticalSection at 0x0048B088, CreateFileA at 0x0048B098, and
CloseHandle at 0x0048B0A0. Global bindings agree with the complete disassembly.

```bash
scripts/repo-python scripts/ghidra.py query .analysis/file-exists-review.txt review \
  0x004585E0
scripts/repo-python scripts/replay-exact-units.py --unit file-exists
```

The canonical unit binds both source files, the complete object extent,
profile, target and historical tools. Production global definitions, original
TU partition, and original normal-COFF/LTCG ownership remain open.
