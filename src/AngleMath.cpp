#include "AngleMath.hpp"

namespace
{
// TH11 stores double representations of float-rounded pi and two pi.
const double kPi = static_cast<double>(3.14159265358979323846f);
const double kTwoPi = static_cast<double>(6.28318530717958647692f);
}

// TH11 0x00458ED0..0x00458F60. One budget covers both correction directions.
float __stdcall NormalizeAngle(float angle)
{
    int corrections = 0;
    while (angle > kPi)
    {
        angle = static_cast<float>(angle - kTwoPi);
        if (corrections++ > 32)
            break;
    }
    while (angle < -kPi)
    {
        angle = static_cast<float>(angle + kTwoPi);
        if (corrections++ > 32)
            break;
    }
    return angle;
}

// TH11 0x00458E30..0x00458ECC rounds the sum to float before correction.
float __stdcall AddAngles(float angle, float offset)
{
    angle += offset;
    int corrections = 0;
    while (angle > kPi)
    {
        angle = static_cast<float>(angle - kTwoPi);
        if (corrections++ > 32)
            break;
    }
    while (angle < -kPi)
    {
        angle = static_cast<float>(angle + kTwoPi);
        if (corrections++ > 32)
            break;
    }
    return angle;
}
