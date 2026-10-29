#pragma once
#include "SecondaryPattern.hpp"

// Physical controller fields; original class identity and total size are open.
struct SecondaryPlayerControllerView
{
    unsigned char unknown000[0x14];
    AnimationVmView animation;
    unsigned char unknown448[0x87c - 0x448];
    CameraVector position;
    int x, y;
    unsigned char unknown890[0x8cc - 0x890];
    CameraVector minimum, maximum, halfSize, size, otherSize, savedPosition;
    unsigned char unknown914[0x920 - 0x914];
    int fireCount, unknown924, mode;
    SecondaryPatternConfigurationView *configuration;
    StageTimerView patternTime, stateTime, auxiliaryTime;
    unsigned char unknown96c[0x7c9c - 0x96c];
    SecondaryDamageRegionView regions[32];
    unsigned char unknown8b1c[0x8bb0 - 0x8b1c];
    StageTimerView statusTime;
    unsigned int flags, unknown8bc8;
    CameraVector scaledMinimum, scaledMaximum, otherMinimum, otherMaximum, fullMinimum, fullMaximum;
    float angle;
    unsigned char unknown8c18[0x8d20 - 0x8c18];
    int focused;
};
struct SecondaryControllerHudView
{
    unsigned char unknown000[0x4420];
    unsigned int flags;
    unsigned char unknown4424[0x4438 - 0x4424];
    int blocked;
};
struct SecondaryControllerHostView
{
    unsigned char unknown000[0x10];
    int deaths, unknown014, deathState;
    unsigned char unknown01c[0x70 - 0x1c];
    int enabled;
};
struct SecondaryControllerModeView
{
    unsigned char unknown000[0x10];
    int mode;
};
struct SecondaryControllerDropManager;
extern SecondaryControllerModeView *g_SecondaryControllerMode;
extern SecondaryControllerDropManager *g_SecondaryControllerDrops;
extern int g_SecondaryControllerGauge, g_SecondaryPlayerLives, g_SecondaryPlayerValue;

int __stdcall UpdateSecondaryPlayerController(SecondaryDamageReferencesView *self);
int __fastcall UpdateSecondaryPlayerCallback(SecondaryDamageReferencesView *self);
int __fastcall DrawSecondaryPlayerCallback(SecondaryDamageReferencesView *self);
void __stdcall RefreshSecondaryPlayerEmitters(SecondaryDamageReferencesView *self);
float __stdcall SecondaryPlayerAimAngle(SecondaryDamageReferencesView *self, const CameraVector *position);
float __stdcall AngleToSecondaryPlayer(const CameraVector *position);

// Real target callees; private register allocation is not encoded as a fake ABI.
int __stdcall ActivateSecondaryPlayerSpecial();
int __stdcall ClearSceneProjectilesInRadius(const CameraVector *position, float radius, int effects, int filter);
void __stdcall ClearSceneProjectiles(int filter);
int __stdcall ClearSecondaryObjectsInRadius(const CameraVector *position, float radius, int mode, int flags);
int __stdcall ClearSecondaryObjects(int mode, int flags);
int __stdcall SpawnSecondaryPlayerDrop(SecondaryControllerDropManager *manager,
    const CameraVector *position, int kind, unsigned int color, float angle, float speed);
void __stdcall EnterSecondaryContinueMenu();
void __stdcall EnterSecondaryGameOverMenu();
void __stdcall UpdateSecondaryPlayerMovement(SecondaryDamageReferencesView *self);
void __stdcall BeginSecondaryPlayerDeath(SecondaryDamageReferencesView *self);
void __stdcall UpdateSecondaryControllerAnimation(AnimationVmView *vm);

typedef char SecondaryControllerAnimationOffset[(offsetof(SecondaryPlayerControllerView, animation) == 0x14) ? 1 : -1];
typedef char SecondaryControllerPositionOffset[(offsetof(SecondaryPlayerControllerView, position) == 0x87c) ? 1 : -1];
typedef char SecondaryControllerBoundsOffset[(offsetof(SecondaryPlayerControllerView, minimum) == 0x8cc) ? 1 : -1];
typedef char SecondaryControllerSavedPosition[(offsetof(SecondaryPlayerControllerView, savedPosition) == 0x908) ? 1 : -1];
typedef char SecondaryControllerStateTime[(offsetof(SecondaryPlayerControllerView, stateTime) == 0x944) ? 1 : -1];
typedef char SecondaryControllerRegionsOffset[(offsetof(SecondaryPlayerControllerView, regions) == 0x7c9c) ? 1 : -1];
typedef char SecondaryControllerStatusOffset[(offsetof(SecondaryPlayerControllerView, statusTime) == 0x8bb0) ? 1 : -1];
typedef char SecondaryControllerScaledBounds[(offsetof(SecondaryPlayerControllerView, scaledMinimum) == 0x8bcc) ? 1 : -1];
typedef char SecondaryControllerAngleOffset[(offsetof(SecondaryPlayerControllerView, angle) == 0x8c14) ? 1 : -1];
typedef char SecondaryControllerFocusOffset[(offsetof(SecondaryPlayerControllerView, focused) == 0x8d20) ? 1 : -1];
