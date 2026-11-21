#include "AnimationImmediateDraw.hpp"
#include "AnimationImmediateDrawViews.hpp"
#include "AnimationExecution.hpp"
#include "MainRuntime.hpp"
#include <math.h>

namespace
{
struct ImmediateFogView
{
    float start, end, blue, green, red, alpha;
    unsigned int packed;
};
struct ImmediateCameraView
{
    unsigned char unknown000[0xfc];
    float fogStart, fogEnd;
    unsigned char unknown104[0x114 - 0x104];
    unsigned int fogColor;
};
struct ImmediateRuntimeView
{
    unsigned char unknown000[0x204];
    CameraVector fogEye;
    unsigned char unknown210[0x300 - 0x210];
    ImmediateFogView fog;
    unsigned char unknown31c[0x54c - 0x31c];
    ImmediateCameraView *camera;
};
typedef char ImmediateRuntimeCameraOffset[(offsetof(ImmediateRuntimeView, camera) == 0x54c) ? 1 : -1];

__forceinline ImmediateRuntimeView &Runtime()
{
    return *reinterpret_cast<ImmediateRuntimeView *>(&g_MainRuntime);
}
__forceinline IDirect3DDevice9 *Device()
{
    return static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice);
}
__forceinline unsigned int ScaleChannel(unsigned char value, unsigned char scale)
{
    unsigned int result = (static_cast<unsigned int>(value) * scale) >> 7;
    if (result >= 256) result = 255;
    return result;
}
}

int ImmediateVmView::DrawWorld(RenderBatchView *renderer)
{
    ImmediateVmView *self = this;
    D3DMATRIX world, rotation, texture;
    if (!(self->flags & 1)) return -1;
    if (!(self->flags & 2)) return -1;
    if (!self->color.bytes.alpha) return -1;
    if (reinterpret_cast<ImmediateRendererView *>(renderer)->quadCount)
        FlushRenderBatch(renderer);
    if (!(self->flags & 0x4000) && (self->flags & 0x0c))
    {
        self->transformed = self->base;
        self->transformed._11 = self->scaleX * self->transformed._11;
        self->transformed._22 = self->scaleY * self->transformed._22;
        self->flags &= ~8u;
        if (self->rotation.x != 0.0f)
        {
            D3DXMatrixRotationX(&rotation, self->rotation.x);
            D3DXMatrixMultiply(&self->transformed, &self->transformed, &rotation);
        }
        if (self->rotation.y != 0.0f)
        {
            D3DXMatrixRotationY(&rotation, self->rotation.y);
            D3DXMatrixMultiply(&self->transformed, &self->transformed, &rotation);
        }
        if (self->rotation.z != 0.0f)
        {
            D3DXMatrixRotationZ(&rotation, self->rotation.z);
            D3DXMatrixMultiply(&self->transformed, &self->transformed, &rotation);
        }
        self->flags &= ~4u;
    }
    world = self->transformed;
    unsigned int flags = self->flags;
    switch ((flags >> 18) & 3)
    {
    case 0: world._41 = self->offset.x + self->position.x + self->extraOffset.x; break;
    case 1:
        {
            double translation = self->offset.x + self->position.x + self->extraOffset.x;
            world._41 = static_cast<float>(translation - AnimationExtentMagnitude(
                static_cast<float>(self->width * self->scaleX * 0.5)));
        }
        break;
    case 2:
        world._41 = self->offset.x + self->position.x + self->extraOffset.x
            + AnimationExtentMagnitude(static_cast<float>(self->width * self->scaleX * 0.5));
        break;
    }
    switch ((flags >> 20) & 3)
    {
    case 0: world._42 = self->offset.y + self->position.y + self->extraOffset.y; break;
    case 1:
        {
            double translation = self->offset.y + self->position.y + self->extraOffset.y;
            world._42 = static_cast<float>(translation - AnimationExtentMagnitude(
                static_cast<float>(self->height * self->scaleY * 0.5)));
        }
        break;
    case 2:
        world._42 = self->offset.y + self->position.y + self->extraOffset.y
            + AnimationExtentMagnitude(static_cast<float>(self->height * self->scaleY * 0.5));
        break;
    }
    world._43 = self->offset.z + self->position.z;
    renderer->ApplyAnimationVmWorldState(reinterpret_cast<AnimationVmView *>(self));
    world._43 = self->offset.z + self->position.z + self->extraOffset.z;
    Device()->SetTransform(D3DTS_WORLD, &world);
    ImmediateRendererView *state = reinterpret_cast<ImmediateRendererView *>(renderer);
    if (state->texture != self->sprite->texture)
    {
        state->texture = self->sprite->texture;
        Device()->SetTexture(0, state->texture->texture);
    }
    // The original tests offsetU twice here; offsetV alone does not invalidate.
    if (state->sprite != self->sprite || self->offsetU != 0.0f || self->offsetU != 0.0f)
    {
        state->sprite = self->sprite;
        texture = self->textureTransform;
        texture._31 = self->uv[0].u + self->offsetU;
        texture._32 = self->uv[0].v + self->offsetV;
        Device()->SetTransform(D3DTS_TEXTURE0, &texture);
    }
    if (state->geometry != 2)
    {
        Device()->SetStreamSource(0, state->worldVertices, 0, sizeof(AnimationWorldVertex));
        Device()->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
        Device()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
        Device()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
        state->geometry = 2;
    }
    Device()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    return 0;
}

