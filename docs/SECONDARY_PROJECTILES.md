# Complete projectile update, homing and special damage

Eleven new complete source mappings cover2,541 target bytes, including update896,
homing498 and special damage246/385. Three public primary-TU owners pass317 bytes
and eight fields. Nineteen new boundary/origin reviews yield17 indeterminate
origins and two VC9 CRT exclusions; one missing ordinary atan2 candidate is added.
All facts below refer to the original hash-pinned Japanese TH11 image. Original
class/header/TU ownership and whole-game runtime remain separate from codegen.

| Owner | Address | Full target bytes | Current result |
| --- | --- | ---: | --- |
| UpdateSecondaryProjectiles | 434440 | 896 | Source908/765 masked diagnostic |
| UpdateSecondaryProjectileHoming | 434E30 | 498 | Source493/401 |
| SampleSecondarySpecialDamage3 | 406E90 | 246 | Source252/204 |
| SampleSecondarySpecialDamage4 | 407220 | 385 | Source385/337 |
| AcquireSecondaryProjectileTarget | 417E70 | 123 | Exact;1 field |
| SecondaryProjectileAngleDifference | 435480 | 101 | Exact;3 fields |
| SecondaryProjectileOutside | 435560 | 93 | Exact;4 fields |
| TrackSecondaryProjectileTarget | 435030 | 73 | Source61/68; corrected stack contract |
| SecondaryPositionLength | 435400 | 47 | Source33/42 |
| SecondaryVelocityLength | 435430 | 48 | Source34/42 |
| SecondaryPositionAngle | 435460 | 31 | Source19/27 |

## Complete896-byte owner

The existing650-limit Ghidra export covers all278 native instructions and the
one RET4; it is not the earlier truncated250-instruction prefix. Independent PE
flow/return/neighbor/reference review finds no table or foreign interior entry.
The full source preserves the256-record loop and all of these paths:

- Kind4/state1 checks signed pattern time/index/slot and HUD/host availability,
  emits both event1 signals and clears the observed slot ID. A separate hit-reset
  gate emits event3, then the per-frame hit flag is cleared before the callback.
- The real update callback receives owner in ECX and record in EDX. Motion uses
  actual FSINCOS or angular/radial updates, normalization and Compose boundaries.
- Primary lookup is genuinely inline and checks both real renderer lists. Missing
  primary animation sets state0, releases only the auxiliary animation and clears
  both IDs. The outside path releases only the primary, clears its ID/state and
  leaves the auxiliary ID alone. These cleanup paths cannot be merged.
- Kind4/early-age skip culling. The real93-byte helper receives the observed crossed
  sprite/scale products. Kind2 has the exact extended window exception; negated
  ordered comparisons preserve its unordered paths.
- Main and optional auxiliary VM positions receive separate double x+32+192/y+16
  operations and z. Optional rotation updates the main angle/flag. The actual
  timer stores previous, advances near-one scale or rounds/scales/truncates, and
  is skipped after lost/culled animations.

The physical view now exposes signed parameter index1C, callback28, pattern timer
930, slot7C90, tracked pointer7C94 and raw slot storage8B8C. Existing damage fields
and all old offsets remain fixed. Slot count/original owner sizes are not invented.
The four affected exact damage helpers remain byte-identical under this header.

The final independent audit agrees on all13 direct-call targets/multiplicities,
the callback and return. Source908/765 still differs in private allocation,
inline lookup/cleanup layout and scheduling; no exact or runtime claim follows.

## Homing and callback interfaces

The498-byte callback preserves state2 exit, acquisition from a copy of position
with y minus double128 and radius80, target flags21/C00000, rounded atan2 inputs
and output, wrapped angle difference, age40 and speed/turn thresholds. Widened
float constants0.1f/0.2f/0.3f/0.04f retain their actual double representations.
The small-angle branch uses a negated ordered comparison, preserving the native
unordered path; no-target acceleration and caps remain distinct.

The native descriptor has a DATA reference at4A3A40 to this function. The diagnostic
probe stores its address in the observed callback field, retaining the real
fastcall ECX/EDX interface. A direct-only probe let LTCG specialize the record
receiver to ESI. Address escape restores the fixed callback contract; it is not
a fabricated calling convention. Complete context changes yield initial495/379,
escaped502/331 and final493/401 with the final math support. The native EBP/aligned
frame and source scheduling remain open. Direct argument-expression spelling
adds no improvement. All five call targets and four returns agree independently.

The73-byte tracked-target callback is a separate contract. Native ECX/EDX reads
and RET4 prove an additional unused stack word that the old two-input hypothesis
missed. Source retains it using the matching hit-callback shape; its original
nominal type/role remains provisional and there is no inferred native registration
site. Source61/68 keeps the correct cleanup width but still lacks the native frame.
No dummy input, aligned local, volatile access or instruction padding forces equality.

## Special damage and corrected old interpretation

Branch3 first rounds the coordinate squared sum and compares against2304 for50
damage. Otherwise it forms the actual float bounds and the enemy's narrow/tall
rectangle, using ordered rejection and returning8/0. The intermediate enemy y
minus224 stays unrounded until each final corner store.

