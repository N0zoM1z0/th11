#include "Camera.hpp"
#include "MainRuntime.hpp"
#include "RenderBatch.hpp"

void __stdcall ApplyCamera(CameraView *self)
{
    if (g_RenderBatch)
        FlushRenderBatch(g_RenderBatch);

    float centerX = static_cast<float>(self->viewport.Width) * 0.5;
    float centerY = static_cast<float>(self->viewport.Height) * 0.5;
    float aspect = static_cast<float>(self->viewport.Width) /
                   static_cast<float>(self->viewport.Height);
    CameraVector eye = { centerX, centerY, centerY / CameraTangent(0.15707963705062866f) };
    CameraVector at = { centerX, centerY, 0.0f };
    CameraVector up = { 0.0f, -1.0f, 0.0f };
    D3DXMatrixLookAtLH(&self->view, &eye, &at, &up);
    D3DXMatrixPerspectiveFovLH(&self->projection, 0.3141592741012573f, aspect, 1.0f, 10000.0f);

    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetTransform(D3DTS_VIEW, &self->view);
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetTransform(D3DTS_PROJECTION, &self->projection);
    if (g_RenderBatch)
    {
        g_RenderBatch->cameraOffsetX = self->offsetX;
        g_RenderBatch->cameraOffsetY = self->offsetY;
    }
}
