# Complete secondary damage and collision source

Ten new complete source mappings cover2,557 target bytes, including damage1579
and collision244. Four complete public primary-TU contributions pass361 bytes
and seven independently reviewed fields. Fourteen new boundary/origin reviews
remain indeterminate. These names describe physical contracts; original class,
header and game-versus-reused-engine ownership remain unresolved.

Evidence is the original hash-pinned Japanese TH11 image, direct local full-text
attested Ghidra queries/decompilation hypotheses, independent complete PE decoding
and pinned VC9 RTM21022 compilation. Diagnostic images are never executed.

| Maintained owner | Address | Full target bytes | Current result |
| --- | --- | ---: | --- |
| SampleSecondaryActorDamage | 4347F0 | 1579 | Source1513/1414 masked diagnostic |
| CheckSecondaryActorCollision | 431F70 | 244 | Source228/211 |
| SampleSecondarySpecialDamage | 406720 | 124 | Source83/97; full owned table retained |
| SampleSecondarySpecialDamage2 | 407FC0 | 66 | Source68/62 |
| SignalSecondaryProjectileHit | 40FBE0 | 70 | Exact;2 fields |
| ClearSecondaryProjectileHit | 40FC30 | 70 | Exact;2 fields |
| PositionSecondaryDamageAnimation | 420880 | 35 | Source35/6 |
| AllocateSecondaryDamageRegion | 433A90 | 192 | Exact;3 fields |
| SecondaryDamageTimerElapsed | 433D60 | 29 | Exact;0 fields |
| QueueSecondaryActorSound | 44A260 | 148 | Source144/93 |

## Complete large owner and physical data

The1579-byte damage owner has510 instructions, two RET8 paths, one callback and
13 internal alignment bytes absent from the sparse1566 count. The existing
650-limit export covers its complete body. Ghidra's four-argument fastcall
prototype includes unused incoming registers; both actual callers and all stack
reads establish two real stack arguments: position and two-component extent.
The maintained interface preserves these observed inputs without dummy arguments.

The new physical views preserve these verified offsets:

- Global944 starts the five-field damage timer. Equal current/previous returns0.
- Global96C starts256 projectile records of6C bytes. Each record has a timer of0x14
  bytes and a motion record of0x34 bytes, state48, animation ID4C, auxiliary ID50, target54,
  per-frame hit58, signalled-hit5C, damage60 and parameters pointer64.
- Parameter width/height are0C/10, kind1D, signed script20 and callback30.
  The actual callback receives owner in ECX, record in EDX and position on stack.
- Global7C9C starts32 region records of74 bytes. Radius0, growth4, angle8,
  width/height10/14, motion18, timer4C, damage60, accumulated64, limit68,
  period6C and flags70 are independently decoded.
- The earlier partial actor-reference view at global9C0 is record96C+54: the
  projectile's target pointer. It does not imply that the whole projectile record
  is an actor. Existing shared headers need no speculative replacement.

All record sizes/used offsets are checked in the source. Unknown storage is
retained without inventing field semantics or original complete owner sizes.

The first loop rejects inactive/state2 projectiles, forms float-rounded rectangle
corners, applies ordered out-of-bounds/plane rejection, invokes the optional
callback and signals event2 only once. It preserves kind4's timer-period damage
predicate, kind1's damage decay, kind2's positional sound and kind3's region
allocation. Other kinds replace the animation using the actual signed script,
active-owner offset, layer0x16, copied angle/position, state2 and speed scaling.
No null guards or simplified damage-only path replace native unchecked behavior.

Special damage is then accumulated through the real406720 dispatcher. The second
loop preserves active/period gates and separate axis-aligned, rotated and circular
region tests. It adds region damage to both total and accumulated value, then
zeros the region's damage when the signed limit is reached. Its period condition
skips changed frames divisible by the period; this is intentionally different
from kind4 projectile damage's elapsed-period predicate.

The tail preserves wrapped32-bit accumulation and the observed signed compression:
for total>=30, wrapped(3*total-90)/5+30; if the new value>=70, (value-50)/5+50;
if that reaches80, cap80. Nonzero damage adds `(damage/10+10)/10` to the signed
wrapped score and caps only results>=1,000,000,000 at999,999,999. No lower clamp,
overflow saturation or generic score helper with another division is substituted.

## Floating-point observations and compiler attempts

Native x87 comparisons establish ordered rejection, rather than conjunctions of
`<=` hit tests: an unordered comparison must not silently become a miss. The
collision thresholds similarly use a negated ordered comparison, with strict
boundary behavior and observed unordered paths. Decompiled boolean formulas are
hypotheses; the FCOM/FNSTSW/test/branch instructions determine these conditions.

