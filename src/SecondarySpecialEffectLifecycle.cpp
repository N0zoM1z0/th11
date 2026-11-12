#include "MeshEffects.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryPlayerController.hpp"
#include "AnimationVmFactory.hpp"
#include "StageMesh.hpp"

struct MeshEffectActorView;
extern MeshEffectActorView *g_MeshEffectActor;
int __stdcall DrawSecondaryEffect(MeshEffectControllerView *self);

// Two separately observed complete source-open helpers. Their original
// names, classes and exact register allocation are not established.
void __stdcall EndSecondarySpecialState(int code);  // 00406860
void __cdecl QueueSecondarySpecialPulse();          // 0044A300
SecondaryDamageRegionView *__stdcall AllocateSecondaryDamageRegion(
    SecondaryDamageReferencesView *, const CameraVector *, float, float, int, int);

namespace
{
__forceinline SecondaryDamageReferencesView *Player()
{
    return reinterpret_cast<SecondaryDamageReferencesView *>(g_MeshEffectActor);
}
__forceinline AnimationVmView *PlayerAnimation()
{
    return reinterpret_cast<AnimationVmView *>(
        reinterpret_cast<unsigned char *>(Player()) + 0x14);
}
__forceinline void ResetPlayerSpecialPosition()
{
    unsigned char *player = reinterpret_cast<unsigned char *>(Player());
    *reinterpret_cast<unsigned int *>(player + 0x914) = 0;
    *reinterpret_cast<unsigned int *>(player + 0x918) = 0;
}
__forceinline unsigned int &SecondEffectVm(MeshEffectControllerView *self)
{
    return *reinterpret_cast<unsigned int *>(
        reinterpret_cast<unsigned char *>(self) + 0x44);
}
}

// Native 0045DD70..0045DE2B: complete 188-byte effect initialization.
int __stdcall InitializeSecondaryEffect(MeshEffectActorView *owner)
{
    MeshEffectControllerView *self = reinterpret_cast<MeshEffectControllerView *>(owner);
    self->parameter490 = 32.0f;
    self->parameter494 = 10.0f;
    self->origin.x = 0.0f;
    self->origin.y = 224.0f;
    self->origin.z = 0.0f;
    QueueMeshEffectSound(0x31);
    self->vmId = CreateAnimationVmId(Player()->file, 0x23, 0x16);
    SecondEffectVm(self) = CreateAnimationVmId(Player()->file, 0x24, 0x16).value;
    UpdateMeshEffectSpellState();
    BindAnimationScript(Player()->file, 0x1a, PlayerAnimation());
    Player()->flags |= 2;
    ResetPlayerSpecialPosition();
    ++*reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(g_SecondaryMaster) + 0x14);
    return 0;
}

// Native 0045DE30..0045DEE0: 177-byte update and terminal cleanup.
int __stdcall UpdateSecondaryEffect(MeshEffectControllerView *self)
{
    if (!FindAnimationVmById(g_RenderBatch, self->vmId))
    {
        self->vmId.value = 0;
        BindAnimationScript(Player()->file, 0, PlayerAnimation());
        Player()->flags &= ~2u;
        ResetPlayerSpecialPosition();
        EndSecondarySpecialState(0x28);
        return -1;
    }
    int frame = *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(self) + 0x18);
    if (frame >= 180)
    {
        if (frame == 180)
        {
            QueueMeshEffectSound(0x26);
            QueueSecondarySpecialPulse();
            AllocateSecondaryDamageRegion(Player(), &self->origin, 32.0f, 10.0f, 30, 50);
        }
        DrawSecondaryEffect(self);
        self->parameter490 = self->parameter494 + self->parameter490;
    }
    return 0;
}

// Native 0045DF10..0045DF63: two actual radius-clearing dispatches.
int __stdcall DrawSecondaryEffect(MeshEffectControllerView *self)
{
    unsigned int effectMask = (~SecondaryActorSpell()->flags) & 1u;
    ClearSceneProjectilesInRadius(&self->origin, self->parameter490,
        static_cast<int>(effectMask), 1);
    // The native reloads the spell flags AFTER the first call: that callee
    // can mutate game state, so the two masks must not be shared.
    unsigned int secondMask = (~SecondaryActorSpell()->flags) & 1u;
    ClearSecondaryObjectsInRadius(&self->origin, self->parameter490,
        static_cast<int>(secondMask), 1);
    return 0;
}
