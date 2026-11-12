# TH11 secondary host state leaves (2026-11-12)

Four previously unimplemented helpers around the 0x412E30 instruction
interpreter and the secondary master are now present in natural C++ source.
Direct mapped-text Ghidra review confirms each original complete span,
all local branches/returns, incoming callers and adjacent CC padding, with
no foreign interior references or embedded tables.

| Original | Source behavior | VC9 normal-COFF | Strict result |
| --- | --- | --- | --- |
| `0x4107B0` 38 bytes | Mark spell state `+0x8E0` with `0x10`, release animation ID `+0x884` and clear it | 38 bytes | **38/38 exact** |
| `0x410880` 28 bytes | Clear effect state `0x204` bytes and set `+0x2C` from target float constant `0x497FDC` | 32 bytes | Source only; private ESI receiver |
| `0x4108A0` 27 bytes | Copy a three-float vector to host state `+0x44C` | 27 bytes | Source only; private EAX source register |
| `0x410E60` 35 bytes | Set pause bit 2 on present update/draw chains at master `+8/+0xC` | 35 bytes | **35/35 exact** |

`ReleaseSecondaryHostSpellAnimation()` also supplies the existing missing
source declaration in `SecondaryActorInstructionControl.hpp`. It reuses the
already strictly matched 66-byte `ReleaseSecondaryAnimation(AnimationVmId)`
callee, rather than replacing the real VM tree release with a no-op. The new
standalone `release-secondary-host-spell-animation` unit has two fully
verified relocations: spell-global `0x4A8D6C` at +3 (`DIR32`), and exact VM
release `0x4562E0` at +22 (`REL32`). Locked VC9 RTM normal-COFF full-body
cold replay establishes a unique **38/38 byte exact match, zero differences**.

`PauseSecondaryMasterChains` retains the original repeated loads of the two
chain pointers, which permits VC9 to reproduce the EAX/ECX/EDX scheduling
with one native DIR32 at offset +1 to the master at `0x4A8D7C`. Full strict
35/35 comparison is accepted as a separate canonical unit.

`ResetSecondaryHostEffectSnapshot` preserves the physical `0x204`-byte
reset and readonly floating point constant, but source uses a conventional
parameter ABI; the original implicitly receives the effect state in ESI and
returns it in EAX. `CopySecondaryHostVector` is source-present with its own private register codegen gap.
`PauseSecondaryMasterChains` repeats both master node field reads naturally,
restoring the native `CMP [EAX+offset],0` (rather than the compiler-shortened
`TEST` on a saved pointer), branch lengths and EAX/ECX/EDX order. Its separate
`pause-secondary-master-chains` strict normal-COFF unit has a single DIR32 field
to master global `0x4A8D7C` at +1; cold replay verifies all **35/35** bytes.
Their original nominal class and translation-unit identity are not assumed.
The two remaining helper sources are not assigned any exactness credit.
