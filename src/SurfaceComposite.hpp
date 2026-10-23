#pragma once

#include <d3d9.h>
#include <stddef.h>

// Physical fields observed in the pixel compositor. The original owner is open.
struct SurfaceCompositeView
{
    unsigned char unknown000[0x100];
    D3DFORMAT format;
    int width, height;
    unsigned int unknown10c;
    unsigned int rowBytes;
    unsigned char unknown114[8];
    int hasBuffer;
    unsigned char *pixels;

    bool BlendToSurface(IDirect3DSurface9 *surface, int x, int y,
        int copyWidth, int copyHeight);
};

typedef char CompositeFormatOffset[(offsetof(SurfaceCompositeView, format) == 0x100) ? 1 : -1];
typedef char CompositePixelsOffset[(offsetof(SurfaceCompositeView, pixels) == 0x120) ? 1 : -1];
