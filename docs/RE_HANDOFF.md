# Agent handoff

Work in `/home/pentester/coding/codex_ida/th11-reconstruction/th11`; the outer
folder is not Git. Read AGENTS.md and relevant source/ledger rows. Current
source, receipts and target bytes outrank historical notes.

## Checked-in state

- Original Japanese TH11 v1.00a only; target identity: `config/target.toml`.
- 1,995 candidates;172 reviewed boundaries,163 source-present mappings.
- 82 exact functions /15,952 bytes:8 normal COFF and74 linked PE.
- Origins:90 authored,73 reviewed indeterminate,7 exclusions,1,825 pending.
- Direct local scripts/Ghidra; no Factory MCP or REA. Pinned VC9 RTM/SDK6.0A.
- Commit prefix: `gpt-6.1-sol: `. All dirty work belongs to this effort.

This checkpoint adds nine complete immediate-draw/dispatch mappings covering
3,363 recorded code bytes, including world1036, state572 and projected fog546.
Four new full cold proofs add964 bytes: strip/fan224 each and two UV258 owners.
Nine further boundaries/origins reviewed: eight engine/math origins remain
indeterminate; D3DXVec3Transform's six-byte import thunk is a verified exclusion.
See [ANIMATION_IMMEDIATE_DRAW.md](ANIMATION_IMMEDIATE_DRAW.md).

Caller audit corrects four draw wrappers from void to actual int status returns.
The54-byte unrounded unit's symbol changed accordingly; six affected old units
remain exact. New canonical family: `build/match-linked/AnimationImmediateDraw`.
Earlier AnimationQuad/AnimationVmDraw proofs remain current after this replay.
Original TU/data ownership and complete native product closure remain open.

## Current gaps

| Function | Target code bytes | Current source / diagnostic differences |
| --- | ---: | --- |
| DrawWorldAnimationVm,4513A0 | 1,036 | 1,038 /750 |
| ApplyAnimationVmWorldState,44F4B0 | 572 | 580 /411; switch-table ownership also open |
| DrawFoggedProjectedAnimationVm,451170 | 546 | 548 /150 |
| DrawAnimationVmByMode,451EF0 | 217 | 272 /115; full contribution includes table |
| AnimationExtentMagnitude,40B930 | 28 | 17 /28; aligned-frame gap |
| BuildBillboardAnimationVmQuad,450700 | 976 | Source open; complete295-instruction review |
| BuildRotatedAnimationVmQuad,4503D0 | 571 | Source open; complete target review |

Other large gaps remain in submission1112, projected construction792, billboard
fog794, blend state368, VM/script/tween and resource paths. Older projected
probe numbers predate the wrapper return correction; do not treat them as
current receipts. Topic docs retain historical evidence and concrete contracts.

## Concrete next work

1. Continue world1036. Three real matrices now live at function scope, restoring
   independent rotation/texture storage: frameD4 versus nativeD0, formerly94.
   Native has aligned EBP frame/private EBX VM/stack renderer; the candidate
   uses stack VM and a different register allocation. Source has complete
   dirty-cache, fabs half-anchor, world/texture and geometry/stream paths.
   Do not add padding/alignment or misdeclare ABI to close it.
2. State572 differs in shared blend-call blocks; dispatcher217 differs in
   private callees/parameter allocation. Their code and separately reviewed
   tables44F6EC..44F707 and451FCC..452003 need complete PDB ownership
   reconciliation before any exact claim. Never crop a table/contribution.
   Separate eligibility returns and natural index+=2 loops close the four
   new224/258-byte owners; explicit remaining-count loops were263 bytes.
3. Fog546 has alpha-store/red arithmetic scheduling and flag-ABI gaps. Do not
   introduce a float red-difference temporary: it added a non-native float
   rounding store. It uses fixed runtime eye+204, fractional fog channels,
   preserved alpha, no tint and no far early return. Beyond range use camera
   packed fog RGB with selected VM alpha. Restore all RHW after submission.
4. Preserve observed quirks: world UV cache tests U twice and ignores V;
   world uses FVF102/stride20/texture-factor args. Modes9/C/D are triangle
   strip, B triangle fan. Fan has no eligibility gate, flushes global
   g_RenderBatch and disables Z writes without restoring them. Four wrappers
   forward EAX status. Mode counts double and subtract2 with unsigned wrap.
5. Complete976-byte billboard/571-byte rotation without replacing FSINCOS.
   Precise sin/cos calls and fast separate FSIN/FCOS did not recover pairing.
   Submission1112's real CRT _frnd preserves RC but is not a VC9 intrinsic;
   pragma gives C4163. Continue larger VM/script/tween work and origin review.

## Evidence and checkpoints

Latest proof: `.analysis/immediate-draw-ten-cold.log`, four new/six affected
units only. All ten passed complete current receipts/ownership/fields/bytes.
Current new-family full diagnostics: `.analysis/immediate-draw-current.log`
and `immediate-draw-current/diagnostic.json`. All17 address fields of strip/fan
were independently bound; UV owners have none. No full cohort or broad runtime
suite ran. Infrastructure code did not change.

Topic docs link full Ghidra/reference/neighbor/table reviews and compiler
experiments (guards/loops, matrix lifetime, blend-pair/red temporary, restricted
caller graph). The latter two variants did not improve the remaining gaps.
Target, compiler payloads, extracted CRT objects and `ghidra-project/TH11`
remain private. No tool job is live.

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
