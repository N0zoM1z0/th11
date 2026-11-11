# Secondary player animation-pair and controller reset

The complete native TH11 controller animation-clear owner is
`0x42F580..0x42F69D` (286 bytes). Ghidra reports two body ranges separated
by a three-byte alignment instruction at `42F59D`: all 97 instructions,
286 bytes, one plain `RET`, no callees or jump tables were checked against
the pinned original executable. Its one caller at `42FFE2` is part of
the complete 304-byte controller reset at `42FEC0..0x42FFEF`, also now
source-present. This parent has 65 original instructions, four native
callees and one plain return.

The maintained code in `src/SecondaryPlayerAnimationReset.cpp` uses
ordinary C++ typed arguments. The native optimized child receives
the controller in **EBX**, scans eight 0xE4-stride entries from
`controller+0x7624` and propagates event 2 to each pair of animation VM
IDs and their related nodes, using the two render-batch lists.
It clears controller flags bit 3 on entry and zeroes +8BC8 on exit.
The `ids[-1]` / `ids[0]` pointer walk, rather than indexed structure
access, naturally emits the original byte ordering. Its only
absolute-address field points to the render-batch singleton at
`0x004C3268`.

The 304-byte parent resets its three timers and flags, clears firing
state, releases the focused animation, updates the lives display,
then invokes the animation pair reset and emitter reset in native
order. The parent is **source-present but non-exact**: an initial
pinned scoped VC9 `/GL` build emits 319 bytes versus native 304.
The child in that same naturally linked context emits 286 bytes and
differs only at the singleton address field before rebinding.
The cold-pinned linked Oracle `secondary-player-animation-pairs-reset`
has now replayed that reviewed DIR32 field at offset 9 against
`g_RenderBatch` (target `0x004C3268`). The complete **286/286 bytes are
exact**, with one unique public primary-TU PDB code owner and full
source/support/toolchain/PE/MAP/PDB receipt. The 304-byte caller still
does not match and remains open.

Local evidence: `.analysis/pause-next-big-20261111-review.txt`,
`.analysis/secondary-player-pairs-shifted-compare.json`, and
`build/probe-ltcg/SecondaryPlayerAnimationResetContext/`.
