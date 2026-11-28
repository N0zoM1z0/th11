# TH11 secondary auxiliary owner lifecycle (2026-11-12)

Three contiguous, previously source-missing owners form a separate 0x24-byte
auxiliary feature at native singleton **0x4A8D74**. This is not the replay
file browser at 0x4A8EC0 or the leaderboard at 0x4A8EBC. The original
class/TU attribution remains indeterminate. Maintained source lives in
`SecondaryAuxiliaryOwner.cpp/.hpp`, including its chain-node construction,
renderer document, conditional locks, and genuine failure branches.

All three owners are canonical exact (2026-11-28) in one linked `/GL`
context, `build/match-linked/SecondaryAuxiliaryOwner`: units
`initialize-secondary-auxiliary-owner`, `destroy-secondary-auxiliary-owner`
and `create-secondary-auxiliary-owner`. The context links the real
`ChainRegistration.cpp`, `ChainRemove.cpp`, `AnimationDocument.cpp` and
`LogBuffer.cpp`; the probe `probes/secondary-auxiliary-owner-context.cpp`
(rewritten on 2026-11-28; no other unit used it) creates the owner, calls the
chain helpers and the document loader with generic arguments and destroys the
owner.

Two changes closed the gaps recorded below:

- The destructor's swapped `MOV ECX,ESI` / `MOV EDX,EDI` was seen in a
  context without `ChainRemove.cpp`, where `UnregisterChainElement` is an
  anchor with the plain stack convention. The current context links its real
  source, and the destructor matches without any source change (no control
  build without it was made).
- The constructor is `memset(this, 0, sizeof(*this))`, `flags |= 2` and the
  singleton publication, all inside the constructor that `new` runs only on a
  successful allocation. The earlier word loop and the flag/publication after
  `new` gave 97-98 bytes. The constructor is defined in the .cpp so that the
  header carries only its declaration.

The 0x424970 laser manager initializer has the same shape (see [LASER](LASER.md)).

Historical state before these changes:

| Original owner | Native size | Current scoped VC9 diagnostic | State |
| --- | ---: | ---: | --- |
| `0x40E0A0` resource/chain setup | 238 | 217–234 bytes, all six native calls | Source-present, nonexact |
| `0x40E190` double-chain teardown | 176 | **176 bytes, 47 instructions, six calls; four non-field differences** | Source-present, nonexact |
| `0x40E240` allocation/startup | 95 | 97–98 bytes, four native calls | Source-present, nonexact |

The 238-byte setup acquires animation document slot 6 from the original
resource name, publishes the document in owner `+0x10`, and logs a real
failure string if acquisition fails. On success it allocates two actual
0x24-byte chain nodes, registers update priority 23 and draw priority 31,
sets their original callback addresses at `0x40E300/0x40E310` and publishes
the owned pointers at `+8/+0xC`. The 176-byte destructor unregisters both
chains under independently guarded runtime critical-section pairs and resets
the singleton; it does not free the renderer document independently. The
95-byte entry allocates nine DWORDs, initializes the zero state, sets its
startup flag, publishes the singleton and cleans up on failed setup.

Direct full-body original Ghidra reviews confirm all three function
boundaries, calls and following padding. Pinned VC9 normal-COFF and scoped
`/GL` matrix (`probes/secondary-auxiliary-owner-context.cpp` plus genuine
`ChainRegistration.cpp`, document and logger producers) compile all three.
The destructor has the correct full 176-byte instruction layout; only the
ordering of `MOV ECX,ESI` and `MOV EDX,EDI` before its two real
`UnregisterChainElement` calls differs, resulting in four non-address bytes.
No instruction patches, clipped bounds or masked-difference exceptions are
used, and no new exactness is credited. The remaining work is original
private argument-move scheduling and separate caller context analysis.
