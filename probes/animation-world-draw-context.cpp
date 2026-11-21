#include "AnimationImmediateDraw.hpp"
#include "AnimationImmediateDrawViews.hpp"

extern "C" int __stdcall AnimationWorldDrawProbe(RenderBatchView *renderer,
    AnimationVmView *vm)
{
    return reinterpret_cast<ImmediateVmView *>(vm)->DrawByMode(renderer);
}