int __stdcall DrawFoggedProjectedAnimationVm(RenderBatchView *renderer, AnimationVmView *vm)
{
    BuildProjectedAnimationVmQuad(renderer, vm);
    ImmediateVmView *self = reinterpret_cast<ImmediateVmView *>(vm);
    ImmediateRendererView *state = reinterpret_cast<ImmediateRendererView *>(renderer);
    float fogRange = Runtime().camera->fogStart - Runtime().camera->fogEnd;
    ImmediateColor color;
    color.value = (self->flags & 0x8000) ? self->alternateColor.value : self->color.value;
    AnimationTransformVector transformed[4];
    for (int i = 0; i < 4; ++i)
    {
        D3DXVec3Transform(&transformed[i], &state->corners[i].position, &state->world);
        CameraVector delta;
        delta.x = transformed[i].x - Runtime().fogEye.x;
        delta.y = transformed[i].y - Runtime().fogEye.y;
        delta.z = transformed[i].z - Runtime().fogEye.z;
        float distance = AnimationDistanceRoot(static_cast<float>(
            delta.x * delta.x + delta.y * delta.y + delta.z * delta.z));
        if (Runtime().fog.start < distance)
        {
            distance = (Runtime().fog.start - distance) / fogRange;
            if (distance >= 1.0f)
            {
                g_AnimationQuad[i].color = Runtime().camera->fogColor;
                reinterpret_cast<unsigned char *>(&g_AnimationQuad[i].color)[3] = color.bytes.alpha;
            }
            else
            {
                unsigned char *bytes = reinterpret_cast<unsigned char *>(&g_AnimationQuad[i].color);
                bytes[0] = static_cast<unsigned char>(color.bytes.blue - static_cast<int>(
                    (color.bytes.blue - Runtime().fog.blue) * distance));
                bytes[1] = static_cast<unsigned char>(color.bytes.green - static_cast<int>(
                    (color.bytes.green - Runtime().fog.green) * distance));
                bytes[3] = color.bytes.alpha;
                bytes[2] = static_cast<unsigned char>(color.bytes.red - static_cast<int>(
                    (color.bytes.red - Runtime().fog.red) * distance));
            }
        }
        else
            g_AnimationQuad[i].color = color.value;
    }
    int result = DrawAnimationVm(renderer, vm, 2);
    g_AnimationQuad[0].rhw = g_AnimationQuad[1].rhw =
        g_AnimationQuad[2].rhw = g_AnimationQuad[3].rhw = 1.0f;
    return result;
}

int __stdcall DrawAnimationVertexStrip(RenderBatchView *renderer, AnimationVmView *vm,
    const RenderVertex *vertices, unsigned int vertexCount)
{
    ImmediateVmView *self = reinterpret_cast<ImmediateVmView *>(vm);
    ImmediateRendererView *state = reinterpret_cast<ImmediateRendererView *>(renderer);
    if (!(self->flags & 1)) return -1;
    if (!(self->flags & 2)) return -1;
    if (!self->color.bytes.alpha) return -1;
    if (renderer->quadCount)
        FlushRenderBatch(renderer);
    if (state->texture != self->sprite->texture)
    {
        state->texture = self->sprite->texture;
        Device()->SetTexture(0, state->texture->texture);
    }
    if (state->geometry != 3)
    {
        Device()->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
        state->geometry = 3;
    }
    ApplyAnimationVmDrawState(renderer, vm);
    Device()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    Device()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    Device()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, vertexCount - 2, vertices, sizeof(RenderVertex));
    return 0;
}

