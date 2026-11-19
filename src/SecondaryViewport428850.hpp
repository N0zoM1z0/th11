#pragma once
#include "Camera.hpp"

// Physical three-float viewport point. The native 0x428850 member receives
// this in ECX and two half-extents on the stack (RET 8).
struct SecondaryViewportPoint428850 : CameraVector
{
    int IsOutside(float halfWidth, float halfHeight) const;
};
typedef char SecondaryViewportPoint428850Size[(sizeof(SecondaryViewportPoint428850) == sizeof(CameraVector)) ? 1 : -1];
