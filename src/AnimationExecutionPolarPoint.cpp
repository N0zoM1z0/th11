#include "AnimationExecution.hpp"
#include <math.h>

#pragma optimize("y", on)
void AnimationExecutionPolarPoint::Set(float angle, float radius)
{
    // Keep the ECX thiscall receiver in a volatile stack slot across CRT calls.
    float * volatile output = &x;
    output[0] = static_cast<float>(cos(static_cast<double>(angle)) * radius);
    output[1] = static_cast<float>(sin(static_cast<double>(angle)) * radius);
}
#pragma optimize("y", off)
