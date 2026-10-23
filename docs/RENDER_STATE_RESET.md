# Device state and renderer texture recovery

Direct Ghidra review covers three complete contiguous bodies in the attested
original Japanese TH11 v1.00a. Returns, callers, all indirect D3D operations,
adjacent return/CC padding and zero external interior references were checked.
The778-byte state's initial250-instruction review was supplemented by an
explicit tail read; no trailing instructions were omitted.

| Function | Complete target extent | Bytes | State |
| --- | --- | ---: | --- |
| `PrepareRendererForReset` | 0x00445320..0x004453A6 | 135 | Source present, two byte differences |
| `RestoreRendererAfterReset` | 0x004453B0..0x00445451 | 162 | Exact |
| `ResetDeviceRenderStates` | 0x00447270..0x00447579 | 778 | Exact |

The two strict public primary-TU proofs add940 owned bytes and42 independent
bindings. Custom renderer file/cache layouts and runtime state integration
support authored-game classification separately from codegen equality.

## Thirty-three D3D state operations

The state reset reads the current device for every operation; it does not
cache one pointer across unknown COM calls. It ignores their HRESULTs.

| Render state group | Values in native order |
| --- | --- |
| Depth, lighting, culling | Z enabled; lighting off; cull none |
| Blend and shading | Alpha blend on; Gouraud; source alpha/inverse source alpha |
| Comparisons | Z always; alpha test on, reference1, greater-or-equal |
| Fog | Enabled; density1; table none; vertex linear; color0xFFA0A0A0; start1000, end5000 |
| Multisampling | Antialiasing off |

There are19 SetRenderState calls. Stage0's eight settings select MODULATE for
alpha/color, TEXTURE as arg1 and TFACTOR as arg2, two-coordinate texture
transformation and coordinate index0. Sampler0's six settings disable mip
filtering, use linear mag/min filtering, clamp W and wrap U/V.

Float-to-DWORD marshalling uses an actual4-byte temporary: FLD1 or a float
literal, FSTP into the slot, then integer load/push. The natural inline
MSVC/DirectX bit-view helper reproduces this complete sequence and the
PUSH/POP ECX frame. ECX's incoming value is never consumed as a parameter;
the initial Ghidra fastcall hypothesis is not the native call boundary.

After the final sampler call, the function reloads the renderer pointer and
conditionally resets its cache at+0x4355BC: DWORD0, flags byte+4 to7 and bytes
+5/+6/+8 to0xFF. Byte+7 and neighboring unknown bytes remain untouched. Source
keeps a local renderer pointer for this final group, matching the native
single reload. The cache view does not claim a complete original class.

## Default-pool texture release and recreation

Both loops retain the initial pointer to32 file slots at renderer+0x4350C0.
Null files and nonpositive signed texture counts are skipped. File count is
+0x10C; the20-byte texture-entry table is+0x120; entry flags are+0x10. These
additional physical views leave prior AnimationVm/MainRuntime headers intact.

Prepare tests flag1 and a non-null texture, calls Release, then reloads the
current file/table through the slot before clearing that indexed pointer.
It does not keep an old entry reference across a possible COM side effect.
The135-byte candidate differs only at SIB bytes76 and79, commuting the same
two registers. It gets no exact credit. An inline release method produced123;
direct index/pointer-add forms leave the two differences. Do not patch or
normalize those operand bytes.

Restore tests flag1, reobtains the entry for its inline recreation operation,
sets that flag, and calls CreateTexture with current presentation width,
height and format, one level, RENDERTARGET usage, DEFAULT pool and null shared
handle. It does not require a null old pointer or check HRESULT. After the
call it reloads presentation format and writes entry+0xC as4 for X8R8G8B8,
otherwise2, even on failure. Other entry fields are preserved.

Holding one entry reference for both condition and recreation cached its
flags and produced166 bytes. Separate condition/call accesses retain the
observed memory OR and register assignment, producing the complete162 bytes
including loop alignment. Both loops reread current file/count at iteration
end rather than freezing those values across callbacks. No extra null guards,
inert padding, fake returns or private register declarations were added.

## Independent fields and cold acceptance

State reset has36 DIR32 fields:33 current-device reads through MainRuntime
root0x004C3280 plus8, float1000 at0x00497F64, float5000 at0x00497F60 and
renderer-pointer cell0x004C3268. These values and method/state enums were
reviewed independently from target instructions and the pinned local SDK.

Recreation's six fields bind renderer0x004C3268 and MainRuntime members:
device+8, width+0xF4, height+0xF8 and format+0xFC before/after the call.
Separate cold families keep each accepted complete PDB contribution in its
own primary source TU. Current source/header/tool/producer and PE/MAP/PDB
receipts, exhaustive field replay and all940 owned bytes match exactly.

```bash
scripts/repo-python scripts/replay-exact-units.py \
  --unit reset-device-render-states --unit restore-renderer-after-reset
```

Only these two new canonical units are replayed. Previous29 canonical inputs
remain unchanged. Device/resource creation, renderer state and frame routines
now have maintained source, but full native ownership and game runtime gates
remain open.

A focused caller diagnostic adds these real reset bodies, runtime surfaces,
animation VM lifecycle and the frame-clock body to the maintained frame family.
RunFrame stays315; PresentFrame becomes233 rather than the opaque-dependency
family's224. A private float-elapsed/fast primary with precise supports also
gives233. Neither closes the native307/226 extents. Public frame source and
all previous canonical contexts remain unchanged; do not repeat these exact
full-dependency variants without new evidence.

Private evidence: `.analysis/render-state-reset-review.txt`,
`render-state-reset-tail.txt`, `render-state-linked.json`,
`renderer-texture-reset-review.txt`, `renderer-textures-linked.json` and
the two focused cold-replay logs. Caller results are in
`frame-loop-full-reset-*` and `frame-loop-full-float-fast-*`.
Compiler profile remains pinned VC9 RTM
`/TP /O2 /Ob1 /Gy /MT /Isrc /GL`.
