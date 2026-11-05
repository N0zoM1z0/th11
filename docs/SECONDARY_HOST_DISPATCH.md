# Complete secondary actor instruction dispatcher

The large host dispatcher at412E30 owns16,556 bytes, not the initial15,780-byte
code-only inventory span. This checkpoint reviews its complete boundary, tables,
control flow and call graph. It adds no maintained implementation or exact bytes.
The existing7,093-byte RunSecondaryScript reconstruction remains non-exact.

## Full extent and entry contract

The complete interval is412E30..416EDB; four CC bytes separate the next owner
at416EE0. Its parts are:

| Part | Bytes | Observation |
| --- | ---: | --- |
| Code interval |15,780 |15,776 reachable bytes plus four internal alignment bytes |
| Primary jump table |540 |135 distinct targets, including the common default return |
| Primary selector |195 |Signed opcodes100..1C2;154 handled values,41 default values |
| Table alignment |1 |Between selector and nested tables |
| Two nested jump tables |40 |Five DWORD cells each; each has four unique code targets |

Direct Ghidra's full4,001-instruction export agrees byte-for-byte with independent
PE decoding at every exported instruction. Linear decoding also accounts for the
one-byte alignment gap at415B5F and three-byte gap at41645D, giving4,003 instructions.
CFG traversal reaches every other instruction. All immediate branch destinations
and all145 jump-table cells resolve to decoded instruction starts in this owner.
The145 references that Ghidra reports as entering its code interior originate
exactly in these owned tables; there is no foreign interior reference.

The actor-manager virtual slot at494074 points to the11-byte412E20 adapter.
It adds103C to ECX and tail-transfers to412E30, agreeing with the existing physical
manager/state view. The dispatcher reads state.owner at1614, owner.current at4
and current.instruction at4, then dispatches the signed opcode. Its single common
epilogue sets EAX to zero and returns without stack arguments. Ghidra's inferred
fastcall/void prototype is not the ABI evidence. Ordinary member declarations are
the prospective source representation; original class identity remains unknown.

The native prologue aligns the stack to eight bytes, allocates310, registers an
exception frame with handler48AA9E and installs security-cookie checks. These are
target observations. Source lifetimes, EH/funclet ownership, original TU partition
and linked acceptance remain open; no constructor or forced frame is invented.

## Why this changes the interpreter work

The complete owner has465 direct call sites to64 targets, two indirect callbacks
and one RET. Among its direct calls are83 ReadSecondaryIntArgument and107
ReadSecondaryFloatArgument sites. It also uses the host-state argument adapters
at417D80/417DD0/417DF0/417E30 and calls SecondaryFloatDestination once.

The current interpreter diagnostic includes the actual UpdateSecondaryScripts
caller and independent API inputs, but has no source for this large host owner.
Thus it omits most real argument-reader call sites. This is a concrete context
gap, not proof that adding this owner will fix register allocation or tail sharing.
Next source work should reconstruct the complete host switch and its actual
argument/data contracts; a synthetic caller that imitates register choices would
not establish original LTCG context. Keep all tables in the extent and retain
the established /GL owners.

Two targeted compiler hypotheses preceding this review are rejected. Replacing
the union-based integer pop wrapper with a typed int-pointer implementation leaves
the full Run result7,013/5,821, frame120 and both missing call sites unchanged.
Using memcpy bit transport instead gives7,381/6,120 and frame1B0, still with a
merged unary84 push and shared82/87 destination. Neither changes public source.
No broad behavior harness or runtime comparison was run.

## Evidence and status

Private evidence is .analysis/secondary-host-dispatch-boundary-audit.json,
secondary-host-dispatch-complete.txt, secondary-host-getters-review.txt and their
mapped-text-attested Ghidra logs. The audit accounts for full ownership, sparse
alignment, every table/selector, reachable instruction, branch and call site.
Private decompiler output is a hypothesis for subsequent source work only.

The dispatcher and its necessary virtual adapter now have reviewed boundaries
and indeterminate origins. Source mappings and exact totals are unchanged.
The prior27-unit cold proof remains current: all37 receipt source/header/support
inputs are byte-identical, and no public source, tool or Oracle implementation
changed. No exact unit is rebuilt merely for this boundary/documentation checkpoint.
