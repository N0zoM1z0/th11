# Resume brief

Target: original Japanese TH11 v1.00a, SHA-256 in `config/target.toml`.
Use direct Ghidra only. Read `AGENTS.md` and inspect dirty work before changes.

## Current checkpoint

Inventory:1,989 provisional candidates; fifty reviewed/source-present.
Thirty-two canonical exact functions total7,204 owned bytes:

- Eight normal-COFF units: ASCII update22; AddAngles157; NormalizeAngle145;
  repeat state105; keyboard polling888; file check141; draw chain284 including
  table; callback removal114.
- Twenty-four linked-PE units: joystick button capture267, ordered update/draw
  registration147 each, ASCII string appender182, frame clock260, random
  generation105/153, log append211, screenshot writer229, shared file open
  modes180 each, write helper88, resource load466, complete-file write297,
  archive lookup53, camera344, batch143, fog54, runtime thread stop101,
  animation VM reset303, surface release80, renderer texture restore162,
  device-state reset778 and main window callback418.

This checkpoint adds complete surface read201/write217 source and exact main
window callback418. The callback's strict cold public primary-TU proof covers
all418 bytes and21 independently reviewed runtime/IAT bindings. Only the new
`main-window-proc` unit was cold-built; previous31 inputs/proofs are unchanged.
Read/write remain non-exact202/205. Full scopes, message behavior, COM failure
asymmetry and bounded probes are in `WINDOW_SURFACE_BUFFER.md`.

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

- New medium candidates have bounded decompiler hypotheses in the private
  `render-medium-candidates.c`: font setup0x00444480(346), startup title/link
  handling0x00447580(344) and executable checksum0x004476F0(246). These still
  need complete boundary/ABI/origin review before source or exact credit.
- Surface read/write201/217 now have maintained202/205-byte member source;
  review `WINDOW_SURFACE_BUFFER.md` before revisiting the failed contexts.
  The adjacent657-byte compositor has complete instruction review but no
  source mapping or credit; its pixel-format cases are another useful target.
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
