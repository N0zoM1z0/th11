#include "StageTween.hpp"

extern "C" __declspec(dllexport) float __stdcall StageEasingProbe(int curve, float progress)
{
    return EvaluateStageEasing(curve, progress);
}
