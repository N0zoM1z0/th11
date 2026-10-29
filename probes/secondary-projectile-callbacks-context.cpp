#include "SecondaryProjectileCallbacks.hpp"
#include "Random.hpp"

extern "C" int __stdcall SecondaryProjectileCallbacksProbe(SecondaryDamageReferencesView *self,
    SecondaryProjectileView *projectile, const CameraVector *position, float angle,
    unsigned int context, int frames, RenderBatchView *renderer, RandomStateView *random)
{
    g_SecondaryProjectileSpawnCallbacks[1] = InitializeSecondaryProjectileTarget;
    g_SecondaryProjectileSpawnCallbacks[2] = InitializeSecondaryProjectileAngle;
    g_SecondaryProjectileSpawnCallbacks[3] = InitializeSecondaryProjectileAcceleration;
    g_SecondaryProjectileUpdateCallbacks[2] = UpdateSecondaryProjectileGravity;
    g_SecondaryProjectileUpdateCallbacks[3] = PositionSecondaryProjectileFromEmitter;
    InitializeSecondaryProjectileTarget(self, projectile, context);
    InitializeSecondaryProjectileAngle(self, projectile, context);
    InitializeSecondaryProjectileAcceleration(self, projectile, context);
    UpdateSecondaryProjectileGravity(self, projectile);
    PositionSecondaryProjectileFromEmitter(self, projectile);
    UpdateSecondaryProjectileHomingLong(projectile);
    DetachSecondaryProjectileTargets(projectile->target);
    int result = SpawnSecondaryFixedProjectile(position, angle);
    result += SecondaryPatternTimerChanged(&self->patternTime);
    SetSecondaryPatternMultiplier(GetSecondaryPatternMultiplier() + 1);
    RewindSecondaryTimer(&self->patternTime, frames);
    return result + (FindAnimationVmById(renderer, AnimationVmId(projectile->animationId)) != 0) + RandomNext32(random);
}
