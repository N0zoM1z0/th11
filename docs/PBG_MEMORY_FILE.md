# Pbg memory-backed file interface (2026-11-13)

The original Japanese TH11 v1.00a contains a distinct Pbg::MemFile family
at `0x442C20..0x442E2C`. It is not the 12-byte disk-backed Pbg::File nor the
16-byte resource archive view. The memory stream has a **16-byte** physical
layout: derived vtable at `+0`, byte length at `+4`, current cursor at `+8`,
and owned backing buffer at `+0xC`. The native derived vtable is
`0x4965CC`, with the base IFile vtable at `0x496514`. Two vtables also
publish shared interface methods at known addresses, including the
unsupported five-byte write stub, the 7-byte Tell and 4-byte Size methods.

The maintained source `src/PbgMemoryFile.cpp/.hpp` implements the whole
family, including **69-byte scalar deletion**, **49-byte non-deleting
lifetime cleanup**, **44-byte named resource open**, **37-byte content reset**,
**88-byte bounded read**, **7-byte Tell**, **4-byte Size**, and **109-byte
absolute/current/end Seek**, plus the **5-byte unsupported Write**.

The current pinned VC9 codegen and canonical replay prove **eight** complete
owners: 5-byte unsupported Write, 69-byte scalar deleting destructor,
49-byte ordinary destructor, 37-byte Reset, 44-byte Open, 88-byte Read,
7-byte Tell and 4-byte Size. Every owner has a separate strict
`pbg-memfile-*` unit. Open's downstream 218-byte resource read and 202-byte
size APIs remain independently source-present **nonexact**; their state does
not affect Open's accepted 44-byte codegen. Only the 109-byte Seek member
remains **nonexact** in this MemFile family.

## Exact 44-byte Open and 88-byte bounded Read (2026-11-13)

Two more complete native MemFile owners are now strict exact. The **44-byte**
Open member at `0x442CE0..0x442D0B` accepts **two stack arguments** (the
filename and a mode string) and ends with `RET 8`; the original simply does
not use the mode. An earlier `Open(filename)` declaration emitted `RET 4`
and differed by one byte even though the entire remaining code matched.
Declaring the genuine second source argument naturally restores **44/44
bytes**. Its two reviewed REL32 fields target the real 218-byte
`ReadPbgResourceByName` at `0x4421E0` and 202-byte
`GetPbgResourceSizeByName` at `0x4422C0`, neither of which is claimed exact
by association.

The **88-byte** Read at `0x442D40..0x442D97` has two bounded `memcpy` call
paths: copy the requested length if it fits, otherwise copy available bytes,
then advance the current cursor and return the count. The source-level
comparison `requested <= available` originally generated a reversed `CMP`
plus `JA`. Expressing the same unsigned condition as
`available >= requested` restores the native `CMP ESI,EBX`/`JB` order,
recovering **88/88 bytes** without instruction patches. Two original REL32
`memcpy` calls at offsets `+0x20` and `+0x3E` target `0x46D630`.
Both canonical units strictly replay their entire body and all fields.

## Full 109-byte Seek source and linked-code layout (2026-11-13)

The native MemFile Seek owner at `0x442DC0..0x442E2C` includes all three
position modes in a single 109-byte member with four returns and **no jump
table or external call fields**. Absolute mode 0 accepts only a nonnegative
offset strictly below stream length, relative mode 1 allows a signed offset
strictly less than remaining bytes, and end mode 2 accepts only nonpositive
offsets whose absolute magnitude is strictly less than the full length.
All success paths report `AL=1` and failure reports `AL=0`; no compensating
bounds validation beyond these original comparisons is inserted.

The maintained source uses a normal three-way C++ `switch(origin)`. This
recovers the target's original progressive `SUB EAX,0` / `SUB EAX,1` /
`SUB EAX,1` mode dispatch; bounded VC9 normal-COFF comparison improves the
original source's **10/109** matching positional nonfield bytes to **74/109**.
The resulting complete candidate is **120/109 bytes**: the compiler still
duplicates 11 bytes of relative-seek update/return code that the native
shares between modes 1 and 2, and its branch targets/one return placement
differ. Multiple natural `goto`/shared-cursor/branch-order variants have
also been compiled and compared without restoring the missing shared native
code block. The owner is therefore **source-present, nonexact**. Its ABI,
original returns and independent byte differences are preserved for further
work rather than cropping the function or relaxing Oracle comparison.

The eight already exact MemFile owners in the same source file are cold
replayed after this control-flow improvement, with no regression.
