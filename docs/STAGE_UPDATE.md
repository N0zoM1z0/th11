# Stage update and object traversal

`StageUpdate.cpp` provides the complete original Japanese v1.00a stage update
and object traversal. Direct local Ghidra review attested the complete mapped
text section and checked the full owners, branches, callers and neighbors.

| Function | Reviewed target owner | Current primary contribution | State |
| --- | ---: | ---: | --- |
| UpdateStage,0x00402AA0 | 1,677 bytes through0x0040312C | 1,672 bytes (real-script-mesh frontier) | Non-exact; 395/443 keys aligned (2026-11-27, was 364) |
| UpdateStageObjects,0x004040F0 | 153 bytes through0x00404188 | 148 bytes | Natural C++ owner-pointer traversal; non-exact (33/51 instruction keys aligned) |

## 2026-11-27 distortion-mesh fixes

In `update-stage-real-script-mesh-callees` the boss is now 1,672 bytes with
395/443 instruction keys aligned (was 1,679 / 364):

- Mode 2 compares `radius > minimumRadius`; native loads the radius first.
- The vector normalized in mode 2 is the same function-scope `displacement`
  that mode 1 uses (native `esp+0x38`). Because its address goes to
  `D3DXVec3Normalize`, VC9 keeps it in memory, so mode 1 stores `offsetY` into
  `displacement.y` every iteration as native does. The mode-2 difference is a
  separate block-local `offset`, copied into `displacement`.
