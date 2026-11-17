#include "StageAuxiliary.hpp"
#include "StageMesh.hpp"
#include "MeshEffects.hpp"
#include "SecondaryPlayerController.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryDamage.hpp"

// Original source identity is open; these are the already maintained global
// pointer types for the secondary player and stage-effect event owner.
extern SecondaryPlayerControllerView *g_SecondaryPlayerControllerInstance;
struct MeshEffectEventView;
extern MeshEffectEventView *g_MeshEffectEvent;

void __stdcall EndSecondarySpecialState(int value);
void __stdcall ResetSecondaryStageEffect(int type);
void __stdcall ResetSecondaryPlayerEmitters(SecondaryDamageReferencesView *self);
int __stdcall UpdateStageAuxiliaryMode5Effect(StageAuxiliaryView *self);

struct StageAuxiliaryEventCounterView
{
    unsigned char unknown000[0x14];
    int count;
};

typedef char StageAuxiliaryPlayerModeOffset[
    (offsetof(SecondaryPlayerControllerView, mode) == 0x928) ? 1 : -1];
typedef char StageAuxiliaryPlayerFlagsOffset[
    (offsetof(SecondaryPlayerControllerView, flags) == 0x8bc4) ? 1 : -1];

namespace
{
// No clamp: the native OR-and-shift sequence propagates non-byte bits from
// malformed frame values. Signed division is truncation toward zero.
static __forceinline unsigned int RepeatGrayWithOpaqueAlpha(int shade)
{
    const unsigned int value = static_cast<unsigned int>(shade);
    return (((value | 0xffffff00u) << 8 | value) << 8) | value;
}
}

// The complete 636-byte owner at 0x407720 performs the spell's screen tint,
// VM/player synchronization, finite-duration damage region and final cleanup.
int __stdcall UpdateSecondarySpecialMode5(StageAuxiliaryView *self)
{
    AnimationVmView *vm = FindAnimationVmById(
        g_RenderBatch, AnimationVmId(self->animationId040));
    if (!vm)
        self->animationId040 = 0;

    if (self->timer028.current > 1)
    {
        int numerator = static_cast<int>(
            static_cast<unsigned int>(self->timer028.current) * 96u);
        g_StagePrimary->tint = RepeatGrayWithOpaqueAlpha(numerator / 30 + 32);
    }
    else if (self->timer014.current >= 890)
    {
        int numerator = static_cast<int>(
            (static_cast<unsigned int>(self->timer014.current) * 3u -
             2670u) * 32u);
        g_StagePrimary->tint = RepeatGrayWithOpaqueAlpha(numerator / 30 + 32);
    }
    else if (self->timer014.current < 30)
    {
        int numerator = static_cast<int>(
            static_cast<unsigned int>(self->timer014.current) * 96u);
        g_StagePrimary->tint = RepeatGrayWithOpaqueAlpha(128 - numerator / 30);
    }
    else
        g_StagePrimary->tint = 0xff202020;

    SecondaryPlayerControllerView *player = g_SecondaryPlayerControllerInstance;
    if (!vm)
    {
        player->flags &= ~4u;
        QueueSecondaryActorSound(0x33, player->position.x);
        ResetSecondaryStageEffect(10);
        ResetSecondaryPlayerEmitters(
            reinterpret_cast<SecondaryDamageReferencesView *>(player));
        return -1;
    }

    self->origin = player->position;
    PositionSecondaryAnimationVm(vm, &player->position);

    if (!(player->flags & 4) || player->mode == 4 || player->mode == 2)
    {
        UpdateMeshEffectSpellState();
        ++reinterpret_cast<StageAuxiliaryEventCounterView *>(g_MeshEffectEvent)->count;
        player->mode = 1;
        if (self->timer028.current == 0)
        {
            self->timer028.SetFrame(0);
            self->timer028.fraction = 1.0f;
            self->timer028.current = 1;
            QueueMeshEffectSound(0x26);
            AllocateSecondaryDamageRegion(
                reinterpret_cast<SecondaryDamageReferencesView *>(player),
                &self->origin, 32.0f, 20.0f, 30, 75);
        }
    }

    if (self->timer028.current > 0)
    {
        player->flags &= ~4u;
        SignalSecondaryAnimation(&self->animationId040);
        UpdateStageAuxiliaryMode5Effect(self);
        self->radius = static_cast<float>(self->radiusStep + self->radius);
        AdvanceStageTimer(&self->timer028);
        EndSecondarySpecialState(40);
        if (self->timer028.current > 30)
        {
            g_SecondaryPlayerControllerInstance->flags &= ~4u;
            ReleaseSecondaryAnimation(AnimationVmId(self->animationId040));
            self->animationId040 = 0;
            return -1;
        }
    }
    return 0;
}
