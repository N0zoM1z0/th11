# Stage interpolation and timers

The StageTween family has nine complete function owners with natural C++ source in src/StageTween.cpp. AdvanceStageTimer additionally has an isolated private-ESI translation unit in src/StageTimerExact.cpp that reproduces its complete 83-byte native owner without changing the vector-tween LTCG context.
Direct local Ghidra review attested the mapped text hash and reconciled full
flows, returns, references and neighbors. These nine owners are contiguous,
without embedded tables or external interior references.

| Function | Address | Target bytes | Selected candidate bytes | State |
| --- | --- | ---: | ---: | --- |
| StepStageVectorTween | 00405240 | 883 | 883 | Exact linked unit (four reviewed fields; zero differences) |
| StepStageFogTween | 004055D0 | 737 | 794 (StageTween family); 737 (wide proof) | Exact only in the separate wide secondary link (step-stage-fog-tween) |
| StageVectorTweenFactor | 00405960 | 39 | 39 | Exact linked unit |
| StageFogTweenFactor | 00405990 | 45 | 45 | Exact linked unit |
| SubtractStageFog | 004059D0 | 61 | 61 | Exact linked unit |
| ScaleStageFog | 00405A10 | 64 | 64 | Exact linked unit |
| AddStageFog | 00405A50 | 61 | 61 | Exact linked unit |
| SetStageTweenControl2 | 00405B30 | 18 | 18 | Exact linked owner, 0 fields |
| SetStageTweenControl1 | 00405B50 | 18 | 18 | Exact linked owner, 0 fields |
| PackStageFog | 00405B70 | 171 | 171 | Exact linked unit (now in this TU) |
| SetStageTimer | 00406100 | 59 | 59 | Exact linked unit |
| AdvanceStageTimer | 00459270 | 83 | 83 | Exact linked-PE unit (advance-stage-timer) |

The generic timer bodies' and factor getters' original provenance remains
indeterminate. Fresh review finds the same factor pattern across five other
layouts; reuse and game callers alone do not establish authorship. The other
mappings describe the stage-specific vector/fog interpolation path.

## State and behavior

A vector tween contains four12-byte vectors, a20-byte timer at+30, signed
duration+44 and mode+48. Fog uses four28-byte values, timer+70, duration+84 and
mode+88. A fog value contains start/end, four float color channels and the
packed DWORD at+18. Its arithmetic operates on six floats and calls the real
Pack helper after every operation, without clamping. PackStageFog saves the
x87 control word, sets RC=11b (0xC00) for FISTP, then restores the saved word;
the byte conversion therefore truncates toward zero. The source's
`static_cast<int>` is intentional and matches this behavior.

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
An earlier StageScript-primary context emitted 785-byte vector and 536-byte
fog candidates. Those are not the current StageTween-family sizes: the current
family emits the exact 883-byte vector owner and a 794-byte fog candidate.
Earlier 272/280, 276/280 and 278/280 measurements predate the final mode-17
Y-order correction below. The separate wide secondary fog context emits the
exact 737-byte fog owner. These /GL results belong to different producer
contexts and must not be collapsed into one candidate measurement. See
[SECONDARY_PAIR_MATH](SECONDARY_PAIR_MATH.md).

Coefficient evaluation promotes t to double. Each coefficient, scaled vector
and pairwise vector sum rounds to float at the observed boundary. Fog performs
packing after each of four scales and three additions. Other modes use
`(target-initial)*factor+initial`, again preserving intermediate float stores.
The fallback Z sum is intentionally written as `initial.z + scaled.z` through
a volatile reference: native x87 loads initial Z before adding scaled Z, while
the fast VC9 optimizer otherwise commutes the two operands. The emitted Z
fallback pair matches that native order. The mode-17 Y accumulation likewise
uses `previousY + control.y` through a volatile reference; this is the final
operand-order correction that completes the exact linked owner. The first
Z-order experiment temporarily moved alignment from 276/280 to 274/280; the
scalar-first Hermite control1 correction and final mode-17 Y correction
supersede those intermediate measurements.
The factor helpers round fraction/signed-duration to float, then invoke the
easing dispatcher only when unsigned(mode-1)<16, and round its result to float.

