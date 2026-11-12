# Secondary player controller

Later motion/death work closes orbit255, aim495, both12-byte callbacks, angle
difference101 and life HUD144. Death703 now has complete non-exact source;
see [SECONDARY_EMITTER_MOTION](SECONDARY_EMITTER_MOTION.md).

The complete controller431070/2888 now has maintained source in
`src/SecondaryPlayerController.cpp`. Its reviewed owner includes2867 code bytes,
one NOP and five switch entries through431BB7. Current complete source produces
2800 bytes with2285 normalized diagnostic differences. It remains non-exact;
its table and late projectile paths are not cropped.

## Full controller and real inputs

The controller takes one actual owner on the stack and returns1 with RET4.
Ghidra's additional ECX/EDX parameters arise from private callees, not two extra
logical controller inputs. Source preserves all five modes and their shared tail:

- State0 computes the wrapped fixed-point entrance position, refreshes all eight
  emitters, clears counters and performs the two-radius/full-clear paths. At60
  frames it resets state time and enters state1 in the same frame.
- State1 preserves HUD/enemy/power/input gates, activates the real special,
  consumes one power unit, resets emitters, clears during the first30 frames and
  calls actual movement. State4 has its own special gate without an added HUD
  guard. At age>=8 it performs death processing and enters state2 immediately.
- State2 snapshots old power before subtracting maximum power. At frame3 it
  constructs the actual aim vector `(0, position.y-224, 0)`, selects seven drop
  kinds from old power/lives, spawns all seven through the real manager and
  resets emitters. After30 frames it enters the appropriate menu or preserves
  the damage-region/position/timer respawn path. State3 retains its exact clear
  timing and argument choices.
- All32 region records retain motion composition, radius/angle growth, timer
  decrement and active-bit expiration. Region+C is consumed as float angular
  growth here; the shared physical header's unnamed word is not reinterpreted
  as integer addition. Status flash uses the actual three-frame test, then the
  embedded VM is updated and all eight bounding vectors are rebuilt.
- Both timers advance with the observed near-one scale tests and float stores.
  Host/HUD gates control value accumulation, pattern update and projectile update.
  The disabled path initializes/sets the pattern timer to previous/current -2/-1,
  clears target/lock and still updates projectiles. Late HUD dereferences remain
  unchecked where the target is unchecked.

The new physical controller view preserves offsets14/87C/8CC/908/944/7C9C/8BB0/
8BCC/8C14/8D20 through compile-time layout checks. It does not establish an
original C++ class or total allocation size. Old shared headers are unchanged.

Raw instructions establish position inputs missed by decompilation. Radius clear
40AF90 has EBX position plus three real stack words;424FE0 similarly has ESI
position plus radius/two flags. Spawn424230 uses position, manager, kind, color,
angle and speed. All these logical inputs are retained without dummy parameters
or register-specific declarations. Declaration/support coverage is separate from
complete source for those still-open callees.

The complete source/native call audit agrees on all22 direct targets and38 call
sites. Both actual return paths agree after the real caller context is restored.
This supports the reconstruction but does not prove byte equality or runtime
behavior.

## Callbacks and complete helper owners

Two omitted entries were independently separated from the32-byte431C50..431C6F
span by their returns and CC bookends:

| Address | Bytes | Contract |
| --- | ---: | --- |
| 431C50 | 7 | Fixed ECX owner, push owner, real controller call, RET |
| 431C60 | 10 | Fixed ECX owner, capture EAX draw receiver, save/restore ECX, RET |

Native initialization42F962/42F9B7 publishes them to actual chain callback+8,
context+20, flags and update/draw registration. The diagnostic probe uses those
actual callback/context fields and independent position input. It does not add
arguments to target functions or invent a registration API.

With all support compiled /GL, the controller has a private ESI input and two
plain returns at2784/2356; callback source contributes10/7 versus native7/10.
Ordinary COFF compilation of the real callback source makes the controller keep
its stack input and both RET4, giving2800/2287. Preserving native Y/Z operand order in the two unscaled upper
bounds gives current2800/2285. The primary controller stays /GL.
Draw callback remains7 versus10 and non-exact. No fixed register, noinline,
alignment directive or padding is introduced.

The newly recovered update callback has no established LTCG primary owner.
Its independent normal-COFF unit proves the full7-byte function and sole REL32
at+2 to431070. This does not establish original normal-COFF/LTCG TU identity or
close the controller. It does not borrow a support PDB contribution.

`src/SecondaryPlayerControllerHelpers.cpp` supplies three complete public primary
owners: refresh410610/54, aim431D20/101 and inverse aim431D90/106. The natural
refresh loop unrolls to all eight D4 stores. The angle functions preserve rounded
float deltas, ordered both-zero fallback pi/2, real x87 CIatan2 and rounded float
result. Actual controller input proves aim takes both owner and position.

Exhaustive independent field review:

| Owner | Offset | Binding |
| --- | ---: | --- |
| Refresh54 | none | Complete empty linkage set |
| Aim101 | 72 | Float pi/2 at497F90 |
| Aim101 | 85 | CIatan2 at486C9A |
| Inverse106 | 10 | Player global4A8EB4 |
| Inverse106 | 77 | Float pi/2 at497F90 |
| Inverse106 | 90 | CIatan2 at486C9A |

