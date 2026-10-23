# Runtime surfaces and animation VM lifecycle

Direct Ghidra review covers four complete bodies in the attested original
Japanese TH11 v1.00a. They have no jump tables or external interior references.
Entry/return paths, callers, callees and neighboring code were reviewed.

| Function | Complete target extent | Bytes | State |
| --- | --- | ---: | --- |
| `ResetAnimationVm` | 0x00401FD0..0x004020FE | 303 | Exact |
| `EnsureRuntimeSurfaces` | 0x0042B530..0x0042B5FD | 206 | Source present, non-exact |
| `ReleaseRuntimeSurfaces` | 0x0042B600..0x0042B64F | 80 | Exact |
| `BindAnimationScript` | 0x0044ACD0..0x0044AD9B | 204 | Source present, non-exact |

The two exact functions add383 owned bytes through separate cold public
primary-TU proofs. The custom game VM, renderer/runtime fields and script
integration support authored-game origin independently of byte equality.

## Surface initialization and release

Initialization takes a stack runtime pointer and returns with RET4. A non-null
primary cached surface skips the entire routine, even if other cells are null.
Otherwise it gets level0 of texture slot2, binds animation script81 to the
primary VM, and checks the surface pointer again after script execution. If
it remains null, it copies the full280-byte camera0 record to camera2.
It then gets texture slot3's level0, binds script82 to the secondary VM and
81 to the overlay VM, and gets swap-chain0/back-buffer0 of type MONO.
HRESULTs are ignored; there are no added null-texture or script-range guards.

The source preserves MainRuntime-relative surface cells+0x1AC/+0x1B0/+0x1B4,
VM pointers+0x1BC/+0x1C0/+0x1C4, camera base+0x204 with0x118 stride and
animation-file pointer+0x588. Texture records are20 bytes, with the COM texture
at+0. The file's texture table is+0x120. The 13-byte getter immediately after
surface release independently corroborates that stride. It has no mapping or
credit and is missing from the provisional inventory; no database mutation
or artificial padding was used to separate these adjacent bodies.

Release conditionally calls COM Release and clears each of the three global
surface cells, then unconditionally clears the first again. Later COM calls
can affect shared runtime state, so the final clear is preserved. Its80-byte
body ends at RET immediately before the getter at0x0042B650, without CC padding.

## VM reset and script binding

Reset preserves DWORD+0x20 and the12-byte block at+0x3E8 across a0x434-byte
clear. It restores unit X/Y scale, identity transform, sprite index-1, low
flags word7, timer fields and nine additional reset DWORDs. Each of two list
nodes receives a VM-owner self pointer and null next/previous links.
The preserved block's original semantic type remains unknown; source uses
an opaque three-word aggregate. The view covers the observed reset span,
not a proven complete original class size.

Aggregate preservation reproduces the native16-byte temporary frame. Scalar
preservation kept only an8-byte frame and changed scheduling. Inline timer/
node operations and restoring the separate word before the aggregate retain
the target's store order. The final complete303-byte contribution has one
REL32 at45 bound independently to CRT memset0x0046A720 and otherwise matches
without normalization. A64-bit-head alternative left four differences and
was discarded; no dummy stack space or register ABI was introduced.

Binding checks the selected script pointer before the file's nonzero disable
guard. Valid binding resets the VM, writes truncated16-bit script/file IDs,
clears mode bits0x600, publishes file/start/current pointers, lazily initializes
the timer scale pointer and sets its frame to-1. It clears VM flag1, executes
one VM update and increments renderer+0xA0. Invalid/disabled binding instead
clears all0x434 bytes, including the otherwise preserved state; it does not
use normal reset defaults.

File fields are ID+0, script table+0x11C, texture table+0x120 and disable guard
+0x124. VM script/file fields occupy+0x39E/+0x3A2/+0x3A4/+0x3A8/+0x3B0;
timer is+0x5C and flags+0x404. The scale pointer targets float0x004A7948,
also read by the VM interpreter. Its entry review confirms a stack VM input;
the caller's PUSH and callee entry agree. The11640-byte interpreter at
0x0044B4B0 remains a prototype-only dependency with no source or exact credit.

## Remaining codegen differences

`EnsureRuntimeSurfaces` currently has a complete206-byte primary contribution
but43 unrelocated differences, chiefly conditional register-save placement
versus the target's entry saves. Block versus early-return guard styles and
`/EHsc`/`/Ob2` probes leave the same result. Its three script-binding calls
naturally use file-in-EDI, script-in-EBX and VM-in-EAX.

`BindAnimationScript` is191 versus204 with165 unrelocated diagnostic
differences. Timer/node member operations prevent an earlier flags-cache
reordering, but the target's aligned EBP frame remains absent. The initial
scalar form was199; aggregate/timer/node work yields the maintained191.
No masked comparison, cropped extent or inferred equivalence grants credit.
Do not repeat these bounded configurations or invent inert stack reservations.

## Strict proof and focused reproduction

Pinned VC9 RTM profile: `/TP /O2 /Ob1 /Gy /MT /Isrc /GL`. One family places
VM reset in primary AnimationVm.cpp; the other places release in primary
RuntimeSurfaces.cpp. Full receipts bind source/headers, historical tools,
producers and PE/MAP/PDB identities. Separate complete contributions match
all383 bytes and all eight independently reviewed fields: one CRT call and
seven MainRuntime root-plus-member bindings. No support-TU bytes are accepted.

```bash
scripts/repo-python scripts/replay-exact-units.py \
  --unit reset-animation-vm --unit release-runtime-surfaces
```

Only these two new units were replayed. All previous27 canonical source/header
inputs remain intact. The new views are not native allocations; full resource,
animation interpreter and game-product/runtime ownership remain open.

Private evidence: `.analysis/runtime-surfaces-review.txt`,
`animation-bind-review.txt`, `animation-reset.c`, `animation-run-entry.txt`,
`runtime-surfaces-adjacent-getter.txt`, `runtime-surfaces-*-linked.json` and
`runtime-surfaces-cold-replay.log`. Canonical families are under ignored
`build/match-linked/AnimationVm` and `build/match-linked/RuntimeSurfaces`.
