#include "AnimationImmediateDraw.hpp"
#include <math.h>

float __stdcall AnimationExtentMagnitude(float value)
{
    __declspec(align(8)) volatile float magnitude = static_cast<float>(fabs(static_cast<double>(value)));
    return magnitude;
}
