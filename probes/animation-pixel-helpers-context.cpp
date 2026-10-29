// Independent pixel, sums, count and format inputs; this image is not runnable.
#include "AnimationPixelHelpers.hpp"
extern "C" unsigned int __stdcall AnimationPixelHelpersProbe(int kind,
    unsigned int format, AnimationPixelSums *sums, const void *pixel, unsigned int *count)
{
    switch (kind)
    {
    case 0: return ResolveAnimationTextureFormat(format);
    case 1: AccumulateAnimationArgbPixel(sums, static_cast<const AnimationArgbPixel *>(pixel), count); break;
    case 2: AccumulateAnimationA1Pixel(sums, static_cast<const unsigned short *>(pixel), count); break;
    case 3: AccumulateAnimationA4Pixel(sums, static_cast<const unsigned short *>(pixel), count); break;
    }
    return 0;
}
