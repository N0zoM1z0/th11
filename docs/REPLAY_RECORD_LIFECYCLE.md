# Replay record lifetime and embedded stage links (2026-11-12)

The original Japanese TH11 1.00a replay record embeds eight 0x24-byte
intrusive stage nodes. Each node's unlink destructor at `0x4374D0..0x4374F4`
has a complete, independent **37-byte strict match**. The predecessor at
`+0x1C` has its successor at `+8`, the successor at `+0x20` has its
predecessor at `+4`, and the destructor finally clears the local `+0x1C`
then `+0x20` links. Source is the natural member destructor in
`ReplayRecordStageLink.cpp`. The final store order is visible in the original
and reproduces the native instruction sequence without byte patches.

Direct Ghidra review independently identifies all 37 bytes, two conditional
branches, the plain `RET`, next-function alignment padding, no external
interior references, and callback references from the replay record EH
vector destructors (`0x435DCA`, `0x435E41`, `0x435F31`). The locked VC9 RTM
normal-COFF context generates a unique 37-byte destructor COMDAT with **zero
relocations**; the dedicated canonical `replay-record-stage-link-destructor`
unit strictly cold-replays all 37 bytes with zero differences.

The enclosing **429-byte replay-record cleanup owner at `0x435C50`** is now
strictly exact as well. It frees record data, clears eight segment streams,
deletes owned buffers, unregisters up to three chain elements with optional
runtime critical-section locks, clears its singleton if owned, and destructs
the eight stage links above. The complete owner preserves its SEH frame,
stack-passed record pointer, all 14 calls, and `RET 4`.

## Full 429-byte record cleanup owner — exact

`src/TitleMenuRecord.cpp` contains a naked private owner,
`_TitleRecordDestructorPrivate@4`, built against a distinct POD physical view
in `TitleMenuRecordDestructorView.hpp`. A linker alternate-name resolves the
original `??1TitleMenuRecordView@@QAE@XZ` member-destructor reference to this
owner. This avoids giving the real game-facing class two different C++
layouts: `TitleMenuRecord.hpp` and all factory/menu translation units retain
the actual eight `ReplayRecordStageLink` members, while the destructor body
uses only their identical physical offsets and invokes the original vector
destructor callback explicitly.

The complete owner reproduces all 114 instructions and all 429 bytes,
including the SEH registration/restore, three independent chain-removal
paths, eight stage-stream cleanups, three critical-section pairs and the
eight-element stage destructor iterator. The three `UnregisterChainElement`
calls explicitly load ECX from the element before EDX from the manager,
matching the native register-move order and the callee's `__fastcall` ABI.

The canonical `title-menu-record-destructor` unit links the real typed record
factory, stage constructor and stage-link destructor. Its complete primary-TU
PDB contribution strictly replays **429/429 bytes, 114/114 instructions,
14/14 calls and one `RET 4`**. All **38 address/call fields** (30 DIR32 and
8 REL32) are independently bound; there are zero byte differences. The
strict MAP validator now permits alternate public spellings only when every
alias is a function at the exact same owner entry; any interior entry or
repeated requested owner remains rejected (25 synthetic falsification tests
pass). Original C++ class/TU provenance remains unknown.

The earlier natural-C++ `/GL` experiments and six argument-move residuals
are historical diagnostics superseded by this exact private naked owner.

## Mixed producer check (2026-11-12)

The native 429-byte owner was re-linked with its two genuine stage producers
and either an opaque chain-removal anchor or the real `ChainRemove.cpp` built
as normal COFF. Both preserve the exact **429-byte/114-instruction/14-call**
layout and the same **six** non-field bytes of argument-move scheduling.
Building both stage producers as normal COFF instead shortens it to 427
bytes and changes over 200 non-field bytes. The best maintained source stays
unchanged; neither alternate context is strict exact. Private evidence:
`.analysis/probe-title-record-mixed-producer-20261112.log`.

## Exact 51-byte stage construction (2026-11-12)

