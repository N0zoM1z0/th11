# Secondary HUD front-layer draw owner

The native owner at 0x41C5F0 is 105 bytes/33 instructions, with one
incoming callback at 0x41CFC3 and one callee family, DrawAnimationVmByMode
at 0x451EF0. Direct Ghidra review and full native PE decode found no
foreign interior references and confirmed the following CC boundary.

The natural C++ owner at src/SecondaryHudFrontDraw.cpp walks all nine
primary VMs with the physical 0x434 stride, then queries secondary master
slot zero. If the actor exists, the two independent original gates require
bit 5 and bit 0 to be clear before drawing the indicator VM at +0x3F1C.
Every path returns one. The actor flag test is expressed as two complement/
shift predicates, retaining the native SHR/NOT/TEST sequence.

Pinned VC9 /GL emits a complete 105-byte/33-instruction primary-PDB
owner whose instruction sequence matches the native. Its five external
fields are the two renderer global loads, the master singleton load and
the two calls to DrawAnimationVmByMode. This source contains no assembly
or artificial padding. The adjacent 10-byte callback is a separate owner at 0x41CFC0. Its
ordinary fastcall source uses a volatile function-pointer consumer so VC9
retains the exact native ECX-to-EBX handoff. The callback is independently
cold-replayed at 10/10 bytes with its sole REL32 to this exact body.
