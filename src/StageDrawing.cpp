#include "StageDrawing.hpp"
#include "FrameLoop.hpp"
#include <string.h>

static __forceinline StageCameraView &RuntimeStageCamera()
{
    return *reinterpret_cast<StageCameraView *>(
        reinterpret_cast<unsigned char *>(&g_MainRuntime) + 0x434);
}

static __forceinline StageRenderTintView &RenderTint(RenderBatchView *renderer)
{
    return *reinterpret_cast<StageRenderTintView *>(renderer);
}

static __forceinline void StageRenderState(D3DRENDERSTATETYPE state, DWORD value)
{
    FlushRenderBatch(g_RenderBatch);
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetRenderState(state, value);
}

static __forceinline void StageFog(int enabled)
{
    if (FrameRuntime().fogEnabled != enabled)
    {
        FlushRenderBatch(g_RenderBatch);
        FrameRuntime().fogEnabled = enabled;
        static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetRenderState(D3DRS_FOGENABLE, enabled);
    }
}

static __forceinline void StagePerspective()
{
    FrameRuntime().activeCamera = reinterpret_cast<CameraView *>(&RuntimeStageCamera());
    ApplyPerspectiveCamera(&RuntimeStageCamera().view);
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetViewport(&FrameRuntime().activeCamera->viewport);
    FrameRuntime().activeCameraIndex = 2;
}

static __forceinline void StageCopyCamera(StageResourceView *self)
{
    FlushRenderBatch(g_RenderBatch);
    self->camera.view.offsetX = RuntimeStageCamera().view.offsetX;
    self->camera.view.offsetY = RuntimeStageCamera().view.offsetY;
    RuntimeStageCamera() = self->camera;
    StagePerspective();
}

static __forceinline DWORD StageFloatState(const float &value)
{
    DWORD result;
    memcpy(&result, &value, sizeof(result));
    return result;
}

static __forceinline void StageFogParameters(StageResourceView *self)
{
    StageRenderState(D3DRS_FOGCOLOR, self->camera.fogColor);
    StageRenderState(D3DRS_FOGSTART, StageFloatState(self->camera.fogStart));
    StageRenderState(D3DRS_FOGEND, StageFloatState(self->camera.fogEnd));
}

static __forceinline void StageRestoreTint(StageResourceView *self, RenderBatchView *renderer)
{
    RenderTint(renderer).enabled = 0;
    RenderTint(renderer).tint = 0x80808080;
    if (self->accent)
    {
        RenderTint(renderer).enabled = 1;
        RenderTint(renderer).tint = 0xff404040;
    }
}

int __stdcall DrawStage(StageResourceView *self)
{
    if (self->flags2ff0 & 8)
        return 1;
    if (!(self->flags2ff0 & 4) || self->timer2ff4.current < 60)
    {
        StageCopyCamera(self);
        StageRenderState(D3DRS_ZWRITEENABLE, TRUE);
        StageRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
        StageFogParameters(self);
        const D3DVIEWPORT9 &viewport = RuntimeStageCamera().view.viewport;
        if ((self->flags2ff0 & 4) && self->reset300c < 34)
        {
            D3DRECT rect = {viewport.X, viewport.Y,
                viewport.X + viewport.Width, viewport.Y + viewport.Height};
            static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->Clear(
                1, &rect, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        }
        else
        {
            D3DRECT rect = {viewport.X, viewport.Y,
                viewport.X + viewport.Width, viewport.Y + viewport.Height};
            static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->Clear(
                1, &rect, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, self->camera.fogColor, 1.0f, 0);
        }
    }
    if (self->flags2ff0 & 4)
    {
        if (self->timer2ff4.current < 30)
        {
            CreateStageTransition(3, 30, 0, 0, 10);
            self->flags2ff0 |= 1;
            SetStageTimer(&self->timer2ff4, 1);
        }
        else
        {
            reinterpret_cast<unsigned char *>(&self->tint)[3] = 0;
            self->flags2ff0 &= ~1u;
        }
    }
    RenderBatchView *renderer = g_RenderBatch;
    if (self->tint & 0xff000000)
    {
        RenderTint(renderer).enabled = 1;
        RenderTint(renderer).tint = self->tint;
    }
    self->drawOffset.x = self->drawOffset.y = self->drawOffset.z = 0.0f;
    if (self->flags2ff0 & 1)
    {
        if (reinterpret_cast<AnimationVmView *>(&self->vms180[0])->unknown3ac)
        {
            SelectRuntimeCamera(&g_MainRuntime, 2);
            DisableRuntimeFog(&g_MainRuntime);
            FlushRenderBatch(g_RenderBatch);
            StageRenderState(D3DRS_ZWRITEENABLE, FALSE);
            StageEmbeddedVm *vm = self->vms180;
            for (int i = 0; i < 8; ++i, ++vm)
                if (reinterpret_cast<AnimationVmView *>(vm)->unknown3ac)
                    DrawAnimationVm(g_RenderBatch, reinterpret_cast<AnimationVmView *>(vm));
            StageRenderState(D3DRS_ZWRITEENABLE, TRUE);
            StagePerspective();
        }
        StageFog(TRUE);
        DrawStageLayer(self, 0);
        DrawStageLayer(self, 1);
        DrawStageLayer(self, 2);
        DrawStageLayer(self, 3);
        DrawStageLayer(self, 4);
        DrawStageLayer(self, 5);
        DrawStageLayer(self, 6);
        DrawStageLayer(self, 7);
        FlushRenderBatch(g_RenderBatch);
        renderer = g_RenderBatch;
        if (self->drawCount)
            ++self->drawCount;
    }
    StageRestoreTint(self, renderer);
    FlushRenderBatch(renderer);
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    StageRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
    return 1;
}

int __stdcall DrawStageOverlay(StageResourceView *self)
{
    if (self->flags2ff0 & 8)
        return 1;
    if (!(self->flags2ff0 & 4) || self->timer2ff4.current < 60)
    {
        StageCopyCamera(self);
        StageFog(FALSE);
        StageRenderState(D3DRS_ZWRITEENABLE, FALSE);
        StageRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
        DrawAnimationGroup(g_RenderBatch, 25);
        StageRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
        DrawAnimationGroup(g_RenderBatch, 26);
        StageRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
        StageFogParameters(self);
    }
    if ((self->flags2ff0 & 4) && self->timer2ff4.current >= 30)
        reinterpret_cast<unsigned char *>(&self->tint)[3] = 0;
    if (self->flags2ff0 & 1)
    {
        StageRenderState(D3DRS_ZWRITEENABLE, FALSE);
        StageFog(TRUE);
        DrawStageLayer(self, 8);
        DrawStageLayer(self, 9);
        DrawStageLayer(self, 10);
        DrawStageLayer(self, 11);
        FlushRenderBatch(g_RenderBatch);
    }
    RenderBatchView *renderer = g_RenderBatch;
    StageRestoreTint(self, renderer);
    if (self->timer2ff4.current > 0)
    {
        AddStageTimer(&self->timer2ff4, -1.0f);
        if (self->timer2ff4.current <= 0)
        {
            reinterpret_cast<unsigned char *>(&self->tint)[3] = 0xff;
            if (self->flags2ff0 & 2)
                self->flags2ff0 |= 8;
            self->flags2ff0 &= ~6u;
            self->tint = 0x00ffffff;
        }
    }
    FlushRenderBatch(renderer);
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    StageRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
    StageFog(FALSE);
    return 1;
}
