#include "SecondaryPattern.hpp"

extern "C" int __stdcall SecondaryPatternProbe(SecondaryDamageReferencesView *self,
    SecondaryProjectileParameters *parameters, const CameraVector *position, unsigned int context, int index)
{
    unsigned int id;
    CopySecondaryEmitterId(&id, self, index);
    SecondaryEmitterView *emitter = GetSecondaryEmitter(self, index);
    return SpawnSecondaryProjectile(self, parameters, position, context) + id + emitter->x;
}
