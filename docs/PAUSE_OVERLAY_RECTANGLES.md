# Pause overlay rectangle queue and VM capture

Two native owners adjacent to the game-over menu at `42F400..42F466`
(103 bytes) and `42F480..42F4E4` (101 bytes) now have natural C++
implementations in `src/PauseOverlayRectangle.cpp/.hpp`. The **103-byte
queue is now strict linked-PE exact**, while the 101-byte VM capture
remains non-exact. Original class/TU/ownership is unresolved.

## Original boundaries and data

Hash-pinned direct Ghidra reviews, original-PE control flow and callers
show a complete rectangle queue function at 42F400. It iterates four
40-byte physical draw slots at the supplied renderer base, checking
a signed texture identifier for the first negative/free slot. On success
it stores the texture/sprite identifiers, fixed min/max bounds
(32,16,384,448), and the four caller coordinates. Both the successful
and exhausted paths return zero, without an overflow fallback. There
are two RET24 exits and one native caller at 42F4D9; CC bytes follow
the complete 103-byte extent.

The 101-byte capture function at 42F480 is called from four native
pause menu routines (42C6A2, 42C7E9, 42D61F and 42D75E). It resolves
an animation VM through `FindAnimationVmById` and the
`g_RenderBatch` singleton. Native VM offsets +0x3AC and +0x3B0
identify its sprite and texture pointers. The sprite's left/top values at
+0x0C/+0x10 and height/width values at +0x34/+0x38 are each converted
through the target's floating-to-integer runtime helper at 4864E0
before the queue call. The native path writes zero into its local
animation-ID argument when lookup fails but still dereferences the
returned VM; the maintained source retains that non-guarding bookkeeping.
No foreign interior function references or jump tables were observed.

## Narrow codegen observations

Pinned VC9 normal-COFF compilation and a scope-limited VC9 /GL PE/MAP/PDB
link through `probes/pause-overlay-context.cpp` both succeed.

| Owner | Native bytes | Normal COFF | Linked PDB |
| --- | ---: | ---: | ---: |
| QueuePauseOverlayRectangle | 103 | 107 | **103, exact** |
| CapturePauseOverlayFromVm | 101 | 101 | 100, non-exact |

The capture COFF has eight fields; the queue's COFF has none.
The target's queue receives its renderer in EDX with six stack
coordinates and RET24, while the maintained source declares ordinary
C++ arguments so private compiler allocation can emerge naturally in
a real linked caller. The rectangle queue now has a canonical cold linked-PE exact proof:
the independently verified 103-byte primary PDB contribution has **zero
relocation fields**, and all **103/103 bytes** match the original
without normalization or masking. Unit: `pause-overlay-queue-rectangle`.
The input/output ABI emerged naturally from the queue's C++ caller
context, not from a fabricated register-shim declaration. Only the
capture function still lacks exact credit.

The original review and diagnostic objects are held under
`.analysis/pause-overlay-*` and
`build/probe-ltcg/PauseOverlayContext/`. Do not replay an unrelated
cold cohort. Next: inspect the four real native caller contexts to
recover optimized register ownership without inventing signatures.

## Queue codegen convergence (2026-11-11)

An initial interleaved scan-and-fill implementation compiled into a
95-byte VC9 /GL primary PDB contribution, compared with 103 bytes native.
Keeping the slot scan index and cursor distinct, then filling the chosen
indexed slot after a genuine early-return path, reproduces native branch
placement and all field stores. The final ordinary C++ uses a
`goto insert` shared by the first-free-slot scan; no native assembly
or inert padding was inserted. The source is preserved in
`src/PauseOverlayRectangle.cpp`, not in the ignored experiment alone.

The canonical `pause-overlay-queue-rectangle` source is cold linked with
`probes/pause-overlay-context.cpp` and its two legitimate compile-only
entrypoints: VM capture and a generic receiver-driven queue call. The
result is a non-runnable diagnostic image, and the genuine production
TU partition is still unknown. The strict field-complete comparator
independently validates the unmodified 103-byte owner and unique
PE/MAP/PDB contribution. The adjacent capture function remains non-exact;
its corrected float conversion order now follows the native
**height -> width -> top -> left** sequence.

## Capture register-allocation trials

The adjacent 101-byte VM capture was separately cold-linked in an isolated
probe with the already exact `FindAnimationVmById` implementation supplied
from `StageMesh.cpp`. The native routine retains renderer in EBP and sprite
texture index in EBX across four calls to `__ftol2_sse`. The faithful source
currently links to 100 bytes with two generic probe callers, or 99 bytes
with the native finder and a cached renderer.

A focused experiment forcing an early volatile read of the texture index
gave a **101-byte linked candidate**, but its registers are reversed:
renderer EBX and index EBP instead of the native EBP/EBX. The full machine
code is **not identical**; size equality does not confer exactness.
A separate explicit float temporary placed the native height load ahead
of the texture-index load, but forced an extra float stack round-trip
and 111 bytes. Neither experiment was copied to the maintained source.
The source continues to use the real `AnimationSpriteView` offsets
and ordinary ABI, without fabricated register shims.

Local diagnostic sources/logs are under
`.analysis/pause-overlay-capture-*` and the limited images below
`build/probe-ltcg/PauseOverlayCapture*`. No other exact cohort was replayed.
