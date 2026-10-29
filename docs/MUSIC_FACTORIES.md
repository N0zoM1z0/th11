# Streaming factories and sound lifetimes

Eight newly source-present complete target owners cover1,755 bytes, including
file713 and memory665 factories. The real C++0x38-byte base/0x7C-byte derived
hierarchy replaces the earlier declaration-only controller destructor. Two
ordinary destructor owners /144 bytes have unique complete public primary-TU
contributions. Scalar-delete30/36 diagnostic zeros fail the current ownership
check and receive no exact credit. Eight new boundary/origin reviews recover
five further omitted entries; all origins remain indeterminate.

| Owner | Address | Full bytes | Current source / diagnostic differences |
| --- | --- | ---: | --- |
| CreateFileMusicStream | 459EB0 | 713 | 675 /590; EH ownership open |
| CreateMemoryMusicStream | 45A1B0 | 665 | 595 /556; EH ownership open |
| MusicSoundBaseConstructor | 45A450 | 119 | 116 /83 |
| MusicSoundBaseScalarDeletingDestructor | 45A4D0 | 30 | 30 /0; ambiguous MAP aliases |
| MusicSoundBaseDestructor | 45A660 | 133 | Exact;4 fields |
| MusicStreamingConstructor | 45AD60 | 48 | 48 /23 |
| MusicStreamingScalarDeletingDestructor | 45AD90 | 36 | 36 /0; ambiguous MAP aliases |
| MusicStreamingDestructorTail | 45ADC0 | 11 | Exact;2 fields |

Inventory names for constructors/destructors remain semantic boundary names.
Their source is actual C++ constructors, virtual destructors and compiler-emitted
scalar-delete entrypoints. No C++ method with an invented flag ABI, raw vtable
write, hand-written deleting wrapper or instruction payload is supplied.

## Complete factory behavior

Both device-owner methods first reject a null device. Their buffer/notification
locals start at zero before decoder allocation. The decoder constructor writes
only record90, unknown DWORD0, dataSize2C and memoryMode7C. Other fields retain
native uninitialized state; the source does not silently initialize a whole
object. The unknown DWORD0 is not assigned an unproved format-pointer role.

The file path sets memoryMode0 and closeOnStop1, then calls the real archive-file
open with the target filename symbol494B9C (thbgm.dat) and record. Any nonzero
HRESULT, including a positive result, takes decoder destruction/delete and
E_FAIL. This differs from testing FAILED. The memory path instead calls the real
memory binder and ignores its observed E_NOTIMPL result after its state writes.
It does not invent closeOnStop/file initialization for this mode.

Both initialize a36-byte description, flags18188, bytes=16*notificationBytes,
caller-provided GUID and the wave-format view at record20. Device buffer creation
and notification QueryInterface failures return E_FAIL without added rollback.
Each allocates16 positions; allocation failure returns E_OUTOFMEMORY. Positions
contain notificationBytes*(index+1)-1 and the supplied event. Registration failure
releases the interface/deletes positions, then returns E_FAIL. Success has that
same temporary-interface/array cleanup before constructing the real controller.
Later failure lifetimes differ from the file-open error path; source retains them.

The caller publishes new controller, copies all nine DWORDs of the description
to38, and stores device owner5C, event74 and busy78=0. It does not add a null guard
after controller allocation. Array allocation overflow behavior and constructor
exception cleanup come from the actual compiler's ordinary new expressions.
Full source does not establish exact EH metadata or whole-game runtime closure.

The factories began as free functions with a shared inline implementation.
Defining real device-owner methods reproduced the native32/36 stack-pop contracts
without a forced register convention. Explicit complete bodies retain the two
observed pre-allocation output locals; their final675/595 diagnostics remain open.
Original C++ spelling/TU boundaries remain unknown; these are natural source
expressions of the independently reviewed target behavior.

## Base and streaming construction/destruction

The base constructor allocates/copies a count-sized buffer pointer array, stores
buffer size/count/decoder, calls actual refill and SetCurrentPosition0 while
ignoring both results, then clears active/paused. The real derived caller passes
count1; pinned LTCG naturally specializes the allocation/copy and count store.
No artificial constant parameter use is added. Candidate116 versus target119
still depends on refill's native EBX receiver versus candidate stack argument.

