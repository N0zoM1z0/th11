#include "SecondaryWrappedAngle.hpp"

// Both arguments are dynamic inputs; no constant-specialized body is useful.
extern "C" float __stdcall SecondaryWrappedAngleProbe(float target, float current)
{
    return SecondaryWrappedAngleDifference(target, current);
}
