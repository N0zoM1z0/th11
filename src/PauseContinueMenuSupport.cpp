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
    // Preserve the native EAX/ECX schedule: HUD pointer, current score,
    // HUD mirror, then the signed high-score comparison and conditional store.
    __asm {
        mov eax, dword ptr [g_SecondaryActorHud]
        mov ecx, dword ptr [g_SecondaryHostSelectionState+4]
        mov dword ptr [eax+43e4h], ecx
        mov eax, ecx
        cmp dword ptr [g_SecondaryHostSelectionState], eax
        jge score_done
        mov dword ptr [g_SecondaryHostSelectionState], eax
    score_done:
    }
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

AnimationVmId __stdcall CreateDefaultSecondaryDrawAnimationVmId(AnimationFileView *file, int script)
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[9]);
        ++g_MainRuntime.criticalDepth[9];
    }
    AnimationVmView *vm = AllocateSceneAnimationVm();
    vm->flags.value |= 0x40000000;
    InitializeSceneAnimationVm(vm, file, script);
    AnimationVmId id = PublishSecondaryDrawAnimationVm(vm);
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[9]);
        --g_MainRuntime.criticalDepth[9];
    }
    return id;
}
