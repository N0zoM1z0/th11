#include "AnimationImmediateDraw.hpp"

extern "C" int __stdcall AnimationWorldDrawProbe(RenderBatchView *renderer,
    AnimationVmView *vm)
{
    return DrawAnimationVmByMode(vm, renderer);
}
