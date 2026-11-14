#include "TitleMenuRecord.hpp"
#include "TitleMenuRecordDestructorView.hpp"

extern "C" void __cdecl TitleRecordFreeHeapBlock(void *memory);
extern "C" void __cdecl TitleRecordClearReplayStageSegments();
extern "C" void __cdecl TitleRecordUnregisterChainElement();
extern "C" void __cdecl TitleRecordVectorDestructorIterator();
extern "C" unsigned int TitleRecordSecurityCookie;
extern "C" void *TitleRecordIatEnterCriticalSection;
extern "C" unsigned int TitleRecordRuntimeFlags;
extern "C" void *TitleRecordChainManager;
extern "C" unsigned char TitleRecordCriticalSection;
extern "C" unsigned char TitleRecordCriticalDepth;
extern "C" void *TitleRecordIatLeaveCriticalSection;
extern "C" void *TitleRecordSingleton;
extern "C" void TitleRecordExceptionHandler();
extern "C" void TitleRecordStageLinkDestructor();

// Complete 429-byte native SEH destructor. The naked body preserves the
// original stack-passed record pointer, exception-list registration, all
// three guarded chain removals, eight stage cleanup calls and vector teardown.
#pragma comment(linker, "/alternatename:??1TitleMenuRecordView@@QAE@XZ=_TitleRecordDestructorPrivate@4")

extern "C" __declspec(naked) void __stdcall TitleRecordDestructorPrivate(TitleMenuRecordDestructorView *record)
{
    __asm {
        push -1
        push offset TitleRecordExceptionHandler
        mov eax, dword ptr fs:[0]
        push eax
        push ebx
        push ebp
        push esi
        push edi
        mov eax, dword ptr [TitleRecordSecurityCookie]
        xor eax, esp
        push eax
        lea eax, [esp + 014h]
        mov dword ptr fs:[0], eax
        mov ebx, dword ptr [esp + 024h]
        xor ebp, ebp
        mov dword ptr [esp + 01Ch], ebp
        mov eax, dword ptr [ebx + 014h]
        push eax
        call TitleRecordFreeHeapBlock
        add esp, 4
        xor esi, esi
        _emit 08Dh
        _emit 064h
        _emit 024h
        _emit 000h
    loc_00435C90:
        mov eax, esi
        mov ecx, ebx
        call TitleRecordClearReplayStageSegments
        inc esi
        cmp esi, 8
        jl loc_00435C90
        mov eax, dword ptr [ebx + 018h]
        push eax
        call TitleRecordFreeHeapBlock
        add esp, 4
        mov dword ptr [ebx + 018h], ebp
        lea esi, [ebx + 01Ch]
        mov edi, 8
    loc_00435CB6:
        mov eax, dword ptr [esi]
        push eax
        call TitleRecordFreeHeapBlock
        mov dword ptr [esi], ebp
        add esp, 4
        add esi, 4
        sub edi, 1
        jne loc_00435CB6
        mov esi, dword ptr [ebx + 8]
        cmp esi, ebp
        mov ebp, dword ptr [TitleRecordIatEnterCriticalSection]
        je loc_00435D1D
        test dword ptr [TitleRecordRuntimeFlags], 08000h
        mov edi, dword ptr [TitleRecordChainManager]
        je loc_00435CF7
        push offset TitleRecordCriticalSection
        call ebp
        inc byte ptr [TitleRecordCriticalDepth]
    loc_00435CF7:
        mov ecx, esi
        mov edx, edi
        call TitleRecordUnregisterChainElement
        test dword ptr [TitleRecordRuntimeFlags], 08000h
        je loc_00435D1D
        push offset TitleRecordCriticalSection
        call dword ptr [TitleRecordIatLeaveCriticalSection]
        dec byte ptr [TitleRecordCriticalDepth]
    loc_00435D1D:
        mov esi, dword ptr [ebx + 01D0h]
        test esi, esi
        je loc_00435D6C
        test dword ptr [TitleRecordRuntimeFlags], 08000h
        mov edi, dword ptr [TitleRecordChainManager]
        je loc_00435D46
        push offset TitleRecordCriticalSection
        call ebp
        inc byte ptr [TitleRecordCriticalDepth]
    loc_00435D46:
        mov ecx, esi
        mov edx, edi
        call TitleRecordUnregisterChainElement
        test dword ptr [TitleRecordRuntimeFlags], 08000h
        je loc_00435D6C
        push offset TitleRecordCriticalSection
        call dword ptr [TitleRecordIatLeaveCriticalSection]
        dec byte ptr [TitleRecordCriticalDepth]
    loc_00435D6C:
        mov esi, dword ptr [ebx + 0Ch]
        test esi, esi
        je loc_00435DB8
        test dword ptr [TitleRecordRuntimeFlags], 08000h
        mov edi, dword ptr [TitleRecordChainManager]
        je loc_00435D92
        push offset TitleRecordCriticalSection
        call ebp
        inc byte ptr [TitleRecordCriticalDepth]
    loc_00435D92:
        mov ecx, esi
        mov edx, edi
        call TitleRecordUnregisterChainElement
        test dword ptr [TitleRecordRuntimeFlags], 08000h
        je loc_00435DB8
        push offset TitleRecordCriticalSection
        call dword ptr [TitleRecordIatLeaveCriticalSection]
        dec byte ptr [TitleRecordCriticalDepth]
    loc_00435DB8:
        cmp ebx, dword ptr [TitleRecordSingleton]
        jne loc_00435DCA
        mov dword ptr [TitleRecordSingleton], 0
    loc_00435DCA:
        push offset TitleRecordStageLinkDestructor
        push 8
        push 024h
        add ebx, 0A4h
        push ebx
        mov dword ptr [esp + 02Ch], 0FFFFFFFFh
        call TitleRecordVectorDestructorIterator
        mov ecx, dword ptr [esp + 014h]
        mov dword ptr fs:[0], ecx
        pop ecx
        pop edi
        pop esi
        pop ebp
        pop ebx
        add esp, 0Ch
        ret 4
    }
}

// Public source-level cleanup entry used by menu and replay consumers.
void __stdcall DestroyTitleMenuRecord(TitleMenuRecordView *record)
{
    record->~TitleMenuRecordView();
}
