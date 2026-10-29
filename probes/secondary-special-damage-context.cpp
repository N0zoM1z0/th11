// Independent object input; this diagnostic image is never executed.
#include "SecondaryProjectile.hpp"
extern "C" int __stdcall SecondarySpecialDamageProbe(SecondaryDamageEnemyView *self,
    const CameraVector *position, const float *extent)
{
    return SampleSecondarySpecialDamage3(self, position, extent) +
        SampleSecondarySpecialDamage4(self, position);
}
