# 東方地霊殿 ～ Subterranean Animism

<p align="center">
  <img src="resources/progress.svg" alt="TH11 reconstruction progress">
</p>

This project reconstructs the original Japanese TH11 version 1.00a executable.
It follows the [TH10 reconstruction](https://github.com/N0zoM1z0/th10) workflow:
target identity, reviewed boundaries and origins, maintained source, exact
historical code generation, native product closure, and runtime validation are
tracked separately.

## Exact target

Supply your own copy at the ignored path `resources/th11.exe`.

| Property | Required value |
| --- | --- |
| Version | original Japanese 1.00a |
| Size | `688,128` bytes |
| SHA-256 | `2978b17f6184d100d249d4311348dd30c5c32ec75c014b667a525b797d3d8813` |
| MD5 | `6af1e3b37f28293f4fc127ee01d2632c` |
| Image base | `0x00400000` |
| Entry point | `0x00461B1D` |

The pinned [thcrap version database](https://github.com/thpatch/thcrap-tsa/blob/f08b582ce57bce800955dd371fc9a68dbad5b324/base_tsa/versions.js)
identifies this exact SHA-256 as `th11`, `v1.00a`, `(original)`.
The localized `th11c.exe` is a different target.

```bash
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
```

Original executables, game data, Ghidra databases, decompiler output, compiler
payloads, and generated build artifacts are private and never committed.

## Current status

Current reviewed boundaries, source mappings and exact-byte counts are generated
in [`docs/PROGRESS.md`](docs/PROGRESS.md). Start continuation at
[`docs/RE_HANDOFF.md`](docs/RE_HANDOFF.md); the concise
[`working knowledge`](docs/KNOWLEDGE_BASE.md) links the detailed topic records.

The latest checkpoint adds seven projected-quad/software-fog source mappings
covering1,896 target bytes, including the792/794-byte owners, and14 further
boundary/origin reviews. These sources remain non-exact. The preceding
checkpoint closed the632/537-byte builders,40-byte adapter and54-byte dispatch.
Stage meshes, VM creation, the full stage interpreter, interpolation, stage
update/drawing, ANM resources, rendering, text, input and file paths have varying
levels of maintained source and exact proofs. Source presence, exact codegen and
a runnable product remain separate claims. The Windows i386 production build,
full runtime and portability are open.

| Current area | Details |
| --- | --- |
| Renderer quad construction/submission | [quads](docs/ANIMATION_QUAD.md), [submission](docs/ANIMATION_VM_DRAW.md), [projection/fog](docs/ANIMATION_PROJECTED_QUAD.md) |
| ASCII lifecycle, glyph drawing and formatting | [lifecycle](docs/ASCII_LIFECYCLE.md), [drawing](docs/ASCII_DRAWING.md) |
| Mesh effects and VM factory | [mesh](docs/STAGE_MESH.md), [effects](docs/MESH_EFFECTS.md), [factory](docs/ANIMATION_VM_FACTORY.md) |
| Stage interpreter and interpolation | [script](docs/STAGE_SCRIPT.md), [tweens](docs/STAGE_TWEEN.md), [easing](docs/STAGE_EASING.md) |
| Stage lifecycle/update/drawing | [lifecycle](docs/STAGE_LIFECYCLE.md), [update](docs/STAGE_UPDATE.md), [drawing](docs/STAGE_DRAWING.md) |
| Animation resources | [chunks](docs/ANIMATION_CHUNKS.md), [textures](docs/ANIMATION_TEXTURES.md), [pixels](docs/ANIMATION_TRANSPARENT_PIXELS.md) |
| Other subsystems | [topic index](docs/KNOWLEDGE_BASE.md#topic-index) |

TH11's PE linker version is **9.0** and its dominant Rich compiler build is
**21022**. The compiler candidate is **VC9 / Visual C++ 2008 RTM**, selected
from TH11-local evidence. TH10's VC7.1 compiler is not used. Tool hashes and
independent normal C/C++, `/GL`, resource, linker, and PDB smoke checks are
tracked in [`config/tools.lock.toml`](config/tools.lock.toml).

```bash
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/replay-exact-units.py --unit publish-stage-mesh
scripts/repo-python scripts/progress.py
```

Canonical normal-COFF units cold-compile maintained C/C++ with the locked VC9
compiler. The Oracle requires a complete compiler-owned function extent and
exhaustive declared relocations, replays their target bindings, and compares
every byte. Artifact receipts bind source inputs, profile, target, tools, and
object hashes. Masked probes do not grant exact credit. Public primary-TU
linked contributions use a separate [VC9 linked Oracle](docs/LINKED_ORACLE.md),
with complete PE/MAP/PDB identity, ownership, artifact receipts and exhaustive
field replay. Specialized static/EH/split-owner contexts remain open.

## Tools and resource limits

Use `scripts/repo-python` for the pinned Capstone interpreter. Direct Ghidra
headless scripts verify the selected tool hashes, executable identity, mapped
samples, and the complete mapped `.text` hash before queries. REA is not used.
The private database is `ghidra-project/TH11`.

Ghidra runs at nice 10 with one CPU affinity, one analysis worker, and a 768 MiB
Java heap limit. JVM and native overhead are additional. Ghidra jobs and
Wine/compiler jobs are serialized. Tool installation and reproduction details
are in [`docs/TOOLS.md`](docs/TOOLS.md).

## Current workflow

Focus on complete functions and byte-exact historical code generation: review
the target, write natural C++, compile, compare full owners/fields, cold-replay
only affected exact units, and commit meaningful progress. Broad behavioral,
fault-injection, randomized/multiframe and caller regression suites are not
routine prerequisites. Native product/runtime and portability work are deferred.

## Project map

- [`AGENTS.md`](AGENTS.md) — session recovery and evidence rules.
- [`config/target.toml`](config/target.toml) — TH11 identity, PE, and Rich facts.
- [`config/build.toml`](config/build.toml) — explicitly open production graph.
- [`config/match-units.toml`](config/match-units.toml) — canonical exact contexts.
- [`docs/RE_WORKFLOW.md`](docs/RE_WORKFLOW.md) — reconstruction loop and gates.
- [`docs/ORACLES.md`](docs/ORACLES.md) — bounded acceptance rules.
- [`docs/RE_HANDOFF.md`](docs/RE_HANDOFF.md) — resume state and next work.
- [`docs/ARTIFACTS.md`](docs/ARTIFACTS.md) — retained private artifacts and cleanup policy.
- [`docs/KNOWLEDGE_BASE.md`](docs/KNOWLEDGE_BASE.md) — durable TH11-local facts.
- [`docs/RENDER_CAMERA.md`](docs/RENDER_CAMERA.md) — camera and batch submission evidence.

## License

Repository-authored code and documentation are provided under the MIT License.
This does not grant rights to the original game or its assets. Infrastructure
adapted from TH10 retains its MIT license and attribution.
