# Secondary emitter reset and VM factories

The complete `ResetSecondaryPlayerEmitters` owner at432CC0 now has maintained
source in `src/SecondaryEmitterReset.cpp`. Its reviewed2508-byte extent includes
2457 code bytes, three alignment bytes and two six-entry switch tables through
43368B. It remains non-exact: the current complete primary contribution is2444
bytes with1967 normalized diagnostic differences. Neither the tables nor the
body are cropped for acceptance.

## Target observations and source

Power divided by the actual power unit selects the emitter count. Maximum-power
processing runs before the same-slot early return: it releases/recreates each
auxiliary ID B4 using the primary-list prepend factory455BC0. Below maximum it
signals event1 to all eight auxiliary IDs without clearing them. A zero slot also
normalizes owner angle8C14 before the equality test.

The main loop preserves the native unclamped count. Each E4 record gets owner
integer coordinates, release/clear of main ID B0, its index and one of six
configuration paths. Remaining records are disabled and signaled; their IDs
remain intact. The owner slot is updated and all eight refresh fields become1.
No blanket record/callback clear or new bounds guard is substituted.

Raw instructions correct several decompiler hypotheses:

- Configuration branch3 reads its focused pair at base148, not78.
- Branch4 writes **seven** integer pairs at64..9B. Its configuration bases are
  28,28,28,78,C8,118,168; the three initial reads are preserved. Only pending54/58
  receives the selected offset; actual5C/60 retains the common owner position.
- Branch2 writes two float pairs at1C/24 and then uses the existing integer
  offsets. It does not initialize those offsets first.
- Dynamic script expressions in branches3/4 remain full32-bit, without the
  decompiler's apparent short truncation. Integer positions use wrapped32-bit
  addition, and each float-to-integer conversion uses the observed double128
  multiplication and truncation.

The containing records remain eight objects at7570..7C8F; coordinate prefix75CC
is record+5C. New physical fields preserve the E4 stride, D0 index, D4 refresh,
E0 callback, owner integer position888/88C and angle8C14. Original C++ class and
header identities remain unknown. Callback entries433690/4337A0 are declared
from their actual ECX publication/call contexts; their private helpers are not
assigned fabricated register signatures.

The current root has the same30 integer-conversion calls, five auxiliary
factory calls and two angle normalizations as the target. VC9 merges its main
factory calls into one location, whereas the target has four. This is a concrete
codegen gap, not complete call-shape or behavioral proof. The no-argument slot
wrapper4355F0 naturally closes its full24 bytes with the actual root present.

## Publisher and factory review

All thirteen following owners (1694 bytes/427 instructions) received direct local
Ghidra full mapped-text attestation plus independent PE instruction/branch/return/
neighbor review. Each
is contiguous, has one return, no owned table and no external interior entry.
The three missed entries were independently separated from the explicit448-byte
455E60..45601F export by their complete returns and CC bookends. That export is
not treated as one function.

| Address | Bytes | Observed role |
| --- | ---: | --- |
| 455760 | 118 | Primary-list prepend publisher |
| 4557E0 | 123 | Secondary-list append publisher |
| 455860 | 118 | Secondary-list prepend publisher |
| 455A70 | 153 | Positioned primary append, explicit layer |
| 455BC0 | 112 | Primary prepend, explicit layer, initialize171 |
| 455C30 | 151 | Positioned primary prepend, layer8 |
| 455CD0 | 167 | Offset-position primary prepend, layer8 |
| 455D80 | 105 | Secondary append, no layer write, initialize171 |
| 455DF0 | 112 | Secondary append, explicit layer, initialize171 |
| 455E60 | 151 | Missed positioned secondary append, layer0 |
| 455F00 | 167 | Missed offset-position secondary append, layer0 |
| 455FB0 | 105 | Missed secondary prepend, no layer write, initialize171 |
| 456020 | 112 | Secondary prepend, explicit layer, initialize171 |

Primary head/tail are renderer7B562C/30, secondary head/tail7B5634/38. All three
publishers link embedded node4 and use shared unsigned counter7BD888, incrementing
again at zero. Prepend retains the observed node.next test after node reset.
The offset-position factories add double32 then192 to x and double16 to y;
these operations are not reassociated. Positioned factories call the distinct
204-byte `BindAnimationScript`44ACD0 directly, preserving position. The171-byte
`InitializeSceneAnimationVm`456910 used by other wrappers clears position and
calls191-byte `BindAnimationFileScript`44ADA0. These contracts are not conflated.

Four complete sources are added in `src/SecondaryEmitterAnimation.cpp`: primary
prepend and secondary append publishers plus their explicit-layer factories.
A natural pair of head tests preserves the prepend insertion and lets the
primary factory retain the native EBX VM. Both112-byte factory contributions
close. The publishers remain open at111/33 and116/47 respectively. The counter
zero-check/add-versus-store and secondary publisher register assignment remain
unresolved. A generic insertion-helper spelling and an externally named registry
type do not close them; no forced registers, barriers or padding are introduced.

Each factory's eleven fields was reviewed independently: runtime flags base
4C3280+590, critical section9 at+8E8, Enter/Leave IAT48B084/88, depth byte+939,
allocator4569C0, initializer456910 and the respective publisher455760/4557E0.
The flags and depth bindings each occur twice. Wrapper4355F0's two fields bind
player global4A8EB4 and the complete reset432CC0. Real source/support definitions
and the existing four-byte AnimationVmId preserve the hidden-result ABI.

All thirteen origins remain **indeterminate**. Useful renderer/gameplay roles,
common lock scaffolding and sibling similarity do not establish original
class/header/library versus game authorship. Nine reviewed factory/publisher
owners remain source-open. Six new mappings cover2997 target bytes; source
presence is separate from exactness.

## Proof and remaining work

The three new canonical units are `reset-secondary-pattern-slot`,
`create-secondary-emitter-animation-vm-id` and
`create-secondary-draw-animation-vm-id`:248 bytes and24 independently reviewed
fields. Acceptance requires complete public primary-TU PDB ownership, current
pinned VC9 cold receipts, full field replay and every byte equal. This context
is diagnostic and does not constitute a runnable native game or original TU
partition proof.

Only these new units and the ten prior exact units whose recorded dependency
sets contain `SecondaryPattern.hpp` are replayed: two emitter-prefix helpers,
one pattern loader and seven projectile callbacks. The full exact cohort is
not replayed. All13 selected cold replays pass638 bytes/41 fields with frozen
source. No Oracle/build infrastructure changes require synthetic suites.

Real Sound support was also tried against complete Spawn433F90/853 with actual
callers. It yields846/733; the real-only context with independent RNG yields
856/690. Neither closes that owner, and neither speculative source variant is
adopted. Further work should use the root's concrete call-merging/counter gaps
and remaining real positioned factory callers, preserving full extents.

The later [controller checkpoint](SECONDARY_PLAYER_CONTROLLER.md) supplies full
controller2888 source, real chain callbacks and exact refresh/angle helpers.
Controller source is complete but non-exact; it is no longer source-open.
