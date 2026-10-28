#pragma once

#include "StageScript.hpp"

float __stdcall StageVectorTweenFactor(StageVectorTween *self);
float __stdcall StageFogTweenFactor(StageFogTween *self);
// Curve dispatcher boundary; its complete code/data ownership remains open.
float __stdcall EvaluateStageEasing(int curve, float progress);
