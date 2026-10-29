# Secondary extension parser, file entry and trace context

Five new complete source mappings and exact owners cover340 target bytes. The
full238-byte ANIM/ECLI callback,95-byte file entry,5-byte file getter and two
genuine RET-only trace functions pass complete strict cold proof. At this
extension checkpoint exact state was156 functions/21,872 bytes; boundary/origin state was2,082 candidates,
349 reviewed boundaries,277 mappings,90 authored,249 indeterminate,8 exclusions
and1,735 pending origins. Later lifetime/source/proof updates are in
[SECONDARY_LIFETIME](SECONDARY_LIFETIME.md).

| Owner | Address | Full bytes | Exhaustive address fields |
| --- | --- | ---: | ---: |
| ProcessSecondaryProgramExtensions | 410D50 | 238 | 5 |
| LoadSecondaryProgramFile | 4104D0 | 95 | 5 |
| SecondaryProgramFile | 40FFB0 | 5 | 0 |
| TraceAnimationLoad | 456AD0 | 1 | 0 |
| TraceSecondaryScript | 45B5B0 | 1 | 0 |

## Complete extension and file behavior

The corrected238-byte owner includes all three RET4 paths and native internal
alignment. Bad ANIM magic returns0. The unsigned count loop loads each name
through AcquireAnimationDocument(index+8,name), then publishes the returned
pointer at singleton+44+index*4 before testing failure. Failure calls the real
AppendLog on the runtime log prefix and returns-1; it does not erase the published
null or release earlier files. Success advances each name by strlen+1.

The cursor is aligned relative to the original extension start using signed
remainder4. ECLI magic enables a second unsigned count/name walk through actual
virtual slot8; those return values are ignored. The parser finally returns0.
No guessed extra header fields, capacity checks or allocation/null guards appear.
The singleton is the same address4A8D7C used by the mesh-effect event-count view;
its complete type and slot capacity remain unknown. The new byte view does not
create another native storage object.

The derived program view overrides slot4 and adds LoadFile at8. It describes the
observed virtual prefix. Its derived constructor and full1098-byte raw storage
view are now reconstructed in the lifetime record; original member identities
remain open. LoadFile clears shared filename byte0, uses strcat with its observed
source scan/destination scan/copy order, loads through LoadResourceFile with null
size and mode0, calls the base Install directly and translates index<0 to-1,
otherwise0. There is no added load-null guard or ownership cleanup. The filename
buffer at4C9928 has no invented public capacity.

Native file getter40FFB0 is files[index] at program+C. Its five-byte full extent
ends before eleven CC bytes. A normal program member reproduces it without
assigning the native private EAX/ECX convention in source.

## Compiler evidence and exact field meanings

Initial full source generated238/67 and91/31. Real animation/loader/log sources
are support TUs, not synthetic implementations. Independent variable filename,
size and mode inputs keep LoadResourceFile from specializing its two null/zero
arguments away, recovering both native pushes and all95 bytes.

The native animation trace is RET-only and preserves EAX/ECX across the cached
loader call. An opaque declaration falsely forces volatile-register saves.
Adding the real body recovers the native index-in-ECX caller contract. A fixed
ignored format parameter is removed by LTCG and loses native format pushes;
the source now declares all supplied values variadic. The shared declaration
changes no physical layouts. AnimationTrace.cpp contains the actual empty body,
not a success substitute for a missing operation. Its simple diagnostic caller
passes independent message/value arguments and is never executed.

The parser's last66 differences came from retaining a remainder temporary:
extra EDX/EAX movement shifted the alignment/error region by two bytes. Expressing
the repeated signed relative remainder directly in the condition and increment
lets VC9 recover the native register use and internal alignment. The complete
238-byte contribution then has zero differences. No instruction bytes, padding,
fake ABI, manufactured aliases or Oracle exceptions were introduced.

Independent raw PE decoding establishes every external address field:

| Owner | Field offset | Type | Native target |
| --- | ---: | --- | --- |
| Extension | 52 | REL32 | AcquireAnimationDocument454360 |
| Extension | 58 | DIR32 | singleton pointer4A8D7C |
| Extension | 156 | DIR32 | load-error text493874 |
| Extension | 161 | DIR32 | runtime log prefix4A5940 |
| Extension | 166 | REL32 | AppendLog458A10 |
| File entry | 11,26,60 | DIR32 | shared filename4C9928 |
| File entry | 67 | REL32 | LoadResourceFile458400 |
| File entry | 75 | REL32 | base Install45D900 |

