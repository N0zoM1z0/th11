# Archived reconstruction handoff

Historical checkpoint notes before the VM/stage resource handoff cleanup.
Counts and next-target suggestions below describe that earlier checkpoint.
Use [the current handoff](../RE_HANDOFF.md) and generated progress for current state.

---

# Resume brief

Target: original Japanese TH11 v1.00a, SHA-256 in `config/target.toml`.
Use direct Ghidra only. Read `AGENTS.md` and inspect dirty work before changes.

## Current checkpoint

Inventory:1,989 provisional candidates; seventy-one reviewed/source-present.
Forty canonical exact functions total8,894 owned bytes:

- Eight normal-COFF units: ASCII update22; AddAngles157; NormalizeAngle145;
  repeat state105; keyboard polling888; file check141; draw chain284 including
  table; callback removal114.
- Thirty-two linked-PE units: joystick button capture267, ordered update/draw
  registration147 each, ASCII string appender182, frame clock260, random
  generation105/153, log append211, screenshot writer229, shared file open
  modes180 each, write helper88, resource load466, complete-file write297,
  archive lookup53, camera344, batch143, fog54, runtime thread stop101,
  animation VM reset303, surface release80, renderer texture restore162,
  device-state reset778, main window callback418, text surface release105/
  creation396, text/font initialization346, error log207, main viewport99
  back-buffer priming157 and formatted text156/right alignment224.

This checkpoint adds direct formatted text156 and right alignment224 as strict
cold primary-TU exact proofs:380 owned bytes and10 independently reviewed fields.
Centered formatting now has full212-byte source, including the36-byte tail
missing from its176-byte initial candidate; it remains non-exact with13 stack
offset differences from a4-byte frame discrepancy. Read FORMATTED_TEXTURE_TEXT.md.
Only the two new exact units were cold-built/compared; previous38 inputs and
canonical proofs are unchanged. The source file is shared by all three: future
centered edits require focused revalidation of these two units, not the cohort.

The observed132-byte character region and default17-then-byte-height conditional
close direct/right codegen. Initial128-byte and conditional-expression forms
failed; do not repeat them. Centered native frame0x8C/source0x88 stays open;
do not add dummy locals or padding. Raw zero height forwards0; aligned routines
default17 before computing x. Right x truncates after floating subtraction;
centered x truncates width then uses signed /2 before unsigned span subtraction.
All three keep plain bit+408 and post-dispatch OR1 at+404.

Private123-case formatted-wrapper diagnostic agrees on varargs, output through
131 bytes, physical dispatch parameters, finite x87 values, VM state, GS cookie,
registers and stack. It models printf/FTOL/dispatch and grants no full native/
runtime authority. One CPU/nice10, max RSS106,712 KiB. Raw/right proof logs are
`.analysis/formatted-text-proof-raw.log` and `formatted-text-proof-right.log`;
the initial cold runner reached comparison before source-owner rows were
registered, then both comparisons passed against that same current cold family
after owner reconciliation. No source recompilation or old proof replay was
needed for that tracking correction. All three origins remain indeterminate.

The preceding checkpoint added outlined texture text631, DIB outlined text525 and the
real placement dispatcher172:1,328 native bytes of new maintained source.
They remain non-exact637/530/173, with520/466/130 unequal diagnostic positions.
Read `OUTLINED_TEXT.md`. Exact totals remain38/8,514; no previous exact source
or header changed, and none of the previous38 units was replayed.

The cold family includes actual directly needed plain text and pixel-filter
bodies. Its123 bounded machine-code cases agree on pixels, views, API calls,
rectangles, cleanup, AL, registers and stack; an independent rectangle reference
checks all surface outline passes. GDI/D3DX and finite FTOL conversion are
modeled; no native/full semantic or exact authority. One CPU/nice10, max RSS
117,400 KiB (about115 MiB). No further rerun is needed for unchanged inputs.

Surface outline has six calls including duplicate second/fourth rectangles,
then independent foreground; CLR_INVALID skips only the six outline calls.
Texture outline has no such sentinel: zero spacing draws the whole byte string
five times, otherwise fixed two-byte chunks including a trailing NUL for odd
length. Empty/nonzero-spacing text draws no chunks; negative spacing remains.
Keep both alpha passes and actual filter bodies, right-only clipping, ignored
HRESULTs and uninitialized output behavior.

