#pragma once
#include "StageMesh.hpp"

// Four physical draw-command slots at the base of the renderer; original
// nominal class and ownership remain unresolved.
struct PauseOverlayRectangleSlot
{
    int texture, textureIndex;
    int minX, minY, maxX, maxY;
    int x, y, width, height;
};
typedef char PauseOverlaySlotSize[(sizeof(PauseOverlayRectangleSlot) == 0x28) ? 1 : -1];

int __stdcall QueuePauseOverlayRectangle(RenderBatchView *renderer,
    int texture, int textureIndex, int x, int y, int width, int height);
void __stdcall CapturePauseOverlayFromVm(AnimationVmId id);
