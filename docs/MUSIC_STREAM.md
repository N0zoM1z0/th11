# Streaming buffers, decoder state and fades

Twenty-six complete source mappings cover3,230 target bytes, including buffer
refill570 and streaming update543. Seventeen new complete cold linked proofs
add1,185 bytes. Both larger bodies and seven other owners remain non-exact.
Thirteen omitted entries are recovered, bringing inventory to2,060. Twenty-three
new boundary/origin reviews supplement eight previously reviewed owners;
all31 reviewed origins remain indeterminate.

| Owner | Address | Full bytes | Current state |
| --- | --- | ---: | --- |
| RestoreMusicBuffer | 45A930 | 93 | Exact;1 field |
| SelectAvailableMusicBuffer | 45A990 | 100 | Source;100 /21 diagnostic differences |
| RefillMusicBuffer | 45A6F0 | 570 | Source;545 /500 |
| ResetMusicBufferPositions | 45ACD0 | 52 | Exact; no fields |
| PlayMusicStream | 45AA60 | 133 | Source;132 /20 |
| StopMusicStream | 45AB70 | 129 | Source;135 /129 |
| PauseMusicStream | 45AC00 | 117 | Exact;2 fields |
| ResumeMusicStream | 45AC80 | 77 | Source;77 /62 |
| UpdateMusicStream | 45AEE0 | 543 | Source;544 /421 |
| ResetMusicPosition | 45B100 | 223 | Source;220 /160 |
| OpenMusicDecoderFile | 45B220 | 164 | Exact;5 fields |
| ConfigureMusicDecoder | 45B2D0 | 130 | Exact;4 fields |
| ResetMusicDecoder | 45B3A0 | 223 | Exact;6 fields |
| ReadMusicDecoder | 45B480 | 192 | Source;158 /179 |
| QueryMusicBufferInterface | 45AA20 | 49 | Exact;1 field |
| IsAnyMusicBufferPlaying | 45AD10 | 78 | Source;78 /13 |
| StepMusicFadeMode1 /2 /3 /4 | 45ADD0 /45AE20 /45AE60 /45AEA0 | 70 /51 /51 /57 | Exact;1 field each |
| BindMusicDecoderMemory | 45B360 | 43 | Exact; no fields |
| MusicDecoderDataSize | 45B390 | 4 | Exact; no fields |
| CloseMusicDecoder | 45B540 | 32 | Exact;1 field |
| MusicDecoderSavedPosition | 45B560 | 7 | Exact; no fields |
| ConfigureControllerMusicDecoder | 45B570 | 16 | Exact;1 field |
| SaveMusicDecoderPosition | 45B580 | 26 | Exact;1 field |

## Corrected physical types and COM protocol

Record+1C is a data-byte count. The earlier `loop` name inferred from a boolean
use in the command dispatcher was wrong. Decoder restart/read paths copy it to
remaining bytes and memory extent; loopStart is the separate signed field+18.
The dispatcher still passes `dataSize != 0` at its observed refill sites. This
renaming/type correction changes no intended dispatcher branch behavior.

The34-byte record has dataOffset10, storedBytes14, loopStart18, dataSize1C,
format tag/channels20/22, sample rate24, bytes per second28, block alignment2C
and bits per sample2E. The decoder physical prefix has remaining8, dataSize2C,
closeOnStop78, memoryMode7C, base80, cursor84, memory size88, file8C, record90,
filename94 and saved file position98. Assertions check observed offsets.
Names do not establish original source types or full object/data ownership.

Controller fields60/64/68 are last play position, accumulated progress and next
write offset;6C selects silence and70 is notification size. Paused34 is distinct
from active30. The existing1C field named fadeEnabled stores modes1..4, not a
boolean. Busy78 retains the prior scoped worker protocol.

`SoundBuffer.hpp` now declares the complete21-method COM order, including
GetCurrentPosition10, GetStatus24, Lock2C, Play30, Stop48, Unlock4C and Restore50.
All methods have their actual stdcall receiver/argument/result contract.
Unused structure pointees remain opaque. Signatures were checked against the
installed DirectSound protocol declaration; no library implementation or new
compiler payload is imported. This replaces the earlier explicit function-table
view with ordinary pure virtual interface calls.

