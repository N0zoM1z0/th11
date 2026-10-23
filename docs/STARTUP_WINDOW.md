# Startup paths and main window creation

Three useful startup functions now have complete maintained source covering
1,195 target bytes. Direct Ghidra review checked every native branch, return,
call, reference and neighboring RET/CC boundary. All three bodies are
contiguous, have no embedded tables and have zero external interior entries.

| Function | Complete target extent | Target bytes | Current candidate | Exact |
| --- | --- | ---: | ---: | --- |
| `InitializeMainWindow` | 0x00446AE0..0x00446D20 | 577 | 569 | No |
| `CheckStartupPath` | 0x00447580..0x004476D7 | 344 | 353 | No |
| `ReadShortcutTarget` | 0x004477F0..0x00447901 | 274 | 243 | No |

No new canonical exact unit or byte credit is declared. Previous35 canonical
source/header inputs are unchanged and their units were not replayed.

## Startup title and path handling

The initial void decompiler hypothesis omitted an actual EAX result.
The native caller at0x004455CC tests EAX against-1 immediately after the call.
CheckStartupPath returns0 when an opaque integer at0x004C9898 is nonzero,
otherwise-1. That cell is written earlier by startup at0x004455A3; its broader
meaning remains open. The path mismatch flag does not determine this result.

The function initializes STARTUPINFOA's size and zeroes its remaining64 bytes,
queries its module filename and console title into separate261-byte buffers,
then obtains startup info. A null startup title sets MainRuntime flags mask0x40.
A non-null title clears that mask after processing, even if the path is absent
or lacks an extension.

Processing first finds the title's last dot, then calls FileExists even when
that dot is absent. With a readable path and an extension, `.lnk` comparison
is case insensitive. A shortcut reads its target into the title buffer and
repeats while that output still has a `.lnk` suffix. Each iteration continues
to load the **original startup filename**; it does not replace the input with
the previous output. The shortcut reader's return value is ignored. Other
extensions copy the startup title into the buffer. A case-sensitive difference
from the module filename sets the byte at MainRuntime+0xB30(0x004C3DB0) to1.
No additional suffix, copy-length or shortcut-chain checks were inserted.

The module path and console query return values are ignored. The opaque return
gate and missing-title flag remain independent from file existence and mismatch.
The complete353-byte candidate has265 unequal unrelocated positions against
the344-byte target; calling context, frame alignment and branch placement remain
open. This is a whole-span diagnostic, not a percentage or partial exact claim.

## Shell-link reader

Target instructions and the real caller independently show destination in EDI,
filename on the stack, an integer boolean in EAX and RET4. The nominal source
is a natural two-input stdcall function. Its current linked private convention
instead keeps filename in EBX and destination in EDI with RET: that difference
remains explicitly unresolved.

A null destination returns0 without COM calls. Otherwise the function ignores
CoInitialize's result, creates CLSID_ShellLink in-process as IShellLinkA, and
queries IPersistFile. It allocates260 WCHARs, converts the source filename from
CP_ACP with length-1, loads it in STGM_READ mode, then calls GetPath with output
capacity260, WIN32_FIND_DATAA storage and flags0. No IShellLink::Resolve call
occurs. Conversion's result is ignored; SUCCEEDED comparisons accept nonnegative
HRESULTs, including S_FALSE.

Success becomes1 only after nonnegative GetPath. After a successful query,
the wide array is deleted and IPersistFile is released on either Load/GetPath
outcome. IShellLinkA is released after any successful creation. CoUninitialize
is called for every non-null-destination path, even if initialization or object
creation failed. Failure does not add an output-buffer clear.

The three native16-byte identities were read independently and checked against
the pinned SDK declarations:

| Identity | Target address | GUID |
| --- | --- | --- |
| CLSID_ShellLink | 0x0048FCCC | 00021401-0000-0000-C000-000000000046 |
| IID_IShellLinkA | 0x0048E0AC | 000214EE-0000-0000-C000-000000000046 |
| IID_IPersistFile | 0x0049013C | 0000010B-0000-0000-C000-000000000046 |

SDK slots identify QueryInterface0, Release8, GetPath0xC and Load0x14.
The native allocating routine at0x0045FCE4 retries malloc/new-handler and throws
bad_alloc on failure; deletion0x0045FD49 forwards to free. These are dependency
observations, not new source mappings or exact credit. Allocation/exception
ownership remains open. The native checked260*2 array-size sequence is absent
from the current243-byte candidate, which has207 unequal unrelocated positions.
Generic shell-link loading may be reused sample/library code, so its origin
remains indeterminate despite the game-specific caller.

