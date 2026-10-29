# Stage interpolation and timers

Nine complete functions now have natural C++ source in `src/StageTween.cpp`.
Direct local Ghidra review attested the mapped text hash and reconciled full
flows, returns, references and neighbors. These nine owners are contiguous,
without embedded tables or external interior references.

| Function | Address | Target bytes | Source bytes | State |
| --- | --- | ---: | ---: | --- |
| StepStageVectorTween | 00405240 | 883 | 785 | Non-exact |
| StepStageFogTween | 004055D0 | 737 | 536 | Non-exact |
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
a = (t-1)*(t-1)*(2*t+1)
b = (3-2*t)*(t*t)
c = (1-t)*(1-t)*t
d = (t-1)*t*t
result = ((initial*a + target*b) + control1*c) + control2*d
```

Independent x87 review confirms that a forms the square before the last factor.
The current full diagnostics are vector785/824 and fog536/643; neither is exact.
See [SECONDARY_PAIR_MATH](SECONDARY_PAIR_MATH.md).

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


The complete easing dispatcher is exact; see [STAGE_EASING.md](STAGE_EASING.md).
Large step functions, fog operators and AdvanceStageTimer retain the codegen
gaps above. Broad behavioral matrices are not part of the exact workflow.
