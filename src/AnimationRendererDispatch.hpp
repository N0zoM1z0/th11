#pragma once

#include "AnimationRendererLifecycle.hpp"
#include "AnimationVmFactory.hpp"

int __stdcall UpdateAnimationRenderer(AnimationRenderer *self);
int __stdcall UpdateOtherAnimationRenderer(AnimationRenderer *self);
int __stdcall DrawAnimationRendererLayer(AnimationRenderer *self, int layer);
// AllocateSceneAnimationVm's existing no-argument boundary is implemented here.
