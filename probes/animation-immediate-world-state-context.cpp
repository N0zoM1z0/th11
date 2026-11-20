#include "RenderBatch.hpp"

extern "C" void __stdcall AnimationWorldStateProbe(RenderBatchView *renderer,
    AnimationVmView *vm)
{
    renderer->ApplyAnimationVmWorldState(vm);
}
