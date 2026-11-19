#include "SecondaryAngleDifference4287A0.hpp"

// One float-rounded signed angular difference, corrected by at most one turn.
// The native 0x4287A0 owner has three separate RET 8 return paths.
float __stdcall SecondaryAngleDifference4287A0(float target, float current)
{
    if (target - current > static_cast<double>(3.1415927f))
        return static_cast<float>(target - (current + static_cast<double>(6.2831855f)));
    if (current - target > static_cast<double>(3.1415927f))
        return static_cast<float>(target - (current - static_cast<double>(6.2831855f)));
    return target - current;
}
