# Resume brief

Target: original Japanese TH11 v1.00a, SHA-256 in `config/target.toml`.
Use direct Ghidra only. Read `AGENTS.md` and inspect dirty work before changes.

## Current checkpoint

The inventory has 1,988 provisional candidates. Ten have reviewed boundaries
and maintained source; six are canonical exact, totaling 1,458 code bytes:

- AsciiManagerOnUpdate, 22 bytes.
- AddAngles, 157 bytes; NormalizeAngle, 145 bytes.
- UpdateInputRepeatState, 105 bytes.
- PollKeyboardInput, 888 bytes: new complete cold VC9 normal-COFF proof with
  thirteen independently reviewed relocation bindings.
- FileExists, 141 bytes: new complete cold proof with fourteen DIR32 bindings,
  including four independently parsed KERNEL32 import slots.

Separate input-lost and other-error branches solve the previous keyboard
891-versus-888 codegen difference. The complete two-map polling, error paths,
joystick merge and mask publication are covered; see `INPUT_POLLING.md`.

MergeJoystickInput remains non-exact: improved source815 versus target812.
It now models separate aligned buffers, WinMM early returns, the actual retry
counter and format arguments, and observed axis order. Its empty trace callee
preserves EAX in the target; making that sink visible as an empty source body
removes the observed call, so that experiment was discarded.

New ScatterXorBlocks/GatherXorBlocks sources cover two complete284-byte target
spans; normal VC9 extents276 remain non-exact. Each passed692 bounded private
Unicorn target-vs-VC9 execution cases with shared malloc/memcpy/free shims.
Target key-in-AL ABI and code origin remain open. See `BYTE_CIPHER.md`.
AsciiManagerExpireStrings remains100 versus target124.

## Operator priorities

Prioritize useful medium-size functions. Do not spend successive batches on
small leaves. Rebuild/replay only changed units and affected dependencies.
This checkpoint cold-replayed only poll-keyboard-input and file-exists; the
four earlier exact units were not replayed. Byte-cipher and joystick checks
were focused diagnostics. No source candidate receives exact byte credit.

## Analysis and tools

Private Ghidra12.1.3 project: `ghidra-project/TH11`. Ghidra and Wine run serially
with one CPU affinity and nice10; Ghidra heap768MiB. Pinned VC9 RTM compiler
15.00.21022.08/linker9.00.21022.08 have passed normal COFF, /GL, resource,
PE32 and PDB smoke. Private NTLM prerequisites are pinned and bootstrap without
host services.

Temporary `u32@`, `u32-fast1@`, `cookie@` Ghidra selectors restore input return
flow and roll metadata back. The cookie checker at0x0045FC2C preserves EAX on
success. Full joystick269-instruction Capstone decode supplements Ghidra's
250-instruction review cap. No target bytes or saved DB types were patched.

## Recovery

```bash
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
```

## Next useful units

1. Continue medium game/file/input functions. Preserve strict full extents and
   actual ABI; keep source, normal-COFF exactness and production context separate.
2. Joystick's815-versus812 candidate needs real trace callee/register context;
   do not infer similarity from the three-byte size delta. Private probes and
   complete target disassembly remain under `.analysis/`.
3. ByteCipher's key-in-AL/five-stack-argument context likely needs an actual
   linked caller/TU experiment. Normal source declarations explicitly remain
   conventional views. Avoid manually imposing an unsupported ABI for credit.
4. Implement the strict VC9 linked PE/PDB Oracle before accepting LTCG bodies.
   `linked_image.py` and private `.analysis/ltcg_link.py` support bounded probes;
   those probes have no acceptance authority.
5. Production TU/data/resource/library/link-order closure and native runtime
   validation remain open. Semantic reconstruction and portability are later
   gates; no complete game build has been achieved.
