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
void __stdcall AccumulateAnimationArgbPixel(AnimationPixelSums *sums,
    const AnimationArgbPixel *pixel, unsigned int *count);
void __stdcall AccumulateAnimationA1Pixel(AnimationPixelSums *sums,
    const unsigned short *pixel, unsigned int *count);
void __stdcall AccumulateAnimationA4Pixel(AnimationPixelSums *sums,
    const unsigned short *pixel, unsigned int *count);