Distance coordinate differences are kept on x87 until the sum-of-squares float
store. Graze expansion first rounds radius/2.5 to float and applies minimum40;
shape radius plus this value remains unrounded until squaring. Independent
`float dx`, `dy` or expanded-radius assignments introduced extra source stores
and were removed in favor of the actual expression trees. Rotated-region delta,
sin/cos returns and rotated output do retain their observed float boundaries;
real VC9 float overloads are used. No aligned local, volatile access, fabricated
ABI, instruction bytes or padding is introduced to force a frame.

Initial full source emits1494/1427 and collision264/235. Correct expression trees
give1478/1417 and228/211. Explicit pointer/countdown traversal, matching the native
256/32-record loops, gives current1513/1414; reversed predicate spelling changes
neither collision result. Native damage EBP/8-byte-aligned frame0x54 and source
frame/register scheduling remain different. These are complete-owner diagnostics,
not prefix matches or exact/runtime claims.

An independent final call audit agrees on all14 direct damage calls and their
multiplicities, the single callback and both returns. Collision agrees on its
one transition call and all three returns. This supports complete source flow,
not full game behavior or byte equality.

## Exact helper evidence

The70-byte event helpers write word37C values2 and3 respectively, propagate through
related nodes only when marker18 is null, and use the real ID lookup. They do not
clear IDs on lookup failure or change release flags. Both include three native
alignment bytes; source67-style cropping is never used. An independent renderer
probe input keeps the lookup's actual two-input contract intact.

The29-byte timer member requires current!=previous and signed current%period==0.
ECX receiver/stack period and both RET4 paths are observed directly. The physical
timer view introduces no constructor, inheritance or guessed guard.

The192-byte allocator scans32 regions and returns the one-past-array pointer when
all are active. A free slot gets the genuine0x74-byte clear and0x34-byte motion clear,
flags3, copied position/radius/growth, conditional timer initialization, signed
frame/fraction/previous, damage, accumulated0, limit999999 and period4. A natural
local reference to its timer stops premature flag-load scheduling and closes all
192 bytes. A pointer or ordinary timer method also closes the private probe;
a position snapshot alone leaves24 differences. Only the local reference is
adopted. No shared timer header changes or forced scheduling barriers are needed.

| Owner | Offset | Kind | Independently reviewed binding |
| --- | ---: | --- | --- |
| Event2/Event3 | 4 | DIR32 | g_RenderBatch ->4C3268 |
| Event2/Event3 | 10 | REL32 | FindAnimationVmById ->4561E0 |
| Allocator192 | 45 | REL32 | memset ->46A720;0x74-byte clear |
| Allocator192 | 62 | REL32 | memset ->46A720;0x34-byte clear |
| Allocator192 | 132 | DIR32 | g_AnimationTimeScale ->4A7948 |

All addends are zero. Strict cold proof validates unique complete public
primary-TU PDB ownership, current source/header/support/tool/producer/PE/MAP/PDB
receipts, exhaustive fields and full zero-byte comparison for all four owners.

## Expanded boundary and origin review

The dispatcher406720 owns124=100 code+24 bytes of six-entry jump table406784,
through40679B. All six apparent foreign DATA refs into case blocks come from its
own table; every target is a reviewed instruction boundary. The old100-byte span
omitted this ownership. Current source83 emits another complete shape and stays
open; no100-byte clipped unit is registered.

Fourteen new full reviews cover406720/124,406E90/246,407220/385,407FC0/66,
40FBE0/70,40FC30/70,420880/35,432A90/546,433A90/192,433D60/29,4342F0/142,
434380/183,434440/896 and434E30/498. The896-byte projectile update has278
instructions; a new650-limit export explicitly replaces the truncated250 prefix.
Full flows/returns/actual incoming refs/neighbors, callbacks and the owned table
are independently checked. Pattern dispatch, input/timers,256-projectile motion
and homing callbacks get individual indeterminate-origin reasons. Generic engine,
renderer, geometry or gameplay roles are not proof of authored game origin.

## Replay scope and private evidence

Actual old receipt source hashes select zero affected prior exact units: existing
source/shared headers are unchanged. Only the four new canonical units are cold
replayed, all passing after the final source freeze. No full cohort, broad runtime
suite or infrastructure regression is run for these source changes.

Private evidence remains under.analysis/secondary-damage-* and
build/probe-ltcg/SecondaryDamage*/. Canonical artifacts live in
build/match-linked/SecondaryDamageSupport/. Native bytes, binaries, objects,
compiler payloads and decompiler dumps are not committed. Runtime/product and
portability closure remain later work.

Later complete projectile/homing and special246/385 source, five-ID correction,
callback stack-contract correction and three new exact math proofs are recorded
in [SECONDARY_PROJECTILES](SECONDARY_PROJECTILES.md). Damage/collision/sound measurements above belong to the earlier context
before the expanded shared projectile view.
