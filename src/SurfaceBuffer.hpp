#pragma once

#include <d3d9.h>
#include <stddef.h>

// Observed fields shared by the surface-copy and pixel-compositing routines.
struct SurfaceBufferView
{
    unsigned char unknown000[0x100];
    D3DFORMAT format;
    int width;
    int height;
    unsigned int unknown10c;
    unsigned int rowBytes;
    unsigned char unknown114[8];
    int hasBuffer;
    unsigned char *pixels;

    bool ReadFromSurface(IDirect3DSurface9 *surface);
    bool WriteToSurface(IDirect3DSurface9 *surface);
};

typedef char SurfaceBufferFormatOffset[(offsetof(SurfaceBufferView, format) == 0x100) ? 1 : -1];
typedef char SurfaceBufferPixelsOffset[(offsetof(SurfaceBufferView, pixels) == 0x120) ? 1 : -1];
