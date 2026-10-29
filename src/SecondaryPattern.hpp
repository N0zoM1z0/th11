#pragma once
#include "SecondaryProjectile.hpp"

// Observed emitter prefix. E4 is a stride, not a proved complete array type.
struct SecondaryEmitterView
{
    int x, y;
    unsigned char unknown008[0x4c - 8];
    float angle;
    unsigned int unknown050, id;
};

SecondaryEmitterView *__stdcall GetSecondaryEmitter(SecondaryDamageReferencesView *self, int index);
unsigned int *__stdcall CopySecondaryEmitterId(unsigned int *output,
    SecondaryDamageReferencesView *self, int index);
int __stdcall SpawnSecondaryProjectile(SecondaryDamageReferencesView *self,
    SecondaryProjectileParameters *parameters, const CameraVector *position, unsigned int context);

typedef char SecondaryEmitterAngleOffset[(offsetof(SecondaryEmitterView, angle) == 0x4c) ? 1 : -1];
typedef char SecondaryEmitterIdOffset[(offsetof(SecondaryEmitterView, id) == 0x54) ? 1 : -1];
