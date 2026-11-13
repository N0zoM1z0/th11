# Pause menu and replay editing

## Complete target owner

Mapped-text-attested direct Ghidra code, references, explicit span, caller and
callee review plus independent original-PE decoding establishes the full
`42C8D0..42D55B` owner:3212 bytes,859 reachable instructions/3147 code bytes,
nine layout bytes and56 table bytes. The initial3154-byte default omitted the
final MOV EDI,EDI and both tables. Seven skipped bytes precede42CF80; the
reachable NOP42CF3F is code, not skipped layout.

The primary ten-cell table42D524..42D54B dispatches states1..10;5 and7 share an
arm. The four-cell table42D54C..42D55B dispatches the main selector's choices.
All14 apparent foreign interior references are owned DATA. Eight CC separate
the previous42C880 owner, and four follow the tables before42D560. All101
physical calls to20 targets,164 address fields,14 GS epilogues/RET4s and the
68-byte local frame are inventoried. The sole native caller42BF30 pushes one
menu pointer; Ghidra's extra `this` argument is not read by this body.

`PauseMenuView` records observed offsets without claiming the original class:
state04, timer10, selector38, keyboard110, root/backdrop1E8/1EC, name cursor1F0,
save mode1F4, opening kind1FC, saved audio200,25 replay pointers204, nine-byte
name2CC and saved scale2D8. The shared selector remains the existing D8-byte
view, including its exclusion/history storage.

## State and input order

States1/2 wait10 frames, set count4/wrap1 and select0/1. State1 excludes entry2
only when session74 is nonzero; state2 appends exclusions2 then0. Existing
exclusions are not silently cleared. The selected short event is7+current;
opening kind0/1 is published after signaling.

State3 copies previous selection before up/down pressed/repeat10/20. A changed
choice signals7+current before sound12. Confirm80001 queues10, then handles
resume0, retry1, replay2 or exit3, and resets the timer. The seven real child
searches use scripts4E/4F/50/57/58 and the shared event6 operation. Retry/exit
state is5 minus the nonzero session74 flag; exit first writes state5 as observed.

Shortcut200000 is conditional on opening kind0, selects3, signals child50 and
backdrop, and starts closing4. Shortcut10000 independently selects1 and child4E.
They remain separate tests after confirm. Escape100 is then honored only for
opening kind0 and selects resume0. Confirmation and shortcut/cancel masks are
not converted into mutually exclusive tests.

State4 waits12 frames, writes state0, then restores gameplay for choice0,
requests runtime mode4 for1, or publishes10+(session74!=0) for3. Choice2 has no
invented handling. States5/7 push history only on frame20, choose1 of2 and
propagate root event14. From frame30 they signal15+current, navigate, and use
57/58 confirmation children. Choice0 enters8 only from7, otherwise6; choice1
enters6. Escape still takes the observed resume-closing path. State6 waits20,
then restores history and either closes4 or returns to3, with the native event
and timer ordering retained.

## Replay slots, metadata and eight-character name

State8 waits20, enters9, resets the timer, hides the root tree, pushes history,
chooses0 of25 and reads all25 `th11_%.2d.rpy` slots. State9 waits10 and navigates
slots. Confirm takes priority over cancel: enter10/reset, clamp keyboard0 using
its old count before setting count91/wrap1, capture the active replay's header
pointer, call `_time64` at header0C and write stage at header68. That field is
distinct from the existing menu replay difficulty64. Stage is8 only for nonzero
save mode with resource flag10 clear; otherwise the current stage is loaded
after the time call.

The remembered name is copied from statistics2DDE0. A nonblank name moves the
keyboard by-1; trailing spaces determine the cursor0..8. Cancel102 sets14 and
resets the timer, pops history, sets4/wrap1, signals7+current, deletes/nulls all25
replays, writes4, chooses1, resets the timer again and queues11. The transient14
and both resets are retained.

State10 waits10 and edits a13-column keyboard using vertical moves-13/+13 and
horizontal row wrap+12/-1 or-12/+1. Keys0..87 read the native character string;
88 inserts a space,89 deletes,90 saves. At cursor8 an insertion replaces the
last character; reaching8 selects key90 via the existing fastcall operation.
Delete with cursor0 returns immediately, including when cancel is also pressed.
Cancel otherwise queues11 and deletes one character, or returns to9 at cursor0.
The observed unchecked indexing is not replaced with new validation.

Save queues44, formats the selected slot, deletes its previous replay, calls the
source-open436420 writer with filename/name/mode1, reloads the slot, returns to9,
resets the timer and copies the name back to statistics. The writer's result is
not consumed; its full source and exact codegen remain separate work.

## Required supporting operations

These bodies support the large owner; only the event6 child signal below has
its own independent acceptance unit:

- 42C880/72 clears gameplay flag10, queues sound command7 except stage6 before
frame300, then restores saved scale2D8/audio200. One call/seven fields, eight CC.
- 42F380/70 `SignalPauseMenuChild` is now a strict exact unit:67 reachable
bytes plus three loop-alignment bytes,22 instructions and two reviewed fields.
It sends event6 to root and related VMs when marker18 is0.
- 435FC0/18 guards null, destroys replay contents435C50, then invokes operator
  delete45FD49. It is not a direct free. Two calls/fields, fourteen CC.

