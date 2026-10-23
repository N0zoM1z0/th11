# Resume brief

Target: original Japanese TH11 v1.00a, SHA-256 in `config/target.toml`.
Use direct Ghidra only. Read `AGENTS.md` and inspect dirty work before changes.

## Current checkpoint

Inventory:1,988 provisional candidates. Sixteen have reviewed boundaries and
maintained source. Nine are canonical exact, totaling2,123 owned bytes:

- Eight normal-COFF units: ASCII update22; AddAngles157; NormalizeAngle145;
  input repeat105; keyboard polling888; file check141; draw chain284 including
  table; callback removal114.
- PollJoystickButtons267: first canonical linked-PE unit. Full source /GL cold
  build, complete primary-TU PDB contribution, PE/MAP/PDB identities, receipt,
  and all thirteen reviewed field bindings pass with zero differences.

PollJoystickButtons returns the128-byte array at0x004C98A0; the config caller
at0x0043BB39 indexes EAX immediately. Default Ghidra void inference was wrong.
WinMM expands32 bits; successful DirectInput Poll copies128 buttons even if
GetDeviceState returns an error, and does not clear the source state first.
Failure Poll performs up to400 extra Acquire retries without the trace call.
See `INPUT_POLLING.md`.

## Linked acceptance is now implemented for public primary owners

`scripts/ltcg_link.py`, `linked_receipt.py` and `compare-linked-function.py`
provide cold non-runnable contexts and strict complete public primary-TU
acceptance. Build/replay scripts dispatch explicit artifact kinds. All local
quoted headers, profiles, sources, tool/producer hashes, objects, PE, MAP, PDB
and anchor artifacts are bound. Output collisions and contradictory /GL-
profiles are rejected. Entry code cannot resolve to an anchor. Full PDB
contribution size and unique MAP/PDB ownership must agree; every linked field
is declared, symbol-bound and replayed before full byte comparison.

Twenty-four public synthetic controls cover positive replay, wrong final
bytes, cropped/overlapping/wrong owners, metadata identity, interior entries,
field and symbol errors, aliases, stale receipts and control-plane gates.
CI installs pinned Capstone5.0.6. Specialized static-owner/EH/split-owner
contexts remain open. See `LINKED_ORACLE.md`; do not broaden claims from this
single-function lane to a native game or original TU/GL ownership.

Only poll-joystick-buttons was cold-built/replayed in this checkpoint. The
previous eight exact units were not rebuilt. All producer edits were finished
before its final focused cold replay; the receipt is current.

## Existing candidates and useful next work

- Chain update owner352 includes nine-entry table; conventional member source
  is348 and passed440 target diagnostic cases. Two ordered registrations each
  target147; source159 and560 cases each. Actual EBX / ESI+EBX contexts remain
  non-exact. Draw284 includes259-byte code, one NOP and24-byte table; do not
  restore its shorter initial Ghidra bound. See `CALLBACK_CHAIN.md`.
- Joystick gameplay merge815 versus target812 remains non-exact. Empty trace
  body visible to /GL removes the call; a separate normal-COFF empty sink keeps
  the call but still815. Private `.analysis/joystick-native-trace/` records it.
- ByteCipher normal276 / target284 each. A /GL caller naturally produces the
  observed key-in-AL/five-stack-argument convention but linked bodies293 are
  non-exact. New natural source/loop/convention probes remain non-exact:
  `.analysis/cipher-shapes/`; no new exact credit. Original driver evidence:
  `.analysis/cipher-caller/`, `build/probe-ltcg/cipher-caller/`.
- ASCII expiry100 versus124 remains open. Prioritize medium useful functions;
  avoid batches of tiny leaves. Preserve actual ABI and complete extents.

Use new linked contexts when appropriate; keep unsupported static/EH cases
pending rather than forcing them through a normal-COFF or shortened owner.
Production TU/data/resource/library/link-order closure, native runtime,
semantic reconstruction and portability remain open later gates.

## Resources and recovery

Ghidra and Wine share a serialization lock, one CPU affinity and nice10.
Ghidra heap768MiB; JVM/native overhead is additional. Private Ghidra12.1.3
project: `ghidra-project/TH11`. VC9 RTM compiler15.00.21022.08 and
linker9.00.21022.08 are pinned; all compiler artifact-family smoke has passed.
Do not rerun full cohorts when changing one unit.

```bash
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
```
