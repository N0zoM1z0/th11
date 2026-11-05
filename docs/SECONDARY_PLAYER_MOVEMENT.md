# Complete player movement and emitter update publication

Movement430290 now has complete maintained source in SecondaryPlayerMovement.cpp.
Its previously reviewed2,896-byte owner retains2,829 code bytes, three alignment
bytes and two eight-entry tables through430DDF. Sparse Ghidra2811 missed18 internal
alignment bytes. The source preserves the EAX0/RET4 contract with one real owner;
the earlier void declaration is corrected. Source presence grants no exact credit.

## Distinct callback slots

Reset's orbit/aim publications432FDE/43317E write `[EDI+74]` with EDI at player75D8:
the slot is764C, or emitter7570+DC. Movement reads `[EDI+7C]` with EDI at75D0 and
calls it using the emitter base as ECX. Draw431BC0 instead starts its callback
slots at7650 and subtractsE0 to obtain that same emitter base. These are distinct
DC update and E0 draw callbacks. The earlier shared view left DC unknown and
published orbit/aim into E0; both publication sites and the physical view are
corrected. Emitter sizeE4 and all existing position/ID offsets remain intact.

The prior orbit/aim primary bodies are unchanged. The support publication fix
and movement return declaration change require selected cold replay of actual
receipt dependencies, not new exact credit or a claim about original TU ownership.

## Full native movement contract

- Direction masks preserve diagonal precedence50/60/90/A0 before20/10/40/80.
  Focus requires the host's enabled70 and auxiliary timer95C at least4; otherwise
  focus clears and emitter blend8BA0 becomes30. Fire states99/100 use horizontal
  speed-150/+150 and skip focus VM creation/release.
- Normal/focused cardinal and diagonal integer speeds occupy890/894/898/89C.
  Focus VM756C uses script4A/layerB. Unfocus validates and signals event1 before
  clearing that ID. Animation transitions use scripts1..4 or1B..1E according to
  flags bit2 and previous horizontal direction914. Real post-bind field reads
  remain observable; the branches cannot be reduced to one pre-bind snapshot.
- Selection2 doubles integer steps after more than10 released fire/focus frames,
  creates script13, binds the actual signed sprite word at player3B0 through the
  found VM file and publishes its position. This word is not VM.spriteIndex374.
  Scaled current movement8A0/8A4 retains z; remembered movement8AC copies all three
  components only when direction is nonzero. Separate signed truncations produce
  steps8B8/8BC before wrapped fixed-point position accumulation.
- Selection0 calls the actual horizontal-wrap helper430E50. Selection1 calls
  item attraction4245E0 after more than10 still/released frames. Both helpers now
  have complete source; see [WRAP_ITEMS](SECONDARY_PLAYER_WRAP_ITEMS.md). For fire state below99, x clamps to
  +/-5C00 and y to1000..D800; horizontal selection0 boundary contact also starts
  fire state1/3 and clears924. Float position uses double1/128.
- Focus VM position snapshots separate double x+32+192 and y+16 before raw
  publication. Selection4 with unblocked HUD cycles signed multiplier modulo5
  on repeat400, replaces active emitter VM IDs, and copies each of eight selected
  integer pairs into both normal/focused offsets. No new slot/index clamp is added.
- Flag8 increments collapse time. Each active emitter either receives focused/
  normal offsets and its DC update callback, or moves toward the owner during
  collapse. At collapse time30 it disables and signals both VMs without clearing
  their IDs, clears the slot, and skips position publication. Otherwise refresh
  snaps both coordinates; blend at least30 applies wrapped difference*blend/100,
  snapping only when both integer increments vanish. Two real VM publications
  share one float3 position snapshot with z0. Renderer recapture follows them.

## Event helper and evidence

SignalSecondaryAnimationEvent456230 has complete source for67 bytes including
six internal alignment bytes omitted by sparse61. Independent review covers
20 instructions, one RET4, one FindAnimationVmById call,42 incoming entry refs,
all branches/neighbors and no foreign interior entry. The stack ID and SI event
word are actual inputs. It writes VM37C and propagates to related nodes when
marker18 is zero, without clearing the ID. Origin remains indeterminate.

Private evidence includes full fresh direct Ghidra movement/helper exports,
the independent event decode and callback-address arithmetic, complete linked
compiler diagnostics and selected cold replay under `.analysis/secondary-movement-*`
and `.analysis/secondary-player-movement-*`. No runtime harness or broad behavioral
suite is required or run for these codegen/layout questions.

## Compiler checkpoints

The first complete movement source emits2724 bytes with2421 masked diagnostic
differences. Its animation-base expression leaves four BindAnimationScript sites
versus the native five. Explicit normal/alternate animation branches emit2868/
2393 but retain seven sites. Selecting the right/stop-right script for one shared
real bind call restores all twelve direct target multiplicities:22 direct calls
and one indirect update callback. Source and target each have one RET4. The final
full contribution is2836/2434 versus the complete2896 target, including its own
switch tables; the native code ends at offset2829 and candidate code at2771.
Both allocate18 hex stack bytes. Register allocation, branches and vector/store
scheduling remain non-exact; no shorter native extent is registered.
The corrected reset source emits2412/1970 in this same cold movement context;
older2444/1967 numbers concern the previous callback publication/context.

Keeping the event API's ID in the existing four-byte AnimationVmId value wrapper
preserves its stack word while the event remains in SI. Flattening this logical
ID to unsigned int lets VC9 promote it to EAX and emits53/57 in the same independent
renderer context. The wrapper reproduces all67 bytes diagnostically; its transport
matches native evidence without encoding a private register ABI. This does not
establish the original C++ type or TU identity. The independent renderer argument
also prevents global-only specialization of the actual FindAnimationVmById API.

The complete67-byte helper now passes the strict linked Oracle after a cold
primary-TU build. Its current PE/MAP/PDB/source/header/support/tool receipt and
unique complete primary contribution agree; exhaustive DIR32 at6 binds renderer
global4C3268, and REL32 at12 binds FindAnimationVmById4561E0. Every byte, including
the six internal alignment bytes, matches. The source encodes ordinary value
arguments, not the private SI register. No target code, padding or cropped extent
is introduced. This adds one exact function/67 bytes independently of the movement
body's remaining differences.

The callback-slot and return-declaration corrections cold-replay23 actually affected
old units:1,925 bytes and82 fields/relocations pass. Dependency selection follows
current receipt inputs. The complete previous six motion/HUD proofs retain their
full extents and all13 literal uses/92 bytes after this correction. No full cohort
or public Oracle/infrastructure suite is replayed.
Together with the new helper proof,24 selected units pass1,992 bytes/84 fields
and relocations. `.analysis/secondary-movement-final-proof.json` records all results.