At the original pause checkpoint,42D9D0/42E6B0 still lacked source. Both
neighbors have since gained maintained source: the continuation owner is3192
bytes and non-exact; the game-over owner is3208 bytes with a reviewed boundary
and remains non-exact. See [PAUSE_CONTINUE_MENU](PAUSE_CONTINUE_MENU.md) and
[PAUSE_GAME_OVER_MENU](PAUSE_GAME_OVER_MENU.md). The call graph remains
incomplete. Original TU, nominal classes, private ABI, native-product closure
and runtime behavior remain unproved.

## Exact event6 helper (2026-11-13)

`SignalPauseMenuChild` at `0x42F380..0x42F3C5` now replays **70/70 bytes**
exactly. The original owner has 67 reachable code bytes, three loop-alignment
bytes,22 decoded instructions, a plain RET, and two reviewed address fields:
DIR32 `+0x04` to `g_RenderBatch` (`0x4C3268`) and REL32 `+0x0A` to the exact
`FindAnimationVmById` (`0x4561E0`).

The key source expression materializes `AnimationVmId localId(id.value)` before
calling the finder. Passing the reference directly made VC9 keep the incoming
pointer in EAX and load the value into ECX, then use EDX for event6. The local
value form naturally recovers the native EAX ID, EDX renderer and ECX event
register schedule. No inline assembly or byte patch is used.

The canonical `signal-pause-menu-child` unit supplies two independent dynamic
ID arguments and the real `StageMesh.cpp`, `SecondaryActorAnimation.cpp`, and
`stage-mesh-context.cpp` producers. Its unique primary-TU PDB contribution is
70 bytes; both fields replay and the full byte difference count is zero. The
three real menu callers remain covered by the reviewed boundary. Original
class/TU attribution and runtime behavior are still unclaimed.

## Pause-checkpoint compiler evidence

The public59-support recipe retains VC9 RTM21022, /O2 /Ob1 /MT /EHsc /GL and
both established normal menu supports. Its independent compile-only entry takes
a real menu pointer. Source-open calls/data use diagnostic address anchors; the
result is not an executable game or runtime test.

The complete public primary-TU contribution is3084 bytes/160 address fields/
3039 full differences. All101 calls,14 RET4s/GS checks and56 table bytes agree.
Local frame68 and cookie64 now agree with the target; the prologue saves
EBX before the guard, while native delays it until after the guard. Four native address-field occurrences are absent: one session pointer4A8E88,
one active-replay pointer4A8EB8, one character-string base4951E8 and one runtime
base immediate4C3280. Caching, branch merging and callee argument specialization
remain distinct codegen questions. Register allocation and layout are
still open. Equal call counts alone do not establish exactness.

The stage68 metadata view remains distinct from difficulty64. Natural inline
clamps retain the one actual40DF10 call and restore the native physical call
counts; using the separate clamp everywhere adds ten calls. No false padding,
register ABI or return type is introduced.

Supporting resume72 and replay deletion18 replay to zero; event6 tree70 has
seven differing register bytes. These remain support observations, without
independent acceptance units. The shared child finder remains76/native70 with
41 differences and script stack/RET4 versus native EBX/plain RET. Two real
large callers still lack source.

The `signal-pause-menu-child` leaf is now canonical exact and has its own
selected cold-replay receipt. Use the public pause recipe and the current work
in [RE_HANDOFF](RE_HANDOFF.md) when resuming.

The return-temporary control is concrete: an inline `SignalChild` wrapper reused
one hidden result slot for all seven calls, producing frame4C/3041 differences.
Writing each actual `SignalPauseMenuChild(FindSecondaryDialogueChild(...))`
expression gives seven distinct slots and frame68/cookie64, with3084 bytes/160
fields/3039 differences. Accounting for the source's real four-byte script push,
all seven result addresses coincide with the native stack locations. This is
ABI/dataflow evidence; complete byte comparison still includes every prologue,
argument and layout difference. No frame padding or target bytes are introduced.

The retained public-path cold build reproduces the direct-expression result.
All15 previously inspected menu/support owners remain byte-identical to the
difficulty checkpoint after exhaustive field replay. The new large caller does
not resolve the shared child finder's script-register gap on its own.

## Keyboard signed remainder (2026-11-11)

The native 3212-byte pause owner uses four IMUL and zero IDIV instructions,
including the 13-column keyboard. An equivalent signed expression,
`current - (current / 13) * 13`, restores those exact instruction categories
where direct `current % 13` compiled to two IDIV. The local normal COFF changes
from 3256 to 3284 bytes, and a narrow sound/menu /GL probe from 3244 to
3276 bytes: this increases the length gap against native 3212 even though
the arithmetic instructions now agree. The earlier 3084-byte large-support
link predates this source revision; no exactness is claimed. Targeted evidence:
`.analysis/PauseMenu-quotient-compare.json` and
`.analysis/pause-keyboard-focused.log`.
