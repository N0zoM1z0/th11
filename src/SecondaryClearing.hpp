#pragma once
#include "SecondaryPlayerController.hpp"

// Physical views only; original pool/object classes and total manager sizes are open.
struct SceneClearProjectileView
{
    unsigned int flags, kind;
    AnimationVmView animation;
    CameraVector position;
    unsigned char unknown448[0x45c - 0x448];
    float width;
    unsigned int unknown460;
    StageTimerView clearTime;
    unsigned char unknown478[0x4a4 - 0x478];
    int clearScript;
    unsigned char unknown4a8[0x4b2 - 0x4a8];
    unsigned short state;
    unsigned char unknown4b4[0x910 - 0x4b4];
};
struct SceneClearPoolView
{
    unsigned char unknown000[0x64];
    SceneClearProjectileView projectiles[2000];
};
struct SecondaryClearObjectView
{
    // Uncalled slots retain only their observed positions, not original signatures.
    virtual void Unknown00() = 0;
    virtual void Unknown04() = 0;
    virtual void Unknown08() = 0;
    virtual void Unknown0c() = 0;
    virtual void Unknown10() = 0;
    virtual int Clear(int mode, int flags) = 0;
    virtual void Unknown18() = 0;
    virtual int ClearInRadius(const CameraVector *position, float radius, int mode, int flags) = 0;
    unsigned int unknown004;
    SecondaryClearObjectView *next;
    int state;
};
struct SecondaryClearManagerView
{
    unsigned char unknown000[0x18];
    SecondaryClearObjectView *first;
    unsigned char unknown01c[0x45c - 0x1c];
    CameraVector position;
};
extern SceneClearPoolView *g_SceneClearPool;
extern SecondaryClearManagerView *g_SecondaryClearManager;

int __stdcall ClearSceneProjectile(SceneClearProjectileView *projectile);
int __stdcall SceneClearOutside(const CameraVector *position, float halfWidth, float halfHeight);
void __stdcall ClearSecondaryActorsInRadius(const CameraVector *position, float radius, int effects);

typedef char SceneClearProjectileSize[(sizeof(SceneClearProjectileView) == 0x910) ? 1 : -1];
typedef char SceneClearAnimationOffset[(offsetof(SceneClearProjectileView, animation) == 8) ? 1 : -1];
typedef char SceneClearPositionOffset[(offsetof(SceneClearProjectileView, position) == 0x43c) ? 1 : -1];
typedef char SceneClearTimerOffset[(offsetof(SceneClearProjectileView, clearTime) == 0x464) ? 1 : -1];
typedef char SceneClearStateOffset[(offsetof(SceneClearProjectileView, state) == 0x4b2) ? 1 : -1];
typedef char SceneClearPoolOffset[(offsetof(SceneClearPoolView, projectiles) == 0x64) ? 1 : -1];
typedef char SecondaryClearNextOffset[(offsetof(SecondaryClearObjectView, next) == 8) ? 1 : -1];
typedef char SecondaryClearStateOffset[(offsetof(SecondaryClearObjectView, state) == 0xc) ? 1 : -1];
typedef char SecondaryClearPositionOffset[(offsetof(SecondaryClearManagerView, position) == 0x45c) ? 1 : -1];
