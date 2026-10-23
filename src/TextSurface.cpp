#include "TextSurface.hpp"
#include <string.h>

bool __cdecl ReleaseTextSurface()
{
    if (g_TextSurface.dc)
    {
        SelectObject(g_TextSurface.dc, g_TextSurface.previousBitmap);
        DeleteDC(g_TextSurface.dc);
        DeleteObject(g_TextSurface.bitmap);
        g_TextSurface.width = 0;
        g_TextSurface.height = 0;
        g_TextSurface.dc = 0;
        g_TextSurface.bitmap = 0;
        g_TextSurface.previousBitmap = 0;
        g_TextSurface.pixels = 0;
        g_TextSurface.format = static_cast<D3DFORMAT>(-1);
        return true;
    }
    return false;
}

static __forceinline const TextPixelFormat *FindTextPixelFormat(D3DFORMAT format)
{
    unsigned int index = 0;
    while (g_TextPixelFormats[index].format != -1 &&
           g_TextPixelFormats[index].format != format)
        ++index;
    return format == static_cast<D3DFORMAT>(-1) ? 0 : &g_TextPixelFormats[index];
}

bool __stdcall CreateTextSurface(D3DFORMAT format)
{
    ReleaseTextSurface();
    BITMAPV4HEADER info;
    memset(&info, 0, sizeof(info));
    const TextPixelFormat *pixelFormat = FindTextPixelFormat(format);
    if (!pixelFormat)
        return false;
    int pitch = ((1024 * pixelFormat->bits / 8 + 3) / 4) * 4;
    info.bV4Size = sizeof(info);
    info.bV4Width = 1024;
    info.bV4Height = -65;
    info.bV4Planes = 1;
    info.bV4BitCount = static_cast<WORD>(pixelFormat->bits);
    info.bV4SizeImage = pitch * 64;
    if (format != D3DFMT_X1R5G5B5 && format != D3DFMT_X8R8G8B8)
    {
        info.bV4V4Compression = BI_BITFIELDS;
        info.bV4RedMask = pixelFormat->redMask;
        info.bV4GreenMask = pixelFormat->greenMask;
        info.bV4BlueMask = pixelFormat->blueMask;
        info.bV4AlphaMask = pixelFormat->alphaMask;
    }
    void *pixels;
    HBITMAP bitmap = CreateDIBSection(0, reinterpret_cast<BITMAPINFO *>(&info),
                                     DIB_RGB_COLORS, &pixels, 0, 0);
    if (!bitmap)
        return false;
    memset(pixels, 0, info.bV4SizeImage);
    HDC dc = CreateCompatibleDC(0);
    HGDIOBJ previousBitmap = SelectObject(dc, bitmap);
    g_TextSurface.dc = dc;
    g_TextSurface.bitmap = bitmap;
    g_TextSurface.pitch = pitch;
    g_TextSurface.pixels = pixels;
    g_TextSurface.imageBytes = info.bV4SizeImage;
    g_TextSurface.previousBitmap = previousBitmap;
    g_TextSurface.format = format;
    g_TextSurface.width = 1024;
    g_TextSurface.height = 64;
    return true;
}
