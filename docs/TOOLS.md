# Tools and reproduction

Python 3.11+ is required for TOML parsing. `scripts/repo-python` selects only
an interpreter that reproduces the four Capstone 5.0.6 file hashes in the lock.
An ignored `.tools/capstone` package root can supply that decoder without
changing the system Python. Public `python3 scripts/ci.py` needs no target,
Ghidra, Wine, compiler, or Capstone installation.

Local tool selection:

| Ignored path | Role |
| --- | --- |
| `.tools/ghidra` | Ghidra 12.1.3 installation |
| `.tools/jdk` | Temurin JDK 21.0.12.1+1 |
| `.tools/capstone` | Capstone 5.0.6 Python-package root |
| `.tools/msvc900` | Pinned VC9 RTM compiler/SDK payload |
| `.tools/wineprefix-msvc900` | TH11-only 32-bit Wine state |
| `.tools/winbind` | Private hash-pinned Ubuntu amd64 NTLM helper/runtime |

Ghidra/JDK/Capstone may be links to the operator's shared TH10 installations,
provided every lock hash verifies. Mutable databases and Wine state are
TH11-local. `scripts/bootstrap-local-tools.py` installs only these ignored
links and the pinned compiler checkout; it does not change global MCP/client
configuration or redistribute tools.

The VC9 PDB server uses Wine RPC authentication. Missing `ntlm_auth` can appear
as misleading LNK1101 “incorrect MSPDB80.DLL version”. The private helper is
reproducible from seven exact package URLs/SHA-256 values plus a complete
extracted-tree hash in the lock:

```bash
python3 scripts/bootstrap-ntlm-auth.py
python3 scripts/bootstrap-ntlm-auth.py --check
```

No system package or Samba daemon is installed. Only the helper process gets
its private library search path. The Wine wrapper adds the repository helper
to PATH, uses `flock --close` for shared heavy-tool serialization, and closes
that lock descriptor before child/server launch. The verifier captures output
in a temporary regular file so an idle PDB server cannot hold a stdout pipe
open after the linker exits.

```bash
scripts/repo-python scripts/verify-analysis-tools.py
scripts/repo-python scripts/ghidra.py import
scripts/repo-python scripts/ghidra.py check
scripts/repo-python scripts/ghidra.py query .analysis/review.txt review 0x004014A0 0x00401D50
scripts/repo-python scripts/ghidra.py decompile .analysis/functions.c 0x00401D50
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/verify-toolchain.py --execute
scripts/compile-probe.sh src/AsciiStrings.cpp build/probes/ascii.obj /TP /O2 /Gy /Isrc
```

`import` is a one-time initial inventory operation; it refuses existing
projects. `initialize` creates the database without exporting ledgers. Later
queries use read-only/noanalysis mode and verify the entire loaded `.text`.
`inventory` is guarded against overwriting maintained source or exact rows.

Ghidra has one CPU affinity, one analysis worker, 768 MiB maximum heap, and
nice 10. Wine commands also inherit one CPU affinity and nice 10. Native/JVM overhead is separate from heap. Serialize analysis jobs
and compiler jobs. Wine uses Xvfb, disables Mono/Gecko prompts, and stores
prefix/tmp state under `.tools/`. Explicit tool overrides use `TH11_MSVC90_ROOT`
and `TH11_WINEPREFIX`; target override is `TH11_TARGET_PATH`. Canonical tools
must retain their locked hashes.

The compiler repository's license is undeclared. It is a private compatibility
input. Codegen claims remain bounded to their declared profiles and full
function extents; smoke success does not close the production build graph.


For explicit ABI review, decompiler selectors `u32@ADDRESS`,
`u32-fast1@ADDRESS`, and `cookie@ADDRESS` temporarily set a u32 return, a u32
fastcall receiver argument, or the installed security-cookie callfixup. These
are caller-supported analysis hypotheses. All changes occur in a transaction
rolled back before the read-only project session ends. See `INPUT_POLLING.md`.
