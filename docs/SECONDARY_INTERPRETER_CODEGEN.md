# Large interpreter: caller context, trace types and dispatch layout

The current focus is the complete7,093-byte RunSecondaryScript owner at45B860.
Its maintained source now recovers the native fixed120 stack allocation in the
actual-caller context, all89 selector bytes and the physical order of all60
explicit opcode arms. The complete candidate is7,033 bytes/5,779 masked diagnostic
differences. It remains non-exact; no bytes are newly credited for this owner.

## Full owner and actual caller

Fresh direct local mapped-text-attested Ghidra review confirms the sole incoming
call45D443 from UpdateSecondaryScripts. Existing full1783-instruction/code/table
review remains authoritative:6,759 code bytes, one alignment byte,61 dispatch
cells/244 bytes and89 selectors. No dispatch data or trailing bytes are cropped.

The older independent-helper probe directly invoked both Run and its updater.
That diagnostic context gave the maintained old source6,993/5,880 with dynamic
64-byte stack alignment. Routing Run through its only observed caller instead
makes the same old source6,945/5,920 with a fixed128 allocation. Thus64 alignment
is context-dependent; a real double argument does not by itself require it.
A small static compiler isolation with an unresolved ordinary variadic boundary
also passes a QWORD without dynamic alignment. No runtime harness is involved.

The new probes/secondary-interpreter-context.cpp keeps the real independent
helper inputs but reaches Run through UpdateSecondaryScripts. It uses the same
VC9 /TP /O2 /Ob1 /Gy /MT /Isrc /EHsc /GL profile, primary SecondaryScript.cpp and
entry SecondaryInterpreterProbe@32. Original TU ownership remains unknown.
The established helper probe and every /GL owner are retained unchanged.

## Source changes grounded in the full function

Native case30 trace is the final opcode arm, after the geometry operations.
The native layout also groups thread/frame operations, integer arithmetic,
floating arithmetic, integer comparisons, floating comparisons and logical/math
operations in a different order from the previous C++ switch. Reordering the
existing case blocks recovers the native physical order for all60 explicit arms.
A source audit verifies every opcode body is unchanged; selectors remain all89
bytes equal and the dispatch still owns61 cells.

The native trace first selects output format, then reads an integer or real value.
The source now follows that structure through a typed inline helper. Each format
keeps its own reference resolution: the two native format paths each have an int
and float reference call. Seven trace calls, malloc/strchr/free, the integer
conversion and the actual QWORD floating argument remain. No debug behavior is
ablated, no integer argument substitutes for a double, and no fixed format or
fabricated calling convention is introduced.

With both changes in the actual-caller context, the complete cold candidate is
7,033 bytes and naturally uses sub esp,120 without an EBP frame or dynamic stack
alignment. Both RET4 paths match. The original independent-helper context
now happens to emit7,093 bytes but still aligns64; that numerical size coincidence
is not an exact match and is not used as acceptance evidence.

## Remaining concrete codegen differences

The native main VM lives in EDI and instruction/cursor work uses ESI; the current
candidate exchanges those roles and delays the EDI save. Typed stack-pop arithmetic
and stores still differ. There are130 native direct call sites versus128 candidate
sites, and one virtual host callback each. The full per-target audit differs only
at PushSecondaryValue32 versus31 and SecondaryFloatDestination6 versus5.

A control-flow audit identifies extra candidate tail sharing: integer add50 joins
the59/61/63/65/67/69 push tail; subtract52/remainder58 share another push;82/87
share float-destination publication. Native comparison tails also share code,
so blanket disabling of optimization would not reconstruct the observed layout.
The next work is operand/result lifetimes, these specific tails and register
allocation inside this complete owner, not a new leaf-function search.

Moving trace alone to the end gives6,985/5,787 in the old helper context. A separate
/GL trace TU produces the same result and does not remove alignment. Native-order
source before typed trace gives6,889/5,925 in the actual-caller context with128
allocation. Changing advance to int, splitting the top local or using signed top
arithmetic leaves7,033/5,779 unchanged. None of those extra variants is adopted.

## Changed-source proof and limits

Recursive current linked/COFF contracts select27 prior exact units depending on
SecondaryScript.cpp. All pass cold with current source/header/support/tool/producer
and PE/MAP/PDB receipts:1,027 bytes/22 exhaustively replayed fields, full complete
public primary-TU ownership and zero byte differences. Maintained source/header/
probe hashes remain frozen through those builds and the separate actual-caller
cold diagnostic. No unrelated unit or full cohort is replayed.

Private evidence uses .analysis/secondary-interpreter-hard-* and
build/probe-ltcg/SecondaryInterpreterHard/. The maintained audit covers the complete
owner, tables, all direct targets/multiplicities, callback and returns. No broad
behavioral or unchanged-Oracle/infrastructure suite is run. Original types/TU,
whole-game product/runtime and portability closure remain separate open questions.
