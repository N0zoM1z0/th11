# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; the outer
folder is not Git. Read AGENTS.md and relevant source/ledger rows. Current
source, receipts and target bytes outrank historical notes.

## Checked-in state

- Original Japanese TH11 v1.00a only; target identity: `config/target.toml`.
- 2,038 candidates;222 reviewed boundaries,179 source-present mappings.
- 93 exact functions /17,731 bytes:8 normal COFF and85 linked PE.
- Origins:90 authored,123 reviewed indeterminate,7 exclusions,1,818 pending.
- Direct local scripts/Ghidra; no Factory MCP or REA. Pinned VC9 RTM/SDK6.0A.
- Commit prefix: `gpt-6.1-sol: `. All dirty work belongs to this effort.

Latest checkpoint adds eight complete source mappings/3,746 target bytes,
including renderer constructor2,601, destructor225, buffer setup487 and VM
reclaim273. Five full cold exact proofs add890 bytes: setup487, reclaim273,
format40 and packed-pixel45 each. Forty-one further origins/boundaries reviewed,
all indeterminate. See [ANIMATION_RENDERER_LIFECYCLE.md](ANIMATION_RENDERER_LIFECYCLE.md).

Thirty-seven entries were missing from the prior2,001-candidate inventory:
29 constructor-bound callbacks, four pixel helpers and four3-byte identities.
Explicit spans, constructor bindings, RET/tail/padding/reference review and
independent decoding establish them; DB was not mutated. Preserve these CSV
additions instead of regenerating the old Ghidra inventory over them. Callback
bodies and identity types remain source-open.

## Current gaps

| Function | Target bytes | Current source / diagnostic differences |
| --- | ---: | --- |
| AnimationRendererConstructor,4526F0 | 2,601 | 2,601 /153; EH acceptance open |
| AnimationRendererDestructor,453130 | 225 | 202 /73; EH/default-member inlining open |
| AccumulateAnimationArgbPixel,453440 | 30 | 30 /6; green/blue load schedule |
| InitializeFanAnimationVm,4520F0 | 805 | 805 /417 |
| UpdateFanAnimationVm,452420 | 598 | 614 /436 |
| AnimationPolarVector::SetPolar,4526A0 | 30 | Real paired-FSINCOS boundary; source open |
| DrawWorldAnimationVm,4513A0 | 1,036 | 1,038 /750 |
| ApplyAnimationVmWorldState,44F4B0 | 572 | 580 /411; table ownership open |
| DrawFoggedProjectedAnimationVm,451170 | 546 | 548 /150 |
| DrawAnimationVmByMode,451EF0 | 217 | 272 /115; full contribution includes table |

Other large gaps remain in billboard976, rotation571, submission1112,
projected792, billboard fog794, state368 and VM/script/tween/resource paths.
Exact codegen does not establish original TU/data ownership or native product
closure. Source-only mappings and origin review grant no exact credit.

## Concrete next work

1. Renderer construction uses real4096/default/31 VM member lifetimes before
   clearing allocation7BD894, then resetting only the main pool and registering
   29 callbacks. Constructor full span includes3 alignment bytes; destructor
   includes6. Retain them. Current destructor calls the default member out of
   line; target inlines free/clear435580 and has different EH states. Specialized
   EH acceptance remains open even if ordinary body differences are closed.
2. Reclaim4558E0 requires rendererEDI and VMEAX. Pool occupancy map is4096
   bytes at4340BC, after VM poolBC..4340BB. It unlinks both VM nodes; pool
   path free/clear400 then reset, heap path free/clear400 then direct delete.
   Do not invent an early null guard. Full273-byte proof is already accepted.
3. Buffer487 uses20-byte XYZ/UV renderer corners and24-byte projected scratch
   at4C9248, leaving the fourth scalar untouched. Shared4C91D8 vertices are28
   bytes. Direct renderer fields and separate U/V groups closed all487 bytes.
   A local vertex-base form was382 bytes; keep these layouts distinct.
4. Pixel format table496548 is six formats followed by six bytes-per-pixel
   DWORDs. Ushort snapshots close both45-byte packed helpers. ARGB native loads
   green and blue before writing the green sum; explicit temporaries instead
   produced28 bytes/private-register changes and were rejected. No unsupported
   restrict/alias promise is introduced. Four identity entries remain unknown.
5. Continue fan805/598 and broader big functions. Polar4526A0 preserves EDX,
   letting fan update keep VM there; the opaque source boundary cannot convey
   that private fact. Never replace paired FSINCOS with separate sin/cos.
   Fan allocation4B0 is uninitialized;33 vertices, only indices1..31 stepped,
   both UV axes use drift4A4, edge Z accumulates, closing32 copies1. See fan topic.
   World1036's private parameter-order probe stayed1,038/750 and was rejected.

## Evidence and checkpoints

New canonical families: `build/match-linked/AnimationRendererLifecycle` and
`build/match-linked/AnimationPixelHelpers`. Latest proof:
`.analysis/renderer-pixel-five-cold.log`, five new units only. All31 independently
reviewed fields and every byte replay with complete current
source/header/tool/producer/PE/MAP/PDB receipts and primary-TU ownership.
Current full diagnostics: `.analysis/renderer-lifecycle-direct-fields.log` and
`.analysis/pixel-helpers-current.log`, with diagnostic JSON under corresponding
probe directories. Topic doc links full target attestations and rejected probes.

Existing canonical source/header/context files did not change; no old exact
unit, full cohort or broad behavioral suite was replayed. Infrastructure is
unchanged. Target, compiler payloads, decompiler output and Ghidra data remain
private. No tool job is live at checkpoint.

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