Early-return source gave264 bytes; a shared return gives243. Reversing nominal
input order, adding `<new>` and an /EHsc family did not close the physical ABI
or checked-size differences. Do not repeat these contexts without new evidence.

## Main window creation

The native caller at0x00445733 loads the HINSTANCE argument into EBX immediately
before calling0x00446AE0. The maintained explicit memset form now reproduces
that EBX input and zero-byte stack cleanup, while the complete function remains
non-exact. Aggregate WNDCLASSA initialization kept an unwanted stack input;
free/member/fastcall contexts did not solve that form. No dummy input, forced
register declaration, padding or byte patch was added.

Creation zeroes WNDCLASSA and uses BLACK_BRUSH, the arrow cursor, supplied
instance, the existing MainWindowProc callback and class name `BASE`.
It sets activation/inactivity to1/0, then registers the class; registration
and brush/cursor return values are not given added failure checks.

Additional observed fields share MainRuntime root0x004C3280:

| Root offset | Observation |
| --- | --- |
| 0x10 | GetWindowRect destination |
| 0x58 | Second published window handle; consumer/owner remains open |
| 0x114 | Windowed flag |
| 0x1E5 / 0x1E6 / 0x1EB | Display mode / frame skip / timing option |
| 0x1F0 / 0x1F4 | Window X / Y |
| 0xB08 | Main HWND |
| 0xB14 / 0xB18 | Activation / inactivity |
| 0xB40 | Mode and presentation flags |
| 0xB80 / 0xB84 | Reset cell / four opaque three-DWORD records |

The view does not assert a complete original class. The timing byte is tested
against2 here; the earlier frame source's `waitForPresent` is a provisional
name for the same physical byte, not a proved boolean type or complete meaning.

Display mode replaces flags bits2:3. Any nonzero result selects windowed mode.
Flag0x10 is enabled only when frame skip is zero and timing option equals2.
Opaque records are reset to(15,15,0),(12,12,0),(12,12,0),(8,8,0), followed by
clearing the preceding reset cell. Other flags/unknown fields are preserved.

Fullscreen uses WS_POPUP|WS_VISIBLE at(0,0),640x480. Windowed mode uses
WS_VISIBLE|WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX and configured X/Y.
Base dimensions are1280x960 for mode3,960x720 for mode2 and640x480 otherwise.
It adds twice SM_CXFIXEDFRAME/SM_CYFIXEDFRAME and one SM_CYCAPTION to height.
These exact styles and metric calls are retained rather than replaced with
a conventional window-sizing helper.

After CreateWindowExA, GetWindowRect is called and both HWND cells are published
even when the returned handle is null. Null returns1. Success sends SC_MINIMIZE,
sleeps16 ms, sends SC_RESTORE and returns0. Native title bytes are CP932 data;
their data-owner definitions remain open.

Explicit memset fixes the observed input convention and48-byte frame, but the
569-byte contribution still has383 unequal unrelocated positions versus577.
Zero-register lifetime, save/restore placement and instruction scheduling remain
open. The correct black-brush constant, explicit record writes and fullscreen
branch order are retained. No canonical context is declared for this mismatch.

## Bounded machine-code verification

A private diagnostic executed complete target and compiler-produced bodies in
Unicorn with explicit Win32, COM and CRT models. It checked51 distinct scenarios:
nine direct shortcut cases, eighteen startup cases and twenty-four window cases.
These cover null destination, negative COM results, ignored initialization/
conversion failures, nonnegative S_FALSE, title/no-title/extension/existence
branches, repeated original-source shortcut loading, three return-gate values,
four modes, timing/frame-skip combinations and window creation success/failure.

Returns, API order/arguments, allocation size, release/delete paths, destination
strings, runtime state, callee-saved registers and each observed stack convention
agree. The shortcut inputs were marshalled separately for the two different
physical ABIs. Actual target GUID/class/title data were supplied to candidate
data anchors; target function instructions were not patched. The window subset
was refreshed after the final field-name cleanup.

The models cover ASCII path conversion and successful allocation; they do not
exercise real OLE, filesystems, locale conversion, windows or allocator throws.
This is bounded diagnostic evidence with **no exact, native-product or full
semantic acceptance authority**. Full production build/runtime and later gates
remain open. Ghidra/Wine retain their serialized resource limits; the diagnostic
uses one CPU at nice10 and measured about88 MiB peak RSS for the full case set.

Private evidence: `.analysis/font-startup-review.txt`, `startup-dependencies-review.txt`,
`startup-caller-return.txt`, `startup-return-gate-refs.txt`, `window-setup-review.txt`,
`window-setup-caller.txt`, `startup-path-*-linked.json`,
`window-initialization-*-linked.json`, `check-startup-window.py`,
`startup-window-differential.json` and `window-differential.json`.
