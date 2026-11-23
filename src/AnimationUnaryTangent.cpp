#include "AnimationExecution.hpp"
#include <math.h>

#pragma optimize("y", on)

// Match the native x87 tangent wrapper's float rounding through an aligned
// local before returning the result to the animation VM.
float __fastcall AnimationUnaryMathC(float value)
{
    __declspec(align(8)) float rounded = static_cast<float>(tan(static_cast<double>(value)));
    return rounded;
}
#pragma optimize("y", off)
