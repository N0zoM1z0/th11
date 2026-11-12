#include "SecondaryEmitterAnimation.hpp"
#include "MainRuntime.hpp"
#include "Camera.hpp"

// 00455E60..00455EF6: copy XYZ and append to the secondary draw chain.
// This preserves the supplied position through direct BindAnimationScript.
AnimationVmId __stdcall CreatePositionedSecondaryDrawAnimationVmId(
    AnimationFileView *file, const CameraVector *position, int script)
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[9]);
        ++g_MainRuntime.criticalDepth[9];
    }
    AnimationVmView *vm = AllocateSceneAnimationVm();
    vm->flags.value |= 0x40000000;
    vm->persistent020 = 0;
    CameraVector &vmPosition = *reinterpret_cast<CameraVector *>(&vm->persistent3e8);
    vmPosition = *position;
    BindAnimationScript(file, script, vm);
    AnimationVmId id = PublishSecondaryDrawAnimationVm(vm);
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[9]);
        --g_MainRuntime.criticalDepth[9];
    }
    return id;
}

// 00455F00..00455FA6: translate X by double 32+192 and Y by double 16
// before direct binding; unlike the front-draw factories this APPENDS.
AnimationVmId __stdcall CreateOffsetSecondaryDrawAnimationVmId(
    AnimationFileView *file, const CameraVector *position, int script)
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[9]);
        ++g_MainRuntime.criticalDepth[9];
    }
    AnimationVmView *vm = AllocateSceneAnimationVm();
    vm->flags.value |= 0x40000000;
    vm->persistent020 = 0;
    CameraVector &vmPosition = *reinterpret_cast<CameraVector *>(&vm->persistent3e8);
    vmPosition.x = static_cast<float>(position->x + 32.0 + 192.0);
    vmPosition.y = static_cast<float>(position->y + 16.0);
    vmPosition.z = position->z;
    BindAnimationScript(file, script, vm);
    AnimationVmId id = PublishSecondaryDrawAnimationVm(vm);
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[9]);
        --g_MainRuntime.criticalDepth[9];
    }
    return id;
}
