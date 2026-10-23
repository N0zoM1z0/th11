# Screenshot capture source

Direct Ghidra12.1.3 review of the attested original Japanese TH11 v1.00a
identifies two authored-game functions. Both have complete contiguous bodies,
following CC padding, no embedded tables and no outside interior references.

| Source name | Target extent | Target bytes | Current linked candidate |
| --- | --- | ---: | --- |
| `CaptureScreenshot` | 0x00429CA0..0x00429EAB | 524 | 540 bytes; non-exact |
| `HandleScreenshotHotkey` | 0x00446880..0x0044691B | 156 | 156 bytes;23 unrelocated differences; non-exact |

These are maintained source mappings and receive no exact credit.

The separate229-byte `ScreenshotWriteThread` at0x00429BB0 now has a reviewed
complete entry and cold linked-PE exact proof, along with its shared-file
dependencies. See `SHARED_FILE.md`.

## Capture flow and storage

The capture entry receives a filename in EAX and moves it to ESI. It sleeps
for10 milliseconds while the previous writer's handle at0x004C3950 is nonzero.
It calls the release-disabled debug sink at0x0042B8D0 with `SnapShot! %s\n`,
then GetBackBuffer on the device at0x004C3288. It clears a14-byte bitmap file
header at0x004C3954, writes BMP signature0x4D42, size/offset54, and copies the
filename to0x004C396C.

The presentation format cell is0x004C337C. Format22 (X8R8G8B8) allocates a
44-byte BITMAPINFO and640*480*3 bytes of output. The BITMAPINFOHEADER describes
640x480, one plane,24 bits and BI_RGB. The locked source surface is traversed
from row479 down to0, respecting source pitch. Each of640 pixels copies its
first three bytes and skips the fourth. It then unlocks, starts the writer at
0x00429BB0 with `_beginthread`, and stores the returned handle.

Format23 (R5G6B5) logs the observed unsupported-format message. Allocation
failure logs its observed message; neither path starts a writer. The common
tail releases a non-null surface and returns0. The default format logs the
mother.cpp error and returns1 before the common release. Source preserves
these paths, unchecked API results and allocation ownership as observed.

`Screenshot.hpp` provides views into existing opaque MainRuntime storage,
without changing `MainRuntime.hpp` or inventing separate global owners:

| View/field | Root-relative offset | Target address |
| --- | ---: | --- |
| D3DPRESENT_PARAMETERS | 0xF4 | 0x004C3374 |
| Screenshot state/thread | 0x6D0 | 0x004C3950 |
| BITMAPFILEHEADER | 0x6D4 | 0x004C3954 |
| BITMAPINFO pointer | 0x6E4 | 0x004C3964 |
| Pixel buffer pointer | 0x6E8 | 0x004C3968 |
| Filename storage | 0x6EC | 0x004C396C |

The partial screenshot view includes260 filename bytes, ending before the
observed timer storage at0x004C3A70. Full native ownership and the original
declared filename capacity remain open. Japanese messages use their observed
CP932 bytes in string literals. They are not executable byte arrays.

## Hotkey flow

The helper first calls0x00445460, which visits four40-byte entries in the
animation manager and invalidates processed entries. Its full resource role
remains open; source names the operation `FlushScreenshotResources`.
It checks pressed bit0x40000 at0x004C92B4, creates `snapshot`, and scans
`snapshot/th%.3d.bmp` for indices0..999 with `FileExists`. The first unused
name is passed to capture. Exhausting all1000 names returns without capture.
Its local filename buffer is260 bytes and protected by the security cookie.

## Remaining codegen work

Pinned VC9 `/TP /O2 /Ob1 /Gy /MT /Isrc /GL` naturally produces filename-in-EAX
for capture when the actual log source is available. The first nested source
candidate was529 bytes. Matching the observed allocation-failure block and
countdown loops yields540 bytes with the correct conversion operations, but
EBP save placement, stack size, branch ordering and loop alignment still differ.
The actual hotkey source as caller leaves capture at540 bytes. Do not add
padding or change declared ABI to bridge this difference.

The hotkey candidate has the correct156-byte extent but differs in23
unrelocated bytes. Its candidate stack allocation is0x108 versus target0x110,
and ESI is saved inside the pressed-key branch rather than before the flush.
An independent repeated caller produces the same differences. Equal length
does not establish exactness.

The worker at0x00429BB0 was recovered from the initial inventory gap using
read-only Ghidra span decoding, then independently reviewed and added to the
ledgers. It now writes the observed header/info/pixel sequence, closes and
frees buffers through maintained exact source. Full native thread/file/resource
runtime closure remains open. Current capture/hotkey diagnostic families use the
actual `LogBuffer.cpp` as /GL support and `FileSystem.cpp` as normal-COFF
support; unchanged canonical FileExists was not replayed.

Private evidence: `.analysis/screenshot-rng-review.txt`,
`screenshot-dependencies-review.txt`, `frame-timing-review.txt`,
`screenshot-flow-linked.json`, `screenshot-caller-linked.json`,
`screenshot-hotkey-caller-linked.json`. Public compiler contexts remain under
`probes/screenshot-context.cpp` and `probes/screenshot-hotkey-context.cpp`.
