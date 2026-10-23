# Resume brief

Target: original Japanese TH11 v1.00a, SHA-256 in `config/target.toml`.
Use direct Ghidra only. Read `AGENTS.md` and inspect dirty work before changes.

## Current checkpoint

The inventory has1,988 provisional candidates. Fifteen have reviewed boundaries
and maintained source; eight are canonical exact, totaling1,856 owned bytes:

- AsciiManagerOnUpdate22; AddAngles157; NormalizeAngle145.
- UpdateInputRepeatState105; PollKeyboardInput888; FileExists141.
- ExecuteDrawChain284: new complete cold VC9 normal-COFF proof, including its
  six-entry switch table, alignment byte and all24 reviewed relocations.
- UnregisterChainElement114: new complete cold proof including operator-delete
  binding. The target callee is a cdecl tail jump to CRT free.

`CALLBACK_CHAIN.md` records five new maintained engine functions. Update
registration and draw registration each target147 bytes; conventional sources
are159 and retain the observed initialization, priority ordering and locking.
Each passed560 focused target-vs-VC9 execution cases. Update dispatch's full
owner is352 bytes including its nine-entry table; conventional member source
is348 and passed440 cases. Actual registration ESI/EBX and update EBX argument
contexts remain non-exact. Draw also passed220 focused diagnostic cases.

The initial Ghidra draw259/update316 bounds excluded their switch tables.
Reviewed complete spans now end at0x00456F2B/0x00456E0F. Separate table/padding
reference scans found only local indexed jumps; all case destinations were
reviewed. Do not restore the shorter auto-analysis bounds.

Other non-exact maintained work: joystick815 versus target812; paired byte
transforms276 normal versus target284 each; ASCII expiry100 versus target124.
Keyboard888 and file check141 were solved at the preceding checkpoint.

## Operator priorities and limits

Prioritize useful medium-size functions; only a few small support functions.
Rebuild/replay changed units and directly affected dependencies only. This
checkpoint cold-replayed execute-draw-chain and unregister-chain-element;
none of the six earlier exact units were rebuilt. Source presence and bounded
execution diagnostics do not grant exact byte credit.

Ghidra and Wine serialize on the shared tool lock with one CPU affinity and
nice10. Ghidra heap768MiB; native/JVM overhead is additional. Direct Ghidra
12.1.3 project: `ghidra-project/TH11`. VC9 RTM compiler15.00.21022.08 and
linker9.00.21022.08 are pinned and have passed COFF, /GL, resource, PE32 and
PDB smoke. NTLM prerequisites are private and pinned without host services.

## Recovery

```bash
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
```

## Evidence and next useful work

1. Continue medium functions and close the real linked caller/TU contexts.
   Chain sources have conventional declarations where the target uses private
   registers. Preserve the actual ABI and strict complete owned extents.
2. ByteCipher's `/GL /Ob1` caller probe naturally produces key-in-AL plus five
   stack arguments from ordinary six-argument source. Both linked extents293
   remain non-exact against284. Private source/results:
   `.analysis/cipher-caller/`, `build/probe-ltcg/cipher-caller/`.
3. Update dispatcher caller probes vary codegen substantially; latest global
   caller gives348 bytes but still lacks the target's real EBX context.
   Probe evidence: `.analysis/chain-caller/`, `build/probe-ltcg/chain-caller/`.
4. Implement strict VC9 linked PE/PDB acceptance before crediting LTCG bodies.
   Public `linked_image.py` and private `.analysis/ltcg_link.py` support bounded
   diagnostics only; no linked exact Oracle currently grants acceptance.
5. Focused private diagnostic drivers are `.analysis/check-chain.py` and
   `.analysis/check-chain-registration.py`, with corresponding behavior JSON.
   Earlier byte-cipher diagnostics passed692 cases each. Do not rerun unrelated
   suites when continuing a different subsystem.
6. Production TU/data/resource/library/link-order closure and native runtime
   validation remain open. Semantic reconstruction and portability are later
   gates; no complete game build has been achieved.
