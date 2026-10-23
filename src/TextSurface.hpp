#pragma once

#include <d3d9.h>
#include <stddef.h>
#include "Random.hpp"

struct TextPixelFormat
{
    int format;
    int bits;
    DWORD alphaMask, redMask, greenMask, blueMask;
};

// Observed text workspace span; its complete original owner is unknown.
struct TextSurfaceView
{
    unsigned char noise[256];
    D3DFORMAT format;
    int width, height;
    DWORD imageBytes;
    int pitch;
    HDC dc;
    HGDIOBJ previousBitmap;
    HBITMAP bitmap;
    void *pixels;
};

typedef char TextFormatStride[(sizeof(TextPixelFormat) == 24) ? 1 : -1];
typedef char TextSurfaceFormatOffset[(offsetof(TextSurfaceView, format) == 0x100) ? 1 : -1];
typedef char TextSurfacePixelsOffset[(offsetof(TextSurfaceView, pixels) == 0x120) ? 1 : -1];

extern TextSurfaceView g_TextSurface;
extern const TextPixelFormat g_TextPixelFormats[];
extern RandomStateView g_TextRandomState;
extern const char g_FontFaceGothic[];
extern const char g_FontFaceMincho[];
extern HFONT g_LargeGothicFont, g_LargeMinchoFont;
extern HFONT g_SmallGothicFont, g_SmallMinchoFont;

bool __cdecl ReleaseTextSurface();
bool __stdcall CreateTextSurface(D3DFORMAT format);
void __cdecl InitializeTextResources();
