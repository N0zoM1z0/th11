# Resume brief

Target: original Japanese TH11 v1.00a, SHA-256 in `config/target.toml`.
Use direct Ghidra only. Read `AGENTS.md` and inspect dirty work before changes.

## Current checkpoint

Inventory:1,989 provisional candidates; forty-four reviewed/source-present.
Twenty-nine canonical exact functions total5,846 owned bytes:

- Eight normal-COFF units: ASCII update22; AddAngles157; NormalizeAngle145;
  repeat state105; keyboard polling888; file check141; draw chain284 including
  table; callback removal114.
- Twenty-one linked-PE units: joystick button capture267, ordered update/draw
  registration147 each, ASCII string appender182, frame clock260, random
  generation105/153, log append211, screenshot writer229, shared file open
  modes180 each, write helper88, resource load466, complete-file write297,
  archive lookup53, camera setup344, batch submission143, fog disable54,
  runtime thread stop101, animation VM reset303 and surface release80.

This checkpoint adds three maintained medium functions, VM reset303, script
binding204 and runtime surface initialization206, plus paired release80.
Reset/release have strict cold primary-TU exact proofs totaling383 bytes and
eight independently reviewed fields. Only `reset-animation-vm` and
`release-runtime-surfaces` were cold-replayed; prior27 inputs were untouched
and no old canonical unit was replayed. See `RUNTIME_SURFACES.md`.

Reset preserves a12-byte opaque block+0x3E8 and DWORD+0x20 across0x434-byte
clear. Aggregate preservation, inline timer/node operations and restore order
reproduce its16-byte frame and all303 bytes. Scalar and64-bit-head views left
107/four differences; do not repeat them. Original preserved semantic type and
full owner size remain unknown. Invalid script binding clears all0x434 bytes
instead of preserving those fields. Its valid path initializes script/timer,
runs one VM update and increments renderer+0xA0; source191/target204 remains
non-exact with165 diagnostic differences.

Surface initialization206 obtains texture slots2/3 and the back buffer, binds
scripts81/82/81 to three VMs and copies full camera0 to2 on a null primary
surface after binding. It remains non-exact:206 bytes with43 differences in
register-save placement. Early-return/block and EHsc/Ob2 configurations did
not close it. Release80 preserves all three conditional clears and the final
unconditional first-cell clear. Its RET borders an unowned13-byte getter at
0x0042B650 without padding; getter is not mapped or credited.

Frame dispatch307 and presentation226 retain maintained315/224-byte source,
no exact credit. Manager passing remains stack versus native EBX; presentation
lacks the native PUSH/POP ECX frame. Double/float/member/fp-fast probes were
bounded and unsuccessful. Fog54 and thread101 are exact; thread origin stays
indeterminate. See `FRAME_LOOP.md` before revisiting those configurations.

Camera344/batch143 remain exact. Perspective361 has four operand differences
at288,289,291,292;12 independent bindings replay but no exact credit. Preserve
target+0x0C, eye+0x3C, right+0x30 and0x118 camera stride. See `RENDER_CAMERA.md`.

Archive extraction300 remains non-exact312; generic unlock37 differs by one
SIB byte. Decoder472 has source473/177 differences and62 matching bounded
machine-code cases. Preserve allocation size in EAX plus three stack inputs
and persistent8KiB history. Origin remains indeterminate. Screenshot capture/
hotkey524/156 have source540/156, no exact credit. Their detailed docs remain
current; no prior unsuccessful context should be repeated without new evidence.

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
  release80 and VM reset303 are exact. Renderer pre/post reset0x00445320/
  0x004453B0 (135/162) and render-state reset0x00447270 (778) still have no
  bodies. Their input boundaries are in
  `.analysis/frame-presentation-callee-entries.txt`. These useful reset paths
  are next candidates; do not repeat the frame's local-type/profile probes.
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
