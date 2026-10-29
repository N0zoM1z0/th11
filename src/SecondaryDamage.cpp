#include "SecondaryDamage.hpp"
#include "AnimationVmFactory.hpp"
#include <math.h>
#pragma intrinsic(sin, cos)

namespace
{
__forceinline int AddDamage(int left, int right)
{
    return static_cast<int>(static_cast<unsigned int>(left) + right);
}
}

int __stdcall SampleSecondaryActorDamage(const CameraVector *position, const float *extent)
{
    SecondaryDamageReferencesView *self = reinterpret_cast<SecondaryDamageReferencesView *>(g_SecondaryActorReferences);
    int damage = 0;
    if (self->damageTime.current == self->damageTime.previous) return 0;
    float minX = static_cast<float>(position->x - extent[0] * 0.5);
    float minY = static_cast<float>(position->y - extent[1] * 0.5);
    float maxX = static_cast<float>(position->x + extent[0] * 0.5);
    float maxY = static_cast<float>(position->y + extent[1] * 0.5);
    SecondaryProjectileView *projectile = &self->projectiles[0];
    for (int remaining = 256; remaining; --remaining, ++projectile)
    {
        if (!projectile->state || projectile->state == 2) continue;
        SecondaryProjectileParameters *parameters = projectile->parameters;
        float left = static_cast<float>(projectile->motion.position.x - parameters->width * 0.5);
        float top = static_cast<float>(projectile->motion.position.y - parameters->height * 0.5);
        float right = static_cast<float>(projectile->motion.position.x + parameters->width * 0.5);
        float bottom = static_cast<float>(projectile->motion.position.y + parameters->height * 0.5);
        if (top > maxY || maxX < left || minY > bottom || minX > right) continue;
        if (parameters->kind != 4)
        {
            if (top < -0.0f) continue;
        }
        else if (maxY < -0.0f) continue;
        if (parameters->onHit && parameters->onHit(self, projectile, position)) continue;
        if (!projectile->hitSignalled)
        {
            SignalSecondaryProjectileHit(&projectile->animationId);
            projectile->hitSignalled = 1;
        }
        projectile->hitThisFrame = 1;
        unsigned char kind = projectile->parameters->kind;
        if (kind == 4)
        {
            if (projectile->time.Elapsed(4)) damage = AddDamage(damage, projectile->damage);
        }
        else damage = AddDamage(damage, projectile->damage);
        if (kind != 4 && kind != 1)
        {
            AnimationVmView *vm = ClearMissingAnimationId(&projectile->animationId);
            float angle = reinterpret_cast<SecondaryActorAnimationView *>(vm)->angle;
            ReleaseSecondaryAnimation(projectile->animationId);
            projectile->animationId = 0;
            if (projectile->parameters->kind != 2)
                projectile->animationId = CreateAnimationVmId(self->file, projectile->parameters->script + 5, 0x16).value;
            else if (SecondaryActorEnemy()->active)
            {
                projectile->animationId = CreateAnimationVmId(self->file, projectile->parameters->script + 6, 0x16).value;
                damage = AddDamage(damage, projectile->damage / 3);
            }
            else projectile->animationId = CreateAnimationVmId(self->file, projectile->parameters->script + 5, 0x16).value;
            vm = ClearMissingAnimationId(&projectile->animationId);
            vm->flags.value |= 4;
            reinterpret_cast<SecondaryActorAnimationView *>(vm)->angle = angle;
            projectile->motion.position.z = 0.1f;
            projectile->state = 2;
            projectile->motion.speed = static_cast<float>(projectile->motion.speed * 0.125);
            PositionSecondaryDamageAnimation(&reinterpret_cast<SecondaryActorAnimationView *>(vm)->position,
                &projectile->motion.position);
        }
        kind = projectile->parameters->kind;
        if (kind == 1)
        {
            if (projectile->damage > 1)
            {
                projectile->damage /= 10;
                if (projectile->damage <= 0) projectile->damage = 1;
            }
        }
        else if (kind == 2) QueueSecondaryActorSound(0x32, projectile->motion.position.x);
        else if (kind == 3)
            AllocateSecondaryDamageRegion(self, &projectile->motion.position, 32.0f, 1.4f, 0xd, projectile->damage / 3);
    }
    damage = AddDamage(damage, SampleSecondarySpecialDamage(position, extent));
    SecondaryDamageRegionView *region = &self->regions[0];
    for (int remaining = 32; remaining; --remaining, ++region)
    {
        if (!(region->flags & 1)) continue;
        if (region->time.current != region->time.previous && region->time.current % region->period == 0) continue;
        if (!(region->flags & 2))
        {
            if (region->angle == 0.0f)
            {
                if (maxX < region->motion.position.x - region->width * 0.5 ||
                    minX > region->motion.position.x + region->width * 0.5 ||
                    maxY < region->motion.position.y - region->height * 0.5 ||
                    minY > region->motion.position.y + region->height * 0.5) continue;
            }
            else
            {
                SecondaryVector2 delta = { position->x - region->motion.position.x,
                    position->y - region->motion.position.y };
                float angle = -region->angle;
                float sine = sin(angle);
                float cosine = cos(angle);
                SecondaryVector2 rotated = { delta.x * cosine - sine * delta.y,
                    cosine * delta.y + sine * delta.x };
                if (rotated.x + extent[0] * 0.5 < -region->width * 0.5 ||
                    rotated.x - extent[0] * 0.5 > region->width * 0.5 ||
                    rotated.y + extent[1] * 0.5 < -region->height * 0.5 ||
                    rotated.y - extent[1] * 0.5 > region->height * 0.5) continue;
            }
        }
        else
        {
            float distance = (region->motion.position.x - position->x) * (region->motion.position.x - position->x) +
                (region->motion.position.y - position->y) * (region->motion.position.y - position->y);
            if (distance > region->radius * region->radius) continue;
        }
        damage = AddDamage(damage, region->damage);
        region->accumulated = AddDamage(region->accumulated, region->damage);
        if (region->accumulated >= region->limit) region->damage = 0;
    }
    if (damage >= 30)
    {
        damage = static_cast<int>(static_cast<unsigned int>(damage) * 3 - 90) / 5 + 30;
        if (damage >= 70)
        {
            damage = (damage - 50) / 5 + 50;
            if (damage >= 80) damage = 80;
        }
    }
    if (damage)
    {
        g_SecondaryActorScore = AddDamage(g_SecondaryActorScore, (damage / 10 + 10) / 10);
        if (g_SecondaryActorScore >= 1000000000) g_SecondaryActorScore = 999999999;
    }
    return damage;
}

int __stdcall CheckSecondaryActorCollision(const CameraVector *position, float radius)
{
    SecondaryCollisionReferencesView *self = reinterpret_cast<SecondaryCollisionReferencesView *>(g_SecondaryActorReferences);
    float distance = (self->position.x - position->x) * (self->position.x - position->x) +
        (self->position.y - position->y) * (self->position.y - position->y);
    if (!(self->shape->radius * self->shape->radius + radius * radius <= distance))
    {
        SecondaryCollisionHudView *hud = reinterpret_cast<SecondaryCollisionHudView *>(g_SecondaryActorHud);
        if ((!hud || !hud->blocked) && self->mode != 2 && self->mode != 4 && self->mode != 3 && !(self->flags & 2))
        {
            if (self->status <= 0) BeginSecondaryCollision(self);
            return 1;
        }
    }
    else
    {
        float extra = static_cast<float>(radius / 2.5);
        if (extra < 40.0f) extra = 40.0f;
        if (!((self->shape->radius + extra) * (self->shape->radius + extra) + radius * radius <= distance)) return 2;
    }
    return 0;
}
