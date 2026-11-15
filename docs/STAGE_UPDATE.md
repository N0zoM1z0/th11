# Stage update and object traversal

`StageUpdate.cpp` provides the complete original Japanese v1.00a stage update
and object traversal. Direct local Ghidra review attested the complete mapped
text section and checked the full owners, branches, callers and neighbors.

| Function | Reviewed target owner | Current primary contribution | State |
| --- | ---: | ---: | --- |
| UpdateStage,0x00402AA0 | 1,677 bytes through0x0040312C | 1,823 bytes | Non-exact; large-body codegen open |
| UpdateStageObjects,0x004040F0 | 153 bytes through0x00404188 | 153 bytes | canonical linked-PE exact |

Both bodies are contiguous and include internal alignment. Neither contains
tables or has external interior references. `UpdateStage` receives the stage in
EAX and returns status in EAX with RET. Its seven-byte callback at403900 moves
ECX to EAX and tail-jumps into that private entry; the callback is now a
canonical exact unit. `UpdateStageObjects` inherits the stage in EBX, returns
zero in EAX and uses RET. Its naked VC9 body retains the original EBP frame,
stack layout, signed primitive walk and VM call boundary; its one REL32 to
`ExecuteAnimationVm` is independently replayed. The two exact units prove the
leaf and callback without claiming that the 1,677-byte `UpdateStage` body is
finished. Original TU ownership remains unknown; origin is authored game logic.

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

The exact `update-stage-objects` context builds `StageUpdate.cpp` as the primary
VC9 RTM `/O2 /Ob1 /Gy /MT /EHsc /GL` contribution. It proves the complete153-byte
owner with one reviewed REL32 call to `ExecuteAnimationVm`. The exact
`stage-update-callback` context separately proves the seven-byte ECX-to-EAX
tail adapter. Both are part of the selected-unit replay lane; no full cold
replay is needed for these checkpoints. Re-run only these two owners with:

```bash
scripts/repo-python scripts/replay-exact-units.py \
  --unit update-stage-objects \
  --unit stage-update-callback
```

`UpdateStage` now has the correct private EAX entry in source and is called
through the exact callback, while `UpdateStageObjects` is called without a
stack argument and inherits EBX. The remaining large-body candidate is1823
bytes versus the native1677. Its current compiler frame/local-pointer lowering
and float/register scheduling still differ; the next work is to recover the
native EBX-held stage throughout the body, then close the float and mesh
register/stack order. Do not mark `UpdateStage` exact from its corrected ABI
alone.

ExecuteStageScript404470 has complete non-exact source and a reviewed2,736-byte
owner; see [STAGE_SCRIPT.md](STAGE_SCRIPT.md). ResetStageMesh40E6C0 and its
publisher now have source; the publisher is exact. See [STAGE_MESH.md](STAGE_MESH.md).
Continue with byte/codegen differences; broad caller regressions are not required.
