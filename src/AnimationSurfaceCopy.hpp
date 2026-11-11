#pragma once

#include "RenderBatch.hpp"

// Four 40-byte prefix records at the start of the native renderer.
struct AnimationSurfaceCopyView
{
    int documentIndex, textureIndex;
    int sourceX, sourceY, sourceWidth, sourceHeight;
    int destinationX, destinationY, destinationWidth, destinationHeight;
};
typedef char SurfaceCopyRecordSize[(sizeof(AnimationSurfaceCopyView) == 40) ? 1 : -1];

void __stdcall CopyAnimationTextureSurface(RenderBatchView *renderer,
    const AnimationSurfaceCopyView *command);
