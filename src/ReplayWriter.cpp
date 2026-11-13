#include "PauseMenu.hpp"
#include "SecondaryPlayerController.hpp"
#include "SecondaryReplayState.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SharedFile.hpp"
#include "ByteCipher.hpp"
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// The writer and playback loader share the same physical replay-state root.
// The stage-link table is also populated by the original 00436B60 file loader.
#pragma pack(push, 4)
struct ReplayWriteHeader
{
    char name[8];
    unsigned char unknown008[4];
    __time64_t timestamp;
    int score;
    unsigned char unknown018[0x54 - 0x18];
    float slowRate;
    int stageCount, character, shot, difficulty, stage;
    unsigned char unknown06c[4];
};
#pragma pack(pop)
struct ReplayWriteFileHeader
{
    unsigned char unknown000[0x0c];
    int dataEnd;
    unsigned char unknown010[0x1c - 0x10];
    int compressedSize, rawSize;
};
struct ReplayWriteStage
{
    int unknown000, eventCount, bytes;
    unsigned char unknown00c[0x90 - 0x0c];
};
struct ReplayWriteSegment;
struct ReplayWriteSegmentNode
{
    ReplayWriteSegment *segment;
    ReplayWriteSegmentNode *next, *previous;
};
struct ReplayWriteSegment
{
    ReplayWriteSegment()
    {
        memset(this, 0, sizeof(*this));
        eventEnd = reinterpret_cast<unsigned char *>(this);
        secondaryEnd = secondary;
        node.segment = this;
        node.next = 0;
        node.previous = 0;
    }
    unsigned char events[0x1518];
    unsigned char *eventEnd;
    unsigned char secondary[0x18a0 - 0x151c];
    unsigned char *secondaryEnd;
    ReplayWriteSegmentNode node;
};
typedef ReplayStateView ReplayWriteState;
typedef ReplayStageStreamView ReplayWriteStream;
extern ReplayWriteState *g_SecondaryReplayCurrent;
typedef char ReplayHeaderStride[(sizeof(ReplayWriteHeader) == 0x70) ? 1 : -1];
typedef char ReplayStageStride[(sizeof(ReplayWriteStage) == 0x90) ? 1 : -1];
typedef char ReplaySegmentStride[(sizeof(ReplayWriteSegment) == 0x18b0) ? 1 : -1];
typedef char ReplayWriterFlagsOffset[(offsetof(ReplayWriteState, flags) == 0x1d8) ? 1 : -1];
typedef char ReplayHeaderTimestampOffset[(offsetof(ReplayWriteHeader, timestamp) == 0x0c) ? 1 : -1];
typedef char ReplayHeaderStageOffset[(offsetof(ReplayWriteHeader, stage) == 0x68) ? 1 : -1];
typedef char ReplayFileHeaderPackedOffset[(offsetof(ReplayWriteFileHeader, compressedSize) == 0x1c) ? 1 : -1];
typedef char ReplayStreamStartOffset[(offsetof(ReplayWriteState, streams) == 0x40) ? 1 : -1];
typedef char ReplayStreamTailOffset[((offsetof(ReplayWriteState, streams) + 7 * sizeof(ReplayWriteStream) + offsetof(ReplayWriteStream, last)) == 0x9c) ? 1 : -1];
typedef char ReplaySegmentEventEndOffset[(offsetof(ReplayWriteSegment, eventEnd) == 0x1518) ? 1 : -1];
typedef char ReplaySegmentSecondaryEndOffset[(offsetof(ReplayWriteSegment, secondaryEnd) == 0x18a0) ? 1 : -1];


struct PauseRecordTimingView
{
    unsigned char unknown000[0x24];
    double elapsed, limit;
};
extern PauseRecordTimingView *g_PauseRecordTiming;

// These two name tables are observed at 004A3A88 and 004A3AA0.
extern const char *g_ReplayCharacterNames[];
extern const char *g_ReplayDifficultyNames[];

// The native compressor is 004423A0; its original ownership is unresolved.
unsigned char *__stdcall CompressReplayData(const void *input, int size, int *compressedSize);

