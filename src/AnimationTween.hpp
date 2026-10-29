#pragma once

#include "StageTween.hpp"

struct AnimationIntVector { int x, y, z; };
struct AnimationFloatPair { float x, y; };
struct AnimationIntVectorTween
{
    AnimationIntVector initial, target, control1, control2;
    StageTimerView time;
    int duration, mode;
};
struct AnimationIntScalarTween
{
    int initial, target, control1, control2;
    StageTimerView time;
    int duration, mode;
};
struct AnimationFloatPairTween
{
    AnimationFloatPair initial, target, control1, control2;
    StageTimerView time;
    int duration, mode;
};
struct AnimationFloatScalarTween
{
    float initial, target, control1, control2;
    StageTimerView time;
    int duration, mode;
};
// Shared physical timer prefix of both scalar layouts; value types are unproved here.
struct AnimationScalarTweenTimeView
{
    unsigned char values[16];
    StageTimerView time;
};

typedef char AnimationIntVectorTweenSize[(sizeof(AnimationIntVectorTween) == 0x4c) ? 1 : -1];
typedef char AnimationIntScalarTweenSize[(sizeof(AnimationIntScalarTween) == 0x2c) ? 1 : -1];
typedef char AnimationFloatPairTweenSize[(sizeof(AnimationFloatPairTween) == 0x3c) ? 1 : -1];
typedef char AnimationFloatScalarTweenSize[(sizeof(AnimationFloatScalarTween) == 0x2c) ? 1 : -1];

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
