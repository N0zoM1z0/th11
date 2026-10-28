# TH11 reconstruction agent rules

Only reconstruct the original Japanese TH11 v1.00a executable with SHA-256
`2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`.
The canonical private input is ignored `resources/th11.exe`. Never substitute
`th11c.exe`, a patched build, or TH10 bytes.

## Session recovery

Read `docs/RE_HANDOFF.md`, `docs/RE_WORKFLOW.md`, `docs/ORACLES.md`, and relevant
source/ledger rows. Inspect `git status --short --branch` and the current diff
before changes. Use `scripts/repo-python` for repository Python commands.
Verify target/toolchain identity once on resume. Refresh tracking and progress
at checkpoints; existing dirty work is ours and should be continued.

The operator requests **direct Ghidra**, without REA. Use `scripts/ghidra.py`.
Invoke local tools/scripts directly; do not route work through Factory MCP.
Ghidra defaults to one CPU affinity, one analysis worker, 768 MiB maximum Java
heap, and nice 10. Java/native overhead is additional to heap. Serialize Ghidra
jobs and Wine/compiler jobs; do not raise resource defaults without a measured
need. The compiler must follow TH11-local PE/Rich/codegen evidence: the current
candidate is VC9 RTM build 21022, not TH10's VC7.1 toolchain.

## Current focus: exact reconstruction

Prioritize complete functions and byte-level codegen differences. The normal
loop is target review, natural C++, pinned compilation, complete byte/field
comparison, affected cold proof, and commit. Work on substantial functions as
well as useful leaves.

Do not create or run broad behavioral harnesses, allocation-failure/null-pointer
matrices, randomized/multiframe suites, or caller/subsystem regressions as
routine work or checkpoint requirements. Use a small targeted execution check
only when it resolves a concrete exact-codegen or ABI question. Historical
behavior-test counts do not create a requirement to rerun them.

## Exact evidence

Keep target observations, Ghidra hypotheses, compiler observations, source
presence, exact codegen, native product closure, runtime behavior, and port
status separate. TH10 source and infrastructure are reference material, never
TH11 evidence. Default Ghidra extents and origins remain unreviewed.

Use natural C/C++. Never embed target instruction bytes, add inert padding,
patch executable code, fake returns, or misdeclare ABI to force equality.
Exact claims require reviewed full extents, declared relocation semantics,
verified historical tools, and reproducible cold compilation with zero byte
differences. Masked diagnostic matches grant no exact credit. A normal-COFF
match does not establish the target's original TU partition or /GL ownership.
Do not remove /GL from an established LTCG owner to fit a normal-COFF Oracle.
Linked/LTCG acceptance is limited to complete public primary-TU PDB code
contributions through `compare-linked-function.py`: require cold family
receipts, PE/MAP/PDB identity and owner agreement, exhaustive field replay,
and full byte equality. Specialized static-owner, EH and split-owner contexts
remain open. Never crop an extent to fit this lane.

Native product closure, broad runtime comparisons and portability are later
work, not prerequisites for this exact-reconstruction phase. A diagnostic link
context is not the complete game.

## Checkpoints

Keep experiments/logs/dumps in `.analysis/`, build artifacts in `build/`, tools
in `.tools/`, and the mutable private database in `ghidra-project/`. Never
commit game executables/data/archives, compiler payloads, decompiler output,
Ghidra databases, credentials, or local private endpoints.

Use commits prefixed `gpt-6.1-sol: `. The operator authorized a public GitHub
repository and pushes for this project. Save meaningful progress promptly.
Cold-replay only changed exact units and directly affected dependencies. Update
tracking/generated progress and run `git diff --check`; do not replay the full
cohort. Public Oracle/infrastructure tests are needed when their implementation
changes, not for every reconstructed function or documentation edit.
Keep `docs/RE_HANDOFF.md` concise: current exact state, concrete codegen gaps and
next work. Record useful target/compiler findings in `docs/KNOWLEDGE_BASE.md`;
do not expand routine handoffs into behavioral-test reports.
