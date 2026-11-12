# Pbg resource lookup and file loading (2026-11-13)

The Pbg::ResFile object and resource lookup family lie immediately after
`ResolvePbgDiskPath` in the original Japanese TH11 v1.00a binary. The Pbg
resource object is 16 bytes, with a vtable plus three additional DWORDs.
The table at `0x4C3C48` stores 16-byte resource descriptors, and the resource
count is held at `0x4A8ED0`. Original class names and translation-unit
ownership remain indeterminate.

## Exact 31-byte ResFile deleting destructor

The complete native `0x442090..0x4420AE` publishes the base IFile vtable
`0x496514`, conditionally deletes storage when the stack deletion flag has
bit 0 set, returns `this` in EAX and exits with `RET 4`. A natural C++ member
method in `src/PbgResFileDeleting.cpp` compiles under pinned VC9 RTM to
**all 31 original bytes** without register directives or byte patches.
Canonical `pbg-resfile-deleting-destructor` verifies two independently
reviewed fields: DIR32 at +0xA to the IFile vtable and REL32 at +0x12 to
scalar `operator delete` `0x45FD49`; there are no mismatches.

The neighboring full resource lookup (`0x4420E0`, 203-byte span), resource
read (`0x4421E0`, 218 bytes), and size lookup (`0x4422C0`, 202 bytes) have
separate native bodies and are the next larger owners to reconstruct.

## Exact 203-byte resource-name normalization and record lookup boss

The full original `0x4420E0..0x4421AA` takes the requested pathname in ECX,
locates the first `/`, makes an unchecked local 260-byte NUL-terminated copy,
and substitutes the literal five-byte `.dat\0` suffix at the slash if one
was present. It then walks the 16-byte directory entries rooted at
`0x4C3C48`, stopping at count `0x4A8ED0`, and performs an inline strcmp-like
byte comparison of every entry's name against the normalized path. It
returns the physical entry pointer, or null when no match exists.

The natural C++ function in `src/PbgResourceLookup.cpp` has now reached
**203/203 native bytes** under pinned VC9 normal COFF. Two details were
important: the suffix offset is computed as `(slash ? slash+1 : filename) -
filename - 1`, and the buffer copy uses a source cursor and destination
index instead of two unrelated object pointers. The 4-byte suffix word and
last byte are loaded into ordinary locals before calculating the destination
pointer, which restores native ECX/EDX/EAX allocation without any custom
register directives or code injection.

The original Ghidra function reports 200 instruction bytes plus a native
**three-byte `LEA ECX,[ECX]` alignment NOP at 0x44215D..0x44215F**, which
has been independently decoded and is included in the exact 203-byte
comparison. The only incoming code references to the interior loop target
are from within this same function. The canonical strict COFF unit
`pbg-resource-record-lookup` binds all **seven independently reviewed
fields**: security cookie `0x4A2088`, `strchr` `0x4604A0`, `.dat` suffix
`0x496534` plus +4, count `0x4A8ED0`, entry table `0x4C3C48`, and
`__security_check_cookie` `0x45FC2C`. The full cold replay reports zero
byte differences. This is exact code generation for this 203-byte boss,
without claiming the complete Pbg archive manager has been reconstructed.
