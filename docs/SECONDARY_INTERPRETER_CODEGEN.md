# Large interpreter: operand storage, evaluation order and dispatch layout

The current focus is the complete7,093-byte RunSecondaryScript owner at45B860.
Its maintained source now recovers the native fixed120 stack allocation in the
actual-caller context, all89 selector bytes and the physical order of all60
explicit opcode arms. The complete candidate is7,013 bytes/5,821 masked diagnostic
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

The earlier caller/trace checkpoint established the following layout baseline.

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

With both changes in the actual-caller context, that complete cold candidate is
7,033 bytes and naturally uses sub esp,120 without an EBP frame or dynamic stack
alignment. Both RET4 paths match. The original independent-helper context
happened to emit7,093 bytes but still aligned64; that numerical size coincidence
is not an exact match and is not used as acceptance evidence.

## Operand storage and angle evaluation

Fresh direct Ghidra owner review and independent full-code decoding distinguish
arithmetic operand storage from predicate/bitwise result storage. Native integer
arithmetic50/52/54/56/58 updates the left operand and passes that object's address
to PushSecondaryValue. The old inline helper passed every result by value through
PushInt, introducing a separate temporary. The maintained arithmetic now updates
left with the same wrapped add/subtract/multiply and signed divide/remainder.

Integer predicates and bitwise75/76/77 retain a distinct result object and pass
its address directly. Logical73/74 now each have one physical push and use result
slot18, as observed natively. Arithmetic50 no longer joins the integer-comparison
push tail, and52/58 no longer share a push. The native integer-comparison and
floating-comparison shared tails are both retained. All32 native and31 candidate
push sites are accounted for by opcode traversal; no unmapped push is hidden.
Unary84 now negates and publishes the popped object itself, though its remaining
branch layout and stack slot are still wrong.

Native87 snapshots endX/endY as doubles, rounds and pushes the X difference, then
reads the Y argument. The old separate x/y locals read both arguments before
either call argument was pushed. A natural nested angle-call expression recovers
the observed X-push-before-Y-read order in pinned VC9. The final float is still
rounded before destination lookup. Direct destination assignment was rejected:
it splits the shared tail but moves that lookup before the math call.

Only direct switch bodies84/87 and the integer-operation inline helper change.
The complete cold result is7,013/5,821 versus previous7,033/5,779. The full masked
diagnostic is worse, not an overall byte-match improvement; adoption rests on the
reviewed operand addresses, evaluation order and per-opcode call topology. Fixed
120 allocation, both RET4 paths,60-arm physical order,61 dispatch cells, all89
selector bytes and the full trace/reference call contract remain intact.

## Remaining concrete codegen differences

The native main VM lives in EDI and instruction/cursor work uses ESI; the current
candidate exchanges those roles and delays the EDI save. Typed stack-pop arithmetic
and stores still differ. There are130 native direct call sites versus128 candidate
sites, and one virtual host callback each. The full per-target audit differs only
at PushSecondaryValue32 versus31 and SecondaryFloatDestination6 versus5.

The two missing physical calls are now localized: native84 has separate
float-converted and raw integer negate/push paths, while the candidate merges
them; native82/87 each publish through a float destination, while the candidate
shares that tail. Logical73/74 each have one push in both images. Native endY
uses double slot20 versus candidate18; unary84 uses58 versus candidate3C.
The next work is these branch/lifetime differences and register allocation
inside this complete owner, not a new leaf-function search.

The next context investigation finds the real actor host at412E30 owns16,556
bytes and directly contributes83 integer/107 float argument-reader calls.
Its complete tables, branch reachability, vtable adapter and EH frame are now
reviewed; it has no maintained source in the current diagnostic. See
[HOST_DISPATCH](SECONDARY_HOST_DISPATCH.md). This motivates reconstructing its
full source, not assuming its presence would automatically close Run's gaps.

Moving trace alone to the end gives6,985/5,787 in the old helper context. A separate
/GL trace TU produces the same result and does not remove alignment. Native-order
source before typed trace gives6,889/5,925 in the actual-caller context with128
allocation. Changing advance to int, splitting the top local or using signed top
arithmetic leaves7,033/5,779 unchanged. None of those extra variants is adopted.

Further private ordinary-C++ hypotheses are also not adopted: explicit integer
case expansion, live argument-pointer reads, a float-result reference helper,
a return-value pop wrapper, a union operand and visible math support do not close
the remaining paths. Making Run an ordinary VM member leaves the main result
unchanged; making argument accessors members gives7,077/5,767 but changes the
frame to128 and still keeps the wrong VM register. Neither establishes original
class ownership. Removing the explicit reference-index mask gives7,041/5,843
and does not solve allocation; the maintained well-defined wrapping is retained.

## Changed-source proof and limits

Recursive current linked/COFF contracts select27 prior exact units depending on
SecondaryScript.cpp. All pass cold with current source/header/support/tool/producer
and PE/MAP/PDB receipts:1,027 bytes/22 exhaustively replayed fields, full complete
public primary-TU ownership and zero byte differences. Maintained source/header/
support hashes remain frozen through those builds and the separate actual-caller
cold diagnostic. No unrelated unit or full cohort is replayed.

The reviewed complete owner includes all tables, direct targets/multiplicities,
callbacks and returns. Per-opcode CFG analysis distinguishes native sharing
from extra candidate sharing. Original types and TU ownership remain open.
