#include "SecondaryClearing.hpp"
#include "SecondaryActorRuntime.hpp"

// Native 0x412B10 owns the separate 302-byte enemy/actor band clear.
// Keep its source/producer boundary distinct from this scene-projectile pass.
int __stdcall ClearSecondaryActorsInHorizontalBand(
    const CameraVector *center, float halfWidth, int effects);

// Complete original TH11 0x40B0E0..0x40B20B. This variation clears
// projectiles inside a horizontal band and below the given Y threshold.
int __stdcall ClearStageAuxiliaryHorizontalBand(
    const CameraVector *center, float halfWidth, int effects)
{
    SecondaryActorSpellView *spell = SecondaryActorSpell();
    if ((spell->flags & 1u) && spell->phase >= 0x9e && spell->phase <= 0xa1)
        halfWidth = static_cast<float>(halfWidth / 3.0);

    SceneClearProjectileView *projectile = g_SceneClearPool->projectiles;
    for (int remaining = 2000; remaining; --remaining, ++projectile)
    {
        if (!projectile->state || projectile->state == 3)
            continue;
        if (projectile->kind != 0)
            continue;

        const CameraVector &point = projectile->position;
        if (center->x - halfWidth <= point.x &&
            point.x <= center->x + halfWidth &&
            point.y <= center->y)
        {
            ClearSceneProjectile(projectile);
            if (!SceneClearOutside(&point, 2.0f, 2.0f) && effects)
                SpawnSecondaryPlayerDrop(g_SecondaryControllerDrops,
                    &point, 8, 0xffffffffu, -1.5707964f, 0.6f);
        }
    }
    ClearSecondaryActorsInHorizontalBand(center, halfWidth, effects);
    return 0;
}
