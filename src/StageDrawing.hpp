#pragma once

#include "StageResource.hpp"
#include "AnimationVm.hpp"
#include "RenderBatch.hpp"
#include "MainRuntime.hpp"

// Observed renderer tail, separate from its much earlier batch storage.
struct StageRenderTintView
{
    unsigned char unknown000[0x7bd88c];
    unsigned int tint;
    int enabled;
};

typedef char StageRenderTintOffset[(offsetof(StageRenderTintView, enabled) == 0x7bd890) ? 1 : -1];

// Reviewed external boundaries; these declarations do not supply their bodies.
void __stdcall SelectRuntimeCamera(MainRuntimeView *self, int index);
int __stdcall DrawAnimationVm(RenderBatchView *renderer, AnimationVmView *vm);
int __stdcall DrawAnimationGroup(RenderBatchView *renderer, int group);
void __stdcall DrawStageLayer(StageResourceView *self, int layer);
void *__stdcall CreateStageTransition(int kind, int duration, int x, int y, int priority);
void __stdcall SetStageTimer(StageTimerView *self, int frame);
void __stdcall AddStageTimer(StageTimerView *self, float amount);
