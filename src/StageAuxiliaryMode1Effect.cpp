#include "StageAuxiliary.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryPlayerController.hpp"

// Native 0x40B210 is an independently owned stage-actor region query.
// In the original LTCG path it uses EBX for the spell mask.
int __stdcall CullStageAuxiliaryActorsInRadius(
    const CameraVector *position, float radius, int mask);

// 0x407B50..0x407BA4: clear a 96-unit radius across the independent actor
// and secondary-object systems. The mask must be re-read between calls.
int __stdcall UpdateStageAuxiliaryMode1Effect(StageAuxiliaryView *self)
{
    int mask = static_cast<int>((~SecondaryActorSpell()->flags) & 1u);
    CullStageAuxiliaryActorsInRadius(&self->origin, 96.0f, mask);
    int nextMask = static_cast<int>(((~SecondaryActorSpell()->flags) & 1u) | 2u);
    ClearSecondaryObjectsInRadius(&self->origin, 96.0f, nextMask, 1);
    return 0;
}
