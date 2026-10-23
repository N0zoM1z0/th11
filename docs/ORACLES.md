# Oracle policy

| Claim | Required falsification mechanism |
| --- | --- |
| Target identity | Exact size, SHA-256/SHA-1/MD5, PE sections, Rich stream against manifest |
| Ghidra routing | Hash-pinned Ghidra/JDK, original module metadata, image base/entry, distributed mapped samples, complete mapped `.text` SHA-256 |
| Boundary and origin | Complete target flow, all reviewed references/tables/adjacent bytes; provisional when unresolved |
| Compiler candidate | TH11 PE/Rich evidence plus hash/banner checks and executed artifact-family smoke |
| Source presence | Maintained source mapping; grants no byte credit |
| Normal-COFF exactness | Cold VC9 object, source/tool/object receipt, full owned extent, exhaustive relocation replay, zero byte differences |
| LTCG exactness | Open: requires a VC9 linked PE/PDB complete-extent and exhaustive linked-field Oracle |
| Native product | Full build/link/data/resource/library/initialization closure |
| Runtime behavior | Original and reconstructed native products agree on declared deterministic observables |
| Portability | Native baseline and port agree independently |

The original target's `.text` occupies inclusive addresses
`0x00401000..0x0048AE14` (564,757 bytes). Compute its full mapped hash from the
private target; its SHA-256 is
`6e5cb58c1434ef85c1022c1e4629160d0e942910660fa4adbc179c4ea9077f99`.
No target instruction dump is committed. Ghidra check/query
commands compare this entire byte range to loaded database memory, in addition
to distributed mapping probes.

`compare-coff-function.py OBJECT SYMBOL ADDRESS SIZE --json` is a diagnostic:
relocation-masked similarity and prospective bindings have no acceptance
authority. `--unit NAME` requires the declared canonical context and a current
artifact receipt. Its extent comes from a COFF function-definition auxiliary
record or one complete single-function COMDAT section, never a caller-chosen
slice or an adjacent section. Every COFF relocation must match offset, type,
and symbol, then resolve to the separately reviewed target value. All bytes
must agree after replay. The original TU partition and /GL ownership remain
separate unknowns even when normal-COFF code generation reproduces a body.

Synthetic public negative controls reject wrong final bytes, truncated bodies,
escaping auxiliary extents, undeclared relocations, wrong relocation values,
and stale receipts. Private target-bound cold replays provide the actual
exactness evidence; public CI checks cannot replace them.
