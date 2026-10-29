# Mesh effect initialization and VM deformation

Six complete functions now have source in `src/MeshEffects.cpp`, covering2,125
reviewed target bytes. The original gameplay class names and provenance remain
indeterminate. Direct local hash-attested Ghidra reviews cover full contiguous
bodies, references and neighbors; the998-byte function's final instructions
were independently decoded beyond the query's250-instruction display limit.
There are no embedded tables or external interior references in these owners.

| Function | Address | Target bytes | Current source | State |
| --- | --- | ---: | ---: | --- |
| UpdateMeshEffectSpellState | 4068C0 | 91 | 91 | Exact |
| InitializeColumnMeshEffect | 406920 | 998 | 998 | Non-exact; EH |
| InitializeSquareMeshEffect | 407BB0 | 342 | 343 | Non-exact; EH |
| SquareMeshVmCallback | 408070 | 398 | 398 | Exact |
| QueueMeshEffectSound | 44A1E0 | 125 | 126 | Non-exact |
| CreatePositionedAnimationVmId | 455B10 | 171 | 169 | Non-exact |

These are complete compiler contributions and address-normalized diagnostics,
not cropped comparisons. The two initializers have real C++ allocation cleanup;
the linked Oracle's specialized EH acceptance remains open.

## Physical storage and initialization

The controller view has ID+40, mesh pointer+480, origin vector+484, and scalar
parameters+490/+494. Its actor dependency supplies file+10, position+87C and
an existing timer at+8BB0. Globals bind to4A8EB4 (actor),4A8D6C (status),4A8D64
(active-state dependency),4A8D7C (event counter), and selections4A5710/4A5714.
These are partial views, not recovered complete original objects.

Column initialization sets32/10, copies the actor position and subtracts32 from
Y, then creates script2B at layer16. A separate origin shifts X by-60 and Y by64.
It releases the captured old mesh pointer before deleting that same pointer,
then creates four rows with `(int(origin.y)+32)/32+2` columns and alternate mode1.
Reset receives X=shifted origin X, Y=0, width120 and height=shifted Y+32.

For each column, four RNG calls perturb the four rows' position X values by a
signed unit sample scaled by4. The two inner vertex rows receiveFFFF8080 until
the final column, which receivesFFFFFFFF. The function updates the status flags,
sets the actor timer's previous/current/fraction to119/120/120, increments the
event counter and returns0. Position and vertex buffers retain distinct roles.

Square initialization sets the same32/10 scalars, copies the actor position,
queues sound31, creates script25 at layer16, resolves its ID and installs the
408070 callback at VM+428. It replaces the old mesh with13 by13 normal-mode
storage, resets around origin-17 with width/height34, calls the status updater,
increments the event counter and returns0. VM+428 is distinct from the mesh's
existing publication callback at+414.

## Deformation, ID creation and sound queue

The VM callback computes phase/(pointCount-1). For each segment it makes four
RNG calls and perturbs X/Y of two consecutive vertices by samples scaled by8.
It closes the strip by copying its first two complete28-byte vertices into the
last pair, with both final V coordinates set to the preceding V plus the phase
increment, then returns0. Capturing each vertex's Y address before the X sample
restores the native x87 load/add schedule and closes all398 bytes.

RNG sampling first uses the compiler's unsigned-to-float conversion, then
`sample*2^-31-1`, a float rounding point, and the scale with another float
rounding point. A double parameter in the inlined helper introduced an unwanted
8-byte stack alignment. A constant template parameter removes it. An inline
coordinate updater reduces callback differences22 to12. Capturing the Y
address before each X sample removes the final12; postincrement alone did not. The standalone RNG probe keeps input states
independent so LTCG does not specialize every call to the one global state.

Positioned ID creation locks section9 conditionally, allocates/flags the VM,
stores layer and position at+3E8 (X+32+192, Y+16, unchanged Z), then calls the
existing44ACD0 reset/bind routine and publishes the ID. Shared lock flags are
re-read at exit. Its169-byte source differs from the171-byte owner in save/restore
and hidden-result placement. Modeling it as a file-view member retained the
same169/101 diagnostic gap and was not kept.

The sound queue preloads a signed16-bit setting from its8-byte record table,
then searches twelve slots. An existing slot appends a zero frame while its
pending count is below128. A new slot records the sound/setting, clears its
first frame and increments its existing pending count. The partial queue at
4C44A0 has slots+0, pending counts+30 and frame rows+60; records start4A34F6 and
settings4C4288. Register allocation, loop form and store scheduling remain open.

## Exact proofs and current context

When status flag1 is set, age>=60 clears value+8E4 and flags22. Otherwise, an
active dependency and `selection1+3*selection0 != 5` set flag20. Other paths
leave status unchanged. The separate91-byte body has four reviewed absolute
bindings: status pointer4A8D6C, active pointer4A8D64, and the two selections.

The canonical unit is `update-mesh-effect-spell-state`, with MeshEffects.cpp
primary, `probes/mesh-effects-context.cpp` and real mesh/VM/factory/RNG/binding
sources as support. Entry is `MeshEffectsProbe@40`; profile is
`/TP /O2 /Ob1 /Gy /MT /Isrc /EHsc /GL`. Cold pinned VC9 /GL proof verifies
the complete91-byte public primary-TU PDB contribution and all four fields.
`square-mesh-vm-callback` shares that family and verifies the complete398-byte
public contribution and24 RNG/call/float-constant bindings. Current receipts,
PE/MAP/PDB identity, ownership and every byte agree. The handoff checkpoint
cold-replayed only these two units, both passing.
The context also supplies independent positioned-factory, sound and RNG inputs.
No behavioral/failure/random regression suite is part of this work.

## Current column experiment

The maintained column initializer now samples RNG before selecting each row's
position address. The prior coordinate-reference form selected the destination
before RNG; it produced1,022 bytes/498 diagnostic differences. The promoted
sample-first form produces the complete998-byte owner with269 diagnostic
differences in the current canonical family. Sampling is explicit and the
conversion/rounding/scaling helper remains natural C++. Four samples still
perturb the four row X values in their original order.

Current evidence is `.analysis/column-promoted-diagnostic.json`, read from the
cold-rebuilt MeshEffects family. An earlier sampling-order probe independently reached the same result; its
diagnostic record is retained and the unused code copy is removed.
This is a codegen improvement, not exact credit. The remaining differences
include a20-byte versus native24-byte local frame and positioned-ID
argument/return placement. Initializer EH ownership remains open.

Only the directly affected `update-mesh-effect-spell-state` and
`square-mesh-vm-callback` exact units were cold-replayed for this source change;
both remain exact. The complete998-byte initializer was inspected from the same
cold output; no full-cohort replay or behavioral regression suite was run.
