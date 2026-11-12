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
DC update/E0 draw callbacks, owner integer position888/88C and angle8C14. Movement
review corrects orbit/aim publication to DC; see [PLAYER_MOVEMENT](SECONDARY_PLAYER_MOVEMENT.md).
Original C++ class and
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

## Direct-position primary factory source (2026-11-12)

`CreateDirectPositionAnimationVmId` (455A70, complete 153 bytes) now has
maintained C++ in `src/AnimationVmPositioned.cpp`. It locks runtime critical
section 9 conditionally, allocates a VM, sets persistent layer 0x20 and bit
0x40000000, copies all three supplied world-position components into the
persistent vector at 0x3E8, then calls the **direct** `BindAnimationScript`
(44ACD0) and the primary append publisher (4556E0). The distinct initializer
at 456910 would erase this position and is deliberately not used.

A scoped VC9 `/GL` link with the real `StartMenuTransitionOverlay` producer
at 40FCA0 and its stack-constructed position gives a complete **135-byte
primary TU diagnostic** against the native 153-byte owner, both with five
calls. Its caller specializes script/layer values and the factory's private
position/return ABI, so this is **source-only**, not a strict match. The
bounded build and PE/MAP/PDB comparison script are retained privately under
`.analysis/probe-positioned-vm-20261112.py`; the public producer context is
`probes/animation-vm-positioned-context.cpp`. Native boundary and caller
reviews remain as documented above.

## Positioned primary prepend factories (2026-11-12)

`src/SecondaryEmitterPositioned.cpp` now implements both formerly source-open
factories with complete flow: 455C30 (151 bytes) and 455CD0 (167 bytes).
Both conditionally enter runtime critical section 9, allocate one VM, enable
bit 0x40000000 and fix layer 8. The first copies the caller's complete XYZ
position to VM+0x3E8; the second adds double 32 then 192 to X, double 16 to Y,
and preserves Z. Both then use **BindAnimationScript 44ACD0**, not the
position-clearing initializer, and publish to the primary prepend list at
455760 before conditionally releasing the lock. This is distinct from the
112-byte zero-position factory at 455BC0.

A scoped pinned VC9 `/GL` PE/MAP/PDB comparison, including the real publisher
source and dynamic position/script arguments, yielded **138/151** and
**156/167** bytes, with five calls in each candidate and target. Register and
private producer scheduling remain non-exact, so neither is admitted to the
strict match ledger. Private comparison details are under
`.analysis/probe-secondary-emitter-positioned-20261112.py` and the public
producer context is `probes/secondary-emitter-positioned-context.cpp`.

## Exact 105/112-byte secondary-front factories (2026-11-12)

The formerly source-open secondary **head-prepend** publisher at 455860
and its 455FB0/456020 factories now have natural C++ in
`SecondaryDrawFrontAnimation.cpp`. The publisher inserts VM+4 at the
secondary chain head (renderer+7B5634), setting the tail at +7B5638 only
for an empty list, and publishes the wrap-avoiding ID at +7BD888.
Its complete native owner is 118 bytes, 33 instructions and one renderer
DIR32 field. The current scoped publisher is **111/118 bytes**, 32
instructions, with a different counter-zero branch; this remains
**non-exact**, independent of its two accepted callers.

The no-layer factory at 455FB0 (105 bytes) and explicit-layer factory at
456020 (112 bytes) each conditionally enter critical section 9, allocate a
VM, set bit 0x40000000, call the **position-clearing** initializer at
456910, publish through 455860 and release the critical section if enabled.
The explicit-layer owner additionally writes VM+0x20. These differ from the
position-preserving factories at 455C30/455CD0.

An opaque initializer initially generated stack calls and left 5/8 nonfield
byte mismatches, even though both factory extents matched. Adding the actual
`AnimationVmFactory.cpp` TU and `SecondaryEmitterAnimation.cpp` producer to
the dynamic factory caller restores native EAX/EDX register arguments. The
strict **complete primary-TU** VC9 `/GL` family reproduces **105/105 and
112/112 bytes** with **24/24 and 26/26 instructions**, respectively; each
has five calls. All **22 relocation/address fields** were individually
compared to the original Japanese 1.00a mapped PE: for each function,
DIR32 fields to runtime flags (+590), critical section 9 (+8E8), depth
byte (+939) and the Win32 Enter/Leave IAT, plus REL32 calls to allocator
4569C0, real initializer 456910 and publisher 455860. Complete cold linked
PE/MAP/PDB, unique public primary-TU ownership and full field replay return
**zero differing bytes** for both independently registered exact units.

