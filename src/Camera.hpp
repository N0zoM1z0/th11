#pragma once

#include <d3d9.h>
#include <stddef.h>

struct CameraVector
{
    float x, y, z;
};

// Partial camera view, retaining unknown storage ahead of its view matrix.
struct CameraView
{
    unsigned char unknown000[0x4c];
    D3DMATRIX view;
    D3DMATRIX projection;
    D3DVIEWPORT9 viewport;
    unsigned int unknown0e4;
    float offsetX;
    float offsetY;
};

typedef char CameraViewMatrixOffset[(offsetof(CameraView, view) == 0x4c) ? 1 : -1];
typedef char CameraProjectionOffset[(offsetof(CameraView, projection) == 0x8c) ? 1 : -1];
typedef char CameraViewportOffset[(offsetof(CameraView, viewport) == 0xcc) ? 1 : -1];
typedef char CameraOffsetXOffset[(offsetof(CameraView, offsetX) == 0xe8) ? 1 : -1];

extern "C" D3DMATRIX *WINAPI D3DXMatrixLookAtLH(D3DMATRIX *output,
    const CameraVector *eye, const CameraVector *at, const CameraVector *up);
extern "C" D3DMATRIX *WINAPI D3DXMatrixPerspectiveFovLH(D3DMATRIX *output,
    float fieldOfView, float aspect, float nearPlane, float farPlane);

float __stdcall CameraTangent(float angle);
void __stdcall ApplyCamera(CameraView *self);
