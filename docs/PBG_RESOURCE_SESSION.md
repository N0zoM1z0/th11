# Native Pbg resource-session owner (2026-11-12)

The original TH11 v1.00a executable places a small Pbg file/resource-session
lifecycle at `0x4415A0..0x44172C`. The 0x10-byte session has four physical
DWORD fields (+0, +4, +8, +C). Its nominal original C++ class and translation
unit remain indeterminate; the concrete Pbg::File and Pbg::ResFile vtable
references in the adjacent functions do not establish that the session itself
uses either class name.

## Exact 16-byte constructor

Direct original Ghidra review establishes the **complete 16-byte** owner
`0x4415A0..0x4415AF`: `MOV EAX,ECX`, `XOR ECX,ECX`, four separate DWORD
zero stores and plain `RET`. The next function starts at `0x4415B0`; the
constructor also has an EH constructor-vector DATA reference and no foreign
interior entry. Natural C++ four-member initialization in
`src/PbgResourceSessionCtor.cpp` reproduces all **16/16 original bytes**
under the pinned VC9 RTM `/O2 /Ob1 /Gy /MT /Isrc` profile, without
relocations or any manually encoded instructions. The independent canonical
`pbg-resource-session-constructor` unit requires the exact entire body and
strict cold replay, not just matching size or semantics.

## Neighboring larger owners

The 10-byte destructor adapter (`0x4415B0`) forwards to the 109-byte
resource cleanup (`0x4416C0`), while 134-byte `0x4415C0` and 101-byte
`0x441650` open Pbg::File and Pbg::ResFile variants. They own memory,
stream state and failure cleanup and require their own reconstruction and
verification. This constructor's exactness does not imply theirs.
