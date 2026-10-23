#include "RenderBatch.hpp"
#include "MainRuntime.hpp"

#include <d3d9.h>

void __stdcall FlushRenderBatch(RenderBatchView *self)
{
    if (self->quadCount != 0)
    {
        static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetTextureStageState(
            0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
        static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetTextureStageState(
            0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
        static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetFVF(
            D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
        static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->DrawPrimitiveUP(
            D3DPT_TRIANGLELIST, self->quadCount * 2, self->batchStart, sizeof(RenderVertex));
        ++self->drawCalls;
        self->batchStart = self->nextVertex;
        self->quadCount = 0;
    }
}
