# Pause overlay rectangle queue and VM capture

Two native owners adjacent to the game-over menu at `42F400..42F466`
(103 bytes) and `42F480..42F4E4` (101 bytes) now have natural C++
implementations in `src/PauseOverlayRectangle.cpp/.hpp`. They remain
**non-exact**, and the original class/TU/ownership is unresolved.

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
identify its sprite and texture pointers. The sprite's X/Y values at
+0x0C/+0x10 and right/bottom values at +0x34/+0x38 are each converted
through the target's floating-to-integer runtime helper at 4864E0
before the queue call. The native path adds no null checks; neither
does the maintained source. No foreign interior function references
or embedded jump tables were observed.

## Narrow codegen observations

Pinned VC9 normal-COFF compilation and a scope-limited VC9 /GL PE/MAP/PDB
link through `probes/pause-overlay-context.cpp` both succeed.

| Owner | Native bytes | Normal COFF | Linked PDB |
| --- | ---: | ---: | ---: |
| QueuePauseOverlayRectangle | 103 | 99 | 100 |
| CapturePauseOverlayFromVm | 101 | 93 | 86 |

The capture COFF has eight fields; the queue's COFF has none.
The target's queue receives its renderer in EDX with six stack
coordinates and RET24, while the maintained source declares ordinary
C++ arguments so private compiler allocation can emerge naturally in
a real linked caller. These source sizes and comparisons grant
**no exact credit**.

The original review and diagnostic objects are held under
`.analysis/pause-overlay-*</bt> and
`build/probe-ltcg/PauseOverlayContext/`. Do not replay an unrelated
cold cohort. Next: inspect the four real native caller contexts to
recover optimized register ownership without inventing signatures.
