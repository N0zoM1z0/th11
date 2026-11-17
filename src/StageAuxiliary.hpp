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
    unsigned char unknown484[0x49c - 0x484];
    ChainElement *overlayChain;

    StageAuxiliaryView();
    ~StageAuxiliaryView();
};

typedef char StageAuxiliarySize[(sizeof(StageAuxiliaryView) == 0x4a0) ? 1 : -1];
typedef char StageAuxiliaryVmOffset[(offsetof(StageAuxiliaryView, vm) == 0x48) ? 1 : -1];
typedef char StageAuxiliaryCacheOffset[(offsetof(StageAuxiliaryView, cachedData) == 0x47c) ? 1 : -1];
typedef char StageAuxiliaryBindingOffset[(offsetof(StageAuxiliaryView, bindings) == 0x480) ? 1 : -1];
typedef char StageAuxiliaryOverlayOffset[(offsetof(StageAuxiliaryView, overlayChain) == 0x49c) ? 1 : -1];

// This view is rooted at the existing secondary mesh-effect singleton
// (0x004A8D64); do not invent a second global storage owner.
struct MeshEffectActorView;
extern MeshEffectActorView *g_MeshEffectActor;

int __stdcall RegisterStageAuxiliaryChains(StageAuxiliaryView *self);
StageAuxiliaryView *CreateStageAuxiliary();
void DestroyStageAuxiliary();
int __fastcall StageAuxiliaryUpdateCallback(StageAuxiliaryView *self);
int __fastcall StageAuxiliaryDrawCallback(void *context);
int __stdcall UpdateStageAuxiliary(StageAuxiliaryView *self);
