#include "SecondaryEmitterAnimation.hpp"
#include "RenderBatch.hpp"
#include "MainRuntime.hpp"

namespace
{
struct EmitterVmRegistryView
{
    unsigned char unknown000[0x7b562c];
    AnimationVmNode *first, *tail, *second, *secondTail;
    unsigned char unknown7b563c[0x7bd888 - 0x7b563c];
    unsigned int nextId;
};
}

// Native 0x4557E0 private ABI: EBX carries the VM node and EAX points at
// the caller's AnimationVmId result slot. The two native factory callers
// share this register-passed append routine; it has no stack arguments.
__declspec(naked) AnimationVmId __stdcall PublishSecondaryDrawAnimationVmNative()
{
    __asm {
        mov ecx, dword ptr [g_RenderBatch]
        lea edx, [ebx+4]
        mov dword ptr [edx], ebx
        mov dword ptr [edx+4], 0
        mov dword ptr [edx+8], 0
        cmp dword ptr [ecx+7b5634h], 0
        jne has_second_tail
        mov dword ptr [ecx+7b5634h], edx
        jmp update_second_tail
    has_second_tail:
        push esi
        mov esi, dword ptr [ecx+7b5638h]
        push edi
        mov edi, dword ptr [esi+4]
        test edi, edi
        je no_second_next
        mov dword ptr [edx+4], edi
        mov edi, dword ptr [esi+4]
        mov dword ptr [edi+8], edx
    no_second_next:
        mov dword ptr [esi+4], edx
        pop edi
        mov dword ptr [edx+8], esi
        pop esi
    update_second_tail:
        mov dword ptr [ecx+7b5638h], edx
        mov edx, 1
        add dword ptr [ecx+7bd888h], edx
        cmp dword ptr [ecx+7bd888h], 0
        jne no_id_wrap
        add dword ptr [ecx+7bd888h], edx
    no_id_wrap:
        mov edx, dword ptr [ecx+7bd888h]
        mov dword ptr [ebx], edx
        mov ecx, dword ptr [ecx+7bd888h]
        mov dword ptr [eax], ecx
        ret
    }
}

__declspec(naked) AnimationVmId __stdcall CreateSecondaryDrawAnimationVmId(
    AnimationFileView *file, int script, int layer)
{
    __asm {
        test dword ptr [g_MainRuntime+590h], 8000h
        je no_draw_lock
        push offset g_MainRuntime+8e8h
        call dword ptr [EnterCriticalSection]
        inc byte ptr [g_MainRuntime+939h]
    no_draw_lock:
        push ebx
        call AllocateSceneAnimationVm
        mov ecx, dword ptr [esp+8]
        mov edx, dword ptr [esp+0ch]
        mov ebx, eax
        mov eax, dword ptr [esp+10h]
        or dword ptr [ebx+404h], 40000000h
        mov dword ptr [ebx+20h], eax
        push ecx
        mov eax, ebx
        call InitializeSceneAnimationVm
        mov eax, esi
        call PublishSecondaryDrawAnimationVmNative
        test dword ptr [g_MainRuntime+590h], 8000h
        pop ebx
        je draw_done
        push offset g_MainRuntime+8e8h
        call dword ptr [LeaveCriticalSection]
        dec byte ptr [g_MainRuntime+939h]
        mov eax, esi
    draw_done:
        ret 0ch
    }
}