AdvanceStageTimer records previous=current. Scale strictly inside the interval
(double(0.99f),1.01f) advances the signed current with32-bit wrap and adds double
1.0 to fraction before storing float. Otherwise it stores scale+fraction as
float and converts that value to signed32. EAX returns the new current on both
paths. SetStageTimer lazily initializes when flag bit0 is clear, installs the
global scale pointer and preserves other flag bits, then writes previous=frame-1
with32-bit wrap, current=frame and fraction=float(frame).

## Exact native 15-byte timer comparison

At `405E10..405E1E` the original executable contains a complete timer
comparison: native ECX is the timer receiver, an `int` frame bound is passed
on the stack, and the signed `current` at +4 is compared with `<=` to return
0 or 1. One `CC` byte precedes the following distinct timer operation.
The original class, translation unit, and caller are unestablished.

The ordinary C++ method `StageTimerComparisonView::IsAtOrBefore` produces
15 bytes even in normal VC9 COFF, but that isolated context uses ECX instead
of the original EDX scratch register for two operand selector bytes. A
separate-TU dynamic consumer compiled with pinned VC9 `/GL` naturally
recovers EDX while preserving the 15-byte complete primary-TU PDB contribution.
The canonical `stage-timer-is-at-or-before` unit has no link fields and passes
**15/15 byte strict cold replay with zero differences**, without ASM or
modified output bytes.

## Exact 20-byte negative frame timer adapter at 0x405E30

The native helper accepts its timer in ECX and signed frame count in EAX. It
negates the integer first, converts that signed result to x87 float with
`FILD`, forwards the float to `OffsetSecondaryTimer` (`0x459210`) while
preserving the timer receiver, then returns. Its full 20-byte body ends at
`0x405E43`, followed by 11 `CC` bytes.

`src/StageTimerNegativeOffset.cpp` expresses the operation as a normal C++
member method on a layout-compatible `StageTimerNegativeOffsetView`. With
the real `src/SecondaryActorMotion.cpp` implementation in the `/GL` producer,
VC9 naturally reproduces the complete 20-byte private register ABI and sole
`REL32` field at `+0x0e`. The primary PDB contribution is exactly 20 bytes and
strict cold replay reports zero differences. The source-side class name is
only a layout/ABI expression; original class provenance is unknown.

## Exact fixed negative-one-frame timer adapter at 0x405E50

The complete native helper at 0x405E50..0x405E5F loads the original
-1.0f literal from 0x497F20, passes it as the float argument to
OffsetSecondaryTimer (0x459210) while retaining the timer receiver in ECX,
then returns. Its body is 16 bytes, followed immediately by the distinct
renderer tint reset owner at 0x405E60.

src/StageTimerNegativeOne.cpp expresses this as an ordinary C++ member
method on a layout-compatible view. The real SecondaryActorMotion.cpp
callee is linked into the pinned VC9 /GL producer. The complete 16-byte
PDB contribution exactly matches the original with two independently
reviewed fields: DIR32 at +2 to 0x497F20 and REL32 at +0x0b to
0x459210. Focused strict cold replay reports zero differences. The proposed
view/class identity and original source owner remain unknown.

## Exact 25-byte vector addition operator at 0x405DF0

The native body at `405DF0..405E08` adds the three `CameraVector` components
with the left operand in ECX, the right operand in EDX, and the hidden 12-byte
result pointer in EAX. This ABI shape is naturally expressed as a C++ member
`operator+` returning `CameraVector`; a normal free function produced the same
six operand-selector differences until the member return ABI was modeled.

