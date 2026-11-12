#include "SecondaryEmitterAnimation.hpp"
#include "SecondaryActorInstructionAnimation.hpp"
#include "MainRuntime.hpp"
#include "Camera.hpp"

// 00455C30..00455CC6: position-preserving primary-list factory with layer 8.
AnimationVmId __stdcall CreatePositionedSecondaryEmitterAnimationVmId(
    AnimationFileView *file, const CameraVector *position, int script)
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[9]);
        ++g_MainRuntime.criticalDepth[9];
    }
    AnimationVmView *vm = AllocateSceneAnimationVm();
    vm->flags.value |= 0x40000000;
    vm->persistent020 = 8;
    CameraVector &vmPosition = *reinterpret_cast<CameraVector *>(&vm->persistent3e8);
    vmPosition = *position;
    BindAnimationScript(file, script, vm);
    AnimationVmId id = PublishSecondaryEmitterAnimationVm(vm);
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[9]);
        --g_MainRuntime.criticalDepth[9];
    }
    return id;
}

// 00455CD0..00455D76: world-to-animation coordinate offset before binding.
// The native performs x+32+192 and y+16 as separate double additions.
AnimationVmId __stdcall CreateOffsetSecondaryEmitterAnimationVmId(
    AnimationFileView *file, const CameraVector *position, int script)
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[9]);
        ++g_MainRuntime.criticalDepth[9];
    }
    AnimationVmView *vm = AllocateSceneAnimationVm();
    vm->flags.value |= 0x40000000;
    vm->persistent020 = 8;
    CameraVector &vmPosition = *reinterpret_cast<CameraVector *>(&vm->persistent3e8);
    vmPosition.x = static_cast<float>(position->x + 32.0 + 192.0);
    vmPosition.y = static_cast<float>(position->y + 16.0);
    vmPosition.z = position->z;
    BindAnimationScript(file, script, vm);
    AnimationVmId id = PublishSecondaryEmitterAnimationVm(vm);
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[9]);
        --g_MainRuntime.criticalDepth[9];
    }
    return id;
}
