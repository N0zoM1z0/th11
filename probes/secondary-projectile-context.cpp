// Independent object input; this diagnostic image is never executed.
#include "SecondaryProjectile.hpp"
extern "C" int __stdcall SecondaryProjectileProbe(SecondaryDamageReferencesView *self,
    SecondaryProjectileView *projectile)
{
    projectile->parameters->onUpdate = UpdateSecondaryProjectileHoming;
    int result = UpdateSecondaryProjectiles(self);
    return result + UpdateSecondaryProjectileHoming(self, projectile);
}
