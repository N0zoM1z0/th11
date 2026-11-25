#pragma once

#include "StageTweenTypes.hpp"

struct AnimationIntVector
{
    int x, y, z;
};

// Two-component value; reviewed hidden-result contract at 0x44E910/0x44DBEC.
struct AnimationFloatPair
{
    float x, y;
    AnimationFloatPair() {}
    AnimationFloatPair(float xValue, float yValue) : x(xValue), y(yValue) {}
};
typedef char AnimationFloatPairSize[(sizeof(AnimationFloatPair) == 8) ? 1 : -1];

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
