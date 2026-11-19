#include <math.h>

// The native cosine wrapper rounds x87 ST0 through an eight-byte-aligned
// floating-point local before returning the float value.
float __stdcall SecondaryUnaryMathB(float value)
{
    __declspec(align(8)) float rounded = static_cast<float>(cos(static_cast<double>(value)));
    return rounded;
}
