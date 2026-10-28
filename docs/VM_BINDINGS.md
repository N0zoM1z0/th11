# Stage VM binding cleanup

`ReleaseVmBindingBlock`, native `0x00402300..0x004023E6`, has a complete231-byte
strict cold linked proof. It adds one exact function /231 bytes: totals are
41 functions /9,125 owned bytes. The new primary TU and its two-input probe
were cold-built once; no previous canonical unit was replayed.

Read-only direct Ghidra review covers the entire contiguous span, all exits,
nine callers, preceding RET/CC and following CC. There are no embedded tables
or external interior references. The private input is ESI; the function ends
in RET. Natural stdcall source plus two independent receiver calls produces
that private convention without register annotations.

## Physical fields and operation order

The binding view supplies count+0, ID-table pointer+8 and owned storage at
+0C/+10/+14. Storage+10 and+14 are freed and cleared first. The loop processes
indices `0 .. count-2`, using signed comparisons and re-reading count after
each iteration. The last table entry is deliberately not processed. Counts
at most1 skip lookup entirely; the count and unknown+4 word are preserved.

For every nonzero ID, lookup scans renderer list+7B562C first, then+7B5634.
The renderer pointer is the existing cell `0x004C3268`. These list cells follow
the earlier batch pointers+7B5624/+7B5628; no old renderer header was changed.
Node payload+0 points to a VM and next+4 advances the list. Payloads are not
checked for null. VM ID is+0 and flags are+404. The first match wins, including
when the other list contains the same ID.

The matched VM receives flag0x04000000. Only when its opaque marker+18 is zero
does the routine walk related nodes at+14 and OR the same flag into their VMs.
This is one level of propagation; it does not recursively follow those VMs'
own related lists. Every visited ID slot is cleared, including missing/zero
IDs, using the slot pointer captured before lookup.

After the loop the ID table and storage+0C are freed and cleared. Thus the
free order is+10,+14,+8,+0C. Other VM bytes, nodes and renderer lists are
preserved. The names describe observed physical views and operations, not a
recovered complete original class or a proved meaning for marker+18.

## Exact bindings

`release-vm-binding-block` uses pinned VC9 RTM `/O2 /Ob1 /Gy /MT /GL`, complete
public primary-source PDB ownership, current source/header/tool/producer/
PE/MAP/PDB receipts, exhaustive field replay and zero final byte differences.

| Offsets | Type and symbol | Independently reviewed target |
| --- | --- | --- |
| 12,31,200,219 | REL32 `_free` | `0x0045FC3B` |
| 69,98 | DIR32 `g_RenderBatch` | `0x004C3268` |

Both list offsets and all flag/field operands are ordinary compared code
bytes. They are not masked or substituted. The origin review identifies
game-authored stage/VM lifecycle integration through its renderer-specific
lists, flag propagation and real stage destruction callers. Complete original
data ownership and full native integration remain open.
