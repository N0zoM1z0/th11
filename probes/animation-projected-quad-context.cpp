// Independent inputs preserve the reviewed complete owners in a non-runnable link.
#include "AnimationProjectedQuad.hpp"

extern "C" int __stdcall AnimationProjectedQuadProbe(int kind, RenderBatchView *renderer,
    AnimationVmView *vm, CameraVector *quad, float squaredDistance, unsigned int flags)
{
    switch (kind)
    {
    case 0: return BuildProjectedAnimationVmQuad(renderer, vm);
    case 1: return DrawProjectedAnimationVm(renderer, vm);
    case 2: return DrawBillboardAnimationVm(renderer, vm);
    case 3: return DrawFoggedBillboardAnimationVm(renderer, vm);
    case 4: DrawAdaptiveUnroundedAnimationVm(renderer, vm); break;
    case 5: BuildAnimationVmModeQuad(vm, quad); break;
    case 7: return DrawAnimationVm(renderer, vm, static_cast<unsigned char>(flags));
    case 6: return static_cast<int>(AnimationDistanceRoot(squaredDistance));
    }
    return 0;
}