Both emitted constant payloads equal the target's complete four-byte float
pi/2 independently of address replay. Canonical linked acceptance also requires
current cold receipts, unique complete primary PDB ownership and every byte
matching; support ownership alone grants no linked exact credit.

## Expanded boundary and origin review

Thirteen new reviews cover5975 bytes/1585 instructions/23 returns. Eleven are
existing candidates and two are omitted callback entries. All origins remain
indeterminate. Code role, common scaffolding and reconstructed names do not
establish original game/library/class/header authorship.

| Address | Complete bytes | Independently reviewed role/extent |
| --- | ---: | --- |
| 406510 | 268 | Special activation:241 code+3 alignment+six dispatch entries |
| 40AF90 | 334 | Radius clear with four logical inputs and actual phase/filter gates |
| 40B5C0 | 99 | Filtered2000-record projectile clear |
| 410610 | 54 | Eight emitter refresh writes |
| 424230 | 851 | Drop allocation:829 code+3 alignment+two entries+11 selector bytes |
| 424FE0 | 95 | Radius dispatch across real object list/virtual slot1C |
| 425040 | 47 | Object-list clear/virtual slot14, including internal alignment |
| 42C760 | 275 | Continue/menu transition with timers/VMs/state preservation |
| 42D560 | 336 | Game-over transition including runtime-state shortcut |
| 430290 | 2896 | Movement:2829 code+3 alignment+two eight-entry tables |
| 4327D0 | 703 | Death transition including seven internal alignment bytes |
| 431C50 /431C60 | 7 /10 | Actual update/draw callbacks and publications |

Movement's sparse2811 bytes omitted18 internal alignment bytes. Its old2829-byte
inventory also omitted67 owned tail bytes. The allocator's intermediate840-byte
hypothesis still missed its11-byte selector array. All24 apparent foreign table
DATA refs in these three enlarged owners are their own entries, with targets at
independently decoded instruction starts. No other inventory starts are swallowed.
Six new source mappings cover3166 bytes; the other reviewed callees remain
source-open.

Only the four new canonical units are cold-replayed:261 linked bytes/five fields
and7 normal-COFF bytes/one relocation. No previous canonical receipt depends on
these new files, so no old family or full cohort is replayed. All four pass with frozen source and complete
receipt/owner/field/byte checks. After the final bound-operand correction, only
the three affected linked units are cold-replayed; the unchanged callback COFF
receipt is revalidated directly.

Reset experiments also remain open: scoped result temporaries yield2556/1977 and
six main factory sites versus native four; a signed counter view with explicit
wrapped increments leaves publishers111/33 and116/47. Neither is adopted. Further
work should use genuine movement/death/clear source and full vector/store/frame
differences. Never replace full owners with passing prefixes or force their ABI.

## Continue and game-over menu entry bosses (2026-11-12)

`SecondaryPauseEntry.cpp` now implements the full 275-byte `EnterSecondaryContinueMenu`
(42C760) and 336-byte `EnterSecondaryGameOverMenu` (42D560). Their boundaries
had already been independently reviewed. The first has one return and five
calls; the second preserves **both** the mode-1 runtime-only return and the
normal game-over menu return, with five calls. Neither owner is cropped.

Both transitions set the native pause state, initialize the two embedded
stage timers to frame -1 (including their conditional scale-pointer setup),
set scene flag 0x10, create the backdrop VM with script 0x4B and queue its
rectangle capture. The main animation file is read from the HUD at +0x444C.
The continue path also creates the script 0x5B root, clears its native
projectile flag, and queues the `Pause` command only when the stage is not 6
or the dialogue-session frame at +0x14 is at least 300. It saves and replaces
the animation time scale and music state, without inventing null checks.

The game-over path differs: dialogue mode 1 immediately switches the game
runtime to mode 2 or 4 depending on the runtime 0x2000 flag; otherwise it
sets pause state 13 or 21 from the stage resource flag, loads
`bgm/th10_17.wav`, queues sound command 4 conditionally and command 2
unconditionally, marks the native +0x2DE0B game-over state flag, zeros save
mode and changes the saved audio state to 1. The pause-session pointer at
0x4A8EB0 and game-over state root at 0x4A8EBC have narrow external source
names, not invented nominal classes or assumed allocations.

Scoped VC9 RTM `/GL` PE/MAP/PDB diagnostics with the genuine 105-byte exact
secondary draw factory, VM initializer and append producer compile complete
natural source contributions. Continue: **272 candidate / 275 native bytes,
80/82 instructions, 5/5 calls**. Game over: **336/336 bytes,
93/97 instructions, 5/5 calls**. Both are source-present only: register
scheduling, frame stores and the remaining call/field agreement have not
reached strict zero-difference acceptance. Reproduce with the bounded private
`.analysis/probe-secondary-pause-entry-20261112.py`; its public producer is
`probes/secondary-pause-entry-context.cpp`. No full cold replay was performed.

The game-over entry's observed music helper at 42A0D0 has since been
reconstructed as the complete **123-byte strict exact**
`QueueMusicWaveRequest`. `SecondaryPauseEntry.cpp` now calls that actual
helper. The entry function's own last scoped 336/336-byte diagnostic still
has nonfield register/stack differences and receives no exact credit; see
[SOUND_QUEUE](SOUND_QUEUE.md).