Placement floats are+0C/+10/+14/+18. Nonpositive height becomes17; positive1..8
skips all conversion/drawing;9..16 reaches the path's separate minimum17 clamp.
Selector zero means outline, nonzero plain. Its three formatting callers now
have source as described above.
Surface foreground addition-order changes and swapped text/font declaration
order did not close codegen; Os primary gives425/485 and is also non-exact.
Do not repeat those contexts without new evidence. Native631 uses three input
registers/RET24, source637 only two/RET28; original frame/type/TU context is open.
All three origins remain independently indeterminate.

The preceding checkpoint added complete compositor657, text drawing392, neighbor-color
fill872 and required alpha inversion165 sources:2,086 native bytes across four
functions. They remain non-exact686/403/801/182 respectively; exact totals stay
at38/8,514. Read `TEXT_DRAWING_COMPOSITE.md`. No existing canonical source or
header was changed, and none of the previous38 units was replayed.

Private complete-body machine-code diagnostics agree on74 compositor cases
and162 text cases (including actual new filter bodies), with independent pixel
references for composition and standalone processing. Integrated drawing models
GDI raster bytes and D3DX transfer, not real fonts/GPU behavior; no exact/native/
full semantic authority. Each runs on one CPU/nice10; peak RSS about94/106 MiB.

Compositor format mismatch unlocks/succeeds; matching unsupported format fails
without unlocking. A4 byte0 low-nibble blending reads the newly overwritten
high-nibble byte; output alpha uses `(old_byte1>>2)&0x3C`, not standard alpha
accumulation. Preserve these with overlapping buffers. Native ECX owner/EAX y
differs from current compiled member ABI/frame. Aggregate and individually
assigned RECT shapes both produced686; do not repeat them without new evidence.

Text fill walks contiguous logical rows, while vertical neighbors use signed
pitch/2 or /4 truncated toward zero. A4 halves averaged colors. Inversion uses
signed pitch*rows and A1 clears RGB when toggled alpha becomes zero. Drawing
keeps both alpha passes, three selections, minimum height17, byte-counted
strings and right-only1024 clipping. HRESULTs do not gate D3DX load/release.
Its uninitialized surface output remains native-indeterminate if COM fails
without writing it. The current REP seed loop and A1 bitfield code remain
different; avoid forcing volatility, padding or false ABI. All four origins
remain indeterminate despite their identified use.

The preceding checkpoint added complete device creation1,338 plus exact error logging207,
main viewport99 and back-buffer priming157. Three strict cold public primary-TU
proofs cover463 owned bytes and30 independent fields. Only these new units
were replayed; previous35 source/header inputs and canonical proofs remain
unchanged. See `DEVICE_INITIALIZATION.md`.

Device creation source is1,350 versus1,338,845 unequal unrelocated positions,
no exact credit. Thirty-six modeled machine-code cases agree, including two
bounded prefixes of persistent retries: full0xB44 runtime state, presentation,
GPU call order/arguments, logs, matrices/viewport, cleanup, registers and stack.
Models use controlled tangent0.25 and D3DX matrices, not real GPU/math behavior;
no native/full semantic authority. One CPU/nice10, about64 MiB peak RSS.

Creation retains HW HAL -> SW HAL -> SW REF, option-controlled skips, signed
HRESULT tests, refresh/interval retries, capability warnings and exact-zero
format support. Refresh retry clears requested Hz/active status but not the
separate disable flag, so persistent failure can keep retrying. Disabled refresh
changes do not enable Windowed. Named float distance retains observed rounding/
negation; aggregate-eye1,302 and named-scalar1,350 remain non-exact. Original
vector/TU/frame context is open; do not repeat those two shapes without evidence.

Error logging sets+2004 even if append fails; lock/depth index3 and format return
are exact. Viewport context needs an independent unknown batch receiver to retain
native ESI: global-only context96, corrected complete99 exact. Priming is two
clear/present/reset-on-negative cycles,157 exact; counted loop91 is non-exact.
Do not normalize away real byte/owner differences. The initializer's1,335-address
Ghidra body omits3-byte LEA alignment; full-span review covers all1,338 bytes.
D3DX matrix thunks independently resolve to d3dx9_37.dll. Tangent31-byte boundary
was reviewed but has no new source mapping or credit.

The preceding checkpoint added startup/window577/344/274 sources, still
non-exact569/353/243. Its51 bounded machine-code scenarios agree; no old unit
was replayed. See `STARTUP_WINDOW.md` before revisiting those contexts.

