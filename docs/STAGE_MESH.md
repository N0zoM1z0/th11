# Stage mesh code and exact publication

Seven complete functions now have source in `src/StageMesh.cpp`.

| Function | Address | Target | Source | State |
| --- | --- | ---: | ---: | --- |
| StageMeshVmCallback | 40E520 | 16 | 16 | Exact |
| StageScriptMeshConstructor | 40E530 | 389 | 369 | Non-exact |
| ResetStageMesh | 40E6C0 | 325 | 325 | Non-exact |
| PublishStageMesh | 40E890 | 123 | 123 | Exact |
| CreateStageMeshRow | 42B3D0 | 168 | 168 | Exact |
| CreateAlternateStageMeshRow | 42B480 | 170 | 170 | Exact |
| FindAnimationVmById | 4561E0 | 79 | 79 | Exact |

Direct local, hash-attested Ghidra reviews cover full bodies, references,
neighbors and external call contracts. Constructor389 includes7 internal
alignment bytes and publisher123 includes3. No embedded tables or external
interior references occur. Callback40E520 was absent from the initial inventory;
explicit16-byte span review adds one candidate. Original provenance remains
indeterminate; role/layout alone does not establish authorship.

## Recovered layout and operations

The24-byte mesh stores signed rows/columns, row IDs, row VM pointers,28-byte
vertices and12-byte positions. The old `indices` label at+0C was incorrect;
`StageMeshView::rowVms` now expresses the reviewed pointer array.

Construction checks runtime+1AC. Disabled construction clears all six fields.
Enabled construction allocates4*rows-1 bytes for each row array, then
rows*columns*28 and rows*columns*12 bytes for the grid. Preserve these observed
sizes. It creates rows-1 strip VMs, looks them up, installs callback40E520 and
mesh owner+418, and clears VM flags mask70. Normal and alternate creation use
runtime file+588 with script/layer51/1B and52/1C respectively. Row helpers allocate
2*columns vertices; normal small-column handling retains the previous point
count, while alternate handling records columns. Their flag/store ordering is
distinct. Lookup returns null for zero ID and searches both renderer lists in
order. Scene creation now has source in [ANIMATION_VM_FACTORY.md](ANIMATION_VM_FACTORY.md).
Allocation remains external.

Reset uses X+192+32 and Y+16 origins. Width divides by rows-1 and height by
columns-1, with float stores at the observed boundaries. Outer rows advance X;
inner columns advance Y. Vertices copy the position tuple, receive white color
and rhw1, and use X/640 and Y/480 texture coordinates clamped only below zero.
It then publishes the grid. Publication copies alternating vertices from each
pair of adjacent rows into that row VM's buffer. The callback republishes via
VM+418 and returns zero.

## Exact proofs and open differences

`publish-stage-mesh` and `stage-mesh-vm-callback` are complete non-EH public
primary-TU PDB contributions. Publisher has no fields; callback has one REL32
at+8 to40E890. Cold pinned VC9 /GL receipts, PE/MAP/PDB identity and ownership,
full extents, exhaustive field replay and all bytes agree.

```bash
scripts/repo-python scripts/replay-exact-units.py \
  --unit publish-stage-mesh --unit stage-mesh-vm-callback \
  --unit find-animation-vm-by-id --unit create-stage-mesh-row \
  --unit create-alternate-stage-mesh-row
```

The mesh family uses primary `src/StageMesh.cpp`, support
`probes/stage-mesh-context.cpp` and `src/AnimationVmFactory.cpp`, entry
`StageMeshProbe@40`, output `build/match-linked/StageMesh`, and
`/TP /O2 /Ob1 /Gy /MT /Isrc /EHsc /GL`. The complete79-byte lookup now has
a strict primary-TU cold proof with no address fields. Both row helpers now
also have full168/170-byte primary-TU cold proofs with five fields each. All
five mesh exact units total556 bytes. The current checkpoint cold-replays
only three new and ten affected exact units across five families; all pass.
Totals are126 mappings and64 exact functions /12,675 bytes.

The four-byte AnimationVmId value object recovers the native stack ID argument
and genuine hidden-result factory returns. Raw unsigned IDs and explicit
output-pointer declarations did not recover that context. The current
constructor remains non-exact against389. Row initialization now assigns Z,
rhw and color, then increments the vertex pointer. VC9 schedules color/Z/rhw
and anchors the induction pointer at rhw, exactly as native. This gives full
168/170-byte exact rows; five fields each bind runtime+588, ID creation,
renderer, ID lookup and malloc.

A reference to the row ID, or an inline resolver taking that reference, gives
a389-byte constructor but changes its private ABI to RET12 and retains232
diagnostic differences. Using the columns member gives381 bytes/277. Those
variants were not kept; the current constructor is369 bytes/276. Two complete
real callers now have source in [MESH_EFFECTS.md](MESH_EFFECTS.md), including the
998-byte column initializer. Adding their real call graph alone retains369.
No register ABI or padding is imposed.

Reset's scalar coordinate writes produced358 bytes; tuple-prefix copying
restores325. Separating rounded originY and the two axis-step values reduces
address-normalized diagnostic differences from75 to11 at+243..248 and+279..283.
Native loads the position then adds a cached step; the candidate duplicates
the step and adds the memory position. Both complete owners remain325 bytes.
Casts, reversed additions, inline helpers, parameter forms and loop rewrites
retain11 differences. Step arrays/pairs give41; double locals and accumulator
variants are worse. `/fp:except` gives329 bytes and `/fp:fast`352; neither is
retained. Continue from the default profile and current11-difference source.
