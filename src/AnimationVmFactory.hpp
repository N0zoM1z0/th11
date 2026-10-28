#pragma once

#include "AnimationVm.hpp"
#include "AnimationVmId.hpp"

AnimationVmId __stdcall PublishSceneAnimationVm(AnimationVmView *vm);
AnimationVmId __stdcall CreateAnimationVmId(AnimationFileView *file, int script, int layer);
void __stdcall InitializeSceneAnimationVm(AnimationVmView *vm, AnimationFileView *file, int script);
void __stdcall BindAnimationFileScript(AnimationFileView *file, int script, AnimationVmView *vm);

// Reviewed external VM-pool allocation boundary.
AnimationVmView *__stdcall AllocateSceneAnimationVm();