CheckStartupPath returns0/-1 from opaque gate0x004C9898; the native caller tests
-1. Missing title flag0x40, byte mismatch at root+B30 and the return gate are
independent. Shortcut loops reload the original filename each iteration.
ReadShortcutTarget receives native EDI destination/stack filename, EAX boolean/
RET4. Source currently uses EDI/EBX and RET, so ABI remains unresolved. Preserve
ignored CoInitialize/conversion status, all COM releases and unconditional
CoUninitialize for non-null destination. The native checked array sizing and
source243 body remain different; generic helper origin stays indeterminate.

Window creation's actual HINSTANCE input is EBX, confirmed by caller. Explicit
memset rather than WNDCLASSA aggregate initialization restores that private
boundary and48-byte frame, but not full equality. Keep BLACK_BRUSH(4), exact
window styles, metric calls, fullscreen-first flow, opaque record resets and
GetWindowRect/handle publication even on null CreateWindowEx result. Root+58
is a second HWND with unknown consumer. Mode/timing byte meanings remain partial.
Free/member/fastcall aggregate contexts did not close it; input order,<new>,
/EHsc and early/shared-return shortcut contexts also failed. Do not repeat
these configurations without new evidence.

The preceding checkpoint added exact text creation396, release105 and font/
sample initialization346, plus non-exact executable checksum246. See
`TEXT_RESOURCES.md` for their56 exact bindings and checksum251/179 differences.

The text workspace at0x004A57C0 has256 sample bytes, followed by the earlier
surface-copy layout. Presence+11C is the DIB handle; DC+114, previous selection
+118 and pixels+120 are now identified. Release preserves imageBytes/pitch.
Create retains the -65 DIB height but64 cleared/logical rows and signed pitch
arithmetic. Its396-byte proof includes3-byte internal LEA alignment omitted
from Ghidra's393-address sparse body. Later publication of selection/format
keeps both output locals live and reproduces native stores. Earlier392/44 and
396/3 variants remain private failed diagnostics, not alternate proofs.

Initialization keeps both format attempts,256 locked RNG samples shifted9
and all four Japanese font calls. Creation/release origins stay indeterminate;
font initialization has independent game RNG/runtime integration evidence.
Checksum source retains signed size/4-1, final-word omission, failure-1 and
root+998/+99C publication. Indexed224, pointer247 and nested251 contexts remain
non-exact; do not repeat them without new evidence.

The preceding checkpoint added exact window callback418 and surface read201/
write217 source. Read/write remain non-exact202/205. See
`WINDOW_SURFACE_BUFFER.md` for their COM failure asymmetry and bounded probes.

The callback is installed as WNDCLASSA.lpfnWndProc at0x00446B36. Activation
runtime+0xB14 aliases the existing keyboard polling gate; inactivity is+0xB18,
resize flags+0xB40 and Windowed+0x114. Shared default handling, nested size/
system-command switches and do/while ShowCursor(TRUE) reproduce all418 bytes.
Explicit default returns gave400 bytes; equality/while form418 still differed.
Surface buffers use format100,width104,height108,rowBytes110,presence11C,
pixels120. Read ignores LockRect status; write requires exact zero and does
not unlock on failure. Original owner/origin is indeterminate. Free stdcall,
/EHsc, direct-pitch and nested-success variants did not close either routine;
do not repeat those contexts without new evidence.

The preceding checkpoint added renderer/device reset778 and texture recreation162
as strict exact functions. Texture preparation135 still has two commuted SIB
bytes at76/79 different. See `RENDER_STATE_RESET.md`.

State reset has33 D3D calls, real float-to-DWORD marshalling through a4-byte
slot, and renderer cache reset at+0x4355BC. No incoming ECX parameter exists;
the push/pop allocates that slot. Density1, fog1000/5000 and all method/state
constants are target/SDK-reviewed. Cache byte+7 is preserved.

Texture loops retain32 file slots at renderer+0x4350C0; signed texture count
is file+0x10C,20-byte entries at+0x120, flags+0x10. Prepare releases flagged
non-null textures and clears through fresh file/table reloads. Restore uses
presentation width/height/format, RENDERTARGET/DEFAULT and one level; it updates
format metadata even if CreateTexture fails. Holding one entry reference gave
166 bytes; separate condition/call access yields exact162. A release-member
helper gave123 rather than135; index/pointer-add forms retain two SIB differences.
No equivalence credit or operand patch is allowed.

With real reset/surface/VM/clock bodies visible, a new private frame family
still has RunFrame315 and PresentFrame233. A private float-elapsed/fast primary
with precise supports also gives233. Public frame source remains315/224 in
its earlier context, no exact credit. Do not repeat these full-dependency or
older local-type/profile probes without new evidence. The manager remains
stack-passed versus native EBX; frame/clock metadata and original owners remain
open. See `FRAME_LOOP.md` and the new caller diagnostics.

