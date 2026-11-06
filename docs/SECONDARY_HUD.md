# Complete secondary HUD update

UpdateSecondaryHud at41B380 now has complete natural C++ source. Its full owner
is4716 bytes, including the seven-cell stage table. Source presence, reviewed
boundaries and compiler observations are separate from exact acceptance; this
function remains non-exact. Original class identity and TU partition are open.

## Complete boundary and entry

Direct mapped-text-attested Ghidra review, full disassembly and independent PE
control-flow/reference/neighbor checks establish:

| Component | Bytes |
| --- | ---: |
| Reachable code:1302 instructions | 4647 |
| Internal alignment | 39 |
| Pre-table alignment | 2 |
| Stage1..7 table at41C5D0 | 28 |
| Complete owner41B380..41C5EB | 4716 |

The automatic4686-byte bounding span ended at41C5CD. Its seven apparent foreign
DATA references are owned table cells; no foreign interior entry remains. Four
CC bytes separate the table from41C5F0. All27 direct/imported calls to17 targets
are reviewed. Native stack alignment is8, local frameC, stack self, EAX1 and two
RET4 paths. The decompiler's extra integer/float parameters and eight-byte return
are hypotheses contradicted by the actual entry/caller/returns.

The missed41CFB0/7 callback passes ECX as the stack HUD pointer and forwards EAX.
Its registration reference is41A442, adjacent CC gaps are6/9, and its complete
span has no foreign interior entry. The maintained ordinary fastcall callback
is needed for this real owner context, without an independent leaf acceptance
unit or proof of original TU separation.

## Complete source operations

The physical HUD view retains nine primary VMs at10, two digits at25E4, four
gauges at2E4C and the boss indicator at3F1C. Title4380, ten badge IDs4384,
tracked43BC/active43C0, timer43C8, files43EC/444C, boss values43F0/43F4/43F8,
badge count43FC, four bars4400, flags4420, transition timer4424, dialogue4438,
seconds4440/previous4448 and clear fields4450/4454 are observed offsets. These
are physical views, not a recovered allocation size or original array types.

The complete function maintains:

- Transition timer and clear-frame180 screen effect, then frame380 menu/runtime
  selection. Both allocation guard and initializer kind43 are observed. The
  0x44-byte allocation has a real StageTimer at30; member construction precedes
  whole-object clearing and flags2. Initializer448A90 remains source-open; its automatic1129-byte code bound
  is not a complete reviewed owner.
- Tracked-ID disappearance and all three update loops of9/2/4 VMs; player
  position gates switch gauge events3/2, with the original null checks.
- Wrapped signed gauge percentage, all four event bands7..10 after timer20,
  digit geometry/flags, original float stores and double literal widths.
- Boss timer events9/8/7, sounds24/1B and two sprite updates through44AB40.
  This binds sprites rather than replacing animation scripts.
- Boss ratio/smoothing, player-coverage events, all seven stage/title choices,
  ten badge slots, renderer locking/depth changes and ID publication. Both
  title and badge allocation use portraitFile444C. The title calls4556E0;
  badge publication is inline, including intrusive links and zero-ID skipping.
- Real RunSecondaryDialogue call, elapsed timer advancement on result0, otherwise
  a fresh dialogue-pointer read and eight ordered releases40/44/48/4C/50/54/58/5C,
  related-VM flag propagation04000000, deletion and pointer clearing.
- Boss indicator state thresholds700/400/200 versus2000/1000/400 while a spell
  is active, event7..10 transitions, one VM update and position/alpha calculation.
  The final HUD timer advances on every path and returns1.

This caller proves the previously unknown dialogue member4 is a StageTimer.
The interpreter's timers18/2C are separate. Both inline advances retain the
scale-window fast path and wrapped current increment. Renderer searches retain
the two ordered lists and unchecked node payloads; no new allocation/player/
required-VM guards are introduced.

The alpha conversion is naturally a signed-char conversion, producing the
native inline x87 control-word/FISTP sequence without a third ftol call. The
native threshold compares distance>=64 for the FF path, with unordered values
taking the conversion path. A superficially equivalent ordered distance<64
source condition has a different unordered edge and is not retained. Integer
and float phantom return values from the decompiler are not modeled as APIs.

## Compiler comparison

The diagnostic /GL primary family contains the real HUD/dialogue/gameplay/debug/
host/actor cycle and existing animation, timer, sprite, sound and text support.
Only the real HUD/gameplay callbacks and diagnostic entry use ordinary COFF.
The entry calls the HUD callback; it does not directly call the interpreter to
hold its ABI. This context is a compiler experiment, not a linked game or runtime
harness. Missing automatic11640-byte UpdateSecondaryControllerAnimation44B4B0 and screen
initializer448A90 remain external source gaps.

Initial class-outside constructor spelling leaves an extra constructor call.
A normal class-inline definition exposes its observed memset operation and
recovers all27 per-callee call counts. The first full inline candidate is4668
bytes, frameC/two RET4,1305 reachable instructions,4565 code+75 alignment+28
owned table bytes. Its104 reviewed address/table fields replay completely;
4069 bytes still differ; its ordered alpha guard has a wrong unordered edge.

The maintained candidate is4652/frameC:1299 reachable instructions,4549 code+75
alignment+28 table bytes,104 reviewed address fields and4101 full differences.
All27 per-callee physical call counts agree. Static CFG comparison retains loop
reentry and gives equal abstract call/return languages across27 product states;
constant propagation gives1 at both RET4 points. Branches are nondeterministic
in this check: predicates, real loop bounds, aliases, arithmetic and runtime
behavior are not established.

An explicit life-value snapshot restores the native register-to-stack FILD
input before the HUD write. Reordering the actual gauge sum also restores its
x87 operand evaluation order. The complete49-instruction position/alpha region
agrees after erasing branch addresses and the six references to its distance
float slot (nativeESP10/sourceESP14). Registers, x87 operations, literal widths,
comparison masks and control-word/result slots are retained by that comparison.
This local diagnostic grants no cropped or whole-function exact credit.

A further state-machine spelling moves state extraction inside each spell
branch and reads the counter directly in each threshold condition. It recovers
native memory CMPs but expands the whole owner to4812/4264, with added HUD-pointer
reloads through unrelated cleanup paths; it is rejected. Remaining full-owner
gaps include early selfEBX/nativeEDI, the source's long-livedEDI=-2 mask, memory/
register flag comparisons, list/store scheduling and distance slot14/10. Full
byte equality remains the criterion; size and matching call graphs grant no
exact credit.

Private evidence uses secondary-hud-update boundary/review/disassembly exports,
secondary-hud-owners full-function/field/diff reports, secondary-hud-dialogue
and parents comparisons, and the current source/dependency freeze. Experiments
and target/decompiler dumps remain private.
