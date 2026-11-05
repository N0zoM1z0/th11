// Independent real API inputs; this diagnostic image is never executed.
#include "SecondaryClearing.hpp"
#include "MeshEffects.hpp"
extern "C" int __stdcall SecondaryClearingProbe(const CameraVector *position,
    float radius, int effects, int filter, int mode, int flags,
    SceneClearProjectileView *projectile, AnimationFileView *file, float halfHeight)
{
    unsigned int total = ClearSceneProjectilesInRadius(position, radius, effects, filter);
    ClearSceneProjectiles(filter);
    total += ClearSceneProjectile(projectile);
    total += SceneClearOutside(position, radius, halfHeight);
    ClearSecondaryActorsInRadius(position, radius, effects);
    total += ClearSecondaryObjectsInRadius(position, radius, mode, flags);
    total += ClearSecondaryObjects(mode, flags);
    CreatePositionedAnimationVmId(file, position, mode, flags);
    BindAnimationScript(file, mode, &projectile->animation);
    return static_cast<int>(total);
}
