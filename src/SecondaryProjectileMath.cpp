#include "SecondaryProjectile.hpp"
#include <math.h>
#pragma intrinsic(sqrt, atan2, fabs)

float __stdcall SecondaryProjectileAngleDifference(float target, float current)
{
    if (target - current > static_cast<double>(3.1415927f)) return static_cast<float>(target - (current + static_cast<double>(6.2831855f)));
    if (current - target > static_cast<double>(3.1415927f)) return static_cast<float>(target - (current - static_cast<double>(6.2831855f)));
    return target - current;
}

int __stdcall SecondaryProjectileOutside(const CameraVector *position, float width, float height)
{
    if (position->x + width <= -192.0 || position->x - width >= 192.0 ||
        position->y + height <= 0.0 || position->y - height >= 448.0) return 1;
    return 0;
}

SecondaryActorManagerView *__stdcall AcquireSecondaryProjectileTarget(const CameraVector *position, float radius)
{
    SecondaryMasterView *host = reinterpret_cast<SecondaryMasterView *>(g_SecondaryProgramHost);
    SecondaryActorManagerView *closest = 0;
    float limit = radius * radius;
    SecondaryActorNode *node = host->first;
    while (node)
    {
        SecondaryActorManagerView *actor = node->actor;
        SecondaryActorNode *next = node->next;
        if (!(actor->state.flags & 0x21) && !(actor->state.flags & 0xc00000))
        {
            float distance = (position->x - actor->state.world.position.x) * (position->x - actor->state.world.position.x) +
                (position->y - actor->state.world.position.y) * (position->y - actor->state.world.position.y);
            if (distance < limit)
            {
                limit = distance;
                closest = actor;
            }
        }
        node = next;
    }
    return closest;
}

int __fastcall TrackSecondaryProjectileTarget(SecondaryDamageReferencesView *self, SecondaryProjectileView *projectile, const CameraVector *)
{
    projectile->target = 0;
    SecondaryActorManagerView *target = self->tracked;
    if (target)
    {
        projectile->target = target;
        float x = fabs(target->state.world.position.x);
        if (x > 224.0) projectile->target = 0;
    }
    return 0;
}

float __stdcall SecondaryPositionLength(const CameraVector *position)
{
    float squared = position->x * position->x + position->y * position->y;
    return static_cast<float>(sqrt(static_cast<double>(squared)));
}

float __stdcall SecondaryVelocityLength(const SecondaryMotionView *motion)
{
    float squared = motion->velocity.x * motion->velocity.x + motion->velocity.y * motion->velocity.y;
    return static_cast<float>(sqrt(static_cast<double>(squared)));
}

float __stdcall SecondaryPositionAngle(const CameraVector *position)
{
    return atan2(position->y, position->x);
}
