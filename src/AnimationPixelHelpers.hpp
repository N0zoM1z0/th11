#pragma once

#include "AnimationTexture.hpp"

struct AnimationPixelSums
{
    unsigned int red, green, blue;
};
struct AnimationArgbPixel
{
    unsigned char blue, green, red, alpha;
};
unsigned int __stdcall ResolveAnimationTextureFormat(unsigned int index);

// Inline neighbor accumulators. Native keeps only dead out-of-line copies at
// 453440/453460/453490 after the transparent-pixel fixer inlined every call.
inline void __stdcall AccumulateAnimationArgbPixel(AnimationPixelSums *sums,
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

inline void __stdcall AccumulateAnimationA1Pixel(AnimationPixelSums *sums,
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

inline void __stdcall AccumulateAnimationA4Pixel(AnimationPixelSums *sums,
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
