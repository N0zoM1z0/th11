#include "SecondaryEmitterAnimation.hpp"
#include "AnimationVmFactory.hpp"
#include "RenderBatch.hpp"
#include "MainRuntime.hpp"

// 00455DF0. Create a draw-list VM on a layer under the VM lock.
AnimationVmId __stdcall CreateSecondaryDrawAnimationVmId(AnimationFileView *file, int script, int layer)
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[9]);
        ++g_MainRuntime.criticalDepth[9];
    }
    AnimationVmView *vm = AllocateSceneAnimationVm();
    vm->flags.value |= 0x40000000;
    vm->persistent020 = layer;
    InitializeSceneAnimationVm(vm, file, script);
    AnimationVmId id = PublishSecondaryDrawAnimationVm(vm);
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[9]);
        --g_MainRuntime.criticalDepth[9];
    }
    return id;
}
