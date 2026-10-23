# Bounded angle arithmetic

`AddAngles` at `0x00458E30..0x00458ECC` (157 bytes) and `NormalizeAngle` at
`0x00458ED0..0x00458F60` (145 bytes) are reconstructed in `src/AngleMath.cpp`.
Both have complete contiguous stdcall bodies, all return tails, CC following
bytes, and no external interior references. Direct Ghidra review was bound to
the entire original mapped `.text` SHA-256.

The routines correct toward [-pi, pi], retaining the original shared
`corrections++ > 32` guard. Very large values need not reach the interval before
the guard ends correction. Every correction rounds through a float. AddAngles
also rounds its sum before entering correction. These details must not be
replaced by an unbounded loop or fmod.

| Target data | Meaning | Exact double value |
| --- | --- | --- |
| `0x00497E80` | float-rounded pi promoted to double | 3.1415927410125732 |
| `0x00497E78` | float-rounded two pi promoted to double | 6.2831854820251465 |
| `0x00497E70` | negative float-rounded pi promoted to double | -3.1415927410125732 |

Each corresponding eight-byte COFF definition was independently compared with
target data. Both complete functions cold-replay exact with VC9 RTM
`/TP /O2 /Gy /MT /Isrc`; each has three fully declared DIR32 fields. The two
units share one cold-built object. No earlier exact unit was rebuilt for this
batch. This proves bounded code generation, not original TU/LTCG partition or
whole-product data ownership.

```bash
scripts/repo-python scripts/ghidra.py query .analysis/angle-review.txt review 0x00458E30 0x00458ED0
scripts/repo-python scripts/replay-exact-units.py --unit add-angles --unit normalize-angle
```
