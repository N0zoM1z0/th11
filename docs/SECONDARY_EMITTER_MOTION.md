# Emitter motion and player death

Seven complete source mappings cover1,722 target bytes. Six functions close
1,019 bytes through the public primary-TU linked Oracle; the703-byte death
transition has complete source and remains non-exact. All observations concern
the hash-pinned Japanese TH11v1.00a image and direct local Ghidra.

| Function | Address | Full target bytes | Current result |
| --- | --- | ---: | --- |
| SecondaryEmitterOrbitCallback | 433690 | 12 | Exact linked;1 field |
| OrbitSecondaryEmitter | 4336A0 | 255 | Exact linked;10 fields |
| SecondaryEmitterAimCallback | 4337A0 | 12 | Exact linked;1 field |
| AimSecondaryEmitter | 4337B0 | 495 | Exact linked;20 fields |
| SecondaryDamageAngleDifference | 433BE0 | 101 | Exact linked;3 fields |
| UpdateSecondaryLivesDisplay | 41A060 | 144 | Exact linked;empty field set |
| BeginSecondaryPlayerDeath | 4327D0 | 703 | Source715/423 masked diagnostic |

## Native motion contracts

The actual reset publications and draw callback receiver remain intact. The
orbit callback preserves its ECX input in ESI; aim preserves it in EDI. Ordinary
one-owner helper declarations let VC9 choose those private registers through
the real callers. No fabricated ESI/EDI calling convention is encoded.

Orbit selects radius64 unfocused or24 focused, calls the real paired polar
member433CD0, and converts each component after multiplication by double-128.
The integer differences retain wrapped32 subtraction. Angle advances by
double(float0.10471976), with an actual float argument to NormalizeAngle.
Horizontal values below-0x6600 or above0x6600 wrap by0xCC00, set flag1 and request
refresh. Returning inside the band requests refresh if the prior wrap flag was
set, then clears that flag. All three native returns belong to the255-byte owner.

Aim uses the first emitter, unfocused state and rounded absolute movement above
float0.001. Its actual atan2 inputs are remembered movement8AC/8B0, not current
movement8A0/8A4. Absolute wrapped difference below double(float pi/16) snaps to
the target. Otherwise it recomputes the difference and turns by one third below
double(float pi/4), or one fifth otherwise, with the native normalization and
float stores. Focus selects each emitter's angle/radius pair. Position publication
then follows the same double-128 conversion and player-relative integer difference.

The physical polar receiver follows the12-byte native caller local and the
existing float3 motion receiver usage. The30-byte member writes only x/y using
FSINCOS and leaves z untouched. Its source implementation and original vector
class identity remain open; no separate sine/cosine implementation replaces it.

## Compiler observations and exact proof

AngleMath already has an established normal-COFF context. Retaining that context
as normal support prevents whole-program common-expression elimination across
NormalizeAngle. Compiling it as /GL support instead merges the two observed
angle-difference/normalization calls. The new primary owners remain /GL. No
established LTCG primary source is moved to the COFF Oracle.

A float3 local recovers the native stack slots without padding or constructors.
Separate signed integer conversion locals avoid an unsigned floating conversion
introduced when the cast is nested inside wrapped subtraction. Reading Player()
at the component expression preserves the native global-load schedule; retaining
an earlier player pointer hoists that load. Natural right-wrap assignment order
recovers the remaining arithmetic/flag/refresh stores. Explicit double fabs for
the first returned angle difference preserves its native x87 rounding sequence.

Three cold families prove all six complete primary contributions. The motion
family owns255/495/101, the callback family owns12/12, and the death family owns
the HUD144. Current source/header/support/tool/producer/PE/MAP/PDB receipts,
unique complete public primary ownership, exhaustive35-field replay and zero
full-byte differences are required. All13 emitted literal uses are independently
checked across92 bytes. No extent is cropped and no instruction padding is added.

Original TU partition and normal-COFF/LTCG ownership remain unknown. The linked
diagnostic context does not establish complete game linkage or runtime closure.
No prior canonical family depends on these new source files, so only these six
units are cold-replayed. No broad behavioral or Oracle/infrastructure suite is run.

## Death source and additional reviews

Death preserves score subtraction/floor, fractional-score reset, lives decrement,
the actual HUD call, mode2, state timer reset, status180, primary script0, eight
emitter disables and main/auxiliary signals without clearing their IDs, slot0,
the position-bearing penalty call, spell/scene gates and signed value clamp.
The complete703-byte target includes seven internal alignment bytes. Source715
still differs in callee-save placement, global stores and emitter-loop layout.
Its complete three-target/three-call contract agrees; this grants no exact credit.

An independent HUD/count/fragments probe prevents global-only specialization of
the real HUD inputs. Independent file/script/VM inputs likewise preserve the
real BindAnimationScript contract. These are codegen contexts, not runtime tests.

Two additional boundary/origin reviews cover246 bytes/74 instructions/two RET4:

- HUD41A060 owns144, including12 internal alignment bytes omitted by sparse132.
  Five incoming entry calls, all branches, neighbors and the empty interior-entry
  set are reviewed. Nine434-stride VMs start at HUD10. Full lives get flag2/event2;
  the next icon gets flag2/event(fragments+7), then remaining icons lose flag2.
  No bounds clamp is added before the native full-lives loop.
- Penalty42F6F0 owns102. Its real stack position is loaded into EBX and forwarded
  to422560; the no-argument Ghidra hypothesis missed it. Selection-specific list
  aging, returned effect lifetime floor, timers and color are observed. It remains
  a real source declaration, not a reconstructed implementation.

Both new origins remain indeterminate. Private complete instruction/flow reviews,
compiler variants, field/payload checks and selected cold results live under
`.analysis/secondary-emitter-motion-*` and `.analysis/secondary-death-*`;
canonical artifacts live in `build/match-linked`.