// 00437300: append a newly allocated chunk to the current stage's intrusive
// list. The node lives at offset 18A4 inside its 18B0-byte allocation.
extern "C" void *__cdecl ReplayAllocate(unsigned int size);
extern "C" void *__cdecl ReplayMemset(void *destination, int value, unsigned int size);
extern "C" void *__cdecl ReplayMemcpy(void *destination, const void *source, unsigned int size);
#pragma comment(linker, "/alternatename:_ReplayAllocate=??2@YAPAXI@Z")
#pragma comment(linker, "/alternatename:_ReplayMemset=_memset")
#pragma comment(linker, "/alternatename:_ReplayMemcpy=_memcpy")

// Native 00437300 is a private EBX/EDI routine, not a stack-argument C++
// function. Keep the exact allocation, zeroing, cursor initialization and
// generic intrusive-list splice in one naked body so no ABI save/restore code
// is introduced around the two live registers.
__declspec(naked) ReplayWriteSegmentNode *__stdcall AppendReplayWriteSegment()
{
    __asm {
        push esi
        push 18B0h
        call ReplayAllocate
        mov esi, eax
        add esp, 4
        test esi, esi
        jz allocation_failed
        push 18B0h
        push 0
        push esi
        call ReplayMemset
        mov dword ptr [esi+1518h], esi
        lea eax, [esi+151Ch]
        mov dword ptr [esi+18A0h], eax
        add esp, 0Ch
        mov dword ptr [esi+18A4h], esi
        mov dword ptr [esi+18A8h], 0
        mov dword ptr [esi+18ACh], 0
        jmp allocation_complete
    allocation_failed:
        xor esi, esi
    allocation_complete:
        lea ecx, [edi+edi*2+0Fh]
        cmp dword ptr [ebx+ecx*4+4], 0
        lea ecx, [ebx+ecx*4]
        lea eax, [esi+18A4h]
        pop esi
        jz append_tail
        align 16
    append_walk:
        mov ecx, dword ptr [ecx+4]
        cmp dword ptr [ecx+4], 0
        jne append_walk
    append_tail:
        mov edx, dword ptr [ecx+4]
        test edx, edx
        jz append_link
        mov dword ptr [eax+4], edx
        mov edx, dword ptr [ecx+4]
        mov dword ptr [edx+8], eax
    append_link:
        mov dword ptr [ecx+4], eax
        mov dword ptr [eax+8], ecx
        inc dword ptr [ebx+0A0h]
        ret
    }
}

namespace
{
__forceinline int EventBytes(const ReplayWriteSegment *chunk)
{
    const unsigned char *start = reinterpret_cast<const unsigned char *>(chunk);
    return static_cast<int>((chunk->eventEnd - start) / 6) * 6;
}
__forceinline int ExtraBytes(const ReplayWriteSegment *chunk)
{
    return static_cast<int>(chunk->secondaryEnd - chunk->secondary);
}
__forceinline void WriteReplaySection(const void *buffer, unsigned int size)
{
    if (g_SharedFile == INVALID_HANDLE_VALUE)
        return;
    DWORD written;
    WriteFile(g_SharedFile, buffer, size, &written, 0);
    if (written != size)
    {
        CloseHandle(g_SharedFile);
        UnlockSharedFile();
    }
}
__forceinline unsigned int AlignFour(unsigned int length)
{
    return (length + 3u) & ~3u;
}
}

// 00436420..00436B5F. Conventional fastcall signature follows the observed
// ECX filename / EDX name / stack mode entry, without modeling private
// optimized ABIs of its helper functions as source declarations.

