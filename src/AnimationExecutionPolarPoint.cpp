#include "AnimationExecution.hpp"
#include <math.h>

void AnimationExecutionPolarPoint::Set(float angle, float radius)
{
    x = static_cast<float>(cos(static_cast<double>(angle)) * radius);
    y = static_cast<float>(sin(static_cast<double>(angle)) * radius);
}
