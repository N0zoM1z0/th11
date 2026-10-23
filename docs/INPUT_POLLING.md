# Keyboard and joystick polling

Maintained source covers these medium input functions:

| Function | Complete target extent | Source/codegen state |
| --- | --- | --- |
| MergeJoystickInput | `0x00457270..0x0045759B`, 812 bytes | maintained source; normal VC9 candidate 815 bytes; non-exact |
| PollJoystickButtons | `0x004575A0..0x004576AA`, 267 bytes | canonical linked-PE exact; cold receipt and complete PDB-owned extent |
| PollKeyboardInput | `0x004576B0..0x00457A27`, 888 bytes | canonical normal-COFF exact; cold replay covers all 888 bytes |

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
the call pushes the retry counter and the format at 0x00497750,
`"error : DIERR_INPUTLOST %d\r\n"`. The declaration now carries these observed
arguments. Its production owner and register-preservation context remain open.
The target uses an 8-byte-aligned 0x110-byte DirectInput buffer, separate from
JOYINFOEX; the source models both buffers and their observed order.

## Keyboard exact proof

Separate `if (result == DIERR_INPUTLOST)` and `else if (result != 0)` error
branches reproduce the complete VC9 target layout. Combining those conditions
produces a different 891-byte body even though the high-level decisions agree.
The canonical `poll-keyboard-input` unit cold-builds the source with pinned
VC9 RTM `/TP /O2 /Gy /MT /Isrc`, recovers its complete 888-byte COMDAT extent,
and replays all thirteen declared relocations with zero byte differences.

Bindings were reviewed independently: the cookie at 0x004A2088, keyboard gate
at 0x004C3D94, backend flags at 0x004C3810, keyboard device at 0x004C32A0,
InputState base at 0x004C92A8, joystick merge at 0x00457270, repeat update at
0x00459B50, and cookie check at 0x0045FC2C. The USER32/GetKeyboardState import
was independently parsed at IAT slot 0x0048B244. The previous-mask store uses
the InputState relocation plus its original COFF addend 4. The receipt binds
source, both local headers, profile, target, tool lock, and object.

## Remaining exact work

The joystick candidate is 815 versus 812 target bytes, with substantial
instruction/register differences. Early returns reproduce the observed WinMM
failure layout; the retained trace call still needs its actual optimizer and
callee context. A private same-TU empty-sink experiment removed that call and
was discarded. No joystick exact credit is claimed.

Partial COM/global views do not close production data ownership, platform
initialization, or native runtime behavior. Original TU partition and /GL
ownership remain open even for the keyboard normal-COFF proof.

## Input-configuration joystick buttons

PollJoystickButtons clears the128-byte array at0x004C98A0, then selects the
same joystick backend flag0x800. WinMM expands each bit of the32-bit button
mask into byte0x80. DirectInput Poll failure attempts Acquire, then up to400
additional input-lost retries; this routine does not call the trace sink.
Poll success reads0x110 bytes and copies its128 buttons from+0x30. It neither
clears the state buffer nor checks GetDeviceState's HRESULT before copying.

The function returns the array pointer in EAX. Default Ghidra inferred void,
but caller0x0043BB39 immediately indexes button bytes through EAX. Complete
raw flow and that caller were reviewed independently. Both normal COFF and
/GL probes reproduce267 bytes; canonical acceptance uses the new receipt-bound
linked-PE lane, not masked diagnostics. Its complete cold PDB contribution,
PE/MAP/PDB identity and thirteen separately reviewed fields replay exactly.
The source/header, profile, tools, producers and artifact family are bound.
