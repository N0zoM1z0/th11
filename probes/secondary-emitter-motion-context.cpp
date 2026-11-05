#include "SecondaryEmitterMotion.hpp"
#include "AnimationImmediateDraw.hpp"

extern "C" int __stdcall SecondaryEmitterMotionProbe(SecondaryDamageReferencesView *self,
    RenderBatchView *renderer, AnimationVmView *vm)
{
    ResetSecondaryPlayerEmitters(self);
    int result = DrawSecondaryPlayerEmitters(self);
    DrawAnimationVmByMode(renderer, vm);
    return result;
}
