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

## Proof scope and private evidence

Cold families are `build/match-linked/AnimationRendererLifecycle` and
`build/match-linked/AnimationPixelHelpers`. Only the five new units were
cold-replayed, with current source/header/tool/producer receipts, complete
public primary-TU PDB contributions, PE/MAP/PDB identity,31 reviewed fields and
zero final byte differences. No old canonical source/header/context changed;
no old exact cohort, infrastructure suite or broad behavior harness was run.
Aggregate state is93 exact functions/17,731 bytes,2,038 candidates,222 reviewed
boundaries and179 source mappings. Authored90, reviewed-indeterminate123,
excluded7 and pending1,818 are separate origin counts.

Private evidence includes `renderer-constructor-full-review.txt`,
`vm-following-effects-review.txt`, `renderer-delete-vm-review.txt`,
`renderer-chain-callbacks-review.txt`, `renderer-value-gaps-review.txt`,
`renderer-tail-gap-review.txt`, `texture-pixel-helper-gap-review.txt` and their
full mapped-text attestation logs. Independent partition/binding receipts are
`renderer-callback-boundaries.json` and `renderer-exact-independent-bindings.json`.
Current diagnostics are `renderer-lifecycle-direct-fields.log` and
`pixel-helpers-current.log`; baseline/typed-snapshot probes retain rejected
hypotheses. `renderer-pixel-five-cold.log` records full cold acceptance. Target
bytes, decompiler hypotheses, compiler payloads and mutable Ghidra data remain
private.
