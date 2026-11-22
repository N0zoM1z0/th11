# Stage data and animation document loading

Three related loader functions have full maintained source. All remain
non-exact; the VM binding cleanup in `VM_BINDINGS.md` is independently exact.

| Function | Native span | Native / source bytes | Candidate diagnostics |
| --- | --- | ---: | ---: |
| `LoadStageResource` | `0x00403FC0..0x004040E8` | 297 /297 | 20 non-field bytes; 84/99 instructions aligned |
| `BeginLoadAnimationDocument` | `0x00454190..0x00454355` | 454 /434 | 337 non-field differences |
| `AcquireAnimationDocument` | `0x00454360..0x004543DB` | 124 /128 | 28/38 instruction keys aligned |

Counts outside candidate address fields are diagnostics only. Direct Ghidra
explicit spans include3-byte internal alignment omitted by the begin routine's
451-address body and6 bytes omitted by acquisition's118-address body. Every
branch, return, real caller and adjacent RET/CC was reviewed; there are no
embedded tables or external interior references. No extent is cropped.

## Stage data

The stage view contains copied data+10, relocated pointer table+14, two section
pointers+18/+1C, animation file+178, VM array+17C, variant+3008 and cached source/
size+3038/+303C. Factory/constructor review now independently establishes the
0x3158-byte storage span and embedded-member layout in `STAGE_LIFECYCLE.md`;
the original source class declaration and TU partition remain open.

If source is absent, the routine clears the shared name buffer at0x004C9928,
concatenates the supplied filename and calls LoadResourceFile with size output
and disk-mode0. Failure returns-1. Existing source skips this path entirely.
Every attempt thereafter allocates and copies source bytes. It acquires the
animation filename at copied header+10 in slot `(variant&1)+3`. Animation
failure logs and returns-1 with the copy retained.

Success derives the object-offset table immediately after the typed
`StageBlobHeaderView` (`data + 1`, statically checked as 0x90 bytes), and section
pointers from the relative DWORDs at header+4/+8. The typed table boundary produces the same complete 297-byte focused VC9 `/GL`
candidate as the original raw `base + 0x90` expression; no fabricated instructions
or padding are introduced.

### Real ResourceFile call graph (2026-11-20)

The dedicated `load-stage-resource-real-callee-frontier` builds
StageResource.cpp as the primary TU and links the real ResourceFile.cpp,
archive lookup/decompression, byte cipher and runtime critical-section
implementations. This replaces the generic LoadResourceFile link anchor.
The call now reproduces the native private EAX filename input with size and
mode on the stack, rather than pushing the filename as a third stack argument.
The complete 297-byte candidate aligns 84/99 native instructions (up from
82/99 in the anchor-only context); 20 same-offset bytes outside candidate
relocation fields still differ. This remains diagnostic, not exact. The
remaining caller-side differences include the size/data register schedule,
AcquireAnimationDocument private ECX/EBX inputs, section-pointer register
order and the final VM-count register assignment. The signed16-bit object count at+0 controls in-place
pointer relocation by adding the copied-data base. The signed16-bit VM count
at+2 is multiplied by `sizeof(StageEmbeddedVm)` (statically checked as
0x434) for malloc. Its return is published without a null check, and the
function returns0. This replaces the last magic VM stride in the loader with
the actual C++ member type without changing the 297-byte /GL contribution or
84/99 native instruction-key alignment. The source keeps these failure and ownership
asymmetries. It does not insert validation, cleanup or bounds absent in native.

## Animation document creation

The renderer's32 file cells begin at+4350C0. The allocated document is0x134 bytes:
index+0, filename+4 (260-byte region), raw data+108, texture/script/sprite counts
+10C/+110/+114, sprite/script/texture tables+118/+11C/+120 and busy word+124.
These observations extend the earlier partial texture-file and binding views;
old source/header inputs remain unchanged.