All internal branch targets land on instruction boundaries. The94-instruction
parser and41-instruction file entry have no owned tables or foreign interior
entries. The real derived vtable494090 independently binds Install, extension
and file slots. The three tiny owners have no address fields. Full public
primary-TU PDB contributions and current receipts are required for all five;
support-TU loader/log bodies receive no exact credit in this family.

## Wider boundary and origin review

Thirteen complete spans were inspected, ten newly reviewed. Seven omitted
entries are recovered: file getter40FFB0/5, base constructor40FFC0/38, four
singleton helpers410530/11,410540/9,410550/10,410560/21, and derived constructor
410D20/38. Explicit read-only spans and independent full decoding partition
the groups at their real returns and exclude CC separators. They have no
foreign interior references or tables. The four helpers store indexed DWORDs
at40/1C, read field68 or replace flag3C's low bit; stronger meanings and types
remain unknown.

Base construction really writes49404C before memset1098, erasing its own vptr.
Derived construction clears1098 first and publishes494090 afterward. The former
matches the unusual browser initialization previously observed. Do not rewrite
either into an assumed safe constructor. Later source models the full observed
1098 allocation, closes the derived38 bytes and retains the base38/3 and owner
gap; see the lifetime record. Original raw-region member identities remain open.

New host reviews cover initialization410BE0/318 and state reset410440/126. The
initializer imports an existing animation file, constructs/publishes the derived
program, calls slot8 and registers two24-byte chain nodes at priorities18/20 with
callbacks4111A0/B0. Its original unchecked failures, timer writes and return0
remain observations. The reset clears nine flag bits, zeroes eight214-byte records
with208 sentinel-1, clears two final flags and returns self. Full host identity,
construction and source remain open. Trace456AD0 is also newly reviewed. All ten
new origins stay indeterminate; custom formats and generic RET/constructor
bodies alone do not prove original authorship.

## Large interpreter investigation and replay scope

The real script trace body is now maintained at its observed one-byte owner.
Full Run changes6989/5958 to6993/5880 and still owns61 dispatch cells/89 selectors;
its complete target is7093 bytes. Enter-frame remains572/546, pop83/2 and
end-frame41/2. No encoding gaps are masked.

A private trace-arm ablation gives6541/5989 and alignment8 instead of64. Removing
only the double-valued variadic trace call gives6965/5960 and alignment8. These
diagnostics localize the64-byte alignment to that call; they omit native behavior
and are rejected. The native caller really passes a QWORD trace value and needs
no dynamic64 alignment. Adding a fixed format parameter to the real trace body
does not fix alignment. A controlled /fp:fast probe also retains64 and gives
6961/5940; it is rejected. Canonical /O2 /Ob1 /GL and real double arguments remain.
The remaining EBP/stack/register/rounding context is open, not papered over by
changing types or declaring alignment.

All84 selected units pass `.analysis/secondary-extension-affected-cold.log`:
five new complete unique public primary-TU contributions/340 bytes/ten fields,
and79 affected prior owners. Current source/header/support/tool/producer and
PE/MAP/PDB receipts, full ownership, exhaustive replay and every byte pass.
Actual program/trace files froze before their cold builds. Selection comes from actual prior receipt
source/header hashes, including transitive includes:79 prior units are affected
by the AnimationDocument.hpp trace declaration and script/program changes,
plus five new units. No unrelated unit or full cohort is selected. The compared
infrastructure is unchanged; broad behavior and infrastructure suites are not
checkpoint prerequisites.

Private evidence includes secondary-extension-loaders-review, secondary-host-gaps-review,
secondary-program-accessor-constructor-review, secondary-program-constructor-review,
animation-trace-context-review and secondary-trace-owner-review (mapped-text-attested);
independent full spans/vtable/ten fields; full-source, animation-trace,
animation-ellipsis, explicit-offset and inline-offset diagnostics; trace ablation,
body/fixed-format/no-double and fp:fast probes; current affected cold receipts.
Original bytes, decompiler output, compiler payloads and build artifacts stay
private. Native product, runtime and portability remain open.
