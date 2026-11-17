#pragma once

#include "Camera.hpp"

// A layout-compatible three-float vector with the native 0x419920
// view-rectangle member. Only this observed method is declared.
struct SecondaryActorViewportPoint : CameraVector
{
    int IsOutside(float halfWidth, float halfHeight) const;
};
typedef char SecondaryActorViewportPointSize[
    (sizeof(SecondaryActorViewportPoint) == sizeof(CameraVector)) ? 1 : -1];
