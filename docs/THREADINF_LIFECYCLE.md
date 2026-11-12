# TH11 ThreadInf virtual destruction (2026-11-12)

The original Japanese v1.00a binary has a separately bounded **16-byte**
virtual `ThreadInf` member destructor at `0x459410..0x45941F`. Direct
mapped-text-attested Ghidra review shows `PUSH ESI`, `MOV ESI,ECX`, a
virtual-table store at `this+0`, `CALL 0x459430`, `POP ESI` and plain `RET`.
There are five incoming exception-unwind calls and no external references
into the function interior; the following bytes are CC padding. The vtable
is the observed `0x4978D0` root, and the callee is the independently reviewed
101-byte runtime-thread stop routine.

The maintainable source in `src/ThreadInfLifecycle.cpp` uses a **real virtual
C++ destructor**, which delegates to the existing `StopRuntimeThread` with a
physical `RuntimeThreadView` layout, and a small ordinary owner caller. Under
isolated VC9 compilation this destructor was 13 bytes, not matching the
original. When `src/FrameRuntime.cpp` participates as the **real `/GL`
producer**, pinned VC9 RTM restores ESI as the private receiver and emits
**all 16 original bytes and six instructions**. The generated vtable has the
same physical interpretation without assuming a proven original class name.

Canonical `threadinf-virtual-destructor` compares the complete public
primary-TU PDB contribution with full source/PE/MAP/tool receipts and the
two reviewed fields: DIR32 at +5 to `0x4978D0`, REL32 at +10 to
`StopRuntimeThread` `0x459430`. Its affected-only cold Oracle replay reports
zero differences. This is exact code generation for this single owner, not
proof of the original translation-unit partition. The adjacent 36-byte
*deleting* destructor at `0x4593E0` and full runtime-thread stop producer
remain independent areas for investigation.

## Three-member lifecycle recovery and deleting-owner limitation

The maintained `ThreadInfView` now has a real four-field constructor and
inline virtual destructor. In a genuine VC9 `/GL` context with
`FrameRuntime.cpp`, construction, scalar deletion and non-deleting destruction
compile to the complete original **21/21**, **36/36**, and **16/16** bytes
respectively. The native constructor at `0x4593C0..0x4593D4` was omitted
from the initial inventory: independent Ghidra review confirmed 21 contiguous
bytes, seven instructions, a `RET` and CC neighbors, with vtable field at +4.
It writes `vtable`, zeroes fields +4/+8/+C/+10, and deliberately does not
clear the final two member words. Canonical `threadinf-constructor` now cold
replays that complete 21-byte PDB/MAP owner.

The scalar deleting destructor at `0x4593E0..0x459403` preserves the original
conditional storage deletion, worker shutdown, and `RET 4`. Three native
address fields are aligned (vtable at +5, worker stop at +10, scalar delete
at +23); all 36 compiled code bytes match the original when replayed.
**It is nevertheless not a canonical exact match**: this actual historical
VC9 link publishes *both* `??_EThreadInfView` and `??_GThreadInfView`
functions at the same 36-byte entry in MAP, so the strict linked oracle
rightly rejects ambiguous ownership. No field masking, symbol filtering or
modified oracle was used to claim success. The deleting function remains
source-mapped/non-exact until a unique production owner can be established.

Only the 21-byte constructor and 16-byte normal destructor carry canonical
exact credit. All three mappings and the dirty worktree are preserved.

## Separate eleven-byte global publication entry

A further explicit Ghidra span revealed a distinct function at
`0x459420..0x45942A`, immediately between the 16-byte virtual destructor
and the 101-byte worker-stop routine. It consists of `MOV DWORD PTR
[0x4A577C],1; RET`, followed by five alignment CC bytes. No incoming
reference or code/data overlap has been established. The meaning of the
`0x4A577C` global, nominal original class and translation unit are unknown.

The minimal natural source `SetRuntimeThreadFlagA577C` encodes exactly
that physical operation. Pinned VC9 normal-COFF produces a unique 11-byte
owner and its single reviewed DIR32 field at +2. The canonical
`runtime-thread-global-flag-a577c` unit strictly replays all bytes and the
field with zero differences, without inferring behavioral semantics.