- Mode 1's column loop has the same `column = 0; if (column < columns) {
  ...; do { } while (column < columns); }` shape as mode 2; native has no
  second guard after the sine call. Mode 1 now aligns completely.

Remaining: in mode 2 native reloads the mesh pointer for the column guard of
each row, the `offset` to `displacement` copy is scheduled differently, the
three color-channel computations are ordered differently, and several stack
slots differ (frame layout).

Both target bodies are contiguous, include internal alignment and have no
embedded tables or external code-interior references. The native `UpdateStage`
entry receives the stage in EAX and returns status in EAX with RET. The
selected natural C++ LTCG context also emits an EAX-consuming entry: the
candidate begins by copying EAX into EBX, matching the native entry's live-in
register. The `StageUpdateProbe` is an ordinary C++ stack-argument wrapper; the
compiler materializes its argument in EAX before the boss call. Do not infer
the final private ABI from the source-level `__fastcall` decoration alone.

The seven-byte callback at403900 moves ECX to EAX and tail-jumps into the
native entry. Maintained `StageUpdateCallback` is an ordinary C++ forwarding
call rather than a naked adapter, so its separate pinned VC9 candidate is10
bytes and remains non-exact. The narrow real-script/mesh/tween frontier produced
a1679-byte boss candidate with336/443 instruction keys aligned. The expanded
`update-stage-real-interpreter-graph` adds the actual `ExecuteAnimationVm`
implementation and all of its natural source dependencies; that complete
context produces a1684-byte `UpdateStage` candidate with327/443 keys aligned.
The expanded graph is preferred for semantic coverage even though the narrower
context is five bytes smaller. Both remain diagnostic, not exact proofs.

The two wave phase locals are copied with `memcpy` so VC9 preserves their raw
float bit patterns through the native integer-register loads instead of
introducing x87 load/store round trips. The camera `updateOffset` vector is a
separate local from the mesh displacement vector; preserving both lifetimes
moves the candidate's camera scratch to the native stack slots and improves
instruction alignment. Wave mode overwrites displacement X/Y before use;
radial mode initializes and preserves its Z scratch across every vertex. The
radial inner loop is expressed as the native guarded do/while, so its
squared-radius value is computed only for nonempty rows and its signed column
count is tested at the same loop boundary. These source-level lifetime and
loop-shape changes preserve the observed rounding and update order. The
remaining differences are in local stack layout, color conversion register
allocation, mesh-loop scheduling and final float scheduling.

The native `UpdateStageObjects` owner receives its stage through private EBX,
returns zero in EAX and uses RET. Maintained source now expresses the complete
signed object/primitive walk as ordinary C++ with an explicit
`StageResourceView *` parameter. Its current pinned VC9 candidate is148 bytes
versus153 native, with33/51 instruction keys aligned. It reloads the object
count and object table at the observed loop boundary, preserves signed VM
indices and primitive byte strides, checks each VM's instruction pointer after
execution, and clears only the active flag when no VM remains live. The source
uses no private-register entry, naked function or inline assembly. Original TU
ownership remains unknown; origin is authored game logic.

The four `UpdateStage` frontiers carry20 explicit REL32 dependency bindings
for calls inside the native 1,677-byte owner: the D3DX normalize import thunk,
stage script, animation VM, mesh reset, CRT sine, angle wrapping and text RNG.
The real-mesh frontier compiles `StageMesh.cpp` as a support translation unit;
its complete 325-byte `ResetStageMesh` candidate begins with the same private
EAX-to-EBX handoff as the target and aligns100/104 instruction keys. The four
remaining keys are the two x87 axis-advance load/add pairs. Both boss callsites
call that real source body directly, without the anchor-only candidate's extra
stack pointer push.
The narrow real-script frontier also links `ExecuteStageScript`,
`AdvanceStageTimer`, both stage tween implementations, `ResetAnimationVm`,
`ReleaseVmBindingBlock`, `RandomNext32` and other actual support owners.

The full interpreter frontier adds `AnimationExecution.cpp`,
`AnimationExecutionArguments.cpp` and the actual tween, drawing, fan, mesh,
renderer and secondary-animation support sources. It links the complete
11,620-byte `ExecuteAnimationVm` contribution from its natural C++ source,
along with `ReadAnimationFloat` (544 bytes),
`ReadAnimationInteger` (219 bytes), `RandomNext32` (153 bytes),
`ResetStageMesh` (325 bytes) and `ExecuteStageScript` (2,560 bytes). The
interpreter candidate aligns703/3,308 instruction keys; it has264 calls versus
262 native and380 conditional branches versus406. Its prologue reserves an
E8-byte frame without the native eight-byte ESP alignment and keeps the VM in
EBX instead of EDI. These are substantial remaining boss differences, not
exactness claims. `RandomNext32` in this graph has the same153-byte extent and
its two remaining instruction-key differences are the linked import-thunk
addresses; its dedicated canonical unit remains cold-replayed exact.

The full graph's `UpdateStage` candidate is1684 bytes with327/443 instruction
keys aligned; the narrower real-script frontier is1679 bytes with336/443.
The expanded graph is retained because it contains the real interpreter call
chain rather than a target-address substitute. The candidate's final extra
angle-wrap call lies beyond the native owner end and is intentionally not
replayed as a target field. The D3DX import thunk and CRT routine retain their
separately recorded provenance and acceptance limits.

## Update order

Flag mask08 returns1 before any writes. Mask04 together with signed timer
current at least60 does the same. Otherwise update clears camera offsets+E8/+EC
and camera vector+F0, then normalizes camera targetOffset+0C into the observed
vector+24. That destination is not the eyeOffset+3C field. The normalized value
is computed before scripts and VMs can modify the camera. Stage tint becomes
00808080.

Unless mask04 is set with timer at least30, update traverses stage objects,
executes the stage script, and executes all eight embedded VMs at stage+180.
These are unconditional VM calls, unlike the draw callback's activation gate.
If accent+232C is nonzero after those calls, update saves the current global
animation time scale, writes1, executes three VMs at stage+2348/+277C/+2BB0,
then restores the saved value. It writes1 only before the first extra VM, so
changes made during that group remain visible to the later calls. It saves
the scale after the first eight VMs, not at function entry.

Accent is cleared even when VM/script dispatch was skipped. All0x118 stage
camera bytes are then copied to runtime+434. A null binding block skips mesh
work. Finally the stage age+300C increments with32-bit wrap and update returns1.

## Grid and arithmetic

The binding block has an additional observed mesh view: signed row/column
counts at+0/+4, IDs+8, index storage+C, RenderVertex array+10, CameraVector
positions+14. No shared binding-header definitions were changed. Stage fields
+3014/+3018 are minimum radius/current radius, +3020/+3024 are phases and
+3028 selects distortion. All class offsets and sizes remain unchanged.

The position pointer and phase values are captured before ResetStageMesh.
The vertex pointer is acquired afterward. Loop bounds reload the current
binding block at the observed boundaries. Counts, VM indices and primitive
strides remain signed; there are no added bounds checks or allocation guards.

Mode1 calls ResetStageMesh with `(-192,0,384,448)`. It sets alpha C0 on every
visited vertex. One sine of phaseY supplies each nonempty row's Y offset,
and a sine of phaseX supplies each vertex's X offset; both scale by12. Only
vertices strictly inside all four grid edges move. Their vertex Z and source
position Z become zero. PhaseX advances by float-rounded40 degrees after
every vertex and continues across rows; phaseY decreases by float-rounded72
degrees after every row, including rows with no columns. Final stored phases
advance independently by float-rounded pi/64 and pi/80.

Mode2 captures the old radius, decrements the stored radius by2 only when
minimumRadius is less than current radius, and uses the captured radius for
the entire update. Reset arguments are `(-r,224-r,2*r,2*r)`. The radial center
used afterward is `(224,240)`. A scratch displacement Z starts at zero and
becomes `sourceZ - previousDisplacementZ` for every visited point, including
points outside the radius. Preserve that traversal dependency; do not reset Z
per vertex or row. A separate copy is normalized for radial displacement.

The squared XY distance is rounded to float before subtraction from the
double radius square; the remaining distance is rounded again. The inside
test is `remaining >= 0`, so unordered/NaN values take the outside path.
Inside points divide by the radius square, reset color toFFFFFFFF then alpha
60, and apply the original three color conversions even though finite factors
leave RGB white. Zero radius can produce NaN conversions; do not fold that
arithmetic away. The normalized displacement scales by factor*32 and receives
sine perturbations of factor*8 in X/Y. Vertex/source Z then become zero.
Outside points only have alpha cleared. Phases advance after every point by
float-rounded pi/2 and minus40 degrees.

The final X phase advances by pi/64. RandomNext32 on the existing text RNG
root4C2F00 supplies the Y phase jitter. Unsigned conversion and scaling by
2^-32 are rounded to float; a second rounded step combines random*pi/40 with
pi/80 before adding the existing Y phase. The source preserves these distinct
rounding points. NormalizeAngle is called at the original points.

Constants were read independently from the verified target. Its x87 sine
boundary486CF0 consumes/returns ST0, with no ECX parameter. Inspection of its
486D48 implementation found FSIN; the decompiler's fastcall guess is wrong.
Natural C++ `sin(double)` plus a float result cast emits the VC9 `_CIsin`
intrinsic boundary. This is a CRT dependency, not a new game function mapping.

## Object traversal

For each signed objectCount, an active object tests flags byte+3 mask01 and
starts at its first primitive+1C. A negative signed16 primitive kind terminates
the stream. Each nonnegative kind supplies a signed16 VM index+6; traversal
executes the VM at `vms + index*0x434` and counts nonzero VM instruction
pointers+3A8 after execution. It advances by the primitive's signed16 byte
size+2. If no VM remains live, it clears only active mask01 in the captured
object. An initially terminated stream clears the flag without a VM call.
Object count and table are reloaded while advancing the outer loop. Negative
indices/strides and null payloads are not sanitized.

## Code generation and remaining work

The anchor-only `update-stage` frontier builds `StageUpdate.cpp` as a primary
VC9 RTM `/O2 /Oy- /Ob1 /Gy /MT /EHsc /GL` contribution. Its linked image
contains the 1,683-byte `UpdateStage` candidate and the 148-byte natural C++
`UpdateStageObjects` candidate (native153; 33/51 instruction keys aligned).
The `update-stage-real-script-mesh-callees` frontier adds the real stage script,
tween, timer, mesh, VM and binding source graph; its boss candidate is1679 bytes
with336/443 keys aligned. The full
`update-stage-real-interpreter-graph` frontier links the complete
`ExecuteAnimationVm` C++ source and its argument-reader/rendering dependencies;
its boss is1684 bytes with327/443 keys aligned and its interpreter is11620 bytes
versus12056 native (704/3308 instruction keys aligned). The 325-byte `ResetStageMesh` and 2,560-byte
`ExecuteStageScript` bodies are linked from their real C++ owners, not
hand-written adapters or target-byte stubs.

The former exact units for the object walker and update callback were retired
when their maintained source was converted from private-register/naked
implementations to ordinary C++. The object walk is ordinary signed C++ with
an explicit stage pointer; LTCG naturally keeps the stage in EBX in the
selected context. The boss source remains ordinary C++ with no private-register
entry or inline assembly. The native boss receives EAX; the selected LTCG
candidate also receives EAX even though the source declaration is
`__fastcall`. A separate callback frontier shows the natural forwarding
callback is still10 bytes versus the native seven-byte adapter.

The next work is to improve the interpreter's native frame/alignment and
EDI VM lifetime, recover its missing call/branch structure, and close the
seven-byte `UpdateStage` gap. The smaller leaf target remains
`ResetStageMesh`: its 325-byte candidate still has four x87 instruction-key
differences in the two coordinate increments. `RandomNext32` is already exact
in its dedicated canonical unit. Do not mark `UpdateStage` or
`ExecuteAnimationVm` exact from size or partial instruction alignment alone.

Rebuild the diagnostic contexts selectively with:

```bash
scripts/repo-python scripts/build-match-unit.py --unit update-stage
scripts/repo-python scripts/build-match-unit.py --unit update-stage-real-mesh-callee
scripts/repo-python scripts/build-match-unit.py --unit update-stage-real-script-mesh-callees
scripts/repo-python scripts/build-match-unit.py --unit update-stage-real-interpreter-graph
scripts/repo-python scripts/build-match-unit.py --unit reset-stage-mesh
```

ExecuteStageScript404470 has complete non-exact source and a reviewed2,736-byte
owner; see [STAGE_SCRIPT.md](STAGE_SCRIPT.md). ResetStageMesh40E6C0 and its
publisher now have source; the publisher is exact. See [STAGE_MESH.md](STAGE_MESH.md).
Continue with byte/codegen differences; broad caller regressions are not required.