VM reset303 and release80 remain exact; binding204 has source191/165 differences,
surface initialization206 has source206/43. Keep12-byte opaque preservation,
DWORD+0x20, full0x434 clear, invalid-bind asymmetry and280-byte camera0-to2 copy.
The13-byte getter at0x0042B650 has no mapping/credit. See `RUNTIME_SURFACES.md`.

Camera344/batch143 remain exact. Perspective361 has four operand differences
at288,289,291,292;12 independent bindings replay but no exact credit. Preserve
target+0x0C, eye+0x3C, right+0x30 and0x118 camera stride. Archive extraction300
is source312, decoder472 source473/177 differences, generic unlock37 one SIB
difference. Preserve allocation size EAX plus three stack inputs and persistent
8KiB history. Screenshot/hotkey524/156 source540/156 also remain non-exact;
read their detailed docs before revisiting known unsuccessful contexts.

## Shared runtime data was decisive

`src/MainRuntime.hpp` is a partial physical view rooted at0x004C3280. Startup
writes instance storage there and passes that base to window setup; D3D cells
are+4/+8. Flags are+0x590 (0x004C3810), twelve24-byte critical sections start
at+0x810 (0x004C3A90), and depth bytes at+0x930 (0x004C3BB0). The last depth
byte0x004C3BBB has five direct-Ghidra read/write references. The0x93C view is
not a recovered complete class size.

This common view preserves the target's flag read after publishing both list
links. Separate scalar views let VC9 hoist that test ahead of the stores;
volatility and tiny-wrapper/profile probes also differed. Root/member binding
uses actual base plus addends; a new negative control rejects a wrong member
addend even when the target base is compensated to fit the final bytes.

Legacy standalone field views in other units remain pending native data-owner
integration. Do not migrate unrelated units gratuitously: source dependencies
would require focused revalidation. See `CALLBACK_CHAIN.md`.

## Oracle and resource rules

The linked lane accepts complete public primary-TU PDB code contributions,
requiring current source/tool/producer/artifact receipts, PE/MAP/PDB identity,
unique owner/extents, exhaustive fields and full byte equality. It now has25
synthetic controls. Specialized static/EH/split-owner acceptance remains open.
The new registration caller lives in `probes/chain-registration-context.cpp`.

Ghidra and Wine share one serialization lock, one CPU affinity and nice10.
Ghidra heap768MiB; JVM/native overhead is additional. Private Ghidra12.1.3
project: `ghidra-project/TH11`. VC9 RTM compiler15.00.21022.08/linker9.00.21022.08
remain pinned. Do not replay full cohorts for a local change.

## Remaining useful work and recent evidence

- Device initialization1,338 now has full non-exact source; viewport99 and
  priming157 are exact. Before revisiting codegen, read DEVICE_INITIALIZATION.md
  for original vector/frame context gaps and the actual retry behavior.
- Window creation577 and startup/shortcut344/274 have non-exact source and51
  bounded cases. Read STARTUP_WINDOW.md before repeating failed contexts.
- Compositor657 and text drawing392 now have full non-exact source, together
  with fill872/inversion165. Read TEXT_DRAWING_COMPOSITE.md before repeating
  their compiler contexts. Sibling outline631, surface text525 and real
  dispatcher172 now also have full non-exact source; read OUTLINED_TEXT.md.
  The font-owning wrapper0x00443D20(222) was reviewed but remains unmapped;
  its constructor/string cleanup need independent recovery. Formatting callers
  now have exact156/224 and non-exact212 source; read FORMATTED_TEXTURE_TEXT.md
  before modifying the shared source or repeating known failed contexts.
- Surface read/write201/217 now have maintained202/205-byte member source;
  review `WINDOW_SURFACE_BUFFER.md` before revisiting the failed contexts.
- `ReadArchivedFile` target300 uses an out-of-line53-byte lookup, stream
  Seek/Read, stored-name8-bit checksum, eight cipher rules, ScatterXorBlocks
  and optional stdcall decompression at0x004426C0. Observed free conditions
  include freeing a reused destination on failure. Source312 remains
  non-exact: error-block layout, alignment and registers differ. Nested and
  goto error flows, size-favoring286 and default Ob2 variants did not close it.
  Private `.analysis/archive-read-*` records bounded experiments; do not
  repeat those same probes. Both independent archive callers are retained
  in new compiler contexts to avoid global-only specialization.
