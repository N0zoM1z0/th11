#include "TextDrawing.hpp"

// Non-runnable context retaining unknown font, dimensions, objects and strings.
extern "C" unsigned int __stdcall TextDrawingProbe(const RECT *destination, int x, int height,
    DWORD color, const char *text, IDirect3DTexture9 *texture, int fontChoice,
    int signedRows, unsigned int rows)
{
    bool inverted = InvertTextAlpha(signedRows);
    bool filled = FillTransparentTextPixels(rows);
    DrawTextToTexture(destination, x, height, color, text, texture, fontChoice);
    return static_cast<unsigned int>(inverted) + 2 * static_cast<unsigned int>(filled);
}