This typing matters for evidence as well as code generation. Ghidra's untyped
COM calls caused it to remove lost-buffer and looping branches as unreachable
in the refill decompilation. Complete raw control flow contains those paths;
source retains them all. Decompiler output is not the source/coverage authority.

The target itself contains RTTI names `CSound` and `CStreamingSound`.
Independent target-byte inspection confirms those strings and the497CBC
streaming vtable reference. The names support class identity but do not prove
unmodified SDK/sample authorship or original TU ownership. All new origin
reviews therefore remain indeterminate.

## Restoration, refill and notification behavior

Restore initializes an optional BOOL output to FALSE and propagates GetStatus
failure. An already available buffer returns S_FALSE. A lost buffer calls
Restore twice per iteration, sleeping10ms only if the first result equals
88780096, then retrying while the second result is any nonzero value. Success
sets optional output TRUE and returns S_OK. This is not a one-call retry loop.
The93-byte owner includes its complete loop/return layout; no padding is added.

Refill rejects a null buffer, restores it, locks its full buffer at offset0,
restarts the decoder and reads. A zero first read fills silence. A short nonzero
read either repeatedly restarts and fills the remaining bytes or fills the
remainder with silence, depending on the actual loop argument. No zero-progress
escape is added to the loop. Eight-bit PCM silence is128; other formats use0.
Unlock status is ignored. Existing early returns leave the native lock lifetime
unchanged; no new cleanup guard is introduced.

Notification first queries play/write positions without checking that HRESULT,
then applies the exact unsigned write-window test. It restores/refills on loss,
or locks the notification block. A nonnull second region returns E_UNEXPECTED
without a new unlock. Short reads repeatedly reset to loopStart and fill the
block; this actual boundary has no extra loop-mode parameter. Silence mode
fills instead. After unlock it checks a second position query, advances wrapped
play progress, optionally stops after decoder dataSize, and computes modular
nextWriteOffset. No readiness, loop-termination or output-count guard is invented.

The selector scans nonnull buffers for a clear playing bit. Each status starts0
and GetStatus HRESULT is ignored. If all buffers are used it calls real CRT
rand46190D and takes unsigned remainder bufferCount. There is no zero-count
recovery. The52-byte position reset ORs every SetCurrentPosition status and
retains native null-entry behavior.

Play selects/restores a buffer, refills on restoration, ignores the position
reset result, clears fade counters, sets volume0, and publishes active1,
priority0/flags1 before Play. Stop ORs all Stop/position statuses, clears active,
paused and fade mode, then optionally closes the decoder file. Pause retains
Stop's result, saves FILE_CURRENT, re-reads decoder and conditionally closes.
Resume ignores configure status, restores active and uses saved play flags.
These separate HRESULT/state behaviors are not collapsed into a generic helper.

## Decoder read, seek and memory binding

Memory read snapshots cursor before clearing an optional output count. That
ordering is present in the target and was corrected in the first source draft.
It clips the copy against base+memorySize, calls memcpy, increments cursor and
publishes copied bytes. The file path separately requires destination/count,
clips against remaining, subtracts before ReadFile, ignores the BOOL result and
returns S_OK with the transferred count. Invalid handle and null handle checks
remain distinct as in the native code; no generic file error policy is added.

Reset uses a positive signed record size to update the memory extent, optionally
moving cursor to positive loopStart. File loop seek uses dataOffset+loopStart+
archive offset and remaining=dataSize-loopStart. Zero offset seeks dataOffset+
archive offset; a nonzero offset instead seeks offset+archive offset and sets
remaining=dataSize+dataOffset-offset. Seek results are ignored. Archive root is
4C9094. The complete223-byte body, all six address fields and every byte pass.

Open validates only the path, calls trace45B5A0, opens with flags8000080, rejects
INVALID_HANDLE_VALUE and publishes record/path before restart and size copy.
Its memory-mode branch is retained even after opening a file. Configure rejects
memory mode, reopens an invalid file using the saved filename, publishes record,
traces, ignores reset's status and updates total size. Both164/130 owners pass
complete cold comparison.

