// Independent inputs retain the real lookup and unspecialized timer contracts.
#include "SecondaryDamage.hpp"
extern "C" int __stdcall SecondaryDamageSupportProbe(SecondaryDamageReferencesView *self,
    const CameraVector *position, CameraVector *output, unsigned int *id,
    int period, RenderBatchView *renderer, AnimationVmId lookup)
{
    FindAnimationVmById(renderer, lookup);
    SignalSecondaryProjectileHit(id);
    ClearSecondaryProjectileHit(id);
    PositionSecondaryDamageAnimation(output, position);
    SecondaryDamageRegionView *region = AllocateSecondaryDamageRegion(self, position,
        position->x, position->y, period, self->regions[0].damage);
    return self->projectiles[0].time.Elapsed(period) + region->damage +
        SampleSecondarySpecialDamage(position, &position->x);
}
