#include "SecondaryPlayerController.hpp"
#include "AnimationVmFactory.hpp"
#include "MeshEffects.hpp"
#include "MainRuntime.hpp"

// Separate physical view of the nullable global at 0x004A8E88; original owner is open.
struct SecondaryCollisionSoundGateView
{
    unsigned char unknown000[0x60];
    unsigned int flags;
};
extern SecondaryCollisionSoundGateView *g_SecondaryCollisionSoundGate;
typedef char SecondaryCollisionSoundFlagsOffset[(offsetof(SecondaryCollisionSoundGateView, flags) == 0x60) ? 1 : -1];

namespace
{
__forceinline void SetCollisionTimer(StageTimerView *time, int frame)
{
    if (!(time->flags & 1))
    {
        time->fraction = 0.0f;
        time->current = 0;
        time->previous = -999999;
        time->scale = &g_AnimationTimeScale;
        time->flags |= 1;
    }
    time->current = frame;
    time->fraction = static_cast<float>(frame);
    time->previous = static_cast<int>(static_cast<unsigned int>(frame) - 1);
}

__forceinline AnimationVmId CreateCollisionAnimation(AnimationFileView *file)
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[9]);
        ++g_MainRuntime.criticalDepth[9];
    }
    AnimationVmView *vm = AllocateSceneAnimationVm();
    vm->flags.value |= 0x40000000;
    vm->persistent020 = 0;
    InitializeSceneAnimationVm(vm, file, 0x4d);
    AnimationVmId id = PublishSceneAnimationVm(vm);
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[9]);
        --g_MainRuntime.criticalDepth[9];
    }
    return id;
}
}

void __stdcall BeginSecondaryCollision(SecondaryCollisionReferencesView *self)
{
    SecondaryPlayerControllerView *owner = reinterpret_cast<SecondaryPlayerControllerView *>(self);
    owner->mode = 4;
    CameraVector position;
    position.x = static_cast<float>(owner->position.x + 32.0 + 192.0);
    position.y = static_cast<float>(owner->position.y + 16.0);
    position.z = owner->position.z;
    PositionSecondaryAnimationRaw(CreateAnimationVmId(g_SecondaryActorFiles->file, 0x4c, 0), &position);
    for (int remaining = 32; remaining; --remaining)
    {
        PositionSecondaryAnimationRaw(CreateCollisionAnimation(g_SecondaryActorFiles->file), &position);
    }
    owner->stateTime.SetFrame(-1);
    // The nullable sound gate is independent of player mode.
    if (g_SecondaryCollisionSoundGate && !(g_SecondaryCollisionSoundGate->flags & 0x200)) QueueMeshEffectSound(4);
    SetCollisionTimer(&owner->statusTime, 6);
    BindAnimationScript(reinterpret_cast<SecondaryDamageReferencesView *>(self)->file, 0, &owner->animation);
    SecondaryActorSpellView *spell = SecondaryActorSpell();
    if (spell->flags & 1)
    {
        if (spell->age >= 60)
        {
            spell->value8e4 = 0;
            spell->flags &= ~0x22u;
        }
        else if (SecondaryActorEnemy()->active &&
            static_cast<int>(static_cast<unsigned int>(g_MeshEffectSelection0) * 3 + g_MeshEffectSelection1) != 5)
            spell->flags |= 0x20;
    }
}
