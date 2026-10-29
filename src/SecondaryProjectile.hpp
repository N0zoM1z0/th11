#pragma once
#include "SecondaryDamage.hpp"

struct SecondaryProjectileSpriteView
{
    unsigned char unknown000[0x34];
    float width, height;
};
struct SecondaryProjectileAnimationView
{
    unsigned char unknown000[0x2c];
    float angle;
    unsigned char unknown030[0x3c - 0x30];
    float scale3c, scale40;
    unsigned char unknown044[0x3a2 - 0x44];
    short script;
    unsigned char unknown3a4[8];
    SecondaryProjectileSpriteView *sprite;
    unsigned char unknown3b0[0x3dc - 0x3b0];
    CameraVector position3dc, position3e8, position3f4;
    unsigned int unknown400, flags;
};

extern unsigned char *g_SecondaryProgramHost;
extern int g_SecondarySpecialDamageScripts[5];

int __stdcall UpdateSecondaryProjectiles(SecondaryDamageReferencesView *self);
int __fastcall UpdateSecondaryProjectileHoming(SecondaryDamageReferencesView *self,
    SecondaryProjectileView *projectile);
SecondaryActorManagerView *__stdcall AcquireSecondaryProjectileTarget(const CameraVector *position, float radius);
float __stdcall SecondaryProjectileAngleDifference(float target, float current);
int __stdcall SecondaryProjectileOutside(const CameraVector *position, float width, float height);
// Native consumes an unused stack slot (RET4), consistent with the hit callback.
int __fastcall TrackSecondaryProjectileTarget(SecondaryDamageReferencesView *self,
    SecondaryProjectileView *projectile, const CameraVector *position);
float __stdcall SecondaryPositionLength(const CameraVector *position);
float __stdcall SecondaryVelocityLength(const SecondaryMotionView *motion);
float __stdcall SecondaryPositionAngle(const CameraVector *position);

typedef char SecondaryProjectileAnimationPosition[(offsetof(SecondaryProjectileAnimationView, position3e8) == 0x3e8) ? 1 : -1];
typedef char SecondaryProjectileUpdateCallback[(offsetof(SecondaryProjectileParameters, onUpdate) == 0x28) ? 1 : -1];
typedef char SecondaryProjectilePatternTimer[(offsetof(SecondaryDamageReferencesView, patternTime) == 0x930) ? 1 : -1];
typedef char SecondaryProjectileSlotStorage[(offsetof(SecondaryDamageReferencesView, slotIdStorage) == 0x8b8c) ? 1 : -1];
