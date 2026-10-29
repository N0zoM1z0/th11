# ASCII manager lifecycle and sprite binding

Direct local Ghidra queries on the hash-attested original Japanese v1.00a
review six complete source-mapped owners, totaling1,369 target bytes. Two
callback entries were absent from the initial Ghidra function inventory and
are now tracked separately. All six reviewed extents are contiguous, end at
RET/RET4, contain no embedded tables or external interior references, and are
followed by CC padding. Destructor EH metadata and funclets are a separate
ownership question.

| Function | Address | Target bytes | State |
| --- | --- | ---: | --- |
| AsciiManagerConstructor | 401000 | 98 | Exact linked proof |
| InitializeAsciiManager | 401080 | 391 | Source present; non-exact |
| AsciiManagerDestructor | 401210 | 475 | Source present; EH/non-exact |
| AsciiManagerDrawCallback | 4014C0 | 9 | Exact linked proof |
| AsciiManagerOverlayCallback | 4014D0 | 7 | Exact linked proof |
| BindAnimationSprite | 44AB40 | 389 | Source present; non-exact |

## Layout and construction

`AsciiManagerObject` supplies a lifecycle view alongside the existing partial
`AsciiManagerView`; size and key offsets are checked at compile time. It has
a virtual destructor, flags+4, update/draw chains+C/+10, two actual embedded
0x434-byte VM members at+14/+448, string records+87C, animation pointer+184AC,
and overlay chain+184B8. It reuses the already recovered `StageEmbeddedVm`
constructor/destructor; the name denotes shared physical storage rather than
a claim that the original class was stage-specific.

Construction writes the vtable, constructs both embedded VMs, then clears the
entire0x184BC-byte object. The original really clears its own vptr and member
state at this point. It sets flag2, both scales to1, singleton4A8D58, color
FFFFFFFF, cell18490=0 and draw mode184A4=9. Do not replace this with a
zero-initialized aggregate or omit the real member lifetimes. The vtable at
4938C8 and nearby original `AsciiInf` diagnostic identify the original class;
493868 contains `ascii.anm`. These target facts support authored-game origin
for the lifecycle and its two explicitly registered callbacks.

## Initialization and cleanup

Initialization acquires animation slot2 with `ascii.anm`; failure logs the
original load error and returns-1. It constructs three36-byte chain elements,
preserving the observed partial flag clear, embedded link ownership and
callback/context stores. It registers update4014A0 at priority4, draw4014C0 at
42, then overlay4014D0 at2B. The two embedded VMs are reset, assigned the
animation file, and bound to sprite0 and62 respectively. Success returns0.

The draw wrapper at4014C0 pushes mode0 and its ECX context, calls401670, and
returns the result. The overlay wrapper at4014D0 pushes the context, calls
401CE0, and returns that result. Independent9/7-byte explicit-span reviews
exclude the seven/nine following CC bytes and find no interior references.
The drawing bodies are now maintained separately in `src/AsciiDrawing.cpp`;
see [full drawing and formatting](ASCII_DRAWING.md). They remain external to
the lifecycle translation unit.

Destruction restores the vptr; removes update, draw and overlay chains in
order, with the same conditional section0 lock and depth bookkeeping as the
stage lifecycle; then destroys/deletes/clears renderer document slots2 and0.
It clears the singleton and automatically destroys overlay VM before text VM.
It does not test whether the singleton aliases this object. The original
475-byte body has an actual C++ EH registration; no exact proof is claimed
for this owner or its unwind contexts.

## Sprite binding

44AB40 receives VM in EAX, sprite index in ECX and document in EDX in the
original LTCG call context. Maintained source retains ordinary C++ calling
conventions. An unloaded document or nonzero ready/busy flag returns-1.
Success sets the signed16-bit marker39C, file3B0 and sprite pointer3AC; writes
the four UV corners (left/top, right/top, left/bottom, right/bottom); stores width/height; initializes
geometry and texture matrices; scales geometry by half the dimensions and
texture by dimension/texture-size times sprite scale; copies geometry into
its saved matrix; and returns0. The marker at39C is not the existing reset
routine's separate cell374. No bounds/fallback behavior is invented.

The focused binding view reads the ready flag through ordinary storage;
it does not model the document loader's polling contract. Original provenance
of this engine-specific function remains indeterminate, independently of its
reviewed extent and complete source.

## Current compiler evidence

The final cold family proves the constructor's complete98-byte primary-TU PDB
contribution and all five fields: vtable4938C8, two constructor calls402270,
memset46A720 and singleton4A8D58. The9/7-byte callbacks each have one reviewed
call binding (401670/401CE0). Every byte agrees after exhaustive replay;
receipts and PE/MAP/PDB identity are current. These add114 exact bytes.

Initializer diagnostics improve from374 bytes/301 differences with opaque
support to391/30 with real document/log/sprite support and a captured file
pointer. Remaining gaps include AcquireAnimationDocument's index register,
log-context register and VM/file register scheduling. Destruction is433/290
against475 target bytes; its incoming-this placement and EH remain open.
Sprite binding is388/289 against389 bytes; it still lacks the native cached
selected-sprite pointer in EDX and has x87 scheduling differences. These are
full-owner address-normalized diagnostics, not exact claims.

The sprite view was corrected by reviewing each native UV store: U0 goes to
80/70, U1 to88/78, V0 to7C/74, and V1 to8C/84. Four2-float corners express
that layout directly. Treating70..8C as separate current/base rectangles
incorrectly swaps7C and84. Layout assertions and the native store review,
not merely a compiled byte count, establish this source correction.

## Reproduction

The canonical lifecycle family uses `src/AsciiLifecycle.cpp` as primary and
`probes/ascii-lifecycle-context.cpp`, real embedded-VM/chain/reset/document/log
and sprite-binding sources as support. Independent document/log/sprite inputs
prevent the optimizer from specializing all helpers to this initializer's
constant arguments. The image is a diagnostic link context, not a runnable
game. Profiles remain VC9 RTM `/TP /O2 /Ob1 /Gy /MT /Isrc /EHsc /GL`.

Private target evidence: `.analysis/ascii-entry-review.txt`,
`ascii-deps-review.txt`, `ascii-draw-callback-review.txt`,
`ascii-overlay-callback-review.txt`, `ascii-bind-review.txt` and their attestation
logs. Compiler records remain in `ascii-lifecycle-probe*.log`; the retired
private driver is removed. Canonical receipt/output location is
`build/match-linked/AsciiLifecycle`. Diagnostic differences alone grant no
exact credit. Only affected units are cold-replayed.
