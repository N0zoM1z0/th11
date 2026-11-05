#include "SecondaryClearing.hpp"
#include "MeshEffects.hpp"

int __stdcall SceneClearOutside(const CameraVector *position, float halfWidth, float halfHeight)
{
    if (position->x + halfWidth <= -192.0 || position->x - halfWidth >= 192.0 ||
        position->y + halfHeight <= 0.0 || position->y - halfHeight >= 448.0) return 1;
    return 0;
}

int __stdcall ClearSceneProjectile(SceneClearProjectileView *projectile)
{
    if (projectile->state != 2 && projectile->state != 1) return 0;
    if (!SceneClearOutside(&projectile->position, 8.0f, 8.0f))
    {
        projectile->state = 3;
        reinterpret_cast<SecondaryActorAnimationView *>(&projectile->animation)->event = 1;
        if (projectile->clearScript >= 0)
            CreatePositionedAnimationVmId(g_SecondaryActorFiles->file,
                &projectile->position, projectile->clearScript, 0x16);
        projectile->clearTime.SetFrame(-1);
        return 1;
    }
    projectile->flags |= 8;
    reinterpret_cast<SecondaryActorAnimationView *>(&projectile->animation)->event = 1;
    projectile->state = 3;
    return 1;
}

int __stdcall ClearSceneProjectilesInRadius(const CameraVector *position, float radius, int effects, int filter)
{
    SceneClearProjectileView *projectile = g_SceneClearPool->projectiles;
    SecondaryActorSpellView *spell = SecondaryActorSpell();
    if ((spell->flags & 1) && spell->phase >= 0x9e && spell->phase <= 0xa1)
        radius = static_cast<float>(radius / 3.0);
    for (int remaining = 2000; remaining; ++projectile, --remaining)
    {
        if (!projectile->state || projectile->state == 3) continue;
        if (filter && projectile->kind) continue;
        float dx = projectile->position.x - position->x;
        float dy = projectile->position.y - position->y;
        float expanded = static_cast<float>(projectile->width * 0.5 + radius);
        float distance = dx * dx + dy * dy;
        // Native ordered rejection also leaves the unordered comparison on this path.
        if (distance > expanded * expanded) continue;
        ClearSceneProjectile(projectile);
        if (!SceneClearOutside(&projectile->position, 2.0f, 2.0f) && effects)
            SpawnSecondaryPlayerDrop(g_SecondaryControllerDrops, &projectile->position,
                8, 0xffffffff, -1.5707964f, 0.6f);
    }
    ClearSecondaryActorsInRadius(position, radius, effects);
    return 0;
}

void __stdcall ClearSceneProjectiles(int filter)
{
    SceneClearProjectileView *projectile = g_SceneClearPool->projectiles;
    SecondaryActorSpellView *spell = SecondaryActorSpell();
    if ((spell->flags & 1) && spell->phase >= 0x9e && spell->phase <= 0xa1) return;
    for (int remaining = 2000; remaining; ++projectile, --remaining)
    {
        if (!projectile->state || projectile->state == 3) continue;
        if (filter && projectile->kind) continue;
        ClearSceneProjectile(projectile);
    }
}

void __stdcall ClearSecondaryActorsInRadius(const CameraVector *position, float radius, int effects)
{
    SecondaryActorNode *next;
    for (SecondaryActorNode *node = g_SecondaryMaster->first; node; node = next)
    {
        SecondaryActorManagerView *actor = node->actor;
        next = node->next;
        if (!(actor->state.flags & 0x800)) continue;
        CameraVector *point = &actor->state.world.position;
        float dx = point->x - position->x;
        float dy = point->y - position->y;
        float expanded = static_cast<float>(radius + 16.0);
        float distance = dx * dx + dy * dy;
        // This owner accepts only ordered less/equal, unlike the scene-pool loop.
        if (distance <= expanded * expanded)
        {
            SecondaryCounterView &counter = actor->state.counter;
            if (counter.flags & 1)
            {
                counter.scaled = static_cast<int>(static_cast<unsigned int>(counter.scaled) - 99999);
                int base = static_cast<int>(static_cast<unsigned int>(counter.baseline) * 7);
                int delta = static_cast<int>(static_cast<unsigned int>(counter.scaled) - base);
                counter.value = static_cast<int>(static_cast<unsigned int>(delta / 7) + counter.baseline);
            }
            else counter.value = static_cast<int>(static_cast<unsigned int>(counter.value) - 99999);
            if (point->x + 2.0 <= -192.0 || point->x - 2.0 >= 192.0 ||
                point->y + 2.0 <= 0.0 || point->y - 2.0 >= 448.0) continue;
            if (effects)
                SpawnSecondaryPlayerDrop(g_SecondaryControllerDrops, point,
                    8, 0xffffffff, -1.5707964f, 0.6f);
        }
    }
}

int __stdcall ClearSecondaryObjectsInRadius(const CameraVector *position, float radius, int mode, int flags)
{
    SecondaryClearManagerView *manager = g_SecondaryClearManager;
    SecondaryClearObjectView *object = manager->first;
    manager->position = *position;
    int total = 0;
    while (object)
    {
        SecondaryClearObjectView *next = object->next;
        if (object->state != 1)
            total = static_cast<int>(static_cast<unsigned int>(total) + object->ClearInRadius(position, radius, mode, flags));
        object = next;
    }
    return total;
}

int __stdcall ClearSecondaryObjects(int mode, int flags)
{
    for (SecondaryClearObjectView *object = g_SecondaryClearManager->first; object; )
    {
        SecondaryClearObjectView *next = object->next;
        if (object->state != 1) object->Clear(mode, flags);
        object = next;
    }
    return 1;
}
