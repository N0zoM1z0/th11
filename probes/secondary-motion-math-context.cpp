// Independent input; this diagnostic image is never executed.
#include "SecondaryMotionMath.hpp"
extern "C" void __stdcall SecondaryMotionMathProbe(SecondaryMotionView *motion,
    SecondaryVector2 *output, const SecondaryVector2 *input, float angle)
{
    QuantizeSecondaryMotionPosition(motion);
    RotateSecondaryVector(output, input, angle);
}
