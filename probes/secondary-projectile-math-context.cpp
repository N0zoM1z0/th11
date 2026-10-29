// Independent inputs retain the observed scalar/vector and lookup contracts.
#include "SecondaryProjectile.hpp"
extern "C" float __stdcall SecondaryProjectileMathProbe(SecondaryDamageReferencesView *self,
    SecondaryProjectileView *projectile, const CameraVector *position, float radius, float angle)
{
    projectile->parameters->onHit = TrackSecondaryProjectileTarget;
    TrackSecondaryProjectileTarget(self, projectile, position);
    SecondaryActorManagerView *target = AcquireSecondaryProjectileTarget(position, radius);
    float value = SecondaryProjectileAngleDifference(angle, projectile->motion.angle);
    value += SecondaryPositionLength(position) + SecondaryVelocityLength(&projectile->motion);
    value += SecondaryPositionAngle(position);
    return value + SecondaryProjectileOutside(position, radius, radius) + (target != 0);
}
