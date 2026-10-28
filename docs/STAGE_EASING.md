# Polynomial easing dispatcher

`src/StageEasing.cpp` reconstructs the complete dispatcher at00459730. Its
initial seven-byte Ghidra inventory described only an unchecked indirect JMP.
Fresh direct local Ghidra review and an independent control-flow walk establish
a764-byte owner through00459A2B:698 code bytes, two alignment bytes and a64-byte
table. The next four bytes are CC padding before entry00459A30.

## Boundary and contract

The16-entry table starts at004599EC. Its destinations lead to21 returns and
cover every byte of the691-byte arm region after the seven-byte entry. Curves6
and7 share the linear arm. No arm calls another function or escapes this owner.
Ghidra reports no external interior references in the full reviewed span.
An independent scan of every raw-backed PE section finds only17 absolute
interior pointers: the entry's table address and the16 table destinations.

The review span includes the following four CC bytes so the linear decoder can
finish across the final data byte. Table bytes are data; the decoder's apparent
instructions there do not establish code ownership. The independently walked
code, two-byte alignment, table and next-function boundary establish the exact
764-byte extent. No database function expansion or mutation was required.

All seven known callers range-check unsigned(mode-1)<=15 before calling:
the stage vector/fog factor getters,419580 and44EDD0/44EE00/44EE30/44EE60.
The dispatcher itself has no check. Its source contract requires curve0..15;
`__assume` expresses that evidenced precondition, including the unreachable
switch default. Invalid indices remain outside the declared contract. Progress
is unrestricted: there is no clamp to0..1. Native curve arrives in EAX,
progress is a stack float, ST0 returns the float result and RET4 removes progress.
Natural ordinary stdcall declarations acquire that private ABI under VC9 LTCG.
Original provenance is indeterminate: polynomial formulas and game callers do
not independently identify the original author or a library.

## Formulas and rounding

Let p be the incoming float and q=float(2*p). In the following table n is2,3,4
for the three entries listed in each group. Powers mean left-associated
multiplications, not a call to pow.

| Curves | Formula |
| --- | --- |
| 0,1,2 | p^n |
| 3,4,5 | 1-(1-p)^n |
| 6,7 | p |
| 8,9,10 | q<1: q^n*0.5; otherwise (2-(2-q)^n)*0.5 |
| 11,12,13 | q<1:0.5-(1-q)^n*0.5; otherwise (q-1)^n*0.5+0.5 |
| 14 | 0 |
| 15 | 1 |

Curves8..10 double p with double2.0;11..13 use p+p. Both store that result to
float before the comparison and polynomial. Comparisons use float1.0, with
unordered input taking the second branch. Nonlinear results store to float
before being returned in ST0. Double literals1.0/2.0/0.5 and the original
expression order are retained; no new clamping or alternate exponentiation is
introduced. Curves14/15 do not load p, including when it is a signaling NaN.

## Code generation and exact evidence

An initial double local shared across the switch produced792 bytes with an
aligned stack frame and hoisted x87 loads. Branch-scoped double locals produced
804 bytes. Repeated expressions without those locals let VC9 retain values on
the x87 stack, producing748 bytes and matching the first256 bytes after address
normalization. Expressing out-in halves as0.5-power*0.5 and power*0.5+0.5,
in the observed order, reproduces the full764-byte body and table naturally.
No inline assembly, target byte arrays, padding or executable patching is used.

The canonical `evaluate-stage-easing` unit is a non-EH public primary-TU PDB
contribution with35 independently reviewed DIR32 fields: one local table base,
16 local arm destinations and18 external double-constant references. Local
destinations retain their exact offsets inside both complete owners. The three
constant objects at497E48/497E60/497E68 contain1.0/0.5/2.0; candidate values and
symbol identities were independently checked. The linked Oracle accepts the
complete764 bytes after exhaustive replay, with current source/tool/producer
receipts and matching PE/MAP/PDB identity and ownership.

```bash
scripts/repo-python scripts/replay-exact-units.py --unit evaluate-stage-easing
```

Its primary source is `src/StageEasing.cpp`, support is
`probes/stage-easing-context.cpp`, entry `StageEasingProbe@8`, output
`build/match-linked/StageEasing`, profile
`/TP /O2 /Ob1 /Gy /MT /Isrc /EHsc /GL`. The exported ordinary wrapper supplies
a stable test boundary around the compiler's actual private ABI.

## Factor getters and caller integration

Linking the real dispatcher naturally recovers the EAX curve convention in
both factor getters. The original assignment-and-if source then produces the
correct39/45-byte sizes but skips a redundant final float store on the non-eased
branch. Changing the unsigned range test from<16 to<=15 matches the target
comparison, leaving only that branch destination different. A double local
retains rounding but introduces an aligned frame, producing61/67 bytes. The
natural conditional expression `return curve<=15 ? EvaluateStageEasing(...) :
progress` instead joins both paths at the target's final float rounding point
and reproduces both complete bodies.

`stage-vector-tween-factor` and `stage-fog-tween-factor` use the existing
StageTween primary family. Each has one independently reviewed REL32 call to
459730, at offset27 or33 respectively. Their full public primary-TU owners,
current receipts and complete linked-field replay are required for exact
acceptance. The common source still declares ordinary functions; private
register conventions arise from LTCG rather than false prototypes.


Real easing is support in the tween and script canonical families. All three
new exact units add848 bytes; the affected setter59 and pack171 remain exact.
Only changed/affected families need cold replay. Broad behavioral matrices and
caller regressions are not checkpoint requirements.

The factor getters' earlier authored-origin assignment was corrected to
indeterminate. Their generic pattern also occurs at419580 and
44EDD0/44EE00/44EE30/44EE60 across layouts; reuse does not establish authorship.
