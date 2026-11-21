#include "AnimationImmediateDraw.hpp"

extern "C" int __stdcall AnimationExtentMagnitudeProbe(float value)
{
    volatile float magnitude = AnimationExtentMagnitude(value);
    return magnitude > 0.0f;
}