- `LeaveRuntimeCriticalSection` target37 receives runtime in EDI and index
  in ESI under the natural linked convention. Its offset31 SIB commutes the
  same two registers; source stays readable and receives no exact credit.
- Current resource loader and lookup families compile existing ByteCipher
  source as directly needed /GL support. That does not grant cipher exactness
  or replay any previous canonical proof. Primary TU ownership is enforced
  separately for the three accepted functions.
- Update dispatch target352 includes its nine-entry table; maintained ordinary
  member source348 remains non-exact. New two-call/loop contexts produced312
  bytes; free-pointer/cdecl/stdlib/global variants did not solve it. Do not
  force count volatility or a fake ABI. Probe logs are private under
  `.analysis/chain-update-*`; these are diagnostic evidence only.
- The complete358-byte real frame caller at0x00446080 was reviewed/decompiled.
  It loads g_ChainManager into EBX immediately before calling update dispatch
  and clears EBX immediately afterwards. Thus the real manager value is not
  kept live after that call. Private evidence:
  `.analysis/frame-dispatch-review.txt`, `.analysis/frame-dispatch-caller.c`.
- MainRuntime grouping may help other source contexts. The gameplay joystick
  candidate815/target812 is still open. Its WinMM axis limits are fields of
  JOYCAPSA at0x004C2F08: current scalar view starts at+0x24 (0x004C2F2C).
  The existing joyGetDevCapsA call reads0x194 bytes. Review and test that
  physical view rather than treating the four limits as a standalone owner.
- The real trace sink is RET, and the gameplay caller preserves Acquire EAX
  across it. Same-TU empty source removes the call; separate normal COFF keeps
  it but still815. This checkpoint tested natural varargs setup/teardown and
  mixed /GL support profiles: release varargs setup yields a one-byte trace
  but removes the call; /Od support preserves a five-byte trace, still non-exact.
  JOYCAPSA and MainRuntime grouping, separately and together, leave normal
  gameplay polling815 bytes. Private `.analysis/joystick-owners/` records these
  failed probes. Do not repeat that batch or embed RET/fake returns.
- ByteCipher normal276/target284 each remains non-exact; its /GL caller
  naturally produces key-in-AL plus five stack arguments but body293 differs.
  ASCII expiry100/124 remains open. Prioritize useful medium functions.
- Frame dispatch307 and presentation226 now have maintained source in
  `FrameLoop.cpp`, with current315/224 candidates. Their fog54 and worker-stop101
  dependencies are exact in `FrameRuntime.cpp`; camera344/batch143 were already
  exact. `FRAME_LOOP.md` records all flows, physical views and bounded probes.
  Runtime surfaces206 and script binding204 now have full non-exact source;
  release80 and VM reset303 are exact. Renderer texture restore162 and state
  reset778 are now exact; texture preparation135 has two SIB differences.
  `RENDER_STATE_RESET.md` records complete flows and new caller probes.
  Do not repeat the frame's local-type/profile or full-reset contexts.
  Animation VM interpreter0x0044B4B0 is11640 provisional bytes, only its stack
  receiver entry is reviewed. Do not count its prototype as implementation.
- Screenshot capture0x00429CA0 is524 bytes. Its new source uses established
  MainRuntime opaque storage for presentation/screenshot views, preserving
  the existing header and unrelated receipts. /GL initial nested allocation
  flow produced529; observed failure block and countdown loops produce540.
  The actual hotkey caller still gives540. Main differences include EBP save,
  stack allocation, branch layout and alignment. No padding/ABI tricks.
- Hotkey0x00446880 has source156/target156 but23 unrelocated differences:
  source frame0x108/target0x110 and conditional versus entry ESI save.
  Independent repeated caller context does not resolve these. Current
  diagnostic families and source dependencies are in `SCREENSHOT.md`;
  private `.analysis/screenshot-*-linked.json` preserves results. Do not
  repeat those same context probes.
- Worker/file helpers now have maintained exact source, but old capture and
  hotkey diagnostic contexts have not yet been rerun with this new worker
  visibility; do not confuse their previous540/156 candidates with new proof.
- RNG entry0x00458CD0 is still missing from the provisional function table;
  raw decoding reveals a153-byte duplicate of Next32 before CC padding.
  The new `review_span` mode can inspect it without mutating Ghidra. Full
  boundary/origin/usefulness review remains open; it has no source or credit.

Full production TU/data/resource/library/link-order closure and native
runtime validation are open. Semantic reconstruction and portability remain
later gates; no complete native game build has been achieved.

## Recovery

```bash
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
```
