#include "PauseContinueMenu.hpp"
#include "VmBindings.hpp"
#include "MainRuntime.hpp"

namespace
{
struct PauseScoreView { int high, current; };
struct PauseHudScoreView
{
    unsigned char unknown000[0x43e4];
    int score43e4;
};
typedef char PauseHudScoreOffset[(offsetof(PauseHudScoreView, score43e4) == 0x43e4) ? 1 : -1];
}

void __cdecl CommitPauseMenuScore()
{
    PauseScoreView *score = reinterpret_cast<PauseScoreView *>(&g_SecondaryHostSelectionState);
    int current = score->current;
    reinterpret_cast<PauseHudScoreView *>(g_SecondaryActorHud)->score43e4 = current;
    if (score->high < current)
        score->high = current;
}

void __stdcall ClosePauseContinueMenu(PauseMenuView *self)
{
    self->state = 19;
    self->time.SetFrame(-1);
    SignalSecondaryAnimation(&self->backdrop.value);
    SignalSecondaryAnimation(&self->root.value);
    g_AnimationTimeScale = self->savedTimeScale;
}

void __stdcall ShowPauseMenuAnimationTree(const AnimationVmId &id)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, id);
    if (vm)
    {
        vm->flags.value |= 2;
        BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
        if (!bound->marker018)
            for (BoundVmNodeView *node = bound->related; node; node = node->next)
                node->vm->flags |= 2;
    }
}

__declspec(naked) AnimationVmId __stdcall CreateDefaultSecondaryDrawAnimationVmId(
    AnimationFileView *file, int script)
{
    __asm {
        test dword ptr [g_MainRuntime+590h], 8000h
        je no_default_lock
        push offset g_MainRuntime+8e8h
        call dword ptr [EnterCriticalSection]
        inc byte ptr [g_MainRuntime+939h]
    no_default_lock:
        push ebx
        call AllocateSceneAnimationVm
        mov edx, dword ptr [esp+0ch]
        mov ebx, eax
        mov eax, dword ptr [esp+8]
        or dword ptr [ebx+404h], 40000000h
        push eax
        mov eax, ebx
        call InitializeSceneAnimationVm
        mov eax, esi
        call PublishSecondaryDrawAnimationVmNative
        test dword ptr [g_MainRuntime+590h], 8000h
        pop ebx
        je default_done
        push offset g_MainRuntime+8e8h
        call dword ptr [LeaveCriticalSection]
        dec byte ptr [g_MainRuntime+939h]
        mov eax, esi
    default_done:
        ret 8
    }
}
