# TH11 reconstruction agent rules

## Highest priority: lightweight exact reconstruction and useful knowledge

These are the user's current workflow requirements. They override older
instructions that mandate broad CI, repeated preflights, document/source-pin
refreshes, per-checkpoint audits or extra workflow machinery. Preserve the
actual exactness, target identity, natural-source and state-isolation rules.

- Prioritize exact reconstruction and reusable knowledge. Use the shortest
  useful loop: inspect relevant state, analyze/edit, compile and compare the
  affected scope, retain useful learning, and create a coherent checkpoint.
- Use existing repository scripts and composable Bash. Add only the smallest
  change needed for an observed problem. Do not introduce a framework, schema,
  registry, state machine, generalized validator, test family or reporting
  process merely to make a local reconstruction task look more engineered.
- Documentation, knowledge and handoff edits require no compiler, analyzer,
  full CI, cold replay or receipt refresh. Inspect the diff. Check syntax only
  for machine-readable inputs actually changed.
- Source changes need the complete affected exact comparison, including its
  owned extents and relocations. Recheck known consumers of changed shared
  inputs. Tool changes need relevant positive/negative cases and a bounded
  real smoke. Run a broad suite only for an explicit request or a demonstrated
  impact that cannot be narrowed; state that reason first.
- Do not add tests or live freshness gates for prose hashes, prompt wording,
  progress totals, handoff source-SHA tables or ordinary source spelling.
  Bind exact evidence to executable inputs and relevant verification settings.
  Keep Git metadata as provenance. A document genuinely consumed by a build
  remains a build input. Never edit historical receipts to manufacture a pass.
- Plain Markdown is sufficient for knowledge: record the finding, applicable
  target/compiler/scope, evidence links, useful failed attempts and open
  questions. No mandatory JSON conversion, promotion workflow or test per note.
  Search relevant sections; do not read an entire large knowledge journal.
- Write documentation and knowledge in natural, fluent English. Lead with the
  concrete finding and connect the evidence and next action clearly. Remove
  filler, circular restatements, repeated conclusions and unnecessary jargon.
  Do not add defensive-claim boilerplate. State a real uncertainty or scope
  limit once when it changes a decision; do not repeat disclaimers about what
  a note does not prove, certify or authorize. Prefer a small useful note over
  an exhaustive report that is difficult to read.
- Keep handoff focused on current work, evidence references, blockers and the
  next useful action. Generate progress from canonical ledgers; avoid manual
  duplicate counts, recurring full reports and unrelated documentation chores.
- Preserve dirty/untracked work, targets, useful evidence, game-specific Wine
  prefixes and analyzer projects. Exactness still requires a complete strict
  comparison; knowledge and successful commands alone confer no exact credit.


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
Run repository scripts directly from an available shell; if access is through
Factory MCP, invoke the same scripts in its repository shell rather than
switching decompiler APIs or inventing an alternate verification path.
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

Use commits prefixed `gpt-web: `. The operator authorized a public GitHub
repository and pushes for this project. Save meaningful progress promptly.
Cold-replay only changed exact units and directly affected dependencies. Update
tracking/generated progress and run `git diff --check`; do not replay the full
cohort. Public Oracle/infrastructure tests are needed when their implementation
changes, not for every reconstructed function or documentation edit.
Keep `docs/RE_HANDOFF.md` concise: current exact state, concrete codegen gaps and
next work. Record useful target/compiler findings in `docs/KNOWLEDGE_BASE.md`;
do not expand routine handoffs into behavioral-test reports.

## Shared Factory runtime

Follow [the shared tools instructions](docs/SHARED_TOOLS.md). Ghidra/JDK and
Wine executables are shared through Factory; repository scripts also work
without MCP. Use `scripts/shared_runtime.py` and the existing compiler/analysis
frontends. Keep this game's prefix, analyzer project, Java home/cache, target
and compiler profile isolated. Do not install duplicate tools or inherit
another game's `WINEPREFIX`. This shared-runtime procedure supersedes older
per-repository installation instructions; pinned versions/hashes still apply.
