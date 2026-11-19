#include <math.h>

// The native 0x40B900 wrapper returns the rounded x87 atan2 result from
// an aligned floating-point local. The x87 intrinsic receives both floats.
float __stdcall SecondaryAngleMath(float y, float x)
{
    __declspec(align(8)) float result = static_cast<float>(atan2(y, x));
    return result;
}
