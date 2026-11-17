#include "StageAuxiliary.hpp"
#include "SecondaryPlayerController.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryDamage.hpp"
#include "MeshEffects.hpp"

// These source views describe the two already-owned singleton pointers:
// 0x4A8D64 is the auxiliary receiver; 0x4A8EB4 is the real player.
struct MeshEffectActorView;
extern SecondaryPlayerControllerView *g_SecondaryPlayerControllerInstance;
struct MeshEffectEventView;
extern MeshEffectEventView *g_MeshEffectEvent;

struct StageAuxiliaryEventCountView
{
    unsigned char unknown000[0x14];
    int count;
};

static __forceinline void PublishModeStartEvent()
{
    UpdateMeshEffectSpellState();
    ++reinterpret_cast<StageAuxiliaryEventCountView *>(g_MeshEffectEvent)->count;
}

// Full native 0x407A40..0x407ABC. Mode one plays sound 0x31 before
// allocating its positioned stage VM (script 0x21, layer 0x16).
int __stdcall InitializeSecondarySpecialMode1(MeshEffectActorView *context)
{
    StageAuxiliaryView *self = reinterpret_cast<StageAuxiliaryView *>(context);
    SecondaryPlayerControllerView *player = g_SecondaryPlayerControllerInstance;
    SecondaryDamageReferencesView *playerFile =
        reinterpret_cast<SecondaryDamageReferencesView *>(player);

    self->radius = 32.0f;
    self->radiusStep = 10.0f;
    self->origin = player->position;
    QueueMeshEffectSound(0x31);
    self->animationId040 = playerFile->file->CreatePositionedVmId(
        &self->origin, 0x21, 0x16).value;
    PublishModeStartEvent();
    return 0;
}

// Full native 0x407130..0x407196. Mode four has the same origin/radius
// setup but no sound; its positioned VM uses script 0x2F.
int __stdcall InitializeSecondarySpecialMode4(MeshEffectActorView *context)
{
    StageAuxiliaryView *self = reinterpret_cast<StageAuxiliaryView *>(context);
    SecondaryPlayerControllerView *player = g_SecondaryPlayerControllerInstance;
    SecondaryDamageReferencesView *playerFile =
        reinterpret_cast<SecondaryDamageReferencesView *>(player);

    self->radius = 32.0f;
    self->radiusStep = 10.0f;
    self->origin = player->position;
    self->animationId040 = playerFile->file->CreatePositionedVmId(
        &self->origin, 0x2f, 0x16).value;
    PublishModeStartEvent();
    return 0;
}
