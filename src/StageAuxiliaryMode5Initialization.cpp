#include "StageAuxiliary.hpp"
#include "SecondaryPlayerController.hpp"
#include "SecondaryDamage.hpp"

// The original private-class receiver at 0x407680 is the 0x4A0-byte
// auxiliary stage object. Preserve the existing game's external ABI name.
struct MeshEffectActorView;
extern SecondaryPlayerControllerView *g_SecondaryPlayerControllerInstance;

// Native 0x407680..0x40771E: initialize the fifth auxiliary special mode
// with the player position, a real positioned VM and a one-frame timer.
int __stdcall InitializeSecondarySpecialMode5(MeshEffectActorView *context)
{
    StageAuxiliaryView *self = reinterpret_cast<StageAuxiliaryView *>(context);
    SecondaryPlayerControllerView *player = g_SecondaryPlayerControllerInstance;
    SecondaryDamageReferencesView *fileOwner =
        reinterpret_cast<SecondaryDamageReferencesView *>(player);

    self->radius = 32.0f;
    self->radiusStep = 10.0f;
    self->origin = player->position;
    self->timer028.SetFrame(-1);
    self->animationId040 = fileOwner->file->CreatePositionedVmId(
        &self->origin, 0x3a, 0x16).value;
    // The original reloads global 4A8EB4 after VM creation, rather than
    // reusing the earlier player pointer across the factory call.
    g_SecondaryPlayerControllerInstance->flags |= 4u;
    return 0;
}
