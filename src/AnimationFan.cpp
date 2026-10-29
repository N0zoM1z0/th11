#include "AnimationFan.hpp"
#include "MainRuntime.hpp"
#include <stdlib.h>

namespace
{
typedef int (__fastcall *FanCallback)(AnimationVmView *vm);
struct FanVmView
{
    unsigned char unknown000[0x374];
    unsigned int color;
    unsigned char unknown378[0x3ac - 0x378];
    AnimationSpriteView *sprite;
    unsigned char unknown3b0[0x3dc - 0x3b0];
    CameraVector position, offset;
    unsigned char unknown3f4[0x400 - 0x3f4];
    void *storage;
    unsigned int flags;
    unsigned char unknown408[8];
    FanCallback update, draw;
};
struct FanRendererView
{
    unsigned char unknown000[0x4355bc];
    AnimationDocumentTextureView *texture;
    unsigned char blend, cachedC1, geometry, cachedC3;
};
struct FanStorage
{
    RenderVertex vertices[33];
    float radii[33], radialDeltas[33];
    float driftU, driftV;
    unsigned char unknown4ac[4];
};
typedef char FanColorVertexSize[(sizeof(AnimationColorVertex) == 20) ? 1 : -1];
typedef char FanVmStorageOffset[(offsetof(FanVmView, storage) == 0x400) ? 1 : -1];
typedef char FanVmUpdateOffset[(offsetof(FanVmView, update) == 0x410) ? 1 : -1];
typedef char FanVmDrawOffset[(offsetof(FanVmView, draw) == 0x414) ? 1 : -1];
typedef char FanStorageSize[(sizeof(FanStorage) == 0x4b0) ? 1 : -1];
typedef char FanRadiusOffset[(offsetof(FanStorage, radii) == 0x39c) ? 1 : -1];
typedef char FanRadialDeltaOffset[(offsetof(FanStorage, radialDeltas) == 0x420) ? 1 : -1];
typedef char FanDriftOffset[(offsetof(FanStorage, driftU) == 0x4a4) ? 1 : -1];

__forceinline IDirect3DDevice9 *Device()
{
    return static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice);
}
__forceinline void OffsetFanVertex(RenderVertex &vertex, FanVmView *self)
{
    CameraVector offset;
    offset.x = self->offset.x + self->position.x;
    offset.y = self->offset.y + self->position.y;
    offset.z = self->offset.z + self->position.z;
    vertex.x += offset.x;
    vertex.y += offset.y;
    vertex.z += offset.z;
}
__forceinline void ShiftFanU(FanStorage *state)
{
    for (int i = 0; i < 33; ++i) state->vertices[i].u += 1.0f;
}
__forceinline void ShiftFanV(FanStorage *state)
{
    for (int i = 0; i < 33; ++i) state->vertices[i].v += 1.0f;
}
__forceinline float SignedFanRandom()
{
    return static_cast<float>(static_cast<float>(RandomNext32(&g_TextRandomState)) *
        (1.0 / 2147483648.0) - 1.0);
}
}

int __stdcall DrawAnimationQuadVertices(RenderBatchView *renderer, AnimationVmView *vm,
    const RenderVertex *quad)
{
    FanVmView *self = reinterpret_cast<FanVmView *>(vm);
    FanRendererView *state = reinterpret_cast<FanRendererView *>(renderer);
    if (!(self->flags & 1)) return -1;
    if (!(self->flags & 2)) return -1;
    if (!reinterpret_cast<const unsigned char *>(&self->color)[3]) return -1;
    if (state->texture != self->sprite->texture)
    {
        state->texture = self->sprite->texture;
        FlushRenderBatch(renderer);
        Device()->SetTexture(0, state->texture->texture);
    }
    if (state->geometry != 1)
    {
        FlushRenderBatch(renderer);
        state->geometry = 1;
    }
    ApplyAnimationVmDrawState(renderer, vm);
    AppendRenderQuad(renderer, quad);
    return 0;
}

int __stdcall DrawAnimationColoredStrip(RenderBatchView *renderer, AnimationVmView *vm,
    const AnimationColorVertex *vertices, unsigned int count)
{
    FanRendererView *state = reinterpret_cast<FanRendererView *>(renderer);
    if (renderer->quadCount) FlushRenderBatch(renderer);
    if (state->geometry != 4)
    {
        Device()->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
        state->geometry = 4;
    }
    ApplyAnimationVmDrawState(renderer, vm);
    Device()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    Device()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    Device()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    Device()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    FlushRenderBatch(g_RenderBatch);
    Device()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    Device()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, count - 2, vertices, sizeof(AnimationColorVertex));
    FanRendererView *global = reinterpret_cast<FanRendererView *>(g_RenderBatch);
    global->geometry = 0xff;
    global->cachedC1 = 0xff;
    global->blend = 7;
    global->cachedC3 = 0xff;
    Device()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    Device()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    Device()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    Device()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    return 0;
}

