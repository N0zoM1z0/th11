#include <math.h>

// 0x405FB0: the sine counterpart of SecondaryUnaryMathB, rounding x87 ST0
// through an eight-byte-aligned float local before returning it.
float __stdcall SecondaryUnaryMathA(float value)
{
    __declspec(align(8)) float rounded = static_cast<float>(sin(static_cast<double>(value)));
    return rounded;
}
