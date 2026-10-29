#pragma once

#include "AsciiLifecycle.hpp"
#include "AnimationSpriteBinding.hpp"
#include "PerspectiveCamera.hpp"
#include "RenderBatch.hpp"

// Reviewed renderer call boundaries; these bodies remain external.
void __stdcall BuildAnimationVmQuad(AnimationVmView *vm, CameraVector *first,
    CameraVector *second, CameraVector *third, CameraVector *fourth);
int __stdcall DrawAnimationVm(RenderBatchView *renderer, AnimationVmView *vm,
    unsigned char flags);
extern RenderVertex g_AnimationQuad[4];
