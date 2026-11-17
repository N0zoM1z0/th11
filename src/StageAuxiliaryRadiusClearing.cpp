#include "StageAuxiliary.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryPlayerController.hpp"

// The original binary retains two distinct 84-byte owners, one per mode.
// Keep them separate rather than synthesizing a single shared executable
// symbol: both inspect the spell flags anew after the first clearing call.
int __stdcall UpdateStageAuxiliaryMode2Effect(StageAuxiliaryView *self)
{
    unsigned int mask = (~SecondaryActorSpell()->flags) & 1u;
    ClearSceneProjectilesInRadius(&self->origin, self->radius,
        static_cast<int>(mask), 1);
    // The first callee may change the spell. Do not reuse its flag snapshot.
    unsigned int nextMask = (~SecondaryActorSpell()->flags) & 1u;
    ClearSecondaryObjectsInRadius(&self->origin, self->radius,
        static_cast<int>(nextMask), 1);
    return 0;
}

int __stdcall UpdateStageAuxiliaryMode5Effect(StageAuxiliaryView *self)
{
    unsigned int mask = (~SecondaryActorSpell()->flags) & 1u;
    ClearSceneProjectilesInRadius(&self->origin, self->radius,
        static_cast<int>(mask), 1);
    unsigned int nextMask = (~SecondaryActorSpell()->flags) & 1u;
    ClearSecondaryObjectsInRadius(&self->origin, self->radius,
        static_cast<int>(nextMask), 1);
    return 0;
}
