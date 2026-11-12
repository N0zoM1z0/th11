# TH11 Pbg low-level file handles and path resolution

This source family reconstructs the concrete 12-byte Pbg stream object. Its
observed layout is a derived/base vtable, Win32 file handle and access mode.
The original Japanese TH11 v1.00a Ghidra owner boundaries and entire natural
VC9 function contributions were reviewed separately; the original source
class/TU attribution remains unknown.

## Two exact lifetime functions

The complete **46-byte** non-deleting destructor at `0x441D20..0x441D4D`
publishes the `Pbg::File` vtable (`0x4964EC`), tests for a valid handle,
closes it if present, writes `INVALID_HANDLE_VALUE` and clears access, then
publishes the base `Pbg::IFile` vtable (`0x496514`). A real C++ member
destructor in `src/PbgLowLevelFile.cpp` compiles to **46/46 native bytes**.
The strict `pbg-low-level-file-destructor` unit checks all three absolute
fields (two vtables and the `CloseHandle` import at `0x48B0A0`) without
relocation masking or byte patches.

The separate **66-byte** scalar deleting entry at `0x441CD0..0x441D11`
performs the same close/base transition, tests deleting flags bit 0 and calls
native scalar `operator delete` only when required, returning the object and
using `RET 4`. The natural member method `DestroyAndMaybeDelete` yields
**66/66 bytes**, including the three absolute fields and the REL32 scalar
delete call to `0x45FD49`. Its canonical strict unit is
`pbg-low-level-file-deleting-destructor`. Both functions are independent
complete owners with reviewed CC padding and caller references; their exact
codegen does not establish original nominal source names.

## Larger file operation and disk resolver

`PbgLowLevelFileView::Open` covers the **238-byte** original
`0x441D50..0x441E3D`. It closes the current stream, parses `'r'`, `'w'` or
`'a'` mode, resolves a 260-byte path, calls Win32 `CreateFileA`, and
optionally seeks to EOF for append mode. The native uses a security-cookie
frame. Maintained source compiles under VC9 as 219 bytes in the initial
ordinary COFF context, but remains **non-exact** due to stack/cookie/ABI
codegen and some private mode-loop details.

`ResolvePbgDiskPath` covers original **121-byte** `0x442000..0x442078`:
absolute drive paths are copied, while relative names are resolved against
the executable directory and appended. The initial natural source compiles
to a 121-byte function but does not match the native EAX filename / ECX
destination private register allocation. It remains **non-exact**. Both
functions are real maintained C++ paths, not placeholders, and were not
counted toward exactness on size alone.

The neighboring 134-byte Pbg File session open, 101-byte ResFile open,
109-byte exact resource cleanup, 26-byte exact element destructor and
16-byte exact session constructor are documented in `PBG_RESOURCE_SESSION.md`.

The path-resolver source also retains the native no-separator edge case:
`GetModuleFileNameA` and `strrchr` are followed by an unconditional write to
the returned separator pointer, even if the null path first caused the output
buffer's first byte to be cleared. Adding a defensive `else` changes the
original control flow; the maintained source deliberately does not claim that
such a guard existed. Its 121-byte length match is still not exact codegen.
