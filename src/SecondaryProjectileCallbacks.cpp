#include "SecondaryProjectileCallbacks.hpp"
#include "AnimationImmediateDraw.hpp"
#include "AngleMath.hpp"
#include "Random.hpp"
#include <math.h>
#pragma intrinsic(atan2, sqrt, fabs)

extern RandomStateView g_TextRandomState;

namespace
{
__forceinline SecondaryEmitterView *EmitterAt(SecondaryDamageReferencesView *self, int index)
{
    return reinterpret_cast<SecondaryEmitterView *>(reinterpret_cast<unsigned char *>(self) +
        0x75cc + static_cast<unsigned int>(index) * 0xe4);
}
struct SpawnAngleView
{
    unsigned char unknown000[0x3c4];
    float angle;
};
}

int __fastcall InitializeSecondaryProjectileTarget(SecondaryDamageReferencesView *,
    SecondaryProjectileView *projectile, unsigned int)
{
    projectile->target = 0;
    return 0;
}

int __fastcall InitializeSecondaryProjectileAngle(SecondaryDamageReferencesView *self,
    SecondaryProjectileView *projectile, unsigned int)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch,
        AnimationVmId(EmitterAt(self, projectile->parameters->index - 1)->id));
    projectile->motion.angle = NormalizeAngle(reinterpret_cast<SpawnAngleView *>(vm)->angle);
    return 0;
}

int __fastcall InitializeSecondaryProjectileAcceleration(SecondaryDamageReferencesView *,
    SecondaryProjectileView *projectile, unsigned int)
{
    float random = static_cast<float>(static_cast<float>(RandomNext32(&g_TextRandomState)) *
        4.656612873077392578125e-10 - 1.0);
    projectile->acceleration = static_cast<float>(random * static_cast<double>(0.002f));
    return 0;
}

int __fastcall UpdateSecondaryProjectileGravity(SecondaryDamageReferencesView *, SecondaryProjectileView *projectile)
{
    if (projectile->state == 2) return 0;
    if (SecondaryActorEnemy()->active)
        projectile->motion.velocity.y = static_cast<float>(projectile->motion.velocity.y - static_cast<double>(0.6f));
    else
        projectile->motion.velocity.y = static_cast<float>(projectile->motion.velocity.y - static_cast<double>(0.38f));
    projectile->motion.velocity.x = projectile->acceleration + projectile->motion.velocity.x;
    float direction = atan2(projectile->motion.velocity.y, projectile->motion.velocity.x);
    projectile->motion.angle = NormalizeAngle(direction);
    float squared = projectile->motion.velocity.x * projectile->motion.velocity.x +
        projectile->motion.velocity.y * projectile->motion.velocity.y;
    projectile->motion.speed = static_cast<float>(sqrt(static_cast<double>(squared)));
    return 0;
}

int __fastcall PositionSecondaryProjectileFromEmitter(SecondaryDamageReferencesView *self,
    SecondaryProjectileView *projectile)
{
    SecondaryEmitterView *emitter = EmitterAt(self, projectile->parameters->index - 1);
    CameraVector position = {static_cast<float>(emitter->x * 0.0078125),
        static_cast<float>(emitter->y * 0.0078125), 0.0f};
    projectile->motion.position = position;
    projectile->motion.position.x = projectile->parameters->offsetX - projectile->motion.velocity.x + projectile->motion.position.x;
    projectile->motion.position.y = projectile->parameters->offsetY - projectile->motion.velocity.y + projectile->motion.position.y;
    return 0;
}

int __stdcall UpdateSecondaryProjectileHomingLong(SecondaryProjectileView *projectile)
{
    if (projectile->state == 2) return 0;
    SecondaryActorManagerView *target = projectile->target;
    if (target && ((target->state.flags & 0x21) || (target->state.flags & 0xc00000)))
    {
        projectile->target = 0;
        target = 0;
    }
    float speed;
    if (!target)
    {
        speed = static_cast<float>(projectile->motion.speed + static_cast<double>(0.1f));
        if (speed > 16.0f) speed = 16.0f;
    }
    else
    {
        float dy = target->state.world.position.y - projectile->motion.position.y;
        float dx = target->state.world.position.x - projectile->motion.position.x;
        float direction = atan2(dy, dx);
        float difference = SecondaryProjectileAngleDifference(direction, projectile->motion.angle);
        speed = projectile->motion.speed;
        if (projectile->time.current >= 240)
            speed = static_cast<float>(speed + static_cast<double>(0.2f));
        else
        {
            float absolute = fabs(difference);
            if (!(absolute >= static_cast<double>(0.7853982f)))
            {
                if (AnimationExtentMagnitude(difference) < 0.2617994f)
                {
                    speed = static_cast<float>(speed + static_cast<double>(0.1f));
                    if (speed > 16.0f) speed = 16.0f;
                }
            }
            else
            {
                speed = static_cast<float>(speed - static_cast<double>(0.3f));
                if (speed < 4.0f) speed = 4.0f;
            }
            float angle = static_cast<float>(difference * static_cast<double>(0.1f) + projectile->motion.angle);
            projectile->motion.angle = NormalizeAngle(angle);
        }
    }
    projectile->motion.speed = speed;
    if (projectile->time.current >= 120)
    {
        ReleaseSecondaryAnimation(projectile->animationId);
        projectile->animationId = 0;
    }
    return 0;
}

void __stdcall DetachSecondaryProjectileTargets(SecondaryActorManagerView *target)
{
    SecondaryDamageReferencesView *self = reinterpret_cast<SecondaryDamageReferencesView *>(g_SecondaryActorReferences);
    if (self->tracked == target)
    {
        self->tracked = 0;
        self->trackingLocked = 0;
    }
    SecondaryProjectileView *projectile = &self->projectiles[0];
    for (int remaining = 256; remaining; --remaining, ++projectile)
        if (projectile->target == target) projectile->target = 0;
}

int __stdcall SpawnSecondaryFixedProjectile(const CameraVector *position, float angle)
{
    SecondaryDamageReferencesView *self = reinterpret_cast<SecondaryDamageReferencesView *>(g_SecondaryActorReferences);
    g_SecondaryFixedProjectile.angle = angle;
    return SpawnSecondaryProjectile(self, &g_SecondaryFixedProjectile, position, 0);
}

int __stdcall SecondaryPatternTimerChanged(const StageTimerView *time)
{
    return time->current != time->previous;
}

int __cdecl GetSecondaryPatternMultiplier()
{
    return reinterpret_cast<SecondaryPatternOwnerView *>(g_SecondaryActorReferences)->multiplier;
}

void __stdcall SetSecondaryPatternMultiplier(int value)
{
    reinterpret_cast<SecondaryPatternOwnerView *>(g_SecondaryActorReferences)->multiplier = value;
}

void __stdcall RewindSecondaryTimer(StageTimerView *time, int frames)
{
    int negative = static_cast<int>(0u - static_cast<unsigned int>(frames));
    OffsetSecondaryTimer(time, static_cast<float>(negative));
}
