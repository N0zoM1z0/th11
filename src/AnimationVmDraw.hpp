#pragma once
#include "AnimationQuad.hpp"
#include "AnimationDocument.hpp"

extern RenderVertex g_AnimationQuad[4];
int __stdcall DrawAnimationVm(RenderBatchView *renderer, AnimationVmView *vm,
    unsigned char flags);
int __stdcall DrawUnrotatedAnimationVm(RenderBatchView *renderer, AnimationVmView *vm);
void __stdcall BuildUnroundedAnimationVmQuad(AnimationVmView *vm, CameraVector *first,
    CameraVector *second, CameraVector *third, CameraVector *fourth);
int __stdcall DrawUnroundedAnimationVm(RenderBatchView *renderer, AnimationVmView *vm);
int __stdcall DrawAdaptiveAnimationVm(RenderBatchView *renderer, AnimationVmView *vm);

// Reviewed 0x4503D0 external boundary; the571-byte rotated body is still open.
void __stdcall BuildRotatedAnimationVmQuad(AnimationVmView *vm, CameraVector *first,
    CameraVector *second, CameraVector *third, CameraVector *fourth);
