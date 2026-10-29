#pragma once
#include "SecondaryActorRuntime.hpp"

// Physical timer/record views; original class identities are unresolved.
struct SecondaryDamageTimerView
{
    int previous, current;
    float fraction;
    float *scale;
    unsigned int flags;
    int Elapsed(int period) const;
};
struct SecondaryDamageReferencesView;
struct SecondaryProjectileView;
struct SecondaryProjectileParameters
{
    unsigned char unknown000[0xc];
    float width, height;
    unsigned char unknown014[0x1d - 0x14];
    unsigned char kind;
    unsigned char unknown01e[2];
    short script;
    unsigned char unknown022[0x30 - 0x22];
    int (__fastcall *onHit)(SecondaryDamageReferencesView *self,
        SecondaryProjectileView *projectile, const CameraVector *position);
};
struct SecondaryProjectileView
{
    SecondaryDamageTimerView time;
    SecondaryMotionView motion;
    unsigned int state, animationId, auxiliaryId;
    SecondaryActorManagerView *target;
    int hitThisFrame, hitSignalled, damage;
    SecondaryProjectileParameters *parameters;
    unsigned int unknown068;
};
struct SecondaryDamageRegionView
{
    float radius, growth, angle;
    unsigned int unknown00c;
    float width, height;
    SecondaryMotionView motion;
    SecondaryDamageTimerView time;
    int damage, accumulated, limit, period;
    unsigned int flags;
};
struct SecondaryDamageReferencesView
{
    unsigned char unknown000[0x10];
    AnimationFileView *file;
    unsigned char unknown014[0x944 - 0x14];
    SecondaryDamageTimerView damageTime;
    unsigned char unknown958[0x96c - 0x958];
    SecondaryProjectileView projectiles[256];
    unsigned char unknown756c[0x7c9c - 0x756c];
    SecondaryDamageRegionView regions[32];
};
struct SecondaryDamageEnemyView
{
    unsigned char unknown000[0x3c];
    int active;
    unsigned char unknown040[0x484 - 0x40];
    float x, y;
    unsigned int unknown48c;
    float radius;
};
struct SecondaryCollisionShapeView
{
    unsigned int unknown000;
    float radius;
};
struct SecondaryCollisionReferencesView
{
    unsigned char unknown000[0x87c];
    CameraVector position;
    unsigned char unknown888[0x928 - 0x888];
    int mode;
    SecondaryCollisionShapeView *shape;
    unsigned char unknown930[0x8bb4 - 0x930];
    int status;
    unsigned char unknown8bb8[0x8bc4 - 0x8bb8];
    unsigned int flags;
};
struct SecondaryCollisionHudView
{
    unsigned char unknown000[0x4438];
    int blocked;
};

void __stdcall SignalSecondaryProjectileHit(unsigned int *id);
void __stdcall ClearSecondaryProjectileHit(unsigned int *id);
void __stdcall PositionSecondaryDamageAnimation(CameraVector *output, const CameraVector *position);
SecondaryDamageRegionView *__stdcall AllocateSecondaryDamageRegion(
    SecondaryDamageReferencesView *self, const CameraVector *position,
    float radius, float growth, int frame, int damage);
int __stdcall SampleSecondarySpecialDamage(const CameraVector *position, const float *extent);
int __stdcall SampleSecondarySpecialDamage2(SecondaryDamageEnemyView *self, const CameraVector *position);
int __stdcall SampleSecondarySpecialDamage3(SecondaryDamageEnemyView *self,
    const CameraVector *position, const float *extent);
int __stdcall SampleSecondarySpecialDamage4(SecondaryDamageEnemyView *self, const CameraVector *position);
void __stdcall BeginSecondaryCollision(SecondaryCollisionReferencesView *self);

typedef char SecondaryDamageTimerSize[(sizeof(SecondaryDamageTimerView) == 0x14) ? 1 : -1];
typedef char SecondaryProjectileSize[(sizeof(SecondaryProjectileView) == 0x6c) ? 1 : -1];
typedef char SecondaryProjectileStateOffset[(offsetof(SecondaryProjectileView, state) == 0x48) ? 1 : -1];
typedef char SecondaryProjectileParametersOffset[(offsetof(SecondaryProjectileView, parameters) == 0x64) ? 1 : -1];
typedef char SecondaryProjectileCallbackOffset[(offsetof(SecondaryProjectileParameters, onHit) == 0x30) ? 1 : -1];
typedef char SecondaryDamageRegionSize[(sizeof(SecondaryDamageRegionView) == 0x74) ? 1 : -1];
typedef char SecondaryDamageRegionTimerOffset[(offsetof(SecondaryDamageRegionView, time) == 0x4c) ? 1 : -1];
typedef char SecondaryDamageProjectileOffset[(offsetof(SecondaryDamageReferencesView, projectiles) == 0x96c) ? 1 : -1];
typedef char SecondaryDamageRegionsOffset[(offsetof(SecondaryDamageReferencesView, regions) == 0x7c9c) ? 1 : -1];
typedef char SecondaryCollisionShapeOffset[(offsetof(SecondaryCollisionReferencesView, shape) == 0x92c) ? 1 : -1];
typedef char SecondaryDamageEnemyRadiusOffset[(offsetof(SecondaryDamageEnemyView, radius) == 0x490) ? 1 : -1];
