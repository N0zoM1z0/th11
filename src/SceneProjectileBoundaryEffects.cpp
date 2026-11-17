#include "SecondaryClearing.hpp"
#include "SecondaryActorRuntime.hpp"
#include "StageMesh.hpp"

// Physical scene bounds inside the existing 2,000-projectile pool.
// This view does not alter the shared pool layout used by exact owners.
struct SceneProjectileRegion
{
    unsigned char unknown000[0x44];
    float centerX, centerY;
    unsigned char unknown04c[4];
    float width, height;
};

struct SceneProjectileSprite
{
    unsigned char unknown000[0x38];
    float width;
};

typedef char SceneProjectileRegionCenterOffset[
    (offsetof(SceneProjectileRegion, centerX) == 0x44) ? 1 : -1];
typedef char SceneProjectileRegionWidthOffset[
    (offsetof(SceneProjectileRegion, width) == 0x50) ? 1 : -1];

// Original three mutable script-dependent sprite-index tables.
extern int g_SceneProjectileSpriteIndicesSmall[];
extern int g_SceneProjectileSpriteIndicesMedium[];
extern int g_SceneProjectileSpriteIndicesLarge[];

// Complete native 0x40B3A0..0x40B5B8. A live scene projectile whose bounds
// intersect the scene rectangle is marked and gets an optional drop plus VM.
int __stdcall UpdateSceneProjectileBoundaryEffects(int effects)
{
    SecondaryActorSpellView *spell = SecondaryActorSpell();
    if ((spell->flags & 1u) && spell->phase >= 0x9e && spell->phase <= 0xa1)
        return 0;

    SceneProjectileRegion *region =
        reinterpret_cast<SceneProjectileRegion *>(g_SceneClearPool);
    const float halfWidth = static_cast<float>(region->width * 0.5);
    const float halfHeight = static_cast<float>(region->height * 0.5);
    const float left = static_cast<float>(region->centerX - halfWidth);
    const float right = static_cast<float>(region->centerX + halfWidth);
    const float bottom = static_cast<float>(region->centerY - halfHeight);
    const float top = static_cast<float>(region->centerY + halfHeight);

    SceneClearProjectileView *projectile = g_SceneClearPool->projectiles;
    for (int remaining = 2000; remaining; --remaining, ++projectile)
    {
        if (!projectile->state || projectile->state == 3)
            continue;
        const float xHalf = static_cast<float>(projectile->width * 0.5);
        const float yHalf = static_cast<float>(
            *reinterpret_cast<float *>(
                reinterpret_cast<unsigned char *>(projectile) + 0x460) * 0.5);
        const float xMin = static_cast<float>(projectile->position.x - xHalf);
        const float xMax = static_cast<float>(projectile->position.x + xHalf);
        const float yMin = static_cast<float>(projectile->position.y - yHalf);
        const float yMax = static_cast<float>(projectile->position.y + yHalf);

        if (!(left <= xMax && xMin <= right && bottom <= yMax && yMin <= top))
            continue;

        projectile->flags |= 8u;
        if (effects)
            SpawnSecondaryPlayerDrop(g_SecondaryControllerDrops,
                &projectile->position, 8, 0xffffffffu,
                -1.5707963705062866f, 0.6000000238418579f);

        AnimationVmId id = g_SecondaryActorFiles->file->CreatePositionedVmId(
            &projectile->position, 0x5d, 0x16);
        AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, id);
        const SceneProjectileSprite *sprite =
            reinterpret_cast<const SceneProjectileSprite *>(
                projectile->animation.unknown3ac);
        if (sprite)
        {
            const short script = *reinterpret_cast<const short *>(
                reinterpret_cast<const unsigned char *>(projectile) + 0x90e);
            if (sprite->width <= 16.0f)
                vm->spriteIndex = g_SceneProjectileSpriteIndicesSmall[script];
            else if (sprite->width <= 32.0f)
                vm->spriteIndex = g_SceneProjectileSpriteIndicesMedium[script];
            else
                vm->spriteIndex = g_SceneProjectileSpriteIndicesLarge[script];
        }
        *reinterpret_cast<unsigned int *>(
            reinterpret_cast<unsigned char *>(projectile) + 0x4bc) = 0;
    }
    return 0;
}
