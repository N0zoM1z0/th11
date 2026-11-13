# Native Win32 process launcher adjacent to Pbg memory streams (2026-11-13)

The original Japanese TH11 v1.00a contains a complete **215-byte** Win32
process helper at `0x442E40..0x442F16`, adjacent to but independent of the
Pbg::MemFile source family. Neither a unique caller nor the original class
or translation unit has been established, so its placement alone does not
prove the function was authored specifically for Pbg.

`src/PbgProcessLauncher.cpp/.hpp` implements the complete observable native
path. It lays out a 0x44-byte `STARTUPINFOA` and `PROCESS_INFORMATION`, sets
four geometry fields to 0x80000000, show mode to 10, the character grid to
80×25 and original zero/null attributes, then calls `CreateProcessA` with
process creation flags `0x20` (`NORMAL_PRIORITY_CLASS`). If creation fails
it returns -1. Without a wait request it returns zero. Otherwise it polls
`GetExitCodeProcess` against `STILL_ACTIVE` (0x103) and returns the terminal
process code. The original does not close either created handle in this
owner; the maintained source preserves that behavior without inventing
cleanup or special authorization requirements.

The natural VC9 RTM `/TP /O2 /Ob1 /Gy /MT /Isrc` codegen matched the target
**on its first compilation**: all **215/215 bytes**, 2 independently reviewed
Win32 import DIR32 fields (`CreateProcessA` IAT `0x48B0B4` at +0x85 and
`GetExitCodeProcess` IAT `0x48B0B8` at +0x9F), the original three-byte
`LEA ECX,[ECX]` alignment at `0x442EED..0x442EEF`, and all native return
paths. Canonical `launch-child-process-optional-wait` cold-replays the full
unmasked code body and fields with zero differences. No custom register
assignments, byte splices, or truncated instruction ranges are used.
