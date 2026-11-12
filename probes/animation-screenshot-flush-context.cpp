#include "AnimationSurfaceCopy.hpp"
#include "Screenshot.hpp"
void __stdcall FlushScreenshotResources();
extern "C" void __stdcall AnimationScreenshotFlushProbe(
    RenderBatchView *renderer, AnimationSurfaceCopyView *record, int invokeDirect)
{
    FlushScreenshotResources();
    if (invokeDirect) CopyAnimationTextureSurface(renderer, record);
}
