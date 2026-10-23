#include "PerspectiveCamera.hpp"
#include "MainRuntime.hpp"
#include "RenderBatch.hpp"

static __forceinline CameraVector *CameraCross(CameraVector *output,
    const CameraVector *left, const CameraVector *right)
{
    CameraVector result;
    result.x = left->y * right->z - left->z * right->y;
    result.y = left->z * right->x - left->x * right->z;
    result.z = left->x * right->y - left->y * right->x;
    *output = result;
    return output;
}

void __stdcall ApplyPerspectiveCamera(PerspectiveCameraView *self)
{
    if (g_RenderBatch)
        FlushRenderBatch(g_RenderBatch);

    CameraVector at = {
        self->targetOffset.x + self->position.x,
        self->targetOffset.y + self->position.y,
        self->targetOffset.z + self->position.z
    };
    CameraVector eye = {
        self->eyeOffset.x + self->position.x,
        self->eyeOffset.y + self->position.y,
        self->eyeOffset.z + self->position.z
    };
    D3DXMatrixLookAtLH(&self->view, &eye, &at, &self->up);
    D3DXMatrixPerspectiveFovLH(&self->projection, self->fieldOfView,
        static_cast<float>(self->viewport.Width) / static_cast<float>(self->viewport.Height),
        30.0f, 1800.0f);
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetTransform(D3DTS_VIEW, &self->view);
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetTransform(D3DTS_PROJECTION, &self->projection);
    D3DXVec3Normalize(&self->right, CameraCross(&self->right, &self->targetOffset, &self->up));
    if (g_RenderBatch)
    {
        g_RenderBatch->cameraOffsetX = self->offsetX;
        g_RenderBatch->cameraOffsetY = self->offsetY;
    }
}
