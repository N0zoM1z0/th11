#include "AnimationExecution.hpp"

void __stdcall SetAnimationUv(AnimationFloatPair *output, const CameraVector *point)
{
    output->x = static_cast<float>(point->x / 640.0);
    output->y = static_cast<float>(point->y / 480.0);
    if (output->x < 0.0f) output->x = 0.0f;
    if (output->y < 0.0f) output->y = 0.0f;
}
