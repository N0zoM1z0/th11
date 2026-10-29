# Secondary pair interpolation, vector leaves and CRT floor

This checkpoint reviews the complete4191F0/673 interpolation owner, its nearby
state/vector leaves and the real floor dependency of459A30/99. Evidence is the
original hash-pinned Japanese TH11 image, direct local read-only Ghidra queries,
independent PE decoding and pinned VC9 RTM21022 compilation. The diagnostic
images are never executed. Original class/header ownership remains unresolved.

## Complete interpolation source

The earlier source evaluated coefficient a as `(2*t+1)*(t-1)*(t-1)`. Independent
x87 stack reconstruction in all three native owners4191F0/673,405240/883 and
4055D0/737 instead establishes `(t-1)*(t-1)*(2*t+1)`: the square is formed first.
The source now preserves that multiplication tree. Coefficient b is spelled
`(3-2*t)*(t*t)` so VC9 also forms the square before multiplying the other factor.
Both retain double arithmetic and the observed float coefficient boundaries;
c/d and the rounded scale/add chain are unchanged.

Modes7/17 capture the previous initial vector before arithmetic, then publish
initial/output components in the observed order. Completion has distinct target
and initial returns. These natural changes recover the native EDI receiver,
frame28, six returns and full673-byte size. Current complete source673 has352
masked diagnostic differences, versus the former632/627. Equal size/registers
are not an exact claim. The output-buffer/returned-pointer contract is observed;
nominal vector-return ABI and original member ownership remain open.

Private alternatives tested vector arguments by value, an actual member/free
adapter, all real callers, timer setter/reference spelling, terminal snapshots
and component publication. No invented constructor, frame alignment, volatile
access, padding or ABI declaration was used to force a match. Terminal snapshots
alone worsen673/356 to673/363; component publication gives673/352. A member model
and broad by-value copy do not improve the complete comparison.

The full4230-byte state caller is rebuilt with actual pair and math definitions.
It emits4161/3261; independent review again confirms all64 direct-call target
multiplicities, three callbacks and five returns. This is source/flow evidence,
not an exact or runtime claim.

The shared StageTween correction leaves complete vector883 at source785/824
and fog737 at536/643. These remain open; an accurate arithmetic tree does not
imply improvement in every whole-function byte diagnostic.

## Independently reviewed small owners

All twelve full spans have complete PE decoding, actual adjacent bytes,
flow/return and incoming-reference review; direct Ghidra reports no foreign
interior entries or tables. Eleven were absent from the candidate inventory;
419520 already existed. No entry is created at4195D0 inside scaling4195C0.

| Owner | Address | Full bytes | Observed contract |
| --- | --- | ---: | --- |
| SetSecondaryPairMode | 4194A0 | 4 | Write mode38 |
| SetSecondaryPairInitial | 4194B0 | 11 | Copy two words to00/04 |
| SetSecondaryPairTarget | 4194C0 | 12 | Copy two words to08/0C |
| ResetSecondaryPairControl1 | 4194D0 | 19 | Global4C3278/7C to10/14 |
| ResetSecondaryPairControl2 | 4194F0 | 19 | Same global to18/1C |
| SetSecondaryPairDuration | 419510 | 4 | Write duration34 |
| ResetSecondaryPairTimer | 419520 | 52 | Conditional initialization; current/fraction0, previous-1 |
| GetSecondaryPairDuration | 419560 | 4 | Read duration34 |
| StopSecondaryPairTween | 419570 | 8 | Clear duration34 |
| SubtractSecondaryPair | 4195B0 | 16 | Two rounded differences to output |
| ScaleSecondaryPair | 4195C0 | 23 | Stack float, two rounded products, RET4 |
| AddSecondaryPair | 4195E0 | 16 | Two rounded sums to output |

The preset copies use separate component assignments to preserve the observed
load/store order. The addition preserves two component stores. These are ordinary source
operations, while private input-register/operand scheduling remains open. The duration setter has one register-encoding difference; the two presets
remain19/8 each and addition16/4. These stay non-exact; parameter order or member ABI is not invented to hide it.

The complete101-byte4195F0 owner is also independently reviewed: two stack floats,
double pi/tau comparisons, three rounded RET8 paths and the actual418D20 caller.
It stays source-open and origin-indeterminate. The adjacent419660/31 is the
already-reviewed SecondaryUnaryMathB cosine wrapper calling486BA0; its role is
not inferred from proximity to vector helpers.

## Math source and real VC9 overloads

Complete source is added for both math dependencies:

| Owner | Target bytes | Source bytes / masked differences |
| --- | ---: | --- |
| QuantizeSecondaryMotionPosition459A30 | 99 | 88 /74 |
| RotateSecondaryVector458F70 | 86 | 82 /73 |

