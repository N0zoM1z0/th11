# Stage data and animation document loading

Three related loader functions have full maintained source. All remain
non-exact; the VM binding cleanup in `VM_BINDINGS.md` is independently exact.

| Function | Native span | Native / source bytes | Unequal diagnostic positions |
| --- | --- | ---: | ---: |
| `LoadStageResource` | `0x00403FC0..0x004040E8` | 297 /297 | 17 |
| `BeginLoadAnimationDocument` | `0x00454190..0x00454355` | 454 /434 | 337 |
| `AcquireAnimationDocument` | `0x00454360..0x004543DB` | 124 /131 | 80 |

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

Success derives the pointer table at+90, and section pointers from the relative
DWORDs at header+4/+8. The signed16-bit object count at+0 controls in-place
pointer relocation by adding the copied-data base. The signed16-bit VM count
at+2 is multiplied by0x434 for malloc. Its return is published without a null
check, and the function returns0. The source keeps these failure and ownership
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
at0x004543E0 is now recovered as LoadAnimationChunkSource; the later worker
decodes textures/sprites/scripts at0x004545A0. See `ANIMATION_CHUNKS.md`.

## Acquisition and waiting

A non-null cached slot returns immediately after a trace call, without testing
busy or validating the cached data. On a fresh successful creation, busy is
set1. While runtime flags do not contain0x180, Sleep(1) happens before busy is
tested. Busy zero exits; shutdown flags can also exit while busy remains1.
There is no timeout. These details are retained, including one mandatory Sleep
when the flag gate is open, signed indices and incomplete-cache behavior.

Native acquisition receives ECX index/EBX filename. Creation receives ECX
filename plus renderer/index on the stack and RET8; the current compiled
creation instead receives ECX index/EDX filename plus stack renderer/RET4.
Stage loading already has the native EAX filename/stack stage view/RET4, but
17 register/operand differences remain. No ABI or operand normalization grants
credit for these functions.

The native trace entry0x00456AD0 is the previously reviewed RET sink. In cached
acquisition the native caller retains EAX/ECX across it; the opaque source call
produces different preservation/spilling. This is the existing trace/context
gap, not a reason to fabricate a callee contract or pad code. The first loader
family gave297/24 differences; adding directly needed existing resource/log
bodies and independent unknown inputs gives297/17. Old canonical proofs were
not replayed. Do not repeat those same contexts without new evidence.

## Bounded validation and remaining work

Private `.analysis/stage-resource-differential.json` records34 cases:33 returns
and one persistent-wait prefix stopped at8 modeled Sleep calls. It runs all
three complete original/candidate bodies independently, with modeled resource,
allocator, decoder, logger, trace and Sleep contracts. Actual target/candidate
private calling conventions are handled separately. Trace records are call
instrumentation, not native printed output from the RET sink.

Comparisons cover stage storage, relocated pointers, allocation order and
sizes, cache publication, file/table contents, decoder indices, returns, GS
cookies, registers and stack. Cases include cached source, resource/animation/
chunk failures, signed counts/indices, empty/multiple chunks, busy-cache returns,
shutdown before/during waiting and persistence. Negative VM counts preserve the
large unsigned malloc request; the bounded allocator fixture returns null
without allocating that amount. Real I/O, decompression, texture decode,
background threads and the complete native game are not validated here.

The standalone focused fixture recorded one CPU/nice10 and maximum RSS42,148 KiB, with
timely fixture collection. Ghidra/compiler jobs retain
the shared serialization lock and limits. Remaining useful targets include
texture codegen gaps and stage update/draw bodies. The texture
constructors and stage lifecycle now have full source; initialization0x004024C0
is strict630-byte exact, while the603-byte EH destructor remains non-exact.
The former role guesses for0x004543E0/0x004544C0 were incorrect;
their source-loader/cursor behavior is now documented in `ANIMATION_CHUNKS.md`.
The mode-transition routine0x00406510 is unrelated to VM lookup and owns a
six-entry trailing jump table; do not treat its241-byte code-only candidate
as a reviewed standalone lookup function.
