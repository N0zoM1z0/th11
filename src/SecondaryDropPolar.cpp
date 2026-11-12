#include "SecondaryPlayerDrop.hpp"
#include <math.h>
#pragma intrinsic(sin, cos)

// Complete native elliptical-independent two-argument, 30-byte 0x424670
// polar helper. x87 FSINCOS scheduling remains an open codegen target.
void SecondaryDropPolarVector::SetPolar(float angle, float speed)
{
    x = static_cast<float>(cos(angle) * speed);
    y = static_cast<float>(sin(angle) * speed);
}
