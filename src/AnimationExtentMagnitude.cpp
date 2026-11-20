#include "AnimationImmediateDraw.hpp"
#include <math.h>

extern "C" float __stdcall AnimationExtentMagnitude(float value)
{
    return static_cast<float>(fabs(static_cast<double>(value)));
}
