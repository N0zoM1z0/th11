#include "AnimationPixelHelpers.hpp"
#include "MainRuntime.hpp"

unsigned int __stdcall ResolveAnimationTextureFormat(unsigned int index)
{
    if (*reinterpret_cast<const unsigned char *>(
        reinterpret_cast<const unsigned char *>(&g_MainRuntime) + 0x200) & 1)
    {
        D3DFORMAT format = g_AnimationTextureFormats[index];
        if (format == D3DFMT_A8R8G8B8 || format == D3DFMT_UNKNOWN)
            index = 5;
        else if (format == D3DFMT_R8G8B8)
            index = 3;
    }
    return index;
}

void __stdcall AccumulateAnimationArgbPixel(AnimationPixelSums *sums,
    const AnimationArgbPixel *pixel, unsigned int *count)
{
    if (pixel->alpha)
    {
        sums->red += pixel->red;
        sums->green += pixel->green;
        sums->blue += pixel->blue;
        ++*count;
    }
}

void __stdcall AccumulateAnimationA1Pixel(AnimationPixelSums *sums,
    const unsigned short *pixel, unsigned int *count)
{
    unsigned short value = *pixel;
    if (value & 0x8000)
    {
        sums->red += (value >> 10) & 31;
        sums->green += (value >> 5) & 31;
        sums->blue += value & 31;
        ++*count;
    }
}

void __stdcall AccumulateAnimationA4Pixel(AnimationPixelSums *sums,
    const unsigned short *pixel, unsigned int *count)
{
    unsigned short value = *pixel;
    if (value & 0xf000)
    {
        sums->red += (value >> 8) & 15;
        sums->green += (value >> 4) & 15;
        sums->blue += value & 15;
        ++*count;
    }
}
