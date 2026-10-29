// Non-runnable link context; independent owners prevent global specialization.
#include "AnimationQuad.hpp"

extern "C" int __stdcall AnimationQuadProbe(int kind, RenderBatchView *renderer,
    AnimationVmView *vm, CameraVector *first, CameraVector *second,
    CameraVector *third, CameraVector *fourth, const RenderVertex *quad,
    unsigned int value, unsigned int scale)
{
    switch (kind)
    {
    case 0: BuildAnimationVmQuad(vm, first, second, third, fourth); break;
    case 1: ApplyAnimationVmDrawState(renderer, vm); break;
    case 2: return AppendRenderQuad(renderer, quad);
    case 3: return ScaleAnimationColorChannel(value, scale);
    case 4: FlushRenderBatch(renderer); break;
    }
    return 0;
}

extern "C" __declspec(dllexport) float __stdcall AnimationQuadFloorProbe(float value)
{
    return FloorAnimationCoordinate(value);
}

extern "C" __declspec(dllexport) unsigned int __stdcall AnimationQuadColorProbe(
    unsigned int first, unsigned int second, unsigned int third)
{
    unsigned int result = ScaleAnimationColorChannel(first, second);
    return result ^ ScaleAnimationColorChannel(second, third);
}

extern "C" __declspec(dllexport) unsigned int __stdcall AnimationQuadStateProbe(
    RenderBatchView *renderer, AnimationVmView *vm)
{
    ApplyAnimationVmDrawState(renderer, vm);
    return vm->flags.value;
}
