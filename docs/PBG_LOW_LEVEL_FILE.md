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

## Strict 121-byte original disk path resolver (2026-11-13)

The `ResolvePbgDiskPath` source was already complete, including the original
unchecked dereference of the separator return value. In isolated VC9 normal
COFF it produced 121 bytes but differed from the original private register
allocation. A bounded real `/GL` producer containing the full 238-byte
`PbgLowLevelFileView::Open` source and a separate dynamic caller now yields
**121/121 original bytes, 55 instructions and no non-field differences**.
The original filename arrives in EAX and destination in ECX naturally.

The canonical `pbg-resolve-disk-path` unit binds all three code fields: REL32
`strchr` at +0xA to `0x4604A0`, DIR32 `GetModuleFileNameA` import at +0x37
to `0x48B0B0`, and REL32 `strrchr` at +0x3F to `0x460560`. Full hash-pinned
PE/MAP/PDB/source/tool replay has zero differences. The 238-byte Open caller
itself is still nonexact, so this claim is limited to the resolver owner.

## Full six-operation low-level I/O interface (2026-11-13)

The complete native I/O family is now maintained in
`src/PbgLowLevelFileIO.cpp/.hpp`, using the real 12-byte file layout and
ordinary Win32 calls. All six original function boundaries have separately
reviewed instructions, return behavior, callers and following CC padding.
The four smaller operations reproduce the *entire original function* with
one independently reviewed Win32 import each under pinned VC9 RTM normal COFF:

| Operation | Original range | Strict result | Native import |
| --- | --- | --- | --- |
| Close | `0x441E40..0x441E61` | **34/34 exact** | `CloseHandle` at `0x48B0A0` |
| Read | `0x441E70..0x441EA8` | **57/57 exact** | `ReadFile` at `0x48B0A4` |
| Write | `0x441EB0..0x441EEE` | **63/63 exact** | `WriteFile` at `0x48B0A8` |
| Size | `0x441F10..0x441F24` | **21/21 exact** | `GetFileSize` at `0x48B0AC` |

`Read` returns the bytes actually read, not the Win32 BOOL. `Write` returns
success only when the number of bytes written equals the requested length;
the native access-mode tests are preserved. These exact units replay each
whole body and its single native DIR32 field, not a masked snippet.

The two remaining I/O operations are now **strict exact** as well. The
37-byte `Seek` at `0x441F30..0x441F54` returns success or failure in **AL**
only; high EAX bits on failure were never part of its return contract. The
145-byte `ReadWhole` at `0x441F60..0x441FF0` calls the native virtual
Size/Tell/Seek/Read functions, conditionally allocates a buffer, restores
the prior position on success, frees on read failure, and preserves the
original early seek failure without adding compensating cleanup. Both
owners now replay all their original bytes and independently reviewed fields.

## Restored AL-only Seek contract and 145-byte ReadWhole boss

An earlier source declaration returned `unsigned int` for `Seek`, forcing
VC9 to preserve high EAX bits and emit a 46-byte body instead of the native
37 bytes. The actual original writes only **AL**, returning 0 for an invalid
handle and 1 after calling SetFilePointer; the upper register bits are
incidental. Declaring the real C++ result as `unsigned char` emits the
original **37/37 bytes**, with one independently reviewed DIR32 Win32 import
to `SetFilePointer` `0x48B09C` at offset +0x1C.

That contract also explains a mismatch in the **145-byte `ReadWhole` boss**:
its first virtual seek must test **AL**, not EAX. Expressing the true length
comparison as `length > maximumSize`, placing read-failure free/return before
restore-and-success, and correcting the virtual Seek result to `unsigned char`
restores the complete original function, including 145/145 bytes and native
branch layout. The two reviewed REL32 fields call `malloc` `0x460192` from
+0x2C and `free` `0x45FC3B` from +0x6C. Both new canonical units replay
**every byte, with zero differences**, without synthetic padding, custom
register shims or masked exceptions.

Changing `PbgLowLevelFile.hpp` affects the same source unit as Close, Read,
Write and Size. All six normal-COFF I/O units must be locally cold-replayed
after this change; broader source and full-game replay are unnecessary.

## Corrected return contract for the 238-byte Open boss (2026-11-13)

The complete native `0x441D50` shows the file `Open` method returns a status
in **AL**: invalid/empty mode and unsuccessful `CreateFileA` return zero;
successful creation (and optional append seek) returns one. The earlier
`void Open()` source omitted this observable result and the invalid-handle
failure branch. Its maintained signature is now `unsigned char Open(...)`,
and the native mode failure/success paths are explicitly present. The pinned
VC9 isolated COFF candidate is **223/238 bytes** (still nonexact), so this
is a behavioral and ABI correction rather than a byte-level exact claim.
The previously exact low-level 46/66-byte destructors and the 121-byte path
resolver are independently reverified after the signature change.

## Exact 25-byte Tell/current file position slot (2026-11-13)

The previously omitted original virtual slot 4 at `0x441EF0..0x441F08` is
now implemented as `PbgLowLevelFileView::Tell()` in the actual low-level I/O
source. It returns 0 for `INVALID_HANDLE_VALUE`; otherwise it returns
`SetFilePointer(handle,0,0,FILE_CURRENT)`. This is the position function
called by the independently exact 145-byte `ReadWhole` owner, and it is a
separate complete 25-byte vtable member rather than part of Size/Seek.

The pinned VC9 normal-COFF contribution reproduces the complete **25/25
original bytes**. The sole decoded native field is a DIR32 import at +0x14
for `SetFilePointer` `0x48B09C`. No other bytes are ignored, and this new
canonical `pbg-file-tell` unit is cold-replayed with the six existing exact
I/O functions affected by its shared header.