Derived construction calls the base, clears notification fields60/64/68/6C and
sets70. C++ emits the actual derived vptr. The description, owner, event and busy
are deliberately left to the factories. Its48-byte candidate still has23
non-address diagnostic differences in scheduling/registers.

The ordinary base destructor emits its base vptr, releases/clears nonnull buffers,
deletes/clears the array, snapshots decoder, conditionally closes its file for
closeOnStop==1, deletes decoder and clears the member. It preserves native null-
array/positive-count behavior. The complete133-byte body has no EH frame and
one unique primary-source public contribution.

The real empty derived virtual destructor produces an11-byte ordinary public
entry with derived vptr and tail jump to the base destructor. Making that empty
definition inline also lets the compiler emit the native36-byte scalar-delete
body. This is normal C++ lifetime generation; source does not encode flags,
returns, vtable addresses or native instructions manually.

## Ownership rejection and cold proof

The first affected cold replay attempted four lifetime owners after all29 older
affected units passed. It rejected base scalar-delete30 with `ambiguous MAP owner
or interior public entry`. Inspection shows both `??_E...` vector-delete and
`??_G...` scalar-delete public names share its entry; the derived36-byte entry has
the same ambiguity. Complete zero masked diagnostics therefore do not establish
accepted ownership. Both remain source-present/non-exact; their two candidate
units were removed. The Oracle and ambiguity rule were not weakened or patched.

Base133 and ordinary derived11 have one public MAP symbol each, complete primary
PDB contributions and no static, split or EH owner. Six independent target fields
bind base vtable497CB4, derived vtable497CBC, real base destructor45A660, operator
delete45FD49 and CloseHandle48B0A0. Raw target tables independently reference
scalar-delete45A4D0/45AD90; this validates address meaning without claiming whole
vtable/RTTI data reconstruction. Original RTTI still does not prove authorship.

Final cold scope is two new plus exactly29 affected MusicStream/SoundQueue owners:
31 complete contributions /2,885 bytes /65 fields. Every source/header/tool/
producer/PE/MAP/PDB receipt and full byte comparison pass with zero differences. Other exact
families, the full cohort and broad behavior/infra suites are outside this change.
Infrastructure is unchanged.

## Additional target review and remaining large paths

Fresh complete Ghidra review covers both factories and five existing lifetime
entries. Their713/665 intervals still include10/8 internal alignment bytes omitted
by sparse703/657 Ghidra bodies. Plain derived11 remains independently bounded in
the earlier explicit destructor gap and fresh PE decoding. Nothing is cropped.

A112-byte explicit read-only span45B5A0..45B60F and reference queries recover five
missing helpers: relative stack address15, absolute base+offset3, frame-offset
get7/set7 and table102C getter7. RET/RET4 and CC padding partition all five.
Eight group references enter the separate RET-only trace45B5B0, not the middle
of any individual accessor. Trace45B5A0/1 and45B5B0/1 remain source-open here.

The adjacent584-byte secondary-script frame helper is fully reviewed, preserving
one internal alignment byte omitted by sparse583. It handles f/g operand type
bytes, argument masks, integer/float conversion, bounded frame pushes, saved
frame state and subroutine lookup; failure traces and clears current context.
Two callers are45B860 and45D7A0. The later [SECONDARY_SCRIPT](SECONDARY_SCRIPT.md)
checkpoint supplies full source and corrects the earlier code-only6759 interval
to7093 including its two tables; exact codegen remains open.
Generic VM/frame/helper roles do not establish original game/header authorship.

Removing the diagnostic entry's extra direct Read/Refill calls changed no larger
owner code, both before and after actual factory/lifetime support; that hypothesis
is rejected. Canonical diagnostic roots stay intact. Refill569/504, read160/177,
notification544/421, reset-position220/160 and play132/18 still require work.

Private evidence: music-factory-lifetimes-review.txt and complete mapped-text
attestation; music-after-gap-review.txt/xrefs.txt; music-factory-independent-review.json;
full-lifetimes/member-factories/explicit-factory-locals and native-caller probes;
music-lifetime-four-provisional-bindings.json and current two-owner bindings;
failed33-owner and final31-owner cold logs. Native bytes, decompiler hypotheses,
compiler payloads and PE/MAP/PDB artifacts remain private.