`src/StageVectorMathLeaves.cpp` now uses a small `StageVectorMathView` with the
same three-float layout. Its `__fastcall` member operator returns a normal
`CameraVector` aggregate. VC9 `/GL` emits the complete native 25-byte owner,
including all three x87 `FLD/FADD/FSTP` sequences and the final `RET`, with no
relocations. The full PDB primary contribution is 25 bytes, and strict cold
replay reports **zero differences**. The original class identity remains
unknown; `StageVectorMathView` is only the source-side ABI/layout expression.
The two sibling math functions in the same source/PDB context continue to
pass their 43-byte and 33-byte exact comparisons.

## Recovered vector math owners at 0x405D90 and 0x405DC0

The original `.text` includes two separate arithmetic owners missing from
Ghidra's initial function inventory. At `405D90..405DBA` the 43-byte
helper computes reciprocal scale `1.0f/divisor` and multiplies the three
`CameraVector` coordinates, with each float result rounded at the original
x87 store. It uses private ECX source/EAX output and a stack float argument.
At `405DC0..405DE0` the 33-byte helper computes x*x+y*y+z*z and returns a
float in ST0, using EAX for its input pointer. Following `CC` bytes and
PDB code contributions establish the complete reviewed extents; the original
source class and calls are not established.

Natural C++ functions `ScaleStageVectorByReciprocal` and
`StageVectorSquaredLength` in `src/StageVectorMathLeaves.cpp` compile under
pinned VC9 `/GL` with their own small dynamic cross-TU caller. Both complete
linked-PDB primary contributions exactly match the native bodies: **43/43**
and **33/33** bytes, no relocations and no different code bytes. Focused
verification uses `scale-stage-vector-by-reciprocal` and
`stage-vector-squared-length`; both pass independent cold replay. The
neighboring `AddStageVectorComponents` at `405DF0` is source-present but
remains non-exact with six native operand-selector differences.

## Adjacent exact integer-frame timer offset wrapper

The native `0x405E20..0x405E2F` is a 16-byte adapter from signed integer frame
count to floating-point timer offset. VC9 computes `float(frames)`, passes it
to `OffsetSecondaryTimer` (native `0x459210`) and returns that result; the
receiver is kept in the private ECX register. The following `0x405E30`
starts a distinct helper. The original name and source TU are unknown.

`src/StageTimerIntegerOffset.cpp` expresses the conversion and delegation in
natural C++. Its focused VC9 `/GL` producer uses the actual
`src/SecondaryActorMotion.cpp` implementation, so private calling conventions
and float argument passing are derived without hand-written register shims.
The full **16/16-byte** primary PDB/PE/MAP contribution matches after strict
replay of the sole original REL32 field at `+0x09 -> 0x459210`. Canonical
single-unit verification: `offset-stage-timer-by-frames`.

## Native 10-byte allocation forwarding adapter

The ten native bytes at `0x00405D80..0x00405D89` transfer the private EAX byte
count into the stack calling convention of pinned CRT `malloc` at `0x460192`:
`PUSH EAX`, `CALL _malloc`, caller stack cleanup and `RET`. Six `CC` bytes
follow. Ghidra did not classify a function there; no original caller is
established, and the original source class/TU is unknown.

`src/StageHeapAllocation.cpp` recovers this through an ordinary C++ `malloc`
wrapper. A dynamic, separate-TU VC9 `/GL` consumer naturally passes `size_t`
in EAX and compiles the **complete 10/10-byte** function. The canonical
`allocate-stage-heap-bytes` linked unit verifies the primary-TU PDB/PE/MAP
contribution and the sole native `REL32` at +2 to `_malloc`, with zero other
byte differences. Focused replay:

```bash
scripts/repo-python scripts/replay-exact-units.py --unit allocate-stage-heap-bytes
```

## Adjacent 18-byte vector setters (2026-11-16)

