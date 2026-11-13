#include "SecondaryGameplay.hpp"
#include "SharedFile.hpp"
#include "ErrorLog.hpp"
#include "InputBackend.hpp"
#include "ReplayPayloadChecksum.hpp"
#include "ByteCipher.hpp"
#include <windows.h>
#include <string.h>

extern unsigned int g_InputRuntimeFlags;
extern CRITICAL_SECTION g_FileCriticalSection;
extern unsigned char g_FileCriticalDepth;
extern void *g_SecondaryStatisticsIat_WriteFile;
extern void *g_SecondaryStatisticsIat_CloseHandle;
extern void *g_SecondaryStatisticsIat_LeaveCriticalSection;
extern const char g_SecondaryStatisticsFilename[];
extern const char g_SecondaryStatisticsWriteError[];

extern "C" void *__cdecl SecondaryStatisticsMalloc(unsigned int size);
extern "C" void __cdecl SecondaryStatisticsFree(void *memory);
extern "C" void *__cdecl SecondaryStatisticsMemcpy(void *destination, const void *source, unsigned int size);
extern "C" unsigned int __cdecl SecondaryStatisticsSumPayloadBytes(int length);
#pragma comment(linker, "/alternatename:_SecondaryStatisticsMalloc=_malloc")
#pragma comment(linker, "/alternatename:_SecondaryStatisticsFree=_free")
#pragma comment(linker, "/alternatename:_SecondaryStatisticsMemcpy=_memcpy")
#pragma comment(linker, "/alternatename:_SecondaryStatisticsSumPayloadBytes=?SumPayloadBytes@ReplayPayloadChecksumView@@QAEIH@Z")

unsigned char *__stdcall CompressReplayData(const void *data, int size, int *packedSize);

