// Independent object input; this diagnostic image is never executed.
#include "SecondaryDamage.hpp"
extern "C" int __stdcall SecondaryDamageProbe(const CameraVector *position,
    const float *extent, float radius)
{
    return SampleSecondaryActorDamage(position, extent) + CheckSecondaryActorCollision(position, radius);
}
