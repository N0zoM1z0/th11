# Complete partner-selection menu

`src/MenuPartnerSelection.cpp` reconstructs page07, the three-partner selection
state machine preceding practice-stage selection or game entry. The required
visibility operation is in `src/MenuPartnerSelectionSupport.cpp`; the existing
shared child finder in `src/SecondaryDialogueAnimation.cpp` is reused. Original
class/TU identity, native product and runtime behavior remain separate unknowns.

## Complete ownership

Direct mapped-text-attested Ghidra code/reference/neighbor/span/table review and
an independent original-PE CFG establish the complete owners:

| Function | Complete extent | Bytes | Reachable instructions/code | Owned table |
| --- | --- | ---: | --- | --- |
| Partner-selection page07 |43D0B0..43D69B|1516|392 /1496|Five-cell20-byte table43D688..43D69B|
| Hide animation tree |456650..45668F|64|20 /64|None|

The old1496-byte page extent stopped at the last RET and omitted every table
cell. Its five apparent foreign interior entries are owned table DATA. The
complete page has38 physical calls to16 distinct targets,93 address fields and
six RET4/EAX1 paths. It takes one stack menu pointer, unlike page08's native EAX
input. The two-byte MOV EDI,EDI at43D4EE is reachable loop alignment, included
in the full1496-byte code inventory. Four CC follow the table before page08.
The visibility operation includes its six-byte loop-alignment LEA, has one
lookup call/two fields and ends directly before456690. Neither support body
has an external interior entry; the70-byte child finder was already reviewed.

## Opening and Extra exclusions

Opening sets selection count3. Only difficulty4 applies the Extra exclusions.
It checks the10 bit of statistics `2DDF0 +character*3 +partner`. A locked current
partner is changed to an available alternative through the observed count
clamp, then appended to the existing exclusion list at selection90 using
excludedCountD4. Exclusions run in order0,1,2. There is no new count reset,
all-locked fallback or capacity guard: the caller/history protocol is retained.
The third native flag address is split into1E94C+F4A6=2DDF2; this is one logical
observed offset, not evidence for an original nominal record class.

The page creates script62, stops/recreates8E+character, executes event3 and sends
short event11+current to that character's root. It enters state1, then hides
three record indicators when the corresponding counters are zero. Counters are
DWORD arrays at offsets598/6E6C/D740, indexed by `character*4E9F +difficulty`.
Their physical spacing is68D4; no score/clear-count semantic name is inferred.
Children are88/89/8A+character*3. State1 activates when time is greater than6.

The shared child finder clears a missing parent ID, then retains its observed
continuation from VM drawNode10, matching a signed-short script and returning
an AnimationVmId value. The visibility operation independently looks up that
ID, clears flag2 and propagates the flag clear through related nodes when
marker18 is zero. A missing VM does not clear the supplied const ID reference.

## Active input and transition paths

The active state snapshots current to previous, processes up/down pressed or
repeat input, and on change queues sound12, executes event3 and sends event7+
current. Cancel102 takes priority over confirm80001, enters state4 and queues11.
Confirmation enters state3, queues10, looks up child
`7C +character*6 +current`, then sends short event6 to that VM and its related
nodes when applicable. All late event propagation and return paths are retained.

At state3 frame10, non-practice starts the480.0f/392.0f overlay and the existing
five-argument screen-effect factory with `(5,32,0,0,61)`. Practice flag10 instead
saves partner to both globals, pushes selection history, stops62 and selects
page8. The independent frame40 path remains, including the practice fallback
when frame10 was skipped. It saves/pushes before checking the practice flag.
Normal entry selects page2, then stage1/table+40 for difficulty below4 or
stage7/table+1C0 otherwise. Runtime558 is written7 **before** queuing float6.0f
fade; page08 writes its runtime field after the fade. Preserve this ordering.
As on page08, the fade call's pushed integer slot is overwritten by FSTP: its
argument is a float, not the decompiler's apparent integer7/extra register.

Closing at frame6 saves both partner globals, stops8E+character and62, selects
page6 and pops selection history. No behavioral harness or synthesized caller
is introduced for these paths.

## Cold compiler context

`probes/menu-partner-selection-context.json` retains VC9 RTM21022 and
/O2 /Ob1 /MT /EHsc /GL. It uses the real controller callback and52 support
sources, including the existing child finder and both full practice/player-data
callers; the same two established normal supports remain unchanged. Missing
callee/data anchors keep the link diagnostic rather than runnable. Complete
public PDB contributions and all address fields are compared without cropping
or branch normalization. Support observations do not grant leaf exact credit.

Current complete52-support diagnostic:

| Function | Native bytes | Source bytes | Full differences | Candidate fields |
| --- | ---: | ---: | ---: | ---: |
| Partner-selection page07 |1516|1480|1440|85|
| Hide animation tree, support only |64|64|0|2|
| Existing child finder, support only |70|76|41|2|
| Practice-stage page08 |1024|1056|981|78|
| Player-data page0A |2448|2488|2319|169|
| Record refresher |750|715|676|17|
| Music-room page0D |2668|2680|2417|94|

All physical call counts agree. The page has397 instructions/1459 reachable
code bytes, one alignment byte and all20 table bytes. Its stack input and six
RET4 paths agree with native, while frame8 differs from native0. Eight missing
fields are six character and two difficulty reloads replaced by caching. The
shared child finder takes script on the stack and returns with RET4, while
native uses EBX/plain RET. Parent ESI and hidden result EDI already agree.
The additional script push/cache lifetimes, initial constant-register choice,
record block scheduling and frame remain concrete full-owner gaps. No volatile
fields, invented temporaries or private ABI declaration is used to repair them.

Flat byte indexing at2DDF2 naturally reproduces the native odd1E94C+F4A6 address
split. This compiler observation does not establish a nested original record.
The initial51-support context gives the same1480/1440/85 page statistics, but
its first input test precedes the previous-selection store. Adding the actual
6980-byte dialogue interpreter restores the native copy-previous-before-input
order:16 complete replayed bytes change while the full mismatch total remains
1440. It also restores two pressed-input fields in page08, which changes from
1044/970/76 to1056/981/78; one saved-selection field remains cached. These are
concrete context improvements, not a new exact claim or a lower size metric.
Music changes2680/2416 to2680/2417 through one SIB byte in this context.

A private66-support union with the real gameplay/debug cycle and its established
normal callback gives all twelve inspected contributions byte-identical to the
52-support context after exhaustive field replay. The larger union adds no
further evidence for these gaps and is not retained. The old dialogue family's
70-byte/zero child-finder diagnostic remains context-specific: both new contexts
keep76/41 and the script stack argument. An independently typed physical
selection-storage variant is byte-identical to the initial51-support candidate
for all twelve inspected owners and is discarded. No ABI/stack workaround is
introduced to fit the older support result.

No accepted unit depends on the added source. Rebuild the public
partner-selection recipe for a new measurement.
