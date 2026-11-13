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

Natural C++ under pinned VC9 `/TP /O2 /Ob1 /Gy /MT /Isrc /EHsc` already
reproduces **all bytes and independently reviewed relocation fields** of six
complete owners: 5-byte Write, 69-byte scalar deleting destructor (derived
vtable, free, base vtable, scalar delete), 49-byte destructor (derived vtable,
free, base vtable), 37-byte Reset (free), 7-byte Tell, and 4-byte Size.
All six use the same source file but have independent canonical strict
`pbg-memfile-*` match units, native boundary reviews, and individual replay.

The original 44-byte Open calls the actual 218-byte `ReadPbgResourceByName`
and 202-byte `GetPbgResourceSizeByName`, which remain source-present but
nonexact. The initial natural 44-byte Open differs in one non-address byte.
Read's natural 88-byte owner retains both memcpy calls and the original size
and boundary decisions, but has remaining loop/branch-code differences.
Seek's native 109-byte owner returns success in AL and has three origin cases;
initial natural source emits 119 bytes. These three are not credited as exact.
The original source class/TU partition is unknown; compile-time codegen
identity is not proof of whole-game runtime closure.
