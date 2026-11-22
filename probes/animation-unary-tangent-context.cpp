#include "AnimationExecution.hpp"

extern "C" float __stdcall AnimationUnaryTangentProbe(float value)
{
    return AnimationUnaryMathC(value);
}
