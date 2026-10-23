#include "DeviceInitialization.hpp"
#include "RenderBatch.hpp"

void __cdecl ResetMainViewport()
{
    if (g_RenderBatch)
        FlushRenderBatch(g_RenderBatch);
    DeviceInitializationView &state = DeviceRuntime();
    state.viewport.MinZ = 0.0f;
    state.viewport.X = 0;
    state.viewport.Y = 0;
    state.viewport.MaxZ = 1.0f;
    state.viewport.Width = 640;
    state.viewport.Height = 480;
    state.device->SetViewport(&state.viewport);
    PrimeDeviceBackBuffers();
}
