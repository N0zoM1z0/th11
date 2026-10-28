# Direct3D device initialization and presentation setup

Four functions now have complete maintained source for1,801 target bytes.
Direct Ghidra review covers every branch, call, return, reference and neighbor.
No function contains a table or external interior entry. The three strict
exact proofs add463 owned bytes and30 independently reviewed fields.

| Function | Complete target extent | Bytes | State |
| --- | --- | ---: | --- |
| `InitializeD3DDevice` | 0x00446D30..0x00447269 | 1,338 | Source present; non-exact |
| `PrimeDeviceBackBuffers` | 0x00447E00..0x00447E9C | 157 | Exact |
| `ResetMainViewport` | 0x00447EA0..0x00447F02 | 99 | Exact |
| `AppendErrorLog` | 0x00458AF0..0x00458BBE | 207 | Exact |

The initializer's default Ghidra body has1,335 addresses in two ranges. A
separate read-only full-span decode covers all1,338 bytes, including the
three-byte LEA loop alignment at0x00446F2D. Its initial250-instruction output
was supplemented by the complete tail. Extents were not cropped to candidates.
Game-specific runtime/configuration, log and renderer integration support
authored-game classification independently of codegen equality.

## Physical runtime fields

The new view extends independently observed fields at MainRuntime0x004C3280;
it does not establish a complete original class or data-owner size.

| Root offset | Observation |
| --- | --- |
| 0x04 / 0x08 | IDirect3D9 / IDirect3DDevice9 pointers |
| 0x5C / 0x9C | View / projection matrices |
| 0xDC / 0xF4 | Viewport /56-byte presentation parameters |
| 0x19C | Saved16-byte adapter display mode |
| 0x1E2 / 0x1E5 / 0x1EB | Color setting / display mode / timing option |
| 0x200 | Compatibility options |
| 0x57C | Disable-refresh-change flag |
| 0x580 / 0x584 | Refresh-change status / final reset cell |
| 0x590 / 0x59C | Runtime flags / D3DCAPS9 |
| 0xB08 / 0xB0C | Main HWND / final reset cell |
| 0xB30 / 0xB40 | Startup path mismatch / presentation option flags |

Native SDK method slots were checked independently: adapter display mode0x20,
device creation0x40 and format check0x28 on IDirect3D9; capabilities0x1C,
Reset0x40, Present0x44, Clear0xAC, SetTransform0xB0 and viewport0xBC/0xC0 on
the device. SDK sizes, enums, cap masks and target log/float data were reviewed.

## Presentation parameters and creation fallback

Initialization clears a local D3DPRESENT_PARAMETERS, queries adapter0's display
mode and publishes all16 bytes. Query failure receives no added guard. A
nonzero display setting with a refresh rate other than60 logs that fact and
clears presentation option0x10, then the display setting is read again.

For fullscreen setting0, compatibility mask1 forces R5G6B5 and color setting1.
Otherwise color0xFF selects X8R8G8B8, becomes0 and logs first initialization;
other zero/nonzero settings choose X8R8G8B8/R5G6B5. A startup mismatch enables
the disable-refresh-change flag. Without that flag the function requests60 Hz;
with it the requested rate is0. Disabling refresh changes does **not** set
Windowed. Interval is immediate for option0x10 or timing3, otherwise one; the
disabled-refresh path always uses immediate.

For nonzero display settings it uses the adapter format, sets Windowed and
selects immediate for option0x10, timing3 or a non-60-Hz desktop. The requested
back buffer remains640x480 even when the window created earlier is larger.
Common parameters select DISCARD, automatic D16 depth and a lockable back
buffer. Other fields, including initial count and device-window member, retain
their zero initialization. Runtime mask2 and refresh-change status1 are set.

Each creation pass tries the following in order:

1. HAL with hardware vertex processing0x40.
2. HAL with software vertex processing0x20.
3. REF with software vertex processing0x20.

Compatibility mask2 skips both HAL attempts. Nonnegative HRESULT succeeds.
Runtime mask1 is set only by hardware-vertex HAL success and cleared by the
other two success paths. REF also disables the later alpha-texture format test.

After all attempts fail, an enabled refresh change logs the failure, requests
rate0, clears refresh-change status and repeats. The disabling flag is not
changed by this branch. Persistent failure can therefore keep retrying that
path; the maintained source retains it rather than adding an artificial bound.
Once refresh changes are disabled, interval-one failure retries at immediate.
Further failure appends an error, conditionally releases/nulls IDirect3D9 and
returns1. HAL failure messages are emitted only after the refresh-retry marker.

