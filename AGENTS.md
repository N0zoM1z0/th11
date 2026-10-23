# TH11 reconstruction agent rules

Only reconstruct the original Japanese TH11 v1.00a executable with SHA-256
`2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813`.
The canonical private input is ignored `resources/th11.exe`. Never substitute
`th11c.exe`, a patched build, or TH10 bytes.

## Session recovery

Read `docs/RE_HANDOFF.md`, `docs/RE_WORKFLOW.md`, `docs/ORACLES.md`, and relevant
source/ledger rows. Inspect `git status --short --branch` and the current diff
before changes. Use `scripts/repo-python` for repository Python commands.
Verify target, toolchain, tracking, and progress before target-dependent work.

The operator requests **direct Ghidra**, without REA. Use `scripts/ghidra.py`.
Ghidra defaults to one CPU affinity, one analysis worker, 768 MiB maximum Java
heap, and nice 10. Java/native overhead is additional to heap. Serialize Ghidra
jobs and Wine/compiler jobs; do not raise resource defaults without a measured
need. The compiler must follow TH11-local PE/Rich/codegen evidence: the current
candidate is VC9 RTM build 21022, not TH10's VC7.1 toolchain.

## Evidence and phase gates

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

Required order: exact reconstruction; faithful Windows i386 build and runtime
closure; semantic reconstruction with target plus native product Oracles;
portability. Never report a diagnostic harness as the complete game.

## Checkpoints

Keep experiments/logs/dumps in `.analysis/`, build artifacts in `build/`, tools
in `.tools/`, and the mutable private database in `ghidra-project/`. Never
commit game executables/data/archives, compiler payloads, decompiler output,
Ghidra databases, credentials, or local private endpoints.

Use commits prefixed `gpt-6.1-sol: `. The operator authorized a public GitHub
repository and pushes for this project. Run focused cold exact replays,
tracking validation, generated progress checks, public CI, and `git diff --check`
before each checkpoint. Prioritize useful medium-size functions; only replay
changed units and directly affected dependencies, never the full cohort by
default. Record durable facts in `docs/KNOWLEDGE_BASE.md` and
resume state in `docs/RE_HANDOFF.md`.
