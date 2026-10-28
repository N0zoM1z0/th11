# Stage drawing

`src/StageDrawing.cpp` supplies the complete main stage draw and overlay
callbacks. Only the original Japanese v1.00a target is evidence. Direct local
Ghidra queries attest the entire mapped text section. The decompiler's overlay
fastcall hypothesis is wrong: both functions consume one stack argument and
return with `RET4`; incoming ECX/EDX are not parameters.

| Entry | Reviewed owner | Source contribution | State |
| --- | ---: | ---: | --- |
| DrawStage,0x00403130 | 1,128 bytes through0x00403597 | 1,106 bytes | Non-exact |
| DrawStageOverlay,0x004035A0 | 854 bytes through0x004038F5 | 852 bytes | Non-exact |

Both owners are contiguous, include their internal alignment, have no embedded
tables and have no external interior references. They are separated from the
preceding/following entries by CC padding. Their only reviewed entry references
are the lifecycle callback adapters. Origin is authored game logic; original
TU partition remains unknown. Diagnostic address-masked differences are754
and526 positions respectively; shifted instructions make these counts unsuitable
as progress percentages. No canonical exact units have been added.

## Fields and behavior

Stage+2324 is an unsigned draw counter, incremented only when nonzero and the
main layer path runs. Stage+2328 is a packed tint, with alpha at+232B. Stage+232C
selects the accent tint. The vector+2FE4 is cleared before main layer drawing.
These replace unknown storage without changing any offsets or the0x3158-byte
class size. Camera+FC/+100 contain fog-start/end float bits; camera+114 contains
fog color. All stage camera0x118 bytes are copied into runtime+434, including
unknown storage. Before copying, stage camera offsets are replaced with the
current runtime offsets. Fog state cache lives at runtime+990; active camera
and index live at+54C/+550.

Both functions return1 immediately for flags2FF0 mask0x08. Otherwise, camera and
initial device setup run unless mask0x04 is set and the signed timer current is at
least60. Stage setup flushes, copies the camera, applies perspective, sets the
viewport and publishes camera index2. Each render-state update has its own
preceding flush, even when the requested state already holds. Fog-enable uses
the separately observed cache comparison. HRESULTs are ignored.

The main callback enables Z writes, selects LESSEQUAL and sets fog parameters.
It clears target and Z within the runtime viewport rectangle. Clear color is
zero when mask0x04 is set and signed stage age+300C is below34; otherwise it uses
camera fog color. Unsigned viewport additions retain wraparound. With mask0x04 set,
timer values below30 create a transition `(3,30,0,0,10)`, set mask0x01 and reset the
timer to1 on every invocation. Values at least30 zero tint alpha and clear mask0x01.
Do not turn this into a one-time transition.

If tint alpha is nonzero, the main callback publishes tint and enables it in
the renderer tail+7BD88C/+7BD890. With mask0x01 set, a nonzero first embedded
VM+3AC enables the background VM pass: select runtime camera2, disable fog,
flush, disable Z writes, visit all eight VMs and draw each nonzero+3AC entry,
restore Z writes and reapply stage perspective. If the first VM is inactive,
the whole embedded pass is skipped even if later VMs are active. Fog is enabled
for explicit layers0..7; the unsigned draw counter then increments if nonzero.

The overlay sets fog off, disables Z writes, draws animation group25 with
ALWAYS and group26 with LESSEQUAL, then reapplies LESSEQUAL and fog parameters.
Mask0x04 with timer at least30 zeros tint alpha without clearing mask0x01 here.
Mask0x01 enables fog and explicit layers8..11. Both functions restore renderer
tint to disabled/80808080, or enabled/FF404040 when accent is nonzero.

Only the overlay decrements a positive timer, through the reviewed add-timer
boundary with amount-1. When its reloaded signed current becomes nonpositive,
it first writes alpha FF, conditionally sets mask0x08 when mask0x02 was set, clears masks0x02
and0x04, and finally overwrites the entire tint with00FFFFFF. Preserve the final
zero alpha and the write order. The overlay retains its captured renderer
pointer across timer adjustment for the following flush. The main callback
similarly preserves or reloads its renderer at the observed call boundaries.
Both tails disable Z writes and select ALWAYS; overlay finally disables cached
fog.

Fog-start/end are copied by reference into DWORD arguments. Passing a float
by value allowed VC9 to emit an x87 load/store that quieted a signaling NaN;
the original transfers these bits with integer loads. The by-reference memcpy
retains the original bits without aliasing violations or instruction tricks.

## Reproduction and limits

Private `.analysis/probe-stage-drawing.py` cold-builds the maintained public
primary TU with pinned VC9, `/O2 /Ob1 /Gy /MT /EHsc /GL`, and the two-stage
caller `probes/stage-drawing-context.cpp`. Existing render-batch, perspective
camera and runtime-fog source supply their real compiled call contracts.
Remaining functions are unresolved diagnostic data anchors; the executable
is not runnable as a product.

Private `.analysis/stage_drawing_fixture.py` executes both complete original
and candidate main bodies under Unicorn. External drawing, camera, timer and
COM methods are explicit models, including the native private register versus
ordinary source stack argument differences. Checks compare ordered calls and
arguments, whole0x3158-byte stage state, runtime state, active camera, both
renderer tints, return values, stack and nonvolatile registers. Cases cover
flags, signed timer and age boundaries, tint and VM activation combinations,
counter wrap, ignored HRESULT failures, signaling/quiet NaNs, viewport overflow,
camera redirection and renderer replacement during callbacks. Null stage and
null renderer faults are compared as faults. Three-frame main/overlay sequences
also run against persistent fixture state. All952 cases agree, including four
matching null-pointer faults; the fixture peak RSS was84,952 KiB.

`StageDrawing.hpp` declares the reviewed external boundaries at405E80,
406100,4489E0,459210,455660,451EF0 and404190. They are not source coverage.
Timer operations, animation groups and stage layers remain modeled dependencies;
these results do not establish whole-game native runtime equivalence. Update
402AA0 is still unimplemented. Full update disassembly was retained privately
with this review; it remains a useful1,677-byte next target.
