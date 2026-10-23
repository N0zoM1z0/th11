# Reconstruction workflow

Read `AGENTS.md` and `RE_HANDOFF.md`; inspect dirty work before touching it.
Run target verification, toolchain hash attestation, tracking validation, and
status reporting. Query the target through direct `scripts/ghidra.py` only.

For each coherent subsystem:

1. Review its complete target control flow, incoming/interior references,
   sparse Ghidra ranges, adjacent bytes, jump tables, and provisional ABI.
2. Record boundary and origin findings independently. Keep unproved facts
   unknown. Cross-game similarities supply hypotheses only.
3. Maintain natural C/C++ and declare source mappings. Probe compiler profiles
   with the TH11 VC9 candidate. Never force byte equality using inline target
   bytes, artificial padding, fake return paths, or incorrect calling conventions.
4. For a normal-COFF candidate, recover the decorated symbol's complete extent
   and exhaustive relocation list. Review every relocation's meaning and target
   independently of its masked diagnostic. Configure a canonical unit only when
   its complete zero-difference replay is reproducible.
5. Cold-replay affected canonical units, update ledgers and generated progress,
   run public CI, and make a concise `gpt-6.1-sol: ...` checkpoint.

Use `.analysis/` for unresolved probes and generated dumps, `build/` for
artifacts, `.tools/` for private tools, and `ghidra-project/` for the private DB.
Do not refresh the initial inventory over maintained mappings or exact rows.
Export later candidates to a separate private location and reconcile changes.

Whole native product closure, target/native runtime comparisons, semantic
reconstruction, and portability are separate later gates. The current normal
COFF Oracle has no authority over established LTCG-owned units. Implement a
VC9 linked/PDB complete-extent Oracle before accepting those units.
