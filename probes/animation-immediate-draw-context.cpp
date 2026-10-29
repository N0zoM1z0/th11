// Non-runnable complete-owner context with independent renderer/VM/buffer inputs.
#include "AnimationImmediateDraw.hpp"

extern "C" int __stdcall AnimationImmediateDrawProbe(int kind, RenderBatchView *renderer,
    AnimationVmView *vm, RenderVertex *vertices, int count, float value, unsigned int flags)
{
    switch (kind)
    {
    case 0: ApplyAnimationVmWorldState(renderer, vm); break;
    case 1: return DrawWorldAnimationVm(renderer, vm);
    case 2: return DrawFoggedProjectedAnimationVm(renderer, vm);
    case 3: return DrawAnimationVertexStrip(renderer, vm, vertices, static_cast<unsigned int>(count));
    case 4: return DrawAnimationVertexFan(renderer, vm, vertices, static_cast<unsigned int>(count));
    case 5: return UpdateStripVertexUv(vm, vertices, count);
    case 6: return UpdateTransposedStripVertexUv(vm, vertices, count);
    case 7: return DrawAnimationVmByMode(renderer, vm);
    case 8: return static_cast<int>(AnimationExtentMagnitude(value));
    case 9: return DrawAnimationVm(renderer, vm, static_cast<unsigned char>(flags));
    }
    return 0;
}
