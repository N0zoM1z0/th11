#include "PauseOverlayRectangle.hpp"
// Compile-only, non-runnable owner entry.
extern "C" void __stdcall PauseOverlayProbe(AnimationVmId id)
{
    CapturePauseOverlayFromVm(id);
}

// A separate external caller with a non-global renderer prevents the linked
// probe from specializing the queue solely to the singleton renderer.
extern "C" int __stdcall PauseOverlayQueueProbe(
    RenderBatchView *renderer, int texture, int index,
    int x, int y, int width, int height)
{
    return QueuePauseOverlayRectangle(renderer, texture, index,
                                     x, y, width, height);
}
