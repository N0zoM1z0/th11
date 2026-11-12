#include "SecondaryEffectPolar.hpp"
#include <math.h>
#pragma intrinsic(sin, cos)

void SecondaryEffectPolarVector::SetPolar(float angle, float width, float height)
{
    x = static_cast<float>(cos(angle) * width);
    y = static_cast<float>(sin(angle) * height);
}
