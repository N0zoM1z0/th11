#pragma once

#include "TextDrawing.hpp"

void __fastcall DrawOutlinedTextToTexture(const char *text, int x,
    const RECT *destination, int fontHeight, DWORD foreground, DWORD outline,
    IDirect3DTexture9 *texture, int spacing, int fontChoice);

bool __stdcall DrawOutlinedSurfaceText(TextSurfaceView *state, const char *text,
    int x, int y, HFONT font, COLORREF foreground, COLORREF outline);

struct TextTexturePlacementView
{
    unsigned char unknown000[0x0c];
    float left, top, right, bottom;
};

void __stdcall DrawPlacedTextureText(IDirect3DTexture9 *texture,
    const TextTexturePlacementView *placement, int x, DWORD foreground,
    DWORD outline, int plain, int spacing, int fontHeight, const char *text,
    int fontChoice);
