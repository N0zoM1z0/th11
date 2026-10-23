# Frame dispatch, presentation and direct runtime helpers

Direct Ghidra review covers four complete contiguous bodies in the attested
original Japanese TH11 v1.00a. Returns, incoming/callee references and adjacent
return/CC padding were reviewed; there are no tables or external interior
references. Default decompiler register inputs were reconciled with native
instructions and actual callers.

| Function | Complete target extent | Bytes | Current source candidate |
| --- | --- | ---: | --- |
| `RunFrame` | 0x00446650..0x00446782 | 307 | 315, non-exact |
| `PresentFrame` | 0x00446790..0x00446871 | 226 | 224, non-exact |
| `DisableRuntimeFog` | 0x0042A300..0x0042A335 | 54 | 54, exact |
| `StopRuntimeThread` | 0x00459430..0x00459494 | 101 | 101, exact |

The first three routines coordinate custom game camera, renderer, callback
and runtime state, supporting authored-game origin. Thread-stop origin remains
indeterminate: a generic Win32 stop/wait wrapper plus its state layout does
not independently distinguish authored from bundled support code. Origin
classification and exact codegen are separate claims.

## Update and draw dispatch

`RunFrame` takes one stack argument and returns with RET4. It flushes pending
rendering, selects camera1, applies its transforms/viewport, and runs the
update chain. Result0 stops the runtime worker and returns1; result-1 stops
the worker and returns2. Other results advance the signed-byte skip counter.
Drawing occurs when that counter reaches unsigned skip-setting+1.

A draw opens a scene, clears the batch count and both vertex cursors, forces
the fog cache to0xFF, disables fog, executes draw callbacks and flushes the
batch. It unbinds texture0, ends the scene, clears the skip counter and presents.
The continuing path publishes current clock minus frame-start time; the two
early exits do not. BeginScene/EndScene and texture-stage HRESULTs are ignored
as observed. No null-renderer fallback or counter-saturation rule was added.

The native update entry receives its manager in EBX; the currently maintained
member implementation instead chooses a stack argument in this generic /GL
family. This affects the caller's register save/zero reuse and leaves315
bytes versus307. A global-only family specializes the manager and gives308;
the conservative independent-manager context gives315 and a312-byte update
callee. This repeats the previously known update-context limitation; do not
force a private register ABI or repeat those caller-only probes.

## Presentation

The native receiver is in ESI. The source uses an ordinary nonvirtual
`FrameLoopView::PresentFrame` member; it does not declare a private register
ABI. It records frame-start time and, only when wait flag equals1, checks a
positive elapsed interval below1/60 second. A positive integer result from
`(lastPresent + 1/60 - frameStart) * 1000 - 3.5` is passed to Sleep.

It records time before presenting, handles the screenshot hotkey and calls
D3D Present with four null arguments. Every negative HRESULT triggers the
observed release/prepare/Reset/restore/surface/render-state sequence and
assigns presentation state2. Reset's HRESULT is ignored. It then records the
last-present time and invokes each of two guarded global post-present hooks.
Those callback names are call-boundary labels; their complete roles and
implementations remain open.

The timing literals were independently read: double1/60 at0x00497F68,
1000 at0x00497EA8 and3.5 at0x004983F8. The integer conversion is emitted by
the compiler and calls target0x004864E0. Callee entry review confirms zero
explicit inputs for the reset/post-present hooks, except the runtime surface
helper at0x0042B530, which takes a stack receiver. The decompiler's apparent
incoming ECX to0x00447270 is not a native input.

The current field-based source produces224 bytes without the target's
PUSH/POP ECX temporary frame;126 unrelocated diagnostic bytes differ after
that shift. Double time locals produced233 bytes with8-byte realignment;
a float elapsed temporary produced235 with rounding stores absent from the
target. `/fp:fast` removed those stores but returned224. A member declaration
also leaves224. Final source keeps the ordinary precise profile and double
state fields. No dummy local, inert stack reservation or executable bytes
were introduced to force the missing frame.

## Runtime views and helper behavior

`FrameRuntimeView` overlays additional observed fields at MainRuntime root
0x004C3280. The original `MainRuntime.hpp` stays unchanged. These are partial
physical views, not a recovered complete owner size or native allocation.

| MainRuntime offset | Observed field |
| --- | --- |
| 0xF4 | D3D presentation parameters |
| 0x1E6 / 0x1EB | unsigned skip setting / wait flag |
| 0x31C | camera1, consistent with base0x204 and stride0x118 |
| 0x54C / 0x550 | active camera pointer / index |
| 0x574 | presentation state |
| 0x7F0 | worker view |
| 0x990 | fog cache, root address0x004C3C10 |
| 0x9B8 | elapsed-frame double |

Frame state has a signed counter+0x14 and doubles at+0x40/+0x60/+0x68.
Compile-time offset checks cover the views.

Fog disable returns0 without a call for zero cache. Otherwise it flushes the
current renderer, clears the cache and returns SetRenderState(FOGENABLE,0).
The natural linked convention puts its receiver in EDI.

Thread stop skips a null handle. Otherwise it writes stop-request1/pending0,
waits200ms, and on WAIT_TIMEOUT repeats those writes, sleeps1ms and waits
again. Any other wait result closes the handle and clears handle+4 and the
unknown DWORD+0x18. Other fields are left untouched. The receiver naturally
arrives in ESI; no volatility or extra synchronization was invented.

## Focused strict reproduction

One cold family puts both helpers in their real primary source TU,
`FrameRuntime.cpp`, with complete separate public PDB contributions. All155 owned bytes replay with zero differences. All five
independently reviewed bindings are declared: renderer cell0x004C3268,
batch submitter0x0044FD10 and WaitForSingleObject/Sleep/CloseHandle IAT cells
0x0048B0F0/0x0048B080/0x0048B0A0. The pinned VC9 RTM profile is
`/TP /O2 /Ob1 /Gy /MT /Isrc /GL`; receipt-bound full replay is required.

```bash
scripts/repo-python scripts/replay-exact-units.py \
  --unit disable-runtime-fog --unit stop-runtime-thread
```

Only these two new units are replayed, sharing one cold family. Previous25
canonical units' inputs remain unchanged. Their sources used as direct
support do not receive new credit or replay those canonical proofs.

The surface initializer/release and animation reset/binder now have maintained
source; reset303 and release80 have exact proofs. See
[`RUNTIME_SURFACES.md`](RUNTIME_SURFACES.md) for their current state.
Device-state reset778 and renderer texture recreation162 now have exact
proofs; preparation135 remains two bytes different. Full-dependency caller
diagnostics remain non-exact. See [`RENDER_STATE_RESET.md`](RENDER_STATE_RESET.md).

Private evidence: `.analysis/frame-timing-review.txt`, `frame-timing.c`,
`frame-loop-dependencies-review.txt`, `frame-presentation-callee-entries.txt`,
`frame-real-caller-entry.txt`, `frame-loop-*-linked.json` and
`frame-runtime-cold-replay.log`. Native frame/device/resource ownership and
whole-game runtime closure remain open.
