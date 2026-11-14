// Independent pixel, sums, count and format inputs; this image is not runnable.
#include "AnimationPixelHelpers.hpp"
extern "C" unsigned int __stdcall AnimationPixelHelpersProbe(int kind,
    unsigned int format, AnimationPixelSums *sums, const void *pixel, unsigned int *count)
{
    switch (kind)
    {
    case 0: return ResolveAnimationTextureFormat(format);
    }
    return 0;
}
