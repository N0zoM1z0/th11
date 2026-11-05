#include "SecondaryPlayerMovement.hpp"

extern "C" int __stdcall SecondaryPlayerMovementProbe(SecondaryDamageReferencesView *self,
    unsigned int id, short event, RenderBatchView *renderer)
{
    int result = UpdateSecondaryPlayerMovement(self);
    SignalSecondaryAnimationEvent(AnimationVmId(id), event);
    return result + (FindAnimationVmById(renderer, AnimationVmId(id)) != 0);
}
