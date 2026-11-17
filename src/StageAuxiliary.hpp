#pragma once

#include "StageResource.hpp"
#include "AnimationVmId.hpp"
#include <stddef.h>

// Physical 0x4A0-byte auxiliary stage object used by the gameplay and
// background-effect callbacks. Its native constructor first constructs two
// timers and one embedded animation VM, then clears the entire object.
struct StageAuxiliaryView
{
    unsigned int flags;
    unsigned int unknown004;
    ChainElement *updateChain;
    ChainElement *drawChain;
    unsigned int unknown010;
    StageTimerView timer014;
    StageTimerView timer028;
    int effectActive;
    unsigned int animationId040;
    unsigned int animationId044;
    StageEmbeddedVm vm;
    void *cachedData;
    VmBindingBlockView *bindings;
    CameraVector origin;
    float radius;
    float radiusStep;
    int transitionActive;
    ChainElement *overlayChain;

    StageAuxiliaryView();
    ~StageAuxiliaryView();
};

typedef char StageAuxiliarySize[(sizeof(StageAuxiliaryView) == 0x4a0) ? 1 : -1];
typedef char StageAuxiliaryVmOffset[(offsetof(StageAuxiliaryView, vm) == 0x48) ? 1 : -1];
typedef char StageAuxiliaryCacheOffset[(offsetof(StageAuxiliaryView, cachedData) == 0x47c) ? 1 : -1];
typedef char StageAuxiliaryBindingOffset[(offsetof(StageAuxiliaryView, bindings) == 0x480) ? 1 : -1];
typedef char StageAuxiliaryOriginOffset[(offsetof(StageAuxiliaryView, origin) == 0x484) ? 1 : -1];
typedef char StageAuxiliaryRadiusOffset[(offsetof(StageAuxiliaryView, radius) == 0x490) ? 1 : -1];
typedef char StageAuxiliaryTransitionOffset[(offsetof(StageAuxiliaryView, transitionActive) == 0x498) ? 1 : -1];
typedef char StageAuxiliaryOverlayOffset[(offsetof(StageAuxiliaryView, overlayChain) == 0x49c) ? 1 : -1];

// The auxiliary instance is published through the existing enemy/effect
// pointer at 0x004A8D64. The actor pointer is a separate global at 0x004A8EB4.
struct MeshEffectEnemyView;
extern MeshEffectEnemyView *g_MeshEffectEnemy;

int __stdcall RegisterStageAuxiliaryChains(StageAuxiliaryView *self);
StageAuxiliaryView *CreateStageAuxiliary();
void DestroyStageAuxiliary();
int __fastcall StageAuxiliaryUpdateCallback(StageAuxiliaryView *self);
int __fastcall StageAuxiliaryDrawCallback(void *context);
int __stdcall UpdateStageAuxiliary(StageAuxiliaryView *self);
