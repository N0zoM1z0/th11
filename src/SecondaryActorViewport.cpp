#include "SecondaryActorViewport.hpp"

// Original TH11 0x419920. The receiver is a three-component position;
// VC9 naturally passes the thiscall receiver in ECX and the two half-extents
// on the stack, returning with RET8. All comparisons use ordered boundaries.
int SecondaryActorViewportPoint::IsOutside(float halfWidth, float halfHeight) const
{
    if (x + halfWidth <= -192.0 ||
        x - halfWidth >= 192.0 ||
        y + halfHeight <= 0.0 ||
        y - halfHeight >= 448.0)
        return 1;
    return 0;
}