Creation traces first, then rejects signed index>=32. There is no lower-bound
test. It formats a260-byte local path and loads resource bytes with null size
output/disk-mode0. A document allocation and zeroing happen even if that load
failed. The pointer is published in the renderer slot before raw data is tested.
Therefore a null return can leave a non-null, incomplete cached document.
Allocation failure is not turned into a clean additional error path.

With data present, the routine publishes the index, data and original filename.
ANM headers provide unsigned16-bit sprite/script counts at+4/+6 and a next
offset at+24, relative to the current chunk. Counts sum across this chain.
It allocates and clears20 bytes per texture/chunk, allocates72 bytes per sprite
and4 bytes per script, then loads chunk texture sources in order. A negative result
returns null without undoing the published document/tables. The source loader
at0x004543E0 is recovered as LoadAnimationChunkSource and is now strict
221/221-byte linked-PE exact with the real `BeginLoadAnimationDocument` producer; the later worker
decodes textures/sprites/scripts at0x004545A0. See ANIMATION_CHUNKS.md.

## Acquisition and waiting

A non-null cached slot returns immediately after a trace call, without testing
busy or validating the cached data. On a fresh successful creation, busy is
set1. While runtime flags do not contain0x180, Sleep(1) happens before busy is
tested. Busy zero exits; shutdown flags can also exit while busy remains1.
There is no timeout. These details are retained, including one mandatory Sleep
when the flag gate is open, signed indices and incomplete-cache behavior.

Native acquisition receives ECX index/EBX filename. The trace-aware
`animation-document-acquire-trace-frontier` links the real `AnimationTrace.cpp`
body rather than a generic call anchor. Its complete 128-byte candidate aligns
28/38 normalized instruction keys, up from 26/38 at 131 bytes in the
anchor-only context. The cached path now preserves EAX/ECX across the no-op
trace call as native does. The slow path remains non-exact: native creation
receives ECX filename plus renderer/index on the stack and RET8; the current
candidate instead uses EDX filename with a different stack/register setup.
No ABI or operand normalization grants exact credit.

**Reverted 2026-11-22.** The 2026-11-20 change (`24ec565`) modelled the
slow-path creator as a `__thiscall` member of a `reinterpret_cast` filename
view (`AnimationDocumentNameView::BeginLoad`) to put the filename in ECX.
That is a declared-ABI device, not natural source, and it broke four exact
callers: `RegisterSecondaryHudChains`, `InitializeSecondaryDebugHudVisuals`
(both one byte: the index moved from ECX to EAX), and
`ProcessSecondaryProgramExtensions` and `InitializeTitleMenuResources`
(owner size changed). The earlier note that both HUD callers still passed
was wrong; `git bisect` over `e7e6e15..24ec565` with the registration unit
identified `24ec565` as the first failing commit. The free function
`BeginLoadAnimationDocument` and the `4636b12` source are restored; all 19
exact units that link `AnimationDocument.cpp` or its probes were replayed.
The `int busy` field and the later `AnimationChunk.cpp` texture-store
changes were kept because `LoadAnimationChunkSource` depends on them.
Any new attempt at the native ECX-filename slow path must replay those 19
units before it is committed.

Stage loading already has the native EAX filename/stack stage view/RET4; its
separate real-ResourceFile frontier remains 297 bytes with 84/99 aligned
instructions and 20 same-offset non-field differences. The first loader
family gave 297/24 differences; adding directly needed existing resource/log
bodies and independent unknown inputs gives297/20 same-offset non-field
differences. Old canonical proofs were not replayed. Do not repeat those same contexts without new evidence.

## Remaining exact work

Remaining useful targets include
texture codegen gaps and stage update/draw bodies. The texture
constructors and stage lifecycle now have full source; initialization0x004024C0
is strict630-byte exact, while the603-byte EH destructor remains non-exact.
The former role guesses for0x004543E0/0x004544C0 were incorrect;
their source-loader/cursor behavior is now documented in `ANIMATION_CHUNKS.md`.
The mode-transition routine0x00406510 is unrelated to VM lookup and owns a
six-entry trailing jump table; do not treat its241-byte code-only candidate
as a reviewed standalone lookup function.
