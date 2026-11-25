#pragma once

#include "StageTween.hpp"

#include "AnimationTweenTypes.hpp"

AnimationIntVector __stdcall StepAnimationIntVectorTween(AnimationIntVectorTween *self);
int __stdcall StepAnimationIntScalarTween(AnimationIntScalarTween *self);
AnimationFloatPair __stdcall StepAnimationFloatPairTween(AnimationFloatPairTween *self);
float __stdcall StepAnimationFloatScalarTween(AnimationFloatScalarTween *self);
float __stdcall AnimationIntVectorTweenFactor(AnimationIntVectorTween *self);
float __stdcall AnimationIntScalarTweenFactor(AnimationIntScalarTween *self);
float __stdcall AnimationFloatPairTweenFactor(AnimationFloatPairTween *self);
float __stdcall AnimationFloatScalarTweenFactor(AnimationFloatScalarTween *self);
void __stdcall ResetAnimationScalarTweenTime(AnimationScalarTweenTimeView *self);

int __stdcall AnimationIntVectorTweenDuration(AnimationIntVectorTween *self);
int __stdcall AnimationFloatPairTweenDuration(AnimationFloatPairTween *self);
int __stdcall AnimationFloatScalarTweenDuration(AnimationFloatScalarTween *self);
void __stdcall SetAnimationFloatScalarTweenMode(AnimationFloatScalarTween *self, int mode);
void __stdcall SetAnimationFloatScalarTweenDuration(AnimationFloatScalarTween *self, int duration);
void __stdcall SetAnimationFloatScalarTweenInitial(AnimationFloatScalarTween *self, const float *value);
void __stdcall SetAnimationFloatScalarTweenTarget(AnimationFloatScalarTween *self, const float *value);
void __stdcall SetAnimationFloatScalarTweenControl1(AnimationFloatScalarTween *self, const float *value);
void __stdcall SetAnimationFloatScalarTweenControl2(AnimationFloatScalarTween *self, const float *value);