The43-byte memory binder really mutates record, memory size, base, cursor and
memoryMode1, then returns E_NOTIMPL. This is observed behavior, not a stub
replacing missing code. Splitting base/cursor assignments preserves the target
store order and closes its two earlier encoding differences. Close returns S_OK
after the observed closeOnStop==1 gate and INVALID_HANDLE_VALUE store. Getter,
controller adapter and saved-position owners retain their actual effects.

## Missing owners, fades and compiler probes

Thirteen missing entries are independently bounded: interface query49, any-playing78,
plain streaming destructor tail11, four fades70/51/51/57, memory binder43,
data-size getter4, close32, saved-position getter7, configure adapter16 and
save-position26. RET/tail-jump endings and CC padding separate each entry.
Group interior references target only known constructor/destructor/reset/read
entries, never an instruction inside an individual owner. No embedded tables
or truncated function slices enter these mappings.

Each fade checks its own mode, decrements remaining and returns1 on completion.
Mode1 also stops the first buffer. Other modes clear the mode without forcing
final volume. Running attenuation uses signed division after32-bit wrapped
multiplication: mode1 is5000*r/n-5000; mode2 -5000*r/n; mode3 -1000*r/n;
mode4 1000*r/n-1000. Original counters and divisor behavior are retained.
All four complete owners have cold zero proofs.

The new COM interface improves selector100 from44 to21 diagnostic differences,
any-playing78 from38 to13, and notification543 from538/428 to544/421. Refill
remains545 bytes/500 differences. Its receiver remains stack-passed in the
candidate, while native uses EBX; decoder read uses extra private registers
instead of native two stack arguments/RET8. Those calling-context differences
propagate through the larger owners.

A genuine member-call refill experiment left its code generation unchanged
and was not adopted. A public `/Ob2` probe left the large gaps unchanged but
inlined reset logic into configure, expanding its130-byte owner to265 bytes;
canonical context retains `/Ob1` and `/GL`. No register promises, synthetic
parameter uses, cropping or alignment padding are introduced.

Source lifetime work is still open for base ctor119/destructor133, streaming
ctor48/scalar-deleting destructor36 and its11-byte plain destructor tail.
Their full bodies and references were reviewed without supplying guessed
constructor/destructor implementations or assigning ordinary exact credit.

## Cold proof and remaining scope

New family `build/match-linked/MusicStream` uses the17 new complete public
primary-TU contributions,1,185 bytes and26 independently reviewed fields.
Fields bind Sleep/CloseHandle/CreateFileA/SetFilePointer IAT slots, archive4C9094,
trace45B5A0 and its two formats, GUID48D11C, real decoder/helper calls and
volume45AAF0. Every field and every byte replay with current source/header/
tool/producer/PE/MAP/PDB receipts and owner agreement.

The existing SoundQueue family now includes actual MusicStream support.
Dispatcher diagnostic improves from1,556/1,085 to1,540/1,077. Its three existing
exact owners (worker177, enqueue151, volume125) retain453 bytes/19 fields of
full zero proof after the header/type/context changes. Cold selection was
exactly17 new plus these three affected owners:20 units,1,638 bytes/45 fields.
No unrelated exact cohort, broad behavior suite or infrastructure test ran.
Infrastructure is unchanged; original TU/data ownership and native product
closure remain separate open work.

Private evidence includes `music-stream-full-review.txt`,
`music-buffer-interface-gap-review.txt`, `music-fade-gaps-review.txt`,
`music-decoder-gaps-review.txt`, `music-parent-lifetimes-review.txt` and their
full mapped-text attestations; `music-stream-independent-review.json`,
`music-stream-independent-bindings.json`, `music-stream-com-protocol-reference.json`;
baseline/member-refill/virtual-buffer/automatic-inline probes;
`sound-queue-stream-owner.log`; `music-stream-twenty-affected-cold.log`.
Decompiler hypotheses, target bytes, tool payloads and PE/MAP/PDB artifacts
remain private.
