#pragma once

#include "AnimationVmDraw.hpp"

extern "C" CameraVector *WINAPI D3DXVec3Project(CameraVector *output,
    const CameraVector *input, const D3DVIEWPORT9 *viewport,
    const D3DMATRIX *projection, const D3DMATRIX *view, const D3DMATRIX *world);
extern "C" D3DMATRIX *WINAPI D3DXMatrixRotationX(D3DMATRIX *output, float angle);
extern "C" D3DMATRIX *WINAPI D3DXMatrixRotationY(D3DMATRIX *output, float angle);
extern "C" D3DMATRIX *WINAPI D3DXMatrixRotationZ(D3DMATRIX *output, float angle);
extern "C" D3DMATRIX *WINAPI D3DXMatrixMultiply(D3DMATRIX *output,
    const D3DMATRIX *left, const D3DMATRIX *right);

float __stdcall AnimationDistanceRoot(float squaredDistance);
int __stdcall BuildProjectedAnimationVmQuad(RenderBatchView *renderer, AnimationVmView *vm);
int __stdcall DrawProjectedAnimationVm(RenderBatchView *renderer, AnimationVmView *vm);
int __stdcall DrawBillboardAnimationVm(RenderBatchView *renderer, AnimationVmView *vm);
int __stdcall DrawFoggedBillboardAnimationVm(RenderBatchView *renderer, AnimationVmView *vm);
void __stdcall DrawAdaptiveUnroundedAnimationVm(RenderBatchView *renderer, AnimationVmView *vm);
void __stdcall BuildAnimationVmModeQuad(AnimationVmView *vm, CameraVector *quad);

// Complete target owner reviewed; paired FSINCOS source/codegen remains open.
int __stdcall BuildBillboardAnimationVmQuad(AnimationVmView *vm);
