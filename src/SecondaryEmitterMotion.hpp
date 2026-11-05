#pragma once
#include "SecondaryPattern.hpp"

// Further physical fields used by the emitter callbacks, not a class identity.
struct SecondaryEmitterMotionOwnerView
{
    unsigned char unknown000[0x888];
    int x, y;
    unsigned char unknown890[0x8a0 - 0x890];
    CameraVector movement, rememberedMovement;
    unsigned char unknown8b8[0x8c14 - 0x8b8];
    float angle;
    unsigned char unknown8c18[0x8d20 - 0x8c18];
    int focused;
};

struct SecondaryEmitterPolarVector
{
    float x, y, z;
    // Reviewed 433CD0: ECX destination, two stack floats, paired FSINCOS.
    // The callers reserve a float3; this operation leaves z untouched.
    // Its implementation remains open.
    void SetPolar(float angle, float radius);
};

void __stdcall OrbitSecondaryEmitter(SecondaryEmitterObjectView *self);
void __stdcall AimSecondaryEmitter(SecondaryEmitterObjectView *self);
float __stdcall SecondaryDamageAngleDifference(float target, float current);

typedef char SecondaryEmitterMovementOffset[(offsetof(SecondaryEmitterMotionOwnerView, movement) == 0x8a0) ? 1 : -1];
typedef char SecondaryEmitterRememberedMovementOffset[(offsetof(SecondaryEmitterMotionOwnerView, rememberedMovement) == 0x8ac) ? 1 : -1];
typedef char SecondaryEmitterAimOffset[(offsetof(SecondaryEmitterMotionOwnerView, angle) == 0x8c14) ? 1 : -1];
typedef char SecondaryEmitterFocusOffset[(offsetof(SecondaryEmitterMotionOwnerView, focused) == 0x8d20) ? 1 : -1];
