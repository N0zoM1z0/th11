#include "SecondaryAngleDifference40B830.hpp"

// This 101-byte owner is another independent copy of the original game's
// float-rounded signed angle difference, with at most one turn correction.
float __stdcall SecondaryAngleDifference40B830(float target, float current)
{
    if (target - current > static_cast<double>(3.1415927f))
        return static_cast<float>(target - (current + static_cast<double>(6.2831855f)));
    if (current - target > static_cast<double>(3.1415927f))
        return static_cast<float>(target - (current - static_cast<double>(6.2831855f)));
    return target - current;
}
