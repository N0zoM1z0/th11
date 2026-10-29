#pragma once

#include "AnimationImmediateDraw.hpp"
#include "Random.hpp"

struct AnimationColorVertex
{
    float x, y, z, rhw;
    unsigned int color;
};
struct AnimationPolarVector
{
    float x, y;
    // Actual reviewed 4526A0 boundary; its paired FSINCOS body remains open.
    AnimationPolarVector &SetPolar(float angle, float radius);
};

extern RandomStateView g_TextRandomState;
int __stdcall DrawAnimationQuadVertices(RenderBatchView *renderer, AnimationVmView *vm,
    const RenderVertex *quad);
int __stdcall DrawAnimationColoredStrip(RenderBatchView *renderer, AnimationVmView *vm,
    const AnimationColorVertex *vertices, unsigned int count);
int __stdcall DrawAnimationColoredFan(RenderBatchView *renderer, AnimationVmView *vm,
    const AnimationColorVertex *vertices, unsigned int count);
unsigned int __stdcall IsAnimationVmActive(AnimationVmView *vm);
float __stdcall AnimationVectorLength(const CameraVector *value);
int __stdcall InitializeFanAnimationVm(AnimationVmView *vm);
int __fastcall UpdateFanAnimationVm(AnimationVmView *vm);
int __fastcall DrawFanAnimationVm(AnimationVmView *vm);