Quantization multiplies by double100, rounds the input to float, calls floor,
rounds its result to float and divides by double100 for each position component.
Rotation rounds sin/cos to float before its two component expressions. The
actual VC9 math.h float overloads delegate through sinf/cosf/floorf to double
functions and return float; this explains the rounded return/spill boundaries.
Explicit double calls, direct expressions, float API spelling, const locals,
reversed products and float first multiplication do not close the owners.
Native quantization has64-byte frame alignment; rotation has8-byte alignment.
No artificial aligned local or replacement math ABI is introduced. The actual
FSINCOS SetPolar459AA0/30 remains separately source-open.

## Vendor floor identity and corrected inventory

The old automatic486590 span claimed3113 bytes through4871B8 and merged unrelated
runtime helpers. Independent vendor symbols and CodeView S_GPROC32 lengths split
out these complete procedures:

| Vendor procedure | Target range | Full bytes |
| --- | --- | ---: |
| _floor | 486590..4865CF | 64 |
| __floor_pentium4 | 4865D0..4866B0 | 225 |

The dispatcher checks SSE2/math control state and tail-transfers either to the
225-byte procedure or fallback4870E4. The latter and other intervening helpers
remain separate unresolved inventory, outside these two vendor procedures.
Read-only explicit-span review agrees with both lengths, real transfers,
neighbors and absence of foreign interior references. The complete contiguous
289-byte vendor text has zero differences after all ten COFF fields are replayed;
all80 constant bytes at498500 also agree. No text is cropped to obtain identity.

| Local witness | SHA-256 |
| --- | --- |
| libcmt.lib | `bd8fe9fa475deb84014af41870182b447bb969c91661d7012ae8d1b26bb6a86c` |
| _floor archive member | `29a9401686c990e8ea2f5fee0e5353421dc83b51f50a7312da10ed2f5f66b4de` |
| 80-byte constant pool | `74d6360aa933292bc01e981280e089e4138849b57953f5b535b3217b6578b4d0` |

The archive is bundled in the local VC9 tool tree, but its hash is an observed
witness, not a pre-existing tools.lock component pin. Vendor debug metadata names
CRT tran/i386/floor_pentium4.asm, MASM and compid95521E/build21022. Combined with
the complete replay, this establishes library origin without attributing the
game's wrapper source to the CRT.

| Text offset | Kind | Symbol | Target |
| ---: | --- | --- | --- |
| 2 | DIR32 | ___use_sse2_mathfcns | 4C9CC4 |
| 9 | REL32 | __floor_default | 4870E4 |
| 58 | REL32 | __floor_default | 4870E4 |
| 74 | DIR32 | _Bns | 498510 |
| 99 | DIR32 | _S | 498540 |
| 182 | REL32 | ___libm_error_support | 486E07 |
| 231 | DIR32 | _One | 498500 |
| 257 | DIR32 | _NegZero | 498530 |
| 266 | DIR32 | _NegZero | 498530 |
| 274 | DIR32 | _NegOne | 498520 |

All raw addends are zero. Both procedures are library exclusions, with no
reconstruction/exact credit. The previously uncertain486590 callee is now the
real double cdecl floor; this resolves dependency identity, not wrapper codegen.

## Exact proof, reproduction and scope

Eight canonical units close130 complete target bytes: mode4, initial11, target12,
timer52, duration getter4, stop8, subtraction16 and scaling23. The sole relocation
is timer offset31 DIR32 g_AnimationTimeScale->4A7948 with zero addend. Independent
native instruction decoding and prior scale ownership establish this binding.
Strict cold proof requires full unique public primary-TU PDB contributions,
exhaustive fields and complete zero-byte comparison with current source/header/
support/tool/producer/PE/MAP/PDB receipts. It passes for all eight owners.

The actual previous receipts select19 affected old units through StageTween,
StageScript, AnimationTween, SecondaryPairTween and SecondaryActorMotion. Final
selected cold replay passes27 units across6 linked families. Source hashes are
frozen before compilation and unchanged afterward; no full cohort is replayed.
The four remaining pair leaves and all large/math owners remain non-exact.

This adds14 source mappings/373 target bytes,11 omitted pair candidates and one
omitted CRT candidate. Fifteen new boundary/origin reviews include13 new
indeterminate origins and two library exclusions. Already reviewed motion math
and419660 retain their earlier indeterminate provenance; no origin is inferred
from successful codegen or proximity.

Private receipts/dumps remain in.analysis and build; no vendor binary, object,
decompiler dump or instruction bytes are committed. Relevant direct Ghidra
queries are span-ADDRESS-review.txt, with the existing full pair/state exports.
Private probes record full contributions and exhaustive decoded fields, never
prefix acceptance. Broad runtime/product/port work remains later.
