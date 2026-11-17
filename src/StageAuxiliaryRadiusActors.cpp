#include "SecondaryClearing.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryPattern.hpp"
#include "AngleMath.hpp"

// Game-owned native projectile descriptor stored at 0x004A3A50. Its angle
// field is the observed float cell at descriptor+0x14 (0x004A3A64).
extern SecondaryProjectileParameters g_SecondarySceneRetaliationProjectile;
// The actor in this global is the secondary player, not the auxiliary-stage
// singleton at 0x004A8D64.
struct SecondaryPlayerControllerView;
extern SecondaryPlayerControllerView *g_SecondaryPlayerControllerInstance;

// Native 0x412C50: separate downstream actor radius clearing, not part of
// this 395-byte scene-pool function's extent.
void __stdcall ClearSecondaryActorsForSceneRadius(
    const CameraVector *center, float radius, int effects);

// Complete native 0x40B210..0x40B39A. Unlike the conventional radius
// clearer, visible scene projectiles also spawn a fresh retaliation pattern.
int __stdcall CullStageAuxiliaryActorsInRadius(
    const CameraVector *center, float radius, int effects)
{
    const SecondaryActorSpellView *spell = SecondaryActorSpell();
    if ((spell->flags & 1u) && spell->phase >= 0x9e && spell->phase <= 0xa1)
        radius = static_cast<float>(radius / 3.0);

    SceneClearProjectileView *projectile = g_SceneClearPool->projectiles;
    for (int remaining = 2000; remaining; ++projectile, --remaining)
    {
        if (!projectile->state || projectile->state == 3 || projectile->kind)
            continue;

        const CameraVector &position = projectile->position;
        const float dx = static_cast<float>(position.x - center->x);
        const float dy = static_cast<float>(position.y - center->y);
        const float expandedRadius = static_cast<float>(projectile->width * 0.5 + radius);
        const float distanceSquared = static_cast<float>(dx * dx + dy * dy);
        if (distanceSquared > expandedRadius * expandedRadius)
            continue;

        ClearSceneProjectile(projectile);
        if (!SceneClearOutside(&position, 2.0f, 2.0f))
        {
            // The native projectile's +0x458 float is the orientation for
            // the retaliating pattern. Normalize the float-rounded sum.
            const float heading = *reinterpret_cast<const float *>(
                reinterpret_cast<const unsigned char *>(projectile) + 0x458);
            g_SecondarySceneRetaliationProjectile.angle =
                NormalizeAngle(static_cast<float>(
                    heading + 3.1415927410125732421875));
            SpawnSecondaryProjectile(
                reinterpret_cast<SecondaryDamageReferencesView *>(
                    g_SecondaryPlayerControllerInstance),
                &g_SecondarySceneRetaliationProjectile, &position, 0);
            if (effects)
                SpawnSecondaryPlayerDrop(g_SecondaryControllerDrops, &position,
                    8, 0xffffffffu, -1.5707963705062866f,
                    0.6000000238418579f);
        }
    }

    ClearSecondaryActorsForSceneRadius(center, radius, effects);
    return 0;
}
