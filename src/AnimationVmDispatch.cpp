#include "AnimationImmediateDrawViews.hpp"
#include "AnimationProjectedQuad.hpp"
#include "AnimationImmediateDraw.hpp"
#include "AnimationVmDraw.hpp"

int __declspec(noinline) __stdcall DrawAnimationVmByMode(AnimationVmView *vm, RenderBatchView *renderer)
{
    ImmediateVmView *self = reinterpret_cast<ImmediateVmView *>(vm);
    register unsigned int flags = self->flags;
    register unsigned int mode = (flags >> 22) & 15;
    register unsigned int alpha = self->color.bytes.alpha;
    if (!(flags & 1)) return -1;
    if (!(flags & 2)) return -1;
    if (!alpha) return -1;
    switch (mode)
    {
    case 0: return DrawUnrotatedAnimationVm(renderer, vm);
    case 1: return DrawAdaptiveAnimationVm(renderer, vm);
    case 4: return DrawBillboardAnimationVm(renderer, vm);
    case 5: return DrawProjectedAnimationVm(renderer, vm);
    case 6: return DrawFoggedBillboardAnimationVm(renderer, vm);
    case 7: return DrawFoggedProjectedAnimationVm(renderer, vm);
    case 8: return self->DrawWorld(renderer);
    case 9:
    case 12:
    case 13: return DrawAnimationVertexStrip(renderer, vm, self->vertices, self->stripPairs * 2u);
    case 11: return DrawAnimationVertexFan(renderer, vm, self->vertices, self->stripPairs * 2u);
    case 2: return DrawUnroundedAnimationVm(renderer, vm);
    case 3: return DrawAdaptiveUnroundedAnimationVm(renderer, vm);
    case 10: return 0;
    default: return 0;
    }
}
