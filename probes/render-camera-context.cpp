// Non-runnable compiler context retaining independent camera and batch owners.
#include "../src/Camera.hpp"
#include "../src/RenderBatch.hpp"

extern "C" void __stdcall RenderCameraProbe(CameraView *first, CameraView *second,
                                            RenderBatchView *batch)
{
    FlushRenderBatch(batch);
    ApplyCamera(first);
    ApplyCamera(second);
    FlushRenderBatch(batch);
}
