#include "SecondaryViewport428850.hpp"

// Four ordered floating comparisons against the playable rectangle. The
// comparisons must not be replaced by a length or unordered rejection test.
int SecondaryViewportPoint428850::IsOutside(float halfWidth, float halfHeight) const
{
    if (x + halfWidth <= -192.0 ||
        x - halfWidth >= 192.0 ||
        y + halfHeight <= 0.0 ||
        y - halfHeight >= 448.0)
        return 1;
    return 0;
}
