// Non-runnable link context with independent renderer, VM, corners and flags.
#include "AnimationVmDraw.hpp"

extern "C" int __stdcall AnimationVmDrawProbe(int kind, RenderBatchView *renderer,
    AnimationVmView *vm, CameraVector *first, CameraVector *second,
    CameraVector *third, CameraVector *fourth, unsigned int flags,
    const RenderVertex *quad)
{
    switch (kind)
    {
    case 0: return DrawAnimationVm(renderer, vm, static_cast<unsigned char>(flags));
    case 1: DrawUnrotatedAnimationVm(renderer, vm); break;
    case 2: BuildUnroundedAnimationVmQuad(vm, first, second, third, fourth); break;
    case 3: DrawUnroundedAnimationVm(renderer, vm); break;
    case 4: DrawAdaptiveAnimationVm(renderer, vm); break;
    case 5: BuildAnimationVmQuad(vm, first, second, third, fourth); break;
    case 6: ApplyAnimationVmDrawState(renderer, vm); break;
    case 7: return AppendRenderQuad(renderer, quad);
    case 8: FlushRenderBatch(renderer); break;
    }
    return 0;
}

extern "C" __declspec(dllexport) float __stdcall VmDrawFloorProbe(float value)
{
    return FloorAnimationCoordinate(value);
}

extern "C" __declspec(dllexport) unsigned int __stdcall VmDrawColorProbe(
    unsigned char first, unsigned char second, unsigned char third)
{
    unsigned int result = ScaleAnimationColorChannel(first, second);
    return result ^ ScaleAnimationColorChannel(second, third);
}
