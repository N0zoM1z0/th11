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
