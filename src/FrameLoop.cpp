#include "FrameLoop.hpp"
#include "FrameClock.hpp"
#include "RenderBatch.hpp"
#include "Chain.hpp"
#include "Screenshot.hpp"

int __stdcall RunFrame(FrameLoopView *self)
{
    FrameRuntimeView &runtime = FrameRuntime();
    FlushRenderBatch(g_RenderBatch);
    runtime.activeCamera = &runtime.screenCamera;
    ApplyCamera(&runtime.screenCamera);
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetViewport(&runtime.activeCamera->viewport);
    runtime.activeCameraIndex = 1;
    int result = g_ChainManager->ExecuteUpdateChain();
    if (result == 0)
    {
        StopRuntimeThread(&runtime.worker);
        return 1;
    }
    if (result == -1)
    {
        StopRuntimeThread(&runtime.worker);
        return 2;
    }
    ++self->skippedFrames;
    if (runtime.frameSkip + 1 <= self->skippedFrames)
    {
        static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->BeginScene();
        g_RenderBatch->quadCount = 0;
        g_RenderBatch->nextVertex = g_RenderBatch->vertices;
        g_RenderBatch->batchStart = g_RenderBatch->vertices;
        runtime.fogEnabled = 0xff;
        DisableRuntimeFog(&g_MainRuntime);
        ExecuteDrawChain();
        FlushRenderBatch(g_RenderBatch);
        static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetTexture(0, 0);
        static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->EndScene();
        self->skippedFrames = 0;
        self->PresentFrame();
    }
    runtime.frameElapsed = GetFrameClockSeconds() - self->frameStart;
    return 0;
}

void FrameLoopView::PresentFrame()
{
    this->frameStart = GetFrameClockSeconds();
    if (FrameRuntime().waitForPresent == 1)
    {
        if (this->frameStart - this->lastPresent < 1.0 / 60.0 &&
            this->frameStart - this->lastPresent > 0.0)
        {
            int wait = static_cast<int>((this->lastPresent + 1.0 / 60.0 - this->frameStart) * 1000.0 - 3.5);
            if (wait > 0)
                Sleep(wait);
        }
    }
    this->beforePresent = GetFrameClockSeconds();
    HandleScreenshotHotkey();
    if (FAILED(static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->Present(0, 0, 0, 0)))
    {
        ReleaseRuntimeSurfaces();
        PrepareRendererForReset();
        static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->Reset(&FrameRuntime().presentation);
        RestoreRendererAfterReset();
        EnsureRuntimeSurfaces(&g_MainRuntime);
        ResetDeviceRenderStates();
        FrameRuntime().presentationState = 2;
    }
    this->lastPresent = GetFrameClockSeconds();
    if (g_PostPresentClock)
        UpdatePostPresentClock();
    if (g_PostPresentAudio)
        UpdatePostPresentAudio();
}
