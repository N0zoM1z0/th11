#include <math.h>

// The native wrapper passes one float on the stack to the VC9 x87 cosine
// intrinsic, then rounds the returned ST0 value back to float.
float __stdcall SecondaryUnaryMathB(float value)
{
    return static_cast<float>(cos(static_cast<double>(value)));
}
