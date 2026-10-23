# Resume brief

Target: original Japanese TH11 v1.00a, SHA-256 in `config/target.toml`.
Use direct Ghidra only. Read `AGENTS.md` and inspect dirty work before changes.

## Current checkpoint

The inventory has 1,988 provisional candidates. Seven have reviewed boundaries
and maintained source; four are canonical exact, totaling 429 code bytes:

- AsciiManagerOnUpdate, 22 bytes, prior checkpoint.
- AddAngles, 157 bytes, and NormalizeAngle, 145 bytes: new complete VC9 normal
  COFF proofs, including all three separately reviewed constant/data bindings.
- UpdateInputRepeatState, 105 bytes: new relocation-free raw-equal VC9 proof.

New keyboard/joystick polling sources cover the observed complete 812-byte and
888-byte target bodies but remain non-exact (current candidates 743/891 bytes).
AsciiManagerExpireStrings remains 100 versus 124 target bytes. Progress and
ledgers keep source presence separate from exactness.

## Operator priorities

Prioritize useful medium-size functions. Do not spend successive batches on
small leaves. Rebuild and compare only changed units and directly affected
source dependencies; no full exact cohort replay by default. This checkpoint
cold-built only the three new units (two objects), never the earlier ASCII unit.

## Analysis and tools

The private Ghidra 12.1.3 project is `ghidra-project/TH11`. Ghidra and Wine run
serially with one CPU affinity and nice 10; Ghidra heap is 768 MiB. The pinned
VC9 RTM 15.00.21022.08 compiler and 9.00.21022.08 linker have executed normal
COFF, /GL, resource, PE32, and PDB contribution/decoder smoke successfully.
Private NTLM prerequisites are hash-pinned and bootstrap without host services.

Caller evidence corrected provisional Ghidra types for input analysis: the
keyboard mask enters 0x00457270 in ECX and returns in EAX; the successful
cookie routine 0x0045FC2C preserves EAX. Temporary `u32@`, `u32-fast1@`, and
`cookie@` selectors recover decompiler code and roll metadata back. The raw
joystick 812-byte body has 269 instructions; a complete pinned Capstone decode
supplements the Ghidra review's 250-instruction display cap.

## Recovery

```bash
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
```

## Next useful units

1. Close PollKeyboardInput 0x004576B0 codegen (891 versus 888) and
   MergeJoystickInput 0x00457270 (743 versus 812), preserving all backend/error
   paths. See `docs/INPUT_POLLING.md` for globals, masks, types, and hypotheses.
   Normal O2 and a simple /GL entry produce the same 891-byte keyboard extent;
   the size delta alone hides substantial shifted-block byte differences.
2. Continue other medium gameplay/input functions as evidence warrants. Keep
   matches strict and source candidate state honest.
3. Complete the VC9 linked PE/PDB exact Oracle when accepting real LTCG units
   requires it. Current linked probes have no acceptance authority. A private
   diagnostic driver and results remain under `.analysis/`; canonical public
   units currently use the normal-COFF lane.
4. Production TU/data/resource/library/link-order closure and native runtime
   validation remain open. Semantic reconstruction and portability are later
   gates; no complete game build has been achieved.
