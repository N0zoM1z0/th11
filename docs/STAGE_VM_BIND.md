# Stage VM reset/bind adapter: complete 30-byte exact function

The original Japanese TH11 v1.00a `0x00405B10..0x00405B2D` is a complete,
contiguous 30-byte native function. Its caller belongs to the large script
interpreter at `0x0040C650` and has repeated call sites including
`0x0040CC2B`, `0x0040CC44` and `0x0040CC75`.

The routine receives the VM in EAX, the animation document in EDI and the
script identifier in EBX under VC9 private LTCG conventions. It preserves the
VM through ESI, calls `ResetAnimationVm` (`0x00401FD0`), writes byte `0x10`
to VM offsets `+0x41D` and `+0x41C` in that order, then restores EAX and
ends with an ordinary tail-jump to `BindAnimationScript` (`0x0044ACD0`).
There is no other callee or embedded data and two subsequent bytes `CC` are
outside the function owner. The source in `src/StageAnimationVmBind.cpp`
expresses this behavior in natural C++ with an ordinary public stack-argument
signature; no inline ASM, hard-coded object bytes, register forcing or
padding is used.

The scoped linked producer `reset-and-bind-stage-animation-vm` compiles the
source with pinned VC9 RTM `/GL` and includes the real `src/AnimationVm.cpp`
callee family plus a dynamic caller probe. This produces the native private
EAX/EDI/EBX call shape, ESI save and tail jump automatically. The full
30-byte primary-translation-unit PE/MAP/PDB owner, producer receipts, both
original REL32 linkages, and all remaining code bytes are checked:

```bash
scripts/repo-python scripts/replay-exact-units.py --unit reset-and-bind-stage-animation-vm
```

Result: **30/30 bytes exact**, with zero differences after independently
reviewed REL32 fields at `+0x04 -> 0x00401FD0` and
`+0x1A -> 0x0044ACD0`. This independent proof does not grant exactness to
the large caller at `0x0040C650` or establish the original authorship or
source translation-unit partition.
