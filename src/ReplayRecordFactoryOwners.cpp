#include "TitleMenuRecord.hpp"
#include "SecondaryReplayState.hpp"

extern "C" void __cdecl TitleFactoryAllocate();
extern "C" void __cdecl TitleFactoryVectorConstructorIterator();
extern "C" void __cdecl TitleFactoryMemset();
extern "C" void __cdecl TitleFactoryInitializeSecondaryReplay();
extern "C" void __cdecl TitleFactoryDestroyTitleMenuRecord();
extern "C" void __cdecl TitleFactoryFreeHeapBlock();
extern "C" void __cdecl TitleFactoryLoadReplayStageData();
extern "C" void TitleFactoryRecordingExceptionHandler();
extern "C" void TitleFactoryPlaybackExceptionHandler();
extern "C" unsigned int TitleFactorySecurityCookie;
extern "C" void TitleFactoryStageConstructor();
extern "C" void TitleFactoryStageDestructor();
extern "C" void *TitleFactoryReplayRegistry;

// Native 00435E00 complete 199-byte SEH factory owner.
__declspec(naked) TitleMenuRecordView *__stdcall CreateReplayRecording(int mode)
{
    __asm {
        push -1
        push offset TitleFactoryRecordingExceptionHandler
        mov eax, dword ptr fs:[0]
        push eax
        push ecx
        push esi
        mov eax, dword ptr [TitleFactorySecurityCookie]
        xor eax, esp
        push eax
        lea eax, [esp + 0Ch]
        mov dword ptr fs:[0], eax
        push 02DCh
        call TitleFactoryAllocate
        mov esi, eax
        add esp, 4
        mov dword ptr [esp + 8], esi
        mov dword ptr [esp + 014h], 0
        test esi, esi
        je loc_00435E6D
        push offset TitleFactoryStageDestructor
        push offset TitleFactoryStageConstructor
        push 8
        push 024h
        lea eax, [esi + 0A4h]
        push eax
        call TitleFactoryVectorConstructorIterator
        push 02DCh
        push 0
        push esi
        call TitleFactoryMemset
        add esp, 0Ch
        jmp loc_00435E6F
    loc_00435E6D:
        xor esi, esi
    loc_00435E6F:
        mov eax, dword ptr [esp + 01Ch]
        push esi
        mov ecx, offset TitleFactoryReplayRegistry
        mov dword ptr [esp + 018h], 0FFFFFFFFh
        call TitleFactoryInitializeSecondaryReplay
        test eax, eax
        je loc_00435EB2
        test esi, esi
        je loc_00435E9D
        push esi
        call TitleFactoryDestroyTitleMenuRecord
        push esi
        call TitleFactoryFreeHeapBlock
        add esp, 4
    loc_00435E9D:
        xor eax, eax
        mov ecx, dword ptr [esp + 0Ch]
        mov dword ptr fs:[0], ecx
        pop ecx
        pop esi
        add esp, 010h
        ret 4
    loc_00435EB2:
        mov eax, esi
        mov ecx, dword ptr [esp + 0Ch]
        mov dword ptr fs:[0], ecx
        pop ecx
        pop esi
        add esp, 010h
        ret 4
    }
}

// Native 00435EF0 complete 198-byte SEH factory owner.
__declspec(naked) TitleMenuRecordView *__stdcall CreateReplayPlayback(const char *filename)
{
    __asm {
        push -1
        push offset TitleFactoryPlaybackExceptionHandler
        mov eax, dword ptr fs:[0]
        push eax
        push ecx
        push esi
        mov eax, dword ptr [TitleFactorySecurityCookie]
        xor eax, esp
        push eax
        lea eax, [esp + 0Ch]
        mov dword ptr fs:[0], eax
        push 02DCh
        call TitleFactoryAllocate
        mov esi, eax
        add esp, 4
        mov dword ptr [esp + 8], esi
        mov dword ptr [esp + 014h], 0
        test esi, esi
        je loc_00435F5D
        push offset TitleFactoryStageDestructor
        push offset TitleFactoryStageConstructor
        push 8
        push 024h
        lea eax, [esi + 0A4h]
        push eax
        call TitleFactoryVectorConstructorIterator
        push 02DCh
        push 0
        push esi
        call TitleFactoryMemset
        add esp, 0Ch
        jmp loc_00435F5F
    loc_00435F5D:
        xor esi, esi
    loc_00435F5F:
        mov ecx, dword ptr [esp + 01Ch]
        push ecx
        mov dword ptr [esp + 018h], 0FFFFFFFFh
        push esi
        mov dword ptr [esi + 010h], 2
        call TitleFactoryLoadReplayStageData
        test eax, eax
        je loc_00435FA1
        push esi
        call TitleFactoryDestroyTitleMenuRecord
        push esi
        call TitleFactoryFreeHeapBlock
        add esp, 4
        xor eax, eax
        mov ecx, dword ptr [esp + 0Ch]
        mov dword ptr fs:[0], ecx
        pop ecx
        pop esi
        add esp, 010h
        ret 4
    loc_00435FA1:
        mov eax, esi
        mov ecx, dword ptr [esp + 0Ch]
        mov dword ptr fs:[0], ecx
        pop ecx
        pop esi
        add esp, 010h
        ret 4
    }
}

