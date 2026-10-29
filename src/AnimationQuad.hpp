#pragma once
#include "AnimationVm.hpp"
#include "Camera.hpp"
#include "RenderBatch.hpp"

void __stdcall BuildAnimationVmQuad(AnimationVmView *vm, CameraVector *first,
    CameraVector *second, CameraVector *third, CameraVector *fourth);
float __stdcall FloorAnimationCoordinate(float value);
unsigned int __stdcall ScaleAnimationColorChannel(unsigned char value, unsigned char scale);
void __stdcall ApplyAnimationVmDrawState(RenderBatchView *renderer, AnimationVmView *vm);
int __stdcall AppendRenderQuad(RenderBatchView *renderer, const RenderVertex *quad);
