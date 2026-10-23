# Main window callback and surface buffer transfers

Direct Ghidra review covers three complete contiguous functions in the
attested original Japanese TH11 v1.00a. Every branch, return, indirect call,
neighboring RET/CC boundary and incoming/interior reference was reviewed.
None contains a table, sparse tail or external interior reference.

| Function | Complete target extent | Bytes | State |
| --- | --- | ---: | --- |
| `SurfaceBufferView::ReadFromSurface` | 0x00444010..0x004440D8 | 201 | Source present; non-exact |
| `SurfaceBufferView::WriteToSurface` | 0x004440E0..0x004441B8 | 217 | Source present; non-exact |
| `MainWindowProc` | 0x00445E00..0x00445FA1 | 418 | Exact |

## Registered window callback

The window creation routine installs address0x00445E00 in WNDCLASSA's
`lpfnWndProc` at0x00446B36 before RegisterClassA. This independently identifies
the four-stack-argument stdcall callback; all exits use RET16. Registration
also initializes the activation cells to1/0. Its game-specific shared runtime
updates support authored-game classification independently of exactness.

| Message | Observed behavior |
| --- | --- |
| WM_CLOSE | Clear runtime flags bit0x100, set0x80, return1. |
| WM_ERASEBKGND | Return1. |
| WM_SIZE | If resize flags have mask1 set and wParam is SIZE_MAXIMIZED, clear bits0xC and set2; then use the default window procedure. |
| WM_ACTIVATEAPP | Store wParam and its zero predicate in the activation cells, then use the default procedure. |
| WM_SETCURSOR | Windowed: load/set arrow and call ShowCursor(TRUE) once. Inactive fullscreen: show arrow until the count is nonnegative. Active fullscreen: hide until the count is negative, then clear cursor. Return1. |
| WM_SYSCOMMAND | Return1 for masked SC_MOUSEMENU/SC_KEYMENU; default handling otherwise. |
| WM_LBUTTONDOWN | Set the foreground window, then use the default procedure. |

Other messages use DefWindowProcA. Preserve its actual return value. The
callback does not post a quit message, resize buffers or clamp cursor counts.

The additional physical runtime view retains unknown storage. MainRuntime
root0x004C3280 supplies Windowed+0x114, flags+0x590, activation+0xB14,
inactivity+0xB18 and resize flags+0xB40. Windowed agrees with the established
D3DPRESENT_PARAMETERS view at+0xF4. Activation0x004C3D94 is also the existing
keyboard polling gate. These observations do not establish a complete owner
size or the full meaning of every runtime flag.

Shared default handling after the outer switch reproduces the native
argument preservation and four default-procedure call sites. Nested switches
for size/system-command values and a do/while show loop reproduce the
remaining native decisions and alignment. Explicit per-case default returns
gave400 bytes and tail jumps; shared handling with an equality test/while
gave418 bytes but192 unrelocated differences. These bounded alternatives are
recorded; no code bytes, inert padding or false ABI were introduced.

## Independent binding and exact proof

The complete418-byte public primary-TU PDB contribution has21 DIR32 fields:
eight runtime-member references and thirteen import references. Runtime
addends are+0x114, +0x590 twice, +0xB14, +0xB18 twice and+0xB40 twice.

| Import | Native IAT cell | References |
| --- | --- | ---: |
| DefWindowProcA | 0x0048B260 | 4 |
| SetForegroundWindow | 0x0048B234 | 1 |
| LoadCursorA | 0x0048B204 | 2 |
| SetCursor | 0x0048B22C | 3 |
| ShowCursor | 0x0048B240 | 3 |

IAT identities were independently read from the target import directory;
message/size/system-command values and callback types were checked against
the pinned local Windows SDK. Cold compilation uses VC9 RTM
`/TP /O2 /Ob1 /Gy /MT /Isrc /GL` and an address-escaping WNDCLASSA context.
Current source/header/tool/producer and PE/MAP/PDB receipts, complete owner
validation, exhaustive field replay and every owned byte pass the strict
linked Oracle with zero differences.

```bash
scripts/repo-python scripts/replay-exact-units.py --unit main-window-proc
```

Only this new canonical unit is cold-built. Previous31 canonical source/header
inputs are untouched and their units are not replayed. The first comparison
correctly rejected missing source/boundary ledger ownership; after entering
the reviewed mapping, the same fresh artifact receipt passes strict replay.

## CPU buffer and D3D surface transfers

Both functions receive the owner in ESI and a surface pointer on the stack,
return a boolean in AL and remove four stack bytes. Natural member source
produces that linked private convention in the declared diagnostic context.
No incoming references are currently known to Ghidra; the original class,
production calling context and game/library origin remain indeterminate.

The common physical fields are format+0x100, width+0x104, signed height+0x108,
row byte count+0x110, buffer-presence field+0x11C and pixel pointer+0x120.
The adjacent657-byte compositor independently reads the same layout and
surface methods; it is reviewed evidence, not maintained source or credit.
Unknown storage and fields between these observations remain intact.

Both reject a zero presence field, call GetDesc, then construct a full-image
RECT from the current width/height and lock it. SDK IDirect3DSurface9 slots
0x30/0x34/0x38 identify GetDesc/LockRect/UnlockRect independently.

Read locks with D3DLOCK_READONLY and ignores the HRESULT. It captures row bytes,
buffer pointer, locked pointer and pitch, then copies `height` rows only when
surface format equals the stored format. It advances the surface pointer by
pitch and the CPU pointer by row bytes. It always unlocks and returns true
after that path, even on format mismatch or nonpositive height.

Write locks with flags0 and continues only for exact D3D_OK, rather than a
general SUCCEEDED check. A nonzero result returns false without unlocking.
On zero, the same format/height gate copies CPU rows to the locked surface,
with the two pointer strides reversed. It unlocks and returns true even if
the format gate skips copying. GetDesc/UnlockRect return values are ignored.
Do not silently strengthen these native failure paths.

The maintained early-guard member candidates are202/205 bytes versus201/217,
with186/199 unrelocated differences; register-save placement, stack allocation
and write-path register selection remain open. Explicit free stdcall inputs
and /EHsc retain those same results. A direct-pitch version gives201/201 but
removes the observed cached pitch; a nested-success flow gives212/212. None
is exact. Avoid repeating these contexts without new target evidence.

Private evidence is retained in `.analysis/window-texture-transfer-review.txt`,
`window-registration-context.txt`, `main-window-*-linked.json`,
`main-window-cold-replay.log`, and the `surface-buffer-*` probe families.
Full native game build/runtime, semantic reconstruction and portability gates
remain open.
