#pragma once

#include "AnimationProjectedQuad.hpp"

struct AnimationWorldVertex
{
    CameraVector position;
    float u, v;
};
struct AnimationTransformVector { float x, y, z, w; };
extern "C" AnimationTransformVector *WINAPI D3DXVec3Transform(
    AnimationTransformVector *output, const CameraVector *input, const D3DMATRIX *matrix);

float __stdcall AnimationExtentMagnitude(float value);
void __stdcall ApplyAnimationVmWorldState(RenderBatchView *renderer, AnimationVmView *vm);
int __stdcall DrawWorldAnimationVm(RenderBatchView *renderer, AnimationVmView *vm);
int __stdcall DrawFoggedProjectedAnimationVm(RenderBatchView *renderer, AnimationVmView *vm);
int __stdcall DrawAnimationVertexStrip(RenderBatchView *renderer, AnimationVmView *vm,
    const RenderVertex *vertices, unsigned int vertexCount);
int __stdcall DrawAnimationVertexFan(RenderBatchView *renderer, AnimationVmView *vm,
    const RenderVertex *vertices, unsigned int vertexCount);
int __stdcall UpdateStripVertexUv(AnimationVmView *vm, RenderVertex *vertices, int count);
int __stdcall UpdateTransposedStripVertexUv(AnimationVmView *vm, RenderVertex *vertices, int count);
int __stdcall DrawAnimationVmByMode(RenderBatchView *renderer, AnimationVmView *vm);
