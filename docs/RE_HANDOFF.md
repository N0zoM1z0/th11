# TH11 reconstruction handoff — 2026-11-18

## Target and repository state

Reconstruct only the original Japanese TH11 v1.00a executable:
resources/th11.exe, 688,128 bytes, SHA-256
2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813.
The target and PE/Rich identity were rechecked on 2026-11-18. The compiler
candidate is pinned VC9 / Visual C++ 2008 RTM build 21022; preserve /GL
for established LTCG owners.

The source checkpoint entering this handoff cleanup was
5e0a53b (gpt-web: restore indexed six-point culling traversal).
The documentation commits in this cleanup are local; check the live
upstream relationship before assuming they have been pushed. The source
worktree is clean after preserving and withdrawing the unhelpful spell-view
method experiment described below.

scripts/progress.py currently reports 2,175 tracked candidates, 1,610
reviewed boundaries, 819 source mappings, and 486 canonical exact functions
covering 53,331 code bytes. These are local ledger totals. The Factory
inventory reports 2,175 subjects, zero imported Oracle results, and an
inventory-import-only warning; do not present local exact counts as
Factory-accepted results or whole-game closure.

## 2026-11-19 selected newly exact geometry/math owners

- `SecondaryAngleDifference4287A0` (0x4287A0, 101 bytes): complete
  independent linked `/GL` C++ owner, 34 instructions, three pi/tau DIR32
  fields, zero strict cold-replay differences and a real caller at 0x428716.
- `SecondaryViewportPoint428850::IsOutside` (0x428850, 93 bytes): complete
  independent native thiscall C++ body, five real callsites, four ordered
  rectangle tests and four float-constant DIR32 fields. Strict VC9 normal-COFF
  cold replay has zero byte differences. This does not prove the native TU
  was a normal COFF unit or verify its original nominal C++ class.

Both use natural C++ without ASM/padding. Their target boundaries, incoming
references and following padding are reviewed; only these two exact units
were cold-replayed. See SECONDARY_PAIR_MATH.md for evidence and limitations.

## Selected exact angle adapter

`SecondaryAngleMath` (0x40B900, 34 bytes) now has natural source and
strict selected VC9 `/GL` cold replay. A genuine aligned float local preserves
all 12 native instructions and one REL32 call to `__CIatan2` at 0x486C9A.
See `docs/SECONDARY_SCRIPT.md`.

## New complete exact replay loader

`LoadReplayStageData` (0x436B60) has a natural C++ pinned VC9 `/GL` 441-byte
complete owner, **137/137 instructions** and **18/18** reviewed relocation
fields, all strictly cold-replayed with zero differences. A typed header local
restored the allocator/register schedule; the original `replay/%s` constant
is bound through the actual source MAP string literal. This is a substantial
complete function, not a cropped leaf. See SECONDARY_REPLAY_LIFECYCLE.md.

## Current large-function frontiers

| Function | Native owner | Current focused VC9 /GL contribution | State |
| --- | ---: | ---: | --- |
| CullStageObject (0x403950) | 1,643 B | 1,648 B; 328/504 aligned instructions | Main active boss; strict depth, NaN extrema, three-argument fastcall; EAX camera input remains open |
| DrawStageLayer (0x404190) | 727 B | 744 B; 112/191 aligned instructions | Nonexact; native scale operand/flag/divide ordering restored |
| ExecuteStageScript (0x404470) | 2,736 B | 2,712 B; 577/610 aligned instructions | Nonexact; per-mode oscillator tail restored; EH/funclet ownership open |
| StepStageVectorTween (0x405240) | 883 B | 883 B; 278/280 aligned instructions | Nonexact; native Z fallback add order restored; Y x87 addition order remains |
| InitializeSecondarySpellStage (0x40C650) | 1,777 B | Diagnostic frontier exists | Nonexact; see SECONDARY_SPELL_STAGE.md |
| UpdateSecondarySpellStageState (0x40C0A0) | 624 B | 624 B in the wide callback producer | Nonexact; signed `idiv` score rounding restored; 161/167 native instructions aligned |

Instruction alignment is a navigation aid, not exactness. The two stage
rendering owners have complete maintained C++ bodies and useful target
control-flow notes in STAGE_CULLING.md.

A second independently reviewed native copy of the 101-byte wrapped-angle
helper at 0x40B830 is now strict exact through natural C++ and its three
pi/tau DIR32 fields. Its real caller is 0x40ABF8; see SECONDARY_PAIR_MATH.md.

### Additional selected exact angle owner

`SecondaryAngleDifference40B830` at 0x40B830 is now cold-replayed
**101/101 bytes exact**, including all three π/2π DIR32 fields, with
separate complete natural C++ and reviewed native caller. The 31-byte unary cosine wrapper at 0x419660 is also strict exact in
natural C++ with its `__CIcos` REL32 and aligned live float local.
See SECONDARY_PAIR_MATH.md.

### Selected exact replay-coordinate owner

`SetSecondaryIntegerPlayerPosition` (0x435630) now has strict pinned VC9
`/GL` complete **110/110-byte** proof: 22/22 instructions and two independently
reviewed DIR32 fields. Integer coordinates are written before float conversion
and all eight item flags are updated. Only this owner and its directly affected
callers require selected replay; other large replay owners remain nonexact.
See `docs/SECONDARY_REPLAY_LIFECYCLE.md`.

### Exact wrapped-angle owner