int __stdcall DrawAnimationVertexFan(RenderBatchView *renderer, AnimationVmView *vm,
    const RenderVertex *vertices, unsigned int vertexCount)
{
    ImmediateVmView *self = reinterpret_cast<ImmediateVmView *>(vm);
    ImmediateRendererView *state = reinterpret_cast<ImmediateRendererView *>(renderer);
    if (renderer->quadCount)
        FlushRenderBatch(renderer);
    if (state->geometry != 3)
    {
        Device()->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
        state->geometry = 3;
    }
    ApplyAnimationVmDrawState(renderer, vm);
    if (state->texture != self->sprite->texture)
    {
        state->texture = self->sprite->texture;
        Device()->SetTexture(0, state->texture->texture);
    }
    FlushRenderBatch(g_RenderBatch);
    Device()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    Device()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    Device()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    Device()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, vertexCount - 2, vertices, sizeof(RenderVertex));
    return 0;
}

int __stdcall UpdateStripVertexUv(AnimationVmView *vm, RenderVertex *vertices, int count)
{
    if (count < 3) return -1;
    ImmediateVmView *self = reinterpret_cast<ImmediateVmView *>(vm);
    float bottom = self->offsetV + self->uv[2].v;
    float side = self->uv[0].u + self->offsetU;
    float span = self->uv[2].v - self->uv[0].v;
    int intervals = static_cast<int>(static_cast<unsigned int>(count) + 1u) / 2 - 1;
    float step = span / intervals;
    float coordinate = bottom;
    for (int index = 0; index < count; index += 2)
    {
        RenderVertex *vertex = &vertices[index];
        vertex->v = coordinate;
        vertex->u = side;
        vertex->color = self->color.value;
        vertex->rhw = 1.0f;
        coordinate -= step;
    }
    side = self->uv[1].u + self->offsetU;
    coordinate = bottom;
    for (int index = 1; index < count; index += 2)
    {
        RenderVertex *vertex = &vertices[index];
        vertex->v = coordinate;
        vertex->u = side;
        vertex->color = self->color.value;
        vertex->rhw = 1.0f;
        coordinate -= step;
    }
    return 0;
}

int __stdcall UpdateTransposedStripVertexUv(AnimationVmView *vm, RenderVertex *vertices, int count)
{
    if (count < 3) return -1;
    ImmediateVmView *self = reinterpret_cast<ImmediateVmView *>(vm);
    float bottom = self->offsetU + self->uv[2].v;
    float side = self->uv[0].u + self->offsetV;
    float span = self->uv[2].v - self->uv[0].v;
    int intervals = static_cast<int>(static_cast<unsigned int>(count) + 1u) / 2 - 1;
    float step = span / intervals;
    float coordinate = bottom;
    for (int index = 0; index < count; index += 2)
    {
        RenderVertex *vertex = &vertices[index];
        vertex->u = coordinate;
        vertex->v = side;
        vertex->color = self->color.value;
        vertex->rhw = 1.0f;
        coordinate -= step;
    }
    side = self->uv[1].u + self->offsetV;
    coordinate = bottom;
    for (int index = 1; index < count; index += 2)
    {
        RenderVertex *vertex = &vertices[index];
        vertex->u = coordinate;
        vertex->v = side;
        vertex->color = self->color.value;
        vertex->rhw = 1.0f;
        coordinate -= step;
    }
    return 0;
}

int __stdcall DrawAnimationVmByMode(AnimationVmView *vm, RenderBatchView *renderer)
{
    ImmediateVmView *self = reinterpret_cast<ImmediateVmView *>(vm);
    register unsigned int flags = self->flags;
    if (!(flags & 1)) return -1;
    if (!(flags & 2)) return -1;
    if (!self->color.bytes.alpha) return -1;
    switch ((flags >> 22) & 15)
    {
    case 0: return DrawUnrotatedAnimationVm(renderer, vm);
    case 1: return DrawAdaptiveAnimationVm(renderer, vm);
    case 4: return DrawBillboardAnimationVm(renderer, vm);
    case 5: return DrawProjectedAnimationVm(renderer, vm);
    case 6: return DrawFoggedBillboardAnimationVm(renderer, vm);
    case 7: return DrawFoggedProjectedAnimationVm(renderer, vm);
    case 8: return self->DrawWorld(renderer);
    case 9:
    case 12:
    case 13: return DrawAnimationVertexStrip(renderer, vm, self->vertices, self->stripPairs * 2u);
    case 11: return DrawAnimationVertexFan(renderer, vm, self->vertices, self->stripPairs * 2u);
    case 2: return DrawUnroundedAnimationVm(renderer, vm);
    case 3: return DrawAdaptiveUnroundedAnimationVm(renderer, vm);
    case 10: return 0;
    default: return 0;
    }
}
