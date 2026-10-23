# Camera setup and batch submission

Direct Ghidra12.1.3 review covers three complete bodies in the attested original
Japanese TH11 v1.00a. Two have strict cold exact proofs totaling487 owned bytes.
All are contiguous, have reviewed entry/return paths, no embedded tables or
external interior references, and adjacent CC padding.

| Function | Complete target extent | Bytes | State |
| --- | --- | ---: | --- |
| `ApplyCamera` | 0x0042A810..0x0042A967 | 344 | Exact |
| `ApplyPerspectiveCamera` | 0x0042A970..0x0042AAD8 | 361 | Source present, non-exact |
| `FlushRenderBatch` | 0x0044FD10..0x0044FD9E | 143 | Exact |

Camera setup has four caller functions, including three frame paths and
camera selection at0x00405E80. That selector computes a camera address using
stride0x118, publishes it at runtime+0x54C and passes it in EDI. The batch
submitter has25 caller functions across ASCII, animation and frame rendering;
its receiver arrives in ESI. Ordinary stdcall source naturally generates
these private conventions under VC9 /GL. No register ABI declaration is used.
The shared game data layout and custom batching/camera orchestration support
authored-game origin independently of codegen equality.

## Camera setup

The routine flushes a non-null current renderer before changing transforms.
It converts unsigned viewport dimensions to float, derives half-width and
half-height, and computes width/height aspect. The eye is at the viewport
center with depth half-height divided by the angle helper's result for
0.15707963705062866f. The look-at point is the center at depth zero; up is
(0,-1,0). It calls the observed D3DXMatrixLookAtLH thunk, then the perspective
thunk with field of view0.3141592741012573f, near1 and far10000. Finally it
sets D3D view/projection transforms and, after reloading the renderer cell,
copies the camera's two offset floats into that renderer.

`CameraView` retains unknown storage before view+0x4C, projection+0x8C,
viewport+0xCC and offsets+0xE8/+0xEC. This partial view is not a complete
camera class; the selector's0x118 stride exceeds it. The float-return helper
at0x0042B6D0 takes its float on the stack and returns with RET4; its31-byte
wrapper was reviewed. The helper's implementation has no new source credit.
D3DX calls resolve through the observed0x0045F9D4/0x0045F9DA import thunks.

Keeping viewport conversions directly in the center/aspect expressions
allows VC9 to retain/reuse their x87 values and compute aspect before the
angle-helper call. Separate long-lived width/height locals delayed the
division and produced342 bytes with a0x3C frame. The final natural source
reproduces the target's0x38 frame and full344-byte control flow. No padding,
volatility, forced register declaration or executable bytes were introduced.

## Perspective camera

The361-byte routine flushes pending batches, constructs its eye and target
relative to the camera position, and uses the stored up vector for LookAtLH.
It sets a perspective projection with stored field of view, unsigned viewport
aspect, near30 and far1800, then publishes both D3D transforms. It computes
and normalizes the target-direction/up cross product into camera+0x30 and
propagates both renderer offsets. There are six caller functions.

The eye offset is+0x3C; target direction is+0x0C. The target is evaluated
before the eye but passed as LookAtLH's second vector. Matching complete
native argument/temporary stores resolved the initially reversed hypothesis.
`PerspectiveCameraView` separately describes these formerly opaque fields
without changing the already-proven screen-camera header.

The maintained vector-output helper uses a three-float temporary and aggregate
copy. It gives361 bytes; four unrelocated bytes at offsets288,289,291,292
remain different in one x87 multiplication with commuted operands. A scalar
helper gave353 bytes; aggregate initialization/field assignment both left
four differences, while a value-return helper left16. No masked, cropped or
commutativity-based exact credit is granted. All of these contexts are private
diagnostics; the current routine has no canonical exact unit.
Replaying its12 independently reviewed bindings over the full361-byte primary
contribution leaves those same four differences. Additional bindings include
30/1800 float cells at0x00497F9C/0x00497FA0 and the normalization thunk at
0x0045F9BC. This noncanonical diagnostic has no receipt acceptance authority.

## Batch submission

A zero quad count returns immediately. Otherwise the function sets stage0
ALPHAARG2 and COLORARG2 to DIFFUSE, selects FVF0x144, and calls DrawPrimitiveUP
with TRIANGLELIST, twice the current quad count, the batch-start pointer and
28-byte stride. It then increments the draw-call counter, moves batch start
to the next-vertex pointer and clears the quad count. D3D return values are
ignored as observed.

`RenderBatchView` describes only observed positions: draw-call count+0xAC,
camera offsets+0xB0/+0xB4, quad count+0x435620, vertex storage+0x435624,
next-vertex pointer+0x7B5624 and batch start+0x7B5628. The frame dispatcher
resets both pointers to the vertex-storage start. The vertex format is four
position floats, diffuse color and two UV floats. Unknown preceding storage
and the complete renderer owner remain open; this view is never instantiated
by the non-runnable proof family.

## Independent bindings and strict proof

The canonical manifest declares all19 fields:15 for camera setup and four
for batch submission. Global bindings are the renderer-pointer cell at
0x004C3268 and MainRuntime root0x004C3280 plus device offset8. The independently
read literal cells are2^32 as float at0x00497F98,0.5 as double at0x00497E60,
the helper angle at0x00497FAC, negative-one float at0x00497F20, far plane at
0x00497FA8 and field of view at0x00497FA4. Function bindings cover the batch
submitter, angle helper and the two D3DX thunks.

Two separate cold VC9 RTM families use `/TP /O2 /Ob1 /Gy /MT /Isrc /GL`,
each placing the accepted function in its primary source TU. The caller
context retains independent camera/batch inputs and both paths. Full
receipt-bound PE/MAP/PDB ownership, exhaustive binding replay and byte equality
are required separately for each function. A support-TU diagnostic alone has
no exact acceptance authority.

```bash
scripts/repo-python scripts/replay-exact-units.py \
  --unit apply-camera --unit flush-render-batch
```

Only these two new units are replayed; no previous canonical unit shares a
changed source/header. Existing MainRuntime and D3D headers remain intact.
Full frame dispatch, presentation/recovery, native renderer ownership and
complete-game runtime validation remain open.

Private evidence is in `.analysis/frame-loop-dependencies-review.txt`,
`frame-loop-dependencies.c`, `camera-caller-review.txt`,
`camera-math-bindings.txt`, `render-camera-*-linked.json` and
`render-camera-cold-replay.log`. The perspective review/full diagnostic is in
`camera-explicit-review.txt` and `perspective-camera-full-replay.json`.
Canonical families live in ignored
`build/match-linked/Camera` and `build/match-linked/RenderBatch`.