A separate natural C++ owner at 0x4195F0 reproduces the original 101-byte
three-path wrapped-angle difference, with all 34 instructions and all three
independently bound pi/tau fields. It is native-byte-identical to the previously
exact 0x435480 angle-difference body. Selected strict cold replay passed;
see `docs/SECONDARY_PAIR_MATH.md`.

### Useful exact leaves

- SetSecondaryIntegerPlayerPosition (0x435630, 110 B) now passes selected
  strict cold linked replay: 22 instructions, two reviewed DIR32 bindings
  at +1/+1E, zero differing bytes. Natural C++ stores both player replay
  coordinates before x87 scaling; see docs/SECONDARY_REPLAY_LIFECYCLE.md.

- SetSecondaryPairDuration (0x419510, 4 B) and AddSecondaryPair
  (0x4195E0, 16 B) are natural C++ and passed selected strict cold replay.
  Evidence: .analysis/secondary-pair-duration-strict-20261118.log,
  .analysis/secondary-pair-add-strict-20261118.log, and
  .analysis/secondary-pair-full-source-exact-20261118.log.
- SecondarySpellDrawCallback (0x40C640, 7 B) passed strict cold linked-PE
  replay, including its REL32 field:
  .analysis/secondary-spell-draw-callback-cold-20261118.log.
- SecondarySpellUpdateCallback (0x40C630, 10 B) is now canonical strict
  linked exact in the wide callback producer: full 10 bytes and its sole REL32
  at +04 to 0x40C0A0 match under selected cold replay. The updater itself
  is nonexact; see `.analysis/secondary-spell-update-callback-strict-20261119.log`.
- PackStageFog (0x405B70, 171 B), ResetAnimationVm (0x401FD0, 303 B),
  and ResetAndBindStageAnimationVm (0x405B10, 30 B) all passed targeted
  strict cold linked-PE replay in this session with zero byte/relocation
  differences. Opcode 9's FISTP is round-toward-zero because native sets
  x87 RC=11b; keep the C++ float-to-int conversion truncating.

## Immediate next work

0. Continue the stage-spell initializer through its real caller. The opcode
   owner now calls `InitializeSecondarySpellStage` directly, and the focused
   `secondary-actor-instruction-adapter` graph links the initializer beside the
   16,260-byte instruction owner. Its difficulty dispatch now keeps cases 1 and
   2 separate, uses the native shared overlay tail, and delays the event-owner
   lookup to its use sites (134/468 aligned keys).
   The native caller uses EAX for the spell-name
   pointer plus two stack integers (`ret 8`); the candidate still uses three
   stack arguments (`ret 12`). Recover this boundary only through natural C++
   and whole-caller LTCG context; do not add register shims or ASM.
1. Continue CullStageObject from the indexed two-group traversal, inclusive
   viewport-edge tests, strict 0 < z <= 1 depth gate, and native asymmetric
   X/Y extrema branches. Unordered coordinates follow the native fallback
   into the minimum bound. The focused candidate is 1,648 B / 328 of 504
   aligned instructions. Its natural three-argument fastcall now models the
   caller's object/offset/distance registers; the native private EAX camera
   input remains open. Rebuild only cull-stage-object-frontier and work on
   the remaining stack/local and x87 ordering differences.
2. Rebuild draw-stage-layer-frontier when a shared source/header changes.
   Its candidate is 744 B / 112 of 191 aligned instructions after the culler
   ABI change and restoration of native scale-operand / flag / divide order.
   Preserve the active-camera reload, explicit float offset stores and
   full-width bit-11 extraction documented in STAGE_CULLING.md.
3. ExecuteStageScript now has per-mode oscillator timer advance/wrap and
   return paths, reaching 2,712 B / 577 of 610 aligned instructions after restoring
   per-opcode timer placement and double-angle x87 arithmetic. Continue its opcode/timer dataflow and EH/funclet work with
   only execute-stage-script-frontier. StepStageVectorTween (883 B, 278/280 instructions, remaining Y-addition order)
   and the 1,777-byte spell-stage initializer remain active alternatives. Avoid broad
   replay.
4. The previous SecondarySpellStageView::Update() method experiment did
   not improve the callback ABI: the candidate still saved ESI, not native
   EDI. It also promoted a physical view to a guessed class identity. Its
   three-file patch is retained at
   .analysis/secondary-spell-view-method-experiment-20261118.patch;
   the tracked sources were restored to the free-function form.

## Focused commands

    scripts/repo-python scripts/build-match-unit.py --unit cull-stage-object-frontier
    scripts/repo-python .analysis/compare-cull-stage-object-20261118.py
    scripts/repo-python scripts/build-match-unit.py --unit draw-stage-layer-frontier
    scripts/repo-python .analysis/compare-draw-stage-layer-20261118.py

Refresh local tracking/progress after a ledger change:

    scripts/repo-python scripts/validate-tracking.py --require-target
    scripts/repo-python scripts/progress.py
    git diff --check

For source changes, cold-replay only affected canonical exact units and
direct dependencies. Do not run the full cold replay. Keep source natural
C++; no inline ASM, embedded instruction bytes or padding. Commit meaningful
checkpoints with the gpt-web: prefix.

## Storage and retained evidence

See ARTIFACTS.md. The private target, VC9 toolchain and Wine prefix,
Ghidra project, configured linked-output directories, exact receipts,
current comparison scripts and useful .analysis evidence are retained.
Unreferenced compiler-generated source variants and duplicate manifest
snapshots from the 2026-11-18 exploration were pruned; the selected findings
and the one live experiment patch remain documented.
