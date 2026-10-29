#include "SecondaryPattern.hpp"
#include "AngleMath.hpp"
#include "Random.hpp"

extern RandomStateView g_TextRandomState;

namespace
{
__forceinline SecondaryEmitterView *EmitterAt(SecondaryDamageReferencesView *self, int index)
{
    return reinterpret_cast<SecondaryEmitterView *>(reinterpret_cast<unsigned char *>(self) +
        0x75cc + static_cast<unsigned int>(index) * 0xe4);
}

__forceinline AnimationVmView *SpawnedVm(unsigned int id)
{
    if (!id) return 0;
    BoundVmListsView *lists = reinterpret_cast<BoundVmListsView *>(g_RenderBatch);
    for (BoundVmNodeView *node = lists->first; node; node = node->next)
        if (node->vm->id == id) return reinterpret_cast<AnimationVmView *>(node->vm);
    for (BoundVmNodeView *node = lists->second; node; node = node->next)
        if (node->vm->id == id) return reinterpret_cast<AnimationVmView *>(node->vm);
    return 0;
}
}

SecondaryEmitterView *__stdcall GetSecondaryEmitter(SecondaryDamageReferencesView *self, int index)
{
    return EmitterAt(self, index);
}

unsigned int *__stdcall CopySecondaryEmitterId(unsigned int *output,
    SecondaryDamageReferencesView *self, int index)
{
    *output = EmitterAt(self, index)->id;
    return output;
}

int __stdcall SpawnSecondaryProjectile(SecondaryDamageReferencesView *self,
    SecondaryProjectileParameters *parameters, const CameraVector *position, unsigned int context)
{
    SecondaryProjectileView *projectile = &self->projectiles[0];
    if (parameters->kind == 4 && self->SlotId(parameters->index)) return 0;
    int i = 0;
    while (projectile->state)
    {
        ++i;
        ++projectile;
        if (i >= 256) return 0;
    }
    projectile->state = 1;
    projectile->parameters = parameters;
    SecondaryDamageTimerView &time = projectile->time;
    if (!(time.flags & 1))
    {
        time.fraction = 0.0f;
        time.current = 0;
        time.previous = -999999;
        time.scale = &g_AnimationTimeScale;
        time.flags |= 1;
    }
    time.fraction = 0.0f;
    time.current = 0;
    time.previous = -1;
    projectile->damage = parameters->damage;
    if (!parameters->index)
        projectile->motion.position = *position;
    else
    {
        SecondaryEmitterView *emitter = EmitterAt(self, parameters->index - 1);
        projectile->motion.position.x = static_cast<float>(emitter->x * 0.0078125);
        projectile->motion.position.y = static_cast<float>(emitter->y * 0.0078125);
        projectile->motion.position.z = 0.0f;
    }
    if (parameters->kind == 4) self->SlotId(parameters->index) = 1;
    projectile->motion.speed = parameters->speed;
    if (parameters->angle >= 1000.0 && parameters->index)
    {
        float random = static_cast<float>(RandomNext32(&g_TextRandomState) *
            4.656612873077392578125e-10 - 1.0);
        projectile->motion.angle = NormalizeAngle(static_cast<float>(
            EmitterAt(self, parameters->index - 1)->angle +
            random * static_cast<double>(3.1415927f) / 12.0));
        random = static_cast<float>(RandomNext32(&g_TextRandomState) *
            4.656612873077392578125e-10 - 1.0);
        projectile->motion.speed = random + random + parameters->speed;
    }
    else if (parameters->angle >= 995.0 && parameters->angle < 1000.0 && parameters->index)
        projectile->motion.angle = NormalizeAngle(EmitterAt(self, parameters->index - 1)->angle);
    else
        projectile->motion.angle = NormalizeAngle(parameters->angle);
    if (!(projectile->motion.flags & 1))
    {
        reinterpret_cast<SecondaryVector2 *>(&projectile->motion.velocity)->SetPolar(
            projectile->motion.angle, projectile->motion.speed);
        projectile->motion.velocity.z = 0.0f;
    }
    else
    {
        projectile->motion.radius = projectile->motion.radialVelocity + projectile->motion.radius;
        projectile->motion.angle = NormalizeAngle(projectile->motion.speed + projectile->motion.angle);
    }
    projectile->motion.position.x = parameters->offsetX - projectile->motion.velocity.x + projectile->motion.position.x;
    projectile->motion.position.y = parameters->offsetY - projectile->motion.velocity.y + projectile->motion.position.y;
    projectile->animationId = CreateAnimationVmId(self->file, parameters->initialScript + 5, 0x16).value;
    SecondaryProjectileAnimationView *vm = reinterpret_cast<SecondaryProjectileAnimationView *>(
        SpawnedVm(projectile->animationId));
    if (!vm) projectile->animationId = 0;
    if (vm->flags & 0x08000000)
    {
        vm->angle = parameters->angle;
        vm->flags |= 4;
    }
    projectile->auxiliaryId = 0;
    if (parameters->onSpawn) parameters->onSpawn(self, projectile, context);
    if (parameters->sound >= 0) QueueSecondaryActorSound(parameters->sound, projectile->motion.position.x);
    vm->position3e8.x = static_cast<float>(projectile->motion.position.x + 32.0 + 192.0);
    vm->position3e8.y = static_cast<float>(projectile->motion.position.y + 16.0);
    vm->position3e8.z = projectile->motion.position.z;
    return 0;
}
