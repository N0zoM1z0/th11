# Resume brief

Target: original Japanese TH11 v1.00a, SHA-256 in `config/target.toml`.
Use direct Ghidra only. Read `AGENTS.md` and inspect dirty work before changes.

## Current checkpoint

Inventory:1,988 provisional candidates; eighteen reviewed/source-present.
Thirteen canonical exact functions total2,859 owned bytes:

- Eight normal-COFF units: ASCII update22; AddAngles157; NormalizeAngle145;
  repeat state105; keyboard polling888; file check141; draw chain284 including
  table; callback removal114.
- Five linked-PE units: joystick button capture267, ordered update/draw
  registration147 each, ASCII string appender182 and frame clock260.

This checkpoint adds two useful medium functions,442 bytes. Only
`ascii-manager-append-string` and `get-frame-clock-seconds` were cold-replayed,
once each; the previous eleven exact units were not rebuilt.

The appender has no linkage fields: all182 bytes match directly. Ordinary
stdcall source and independent calls naturally yield manager-in-ESI,
position-in-EBX and text-in-ECX. See `ASCII_LIFETIME.md`.

The clock's complete260-byte public primary-TU contribution passes26 DIR32
bindings and full replay. Its shared double local precedes LARGE_INTEGER;
the high-resolution comparison is `origin > seconds`. Normal COFF then
differs only in three stack-allocation immediates16 versus20. The ordinary
two-call linked context naturally generates20. Do not add padding or force
alignment. See `FRAME_CLOCK.md`, `src/FrameClock.cpp` and
`probes/frame-clock-context.cpp`.

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
- Direct-Ghidra review/decompilation also covered frame dispatch0x00446650
  (307 bytes), presentation0x00446790 (226) and screenshot hotkey0x00446880
  (156). Private `.analysis/frame-timing-review.txt` and `frame-timing.c`
  contain complete flows. The latter scans snapshot/th%.3d.bmp up to999;
  these three functions have no maintained source yet.

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
