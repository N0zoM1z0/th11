# Animation renderer lifecycle and pixel helpers

Eight complete source mappings cover3,746 target bytes. They include the
2,601-byte renderer constructor,225-byte destructor,487-byte buffer initializer
and273-byte VM reclamation function. Five complete public linked contributions
have cold exact proofs totaling890 bytes. Constructor/destructor EH ownership
and codegen remain open; source presence grants no exact credit.

The maintained inventory adds37 previously absent entries:29 constructor-bound
callbacks, four pixel helpers and four unresolved identity entries. Together
with the four existing lifecycle candidates, this checkpoint reviews41 complete
boundaries and origins. All41 origins remain indeterminate. Explicit read-only
Ghidra spans and independent PE decoding establish entries without mutating the
private database or replacing maintained inventory rows.

| Function | Address | Full bytes | Current state |
| --- | --- | ---: | --- |
| AnimationRendererConstructor | 4526F0 | 2,601 | Source; candidate2,601 /153 diagnostic differences; EH open |
| AnimationRendererDestructor | 453130 | 225 | Source; candidate202 /73; EH open |
| InitializeAnimationWorldBuffer | 453220 | 487 | Exact;25 independently reviewed fields |
| DestroyRendererAnimationVm | 4558E0 | 273 | Exact;4 fields |
| ResolveAnimationTextureFormat | 453410 | 40 | Exact;2 fields |
| AccumulateAnimationArgbPixel | 453440 | 30 | Source; candidate30 /6 |
| AccumulateAnimationA1Pixel | 453460 | 45 | Exact; no fields |
| AccumulateAnimationA4Pixel | 453490 | 45 | Exact; no fields |

## Complete object lifetime

The constructor belongs to the renderer. Its allocation is7BD894 bytes, with
4,096 real434-byte embedded VMs atBC, one VM at435180 and31 more at7B563C.
C++ members express these actual constructor/destructor lifetimes, including
exception cleanup. The subsequent whole-object memset is present in the target,
even though it clears already constructed VM storage. Only the4,096 main VMs
then receive ResetAnimationVm. No replacement allocation or deferred member
construction is introduced.

Two global quads receive fourth-scalar1 and standard UV corners:24-byte scratch
vertices rooted at4C9178 and the existing28-byte RenderVertex quad at4C91D8.
Their XYZ and color cells are untouched here. Renderer textureFactor4355B8
becomes1, worldBuffer4355CC and texture4355BC become0, cache bytesC0/C1/C2/C3
become0 andC4 becomesFF. Four40-byte prefix slots receive marker-1; their other
fields retain unknown names. Object offsets and allocation size have compile
assertions. These physical views do not claim original class or data ownership.

The constructor registers29 permanent chain elements, two update and27 draw.
Each allocation initializes its real chain links, ORs flags3, installs callback
and renderer context, and clears auxiliary fields. The suffixes in source name
registration priorities; they are not layer numbers. The final device operation
is SetVertexShader0.

| Callback entry | Priority | Full bytes | Role |
| --- | ---: | ---: | --- |
| 455120 | 26 | 7 | Update; tail to455480 |
| 455130 | 8 | 7 | Update; tail to455560 |
| 455140 | 4 | 12 | Draw |
| 455150 /455160 | 6 /8 | 15 each | Draw |
| 455170 | 10 | 126 | Draw with camera/state work |
| 4551F0 /455200 /455210 /455220 | 12 /15 /16 /17 | 15 each | Draw |
| 455230 /455240 /455250 /455260 | 18 /19 /21 /23 | 15 each | Draw |
| 455270 /455280 /455290 /4552A0 | 24 /26 /28 /32 | 15 each | Draw |
| 4552B0 /4552C0 /4552D0 | 34 /36 /39 | 15 each | Draw |
| 455320 | 40 | 46 | Draw with state work |
| 455300 /4552F0 /4552E0 /455310 | 50 /60 /61 /49 | 15 each | Draw |
| 455350 /4553A0 | 48 /63 | 74 each | Draw with camera work |
| 4553F0 | 62 | 133 | Draw with camera restoration |

All29 callback bodies remain source-open; declarations allow the real lifecycle
call graph to be compiled. Each registration address was reviewed from the
constructor. The864-byte callback-group span has28 external interior references,
all precisely to the other28 independently partitioned entries. None targets
an interior instruction of an individual callback. RETs/tail jumps and following
CC padding determine complete extents; no group-sized owner is claimed.