Branch4 scans exactly five IDs at4A3DFC..4A3E0F. The earlier32-entry description
was wrong. Each lookup starts from the VM's embedded node at+10, including the
root, then compares signed script IDs and performs two rounded vector sums,
projection and squared-distance/radius testing for20 damage. It is not just a
child-list traversal. If lookup fails, native clears the ID but still forms
address10 and dereferences it; source preserves that path without a null guard.
Both complete branches remain non-exact despite branch4's equal385-byte extent.

## Three exact owners and natural source attempts

Acquisition123 traverses host4A8D7C/list68, rejects both real actor flag groups,
rounds radius/distance to float and retains the strictly nearest candidate.
Capturing next before the body and advancing at its end recovers all123 bytes.
Advancing immediately gives121/28; a normal for iterator gives121/91, unchanged
by adding real callers. No register annotation or padding is introduced.

Angle difference101 uses double representations of float-rounded pi/tau and
three rounded RET8 paths. Named local double constants introduced an extra
aligned frame and117 bytes. Static constant storage gives101/0, but its private
data symbols have no public MAP bindings in this lane. Final natural literal
expressions preserve101 bytes and emit ordinary compiler __real symbols; all
three addresses and double values are independently reviewed. No Oracle or
static-owner exception is added. Other101-byte copies remain separate owners.

Outside93 uses the observed ordered comparisons at double -192/192/0/448 and
both RET8 paths. Its unordered behavior follows the instructions, not decompiler
boolean rewrites. Equal size or a masked match alone never grants exact credit.

| Owner | Offset | Kind | Independently reviewed target |
| --- | ---: | --- | --- |
| Acquisition123 | 6 | DIR32 | g_SecondaryProgramHost ->4A8D7C |
| Angle101 | 16 | DIR32 | float-rounded pi as double ->497E80 |
| Angle101 | 35,69 | DIR32 | float-rounded tau as double ->497E78 |
| Outside93 | 14 | DIR32 | double -192 ->497EC8 |
| Outside93 | 29 | DIR32 | double192 ->497EA0 |
| Outside93 | 51 | DIR32 | double0 ->497EC0 |
| Outside93 | 67 | DIR32 | double448 ->497EB8 |

All addends are zero. Actual candidate/target constant bytes agree. Strict proof
requires unique complete public primary-TU PDB owners, current source/header/
support/tool/producer/PE/MAP/PDB receipts, exhaustive fields and full zero-byte
comparison; all three owners pass. Sqrt/angle wrappers retain alignment gaps.
A separate vendor symbol check confirms the486AA0 dependency as __CIsqrt; that
additional runtime owner is not counted as a new reviewed exclusion here.

## Expanded origins and vendor atan2

Seventeen newly reviewed non-runtime owners are417E70/123,433BE0/101,433C80/31,
433CA0/35,433CD0/30,435030/73,435080/399,4352A0/132,435330/112,435400/47,
435430/48,435460/31,435480/101,4354F0/52,435560/93,4355C0/20 and435630/110.
Full flows, callbacks, actual inputs/returns/neighbors and incoming refs are
independently checked. Alternate record interpretations remain open for generic
vector/emitter callbacks; those roles do not prove original class/game authorship.

Vendor ordinary atan2 at486C90 and _CIatan2 at486C9A are separate10-byte procedures.
COFF publics and CodeView lengths establish both; the object contains exactly20
text bytes. All four relocations replay with zero differences: descriptor4A80F0
at offsets1/11, ordinary dispatcher48723B at6 and intrinsic dispatcher4871C0 at16.
Direct read-only span review adds the missing ordinary entry; DB memory is unchanged.

The observed libcmt.lib SHA-256 remains
`bd8fe9fa475deb84014af41870182b447bb969c91661d7012ae8d1b26bb6a86c`;
the object SHA-256 is
`13e0fa7ad847f669732273caf46dd82a4c5bd3c32c2ae7688077a5e5670035c4`.
Metadata names CRT 87ctriga.asm, MASM and compid95521E/build21022. These hashes
are observed witnesses, not newly invented pre-existing tool pins. Both entries
are library exclusions, with no source/exact credit and no claim about the full
runtime dispatchers' extents.

## Selected proof and private artifacts

Only three new and four affected old exact units are cold compiled, across
SecondaryProjectileMath and SecondaryDamageSupport. All seven complete owners
pass strict proof with the source freeze unchanged. The first strict new-owner
gate ran before source/boundary registration; after recording the reviewed owners,
the three new units pass using those same cold receipts. Old owners had already
passed; no recompilation or full-cohort replay is needed.

Private evidence: .analysis/secondary-projectile-final-proof.json, full23-owner
independent PE records, complete call audit, variant logs and atan2 COFF/CodeView
records. Compiler artifacts remain in build. No native bytes, vendor objects,
binaries or decompiler dumps are committed. Existing damage1513/1414 is the last
measurement before this expanded header/real-special-helper context; it is not
silently presented as a newly replayed caller. Runtime/product/port work remains later.