Canonical units: `create-default-front-secondary-draw-animation-vm-id` and
`create-front-secondary-draw-animation-vm-id`; recorded proof
`.analysis/front-draw-vm-strict-20261112.log` and independent full JSON
comparisons under `.analysis/front-draw-*-strict-20261112.json`. Only these
and the directly affected existing header consumers need cold replay, not
the full cohort. The unresolved 118-byte publisher continues as a separate
full-extent codegen target.

## Secondary drawing append factories and exact default owner (2026-11-12)

Two more independently reviewed **full** owners now have maintained C++ in
`src/SecondaryDrawPositioned.cpp`: `CreatePositionedSecondaryDrawAnimationVmId`
at 455E60 (151 bytes) and `CreateOffsetSecondaryDrawAnimationVmId` at 455F00
(167 bytes). Both allocate under conditional runtime critical section 9,
set bit 0x40000000 with **layer zero**, retain XYZ through direct
`BindAnimationScript` 44ACD0, and publish with the **secondary append**
owner at 4557E0. The offset version preserves distinct double `+32`, `+192`
and `+16` additions with native float stores; it is not a reassociated
`+224` float operation.

Under a real `AnimationVm.cpp` script binder and the real
`SecondaryEmitterAnimation.cpp` publisher, the complete scoped VC9 `/GL`
contributions match native extents and instruction totals exactly:
**151/151 bytes, 39/39 instructions** and **167/167 bytes, 41/41
instructions**, each with five calls. Only **two nonfield bytes per owner**
remain different, at +0x0E and +0x5A / +0x6C. These are the original
hidden-result/file pointer stack-slot order; three dynamic result consumer
variants (conditional, stored-output and sequential) retain the same gap.
Because this is an original private ABI question, we do not swap source
parameters or misdeclare a convention to force equality. Both remain
**source-only, non-exact**. Private scoped evidence:
`.analysis/probe-secondary-draw-positioned-20261112.py` and
`.analysis/try-draw-positioned-consumers-20261112.py`.

The neighboring `CreateDefaultSecondaryDrawAnimationVmId` at 455D80
(previously source-present in `PauseContinueMenuSupport.cpp`) is now
**canonical strict exact: 105/105 bytes, 24/24 instructions and five calls**.
The native initializer's private ABI only reappeared with genuine
`AnimationVmFactory.cpp` and secondary append publisher source in an
independent live script caller. Its unique primary-TU PE/MAP/PDB owner and
all **11 independently reviewed fields** match. These include runtime flags,
critical section/depth, two Win32 lock IAT entries, the original allocator
4569C0, initializer 456910 and append publisher 4557E0. The cold receipt
and complete zero-difference field replay are retained in
`.analysis/default-secondary-draw-strict-20261112.log`; canonical unit
`create-default-secondary-draw-animation-vm-id`. The changed shared factory
header is checked only against its exact direct consumers, not the full
cold cohort.

## Front publisher counter scheduling experiment (2026-11-12)

The still-open 118-byte secondary head-prepend publisher at `0x455860`
produces 111 bytes/32 instructions with ordinary unsigned increments. Its
exact native first 0x55 bytes and tail differ at the ID counter: native
emits `ADD [EDX+0x7BD888], ECX; CMP [EDX+0x7BD888],0; POP ESI; JNE; ADD`,
whereas VC9 reuses the add flags and folds the wrap-recovery write to a
single `MOV`. Eight natural C++ counter variants were compiled under the
same real initializer/publisher-producer context. Ordinary `+=`, a named
one-constant and testing the returned increment all kept **111/118** bytes.
Making the counter member/reference `volatile` emitted an extra saved
register and produced **119/118**, 35 instructions, rather than the native
33. The current 118-byte target remains **non-exact**. Crucially, both
independently accepted 105/112-byte front factories stayed zero-difference
through every experiment; no established canonical source was replaced.
Private script and evidence:
`.analysis/try-front-publisher-counter-20261112.py` and its log.
