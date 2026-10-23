# Resume brief

Target: original Japanese TH11 v1.00a, SHA-256 in `config/target.toml`.
Use direct Ghidra only. Read `AGENTS.md` and inspect dirty work before changes.

## Current checkpoint

Inventory:1,989 provisional candidates; thirty-three reviewed/source-present.
Twenty-three canonical exact functions total4,821 owned bytes:

- Eight normal-COFF units: ASCII update22; AddAngles157; NormalizeAngle145;
  repeat state105; keyboard polling888; file check141; draw chain284 including
  table; callback removal114.
- Fifteen linked-PE units: joystick button capture267, ordered update/draw
  registration147 each, ASCII string appender182, frame clock260, random
  generation105/153, log append211, screenshot writer229, shared file open
  modes180 each, write helper88, resource load466, complete-file write297
  and the required archive lookup53.

The preceding checkpoint added three exact functions,816 bytes. Only
`load-resource-file`, `write-whole-file` and `find-archive-entry` were
cold-replayed. Three families keep each accepted contribution in its own
primary TU. Prior twenty proofs were not replayed. All66 independently
reviewed fields and all owned bytes match.

Resource loading now has the complete archive/disk branch source. Preserve
the observed two-delimiter behavior, archive-miss size zero versus untouched
disk-failure size, ignored extraction/ReadFile status, and all cleanup paths.
Explicit null branches, assigning zero only after failed lookup, and shared
failure labels before disk-read success reproduce the466-byte body. Initial
variants475/407/469 differed; do not simplify the final failure joins.

The297-byte complete-file writer uses a local handle, status-1/-2/0 and
section/depth2. Ordinary stdcall source naturally yields EDI filename, EBX
size and buffer on the stack; the compiler reuses the dead incoming buffer
home for the error-message pointer. The53-byte archive lookup is the needed
case-insensitive16-byte-record scan. See `RESOURCE_FILES.md` for full bindings.

Archive extraction300 and generic unlock37 have maintained source and
reviewed authored-game mappings, but no exact credit. Current candidates are
312 and37 respectively, with one commuted SIB-byte difference in the latter.
Source views include archive state/entries/virtual stream slots and eight
cipher rules. Original owners, stream closure and native archive integration remain open.

CaptureScreenshot524 and HandleScreenshotHotkey156 now have maintained,
compiled source and reviewed authored-game mappings, but no exact credit.
Candidate sizes are540/156; the hotkey has23 unrelocated differences. See
`SCREENSHOT.md` and the bounded probe evidence below.

This checkpoint adds the complete472-byte archive decompressor source and
corrects extraction's missing output-allocation-size argument. Four reviewed
callers confirm output size in EAX plus three stack arguments. The natural
stdcall candidate is473 bytes with177 unrelocated differences; no new exact
credit.62 bounded target/candidate machine-code cases agree on allocation,
returns, output/canaries, history and preserved ABI state. Origin remains
indeterminate. See `ARCHIVE_DECOMPRESSION.md`.

Only `load-resource-file` and `find-archive-entry` were cold-replayed after
adding the real decoder to their support families. Both remain exact; the
other21 canonical units were not replayed. Decoder history remains shared
and is not cleared per call. Do not omit its hidden fourth argument or
repeat the documented class/scalar/output-reference/history-owner probes.

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
- Frame dispatch0x00446650 (307 bytes) and presentation0x00446790 (226) still
  have no maintained source. Private `.analysis/frame-timing-review.txt`
  and `frame-timing.c` contain complete flows.
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
