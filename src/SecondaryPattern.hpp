#pragma once
#include "SecondaryProjectile.hpp"
#include "SecondaryIntegerPair.hpp"

struct SecondaryEmitterFloatPair
{
    float x, y;
};

// Coordinate prefix returned by the helper, at containing record+5C.
struct SecondaryEmitterView
{
    int x, y;
    unsigned char unknown008[0x4c - 8];
    float angle;
    unsigned int unknown050, id;
};

struct SecondaryEmitterObjectView
{
    unsigned int state;
    unsigned char unknown004[0x1c - 4];
    SecondaryEmitterFloatPair polar[2];
    unsigned char unknown02c[0x54 - 0x2c];
    int pendingX, pendingY, x, y;
    SecondaryIntegerPair offsets[7];
    unsigned char unknown09c[0xa8 - 0x9c];
    float angle;
    unsigned int unknown0ac, idB0, idB4;
    unsigned char unknown0b8[0xd0 - 0xb8];
    int index, refresh;
    unsigned int flags, unknown0dc;
    int (__fastcall *onDraw)(SecondaryEmitterObjectView *self);
};

struct SecondaryPatternConfigurationView
{
    unsigned short unknown000, patternCount;
    unsigned char unknown004[0x20 - 4];
    int levelCount;

    __forceinline SecondaryProjectileParameters *&Pattern(int index)
    {
        return *reinterpret_cast<SecondaryProjectileParameters **>(
            reinterpret_cast<unsigned char *>(this) + 0x268 + static_cast<unsigned int>(index) * 8);
    }
};

// Additional physical prefix; the complete original player owner is unresolved.
struct SecondaryPatternOwnerView
{
    unsigned char unknown000[0x87c];
    CameraVector position;
    int x, y;
    unsigned char unknown890[0x920 - 0x890];
    int fireCount, unknown924, mode;
    SecondaryPatternConfigurationView *configuration;
    StageTimerView time;
    unsigned char unknown944[0x7570 - 0x944];
    SecondaryEmitterObjectView emitters[8];
    unsigned char unknown7c90[0x8bac - 0x7c90];
    int multiplier;
    unsigned char unknown8bb0[0x8c14 - 0x8bb0];
    float angle8c14;
    unsigned char unknown8c18[0x8d20 - 0x8c18];
    int focused;
};

typedef int (__fastcall *SecondarySpawnFunction)(SecondaryDamageReferencesView *,
    SecondaryProjectileView *, unsigned int);
typedef int (__fastcall *SecondaryUpdateFunction)(SecondaryDamageReferencesView *,
    SecondaryProjectileView *);
typedef int (__fastcall *SecondaryHitFunction)(SecondaryDamageReferencesView *,
    SecondaryProjectileView *, const CameraVector *);
extern SecondarySpawnFunction g_SecondaryProjectileSpawnCallbacks[];
extern SecondaryUpdateFunction g_SecondaryProjectileUpdateCallbacks[];
extern unsigned int g_SecondaryProjectileOtherCallbacks[];
extern SecondaryHitFunction g_SecondaryProjectileHitCallbacks[];
extern int g_SecondaryPatternPower, g_SecondaryPatternPowerUnit;
extern int g_SecondaryPatternMaximumPower;
extern int g_SecondaryEmitterOffsetIndices[], g_SecondaryEmitterScripts[];

int __stdcall LoadSecondaryProjectilePatterns(SecondaryDamageReferencesView *self, const char *filename);
int __stdcall EmitSecondaryPatternFrame(SecondaryDamageReferencesView *self, int frame);
int __stdcall UpdateSecondaryPattern(SecondaryDamageReferencesView *self);
int __stdcall DrawSecondaryPlayerEmitters(SecondaryDamageReferencesView *self);
void __stdcall ResetSecondaryPlayerEmitters(SecondaryDamageReferencesView *self);
void __cdecl ResetSecondaryPatternSlot();
int __fastcall SecondaryEmitterOrbitCallback(SecondaryEmitterObjectView *self);
int __fastcall SecondaryEmitterAimCallback(SecondaryEmitterObjectView *self);

SecondaryEmitterView *__stdcall GetSecondaryEmitter(SecondaryDamageReferencesView *self, int index);
unsigned int *__stdcall CopySecondaryEmitterId(unsigned int *output,
    SecondaryDamageReferencesView *self, int index);
int __stdcall SpawnSecondaryProjectile(SecondaryDamageReferencesView *self,
    SecondaryProjectileParameters *parameters, const CameraVector *position, unsigned int context);

typedef char SecondaryEmitterAngleOffset[(offsetof(SecondaryEmitterView, angle) == 0x4c) ? 1 : -1];
typedef char SecondaryEmitterIdOffset[(offsetof(SecondaryEmitterView, id) == 0x54) ? 1 : -1];
typedef char SecondaryEmitterObjectSize[(sizeof(SecondaryEmitterObjectView) == 0xe4) ? 1 : -1];
typedef char SecondaryEmitterObjectCoordinates[(offsetof(SecondaryEmitterObjectView, x) == 0x5c) ? 1 : -1];
typedef char SecondaryEmitterObjectOffsets[(offsetof(SecondaryEmitterObjectView, offsets) == 0x64) ? 1 : -1];
typedef char SecondaryEmitterObjectIndex[(offsetof(SecondaryEmitterObjectView, index) == 0xd0) ? 1 : -1];
typedef char SecondaryEmitterObjectCallback[(offsetof(SecondaryEmitterObjectView, onDraw) == 0xe0) ? 1 : -1];
typedef char SecondaryPatternEmittersOffset[(offsetof(SecondaryPatternOwnerView, emitters) == 0x7570) ? 1 : -1];
typedef char SecondaryPatternConfigurationCount[(offsetof(SecondaryPatternConfigurationView, levelCount) == 0x20) ? 1 : -1];
typedef char SecondaryPatternConfigurationOffset[(offsetof(SecondaryPatternOwnerView, configuration) == 0x92c) ? 1 : -1];
typedef char SecondaryPatternMultiplierOffset[(offsetof(SecondaryPatternOwnerView, multiplier) == 0x8bac) ? 1 : -1];
typedef char SecondaryPatternFocusOffset[(offsetof(SecondaryPatternOwnerView, focused) == 0x8d20) ? 1 : -1];
