#include "AnimationVmFactory.hpp"
#include "MainRuntime.hpp"
#include "Camera.hpp"

// 00455A70..00455B08: append a VM at the supplied world position.
// Unlike InitializeSceneAnimationVm, this path binds without clearing the
// position at 0x3E8, and has a caller-selected draw layer.
AnimationVmId __stdcall CreateDirectPositionAnimationVmId(AnimationFileView *file,
    const CameraVector *position, int script, int layer)
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[9]);
        ++g_MainRuntime.criticalDepth[9];
    }
    AnimationVmView *vm = AllocateSceneAnimationVm();
    vm->flags.value |= 0x40000000;
    vm->persistent020 = layer;
    CameraVector &vmPosition = *reinterpret_cast<CameraVector *>(&vm->persistent3e8);
    vmPosition = *position;
    BindAnimationScript(file, script, vm);
    AnimationVmId id = PublishSceneAnimationVm(vm);
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[9]);
        --g_MainRuntime.criticalDepth[9];
    }
    return id;
}
