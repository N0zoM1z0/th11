#include "SecondaryEmitterMotion.hpp"

int __fastcall SecondaryEmitterOrbitCallback(SecondaryEmitterObjectView *self)
{
    OrbitSecondaryEmitter(self);
    return 0;
}

int __fastcall SecondaryEmitterAimCallback(SecondaryEmitterObjectView *self)
{
    AimSecondaryEmitter(self);
    return 0;
}
