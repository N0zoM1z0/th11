# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; the outer
folder is not Git. Read AGENTS.md and relevant source/ledger rows. Current
source, receipts and target bytes outrank historical notes.

## Checked-in state

- Original Japanese TH11 v1.00a only; target identity: `config/target.toml`.
- 2,001 candidates;181 reviewed boundaries,171 source-present mappings.
- 88 exact functions /16,841 bytes:8 normal COFF and80 linked PE.
- Origins:90 authored,82 reviewed indeterminate,7 exclusions,1,822 pending.
- Direct local scripts/Ghidra; no Factory MCP or REA. Pinned VC9 RTM/SDK6.0A.
- Commit prefix: `gpt-6.1-sol: `. All dirty work belongs to this effort.

Latest checkpoint adds eight complete source mappings/2,292 target bytes,
including805-byte fan initialization and598-byte update. Six full cold exact
proofs add889 bytes: quad138, colored strip/fan336 each, active getter10,
vector length44 and fan draw callback25. Nine further origins/boundaries
reviewed; all nine remain indeterminate. See [ANIMATION_FAN.md](ANIMATION_FAN.md).

Six new owners were absent from the1,995-candidate inventory:451AE0/138,
451B70/336,451CC0/336,452080/10,452090/44 and452680/25. Complete explicit spans,
RET/padding/reference review and independent decoding establish them. CSVs now
include them; Ghidra DB was not mutated. Do not regenerate the old inventory
and discard these maintained additions.

## Current gaps

| Function | Target bytes | Current source / diagnostic differences |
| --- | ---: | --- |
| InitializeFanAnimationVm,4520F0 | 805 | 805 /417 |
| UpdateFanAnimationVm,452420 | 598 | 614 /436 |
| AnimationPolarVector::SetPolar,4526A0 | 30 | Real boundary reviewed; FSINCOS body source open |
| DrawWorldAnimationVm,4513A0 | 1,036 | 1,038 /750 |
| ApplyAnimationVmWorldState,44F4B0 | 572 | 580 /411; table ownership open |
| DrawFoggedProjectedAnimationVm,451170 | 546 | 548 /150 |
| DrawAnimationVmByMode,451EF0 | 217 | 272 /115; full contribution includes table |

Other large gaps remain in billboard976, rotation571, submission1112,
projected792, billboard fog794, state368 and the VM/script/tween/resource paths.
Exact codegen does not establish original TU/data ownership or native product
closure. Source-only mappings and origin review grant no exact credit.

## Concrete next work

1. Continue fan805/598. Actual polar4526A0 writes XY with paired FSINCOS and
   returns output in EAX under thiscall, consuming two floats. Its native EDX
   preservation permits update's VM to stay there; the opaque source boundary
   cannot supply that private fact. No separate sin/cos or invented helper
   body is substituted. Candidate update frame1C versus native18 remains open.
2. Preserve fan behavior: allocation4B0 is uninitialized;33 vertices, radius
   slots39C and deltas420, only indices1..31 stepped; drift4A4 used for both
   UV axes while4A8 is unused. Negative UV shifts all33 by1; no positive wrap.
   Edge alpha0, Z accumulates after XY polar calls; closing vertex32 copies1.
   Center/edge colors and closing vertex are unwritten during initialization.
   RNG state4C2F00 uses the existing RandomNext32;65 calls in initialization.
3. Colored336-byte paths use FVF44/stride20, SELECTARG1 diffuse, an unconditional
   global renderer flush and disabled Z writes. After drawing they invalidate
   global geometry/C1/C3 toFF, blend7 and restore MODULATE/texture ARG1. They
   have no eligibility gate and do not restore Z writes. Quad138 uses byte
   alpha access; a DWORD mask was141 bytes. All six new extents are complete.
4. World1036 uses three real matrix lifetimes (frameD4/nativeD0); aligned frame
   and private VM registers remain open. A consistent private VM-first source
   parameter-order experiment produced the same1,038/750 and was not adopted.
   State572/dispatcher217 need full code/table ownership reconciliation before
   acceptance; never crop a contribution. World UV cache checks U twice.
5. Keep broader work moving: paired FSINCOS remains open in billboard/rotation;
   real CRT _frnd preserves RC but pragma intrinsic gives VC9 C4163. Continue
   larger VM/script/tween functions and extend boundary/origin review.

## Evidence and checkpoints

New canonical family: `build/match-linked/AnimationFan`. Latest proof:
`.analysis/animation-fan-six-cold.log`, six new units only. All40 address fields
and every byte replay with complete current source/tool/producer/PE/MAP/PDB
receipts and primary-TU ownership. Current full diagnostics:
`.analysis/animation-fan-current.log` and `animation-fan-current/diagnostic.json`.
Existing canonical sources/headers/families did not change; no old exact unit,
full cohort or broad behavioral suite was replayed. Infrastructure is unchanged.

Topic docs link full Ghidra spans/attestations, references, constants, metadata
and compiler probes, including `world-vm-first.log`. Target, compiler payloads,
extracted CRT objects and `ghidra-project/TH11` remain private. No tool job is live.

```bash
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py --require-target
```

Cold-replay changed exact units and directly affected dependencies only.
Update tracking/progress, check the diff, and save meaningful progress.
Broad behavior/failure/random/caller suites are retired. Full native product,
runtime and portability work remains later work; the reconstruction goal is active.
