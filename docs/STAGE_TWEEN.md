# Stage interpolation and timers

Nine complete functions now have natural C++ source in `src/StageTween.cpp`.
Direct local Ghidra review attested the mapped text hash and reconciled full
flows, returns, references and neighbors. These nine owners are contiguous,
without embedded tables or external interior references.

| Function | Address | Target bytes | Source bytes | State |
| --- | --- | ---: | ---: | --- |
| StepStageVectorTween | 00405240 | 883 | 785 | Non-exact |
| StepStageFogTween | 004055D0 | 737 | 534 | Non-exact |
| StageVectorTweenFactor | 00405960 | 39 | 39 | Exact linked unit |
| StageFogTweenFactor | 00405990 | 45 | 45 | Exact linked unit |
| SubtractStageFog | 004059D0 | 61 | 59 | Non-exact |
| ScaleStageFog | 00405A10 | 64 | 62 | Non-exact |
| AddStageFog | 00405A50 | 61 | 59 | Non-exact |
| SetStageTimer | 00406100 | 59 | 59 | Exact linked unit |
| AdvanceStageTimer | 00459270 | 83 | 83 | Non-exact |

The generic timer bodies' and factor getters' original provenance remains
indeterminate. Fresh review finds the same factor pattern across five other
layouts; reuse and game callers alone do not establish authorship. The other
mappings describe the stage-specific vector/fog interpolation path.

## State and behavior

A vector tween contains four12-byte vectors, a20-byte timer at+30, signed
duration+44 and mode+48. Fog uses four28-byte values, timer+70, duration+84 and
mode+88. A fog value contains start/end, four float color channels and the
packed DWORD at+18. Its arithmetic operates on six floats and calls the real
Pack helper after every operation, without clamping.

Both step functions advance the timer only when duration is positive. They
reload duration afterward. At current>=duration they assign the timer to that
duration, clear duration and return initial for mode7 or target for every other
mode, including17. Terminal fog returns preserve all28 bytes, including the
stored packed DWORD. Nonpositive duration skips advancement but still executes
the selected interpolation mode; it does not imply an endpoint return.

Mode7 adds target to initial and returns the updated initial. Each vector
component uses its own previous component. An early reading incorrectly used
old Y for the Z sum because it missed a POP changing stack offsets. Full-body
execution caught this; the corrected source and independent assertion require
(10,20,30)+(1,2,3)=(11,22,33). There is no observed Z-component bug.

Mode17 first adds control2 to initial, then target to control2, and returns the
new initial. Mode8 evaluates cubic Hermite interpolation. With float-rounded
t=time.fraction/duration, the float coefficients are:

```
a = (2*t+1)*(t-1)*(t-1)
b = t*t*(3-2*t)
c = (1-t)*(1-t)*t
d = (t-1)*t*t
result = ((initial*a + target*b) + control1*c) + control2*d
```

Coefficient evaluation promotes t to double. Each coefficient, scaled vector
and pairwise vector sum rounds to float at the observed boundary. Fog performs
packing after each of four scales and three additions. Other modes use
`(target-initial)*factor+initial`, again preserving intermediate float stores.
The factor helpers round fraction/signed-duration to float, then invoke the
easing dispatcher only when unsigned(mode-1)<16, and round its result to float.

AdvanceStageTimer records previous=current. Scale strictly inside the interval
(double(0.99f),1.01f) advances the signed current with32-bit wrap and adds double
1.0 to fraction before storing float. Otherwise it stores scale+fraction as
float and converts that value to signed32. EAX returns the new current on both
paths. SetStageTimer lazily initializes when flag bit0 is clear, installs the
global scale pointer and preserves other flag bits, then writes previous=frame-1
with32-bit wrap, current=frame and fraction=float(frame).

## Code generation and exact proof

Native vector stepping takes state in EDI and hidden output in EBX; fog takes
state in EBX and output on stack. Both return the output pointer in EAX. Fog
operators use ESI for output; subtraction uses ECX left minus EAX right. The
factor helpers take state in EAX and return ST0. Advance takes ESI and returns
EAX; assignment takes EAX plus a stack frame argument. Ordinary declarations
and an exported test wrapper accommodate the compiler's actual private ABIs;
no false prototypes, byte blobs or padding were introduced.

