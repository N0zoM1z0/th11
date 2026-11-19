#include "SecondaryWrappedAngle.hpp"

// The original uses float-rounded pi/tau as double constants, retains the
// x87 difference for the comparisons, and rounds only on each return path.
float __stdcall SecondaryWrappedAngleDifference(float target, float current)
{
    if (target - current > static_cast<double>(3.1415927f))
        return static_cast<float>(target - (current + static_cast<double>(6.2831855f)));
    if (current - target > static_cast<double>(3.1415927f))
        return static_cast<float>(target - (current - static_cast<double>(6.2831855f)));
    return target - current;
}
