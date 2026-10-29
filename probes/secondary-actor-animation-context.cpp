// Independent input; this diagnostic image is never executed.
#include "SecondaryActorRuntime.hpp"
extern "C" AnimationVmView *__stdcall SecondaryActorAnimationProbe(RenderBatchView *renderer, unsigned int *id,
    AnimationVmView *vm, const CameraVector *position)
{
    AnimationVmView *found = FindAnimationVmById(renderer, AnimationVmId(*id));
    SignalSecondaryAnimation(id);
    ReleaseSecondaryAnimation(*id);
    PositionSecondaryAnimation(*id, position);
    PositionSecondaryAnimationVm(vm, position);
    PositionSecondaryAnimationRaw(*id, position);
    return ValidateSecondaryAnimationId(id) ? ClearMissingAnimationId(id) : found;
}
