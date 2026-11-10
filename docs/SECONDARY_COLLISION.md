# Complete secondary collision transition

BeginSecondaryCollision432A90 now has complete maintained natural C++ source for
its546-byte target owner. The current VC9 linked diagnostic is543 bytes with257
masked differences. This adds one source mapping; it grants no exact byte credit,
runtime result or original class/TU attribution.

## Complete target and observed interface

Fresh direct local Ghidra full mapped-text attestation/review confirms the
contiguous432A90..432CB1 extent, no foreign interior entries and four real callers.
Independent decoding covers all546 bytes/133 instructions, all internal branches,
both plain returns and adjacent bytes. There is no owned table or omitted tail.
The earlier boundary/origin review remains valid and indeterminate.

Ghidra's no-argument signature is incomplete: the owner enters in EAX and is
retained in EDI. The actual431F70 collision caller prepares EAX from ECX at432052;
the432520 caller prepares it from ESI at43273D. The maintained interface retains
the one real owner pointer through ordinary C++/LTCG, without a private-register
ABI declaration. Original calling-convention/class identity remains unresolved.

## Maintained source contract

- Set mode928 to4. Form one float3 from the actual player position: separate
  double x+32+192, double y+16 and the actual z, each stored to float.
- Create the primary VM using the file at global4A8D68+46D674, script4C/layer0,
  then publish its raw position with the exact456330 helper.
- Repeat32 times: reload that file; conditionally enter renderer critical section9
  under runtime flag8000 and increment its byte depth; allocate the scene VM;
  set flag40000000/layer0, initialize script4D and publish to the actual scene
  list; conditionally leave/decrement; publish the same raw float3 position.
- Initialize the timer944 lazily, then set current0/fraction0/previous-1. Preserve
  the nullable global4A8E88 gate: sound4 is queued only when it exists and flag200
  at its60 field is clear. The local physical view leaves its original owner open.
- Initialize timer8BB0 lazily, then set current6/fraction6.0/previous5. Bind script0
  from the player's own file10 into its embedded VM14.
- Preserve the active-spell branch: age>=60 clears value8E4 and flags22; otherwise
  active enemy and wrapped selection3*A+B!=5 sets flag20. No score, lives or
  player-value penalty from the separate death transition is added.

The VM allocator results and embedded nodes retain native unchecked use. The
source introduces no blanket ID clearing, allocation fallback or new timer guard.
The existing physical controller/timer/animation views are reused. Only the sound
gate's used flag offset60 gets a new view/assertion; its total owner size is open.

The imported calls at432B1D/432B62 are independently resolved from the target PE's
IAT to EnterCriticalSection/LeaveCriticalSection. The6.0 status constant at498304
is checked directly. The decompiler's apparent floating return from sound is an
x87 hypothesis: the native sound helper is integer-only, and the zero remains live
across that ordinary void call. It is not represented as a fabricated return.

## Compiler observations and open codegen

The first private full-source draft retained one named ID across both creation
sites and emitted554/421. Directly passing the normal returned ID values at their
respective use sites preserves the primary helper's actual returned-buffer load
and gives maintained543/257. This uses the existing AnimationVmId constructor;
no new constructor or ABI trick is added.

Candidate and target agree on all eight direct call sites/seven targets, both
imported section operations and both plain returns. Float store boundaries,
32-iteration countdown, timer initialization and spell branches are retained.
This is complete-source/call-contract evidence, not a behavioral proof.

The remaining concrete gaps include frame18 versus native1C, EBX saved at the loop
rather than the entry and restored earlier, EDX versus native ESI for sound4, and
integer register/global-store scheduling. A file-singleton pointer snapshot gives
555/402 in a private probe and is not adopted. No artificial local storage,
padding, volatile access or forced register allocation is introduced.

The cold diagnostic uses entry SecondaryCollisionTransitionProbe@24 and profile
/TP /O2 /Ob1 /Gy /MT /Isrc /EHsc /GL. Its real independent API inputs preserve
renderer/ID, file/script/VM and sound argument contracts. Support sources are
probes/secondary-collision-transition-context.cpp, AnimationVmFactory.cpp,
AnimationRendererDispatch.cpp, AnimationVm.cpp, SecondaryActorAnimation.cpp,
StageMesh.cpp and MeshEffects.cpp under src/ for the implementation files.
The complete primary contribution is independently identified through PE/MAP/PDB;
no canonical unit is registered for its nonmatching bytes.

## Checkpoint scope

Only the new collision source and probe are implementation additions. Recursive
current linked/COFF input contracts select zero affected old exact units, so the
previous32 position-marker cold proofs remain current and are not repeated.
Source/probe hashes stay frozen after the maintained cold diagnostic and call
audit. Tracking/progress and diff whitespace checks pass. No full cohort, broad
runtime suite or public Oracle/infrastructure tests are run for unchanged tools.
