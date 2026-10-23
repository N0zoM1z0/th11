# Keyboard and joystick polling candidates

Maintained source now covers two larger input functions:

| Function | Complete target extent | Source/codegen state |
| --- | --- | --- |
| MergeJoystickInput | `0x00457270..0x0045759B`, 812 bytes | maintained source; normal VC9 candidate 743 bytes; non-exact |
| PollKeyboardInput | `0x004576B0..0x00457A27`, 888 bytes | maintained source; normal and simple LTCG candidates 891 bytes; non-exact |

Direct Ghidra review found contiguous bodies, no external interior references,
complete RET tails, and following CC padding. Cookie-aware temporary type
hypotheses recovered return flow that default decompilation was deleting:

```bash
scripts/repo-python scripts/ghidra.py decompile .analysis/input.c \
  cookie@0x0045FC2C u32-fast1@0x00457270 u32@0x004576B0
```

The successful cookie body at 0x0045FC2C compares ECX, returns without changing
EAX, and otherwise jumps to the failure handler. The keyboard caller explicitly
passes its mask in ECX to the joystick merge and consumes its EAX result.
Metadata hypotheses are rolled back; target bytes and saved database types are
unchanged.

## Keyboard path

The gate at 0x004C3D94 controls keyboard polling; its broader owner/meaning is
not established. Shared flags at 0x004C3810 select virtual keys versus DirectInput
scan codes with bit 0x400. The source preserves both separate key maps, arrow
and numeric-pad combinations, diagonal numeric-pad inputs, modifier/action
keys, and Acquire on DirectInput keyboard errors. It then always merges
joystick input, publishes previous/current masks, calls the exact repeat/edge
routine, and returns the merged mask. The gate does not suppress that merge.

## Joystick path

Shared flag bit 0x800 selects DirectInput; the other path uses joyGetPosEx with
a zeroed JOYINFOEX, size 0x34, and flags0xFF. Five signed-short bindings map fire,
bomb, slow, pause, and skip. Negative mappings are ignored. WinMM axes compare
strictly against midpoint plus/minus one quarter of each configured range.
DirectInput reads a zeroed 0x110-byte state, uses buttons at +0x30, and compares
signed X/Y positions against the signed-short thresholds at 0x004C345E/60.

On a failed DirectInput Poll, Acquire is attempted; input-lost results permit
up to 400 additional Acquire calls, each followed by the observed hook at
0x00459BC0. The merge preserves the existing keyboard mask on this path,
including successful reacquisition; a device-state read happens on a later
poll. The hook's target body is RET and preserves the Acquire HRESULT in EAX;
its source remains an external declaration and its production owner is open.

## Remaining exact work

The keyboard candidate's virtual-key mapping region now aligns with the target,
but its DirectInput error block layout and result-publication scheduling differ.
The size delta alone is not an exactness claim: current raw byte differences
remain substantial after shifted blocks. A standalone /GL entry context retains
the same 891-byte extent and grants no acceptance credit. Preserve the natural
source and investigate real caller/TU/compiler context instead of padding it.

The partial COM and global views do not close production data ownership,
platform initialization, or native runtime behavior. These two functions have
source mappings but no canonical units or exact ledger rows.
