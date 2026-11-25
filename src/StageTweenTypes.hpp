#pragma once

#include "Camera.hpp"
#include "StageResource.hpp"

struct StageVectorTween
{
    CameraVector initial, target, control1, control2;
    StageTimerView time;
    int duration, mode;
};

struct StageFogView
{
    float start, end;
    float channels[4];
    unsigned int packed;

    void Pack();
    StageFogView operator+(const StageFogView &other) const;
    StageFogView operator-(const StageFogView &other) const;
    StageFogView operator*(float scale) const;
};

struct StageFogTween
{
    StageFogView initial, target, control1, control2;
    StageTimerView time;
    int duration, mode;
};

typedef char StageVectorTweenSize[(sizeof(StageVectorTween) == 0x4c) ? 1 : -1];
typedef char StageFogSize[(sizeof(StageFogView) == 0x1c) ? 1 : -1];
