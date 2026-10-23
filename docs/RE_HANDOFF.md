# Resume brief

Target: original Japanese TH11 v1.00a, identified by `config/target.toml`.
Worktree started empty on 2026-10-23. TH10 supplied the infrastructure style;
all target facts were re-observed from TH11. Read `AGENTS.md` first.

## Current work

Direct Ghidra 12.1.3 project imported at ignored `ghidra-project/TH11`;
1,988 provisional function candidates. Default resource bounds are one CPU,
768 MiB Java heap, and nice 10. Use direct Ghidra only, per operator request.
VC9 RTM build 21022 tooling replaces TH10's incompatible VC7.1 setup.
The first source batch maintains the ASCII queue lifetime/compaction and its
update callback. The callback at 0x004014A0 is canonical exact: complete 22-byte
normal-COFF extent plus its sole REL32 replay zero-difference in two cold builds. The expiry body
at 0x00401D50 remains non-exact: current normal-COFF source 100 vs target 124 bytes.
See `docs/ASCII_LIFETIME.md` for boundary and relocation review.

VC9 normal C/C++ COFF, /GL C++, resources, PE32 link, and PDB-owned 10-byte
extent/decoder smoke have executed successfully. Private NTLM helper packages
and runtime tree are pinned; no host service was installed. Wine/Ghidra heavy
commands share a flock whose descriptor is closed before child launch.

## Recovery commands

```bash
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
scripts/repo-python scripts/ghidra.py check
```

## Next work

1. Investigate expiry 0x00401D50 compiler/source context (100-byte maintained
   indexed and natural pointer candidates versus 124-byte target). Preserve the
   exact callback when changing its header or callee declarations; cold-replay it.
2. Expand ASCII queue construction/draw/lifetime owners and callback topology.
3. Implement/validate a VC9 linked PE/PDB exact Oracle for private register ABIs
   and established LTCG units. The current normal-COFF lane grants no LTCG
   ownership or whole-product credit.
4. Continue subsystem reconstruction, boundary/origin inventory, and production
   data/library/resource/TU/link-order closure. Native runtime validation,
   semantic reconstruction, and portability remain later phase gates.
