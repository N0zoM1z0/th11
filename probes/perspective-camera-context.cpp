// Non-runnable context for two independent camera receivers.
#include "../src/PerspectiveCamera.hpp"
#include "../src/RenderBatch.hpp"

extern "C" void __stdcall PerspectiveCameraProbe(PerspectiveCameraView *first,
    PerspectiveCameraView *second, RenderBatchView *batch)
{
    FlushRenderBatch(batch);
    ApplyPerspectiveCamera(first);
    ApplyPerspectiveCamera(second);
    FlushRenderBatch(batch);
}
