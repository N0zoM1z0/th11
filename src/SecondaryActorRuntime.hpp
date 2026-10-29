#pragma once
#include "SecondaryActorState.hpp"
#include "StageMesh.hpp"

// Additional physical views of existing globals; complete original owners are open.
struct SecondaryActorSpellView
{
    unsigned char unknown000[0x88c];
    int age;
    unsigned char unknown890[0x8dc - 0x890];
    int phase;
    unsigned int flags, value8e4;
};
struct SecondaryActorEnemyView
{
    unsigned char unknown000[0x3c];
    int active;
};
struct SecondaryActorHudView
{
    unsigned char unknown000[0x4320];
    unsigned int flags;
    unsigned char unknown4324[0x4440 - 0x4324];
    int seconds, hundredths;
};
struct SecondaryActorFilesView
{
    unsigned char unknown000[0x46d674];
    AnimationFileView *file;
};
struct MeshEffectSpellView;
struct MeshEffectEnemyView;
extern MeshEffectSpellView *g_MeshEffectSpell;
extern MeshEffectEnemyView *g_MeshEffectEnemy;
extern int g_MeshEffectSelection0, g_MeshEffectSelection1;
extern SecondaryActorHudView *g_SecondaryActorHud;
extern SecondaryActorFilesView *g_SecondaryActorFiles;
extern int g_SecondaryActorDifficulty;
extern int g_SecondaryActorCollisionValues[];
extern CameraVector g_SecondaryActorSceneOffset;
extern int g_SecondaryActorScore, g_SecondaryCollisionCount, g_SecondaryCollisionValue;
extern StageTimerView g_SecondaryCollisionTimer;

static __forceinline SecondaryActorSpellView *SecondaryActorSpell()
{ return reinterpret_cast<SecondaryActorSpellView *>(g_MeshEffectSpell); }
static __forceinline SecondaryActorEnemyView *SecondaryActorEnemy()
{ return reinterpret_cast<SecondaryActorEnemyView *>(g_MeshEffectEnemy); }
static __forceinline AnimationFileView *SecondaryActorFile(unsigned int offset)
{
    return *reinterpret_cast<AnimationFileView **>(
        reinterpret_cast<unsigned char *>(g_SecondaryMaster) + offset);
}

struct SecondaryActorAnimationView
{
    unsigned char unknown000[0x2c];
    float angle;
    unsigned char unknown030[0x378 - 0x30];
    unsigned int color;
    short event;
    unsigned char unknown37e[0x3e8 - 0x37e];
    CameraVector position;
};

AnimationVmView *__stdcall ClearMissingAnimationId(unsigned int *id);
int __stdcall ValidateSecondaryAnimationId(unsigned int *id);
void __stdcall SignalSecondaryAnimation(unsigned int *id);
void __stdcall ReleaseSecondaryAnimation(unsigned int id);
void __stdcall PositionSecondaryAnimation(unsigned int id, const CameraVector *position);
void __stdcall PositionSecondaryAnimationVm(AnimationVmView *vm, const CameraVector *position);
void __stdcall PositionSecondaryAnimationRaw(unsigned int id, const CameraVector *position);
void __stdcall ChangeSecondaryAnimationScript(unsigned int *id, int script);
int __stdcall SecondaryGlobalStatusBits();
int __stdcall SampleSecondaryActorDamage(const CameraVector *position, const float *extent);
int __stdcall CheckSecondaryActorCollision(const CameraVector *position, float radius);
void __stdcall AddSecondaryActorScore(int value);
void __stdcall IncrementSecondaryCollisionCount();
void __stdcall AddSecondaryCollisionValue(int value);
void __stdcall QueueSecondaryActorSound(int sound, float x);

typedef char SecondaryActorSpellPhaseOffset[(offsetof(SecondaryActorSpellView, phase) == 0x8dc) ? 1 : -1];
typedef char SecondaryActorHudTimeOffset[(offsetof(SecondaryActorHudView, seconds) == 0x4440) ? 1 : -1];
typedef char SecondaryActorAnimationColorOffset[(offsetof(SecondaryActorAnimationView, color) == 0x378) ? 1 : -1];
typedef char SecondaryActorAnimationPositionOffset[(offsetof(SecondaryActorAnimationView, position) == 0x3e8) ? 1 : -1];
typedef char SecondaryActorHookOffset[(offsetof(SecondaryActorStateView, hook1630) == 0x1630) ? 1 : -1];
typedef char SecondaryActorReferencesPositionOffset[(offsetof(SecondaryActorReferencesView, position) == 0x87c) ? 1 : -1];