The destructor traverses activeHead7B562C, then otherHead7B5634, saving next
before reclaiming the current VM. Real members then destroy31 extra VMs, the
default VM and4,096 main VMs in reverse order. Default storage435580 is freed
and cleared inline in the target. This body neither unregisters chain nodes nor
releases the vertex buffer; those operations are not invented here.

Constructor bounding extent2,601 includes three internal alignment bytes;
Ghidra's sparse body has2,598. Destructor225 includes six alignment bytes;
its sparse body has219. Full spans retain both gaps. Native EH handlers are
48A7D7 and48A507. The current destructor instead calls the default member's
out-of-line destructor and has different unwind state transitions; its202-byte
candidate cannot enter the ordinary complete225-byte acceptance lane. Equal
constructor length also grants no exact credit, and specialized EH acceptance
remains open.

## VM reclamation and world buffer

The273-byte reclaim boundary consumes renderer in native EDI and VM in EAX.
The renderer is essential: updateNode VM+4 is compared with activeTail,
activeHead, otherTail and otherHead in that order, adjusting endpoints. It
unlinks both updateNode and drawNode VM+10, clearing next before previous.
Native code dereferences the VM before a late heap-path null test; no early
null guard is added.

VM addresses inside the main pool use a signed byte-distance/434 quotient to
clear used[index] at renderer4340BC. This establishes the4,096-byte occupancy
map immediately after the VM pool. The pool path frees/clears storage400 and
resets the VM; the heap path frees/clears that storage and directly calls
operator delete without ResetAnimationVm. Both return0. Natural field access
through the VM and byte-distance relative to the renderer/header reproduce the
native addressing and273-byte full contribution; a temporary node-base/index
form produced267 bytes/101 diagnostic differences.

World initialization obtains the renderer through4C3268. Its four20-byte
XYZ/UV corners at4355D0 form a +/-128 square, with Z0 and standard UV0/1. It
copies XYZ aggregates and individual UV floats to24-byte projected scratch
vertices at4C9248. Their fourth scalar is left unchanged; its original meaning
is unproved. These are distinct layouts from the shared28-byte diffuse quad.

The device creates80 bytes with usage0, FVF102 and MANAGED pool1, locks the
whole buffer with flags0, copies80 bytes, unlocks and sets stream0 at stride20.
The target adds no HRESULT recovery. Direct renderer-member accesses and
separate U/V assignment groups reproduce487 bytes; an early local vertex-base
form produced382 bytes/351 diagnostic differences. No padding was added.

The25 fields bind renderer4C3268, runtime4C3280+8, float constants497F34/-128
and497F30/+128, and all20 written scratch cells rooted at4C9248. Four reclaim
calls independently bind free45FC3B twice, ResetAnimationVm401FD0 and direct
operator delete45FD49. Full byte equality requires every binding and every
remaining byte; masked diagnostics are separate.

## Pixel boundaries and unresolved identities

The40-byte format helper tests the byte at runtime4C3280+200, bit0. If set,
it maps A8R8G8B8/UNKNOWN to index5 and R8G8B8 to3, retaining other indices.
There is no bounds clamp. The table at496548 has six formats
`0,21,25,23,20,26`; the following six DWORDs are bytes per pixel
`4,4,2,2,3,2`, not more formats. Its two address fields bind that table and the
runtime byte independently. The existing texture TU's inline selector is
unchanged; this separate public function models the reviewed boundary.

ARGB statistics add unscaled red, green and blue bytes only when alpha is
nonzero, then increment count. The target loads both green and blue before
writing the green sum. Straight source differs in six scheduling bytes.
Explicit green/blue temporaries produced a28-byte contribution and changed
private registers; that experiment was rejected. No restrict/alias promise is
invented to force load motion.

Packed-pixel helpers take one ushort snapshot. A1R5G5B5 requires bit8000 and
adds RGB5 at10/5/0; A4R4G4B4 accepts any nonzeroF000 and adds RGB4 at8/4/0.
Neither expands, weights or clamps channels. A ushort local naturally restores
the target's second MOVZX after the alpha test, giving full45-byte contributions
with no address fields. An unsigned-int local produced42 bytes instead.

The four entries4526C0,4526D0,4526E0 and453120 each contain a complete3-byte
ECX-to-EAX identity/RET followed by CC padding. No callers or source types are
established. Empty constructors are only a hypothesis, so no invented identity
wrapper or class is counted as source. Their generic pattern also cannot prove
compiler, header, reused-library or game origin.

## Proof scope

