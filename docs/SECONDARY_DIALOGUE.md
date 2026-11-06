# Complete secondary dialogue interpreter

RunSecondaryDialogue at41D380 owns6980 bytes, with maintained source in
SecondaryDialogue.cpp and its portrait/clear fragments. The original class,
script-file format ownership and TU partition remain unproved. Source presence
and compiler observations grant no exact credit.

## Complete boundary and physical entry

The initial6794-byte bounding extent omitted its tables. Direct mapped-text-
attested Ghidra review and independent complete PE decoding establish:

| Component | Bytes |
| --- | ---: |
| Reachable code:1834 instructions | 6780 |
| Internal alignment | 14 |
| Alignment before tables | 2 |
| Primary27-cell dispatch | 108 |
| Stage7, stage7 and difficulty5-cell tables | 76 |
| Complete owned extent41D380..41EEC3 | 6980 |

All46 apparent foreign DATA references are owned switch cells. No foreign
interior entry remains. The next function begins41EED0 after12 CC bytes. All252
direct call sites/28 callees and both RET4 paths are reviewed. The physical
argument is the stack dialogue pointer; frame140 and EAX0/-1 returns are observed.
The single actual caller is the still source-open4686-byte HUD owner41B380,
calling at41BF18. Its complete boundary/source is a separate task.

The partial view retains mode0, timer18, wait timer2C, animation IDs40..5C,
instruction64, three position vectors68..8B, delay8C, flags90, line94,
initialization98, speaker9C and colorsA0. These are observed spans, not proof of
an original allocation size or original C++ array types. Instruction headers are
unsigned16 frame, byte opcode and byte payload length; advancement is4+length.

## All27 instructions

| Opcodes | Maintained operations |
| --- | --- |
| 0 | Conditional counter58 clear, selection50 clear, return-1 |
| 1/2/3 | Player/opponent/panel animation creation; character0/1, stage5/6 and mode2 branches retained |
| 4/5/6 | Ordered animation signals and the observed player/opponent ID clears |
| 7/8/9 | Flag helpers/events8, speaker0/1/2, four positions and four float3F8 zero stores, text-state resets |
| 10 | Replace flags bit0 from the signed payload byte |
| 11 | Lazy wait timer set, decrement, backup input tests, paused return0, sound0 and text-state reset |
| 12 | Set delay1 without inventing a stop/return |
| 13 | Player children selected by script IDs; three sprite changes per character |
| 14 | All seven stage branches, both characters, stage6/7 mode2 branches and stage7 argument>11 choices |
| 15/16 | Decode and draw the first/second text VM, then signal its existing flag helper |
| 17 | Four blank initializations, normal two-line progression or unchecked `|x,spacing,text` parsing with font2 |
| 18 | Clear the four text/annotation flags |
| 19 | Audio selection38 and HUD file43EC/script2/layer0x16 animation |
| 20 | Seven distinct boss creation paths from master file48 |
| 21 | Ordered clear setup, records, replay/menu gates and stage6/7 rewards |
| 22 | Fade duration8 for stage6 and2 otherwise |
| 23/24 | Event7 to a found portrait and, when draw.previous is null, its following draw nodes |
| 25 | Four int-to-float3F8 stores, rereading the instruction after each VM lookup |
| 26 | Set flags bit1 |

The script skip input uses backup118/124 inside the existing InputState root,
rather than the ordinary current/pressed fields. Waiting returns before advancing
the instruction and dialogue timer. Normal completion advances the real stage
timer; unknown opcodes still advance by the encoded payload size.

Sprite changes use the actual389-byte BindAnimationSprite44AB40 implementation.
The child is found by its signed script number; the following operation
binds its sprite. Existing animation script-binding APIs have different target
addresses and are not interchangeable. Natural AnimationVmId value results and
const-reference consumers retain the actual temporary lifetimes without fake
constructors, registers or frame storage.

The clear instruction writes record5A9 then5A8 with fresh index reads. It retains
wrapped32-bit bonuses, the captured statistics pointer for stage6, the later
reload for stage7, HUD4450/4454 stores, signed99999 saturation and one common
continue-menu operation. Stage7 keeps its three separate score calls. No
allocation/null guard, annotation repair or menu policy is added.

## Real dependencies and full compiler comparison

SecondaryDialogueAnimation.cpp supplies the decoded-text41, child-ID70 and
sprite helpers34/36. Complete Ghidra/PE reviews cover every byte, return,
reference and neighbor. Decoder key77/step7 wraps while writing shared4C9A30
until decoded zero; its capacity remains unknown. Missing-parent continuation
and required-VM accesses retain the target's unchecked dereferences. These
helpers have no independent leaf acceptance units. Their complete support
replays differ2/0/0/0 bytes respectively; these observations do not grant
primary-owner acceptance.

The cold diagnostic family includes the full existing gameplay/debug/host/actor
cycle, real timer/motion, animation ID/finding/sprite/event, score and formatted
text implementations. Real formatted-text dependencies preserve its generic156-
byte contribution; no synthetic caller is added to constrain its arguments.
Only the existing real gameplay callback and the diagnostic entry use normal
COFF. The entry supplies dynamic owner pointers for compilation and is never a
runtime harness or evidence of the original TU partition.

Current public-source contribution6948 includes all four27/7/7/5-cell tables,
6734 reachable code bytes and30 alignment bytes. Frame13C differs from native140.
All252 physical call sites, per-callee counts and46 acyclic per-opcode
call/advance/return skeletons agree, including return0 versus return-1. The check
omits loop reentry and does not prove predicates, operand identities or aliases.
All454 independently reviewed address/table fields replay over the complete PDB
owner;5694 bytes still differ. This is non-exact, despite the nearby size.

The first complete normal-C++ candidate left separate marker lookups and
continue-menu calls. Sharing their actual common operations recovers the native
physical call counts. A positive guard/do-while variant gives6940/6483 and changes
the native early-return layout; it is not adopted. Reordering the complete
handlers by reviewed native entry order gives6948/5698; spelling the comparison
with current time on the left recovers both native comparison forms and gives
the maintained6948/5694 result. No inert padding, extra null
tests, volatile fields, fabricated returns or private ABI declarations are used.

Private evidence: secondary-dialogue-{boundary-proof,helper-boundary-proof,
functions,owners-function-proof,owners-instructions.diff}, the full direct Ghidra
exports, support-proof/checkpoint-proof/source-freeze,
verify-secondary-dialogue-target.py and audit-secondary-dialogue.py. The160-input
union changes four of the prior145 inputs and adds15. Only the old adapter exact
unit is affected; its cold11-byte/one-field replay still has zero differences.
The source-open HUD caller and full byte/codegen differences remain substantial
next work; there is no native-product/runtime/port closure claim.
