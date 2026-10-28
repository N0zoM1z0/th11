#pragma once

#include "AnimationDocument.hpp"

struct AnimationEmbeddedTextureView
{
    unsigned char unknown000[6];
    short format, width, height;
    unsigned char unknown00c[4];
};

typedef char AnimationEmbeddedTextureSize[(sizeof(AnimationEmbeddedTextureView) == 16) ? 1 : -1];

extern const D3DFORMAT g_AnimationTextureFormats[6];
extern const unsigned int g_AnimationTextureBytesPerPixel[6];
void __stdcall FixAnimationTextureTransparentPixels(AnimationDocumentTextureView *texture);

struct D3DXIMAGE_INFO;
extern "C" HRESULT WINAPI D3DXCreateTextureFromFileInMemoryEx(IDirect3DDevice9 *device,
    const void *data, UINT size, UINT width, UINT height, UINT levels, DWORD usage,
    D3DFORMAT format, D3DPOOL pool, DWORD filter, DWORD mipFilter, D3DCOLOR colorKey,
    D3DXIMAGE_INFO *info, PALETTEENTRY *palette, IDirect3DTexture9 **texture);
extern "C" HRESULT WINAPI D3DXCreateTexture(IDirect3DDevice9 *device,
    UINT width, UINT height, UINT levels, DWORD usage, D3DFORMAT format,
    D3DPOOL pool, IDirect3DTexture9 **texture);
extern "C" HRESULT WINAPI D3DXLoadSurfaceFromSurface(IDirect3DSurface9 *destination,
    const PALETTEENTRY *destinationPalette, const RECT *destinationRect,
    IDirect3DSurface9 *source, const PALETTEENTRY *sourcePalette,
    const RECT *sourceRect, DWORD filter, D3DCOLOR colorKey);
extern "C" HRESULT WINAPI D3DXLoadSurfaceFromMemory(IDirect3DSurface9 *destination,
    const PALETTEENTRY *destinationPalette, const RECT *destinationRect,
    const void *sourceMemory, D3DFORMAT sourceFormat, UINT sourcePitch,
    const PALETTEENTRY *sourcePalette, const RECT *sourceRect, DWORD filter,
    D3DCOLOR colorKey);
