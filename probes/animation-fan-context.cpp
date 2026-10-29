// Non-runnable context with independent renderer/VM/storage/count/RNG inputs.
#include "AnimationFan.hpp"
extern "C" int __stdcall AnimationFanProbe(int kind, RenderBatchView *renderer,
    AnimationVmView *vm, const RenderVertex *vertices, unsigned int count,
    RandomStateView *random, const CameraVector *vector)
{
    switch (kind)
    {
    case 0: return DrawAnimationQuadVertices(renderer, vm, vertices);
    case 1: return DrawAnimationColoredStrip(renderer, vm, reinterpret_cast<const AnimationColorVertex *>(vertices), count);
    case 2: return DrawAnimationColoredFan(renderer, vm, reinterpret_cast<const AnimationColorVertex *>(vertices), count);
    case 3: return static_cast<int>(IsAnimationVmActive(vm));
    case 4: return static_cast<int>(AnimationVectorLength(vector));
    case 5: return InitializeFanAnimationVm(vm);
    case 6: return UpdateFanAnimationVm(vm);
    case 7: return DrawFanAnimationVm(vm);
    case 8: return static_cast<int>(RandomNext32(random));
    case 9: return DrawAnimationVmByMode(renderer, vm);
    }
    return 0;
}
