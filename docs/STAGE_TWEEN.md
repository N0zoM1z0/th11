# Stage interpolation and timers

Nine complete functions now have natural C++ source in `src/StageTween.cpp`.
Direct local Ghidra review attested the mapped text hash and reconciled full
flows, returns, references and neighbors. These nine owners are contiguous,
without embedded tables or external interior references.

| Function | Address | Target bytes | Source bytes | State |
| --- | --- | ---: | ---: | --- |
| StepStageVectorTween | 00405240 | 883 | 887 | Non-exact (267/280 aligned) |
| StepStageFogTween | 004055D0 | 737 | 794 | Non-exact |
| StageVectorTweenFactor | 00405960 | 39 | 39 | Exact linked unit |
| StageFogTweenFactor | 00405990 | 45 | 45 | Exact linked unit |
| SubtractStageFog | 004059D0 | 61 | 61 | Exact linked unit |
| ScaleStageFog | 00405A10 | 64 | 64 | Exact linked unit |
| AddStageFog | 00405A50 | 61 | 61 | Exact linked unit |
| PackStageFog | 00405B70 | 171 | 171 | Exact linked unit (now in this TU) |
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
- The terminal endpoint uses `if (mode != 7) return target; return initial;`.
  The ternary form modified EBX and forced the state into EAX with an extra
  save; separate returns let the state arrive in EBX as in native.
- Modes 7 and 17 copy `initial` into a local before the first addition. Native
  copies the whole value with `rep movsd` before adding; operating on
  `self->initial` directly does not.
- Hermite coefficients use the float `progress` directly with `2.0 * progress`
  and `a = (2t+1) * ((t-1)*(t-1))`. This reproduces native's `fadd st0,st0`,
  the square-before-sum scheduling and the shared `2t` subexpression.

Fog stepping is now 794/737 bytes; everything matches structurally except one
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

Vector stepping: with explicit float weight locals in mode 8 (the form that
matches the animation int-vector step) instead of the reference-output helper,
the candidate is 887/883 bytes with 267/280 instructions aligned (previously
234). Commuting operands inside the add helper or building `result`
member-wise changes nothing. Remaining gaps: mode 7 stores the output
before `initial` (native looks like NRVO into the hidden result), and mode 8
evaluates all four coefficients before scaling, as D3DX-style inline vector
operators would. The next step is a small `CameraVector`-local helper type with
inline D3DX-style operators for this TU, without changing the shared
aggregate.

AdvanceStageTimer keeps its earlier eight-byte INC/LEA difference.

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
