#include "SecondaryAngleDifference40B830.hpp"

// Keep both floats dynamic in the original VC9 LTCG profile.
extern "C" float __stdcall SecondaryAngle40B830Probe(float target, float current)
{
    return SecondaryAngleDifference40B830(target, current);
}
