# Resume brief

Target: original Japanese TH11 v1.00a, SHA-256 in `config/target.toml`.
Use direct Ghidra only. Read `AGENTS.md` and inspect dirty work before changes.

## Current checkpoint

Inventory:1,988 provisional candidates; sixteen reviewed/source-present.
Eleven canonical exact functions total2,417 owned bytes:

- Eight normal-COFF units: ASCII update22; AddAngles157; NormalizeAngle145;
  repeat state105; keyboard polling888; file check141; draw chain284 including
  table; callback removal114.
- Three linked-PE units: joystick button capture267 and newly exact ordered
  update/draw registration147 each.

Both registration proofs share one cold primary-source/caller context. All
nine DIR32 fields per function bind and replay with zero differences. VC9
naturally uses element-in-ESI and priority-in-EBX from ordinary stdcall source;
no custom register ABI was imposed. Only these two changed units were
cold-replayed; the previous nine exact units were not rebuilt.

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
  it but still815. Natural varargs setup/teardown outside release logging and
  mixed /GL support profiles have not yet been tested; do not embed a RET or
  introduce fake return values to force preservation.
- ByteCipher normal276/target284 each remains non-exact; its /GL caller
  naturally produces key-in-AL plus five stack arguments but body293 differs.
  ASCII expiry100/124 remains open. Prioritize useful medium functions.

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
