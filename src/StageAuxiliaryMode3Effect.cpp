#include "StageAuxiliary.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryPlayerController.hpp"

// Target 0x40B0E0: game-owned horizontal-band actor clearing.
// This callee has its own 300-byte source-open body.
int __stdcall ClearStageAuxiliaryHorizontalBand(
    const CameraVector *center, float halfWidth, int effects);

// Target 0x424F30: dispatch a rectangle to subscribed scene objects.
// The original stores three float components for both vector inputs.
int __stdcall DispatchStageAuxiliaryArea(
    const CameraVector *origin, const CameraVector *extent, int effects);

// Full native 0x406F90..0x407063. Each clearing call can change spell
// state, so the original reloads its effect mask three separate times.
int __stdcall UpdateStageAuxiliaryMode3Effect(StageAuxiliaryView *self)
{
    unsigned int effects = (~SecondaryActorSpell()->flags) & 1u;
    ClearSceneProjectilesInRadius(&self->origin, 48.0f,
        static_cast<int>(effects), 1);

    effects = (~SecondaryActorSpell()->flags) & 1u;
    ClearStageAuxiliaryHorizontalBand(&self->origin, 32.0f,
        static_cast<int>(effects));

    effects = (~SecondaryActorSpell()->flags) & 1u;
    ClearSecondaryObjectsInRadius(&self->origin, 48.0f,
        static_cast<int>(effects), 1);

    CameraVector dispatchOrigin = self->origin;
    dispatchOrigin.y = static_cast<float>(dispatchOrigin.y - 224.0);

    // Native stores 32 and 448 into the first two floats but does not
    // initialize the third input component in this owner. The downstream
    // dispatcher reads all three; preserve the original open value rather
    // than inventing a semantic Z dimension.
    CameraVector extent;
    extent.x = 32.0f;
    extent.y = 448.0f;
    const unsigned int dispatchEffects = (~SecondaryActorSpell()->flags) & 1u;
    DispatchStageAuxiliaryArea(&dispatchOrigin, &extent,
        static_cast<int>(dispatchEffects));
    return 0;
}
