#include "AnimationPixelHelpers.hpp"

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
                    AnimationPixelSums sums = {0, 0, 0};
                    unsigned int count = 0;
                    if (column > 0) AccumulateAnimationArgbPixel(&sums, reinterpret_cast<const AnimationArgbPixel *>(pixel - 1), &count);
                    if (column < description.Width - 1) AccumulateAnimationArgbPixel(&sums, reinterpret_cast<const AnimationArgbPixel *>(pixel + 1), &count);
                    if (row > 0) AccumulateAnimationArgbPixel(&sums, reinterpret_cast<const AnimationArgbPixel *>(pixel - locked.Pitch / 4), &count);
                    if (row < description.Height - 1) AccumulateAnimationArgbPixel(&sums, reinterpret_cast<const AnimationArgbPixel *>(pixel + locked.Pitch / 4), &count);
                    if (count > 1)
                    {
                        sums.red /= count; sums.green /= count; sums.blue /= count;
                    }
                    pixel->red = static_cast<unsigned char>(sums.red);
                    pixel->green = static_cast<unsigned char>(sums.green);
                    pixel->blue = static_cast<unsigned char>(sums.blue);
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
                    AnimationPixelSums sums = {0, 0, 0};
                    unsigned int count = 0;
                    if (column > 0) AccumulateAnimationA1Pixel(&sums, reinterpret_cast<const unsigned short *>(pixel - 1), &count);
                    if (column < description.Width - 1) AccumulateAnimationA1Pixel(&sums, reinterpret_cast<const unsigned short *>(pixel + 1), &count);
                    if (row > 0) AccumulateAnimationA1Pixel(&sums, reinterpret_cast<const unsigned short *>(pixel - locked.Pitch / 2), &count);
                    if (row < description.Height - 1) AccumulateAnimationA1Pixel(&sums, reinterpret_cast<const unsigned short *>(pixel + locked.Pitch / 2), &count);
                    if (count > 1)
                    {
                        sums.red /= count; sums.green /= count; sums.blue /= count;
                    }
                    pixel->a1.red = static_cast<unsigned char>(sums.red);
                    pixel->a1.green = static_cast<unsigned char>(sums.green);
                    pixel->a1.blue = static_cast<unsigned char>(sums.blue);
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
                    AnimationPixelSums sums = {0, 0, 0};
                    unsigned int count = 0;
                    if (column > 0) AccumulateAnimationA4Pixel(&sums, reinterpret_cast<const unsigned short *>(pixel - 1), &count);
                    if (column < description.Width - 1) AccumulateAnimationA4Pixel(&sums, reinterpret_cast<const unsigned short *>(pixel + 1), &count);
                    if (row > 0) AccumulateAnimationA4Pixel(&sums, reinterpret_cast<const unsigned short *>(pixel - locked.Pitch / 2), &count);
                    if (row < description.Height - 1) AccumulateAnimationA4Pixel(&sums, reinterpret_cast<const unsigned short *>(pixel + locked.Pitch / 2), &count);
                    if (count > 1)
                    {
                        sums.red /= count; sums.green /= count; sums.blue /= count;
                    }
                    pixel->a4.red = static_cast<unsigned char>(sums.red);
                    pixel->a4.green = static_cast<unsigned char>(sums.green);
                    pixel->a4.blue = static_cast<unsigned char>(sums.blue);
                }
            }
        }
        break;
    }
    surface->UnlockRect();
    surface->Release();
}