// The original writer is a 1856-byte mixed-register owner. Keep the native
// EAX/ECX/EDX entry, EBP replay root, stack-cookie frame, and private calls.
extern unsigned int g_InputRuntimeFlags;
extern CRITICAL_SECTION g_FileCriticalSection;
extern unsigned char g_FileCriticalDepth;
extern const double g_ReplayWriterPercentScale;
extern const char g_ReplayWriterString_4955A0[];
extern const char g_ReplayWriterString_4955A8[];
extern const char g_ReplayWriterString_4955F4[];
extern const char g_ReplayWriterString_495618[];
extern const char g_ReplayWriterString_495620[];
extern const char g_ReplayWriterString_495630[];
extern const char g_ReplayWriterString_49563C[];
extern const char g_ReplayWriterString_49565C[];
extern const char g_ReplayWriterString_495668[];
extern const char g_ReplayWriterString_495674[];
extern const char g_ReplayWriterString_495688[];
extern const char g_ReplayWriterString_49569C[];
extern const char g_ReplayWriterString_4956AC[];
extern const char g_ReplayWriterString_4956B8[];
extern const char g_ReplayWriterString_4956CC[];
extern const char g_ReplayWriterString_4956D8[];
extern const char g_ReplayWriterString_4956EC[];
extern void *g_ReplayWriterIat_48B088;
extern void *g_ReplayWriterIat_48B0A0;
extern void *g_ReplayWriterIat_48B0A8;
extern "C" unsigned int __security_cookie;
extern "C" void __fastcall __security_check_cookie(unsigned int value);
extern "C" int __cdecl ReplayMakeDirectory(const char *path);
extern "C" tm *__cdecl ReplayLocaltime64(__time64_t *value);

