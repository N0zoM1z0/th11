#include "FrameLoop.hpp"
#include "RenderBatch.hpp"

HRESULT __stdcall DisableRuntimeFog(MainRuntimeView *self)
{
    FrameRuntimeView &runtime = *reinterpret_cast<FrameRuntimeView *>(self);
    if (runtime.fogEnabled)
    {
        FlushRenderBatch(g_RenderBatch);
        runtime.fogEnabled = 0;
        return static_cast<IDirect3DDevice9 *>(self->d3dDevice)->SetRenderState(D3DRS_FOGENABLE, FALSE);
    }
    return 0;
}

void __stdcall StopRuntimeThread(RuntimeThreadView *self)
{
    if (self->thread)
    {
        self->stopRequested = 1;
        self->pending = 0;
        DWORD result = WaitForSingleObject(self->thread, 200);
        while (result == WAIT_TIMEOUT)
        {
            self->stopRequested = 1;
            self->pending = 0;
            Sleep(1);
            result = WaitForSingleObject(self->thread, 200);
        }
        CloseHandle(self->thread);
        self->thread = 0;
        self->unknown18 = 0;
    }
}
