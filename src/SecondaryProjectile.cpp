#include "SecondaryProjectile.hpp"
#include "AngleMath.hpp"
#include "AnimationImmediateDraw.hpp"
#include <math.h>
#pragma intrinsic(atan2, fabs)

namespace
{
__forceinline AnimationVmView *PrimaryProjectileVm(unsigned int id)
{
    if (!id) return 0;
    BoundVmListsView *lists = reinterpret_cast<BoundVmListsView *>(g_RenderBatch);
    for (BoundVmNodeView *node = lists->first; node; node = node->next)
        if (node->vm->id == id) return reinterpret_cast<AnimationVmView *>(node->vm);
    for (BoundVmNodeView *node = lists->second; node; node = node->next)
        if (node->vm->id == id) return reinterpret_cast<AnimationVmView *>(node->vm);
    return 0;
}

__forceinline void PositionProjectileVm(AnimationVmView *vm, const CameraVector *position)
{
    CameraVector &output = reinterpret_cast<SecondaryProjectileAnimationView *>(vm)->position3e8;
    output.x = static_cast<float>(position->x + 32.0 + 192.0);
    output.y = static_cast<float>(position->y + 16.0);
    output.z = position->z;
}

__forceinline void AdvanceProjectileTime(SecondaryDamageTimerView *self)
{
    int current = self->current;
    self->previous = current;
    if (*self->scale > static_cast<double>(0.99f) && *self->scale < 1.01f)
    {
        self->current = static_cast<int>(static_cast<unsigned int>(current) + 1);
        self->fraction = static_cast<float>(self->fraction + 1.0);
    }
    else
    {
        self->fraction = *self->scale + self->fraction;
        self->current = static_cast<int>(self->fraction);
    }
}

__forceinline void ReleaseProjectileAuxiliary(unsigned int id)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, AnimationVmId(id));
    if (vm)
    {
        vm->flags.value |= 0x4000000;
        BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
        if (!bound->marker018)
            for (BoundVmNodeView *node = bound->related; node; node = node->next)
                node->vm->flags |= 0x4000000;
    }
}
}

int __stdcall UpdateSecondaryProjectiles(SecondaryDamageReferencesView *self)
{
    SecondaryProjectileView *projectile = &self->projectiles[0];
    for (int remaining = 256; remaining; --remaining, ++projectile)
    {
        if (!projectile->state) continue;
        if (projectile->parameters->kind == 4 && projectile->state == 1 &&
            (self->patternTime.current < 0 || self->patternSlot <= projectile->parameters->index - 1))
        {
            SignalSecondaryAnimation(&projectile->animationId);
            SignalSecondaryAnimation(&projectile->auxiliaryId);
            projectile->state = 2;
            self->SlotId(projectile->parameters->index) = 0;
        }
        if (projectile->parameters->kind == 4 && projectile->state == 1)
        {
            SecondaryCollisionHudView *hud = reinterpret_cast<SecondaryCollisionHudView *>(g_SecondaryActorHud);
            if ((hud && hud->blocked) || !g_SecondaryProgramHost)
            {
                projectile->state = 2;
                SignalSecondaryAnimation(&projectile->animationId);
                SignalSecondaryAnimation(&projectile->auxiliaryId);
                self->SlotId(projectile->parameters->index) = 0;
            }
        }
        if (projectile->parameters->kind == 4 && !projectile->hitThisFrame &&
            projectile->state == 1 && projectile->hitSignalled == 1)
        {
            ClearSecondaryProjectileHit(&projectile->animationId);
            projectile->hitSignalled = 0;
        }
        projectile->hitThisFrame = 0;
        if (projectile->parameters->onUpdate)
            projectile->parameters->onUpdate(self, projectile);
        SecondaryMotionView *motion = &projectile->motion;
        if (!(motion->flags & 1))
        {
            reinterpret_cast<SecondaryVector2 *>(&motion->velocity)->SetPolar(motion->angle, motion->speed);
            motion->velocity.z = 0.0f;
        }
        else
        {
            motion->radius = motion->radialVelocity + motion->radius;
            motion->angle = NormalizeAngle(motion->speed + motion->angle);
        }
        ComposeSecondaryMotion(motion);
        AnimationVmView *vm = PrimaryProjectileVm(projectile->animationId);
        if (!vm)
        {
            projectile->state = 0;
            ReleaseProjectileAuxiliary(projectile->auxiliaryId);
            projectile->animationId = projectile->auxiliaryId = 0;
            continue;
        }
        unsigned char kind = projectile->parameters->kind;
        SecondaryProjectileAnimationView *animation = reinterpret_cast<SecondaryProjectileAnimationView *>(vm);
        if (kind != 4 && projectile->time.current >= 10 &&
            SecondaryProjectileOutside(&motion->position,
                animation->sprite->height * animation->scale3c,
                animation->sprite->width * animation->scale40))
        {
            if (!(kind == 2 && !(motion->position.x <= -224.0) &&
                !(motion->position.x >= 224.0) && !(motion->position.y < -32.0f)))
            {
                ReleaseSecondaryAnimation(projectile->animationId);
                projectile->animationId = 0;
                projectile->state = 0;
                continue;
            }
        }
        PositionProjectileVm(vm, &motion->position);
        if (projectile->auxiliaryId)
        {
            AnimationVmView *auxiliary = FindAnimationVmById(g_RenderBatch, AnimationVmId(projectile->auxiliaryId));
            if (!auxiliary) projectile->auxiliaryId = 0;
            else PositionProjectileVm(auxiliary, &motion->position);
        }
        if (vm->flags.value & 0x8000000)
        {
            animation->angle = motion->angle;
            vm->flags.value |= 4;
        }
        AdvanceProjectileTime(&projectile->time);
    }
    return 0;
}

int __fastcall UpdateSecondaryProjectileHoming(SecondaryDamageReferencesView *, SecondaryProjectileView *projectile)
{
    if (projectile->state == 2) return 0;
    if (!projectile->target)
    {
        CameraVector position = projectile->motion.position;
        position.y = static_cast<float>(position.y - 128.0);
        projectile->target = AcquireSecondaryProjectileTarget(&position, 80.0f);
    }
    if (projectile->target)
    {
        unsigned int flags = projectile->target->state.flags;
        if (!(flags & 0x21) && !(flags & 0xc00000))
        {
            float dy = projectile->target->state.world.position.y - projectile->motion.position.y;
            float dx = projectile->target->state.world.position.x - projectile->motion.position.x;
            float direction = atan2(dy, dx);
            float difference = SecondaryProjectileAngleDifference(direction, projectile->motion.angle);
            float speed = projectile->motion.speed;
            if (projectile->time.current >= 40)
            {
                projectile->motion.speed = static_cast<float>(speed + static_cast<double>(0.2f));
                return 0;
            }
            float absolute = fabs(difference);
            if (absolute >= 2.0943952f) difference = 0.0f;
            absolute = fabs(difference);
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
            float angle = static_cast<float>(difference * static_cast<double>(0.04f) + projectile->motion.angle);
            projectile->motion.angle = NormalizeAngle(angle);
            projectile->motion.speed = speed;
            return 0;
        }
        projectile->target = 0;
    }
    float speed = static_cast<float>(projectile->motion.speed + static_cast<double>(0.1f));
    if (speed > 16.0f) speed = 16.0f;
    projectile->motion.speed = speed;
    return 0;
}
