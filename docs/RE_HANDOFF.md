# TH11 reconstruction — next-agent handoff (2026-11-16)

## Target and working rules

Reconstruct only the original Japanese TH11 v1.00a executable:
`resources/th11.exe`, SHA-256
`2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`.
Use the pinned VC9 / Visual C++ 2008 RTM build 21022 toolchain. Preserve
established LTCG profiles and use the `gpt-web: ...` commit prefix. Preserve
live worktree changes; do not reset them merely because they are dirty. Build
only the changed unit and affected exact units during routine work. Do not run
a full cold replay unless the task explicitly requires one.

`docs/PROGRESS.md` is generated from canonical ledgers; do not edit it or copy
its totals into this handoff. A commit or diagnostic similarity is not Oracle
proof. Exactness requires a fresh canonical receipt and accepted replay.

## Active Boss: ExecuteStageScript

The large owner is `ExecuteStageScript` at `0x00404470`. Its complete target
extent is 2,736 bytes: a 2,633-byte code body, three alignment bytes, and 100
bytes of switch tables. The current `execute-stage-script-frontier` PDB
contribution is 2,712 bytes, 24 bytes smaller. Its decoded body is 2,612
bytes; a normalized instruction-key sequence comparison aligns 366 of 609
target instructions (the candidate decodes 619). Its stack frame reserves
0x98 bytes, matching the target's 0x98. These are frontier measurements only;
the owner remains non-exact and EH/funclet ownership remains open.

The current maintained `src/StageScript.cpp` SHA-256 is
`9cc9d5f27c8aaf2fc986a9554fba7320a2ec61caa0b611f721860cc547bf1440`.

The latest frontier profile deliberately keeps `/Oy-` for StageScript while
compiling `src/StageMesh.cpp` and `probes/stage-mesh-context.cpp` in their
non-`/Oy-` profile. The dynamic constructor probe prevents LTCG from
specializing `StageScriptMesh` for only the Boss's `flags == 0` call sites.
That changes the constructor from a 237-byte specialized candidate to a
366-byte generic contribution. The target constructor is 389 bytes. The
Boss's opcode-17 caller now has the native generic argument shape: ESI object,
EAX row count, and two stack arguments; this is still not an exact constructor
or Boss proof.

Current source hypotheses:

- Opcode 2 snapshots the original camera position, applies the script vector,
  then writes the new-minus-old components through raw DWORD copies.
- Opcodes 3 and 5 retain case-local CameraVector argument temporaries. Opcodes
  10 and 11 share two function-scope CameraVector temporaries for the Bezier
  control1/target pair; control2 remains case-local. This recovers the target
  0x98 stack frame from the prior 0xb0 candidate.
- Opcode 9 has a local fog value; the fog postpass uses a separate result
  temporary.
- Opcode 17 uses an ordinary C++ `ReleaseVmBindingBlock` call. LTCG lowers
  that call to the native private ESI receiver ABI. Phase stores remain
  ordered as `phaseY = phaseX = phase`.

The remaining Boss work is stack/local lifetime, opcode-specific codegen,
and EH/funclet ownership. Do not treat a matching function length as exactness.

## Exact leaves and nearby owners

- `ReleaseVmBindingBlock`, target `0x00402300`: 231/231 bytes, six relocation
  fields replayed, zero differences in the focused linked-PE comparison.
- `PackStageFog`, target `0x00405B70`: 171/171 bytes, canonical exact proof.
- `AdvanceStageTimer`: 83-byte exact linked-PE owner.
- `UpdateStageObjects`: 153-byte exact linked-PE owner.
- `StageUpdateCallback`: 7-byte exact adapter.
- `StageScriptMeshConstructor`: current 366-byte candidate versus 389 target
  bytes, with 82 normalized instruction-key alignments. The generic ESI/EAX/
  stack argument ABI is restored, and EBX/EBP now carry columns/row as in the
  target; the constructor remains non-exact.
- `StepStageVectorTween` and `UpdateStage` remain large, non-exact owners.

The StageTween/StageScript family still has a 794-byte candidate for the
737-byte fog tween; a separate wide secondary context retains the exact
737-byte proof. Do not interchange those LTCG contexts. See
`docs/STAGE_SCRIPT.md`, `docs/STAGE_TWEEN.md`, and `docs/STAGE_MESH.md`.

## Next work

1. Continue the `ExecuteStageScript` Boss from the current frontier. Compare
   the complete 2,736-byte target owner, with special attention to the
   remaining 21-byte code-body shortfall, 619 versus 609 decoded instructions,
   and EH/funclet metadata. The current stack frame already matches at 0x98.
2. Keep the StageMesh support profile override and dynamic constructor probe
   in the frontier. Removing either re-specializes the constructor and
   changes the opcode-17 caller ABI.
3. Attack large owners as well as leaf functions. Preserve exact leaves and
   rerun only the changed unit plus dependency-ledger affected exact units.
4. Save meaningful checkpoints with `gpt-web: ...` commits. Do not claim
   exactness without the canonical strict receipt and accepted replay.

## Working references

- `docs/RE_WORKFLOW.md` — reconstruction loop and evidence boundaries.
- `docs/ORACLES.md` — exactness and replay requirements.
- `docs/KNOWLEDGE_BASE.md` — durable target/compiler findings.
- `docs/ARTIFACTS.md` — retained private artifacts and cleanup policy.
- `docs/PROGRESS.md` — generated inventory and exactness totals.