The native `.text` contains two complete 18-byte vector setters at
`0x405B30..405B41` and `0x405B50..405B61`. The bytes load three DWORDs from
an EAX vector and store them into an ECX `StageVectorTween` at offsets
`+0x24..+0x2C` (control2) or `+0x18..+0x20` (control1), followed by a plain
`RET`. Both end before 14 `CC` bytes. They were not present in the initial
Ghidra function inventory, and no original caller is established. Their
proposed semantic names and authorship remain tentative.

`src/StageTweenControlCopies.cpp` uses ordinary C++ member assignments.
A single dynamic cross-TU producer compiled with pinned VC9 `/GL` naturally
chooses the observed EAX/ECX private calling convention. Complete 18-byte
primary-TU PDB/PE/MAP extents and target bytes match for both functions,
without relocations or any injected instructions. Both focused strict replays
pass independently:

```bash
scripts/repo-python scripts/replay-exact-units.py \
  --unit set-stage-tween-control1 --unit set-stage-tween-control2
```

## Code generation and exact proof

### Inline definitions in the tween TU (2026-11-14)

The native layout and call graph fit one model: `SetStageTimer`, the fog
operators and `StageFogView::Pack` are defined `inline` in the tween .cpp,
while the shared header keeps plain declarations. Evidence:

- `SetStageTimer` (0x406100) is inlined into both step functions but called
  out of line from about 90 sites in other TUs, including small functions such
  as 0x434380 (183 bytes) whose inlining budget would not be exhausted. Those
  TUs therefore never see an inline definition.
- `Pack` (0x405B70) sits between the fog operators and the timer helpers, not
  with the stage interpreter, so its out-of-line copy belongs to this TU.
- Every native caller of `Pack` keeps its object in ESI and reloads ECX
  before the call (`mov ecx,esi; call Pack; mov eax,esi`). That is the
  standard thiscall clobber assumption. When `Pack` is an ordinary LTCG
  function, VC9 uses its known register usage instead and produces an ECX
  output ABI for the operators (59/59/62 bytes, non-exact). When `Pack` is an
  inline function whose inlining is declined, its COMDAT copy is treated as
  opaque and the operators come out at the native 61/64/61 bytes with ESI
  output. A non-/GL Pack object gives the same result, but would contradict
  the tween-region placement, so the inline model is kept and /GL is
  unchanged.
- The region 0x405A90..0x405B6F and 0x405C20 holds uninventoried dead copies
  of small inline helpers (vector scale, timer `>=`, fraction getter, a tail
  jump to AdvanceStageTimer, and a six-float fog builder that calls Pack).
  The game was linked without discarding unreferenced COMDATs. These copies
  are a useful map of the original inline helper set.

With these definitions the three operators are exact in the StageTween family
(units `subtract-stage-fog`, `scale-stage-fog`, `add-stage-fog`), each with
one REL32 to Pack. `pack-stage-fog` moved to the same family and remains exact.
The family probe no longer calls the operators directly, because extra callers
change the LTCG register convention that the native step function imposes.

`/Ob2` was tested as an alternative explanation. It produces the same step
inlining, but in the MusicStream family it also inlines `ResetMusicDecoder`
into the exact `ConfigureMusicDecoder`, which native does not. A global
`/Ob2` is therefore rejected; all units keep `/Ob1`.

### Step-function source corrections

Both step functions now follow these native details:

- No `duration` local. Native keeps the loaded `self->duration` in one stack
  slot and stores it again as the inlined `SetStageTimer` argument; reading
  `self->duration` at each use reproduces both stores and the mode-8 `fidiv`.
- The ordinary interpolation fallback now names the target-minus-initial
  difference, scales it, and writes the three returned vector components in
  sequence. This recovers the complete 883-byte PDB extent without assembly or
  padding, but leaves eight normalized instruction keys unmatched and is not
  an exact proof.
- The terminal endpoint uses `if (mode != 7) return target; return initial;`.
  The ternary form modified EBX and forced the state into EAX with an extra
  save; separate returns let the state arrive in EBX as in native.
