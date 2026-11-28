# Reconstruction workflow

> Current workflow: follow the **Highest priority** section in `AGENTS.md`.
> Prefer affected exact comparisons and useful Markdown knowledge. Ordinary
> notes do not require full CI, native replay or document-hash/pin refreshes.
> Older broad validation instructions apply only when their impact is relevant.


Read `AGENTS.md` and `RE_HANDOFF.md`; inspect dirty work before touching it.
Verify target/toolchain identity once on resume; update tracking/progress at
checkpoints. Query the target through direct `scripts/ghidra.py` only.

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
   independently of its masked diagnostic. For a call, confirm the source calls
   the same function as the original, not only that the declared target address
   matches; `validate-tracking.py` catches this only for exact-owner symbols.
   Configure a canonical unit only when its complete zero-difference replay is
   reproducible.
5. Cold-replay the changed unit. When you change a shared source that exact
   units link (a callee body, a support TU, a probe or a header), also replay
   those exact units before committing; LTCG can change their private register
   ABI. `scripts/replay-exact-units.py --file PATH` selects every exact unit
   whose context includes PATH directly or through quoted includes (declared
   `inputs` alone miss most header consumers); `--unit` and `--source` still
   work, and `--keep-going` reports every failure instead of stopping. Update
   ledgers/generated progress, check the diff, and make a concise checkpoint
   commit with your agent prefix.

Broad behavior tests, null/allocation-failure matrices, randomized/multiframe
execution and caller regressions are not part of this loop. Use focused
execution only to answer a specific codegen/ABI question. Do not rebuild callers
or rerun historical suites merely because a dependency now has source; the
step 5 replay is only for exact units whose linked inputs you changed. Run
Oracle/infrastructure tests when those tools change; full CI is not a per-function
or per-documentation-edit prerequisite.

Use `.analysis/` for unresolved probes and generated dumps, `build/` for
artifacts, `.tools/` for private tools, and `ghidra-project/` for the private DB.
Do not refresh the initial inventory over maintained mappings or exact rows.
Export later candidates to a separate private location and reconcile changes.

Whole native product closure, broad runtime comparisons and portability are
deferred; they do not block exact checkpoints. The normal-COFF
Oracle has no authority over established LTCG-owned units. Public primary-TU
linked contributions use the separate VC9 Oracle described in
`LINKED_ORACLE.md`. Authenticated owner-qualified compiler EH
**address fields** are supported; standalone static owners, split
ownership and independent EH-handler code remain open.
