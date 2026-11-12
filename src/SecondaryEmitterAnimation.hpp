#pragma once
#include "AnimationVmFactory.hpp"

AnimationVmId __stdcall PublishSecondaryEmitterAnimationVm(AnimationVmView *vm);
AnimationVmId __stdcall CreateSecondaryEmitterAnimationVmId(AnimationFileView *file, int script, int layer);
AnimationVmId __stdcall PublishSecondaryDrawAnimationVm(AnimationVmView *vm);
AnimationVmId __stdcall CreateSecondaryDrawAnimationVmId(AnimationFileView *file, int script, int layer);

// Independently reviewed prepend owner and its default/explicit-layer factories.
AnimationVmId __stdcall PrependSecondaryDrawAnimationVm(AnimationVmView *vm);
AnimationVmId __stdcall CreateDefaultFrontSecondaryDrawAnimationVmId(AnimationFileView *file, int script);
AnimationVmId __stdcall CreateFrontSecondaryDrawAnimationVmId(AnimationFileView *file, int script, int layer);
