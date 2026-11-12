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
