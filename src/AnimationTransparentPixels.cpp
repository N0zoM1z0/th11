#include "AnimationTexture.hpp"

struct AnimationPixel32
{
    unsigned char blue, green, red, alpha;
};

union AnimationPixel16
{
    unsigned short value;
    struct
    {
        unsigned short blue : 5;
        unsigned short green : 5;
        unsigned short red : 5;
        unsigned short alpha : 1;
    } a1;
    struct
    {
        unsigned short blue : 4;
        unsigned short green : 4;
        unsigned short red : 4;
        unsigned short alpha : 4;
    } a4;
};

void __stdcall FixAnimationTextureTransparentPixels(AnimationDocumentTextureView *texture)
{
    IDirect3DSurface9 *surface = 0;
    texture->texture->GetSurfaceLevel(0, &surface);
    D3DSURFACE_DESC description;
    surface->GetDesc(&description);
    D3DLOCKED_RECT locked;
    surface->LockRect(&locked, 0, 0);
    switch (description.Format)
    {
    case D3DFMT_UNKNOWN:
    case D3DFMT_A8R8G8B8:
        for (unsigned int row = 0; row < description.Height; ++row)
        {
            AnimationPixel32 *pixel = reinterpret_cast<AnimationPixel32 *>(
                static_cast<unsigned char *>(locked.pBits) + row * locked.Pitch);
            for (unsigned int column = 0; column < description.Width; ++column, ++pixel)
            {
                if (!pixel->alpha)
                {
                    unsigned int red = 0, green = 0, blue = 0, count = 0;
                    if (column > 0 && pixel[-1].alpha)
                    {
                        red = pixel[-1].red; green = pixel[-1].green; blue = pixel[-1].blue;
                        count = 1;
                    }
                    if (column < description.Width - 1 && pixel[1].alpha)
                    {
                        red += pixel[1].red; green += pixel[1].green; blue += pixel[1].blue;
                        ++count;
                    }
                    if (row > 0)
                    {
                        AnimationPixel32 *above = pixel - locked.Pitch / 4;
                        if (above->alpha)
                        {
                            red += above->red; green += above->green; blue += above->blue;
                            ++count;
                        }
                    }
                    if (row < description.Height - 1)
                    {
                        AnimationPixel32 *below = pixel + locked.Pitch / 4;
                        if (below->alpha)
                        {
                            red += below->red; green += below->green; blue += below->blue;
                            ++count;
                        }
                    }
                    if (count > 1)
                    {
                        red /= count; green /= count; blue /= count;
                    }
                    pixel->red = static_cast<unsigned char>(red);
                    pixel->green = static_cast<unsigned char>(green);
                    pixel->blue = static_cast<unsigned char>(blue);
                }
            }
        }
        break;
    case D3DFMT_A1R5G5B5:
        for (unsigned int row = 0; row < description.Height; ++row)
        {
            AnimationPixel16 *pixel = reinterpret_cast<AnimationPixel16 *>(
                static_cast<unsigned char *>(locked.pBits) + row * locked.Pitch);
            for (unsigned int column = 0; column < description.Width; ++column, ++pixel)
            {
                if (!pixel->a1.alpha)
                {
                    unsigned int red = 0, green = 0, blue = 0, count = 0;
                    if (column > 0 && pixel[-1].a1.alpha)
                    {
                        red = pixel[-1].a1.red; green = pixel[-1].a1.green; blue = pixel[-1].a1.blue;
                        count = 1;
                    }
                    if (column < description.Width - 1 && pixel[1].a1.alpha)
                    {
                        red += pixel[1].a1.red; green += pixel[1].a1.green; blue += pixel[1].a1.blue;
                        ++count;
                    }
                    if (row > 0)
                    {
                        AnimationPixel16 *above = pixel - locked.Pitch / 2;
                        if (above->a1.alpha)
                        {
                            red += above->a1.red; green += above->a1.green; blue += above->a1.blue;
                            ++count;
                        }
                    }
                    if (row < description.Height - 1)
                    {
                        AnimationPixel16 *below = pixel + locked.Pitch / 2;
                        if (below->a1.alpha)
                        {
                            red += below->a1.red; green += below->a1.green; blue += below->a1.blue;
                            ++count;
                        }
                    }
                    if (count > 1)
                    {
                        red /= count; green /= count; blue /= count;
                    }
                    pixel->a1.red = static_cast<unsigned char>(red);
                    pixel->a1.green = static_cast<unsigned char>(green);
                    pixel->a1.blue = static_cast<unsigned char>(blue);
                }
            }
        }
        break;
    case D3DFMT_A4R4G4B4:
        for (unsigned int row = 0; row < description.Height; ++row)
        {
            AnimationPixel16 *pixel = reinterpret_cast<AnimationPixel16 *>(
                static_cast<unsigned char *>(locked.pBits) + row * locked.Pitch);
            for (unsigned int column = 0; column < description.Width; ++column, ++pixel)
            {
                if (!pixel->a4.alpha)
                {
                    unsigned int red = 0, green = 0, blue = 0, count = 0;
                    if (column > 0 && pixel[-1].a4.alpha)
                    {
                        red = pixel[-1].a4.red; green = pixel[-1].a4.green; blue = pixel[-1].a4.blue;
                        count = 1;
                    }
                    if (column < description.Width - 1 && pixel[1].a4.alpha)
                    {
                        red += pixel[1].a4.red; green += pixel[1].a4.green; blue += pixel[1].a4.blue;
                        ++count;
                    }
                    if (row > 0)
                    {
                        AnimationPixel16 *above = pixel - locked.Pitch / 2;
                        if (above->a4.alpha)
                        {
                            red += above->a4.red; green += above->a4.green; blue += above->a4.blue;
                            ++count;
                        }
                    }
                    if (row < description.Height - 1)
                    {
                        AnimationPixel16 *below = pixel + locked.Pitch / 2;
                        if (below->a4.alpha)
                        {
                            red += below->a4.red; green += below->a4.green; blue += below->a4.blue;
                            ++count;
                        }
                    }
                    if (count > 1)
                    {
                        red /= count; green /= count; blue /= count;
                    }
                    pixel->a4.red = static_cast<unsigned char>(red);
                    pixel->a4.green = static_cast<unsigned char>(green);
                    pixel->a4.blue = static_cast<unsigned char>(blue);
                }
            }
        }
        break;
    }
    surface->UnlockRect();
    surface->Release();
}
