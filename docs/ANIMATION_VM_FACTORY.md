# Animation VM factory and ID values

Four complete functions now have source in `src/AnimationVmFactory.cpp`.

| Function | Address | Target bytes | Current source | State |
| --- | --- | ---: | ---: | --- |
| PublishSceneAnimationVm | 4556E0 | 123 | 116 | Non-exact |
| CreateAnimationVmId | 455A00 | 112 | 112 | Exact |
| InitializeSceneAnimationVm | 456910 | 171 | 171 | Exact |
| BindAnimationFileScript | 44ADA0 | 191 | 191 | Exact |

Direct hash-attested Ghidra reviews cover each complete contiguous body, calls,
returns, neighbors and references. No embedded tables or external interior
references occur. These are reconstructed names and partial type views;
original authorship remains indeterminate.

## ID representation and caller context

The ID is modeled as a four-byte C++ value object with an unsigned member and
an unsigned-value constructor. Natural VC9 /GL compilation then reproduces
both the observed stack-by-value lookup argument and the hidden-result return
convention. Raw unsigned IDs and explicit output-pointer prototypes did not
recover that caller context. No dummy parameter or forced register ABI is used.
This supports the representation and calling behavior, not original type names.

The complete79-byte `FindAnimationVmById` in StageMesh.cpp now has a strict
primary-TU cold proof. Row factories return the same ID object; the mesh ID
array contains these four-byte values. Runtime+588 holds an AnimationFileView pointer.

## Factory chain

Creation conditionally locks critical section9 when runtime flags contain8000,
increments depth9, allocates a VM, sets flag40000000 and stores the requested
layer at+20. It initializes the VM, publishes it, then re-reads the shared flag
before leaving the critical section and decrementing depth9. Native addresses
are runtime base4C3280 plus flags590, critical section8E8 and depth939.

Initialization preserves the full signed script argument for binding while
storing its low16 bits at VM+3A2. It sets flag40000000, clears three separate
12-byte vectors at+3E8/+3DC/+3F4 and sets bytes+41D/+41C to10. Three fresh
zero-vector return values recover171 bytes; reusing one local produces133.

Binding checks the selected file script and bindingDisabled. On the valid path
it writes script/file IDs, clears flags600, stores file and script pointers,
sets the timer to-1, clears flag1, executes the VM and increments renderer+A0.
Unlike the existing44ACD0 binding routine, it does not call ResetAnimationVm.
Assigning fileId before the flags expression recovers the native load/store
schedule. The invalid path clears all434 bytes.

The member execution boundary405D60 and pool allocator4569C0 remain external.
The former receives renderer in ECX and VM on the stack, aligns the stack,
calls the44B4B0 interpreter and returns with RET4. Their declarations grant no
source or exact credit to those callees.

Publication links VM+4 into the renderer's first update list, updates its tail,
increments the ID counter while skipping zero, and writes/returns the ID. Its
116-byte candidate still differs from the complete123-byte target; counter and
return scheduling remain open. The current model uses ordinary unsigned data.

## Complete byte proofs

The three exact factory units share primary `src/AnimationVmFactory.cpp`, support
`probes/stage-mesh-context.cpp` and `src/StageMesh.cpp`, entry `StageMeshProbe@40`,
and output `build/match-linked/AnimationVmFactory`. Profile:
`/TP /O2 /Ob1 /Gy /MT /Isrc /EHsc /GL`.

Canonical units are `create-animation-vm-id`, `initialize-scene-animation-vm`
and `bind-animation-file-script`. Their exhaustive reviewed fields are11,1
and5 respectively: runtime-member references, lock IATs, real factory callees,
animation time scale, renderer, execution and memset. Cold pinned VC9 /GL
compilation, current receipts, complete public primary-TU PDB ownership,
PE/MAP/PDB identity and every field/byte agree with zero differences.
The three factory functions add474 exact bytes; lookup adds79 more.
