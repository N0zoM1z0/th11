#pragma once
#include "AnimationVmFactory.hpp"

AnimationVmId __stdcall PublishSecondaryEmitterAnimationVm(AnimationVmView *vm);
AnimationVmId __stdcall CreateSecondaryEmitterAnimationVmId(AnimationFileView *file, int script, int layer);
AnimationVmId __stdcall PublishSecondaryDrawAnimationVm(AnimationVmView *vm);
AnimationVmId __stdcall CreateSecondaryDrawAnimationVmId(AnimationFileView *file, int script, int layer);
