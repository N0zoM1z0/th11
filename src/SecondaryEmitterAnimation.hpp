#pragma once
#include "AnimationVmFactory.hpp"

struct CameraVector;

AnimationVmId __stdcall PublishSecondaryEmitterAnimationVm(AnimationVmView *vm);
AnimationVmId __stdcall CreateSecondaryEmitterAnimationVmId(AnimationFileView *file, int script, int layer);
AnimationVmId __stdcall PublishSecondaryDrawAnimationVm(AnimationVmView *vm);
AnimationVmId __stdcall CreateSecondaryDrawAnimationVmId(AnimationFileView *file, int script, int layer);

// Independently reviewed prepend owner and its default/explicit-layer factories.
AnimationVmId __stdcall PrependSecondaryDrawAnimationVm(AnimationVmView *vm);
AnimationVmId __stdcall CreateDefaultFrontSecondaryDrawAnimationVmId(AnimationFileView *file, int script);
AnimationVmId __stdcall CreateFrontSecondaryDrawAnimationVmId(AnimationFileView *file, int script, int layer);

// Layer-zero position-preserving secondary-append draw factories.
AnimationVmId __stdcall CreatePositionedSecondaryDrawAnimationVmId(AnimationFileView *file, const CameraVector *position, int script);
AnimationVmId __stdcall CreateOffsetSecondaryDrawAnimationVmId(AnimationFileView *file, const CameraVector *position, int script);
