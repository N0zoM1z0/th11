#include "OutlinedText.hpp"
#include <string.h>

void __fastcall DrawOutlinedTextToTexture(const char *text, int x,
    const RECT *destination, int fontHeight, DWORD foreground, DWORD outline,
    IDirect3DTexture9 *texture, int spacing, int fontChoice)
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
    memset(g_TextSurface.pixels, 0, g_TextSurface.imageBytes);
    HDC dc = g_TextSurface.dc;
    HGDIOBJ previous = SelectObject(dc, font);
    InvertTextAlpha(fontHeight * 2 + 6);
    SetBkMode(dc, TRANSPARENT);
    int length = static_cast<int>(strlen(text));
    if (!spacing)
    {
        SetTextColor(dc, outline);
        int position = x * 2;
        TextOutA(dc, position + 2, 4, text, length);
        TextOutA(dc, position - 2, 4, text, length);
        TextOutA(dc, position + 2, 0, text, length);
        TextOutA(dc, position - 2, 0, text, length);
        SetTextColor(dc, foreground);
        TextOutA(dc, position, 2, text, length);
    }
    else
    {
        char glyph[3] = {0, 0, 0};
        int position = x * 2 - 2;
        for (int i = 0; i < length; i += 2)
        {
            SetTextColor(dc, outline);
            glyph[0] = text[i];
            glyph[1] = text[i + 1];
            TextOutA(dc, position + 4, 4, glyph, 2);
            TextOutA(dc, position, 4, glyph, 2);
            TextOutA(dc, position + 4, 0, glyph, 2);
            TextOutA(dc, position, 0, glyph, 2);
            SetTextColor(dc, foreground);
            TextOutA(dc, position + 2, 2, glyph, 2);
            position += spacing * 2;
        }
    }
    SelectObject(dc, previous);
    int rows = fontHeight * 2 + 6;
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
