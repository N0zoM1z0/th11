#include "SecondaryEmitterMotion.hpp"
#include "AnimationImmediateDraw.hpp"
#include "AnimationImmediateDrawViews.hpp"

extern "C" int __stdcall SecondaryEmitterMotionProbe(SecondaryDamageReferencesView *self,
    RenderBatchView *renderer, AnimationVmView *vm)
{
    ResetSecondaryPlayerEmitters(self);
    int result = DrawSecondaryPlayerEmitters(self);
    vm->DrawByMode(renderer);
    return result;
}
