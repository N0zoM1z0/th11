#include "OutlinedText.hpp"

void __stdcall DrawPlacedTextureText(IDirect3DTexture9 *texture,
    const TextTexturePlacementView *placement, int x, DWORD foreground,
    DWORD outline, int plain, int spacing, int fontHeight, const char *text,
    int fontChoice)
{
    if (fontHeight <= 0)
        fontHeight = 17;
    else if (fontHeight <= 8)
        return;
    RECT destination;
    destination.left = static_cast<int>(placement->left);
    destination.top = static_cast<int>(placement->top);
    destination.right = static_cast<int>(placement->right);
    destination.bottom = static_cast<int>(placement->bottom);
    if (!plain)
        DrawOutlinedTextToTexture(text, x, &destination, fontHeight,
            foreground, outline, texture, spacing, fontChoice);
    else
        DrawTextToTexture(&destination, x, fontHeight, foreground,
            text, texture, fontChoice);
}
