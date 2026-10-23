#pragma once

#include "TextSurface.hpp"

bool __stdcall InvertTextAlpha(int rows);
bool __stdcall FillTransparentTextPixels(unsigned int rows);
void __stdcall DrawTextToTexture(const RECT *destination, int x, int fontHeight,
    DWORD color, const char *text, IDirect3DTexture9 *texture, int fontChoice);

extern "C" HRESULT WINAPI D3DXLoadSurfaceFromMemory(IDirect3DSurface9 *destination,
    const PALETTEENTRY *destinationPalette, const RECT *destinationRect,
    const void *sourceMemory, D3DFORMAT sourceFormat, UINT sourcePitch,
    const PALETTEENTRY *sourcePalette, const RECT *sourceRect, DWORD filter,
    D3DCOLOR colorKey);