// Complete 639-byte native score/statistics writer. Ghidra identifies 636
// body bytes; the owned 3-byte LEA at 437ACD is retained as layout. This owner
// serializes the current secondary gameplay statistics, checksum and seven
// record slots, compresses and gathers the payload, then writes scoreth11.dat.
__declspec(naked) int __cdecl SaveSecondaryGameplayStatisticsFile()
{
    __asm {
        sub esp, 010h
        push ebx
        mov ebx, dword ptr [g_SecondaryGameplayStatistics]
        cmp dword ptr [ebx], 0
        jne loc_00437A87
        or eax, 0FFFFFFFFh
        pop ebx
        add esp, 010h
        ret
    loc_00437A87:
        push ebp
        push esi
        push edi
        push 0200000h
        call SecondaryStatisticsMalloc
        mov ebp, eax
        mov eax, dword ptr [ebx]
        mov ecx, dword ptr [eax]
        mov dword ptr [ebp], ecx
        mov edx, dword ptr [eax + 4]
        mov dword ptr [ebp + 4], edx
        mov ecx, dword ptr [eax + 8]
        mov dword ptr [ebp + 8], ecx
        mov edx, dword ptr [eax + 0Ch]
        mov dword ptr [ebp + 0Ch], edx
        mov ecx, dword ptr [eax + 010h]
        mov dword ptr [ebp + 010h], ecx
        mov edx, dword ptr [eax + 014h]
        add esp, 4
        mov dword ptr [ebp + 014h], edx
        mov dword ptr [esp + 010h], 018h
        xor edi, edi
        lea esi, [ebx + 8]
        jmp loc_00437AD0
        _emit 08Dh
        _emit 049h
        _emit 000h
    loc_00437AD0:
        mov eax, 05243h
        cmp word ptr [esi], ax
        jne loc_00437B09
        push 068D4h
        mov ecx, esi
        mov dword ptr [esi + 0Ch], edi
        call SecondaryStatisticsSumPayloadBytes
        mov ecx, dword ptr [esp + 010h]
        push 068D4h
        add ecx, ebp
        push esi
        push ecx
        mov dword ptr [esi + 4], eax
        call SecondaryStatisticsMemcpy
        add esp, 0Ch
        add dword ptr [esp + 010h], 068D4h
    loc_00437B09:
        inc edi
        add esi, 068D4h
        cmp edi, 7
        jb loc_00437AD0
        xor edx, edx
        xor ecx, ecx
        mov dword ptr [esp + 018h], edx
        mov dword ptr [esp + 014h], edx
        lea eax, [ebx + 02DDDDh]
        mov dword ptr [esp + 01Ch], 0110h
        nop
    loc_00437B30:
        movzx esi, byte ptr [eax - 1]
        add ecx, esi
        movzx esi, byte ptr [eax]
        add edx, esi
        movzx esi, byte ptr [eax + 1]
        add dword ptr [esp + 014h], esi
        movzx esi, byte ptr [eax + 2]
        add dword ptr [esp + 018h], esi
        add eax, 4
        sub dword ptr [esp + 01Ch], 1
        jne loc_00437B30
        mov eax, dword ptr [esp + 014h]
        add edx, eax
        add edx, dword ptr [esp + 018h]
        lea esi, [ebx + 02DDD4h]
        add ecx, edx
        mov edx, dword ptr [esp + 010h]
        mov dword ptr [ebx + 02DDD8h], ecx
        mov ecx, dword ptr [esp + 010h]
        lea edi, [ecx + ebp]
        mov ecx, 0112h
        rep movsd
        mov eax, dword ptr [ebx]
        add edx, 0430h
        mov dword ptr [eax + 014h], edx
        mov eax, dword ptr [ebx]
        mov edx, dword ptr [eax + 014h]
        lea ecx, [eax + 010h]
        push ecx
        push edx
        lea eax, [ebp + 018h]
        push eax
        call CompressReplayData
        mov edi, eax
        mov eax, dword ptr [ebx]
        mov ecx, dword ptr [eax + 010h]
        add ecx, 018h
        mov dword ptr [eax + 4], ecx
        mov edx, dword ptr [ebx]
        mov eax, dword ptr [edx + 010h]
        push eax
        push 010h
        push 035h
        push eax
        push edi
        mov al, 0ACh
        mov dword ptr [esp + 030h], edi
        call GatherXorBlocks
        mov esi, offset g_SecondaryStatisticsFilename
        call OpenSharedFileForWrite
        test eax, eax
        je loc_00437C06
        push offset g_SecondaryStatisticsWriteError
        mov edi, offset g_RuntimeLog
        call AppendErrorLog
        mov eax, dword ptr [esp + 020h]
        add esp, 4
        test eax, eax
        je loc_00437BF2
        push eax
        call SecondaryStatisticsFree
        add esp, 4
    loc_00437BF2:
        push ebp
        call SecondaryStatisticsFree
        add esp, 4
        pop edi
        pop esi
        pop ebp
        or eax, 0FFFFFFFFh
        pop ebx
        add esp, 010h
        ret
    loc_00437C06:
        mov eax, dword ptr [g_SharedFile]
        cmp eax, -1
        je loc_00437C58
        mov edx, dword ptr [ebx]
        push 0
        lea ecx, [esp + 020h]
        push ecx
        push 018h
        push edx
        push eax
        call dword ptr [g_SecondaryStatisticsIat_WriteFile]
        cmp dword ptr [esp + 01Ch], 018h
        je loc_00437C53
        mov eax, dword ptr [g_SharedFile]
        push eax
        call dword ptr [g_SecondaryStatisticsIat_CloseHandle]
        test dword ptr [g_InputRuntimeFlags], 08000h
        je loc_00437C53
        push offset g_FileCriticalSection
        call dword ptr [g_SecondaryStatisticsIat_LeaveCriticalSection]
        dec byte ptr [g_FileCriticalDepth]
    loc_00437C53:
        mov eax, dword ptr [g_SharedFile]
    loc_00437C58:
        mov ecx, dword ptr [ebx]
        mov esi, dword ptr [ecx + 010h]
        cmp eax, -1
        je loc_00437CCF
        push 0
        lea edx, [esp + 01Ch]
        push edx
        push esi
        push edi
        push eax
        call dword ptr [g_SecondaryStatisticsIat_WriteFile]
        cmp esi, dword ptr [esp + 018h]
        je loc_00437CA1
        mov eax, dword ptr [g_SharedFile]
        push eax
        call dword ptr [g_SecondaryStatisticsIat_CloseHandle]
        test dword ptr [g_InputRuntimeFlags], 08000h
        je loc_00437CA1
        push offset g_FileCriticalSection
        call dword ptr [g_SecondaryStatisticsIat_LeaveCriticalSection]
        dec byte ptr [g_FileCriticalDepth]
    loc_00437CA1:
        mov eax, dword ptr [g_SharedFile]
        cmp eax, -1
        je loc_00437CCF
        push eax
        call dword ptr [g_SecondaryStatisticsIat_CloseHandle]
        test dword ptr [g_InputRuntimeFlags], 08000h
        je loc_00437CCF
        push offset g_FileCriticalSection
        call dword ptr [g_SecondaryStatisticsIat_LeaveCriticalSection]
        dec byte ptr [g_FileCriticalDepth]
    loc_00437CCF:
        test edi, edi
        je loc_00437CDC
        push edi
        call SecondaryStatisticsFree
        add esp, 4
    loc_00437CDC:
        push ebp
        call SecondaryStatisticsFree
        add esp, 4
        pop edi
        pop esi
        pop ebp
        xor eax, eax
        pop ebx
        add esp, 010h
        ret
    }
}