- Modes 7 and 17 copy `initial` into a local before the first addition. Native
  copies the whole value with `rep movsd` before adding; operating on
  `self->initial` directly does not.
- Hermite coefficients use the float `progress` directly with `2.0 * progress`
  and `a = (2t+1) * ((t-1)*(t-1))`. This reproduces native's `fadd st0,st0`,
  the square-before-sum scheduling and the shared `2t` subexpression.

In the StageTween family, fog stepping is 794 bytes against the 737-byte target; everything matches structurally except one
gap: native calls the out-of-line `operator+` for mode 17's second addition
(`control2 + target`), while VC9 here inlines all three mode-7/17 additions.
Nested inline timer helpers, a default constructor, channel loops and
statement rewrites did not change this. Two more levers were ruled out on
2026-11-14: a plain-`inline` Hermite helper (no change), and an inline
`operator+=` written as `*this = *this + other`. Used only for mode 17's second
statement it matches native's instruction count (220) but VC9 calls the `+=`
copy instead of inlining it and calling `operator+`; used for all three
mode-7/17 additions it is never inlined (542 bytes). A six-float constructor used by the
operators changes their bodies (temporary spills) and is wrong.

Vector stepping: explicit component stores in the ordinary interpolation
fallback produce an 883-byte PDB contribution, equal in length to the
883-byte native owner. The Hermite branch uses a scalar-first natural C++
scale helper for the target and control2 vectors, while the initial/control1
paths retain their value-first ordering. This restores four original x87
`fld`/`fmulp`/`fxch` instruction keys without changing the interpolation
formula. The complete candidate remains **883 bytes and 280 instructions**.
A balanced Hermite sum tree was previously rejected (887 bytes, 253/280
aligned). The final mode-17 Y operand-order correction below closes the
remaining instruction differences and makes the complete owner byte-exact.
No ASM, padding, or truncated body is used.


2026-11-19 final follow-up: after the Hermite control1 scale-order correction,
mode 17's Y accumulation was the remaining operand-order difference. Writing
`previousY + control.y` through a volatile reference (rather than allowing
VC9 to commute it to `control.y + previous.y`) restores the native x87 order.
The independent StageTween-family linked owner now has an exact 883-byte PDB
contribution, all four reviewed DIR32/REL32 fields match, and the complete
byte replay reports **zero differences**. The canonical unit is
`step-stage-vector-tween`; its focused strict cold replay is:

```bash
scripts/repo-python scripts/replay-exact-units.py --unit step-stage-vector-tween
```

No register shims, ASM or padding were added. The separate StageScript-primary
context remains a distinct LTCG producer and must not be conflated with this
exact StageTween-family proof.

AdvanceStageTimer is exact in the isolated advance-stage-timer linked-PE
unit. The body preserves the private ESI input, ECX save/restore, previous-frame
copy, strict near-one scale comparisons, x87 tick branch and _ftol2_sse
fractional conversion. Four reviewed relocations bind the original 0.99f,
1.01f, 1.0 constants and CRT conversion routine. Keeping this proof in its own
translation unit prevents its private register convention from perturbing
StepStageVectorTween's LTCG caller allocation. Replay it with:
scripts/repo-python scripts/replay-exact-units.py --unit advance-stage-timer


The factor helpers take state in EAX and return ST0; their exact units each
bind one REL32 to 459730; see [STAGE_EASING.md](STAGE_EASING.md).
`SetStageTimer` remains exact with its DIR32 to 004A7948. Verify the family
with:

```bash
scripts/repo-python scripts/replay-exact-units.py --source src/StageTween.cpp
```

All units retain `/TP /O2 /Ob1 /Gy /MT /Isrc /EHsc /GL`; AngleMath is
normal-COFF support. The 2026-11-14 change cold-replayed all 42 units in the
12 families that compile StageTween.cpp or StageScript.cpp, with zero
differences.
