# Stage mesh code and exact publication

Seven complete functions now have source in `src/StageMesh.cpp`.

| Function | Address | Target | Source | State |
| --- | --- | ---: | ---: | --- |
| StageMeshVmCallback | 40E520 | 16 | 16 | Exact |
| StageScriptMeshConstructor | 40E530 | 389 | 362 | Non-exact |
| ResetStageMesh | 40E6C0 | 325 | 325 | Non-exact |
| PublishStageMesh | 40E890 | 123 | 123 | Exact |
| CreateStageMeshRow | 42B3D0 | 168 | 162 | Non-exact |
| CreateAlternateStageMeshRow | 42B480 | 170 | 164 | Non-exact |
| FindAnimationVmById | 4561E0 | 79 | 73 | Non-exact |

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
order. Allocation and scene creation remain external; do not add validation
or change original control flow merely to simplify source.

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
  --unit publish-stage-mesh --unit stage-mesh-vm-callback
```

Both use primary `src/StageMesh.cpp`, support `probes/stage-mesh-context.cpp`,
entry `StageMeshProbe@40`, output `build/match-linked/StageMesh`, and
`/TP /O2 /Ob1 /Gy /MT /Isrc /EHsc /GL`. The five existing units whose declared
header/context changed also pass focused cold replay. No unrelated exact unit
was replayed. Totals are116 source mappings and57 exact/11,693 bytes.

Reset's scalar coordinate writes produced358 bytes. Copying the12-byte tuple
as an aggregate restores325 bytes; temporary-slot scheduling still leaves75
address-normalized diagnostic differences. Assigning the rounded Y origin back
to the input parameter gave79 and was superseded. Constructor and row helpers
retain private parameter/register gaps; lookup uses candidate ECX ID versus
native stack ID. These functions receive no masked-match credit. Continue with
those concrete compiler/caller-context questions rather than broad behavioral
or failure matrices.
