#include "PauseContinueMenu.hpp"
#include "VmBindings.hpp"
#include "MainRuntime.hpp"

namespace
{
struct PauseScoreView { int high, current; };
}

void __cdecl CommitPauseMenuScore()
{
    PauseScoreView *score = reinterpret_cast<PauseScoreView *>(&g_SecondaryHostSelectionState);
    int value = score->current;
    *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(g_SecondaryActorHud) + 0x43e4) = value;
    if (score->high < value) score->high = value;
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
