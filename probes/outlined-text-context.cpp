#include "OutlinedText.hpp"

// Non-runnable LTCG context; both objects, all colors and strings stay unknown.
extern "C" bool __stdcall OutlinedTextProbe(TextSurfaceView *state, const char *text,
    int x, int y, HFONT font, DWORD foreground, DWORD outline,
    int fontHeight, IDirect3DTexture9 *texture, int spacing, int fontChoice,
    const TextTexturePlacementView *placement, int plain)
{
    bool result = DrawOutlinedSurfaceText(state, text, x, y, font, foreground, outline);
    DrawPlacedTextureText(texture, placement, x, foreground, outline, plain,
        spacing, fontHeight, text, fontChoice);
    return result;
}
