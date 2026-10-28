#pragma once

#include "StageScript.hpp"

float __stdcall StageVectorTweenFactor(StageVectorTween *self);
float __stdcall StageFogTweenFactor(StageFogTween *self);
// The dispatcher requires a curve in [0, 15], as its unchecked native table does.
float __stdcall EvaluateStageEasing(int curve, float progress);