// 00436420..00436B5F: exact native 1856-byte writer body.
__declspec(naked) void __fastcall WritePauseMenuReplay(const char *filename,
    const char *name, int mode)
{
    __asm {
        sub esp, 11Ch
        mov eax, dword ptr [__security_cookie]
        xor eax, esp
        mov dword ptr [esp + 118h], eax
        push ebx
        push ebp
        mov ebp, dword ptr [g_SecondaryReplayCurrent]
        push esi
        push edi
        xor ebx, ebx
        mov edi, edx
        mov esi, ecx
        mov ecx, dword ptr [ebp + 18h]
        mov dword ptr [esp + 24h], ebx
        mov dword ptr [esp + 20h], ebx
        mov eax, edi
    loc_00436451:
        mov dl, byte ptr [eax]
        mov byte ptr [ecx], dl
        inc eax
        inc ecx
        test dl, dl
        jne loc_00436451
        mov eax, edi
        lea edx, [eax + 1]
    loc_00436460:
        mov cl, byte ptr [eax]
        inc eax
        test cl, cl
        jne loc_00436460
        sub eax, edx
        cmp eax, 8
        jge loc_0043647d
        mov edi, edi
    loc_00436470:
        mov ecx, dword ptr [ebp + 18h]
        mov byte ptr [eax + ecx], 20h
        inc eax
        cmp eax, 8
        jl loc_00436470
    loc_0043647d:
        test byte ptr [ebp + 1D8h], 1
        jne loc_004364f2
        cmp dword ptr [esp + 130h], ebx
        je loc_004364f2
        mov edx, dword ptr [ebp + 9Ch]
        mov eax, dword ptr [edx]
        mov ecx, dword ptr [eax + 1518h]
        mov edx, 0FFFFh
        mov word ptr [ecx], dx
        mov ecx, dword ptr [eax + 1518h]
        mov word ptr [ecx + 2], dx
        mov ecx, dword ptr [eax + 1518h]
        mov word ptr [ecx + 4], dx
        add dword ptr [eax + 1518h], 6
        mov ecx, dword ptr [eax + 1518h]
        sub ecx, eax
        mov eax, 2AAAAAABh
        imul ecx
        mov eax, edx
        shr eax, 1Fh
        add eax, edx
        cmp eax, 384h
        jl loc_004364f2
        mov edi, dword ptr [ebp + 1D4h]
        mov ebx, ebp
        call AppendReplayWriteSegment
        mov dword ptr [ebp + 9Ch], eax
        xor ebx, ebx
    loc_004364f2:
        push offset g_ReplayWriterString_4955A0
        call ReplayMakeDirectory
        push esi
        lea ecx, [esp + 30h]
        push offset g_ReplayWriterString_4955A8
        push ecx
        call sprintf
        add esp, 10h
        xor ecx, ecx
        lea eax, [ebp + 40h]
        mov dword ptr [esp + 1Ch], ebx
        mov esi, 70h
        mov dword ptr [esp + 10h], ecx
        lea ebx, [ebp + 1Ch]
        mov dword ptr [esp + 14h], eax
        jmp loc_00436530
        // Preserve the original six-byte alignment LEA, including disp32=0.
        _emit 08Dh
        _emit 09Bh
        _emit 00h
        _emit 00h
        _emit 00h
        _emit 00h
    loc_00436530:
        mov eax, dword ptr [ebx]
        test eax, eax
        je loc_0043660c
        cmp dword ptr [esp + 24h], 0
        jne loc_00436545
        mov dword ptr [esp + 24h], ecx
    loc_00436545:
        test byte ptr [ebp + 1D8h], 1
        mov dword ptr [esp + 20h], ecx
        jne loc_00436559
        mov dword ptr [eax + 8], 0
    loc_00436559:
        mov edx, dword ptr [esp + 14h]
        mov edi, dword ptr [edx]
        add esi, 90h
        test edi, edi
        je loc_00436608
        // Original alignment instruction is LEA ECX,[ECX+disp8 0].
        _emit 08Dh
        _emit 049h
        _emit 00h
    loc_00436570:
        mov ecx, dword ptr [edi]
        mov edx, dword ptr [ecx + 1518h]
        sub edx, ecx
        mov eax, 2AAAAAABh
        imul edx
        mov eax, edx
        shr eax, 1Fh
        add eax, edx
        mov edx, dword ptr [ecx + 18A0h]
        lea eax, [eax + eax*2]
        lea eax, [edx + eax*2]
        sub eax, ecx
        test byte ptr [ebp + 1D8h], 1
        lea edx, [esi + eax - 151Ch]
        mov dword ptr [esp + 18h], edx
        jne loc_004365f5
        mov edx, dword ptr [ecx + 1518h]
        mov esi, dword ptr [ebx]
        sub edx, ecx
        mov eax, 2AAAAAABh
        imul edx
        mov eax, edx
        shr eax, 1Fh
        add eax, edx
        mov edx, dword ptr [ecx + 18A0h]
        lea eax, [eax + eax*2]
        lea eax, [edx + eax*2]
        sub eax, ecx
        sub eax, 151Ch
        add dword ptr [esi + 8], eax
        mov eax, dword ptr [edi]
        mov ecx, dword ptr [eax + 1518h]
        mov esi, dword ptr [ebx]
        sub ecx, eax
        mov eax, 2AAAAAABh
        imul ecx
        mov ecx, edx
        shr ecx, 1Fh
        add ecx, edx
        add dword ptr [esi + 4], ecx
    loc_004365f5:
        mov edi, dword ptr [edi + 4]
        mov esi, dword ptr [esp + 18h]
        test edi, edi
        jne loc_00436570
        mov ecx, dword ptr [esp + 10h]
    loc_00436608:
        inc dword ptr [esp + 1Ch]
    loc_0043660c:
        add dword ptr [esp + 14h], 0Ch
        inc ecx
        add ebx, 4
        cmp ecx, 8
        mov dword ptr [esp + 10h], ecx
        jl loc_00436530
        mov edx, dword ptr [ebp + 18h]
        mov eax, dword ptr [esp + 1Ch]
        mov dword ptr [edx + 58h], eax
        mov ecx, dword ptr [ebp + 18h]
        mov edx, dword ptr [g_SecondaryActorScore]
        mov eax, dword ptr [g_PauseRecordTiming]
        mov dword ptr [ecx + 14h], edx
        fld qword ptr [eax + 24h]
        fdiv qword ptr [eax + 2Ch]
        mov eax, dword ptr [ebp + 18h]
        push esi
        fstp dword ptr [esp + 14h]
        fld dword ptr [esp + 14h]
        fld qword ptr [g_ReplayWriterPercentScale]
        fmul st(1), st(0)
        fsubrp st(1), st(0)
        fstp dword ptr [esp + 14h]
        fld dword ptr [esp + 14h]
        fstp dword ptr [eax + 54h]
        call malloc
        mov esi, dword ptr [ebp + 18h]
        mov edx, eax
        lea eax, [ebp + 40h]
        mov dword ptr [esp + 18h], eax
        mov ecx, 1Ch
        mov edi, edx
        lea eax, [ebp + 1Ch]
        add esp, 4
        mov dword ptr [esp + 1Ch], edx
        rep movsd
        mov ebx, 70h
        mov dword ptr [esp + 10h], eax
        mov dword ptr [esp + 18h], 8
        // Preserve the native seven-byte SIB/disp32 alignment LEA.
        _emit 08Dh
        _emit 0A4h
        _emit 024h
        _emit 00h
        _emit 00h
        _emit 00h
        _emit 00h
    loc_004366a0:
        mov esi, dword ptr [eax]
        test esi, esi
        je loc_0043675c
        lea edi, [edx + ebx]
        mov ecx, 24h
        rep movsd
        mov ecx, dword ptr [esp + 14h]
        mov edi, dword ptr [ecx]
        add ebx, 90h
        test edi, edi
        je loc_00436715
    loc_004366c4:
        mov esi, dword ptr [edi]
        mov ecx, dword ptr [esi + 1518h]
        sub ecx, esi
        mov eax, 2AAAAAABh
        imul ecx
        mov eax, edx
        shr eax, 1Fh
        add eax, edx
        lea edx, [eax + eax*2]
        mov eax, dword ptr [esp + 1Ch]
        add edx, edx
        push edx
        add eax, ebx
        push esi
        push eax
        call ReplayMemcpy
        mov ecx, dword ptr [esi + 1518h]
        mov edi, dword ptr [edi + 4]
        sub ecx, esi
        mov eax, 2AAAAAABh
        imul ecx
        mov eax, edx
        shr eax, 1Fh
        add eax, edx
        add esp, 0Ch
        lea ecx, [eax + eax*2]
        lea ebx, [ebx + ecx*2]
        test edi, edi
        jne loc_004366c4
    loc_00436715:
        mov edx, dword ptr [esp + 14h]
        mov edi, dword ptr [edx]
        test edi, edi
        je loc_0043675c
        nop
    loc_00436720:
        mov esi, dword ptr [edi]
        mov eax, dword ptr [esi + 18A0h]
        mov edx, dword ptr [esp + 1Ch]
        sub eax, esi
        sub eax, 151Ch
        push eax
        lea ecx, [esi + 151Ch]
        push ecx
        add edx, ebx
        push edx
        call ReplayMemcpy
        mov eax, dword ptr [esi + 18A0h]
        mov edi, dword ptr [edi + 4]
        sub eax, esi
        add esp, 0Ch
        lea ebx, [ebx + eax - 151Ch]
        test edi, edi
        jne loc_00436720
    loc_0043675c:
        mov eax, dword ptr [esp + 10h]
        add dword ptr [esp + 14h], 0Ch
        mov edx, dword ptr [esp + 1Ch]
        add eax, 4
        sub dword ptr [esp + 18h], 1
        mov dword ptr [esp + 10h], eax
        jne loc_004366a0
        lea ecx, [esp + 18h]
        push ecx
        push ebx
        push edx
        call CompressReplayData
        mov esi, eax
        mov eax, dword ptr [esp + 1Ch]
        push eax
        mov dword ptr [esp + 14h], esi
        call free
        mov edi, dword ptr [esp + 1Ch]
        add esp, 4
        push edi
        push 40h
        push 7Ah
        push edi
        push esi
        mov al, 3Dh
        call GatherXorBlocks
        push edi
        push 800h
        push 0E1h
        push edi
        push esi
        mov al, 0AAh
        call GatherXorBlocks
        mov ecx, dword ptr [ebp + 14h]
        mov dword ptr [ecx + 20h], ebx
        mov edx, dword ptr [ebp + 14h]
        mov dword ptr [edx + 1Ch], edi
        mov eax, dword ptr [ebp + 14h]
        mov ecx, dword ptr [eax + 1Ch]
        add ecx, 24h
        lea esi, [esp + 28h]
        mov dword ptr [eax + 0Ch], ecx
        call OpenSharedFileForWrite
        mov eax, dword ptr [g_SharedFile]
        cmp eax, -1
        je loc_0043687f
        mov ecx, dword ptr [ebp + 14h]
        mov esi, dword ptr [g_ReplayWriterIat_48B0A8]
        push 0
        lea edx, [esp + 1Ch]
        push edx
        push 24h
        push ecx
        push eax
        call esi
        cmp dword ptr [esp + 18h], 24h
        je loc_00436836
        mov edx, dword ptr [g_SharedFile]
        push edx
        call dword ptr [g_ReplayWriterIat_48B0A0]
        test dword ptr [g_InputRuntimeFlags], 8000h
        je loc_00436836
        push offset g_FileCriticalSection
        call dword ptr [g_ReplayWriterIat_48B088]
        dec byte ptr [g_FileCriticalDepth]
    loc_00436836:
        mov eax, dword ptr [g_SharedFile]
        cmp eax, -1
        je loc_0043687f
        mov edx, dword ptr [esp + 10h]
        push 0
        lea ecx, [esp + 18h]
        push ecx
        push edi
        push edx
        push eax
        call esi
        cmp edi, dword ptr [esp + 14h]
        je loc_0043687f
        mov eax, dword ptr [g_SharedFile]
        push eax
        call dword ptr [g_ReplayWriterIat_48B0A0]
        test dword ptr [g_InputRuntimeFlags], 8000h
        je loc_0043687f
        push offset g_FileCriticalSection
        call dword ptr [g_ReplayWriterIat_48B088]
        dec byte ptr [g_FileCriticalDepth]
    loc_0043687f:
        mov eax, dword ptr [esp + 10h]
        test eax, eax
        je loc_00436890
        push eax
        call free
        add esp, 4
    loc_00436890:
        push 0FFFFh
        call malloc
        push 0FFFFh
        mov edi, eax
        push 0
        push edi
        call ReplayMemset
        lea ebx, [edi + 0Ch]
        mov esi, ebx
        push offset g_ReplayWriterString_4955F4
        push esi
        mov dword ptr [edi], 52455355h
        mov byte ptr [edi + 8], 0
        call sprintf
        push offset g_ReplayWriterString_495618
        add esi, eax
        push offset g_ReplayWriterString_495620
        push esi
        call sprintf
        mov ecx, dword ptr [ebp + 18h]
        push ecx
        add esi, eax
        push offset g_ReplayWriterString_495630
        push esi
        call sprintf
        add esi, eax
        mov eax, dword ptr [ebp + 18h]
        add eax, 0Ch
        push eax
        call ReplayLocaltime64
        mov edx, dword ptr [eax + 4]
        mov ecx, dword ptr [eax + 8]
        push edx
        mov edx, dword ptr [eax + 0Ch]
        push ecx
        mov ecx, dword ptr [eax + 10h]
        mov eax, dword ptr [eax + 14h]
        push edx
        inc ecx
        push ecx
        cdq
        mov ecx, 64h
        idiv ecx
        push edx
        push offset g_ReplayWriterString_49563C
        push esi
        call sprintf
        add esi, eax
        mov eax, dword ptr [ebp + 18h]
        mov ecx, dword ptr [eax + 5Ch]
        lea edx, [ecx + ecx*2]
        add edx, dword ptr [eax + 60h]
        add esp, 50h
        mov eax, dword ptr [edx*4 + g_ReplayCharacterNames]
        push eax
        push offset g_ReplayWriterString_49565C
        push esi
        call sprintf
        mov ecx, dword ptr [ebp + 18h]
        mov edx, dword ptr [ecx + 64h]
        add esi, eax
        mov eax, dword ptr [edx*4 + g_ReplayDifficultyNames]
        push eax
        push offset g_ReplayWriterString_495668
        push esi
        call sprintf
        mov edx, dword ptr [ebp + 18h]
        mov ecx, 7
        add esp, 18h
        add esi, eax
        cmp dword ptr [edx + 68h], ecx
        jle loc_00436993
        cmp dword ptr [esp + 24h], ecx
        jne loc_00436983
        push offset g_ReplayWriterString_495674
        push esi
        call sprintf
        add esp, 8
        jmp loc_004369d4
    loc_00436983:
        push offset g_ReplayWriterString_495688
        push esi
        call sprintf
        add esp, 8
        jmp loc_004369d4
    loc_00436993:
        mov eax, dword ptr [esp + 24h]
        mov edx, dword ptr [esp + 20h]
        cmp eax, edx
        jne loc_004369c4
        cmp eax, ecx
        jne loc_004369b3
        push offset g_ReplayWriterString_49569C
        push esi
        call sprintf
        add esp, 8
        jmp loc_004369d4
    loc_004369b3:
        push eax
        push offset g_ReplayWriterString_4956AC
        push esi
        call sprintf
        add esp, 0Ch
        jmp loc_004369d4
    loc_004369c4:
        push edx
        push eax
        push offset g_ReplayWriterString_4956B8
        push esi
        call sprintf
        add esp, 10h
    loc_004369d4:
        add esi, eax
        mov eax, dword ptr [ebp + 18h]
        mov ecx, dword ptr [eax + 14h]
        push ecx
        push offset g_ReplayWriterString_4956CC
        push esi
        call sprintf
        mov edx, dword ptr [ebp + 18h]
        fld dword ptr [edx + 54h]
        add esp, 4
        fstp qword ptr [esp]
        add esi, eax
        push offset g_ReplayWriterString_4956D8
        push esi
        call sprintf
        lea esi, [esi + eax + 1]
        mov eax, esi
        sub eax, edi
        add esp, 10h
        and eax, 80000003h
        jns loc_00436a18
        dec eax
        or eax, 0FFFFFFFCh
        inc eax
    loc_00436a18:
        je loc_00436a23
        mov ecx, 4
        sub ecx, eax
        add esi, ecx
    loc_00436a23:
        mov eax, dword ptr [g_SharedFile]
        sub esi, edi
        mov dword ptr [edi + 4], esi
        cmp eax, -1
        je loc_00436a71
        push 0
        lea edx, [esp + 24h]
        push edx
        push esi
        push edi
        push eax
        call dword ptr [g_ReplayWriterIat_48B0A8]
        cmp esi, dword ptr [esp + 20h]
        je loc_00436a71
        mov eax, dword ptr [g_SharedFile]
        push eax
        call dword ptr [g_ReplayWriterIat_48B0A0]
        test dword ptr [g_InputRuntimeFlags], 8000h
        je loc_00436a71
        push offset g_FileCriticalSection
        call dword ptr [g_ReplayWriterIat_48B088]
        dec byte ptr [g_FileCriticalDepth]
    loc_00436a71:
        push 0FFFFh
        push 0
        push edi
        call ReplayMemset
        push offset g_ReplayWriterString_4956EC
        push ebx
        mov dword ptr [edi], 52455355h
        mov byte ptr [edi + 8], 1
        call sprintf
        lea esi, [ebx + eax + 1]
        mov eax, esi
        sub eax, edi
        add esp, 14h
        and eax, 80000003h
        jns loc_00436aaa
        dec eax
        or eax, 0FFFFFFFCh
        inc eax
    loc_00436aaa:
        je loc_00436ab5
        mov ecx, 4
        sub ecx, eax
        add esi, ecx
    loc_00436ab5:
        mov ebx, dword ptr [g_SharedFile]
        sub esi, edi
        mov dword ptr [edi + 4], esi
        cmp ebx, -1
        je loc_00436b0a
        push 0
        lea edx, [esp + 24h]
        push edx
        push esi
        push edi
        push ebx
        call dword ptr [g_ReplayWriterIat_48B0A8]
        cmp esi, dword ptr [esp + 20h]
        je loc_00436b04
        mov eax, dword ptr [g_SharedFile]
        push eax
        call dword ptr [g_ReplayWriterIat_48B0A0]
        test dword ptr [g_InputRuntimeFlags], 8000h
        je loc_00436b04
        push offset g_FileCriticalSection
        call dword ptr [g_ReplayWriterIat_48B088]
        dec byte ptr [g_FileCriticalDepth]
    loc_00436b04:
        mov ebx, dword ptr [g_SharedFile]
    loc_00436b0a:
        push edi
        call free
        add esp, 4
        cmp ebx, -1
        je loc_00436b3c
        push ebx
        call dword ptr [g_ReplayWriterIat_48B0A0]
        test dword ptr [g_InputRuntimeFlags], 8000h
        je loc_00436b3c
        push offset g_FileCriticalSection
        call dword ptr [g_ReplayWriterIat_48B088]
        dec byte ptr [g_FileCriticalDepth]
    loc_00436b3c:
        or dword ptr [ebp + 1D8h], 1
        mov ecx, dword ptr [esp + 128h]
        pop edi
        pop esi
        pop ebp
        pop ebx
        xor ecx, esp
        xor eax, eax
        call __security_check_cookie
        add esp, 11Ch
        ret 4
    }
}
