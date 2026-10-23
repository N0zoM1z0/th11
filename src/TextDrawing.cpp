#include "TextDrawing.hpp"
#include <string.h>

void __stdcall DrawTextToTexture(const RECT *destination, int x, int fontHeight,
    DWORD color, const char *text, IDirect3DTexture9 *texture, int fontChoice)
{
    HFONT font;
    if (!fontChoice)
        font = g_LargeGothicFont;
    else if (fontChoice == 1)
        font = g_LargeMinchoFont;
    else if (fontChoice == 2)
        font = g_SmallGothicFont;
    else
        font = g_SmallMinchoFont;
    if (fontHeight < 17)
        fontHeight = 17;
    unsigned short background = (((color >> 20) & 15) << 8)
        | (((color >> 12) & 15) << 4) | ((color >> 4) & 15);
    unsigned short *pixels = static_cast<unsigned short *>(g_TextSurface.pixels);
    for (int i = 0; i < static_cast<int>(g_TextSurface.imageBytes); i += 2)
        *pixels++ = background;
    HDC dc = g_TextSurface.dc;
    HGDIOBJ previous = SelectObject(dc, font);
    int rows = fontHeight * 2 + 6;
    InvertTextAlpha(rows);
    SetBkMode(dc, TRANSPARENT);
    int length = static_cast<int>(strlen(text));
    SetTextColor(dc, color);
    TextOutA(dc, x * 2, 0, text, length);
    SelectObject(dc, previous);
    InvertTextAlpha(rows);
    FillTransparentTextPixels(rows);
    SelectObject(dc, previous);
    RECT source;
    source.left = 0;
    source.top = 0;
    source.right = (destination->right - destination->left) * 2 + 22;
    source.bottom = fontHeight * 2 + 2;
    if (source.right > 1024)
        source.right = 1024;
    IDirect3DSurface9 *surface;
    texture->GetSurfaceLevel(0, &surface);
    D3DXLoadSurfaceFromMemory(surface, 0, destination, g_TextSurface.pixels,
        g_TextSurface.format, g_TextSurface.pitch, 0, &source, 4, 0);
    if (surface)
        surface->Release();
}
