// Entry for the transparent-pixel fixer family; this image is not runnable.
#include "AnimationTexture.hpp"

void __stdcall FixAnimationTextureTransparentPixels(AnimationDocumentTextureView *texture);

extern "C" void __stdcall AnimationTransparentPixelsProbe(AnimationDocumentTextureView *texture)
{
    FixAnimationTextureTransparentPixels(texture);
}