int __stdcall DrawAnimationColoredFan(RenderBatchView *renderer, AnimationVmView *vm,
    const AnimationColorVertex *vertices, unsigned int count)
{
    FanRendererView *state = reinterpret_cast<FanRendererView *>(renderer);
    if (renderer->quadCount) FlushRenderBatch(renderer);
    if (state->geometry != 4)
    {
        Device()->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
        state->geometry = 4;
    }
    ApplyAnimationVmDrawState(renderer, vm);
    Device()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    Device()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    Device()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    Device()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    FlushRenderBatch(g_RenderBatch);
    Device()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    Device()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, count - 2, vertices, sizeof(AnimationColorVertex));
    FanRendererView *global = reinterpret_cast<FanRendererView *>(g_RenderBatch);
    global->geometry = 0xff;
    global->cachedC1 = 0xff;
    global->blend = 7;
    global->cachedC3 = 0xff;
    Device()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    Device()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    Device()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    Device()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    return 0;
}

unsigned int __stdcall IsAnimationVmActive(AnimationVmView *vm)
{
    return vm->flags.value & 1u;
}

float __stdcall AnimationVectorLength(const CameraVector *value)
{
    return AnimationDistanceRoot(static_cast<float>(
        value->x * value->x + value->y * value->y + value->z * value->z));
}

int __stdcall InitializeFanAnimationVm(AnimationVmView *vm)
{
    FanVmView *self = reinterpret_cast<FanVmView *>(vm);
    if (self->storage)
    {
        free(self->storage);
        self->storage = 0;
    }
    FanStorage *state = static_cast<FanStorage *>(malloc(sizeof(FanStorage)));
    self->storage = state;
    self->update = UpdateFanAnimationVm;
    self->draw = DrawFanAnimationVm;
    state->driftU = static_cast<float>(SignedFanRandom() * static_cast<double>(1.0f / 120.0f));
    state->driftV = static_cast<float>(SignedFanRandom() * static_cast<double>(1.0f / 120.0f));
    const float pi = 3.1415927410125732f;
    float angle = -pi;
    CameraVector origin;
    origin.x = self->position.x + self->offset.x;
    origin.y = self->offset.y + self->position.y;
    origin.z = self->offset.z + self->position.z;
    *reinterpret_cast<CameraVector *>(&state->vertices[0].x) = origin;
    state->vertices[0].rhw = 1.0f;
    state->vertices[0].u = state->vertices[0].v = 0.5f;
    float radialDelta = static_cast<float>(SignedFanRandom() * static_cast<double>(1.0f / 15.0f));
    for (int i = 1; i < 32; ++i)
    {
        if (angle >= static_cast<double>(pi))
            angle = static_cast<float>(angle - static_cast<double>(2.0f * pi));
        RenderVertex &vertex = state->vertices[i];
        vertex.rhw = 1.0f;
        AnimationPolarVector uv;
        uv.SetPolar(angle, 0.5f);
        vertex.u = static_cast<float>(uv.x + 0.5);
        vertex.v = static_cast<float>(uv.y + 0.5);
        vertex.z = 0.0f;
        float radiusOffset = static_cast<float>(SignedFanRandom() * 8.0);
        state->radii[i] = static_cast<float>(radiusOffset + 80.0);
        state->radialDeltas[i] = radialDelta;
        float variation = static_cast<float>(SignedFanRandom() * static_cast<double>(1.0f / 30.0f));
        radialDelta += variation;
        if (radialDelta < -1.0f / 15.0f) radialDelta = -1.0f / 15.0f;
        else if (radialDelta > 1.0f / 15.0f) radialDelta = 1.0f / 15.0f;
        reinterpret_cast<AnimationPolarVector *>(&vertex.x)->SetPolar(angle, state->radii[i]);
        OffsetFanVertex(vertex, self);
        angle = static_cast<float>(angle + static_cast<double>(0.20268340408802032f));
    }
    return 0;
}

int __fastcall UpdateFanAnimationVm(AnimationVmView *vm)
{
    FanVmView *self = reinterpret_cast<FanVmView *>(vm);
    FanStorage *state = static_cast<FanStorage *>(self->storage);
    float angle = -3.1415927410125732f;
    CameraVector origin;
    origin.x = self->offset.x + self->position.x;
    origin.y = self->offset.y + self->position.y;
    origin.z = self->offset.z + self->position.z;
    *reinterpret_cast<CameraVector *>(&state->vertices[0].x) = origin;
    state->vertices[0].u += state->driftU;
    if (state->vertices[0].u < 0.0f) ShiftFanU(state);
    state->vertices[0].v += state->driftU;
    if (state->vertices[0].v < 0.0f) ShiftFanV(state);
    state->vertices[0].color = self->color;
    for (int i = 1; i < 32; ++i)
    {
        RenderVertex &vertex = state->vertices[i];
        vertex.u += state->driftU;
        if (vertex.u < 0.0f) ShiftFanU(state);
        vertex.v += state->driftU;
        if (vertex.v < 0.0f) ShiftFanV(state);
        vertex.color = self->color;
        reinterpret_cast<unsigned char *>(&vertex.color)[3] = 0;
        state->radii[i] += state->radialDeltas[i];
        reinterpret_cast<AnimationPolarVector *>(&vertex.x)->SetPolar(angle, state->radii[i]);
        OffsetFanVertex(vertex, self);
        angle = static_cast<float>(angle + static_cast<double>(0.20268340408802032f));
    }
    state->vertices[32] = state->vertices[1];
    return 0;
}

int __fastcall DrawFanAnimationVm(AnimationVmView *vm)
{
    DrawAnimationVertexFan(g_RenderBatch, vm,
        static_cast<RenderVertex *>(reinterpret_cast<FanVmView *>(vm)->storage), 33);
    return 0;
}
