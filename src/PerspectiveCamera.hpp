#pragma once

#include "Camera.hpp"

// The camera's observed 3D fields, sharing the matrix/viewport positions.
struct PerspectiveCameraView
{
    CameraVector position;
    CameraVector targetOffset;
    CameraVector up;
    unsigned char unknown024[12];
    CameraVector right;
    CameraVector eyeOffset;
    float fieldOfView;
    D3DMATRIX view;
    D3DMATRIX projection;
    D3DVIEWPORT9 viewport;
    unsigned int unknown0e4;
    float offsetX;
    float offsetY;
};

typedef char PerspectiveViewOffset[(offsetof(PerspectiveCameraView, view) == 0x4c) ? 1 : -1];
typedef char PerspectiveViewportOffset[(offsetof(PerspectiveCameraView, viewport) == 0xcc) ? 1 : -1];
typedef char PerspectiveRightOffset[(offsetof(PerspectiveCameraView, right) == 0x30) ? 1 : -1];

extern "C" CameraVector *WINAPI D3DXVec3Normalize(CameraVector *output, const CameraVector *input);
void __stdcall ApplyPerspectiveCamera(PerspectiveCameraView *self);
