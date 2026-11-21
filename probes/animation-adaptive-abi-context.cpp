#include "AnimationVmDraw.hpp"

extern "C" int __stdcall AnimationAdaptiveAbiProbe(RenderBatchView *renderer,
    AnimationVmView *vm)
{
    return DrawAdaptiveAnimationVm(renderer, vm);
}