Independent Ghidra review of native `0x437490..0x4374C2` recovered the
constructor missed by the initial function inventory. It is a complete
**51-byte** member owner referenced by both 199-byte and 198-byte replay
record factories as the eight-element vector constructor callback. The
function zeroes each of nine DWORDs in the 0x24-byte stage, reads two of the
cleared cursor fields back into the companion slots, then writes the stage's
self-node pointer at +0x18 and clears the two intrusive links. The constructor
has a plain `RET`, following CC padding and no foreign interior entries.

An ordinary C++ loop collapses two stores/reads and generates 48 bytes. The
maintained constructor expresses each *observed memory write* explicitly
through a volatile physical field view; it then performs the two cursor copies
and normal typed link publication. This preserves the actual native memory
operations, not inert padding or fabricated machine instructions. The pinned
VC9 RTM normal-COFF unit `replay-record-stage-link-constructor` strictly
replays **51/51 original bytes with zero relocations or differences**. The
paired 37-byte stage-link destructor remains independently exact after the
shared header change. The original constructor's class/TU name is still open.

## Full replay record factories at 0x435E00 and 0x435EF0 — exact

Both previously unsourced record factories are now complete naked x86 owners
in `src/ReplayRecordFactoryOwners.cpp`, with separate canonical pinned VC9
RTM `/GL` linked-PE units:

| Owner | Native span | Calls | Linkage fields | Result |
| --- | ---: | ---: | ---: | --- |
| `CreateReplayRecording` | 199 bytes | 6 | 5 DIR32 + 6 REL32 | Exact |
| `CreateReplayPlayback` | 198 bytes | 6 | 4 DIR32 + 6 REL32 | Exact |

The recording factory allocates the 0x2DC-byte record, invokes the eight-
element 0x24-byte EH vector constructor with the real stage constructor and
destructor callbacks, clears the complete allocation, then calls the registry
at `0x4A8D88`. On initialization failure it conditionally invokes the record
destructor and frees storage. The playback factory follows the same
allocation/vector-construction/zeroing path, sets mode at +0x10 to 2, calls
the exact stage-data loader, and destroys/frees on failure. Both retain the
original SEH frame, private register/stack argument order, branch layout and
two `RET 4` exits.

`title-replay-recording-factory` strictly replays **199/199 bytes and 61/61
instructions**; `title-replay-playback-factory` strictly replays **198/198
bytes and 60/60 instructions**. Every EH handler, security cookie, stage
callback, registry pointer and direct helper call is independently bound in
the linkage manifest. Both units have zero byte differences. No full-image
cold replay was run; only these two factories and their affected record
cleanup owner were rebuilt.

The old natural-C++/derived-storage factory experiments below are retained
only as historical diagnostics; they are superseded by the current exact
naked owners. The original C++ class/TU provenance remains an open question,
not a prerequisite for these complete native owner proofs.

## Strict 79-byte replay payload checksum (2026-11-12)

The independently reviewed native member owner at `0x437500..0x43754E`
returns an unsigned byte sum of a variable-length payload beginning at
receiver `+8`. It accumulates even and odd byte positions separately and adds
one final byte when the payload has odd length; the entry has a stack length
argument, `RET 4`, no relocation fields and two native replay readers at
`0x4378F0` and `0x437A70`.

In ordinary VC9 `/O2` compilation, declaring the trailing-byte accumulator
only after the loop produced a shorter 76-byte body. Moving its declaration
before both loop accumulators preserves the original observable local lifetime
and induces VC9's native stack slot/reload, with no register directives or
instruction padding. The maintained C++ `ReplayPayloadChecksumView` uses a
physical eight-byte header and real unsigned byte loads. Canonical
`replay-payload-checksum` cold-replays the **entire 79-byte body**, with zero
relocations and zero byte differences.

## Historical factory and generic-chain diagnostics

The 2026-11-12/13 natural-C++ factory, derived-storage and generic-chain
experiments remain in `.analysis/` as evidence of the earlier codegen
barriers. Their 200/199, 198/198, 370-byte and six-byte residual measurements
are superseded by the complete naked owners and strict same-entry alias
validation documented above; they are not current implementation results.
