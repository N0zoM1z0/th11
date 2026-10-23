# Keyboard repeat and edge state

`UpdateInputRepeatState` at `0x00459B50..0x00459BB8` is a complete 105-byte
fastcall function with its receiver in ECX. The body is contiguous, has no
external interior references, ends in RET followed by CC, and has one observed
direct caller: the input-poll function at `0x004576B0`.

The partial `InputStateView` layout is target-derived:

| Offset | Meaning |
| --- | --- |
| 0x00 / 0x04 | current / previous 32-key action masks |
| 0x08 | repeating-key mask |
| 0x0C / 0x10 | pressed / released edge masks |
| 0x14 | 32 unsigned per-key hold counters |
| 0x94..0x12B | unresolved owners |
| 0x12C | held-key mask |

For a down key, increment its counter, set held at >=8, and set repeat at >=26
while subtracting eight. Thus repeat starts on tick 26 and recurs every eight
ticks while held. A released key clears its counter. The routine derives both
edge masks from current XOR previous and leaves publication of previous to the
caller. The source preserves 32-bit unsigned mask shifts and counter behavior.

The complete relocation-free body cold-replays raw-equal with pinned VC9 RTM
`/TP /O2 /Gy /MT /Isrc`. Only this new unit was rebuilt for its acceptance.

```bash
scripts/repo-python scripts/ghidra.py query .analysis/input-repeat.txt review 0x00459B50
scripts/repo-python scripts/replay-exact-units.py --unit input-repeat-state
```

The upper input-poll routines require correcting provisional decompiler type
information: the keyboard mask is passed to 0x00457270 in ECX and returned in
EAX; `__security_check_cookie` at 0x0045FC2C preserves EAX on its successful path.
Ghidra's incorrect inferred cookie return was eliminating live mask-building
code. The direct decompiler accepts temporary `u32@`, `u32-fast1@`, and
`cookie@` selectors, uses the installed Ghidra security-cookie callfixup, and
rolls all metadata changes back in a read-only project session. These are
analysis hypotheses, not byte patches or source exactness claims.
