# TH11 Pbg THA1 directory loading (2026-11-13)

The native `0x441910..0x441A5F` is a substantial **336-byte** resource
directory loading owner shared by the File and ResFile session open paths at
`0x4415C0` and `0x441650`. Its complete original boundary, 15 calls,
returns, success/failure control flow and absence of foreign interior
references were reviewed in the hash-pinned Japanese TH11 1.00a mapped PE
using Ghidra.

## Maintained natural C++ owner

`src/PbgDirectoryLoader.cpp/.hpp` now implements the complete resource
lifetime. The session is the four-DWORD `PbgResourceSessionView`; its stream
pointer is at +0xC. The loader calls the real stream vtable Open at slot 0,
reads a 16-byte `THA1` header via slot 2, applies the first byte transform
with original key `0x1B`, step `0x37`, block 16 and limit 16, and checks
magic `0x31414854`. The encoded directory offset, packed length and entry
count are adjusted by their original constants (`0x075BCD15`,
`0x3ADE68B1` and `0xF7E7F8AC`).

It then uses the virtual size and seek operations at slots 5 and 6 to read
the packed directory from the end of the physical file. That buffer is
allocated with the real CRT allocator, transformed with the second native
key/step/block (`0x3E`, `0x9B`, `0x80`), decoded through the existing full
472-byte `DecompressArchiveData` owner, and handed to the separately bounded
363-byte directory-entry constructor `0x441A60`. On success both temporary
buffers are freed and the session owns the returned 16-byte entry array.
On failure the original stream's virtual deleting destructor at slot 7 is
called with flag 1 and the session stream pointer is cleared. The original
caller does not add defensive checks for archive filename or header bounds;
the maintained source preserves those boundaries rather than replacing the
path with an invented no-op.

## Byte-level status and remaining work

The natural VC9 RTM normal-COFF candidate is **370/336 bytes**. Five scoped
`/GL` producers (`opaque`, real File/ResFile parent, cipher, decoder, and
all real child owners) generate a complete **380-byte primary code function**
with all **15 original calls** but different register, header stack-frame
and private cipher-key AL arrangements. Thus this 336-byte boss is
**source-present, not strict exact**; the existing exact file and resource
lookup functions are independently verified and are not credited to it.
The reproducer is `.analysis/probe-pbg-directory-loader-ltcg-20261113.py`
with a checked-in dynamic producer
`probes/pbg-directory-loader-context.cpp`.

The next substantial downstream owner is the 363-byte `0x441A60` heap
constructor/decoder for directory entry arrays. Reconstructing that owner
may also improve the optimizer context of the 336-byte parent.

## Strict exact 9-byte entry constructor (2026-11-13)

The large directory array parser's VC9 EH vector constructor references a
previously omitted **9-byte** member at `0x441C70..0x441C78`. Direct original
Ghidra review finds `MOV EAX,ECX`, a DWORD zero store to `this+0`, and `RET`,
with genuine CC padding on both sides. The source
`PbgResourceEntryView::PbgResourceEntryView()` in
`src/PbgResourceEntryCtor.cpp` resets **only** the filename pointer, leaving
the three metadata words for the parser. Pinned normal-COFF VC9 produces the
complete **9/9 original bytes** and no relocations; canonical
`pbg-resource-entry-constructor` strictly replays the whole function. The
paired 26-byte entry destructor remains independently exact.