The source fog operators save two bytes by using a different output register.
Advance has eight address-masked diagnostic differences despite equal83-byte
size: native INC EDX/store EDX/MOV EAX,EDX versus candidate LEA EAX/store EAX.
Linking the real easing dispatcher now recovers its EAX-curve/stack-progress
private ABI naturally. Conditional-expression returns retain the getters' final
float rounding point and reproduce both39/45-byte bodies. Their exact units
each bind one REL32 to459730; see [STAGE_EASING.md](STAGE_EASING.md).
The large step functions remain non-exact; none receives masked-match credit.

SetStageTimer is a complete non-EH public primary-TU PDB contribution in
`build/match-linked/StageTween`. Its sole DIR32 field at+36 resolves the global
scale symbol to004A7948 with zero addend. Cold source/tool/producer receipts,
PE/MAP/PDB identity, contribution ownership, exhaustive field replay and all59
bytes pass the linked Oracle. The affected existing171-byte pack unit also
passes in the rebuilt script family:

```bash
scripts/repo-python scripts/replay-exact-units.py \
  --unit set-stage-timer --unit pack-stage-fog
```

All affected primary units retain `/TP /O2 /Ob1 /Gy /MT /Isrc /EHsc /GL`.
AngleMath is normal-COFF support in the tween/script families. Sources, entries and complete contexts are declared in
`config/match-units.toml`. Other exact families have unchanged inputs and were
not replayed. Totals are109 source mappings and55 exact/11,554 bytes:8 COFF,
47 linked PE. Diagnostic executables still contain unresolved data anchors.

## Bounded execution evidence

Private `.analysis/stage_tween_fixture.py` and `check-stage-tween.py` compare
complete original and candidate bodies for all nine functions. An exported
eight-argument wrapper executes actual candidate aggregate returns without
inventing private ABI declarations. The tests compare full state/output bytes,
relevant return values, easing calls/float bits, stack, preserved registers and
x87 control/tag state. Only declared global-scale pointers are normalized.

All2,171 cases agree in both the private probe and canonical setter families,
including four matching null-scale faults. The canonical run peaked at198,284
KiB RSS with real easing. They cover all modes0..18 and extreme signed modes, negative/zero/positive durations,
completion, repeated updates, fractional/negative/scaled clocks, timer flags,
wrap, large odd divisors, nonfinite fog arithmetic and randomized vectors.
Two hundred extra randomized Hermite cases use x87 extended precision control
0x037F in addition to the normal0x027F cases. Independent assertions check XYZ
accumulation, position/velocity order, terminal packed-value preservation and
duration clearing. The canonical family is selected with:

```bash
TH11_STAGE_TWEEN_TEST_DIR=build/match-linked/StageTween \
TH11_STAGE_TWEEN_REAL_EASING=1 \
  scripts/repo-python .analysis/check-stage-tween.py
TH11_STAGE_SCRIPT_REAL_TWEENS=1 TH11_STAGE_SCRIPT_REAL_EASING=1 \
  scripts/repo-python .analysis/check-stage-script.py
```

The second command passes all709 script cases, including five faults under
the remaining declared models and20 six-frame programs. Timers, interpolation,
fog arithmetic, packing and angle normalization now execute actual bodies.
VM reset/bind, mesh construction/allocation/release/delete and CRT sine remain
models. Candidate conversion executes an isolated original-machine dependency
through `.analysis/stage_tween_oracles.py`; the reconstructed easing body now runs with
`TH11_STAGE_TWEEN_REAL_EASING=1` or `TH11_STAGE_SCRIPT_REAL_EASING=1`. The earlier
checks used an original easing Oracle; the new integrated runs supersede that
substitution. This supplies no CRT source credit or independent native product
validation. Original runs execute their actual dependencies directly.

The easing dispatcher now has a complete764-byte exact implementation, including
its table and alignment; see [STAGE_EASING.md](STAGE_EASING.md). Its original
seven-byte inventory was only the indirect JMP. The standalone checks above
originally used an easing dependency Oracle; current real-easing integration
runs the reconstructed dispatcher directly. Mesh reset40E6C0 and constructor
40E530 remain useful script dependencies. Full game build/runtime
and exception-unwind closure remain open.

Target/toolchain checks, target-bound tracking, generated progress and public
CI all pass; public Oracle suites contain8+25 tests. The subsequent easing
checkpoint also validates its new764-byte unit and
the two new39/45-byte factor units. Only changed/affected families were replayed.
