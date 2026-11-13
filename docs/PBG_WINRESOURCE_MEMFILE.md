# Pbg Win32 RCDATA resource-backed memory stream (2026-11-13)

A second independent 16-byte native memory stream implements Pbg resource
loading through the Win32 `RT_RCDATA` API. It publishes the derived resource
vtable at `0x496614`, shares the base `Pbg::IFile` table at `0x496514`, and
stores buffer length, cursor and allocated storage at +4/+8/+C. Its original
nominal class and translation unit remain unknown. The maintained C++ source
is `src/PbgWinResourceMemoryFile.hpp/.cpp`.

## Two separately exact lifetime owners

The target contains distinct copies of the MemFile destructor code at
`0x442F40..0x442F84` (**69-byte scalar deleting destructor**) and
`0x442F90..0x442FC0` (**49-byte non-deleting destructor**). They are genuine
independent native code owners with separate vtable DATA references, not
aliases to the earlier `0x442C50` / `0x442CA0` functions. Natural C++ methods
emit exactly **69/69** and **49/49** bytes respectively under pinned VC9
normal-COFF compilation. The scalar deleter has four reviewed DIR32/REL32
fields (derived MemFile vtable `0x4965CC`, CRT free `0x45FC3B`, base IFile
vtable `0x496514`, scalar delete `0x45FD49`), and the plain destructor has
three (all but scalar delete). Canonical `pbg-winres-memfile-*` units replay
all code and fields without differences.

## Full 169-byte Win32 resource open boss

The adjacent `0x442FD0..0x443078` owner loads a named Win32 resource type 10
through `FindResourceA` (IAT `0x48B0BC`), `LoadResource` (`0x48B0C0`),
`LockResource` (`0x48B0C4`), `SizeofResource` (`0x48B0CC`) and `FreeResource`
(`0x48B0C8`). It first invokes the actual vtable Close operation. After
allocation it copies the entire resource payload to its owned buffer and
publishes the read cursor. All native success/failure paths return AL=1,
including resource lookup errors. The second stack mode argument is unused
but the original function returns with `RET 8`.

The maintained source includes the complete control flow and memory/Win32
API lifecycle. Plain pinned VC9 produces a **169-byte** candidate with all
seven real downstream calls and eight decoded source fields, but differs in
its success/failure block placement after malloc. Reversing the source
`if (buffer)` branch produces 168 bytes and worsens register scheduling;
thus the better 169-byte source is retained, explicitly **nonexact**.
The original source TU and vtable class attribution remain open.
