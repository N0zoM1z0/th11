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

## Full 134-byte File and 101-byte ResFile opening owners

`OpenPbgFileSession` (`0x4415C0..0x441645`) is the entire native **134-byte**
File-mode owner. It first releases any prior session, traces the requested
name through the original `0x441C80` trace boundary, allocates the 12-byte
Pbg::File object (vtable `0x4964EC`), stores it in session `+0xC`, loads the
directory through the real `0x441910` helper, duplicates the name using
`0x441BD0` and calls the stream's virtual open method with mode at `0x496544`.
Failure traces again and tears down the session. It has eight native calls.
Natural C++ emits 143 bytes in normal COFF and 136 in the genuine scoped
`/GL` source family. The function is source-present, **not exact**.

`OpenPbgResFileSession` (`0x441650..0x4416B4`) is the separate **101-byte**
ResFile-mode owner. It follows the same release/parse/name-copy sequence,
but allocates a 16-byte Pbg::ResFile object (vtable `0x496614`, remaining
fields initially zero) and returns success without a virtual file-open call.
Normal COFF emits 106 bytes; the scoped `/GL` candidate has 99 bytes and all
five native calls. This is also **nonexact**. These two complete source owners
are maintained in `src/PbgResourceSessionOps.cpp` and independently bounded
rather than represented by stubs or fabricated register moves.

## Exact 109-byte session release and 26-byte directory destructor

The larger `ReleasePbgResourceSession` at `0x4416C0..0x44172C` performs the
actual cleanup: optionally logs and frees a temporary buffer, clears its
slot even on the empty path, conditionally destroys/free's a vector of
**16-byte Pbg directory entries** with the genuine C++ EH vector destructor
iterator, optionally calls the stream's virtual deleting destructor at vtable
`+0x1C`, and clears all session fields. The native function has **109 bytes,
44 instructions and five calls**. Its current natural C++ source compiles to
109/109 under a real dynamic session `/GL` caller. The only initial differences
were two final ordinary zero stores scheduled in reverse order by VC9;
sequencing the two independent source assignments in the other order restores
native `MOV [ESI+4],EDI` before `MOV [ESI+0xC],EDI`, without `volatile`,
dummy accesses or machine-byte patches. Canonical `pbg-resource-session-release`
checks the full 109-byte primary-TU owner and six actual fields: original
trace data and call, CRT free, directory-entry destructor, EH vector iterator,
and array delete. The original private ESI receiver and function behavior
are preserved by the linked context.

The `PbgResourceEntryView` destructor at `0x441C00..0x441C19` is a separate
**26-byte** owner, also reconstructed. It frees the entry's optional filename
and clears the name only when a free occurred; putting the clear outside the
null guard changes one byte of the native conditional jump. Its sole native
REL32 field calls CRT free at `0x45FC3B`. The natural C++ member destructor
matches all 26 bytes under pinned normal COFF and serves as the exact element
destructor callback used by the larger session.

The adjacent 10-byte session destructor adapter at `0x4415B0` remains
source-present but **nonexact**: an isolated normal and linked forwarder
compiles to a five-byte tail jump, not the original ESI-save/CALL/restore
sequence. It and the two file-opening owners need original `/GL` owner and
private argument scheduling work. Scoped proof is retained in
`.analysis/probe-pbg-resource-session-ltcg-20261112.log` and
`.analysis/probe-pbg-109-store-order-20261112.log`.

## Path duplication and original no-op trace hook

The native `0x441BD0..0x441BFE` contains a **47-byte** path duplication
owner called by both open modes. It computes length including the NUL byte,
allocates an independent buffer with CRT malloc, and copies forward through
the terminating NUL only when allocation succeeds. A maintainable natural
implementation is in `src/PbgResourcePath.cpp`. Plain VC9 normal-COFF emits
46 bytes and an actual `/GL` producer with the File/ResFile open sources emits
44 bytes. Its original filename arrives in a private EDI register that the
current C++ partition has not yet restored. This remains source-present,
**nonexact**; no match percentage is promoted for size-only similarity.

The traced `0x441C80..0x441C80` entry is independently reviewed as a genuine
**one-byte `RET`** with no reads/writes or side effects, not an imagined
printf-style logger. It has incoming calls from the File open/cleanup and
other Pbg resource paths. `src/PbgResourceTrace.cpp` defines this exact
no-op function with the original variadic call shape; pinned VC9 normal COFF
emits the identical single-byte owner without relocations. The canonical
`pbg-resource-trace-noop` unit cold-replays that byte and leaves the larger
134/101-byte owners' separate nonexactness intact.

Bounded experiments with the genuine newly sourced path copier and Pbg entry
destructor included in `/GL` are retained in
`.analysis/probe-pbg-real-file-helpers-20261112.log`. Those experiments did
not improve the 134-byte File or 101-byte ResFile owners, and linking the
trace no-op into their producer would remove native calls, so the canonical
109-byte cleanup continues to use its real opaque trace boundary.