Cold families are `build/match-linked/AnimationRendererLifecycle` and
`build/match-linked/AnimationPixelHelpers`. Only the five new units were
cold-replayed, with current source/header/tool/producer receipts, complete
public primary-TU PDB contributions, PE/MAP/PDB identity,31 reviewed fields and
zero final byte differences. No old canonical source/header/context changed;
no old exact cohort, infrastructure suite or broad behavior harness was run.
Aggregate state is93 exact functions/17,731 bytes,2,038 candidates,222 reviewed
boundaries and179 source mappings. Authored90, reviewed-indeterminate123,
excluded7 and pending1,818 are separate origin counts.

## Exact renderer-priority dispatch callbacks (2026-11-11)

The 22 previously source-open draw-chain callbacks now have natural C++ in
`src/AnimationRendererPriority.cpp`: one 12-byte entry at `0x455140`, and
21 15-byte entries covering the ordinary priority callbacks through `0x45531E`.
They forward a renderer context and **dense draw layer**, not their public
registration priority, to `DrawAnimationRendererLayer` at `0x455660`.
For example, priority 04 draws layer 0, priority 39 draws layer 18, priority
49 draws layer 21, and priority 61 draws layer 24. The larger stateful
priorities 10, 40, 48, 62, and 63 are separate owners recovered in the
following checkpoint.

The genuine `AnimationRenderer` constructor registers these callback symbols.
Using that registration context and the real renderer dispatcher as distinct
VC9 `/GL` source inputs recovers the private EDI renderer/EAX layer ABI
naturally, without declaring a nonstandard ABI or injecting instructions.
The complete normal-COFF callbacks were only nine bytes; the linked context
restores the native 12/15-byte bodies. Each callback has one reviewed REL32
call field pointing to the `0x455660` dispatcher; no other address fields or
embedded tables occur in the complete reviewed extents.

All **22 canonical linked units** have now passed an affected-only cold build,
unique full primary-TU PE/MAP/PDB ownership, producer/source/tool receipts,
exhaustive call-field replay, and **zero final byte differences**. This adds
**327 exact code bytes**. The source, native unit identity and proof are in
`config/match-units.toml`, `build/match-linked/AnimationRendererPriority/`,
and `.analysis/renderer-priority-cold-proof-20261111.log`. No unrelated unit
or whole-program cold replay was run. This proves the complete individual
callbacks, not the construction of the full runnable game.

## Exact stateful renderer-priority callbacks (2026-11-11)

Five additional full owners now have natural source in
`src/AnimationRendererStatePriority.cpp` and strictly **exact** complete
linked-PE proofs. Unlike simple layer forwarding they select one of three
0x118-spaced runtime camera slots, set the current camera pointer at +54C,
call the existing exactly reconstructed `ApplyCamera` at 42A810, and apply
the D3D viewport before storing camera index +550.

| Priority | Native entry / bytes | Recovered stateful behavior | Reviewed code fields |
| --- | --- | --- | ---: |
| 10 | 455170 / 126 | Stage camera +434, index 2, conditionally flush/disable fog cache +990, draw layer 3 | 12 |
| 40 | 455320 / 46 | Clear camera +51C/+520 and batch +B0/+B4 XY offsets, draw layer 19 | 4 |
| 48 | 455350 / 74 | Screen camera +31C, index 1, draw layer 20 | 7 |
| 63 | 4553A0 / 74 | Screen camera +31C, index 1, draw layer 29 | 7 |
| 62 | 4553F0 / 133 | Alternate camera +204, index 0, draw layer 30, restore screen camera +31C/index 1 while preserving first draw result | 13 |

The actual constructor registration, camera source `src/Camera.cpp`,
batch flush source `src/RenderBatch.cpp`, and renderer dispatcher were
linked as separate supports under pinned VC9 `/GL`. The five complete
public-primary-TU PE/MAP/PDB contributions match their target extents;
all **43 independently reviewed DIR32/REL32 fields** resolve to their
actual camera/globals/device source references, are rebound to the native
target, and leave **zero differing bytes**. The natural C++ reproduces even
the 46-byte callback's shared `FLDZ` stores and 133-byte callback's
register-preserved return value without inline machine-code insertion.

This second affected-only cold replay added **453 exact code bytes**.
Evidence: `config/match-units.toml`,
`build/match-linked/AnimationRendererStatePriority/`, and the private
`.analysis/renderer-state-priority-cold-proof-20261111.log`. Combined with
the preceding 22 callbacks, **all 27 registered draw-priority callbacks
now have 780 target bytes strictly exact**. The independently reviewed
renderer update callbacks at 455120 and 455130 are not part of this claim.