## Camera and capability setup

Success copies the complete, possibly API-modified presentation structure to
runtime storage. Eye is(320,-240,-distance), target(320,-240,0), up(0,1,0), where
`distance` is a float assignment of `240.0 / CameraTangent(0.2617993950843811f)`.
The separate scalar assignment retains the observed float store/reload before
negation. An aggregate expression allowed an earlier sign fold in VC9 codegen.

The projection uses field of view0.5235987901687622f, aspect4/3, near100 and
far10000. It publishes VIEW/PROJECTION transforms, queries viewport and caps,
and logs absent D3DTEXOPCAPS_ADD or unsigned maximum texture width<=256.
These query results are used without added HRESULT checks.

Unless compatibility mask1 or REF suppressed it, CheckDeviceFormat tests
HAL/default-adapter support for A8R8G8B8 textures against the current back-buffer
format. Only exact D3D_OK sets runtime mask4. Any other result clears it, sets
compatibility mask1 and logs reduced color. Skipping this test leaves that mask
as it was. Device states are reset, the main viewport/buffers are initialized,
the two final reset cells are cleared, and success returns0.

CameraTangent0x0042B6D0 was independently reviewed as a31-byte stack-float/RET4
wrapper with a float store/reload on return. The decompiler's fastcall label
does not describe its actual input. This dependency has no new source mapping
or byte credit. D3DX matrix calls use the reviewed target import thunks.

The maintained initializer is a complete1,350-byte PDB contribution versus
1,338 native bytes, with845 unequal unrelocated diagnostic positions. The
earlier aggregate-eye form gave1,302. Frame alignment, allocation/scheduling
and original vector/TU context remain open; neither candidate is exact.

## Viewport and both back buffers

ResetMainViewport conditionally flushes the renderer, sets viewport
(X0,Y0,width640,height480,MinZ0,MaxZ1), calls SetViewport and tail-jumps to
PrimeDeviceBackBuffers. Its99-byte exact proof includes that complete jump.
An independent unknown batch caller in the non-runnable context retains the
observed ESI receiver for FlushRenderBatch. A global-only context specialized
that receiver away and gave96 bytes; it has no exact credit.

Priming performs two full Clear/Present cycles. Each clears target plus depth
to opaque black, depth1 and stencil0, presents with four null arguments and
resets using the saved presentation parameters if Present returns negative.
Clear/Reset results are ignored, and a failure in the first cycle does not
remove the second cycle. Two genuine inline operations reproduce all157 bytes.
A counted loop emitted91 and did not match. No extra executable padding or
target-byte injection was used.

## Error logging

AppendErrorLog shares the8192-byte log area/end pointer and adds a one-byte
error flag at+0x2004. Its observed view does not assert original owner size.
It formats variadic input into512 bytes, uses the native strict
`end + length < text + 8191` append test and sets the error flag even when the
append is skipped. It conditionally locks/depth-counts runtime index3 with
separate pre/post flag tests, and returns the original format pointer.
The native private receiver is EDI, with format/variadic words on the stack.
Every one of the207 owned bytes matches.

## Strict cold proofs

The locked VC9 RTM profile is `/TP /O2 /Ob1 /Gy /MT /Isrc /GL`. Three separate
cold families make each accepted function a complete public primary-TU PDB
contribution, with actual required sibling bodies and a non-runnable context.
Matching code emitted from support TUs alone receives no acceptance.

Error logging has11 fields: security cookie, six runtime flag/lock/depth member
accesses, two IATs, vsprintf and cookie-check calls. Viewport has11: renderer
cell/flush, eight runtime member references and the priming tail call. Priming
has eight runtime device/presentation references. All30 bindings were reviewed
against native instructions/SDK/import identities before strict replay.
Current source/include/profile/tool/producer and PE/MAP/PDB receipts, owner
validation, complete extents and every byte pass with zero differences.

```bash
scripts/repo-python scripts/replay-exact-units.py \
  --unit append-error-log --unit reset-main-viewport \
  --unit prime-device-back-buffers
```

Only these three new units were cold-replayed. Previous35 canonical inputs
are unchanged and not replayed; directly required bodies are compiled as new
family support, which grants them no new proof or credit.
