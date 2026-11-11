#include "AnimationSurfaceCopy.hpp"
#include "Screenshot.hpp"

// Native TH11 00445460..0044548F: process all four renderer-prefix
// commands, including invalidating a command only after its copy call.
void __stdcall FlushScreenshotResources()
{
    RenderBatchView *renderer = g_RenderBatch;
    AnimationSurfaceCopyView *command =
        reinterpret_cast<AnimationSurfaceCopyView *>(renderer);
    for (int remaining = 4; remaining != 0; --remaining, ++command)
    {
        if (command->documentIndex >= 0)
        {
            CopyAnimationTextureSurface(renderer, command);
            command->documentIndex = -1;
        }
    }
}
