# Text workspace, fonts and executable checksum

Direct Ghidra review covers four complete native spans in the attested
original Japanese TH11 v1.00a. Branches, returns, incoming/interior references
and neighboring thunk/RET/CC boundaries were checked independently. No span
contains an embedded table or external interior entry.

| Function | Complete target extent | Bytes | State |
| --- | --- | ---: | --- |
| `ReleaseTextSurface` | 0x00443560..0x004435C8 | 105 | Exact |
| `CreateTextSurface` | 0x004435F0..0x0044377B | 396 | Exact |
| `InitializeTextResources` | 0x00444480..0x004445D9 | 346 | Exact |
| `ComputeExecutableChecksum` | 0x004476F0..0x004477E5 | 246 | Source present; non-exact |

The three exact functions add847 owned bytes and56 independently reviewed
fields. Ghidra's creation body reports393 addresses in two ranges: the
bounding396-byte span also contains a three-byte LEA alignment instruction
at0x0044361D, skipped by a jump into the format-search loop. A separate
read-only full-span decode reviewed that instruction and every remaining
byte. The proof uses all396 bytes, including alignment and the full RET4 tail.

## Shared text workspace

The observed global workspace begins at0x004A57C0. Its first256 bytes receive
RNG samples during text initialization. The remaining physical fields agree
with the surface transfer/compositor layout reviewed previously:

| Offset | Observed field |
| --- | --- |
| 0x000 | 256-byte sample table |
| 0x100 | D3DFORMAT |
| 0x104 / 0x108 | Width / height |
| 0x10C / 0x110 | Image byte count / row pitch |
| 0x114 | GDI device context |
| 0x118 | Previously selected bitmap |
| 0x11C | DIB bitmap handle, also the transfer functions' presence test |
| 0x120 | Pixel pointer |

The new view describes this observed span rather than a recovered complete
class. It leaves the earlier SurfaceBufferView and all previous exact inputs
unchanged. Generic creation/release code and shared layout alone do not prove
original game versus library ownership: those two origins remain individually
indeterminate even though their codegen is exact. Font initialization's game
RNG/runtime integration independently supports authored-game classification.

Release returns false without touching fields if DC is null. Otherwise it
reselects the previous bitmap, deletes the DC and bitmap, clears width, height,
DC, bitmap, previous selection and pixels, and sets format to-1. Pitch, image
byte count and the sample table remain unchanged. All GDI return values are
ignored; success returns a boolean in AL.

## Bitmap creation and publication

Creation first releases the old workspace and clears one108-byte
BITMAPV4HEADER. It searches24-byte records at0x004A3770 for the requested
format or the-1 sentinel. The records contain format, signed bit count and
alpha/red/green/blue masks; formats22/21/24/23/25/26 precede the sentinel.
The target rejects argument-1. Other unsupported arguments retain the
sentinel record rather than getting an added validation guard.

Row pitch uses the observed signed expression
`((1024 * bits / 8 + 3) / 4) * 4`. Width is1024, the top-down DIB height is
**-65**, while image byte count and the published logical height cover**64**
rows. The extra row is retained. BI_BITFIELDS and the four masks are assigned
except for X1R5G5B5/X8R8G8B8, which keep the zeroed BI_RGB mode.

CreateDIBSection failure returns false. Success clears only the declared image
bytes, obtains a compatible DC and selects the bitmap. DC/selection failures
receive no additional guard. The native publication group stores DC, bitmap,
pitch, previous selection, format, pixels, byte count and dimensions before
returning true. A local selection result keeps that return value available
while the other fields are published; the maintained natural source generates
the exact native order without padding or executable-byte patches.

Early selection-result publication produced392 bytes with44 differences.
Deferring it and the format publication reproduced the two simultaneous local
loads and native stores: an intermediate396-byte form had three differences,
and the final full contribution has zero. These earlier forms are recorded
private diagnostics, not alternate exact claims.

## Font and sample initialization

Initialization requests A4R4G4B4, falling back to A8R8G8B8 on false. It does
not branch on the fallback result before initializing samples and fonts.
For each of256 entries it runs the observed16-bit RNG operation and stores
`seed >> 9`, giving seven-bit samples. Seed/call count are at0x004C2F00/+4.
The inline operation preserves flag0x8000-controlled lock index10 and its
depth byte, including the separate post-update flag test. It is an actual
inlined RNG use, not another mapped function or additional byte credit.

Four CreateFontA calls use SHIFTJIS_CHARSET, ANTIALIASED_QUALITY,
FIXED_PITCH|FF_ROMAN, and zero width/angles/style/precision values:

| Font face | Height | Weight | Handle cell |
| --- | ---: | ---: | --- |
| ＭＳ ゴシック | 32 | 400 | 0x004C2EEC |
| ＭＳ 明朝 | 32 | 600 | 0x004C2EE8 |
| ＭＳ ゴシック | 15 | 700 | 0x004C0EE4 |
| ＭＳ 明朝 | 15 | 700 | 0x004A8ED4 |

The face strings at0x00496654/0x00496664 were independently decoded as CP932.
String data definitions and complete production ownership remain open;
probes use distinct unresolved data anchors, not a runnable game image.

## Strict cold acceptance

Creation has18 fields: release and two memset REL32 calls, three format-table
DIR32 references, three GDI IAT references and nine workspace member references.
Release has14 DIR32 fields: three GDI IATs and eleven workspace accesses.
Initialization has24 fields: two creation calls, three IATs, six runtime
members, four RNG accesses, the sample table, four face-string references and
four font-handle cells. Target instructions/imports, format records, face
strings and pinned SDK types/constants independently establish these values.

The VC9 RTM profile is `/TP /O2 /Ob1 /Gy /MT /Isrc /GL`. Release/creation share
one primary-TextSurface family; initialization has its own primary-TextResources
family. Both include actual sibling bodies and the same non-runnable context.
Cold receipts bind source/includes/profile/tools/producer and PE/MAP/PDB
artifacts. Strict primary-TU owner validation, complete extents, all56 field
bindings and all847 owned bytes replay with zero differences.

```bash
scripts/repo-python scripts/replay-exact-units.py \
  --unit release-text-surface --unit create-text-surface \
  --unit initialize-text-resources
```

Only these three new units are replayed, deduplicated into two cold families.
Previous32 canonical source/header inputs are unchanged and not replayed.

## Executable checksum source

The246-byte function's explicit EAX results correct the initial void
decompiler hypothesis. It queries its module path into261 bytes and requests
the existing resource loader's disk mode. Either path/load failure returns
0xFFFFFFFF without publishing state. Success sums32-bit words using a signed
`size / 4 - 1` bound: the last full DWORD and any remaining bytes are omitted.
Small/nonpositive bounds produce zero, preserving the native signed division.
It frees the original buffer, publishes size/checksum at MainRuntime+0x99C/
+0x998, and returns the checksum. The primary startup routine calls it.

The maintained candidate is251 versus246, with179 unrelocated differences;
there is no exact claim. Existing loader source and independent loader callers
remain visible in the diagnostic family, retaining its real private filename
convention. An indexed version224 and early-guard pointer version247 differed;
nested-success variants251 still differ in register saves, frame size and
publication scheduling. Do not repeat these shapes without new evidence.

Private evidence: `.analysis/font-startup-review.txt`,
`text-surface-release-review.txt`, `text-surface-create-full-span.txt`,
`text-surface-*-linked.json`, `text-resources-cold-replay.log` and
`executable-checksum-*`. Full native product/runtime closure, semantic
reconstruction and portability remain later open gates.
