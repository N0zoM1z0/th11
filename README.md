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

The initial direct-Ghidra inventory contains 1,988 provisional function
candidates. See generated [`docs/PROGRESS.md`](docs/PROGRESS.md) for current
review, source, and exact counts; unresolved gaps can change the inventory.
Canonical units include bounded angle arithmetic, 32-key repeat/edge state,
the complete keyboard polling path, individual joystick-button capture, draw
callback dispatch/removal, ordered callback registration, file existence
checking, and the ASCII update callback. Update dispatch, gameplay joystick
merging, and paired byte transforms remain maintained non-exact candidates.
Camera setup and renderer batch submission now have exact linked proofs;
the related perspective-camera routine has maintained non-exact source.
Frame dispatch/presentation source and their exact fog/thread helpers are
tracked in [`docs/FRAME_LOOP.md`](docs/FRAME_LOOP.md).
Animation VM reset and runtime surface release have exact proofs; related
initialization/script-binding source is tracked in
[`docs/RUNTIME_SURFACES.md`](docs/RUNTIME_SURFACES.md).
Device-state reset and renderer texture recreation have exact proofs;
[`docs/RENDER_STATE_RESET.md`](docs/RENDER_STATE_RESET.md) records those paths.
The main window callback has an exact proof; CPU surface transfer source is
tracked in [`docs/WINDOW_SURFACE_BUFFER.md`](docs/WINDOW_SURFACE_BUFFER.md).
Text bitmap creation/release and font initialization have exact proofs;
[`docs/TEXT_RESOURCES.md`](docs/TEXT_RESOURCES.md) also records checksum source.
Startup path, shortcut and window-creation source is maintained in
[`docs/STARTUP_WINDOW.md`](docs/STARTUP_WINDOW.md), with unresolved codegen differences.
Device creation now has full source; viewport reset, double-buffer priming and
error logging have exact proofs in
[`docs/DEVICE_INITIALIZATION.md`](docs/DEVICE_INITIALIZATION.md).
CPU pixel composition, text drawing and its alpha/neighbor-color processing
now have complete non-exact source in
[`docs/TEXT_DRAWING_COMPOSITE.md`](docs/TEXT_DRAWING_COMPOSITE.md).
Outlined DIB/texture drawing and the actual placement dispatcher have complete
non-exact source in [`docs/OUTLINED_TEXT.md`](docs/OUTLINED_TEXT.md).
Direct/right-aligned formatting have exact proofs; centered formatting remains
non-exact in [`docs/FORMATTED_TEXTURE_TEXT.md`](docs/FORMATTED_TEXTURE_TEXT.md).
Stage VM binding cleanup has an exact proof in
[`docs/VM_BINDINGS.md`](docs/VM_BINDINGS.md). Stage/animation document loading has
maintained non-exact source in [`docs/STAGE_RESOURCE.md`](docs/STAGE_RESOURCE.md).
The chunk decoder and postload cursor now have maintained source, and sprite
registration is exact across149 bytes; see
[`docs/ANIMATION_CHUNKS.md`](docs/ANIMATION_CHUNKS.md).
All four texture constructors have source; three add423 exact bytes, and the
811-byte decoder now has10 diagnostic differences. Details are in
[`docs/ANIMATION_TEXTURES.md`](docs/ANIMATION_TEXTURES.md).
The transparent-pixel helper now has complete source and a reviewed1,427-byte
owner including switch tables; it remains non-exact. Its validation also
corrects the reviewed surface-descriptor layout; see
[`docs/ANIMATION_TRANSPARENT_PIXELS.md`](docs/ANIMATION_TRANSPARENT_PIXELS.md).
Stage lifecycle source now includes natural C++ member cleanup. Its630-byte
initializer, VM construction/destruction and two drawing callbacks add779 exact
bytes; EH-bearing diagnostic matches remain uncredited. See
[`docs/STAGE_LIFECYCLE.md`](docs/STAGE_LIFECYCLE.md).
The complete stage main/overlay drawing functions now have non-exact source
covering1,982 reviewed target bytes. See
[`docs/STAGE_DRAWING.md`](docs/STAGE_DRAWING.md) for the remaining codegen gaps.
Stage update and object traversal add1,830 reviewed target bytes of non-exact
source. See
[`docs/STAGE_UPDATE.md`](docs/STAGE_UPDATE.md).
The complete stage script interpreter now includes both owned jump tables.
Its source remains non-exact; fog packing adds171 exact bytes. See
[`docs/STAGE_SCRIPT.md`](docs/STAGE_SCRIPT.md) for ownership and codegen gaps.
Nine interpolation,
fog arithmetic and timer functions now have source; timer assignment adds59
exact bytes. See [`docs/STAGE_TWEEN.md`](docs/STAGE_TWEEN.md).
The complete764-byte polynomial easing dispatcher
and both factor getters now have exact proofs;
[`docs/STAGE_EASING.md`](docs/STAGE_EASING.md) records their full byte proofs.
Mesh construction/reset and row-VM helpers now have complete source; vertex
publication and its callback add139 exact bytes. See
[`docs/STAGE_MESH.md`](docs/STAGE_MESH.md).
Animation VM creation, initialization and file-script binding now have complete
source and add474 exact bytes; typed-ID lookup adds79 more. See
[`docs/ANIMATION_VM_FACTORY.md`](docs/ANIMATION_VM_FACTORY.md).
The full998/342-byte mesh-effect initializers,398-byte deformation callback,
positioned VM creation, sound queue and status update now have source. See
[`docs/MESH_EFFECTS.md`](docs/MESH_EFFECTS.md) for exactness and codegen gaps.
The Windows i386 production build is open. Semantic reconstruction and
portability have not started.

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
