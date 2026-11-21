#ifdef ANIMATION_VM_RENDERER_OWNER_MATCH
// Reconstructed original animation VM renderer translation unit.
// Related C++ helpers share one VC9 LTCG owner so internal register
// calling conventions are inferred from their complete call graph.
#include "AnimationVmDraw.hpp"
#include "CrtRounding.hpp"

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







#include "AnimationVmDraw.hpp"
#include "CrtRounding.hpp"
#include "MainRuntime.hpp"

namespace
{
struct QuadUv { float u, v; };
union QuadColor
{
    unsigned int value;
    struct { unsigned char blue, green, red, alpha; } bytes;
};

struct DrawVmView
{
    unsigned char unknown000[0x2c];
    float angle;
    unsigned char unknown030[0x3c - 0x30];
    float scaleX, scaleY;
    unsigned char unknown044[8];
    float width, height, offsetU, offsetV;
    unsigned char unknown05c[0x70 - 0x5c];
    QuadUv corners[4];
    unsigned char unknown090[0x374 - 0x90];
    QuadColor color, alternateColor;
    unsigned char unknown37c[0x3ac - 0x37c];
    AnimationSpriteView *sprite;
    unsigned char unknown3b0[0x3dc - 0x3b0];
    CameraVector position, offset, extraOffset;
    unsigned char unknown400[4];
    unsigned int flags;
};

struct DrawRendererView
{
    unsigned char unknown000[0x4355bc];
    AnimationDocumentTextureView *texture;
    unsigned char unknown4355c0[2];
    unsigned char geometry;
    unsigned char unknown4355c3[0x7bd88c - 0x4355c3];
    QuadColor tint;
    int tintEnabled;
};

struct DrawCameraRuntimeView
{
    unsigned char unknown000[0x54c];
    CameraView *camera;
};

typedef char DrawVmUvOffset[(offsetof(DrawVmView, corners) == 0x70) ? 1 : -1];
typedef char DrawVmColorOffset[(offsetof(DrawVmView, color) == 0x374) ? 1 : -1];
typedef char DrawVmSpriteOffset[(offsetof(DrawVmView, sprite) == 0x3ac) ? 1 : -1];
typedef char DrawVmPositionOffset[(offsetof(DrawVmView, position) == 0x3dc) ? 1 : -1];
typedef char DrawVmFlagsOffset[(offsetof(DrawVmView, flags) == 0x404) ? 1 : -1];
typedef char DrawRendererGeometryOffset[(offsetof(DrawRendererView, geometry) == 0x4355c2) ? 1 : -1];
typedef char DrawRendererTintOffset[(offsetof(DrawRendererView, tint) == 0x7bd88c) ? 1 : -1];

__forceinline CameraVector *Corner(int index)
{
    return reinterpret_cast<CameraVector *>(&g_AnimationQuad[index].x);
}
}

int __stdcall DrawAnimationVm(RenderBatchView *renderer, AnimationVmView *vm,
    unsigned char flags)
{
    DrawVmView *self = reinterpret_cast<DrawVmView *>(vm);
    DrawRendererView *state = reinterpret_cast<DrawRendererView *>(renderer);
    g_AnimationQuad[0].x += renderer->cameraOffsetX;
    g_AnimationQuad[0].y = renderer->cameraOffsetY + g_AnimationQuad[0].y;
    g_AnimationQuad[1].x += renderer->cameraOffsetX;
    g_AnimationQuad[1].y = renderer->cameraOffsetY + g_AnimationQuad[1].y;
    g_AnimationQuad[2].x += renderer->cameraOffsetX;
    g_AnimationQuad[2].y = renderer->cameraOffsetY + g_AnimationQuad[2].y;
    g_AnimationQuad[3].x += renderer->cameraOffsetX;
    g_AnimationQuad[3].y = renderer->cameraOffsetY + g_AnimationQuad[3].y;
    if (flags & 1)
    {
        // _frnd is an attested existing CRT primitive, not a new approximation.
        float left = static_cast<float>(_frnd(g_AnimationQuad[0].x) - 0.5f);
        float right = static_cast<float>(_frnd(g_AnimationQuad[1].x) - 0.5f);
        float top = static_cast<float>(_frnd(g_AnimationQuad[0].y) - 0.5f);
        float bottom = static_cast<float>(_frnd(g_AnimationQuad[2].y) - 0.5f);
        g_AnimationQuad[2].y = g_AnimationQuad[3].y = bottom;
        g_AnimationQuad[0].y = g_AnimationQuad[1].y = top;
        g_AnimationQuad[1].x = g_AnimationQuad[3].x = right;
        g_AnimationQuad[0].x = g_AnimationQuad[2].x = left;
    }
    g_AnimationQuad[0].u = self->corners[0].u + self->offsetU;
    g_AnimationQuad[0].v = self->corners[0].v + self->offsetV;
    g_AnimationQuad[1].u = self->corners[1].u + self->offsetU;
    g_AnimationQuad[1].v = self->corners[1].v + self->offsetV;
    g_AnimationQuad[2].u = self->corners[2].u + self->offsetU;
    g_AnimationQuad[2].v = self->corners[2].v + self->offsetV;
    g_AnimationQuad[3].u = self->corners[3].u + self->offsetU;
    g_AnimationQuad[3].v = self->corners[3].v + self->offsetV;

    float maxX = g_AnimationQuad[1].x < g_AnimationQuad[0].x ? g_AnimationQuad[0].x : g_AnimationQuad[1].x;
    if (maxX < g_AnimationQuad[2].x) maxX = g_AnimationQuad[2].x;
    if (maxX < g_AnimationQuad[3].x) maxX = g_AnimationQuad[3].x;
    float maxY = g_AnimationQuad[1].y < g_AnimationQuad[0].y ? g_AnimationQuad[0].y : g_AnimationQuad[1].y;
    if (maxY < g_AnimationQuad[2].y) maxY = g_AnimationQuad[2].y;
    if (maxY < g_AnimationQuad[3].y) maxY = g_AnimationQuad[3].y;
    float minX = g_AnimationQuad[0].x < g_AnimationQuad[1].x ? g_AnimationQuad[0].x : g_AnimationQuad[1].x;
    if (g_AnimationQuad[2].x < minX) minX = g_AnimationQuad[2].x;
    if (g_AnimationQuad[3].x < minX) minX = g_AnimationQuad[3].x;
    float minY = g_AnimationQuad[0].y < g_AnimationQuad[1].y ? g_AnimationQuad[0].y : g_AnimationQuad[1].y;
    if (g_AnimationQuad[2].y < minY) minY = g_AnimationQuad[2].y;
    if (g_AnimationQuad[3].y < minY) minY = g_AnimationQuad[3].y;
    const D3DVIEWPORT9 &viewport =
        reinterpret_cast<DrawCameraRuntimeView *>(&g_MainRuntime)->camera->viewport;
    if (static_cast<float>(viewport.X) > maxX || static_cast<float>(viewport.Y) > maxY ||
        static_cast<float>(viewport.X + viewport.Width) < minX ||
        static_cast<float>(viewport.Y + viewport.Height) < minY)
        return 0;

    if (state->texture != self->sprite->texture)
    {
        state->texture = self->sprite->texture;
        FlushRenderBatch(renderer);
        static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetTexture(0, state->texture->texture);
    }
    if (state->geometry != 1)
    {
        FlushRenderBatch(renderer);
        state->geometry = 1;
    }
    if (!(flags & 2))
    {
        QuadColor color;
        color.value = (self->flags & 0x8000) ? self->alternateColor.value : self->color.value;
        if (state->tintEnabled)
        {
            color.bytes.red = static_cast<unsigned char>(ScaleAnimationColorChannel(color.bytes.red, state->tint.bytes.red));
            color.bytes.green = static_cast<unsigned char>(ScaleAnimationColorChannel(color.bytes.green, state->tint.bytes.green));
            color.bytes.blue = static_cast<unsigned char>(ScaleAnimationColorChannel(color.bytes.blue, state->tint.bytes.blue));
            color.bytes.alpha = static_cast<unsigned char>(ScaleAnimationColorChannel(color.bytes.alpha, state->tint.bytes.alpha));
        }
        g_AnimationQuad[0].color = color.value;
        g_AnimationQuad[1].color = color.value;
        g_AnimationQuad[2].color = color.value;
        g_AnimationQuad[3].color = color.value;
    }
    ApplyAnimationVmDrawState(renderer, vm);
    AppendRenderQuad(renderer, g_AnimationQuad);
    return 0;
}

int __stdcall DrawUnrotatedAnimationVm(RenderBatchView *renderer, AnimationVmView *vm)
{
    BuildAnimationVmQuad(vm, Corner(0), Corner(1), Corner(2), Corner(3));
    return DrawAnimationVm(renderer, vm, 1);
}

void __stdcall BuildUnroundedAnimationVmQuad(AnimationVmView *vm, CameraVector *first,
    CameraVector *second, CameraVector *third, CameraVector *fourth)
{
    DrawVmView *self = reinterpret_cast<DrawVmView *>(vm);
    float width = self->width * self->scaleX;
    float height = self->height * self->scaleY;
    float halfHeight = static_cast<float>(height * 0.5);
    switch ((self->flags >> 18) & 3)
    {
    case 0:
        first->x = third->x = static_cast<float>(self->offset.x + self->position.x + self->extraOffset.x - static_cast<float>(width * 0.5));
        second->x = fourth->x = first->x + width;
        break;
    case 1:
        first->x = third->x = self->offset.x + self->position.x + self->extraOffset.x;
        second->x = fourth->x = self->offset.x + self->position.x + self->extraOffset.x + width;
        break;
    case 2:
        first->x = third->x = self->offset.x + self->position.x + self->extraOffset.x - width;
        second->x = fourth->x = self->offset.x + self->position.x + self->extraOffset.x;
        break;
    }
    switch ((self->flags >> 20) & 3)
    {
    case 0:
        first->y = second->y = static_cast<float>(self->offset.y + self->position.y + self->extraOffset.y - halfHeight);
        third->y = fourth->y = first->y + height;
        break;
    case 1:
        first->y = second->y = self->offset.y + self->position.y + self->extraOffset.y;
        third->y = fourth->y = self->offset.y + self->position.y + self->extraOffset.y + height;
        break;
    case 2:
        first->y = second->y = self->offset.y + self->position.y + self->extraOffset.y - height;
        third->y = fourth->y = self->offset.y + self->position.y + self->extraOffset.y;
        break;
    }
    first->z = second->z = third->z = fourth->z =
        self->offset.z + self->position.z + self->extraOffset.z;
}

int __stdcall DrawUnroundedAnimationVm(RenderBatchView *renderer, AnimationVmView *vm)
{
    BuildUnroundedAnimationVmQuad(vm, Corner(0), Corner(1), Corner(2), Corner(3));
    return DrawAnimationVm(renderer, vm, 0);
}


#include "AnimationQuad.hpp"
#include "MainRuntime.hpp"
#include <math.h>

namespace
{
struct QuadVmView
{
    unsigned char unknown000[0x3c];
    float scaleX, scaleY;
    unsigned char unknown044[8];
    float width, height;
    unsigned char unknown054[0x3dc - 0x54];
    CameraVector position, offset, extraOffset;
    unsigned char unknown400[4];
    unsigned int flags;
};

struct QuadDrawStateView
{
    unsigned char unknown000[0xa8];
    unsigned int applications;
    unsigned char unknown0ac[0x4355bc - 0xac];
    void *texture;
    unsigned char blend, unknown4355c1, geometry;
    unsigned char unknown4355c3[3];
    unsigned char pointFilter;
};

typedef char QuadVmPositionOffset[(offsetof(QuadVmView, position) == 0x3dc) ? 1 : -1];
typedef char QuadVmFlagsOffset[(offsetof(QuadVmView, flags) == 0x404) ? 1 : -1];
typedef char QuadBlendOffset[(offsetof(QuadDrawStateView, blend) == 0x4355c0) ? 1 : -1];
typedef char QuadFilterOffset[(offsetof(QuadDrawStateView, pointFilter) == 0x4355c6) ? 1 : -1];
}

float __stdcall FloorAnimationCoordinate(float value)
{
    return static_cast<float>(floor(static_cast<double>(value)));
}

void __stdcall BuildAnimationVmQuad(AnimationVmView *vm, CameraVector *first,
    CameraVector *second, CameraVector *third, CameraVector *fourth)
{
    QuadVmView *self = reinterpret_cast<QuadVmView *>(vm);
    float width = self->width * self->scaleX;
    float height = self->height * self->scaleY;
    float halfHeight = static_cast<float>(height * 0.5);
    switch ((self->flags >> 18) & 3)
    {
    case 0:
        first->x = third->x = FloorAnimationCoordinate(static_cast<float>(
            self->offset.x + self->position.x + self->extraOffset.x -
            static_cast<float>(width * 0.5)));
        second->x = fourth->x = first->x + width;
        break;
    case 1:
        first->x = third->x = self->offset.x + self->position.x + self->extraOffset.x;
        second->x = fourth->x = self->offset.x + self->position.x + self->extraOffset.x + width;
        break;
    case 2:
        first->x = third->x = self->offset.x + self->position.x + self->extraOffset.x - width;
        second->x = fourth->x = self->offset.x + self->position.x + self->extraOffset.x;
        break;
    }
    switch ((self->flags >> 20) & 3)
    {
    case 0:
        first->y = second->y = FloorAnimationCoordinate(static_cast<float>(
            self->offset.y + self->position.y + self->extraOffset.y - halfHeight));
        third->y = fourth->y = first->y + height;
        break;
    case 1:
        first->y = second->y = self->offset.y + self->position.y + self->extraOffset.y;
        third->y = fourth->y = self->offset.y + self->position.y + self->extraOffset.y + height;
        break;
    case 2:
        first->y = second->y = self->offset.y + self->position.y + self->extraOffset.y - height;
        third->y = fourth->y = self->offset.y + self->position.y + self->extraOffset.y;
        break;
    }
    first->z = second->z = third->z = fourth->z =
        self->offset.z + self->position.z + self->extraOffset.z;
}

unsigned int __stdcall ScaleAnimationColorChannel(unsigned char value, unsigned char scale)
{
    unsigned int result = (static_cast<unsigned int>(value) * scale) >> 7;
    if (result >= 256)
        result = 255;
    return result;
}

void __stdcall ApplyAnimationVmDrawState(RenderBatchView *renderer, AnimationVmView *vm)
{
    QuadDrawStateView *state = reinterpret_cast<QuadDrawStateView *>(renderer);
    if (state->blend != ((vm->flags.value >> 4) & 7))
    {
        FlushRenderBatch(renderer);
        unsigned char blend = static_cast<unsigned char>((vm->flags.value >> 4) & 7);
        state->blend = blend;
        switch (blend)
        {
        case 0:
            static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            break;
        case 1:
            static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
            break;
        case 2:
            static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
            static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
            break;
        case 3:
            static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
            static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
            break;
        case 4:
            static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
            static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            break;
        case 5:
            static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
            static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
            break;
        case 6:
            static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCCOLOR);
            static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            break;
        }
    }
    if (state->pointFilter != (vm->flags.value >> 31))
    {
        FlushRenderBatch(renderer);
        unsigned char pointFilter = static_cast<unsigned char>((vm->flags.value >> 31) & 1);
        state->pointFilter = pointFilter;
        if (!pointFilter)
        {
            static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
            static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        }
        else
        {
            static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
            static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        }
    }
    ++state->applications;
}

int __stdcall AppendRenderQuad(RenderBatchView *renderer, const RenderVertex *quad)
{
    renderer->nextVertex[0] = quad[0];
    renderer->nextVertex[1] = quad[1];
    renderer->nextVertex[2] = quad[2];
    renderer->nextVertex[3] = quad[1];
    renderer->nextVertex[4] = quad[2];
    renderer->nextVertex[5] = quad[3];
    renderer->nextVertex += 6;
    ++renderer->quadCount;
    return 0;
}


#include "AnimationVmDraw.hpp"
#include <math.h>
#pragma intrinsic(sin, cos)

namespace
{
// Physical VM fields consumed by the rotated draw path. The shared VM view
// intentionally does not give names to every animation/rendering field.
struct RotatedDrawVmView
{
    unsigned char unknown000[0x2c];
    float angle;
    unsigned char unknown030[0x3c - 0x30];
    float scaleX, scaleY;
    unsigned char unknown044[0x4c - 0x44];
    float width, height;
    unsigned char unknown054[0x3dc - 0x54];
    CameraVector position, offset, extraOffset;
    unsigned char unknown400[4];
    unsigned int flags;
};
typedef char RotatedDrawAngleOffset[(offsetof(RotatedDrawVmView, angle) == 0x2c) ? 1 : -1];
typedef char RotatedDrawPositionOffset[(offsetof(RotatedDrawVmView, position) == 0x3dc) ? 1 : -1];
typedef char RotatedDrawFlagsOffset[(offsetof(RotatedDrawVmView, flags) == 0x404) ? 1 : -1];
}

// Full TH11 004503D0..0045060A rotated quad: four anchored local corners,
// one sine/cosine pair, a rotation around the summed VM position and shared Z.
void __stdcall BuildRotatedAnimationVmQuad(AnimationVmView *vm, CameraVector *first,
    CameraVector *second, CameraVector *third, CameraVector *fourth)
{
    const RotatedDrawVmView *self = reinterpret_cast<const RotatedDrawVmView *>(vm);
    float angle = self->angle;
    float cosine = static_cast<float>(cos(angle));
    float sine = static_cast<float>(sin(angle));
    float centerX = self->offset.x + self->position.x + self->extraOffset.x;
    float centerY = self->offset.y + self->position.y + self->extraOffset.y;
    float width = self->width * self->scaleX;
    float height = self->height * self->scaleY;
    float local_20, local_1c, local_18, local_14;
    float local_10, local_c, local_8, local_4;

    switch ((self->flags >> 18) & 3)
    {
    case 0:
        local_10 = static_cast<float>(-width * 0.5);
        local_c = static_cast<float>(width * 0.5);
        local_8 = local_10;
        local_4 = static_cast<float>(width * 0.5);
        break;
    case 1:
        local_8 = 0.0f;
        local_10 = local_8;
        local_c = width;
        local_4 = width;
        break;
    case 2:
        local_10 = -width;
        local_4 = 0.0f;
        local_c = local_4;
        local_8 = local_10;
        break;
    }
    switch ((self->flags >> 20) & 3)
    {
    case 0:
        local_20 = static_cast<float>(-height * 0.5);
        local_18 = static_cast<float>(height * 0.5);
        local_1c = local_20;
        local_14 = local_18;
        break;
    case 1:
        local_1c = 0.0f;
        local_20 = 0.0f;
        local_18 = height;
        local_14 = height;
        break;
    case 2:
        local_20 = -height;
        local_14 = 0.0f;
        local_18 = 0.0f;
        local_1c = local_20;
        break;
    }

    first->x = centerX + (cosine * local_10 - sine * local_20);
    first->y = centerY + (cosine * local_20 + sine * local_10);
    second->x = centerX + (local_c * cosine - local_1c * sine);
    second->y = centerY + (local_1c * cosine + local_c * sine);
    third->x = centerX + (local_8 * cosine - local_18 * sine);
    third->y = centerY + (local_18 * cosine + local_8 * sine);
    fourth->x = centerX + (local_4 * cosine - local_14 * sine);
    fourth->y = centerY + (local_14 * cosine + local_4 * sine);
    float z = self->offset.z + self->position.z + self->extraOffset.z;
    fourth->z = third->z = second->z = first->z = z;
}

int __stdcall DrawAdaptiveAnimationVm(RenderBatchView *renderer, AnimationVmView *vm)
{
    if (reinterpret_cast<DrawVmView *>(vm)->angle == 0.0f)
    {
        BuildAnimationVmQuad(vm, Corner(0), Corner(1), Corner(2), Corner(3));
        return DrawAnimationVm(renderer, vm, 1);
    }
    else
    {
        BuildRotatedAnimationVmQuad(vm, Corner(0), Corner(1), Corner(2), Corner(3));
        return DrawAnimationVm(renderer, vm, 0);
    }
}

#include "AnimationProjectedQuad.hpp"
#include "MainRuntime.hpp"
#include <math.h>

namespace
{
struct ProjectedVmView
{
    unsigned char unknown000[0x24];
    CameraVector rotation;
    unsigned char unknown030[0x3c - 0x30];
    float scaleX, scaleY;
    unsigned char unknown044[0x2b4 - 0x44];
    D3DMATRIX base, transformed;
    unsigned char unknown334[0x374 - 0x334];
    unsigned int color, alternateColor;
    unsigned char unknown37c[0x3dc - 0x37c];
    CameraVector position, offset, extraOffset;
    unsigned char unknown400[4];
    unsigned int flags;
};
union ProjectedColor
{
    unsigned int value;
    struct { unsigned char blue, green, red, alpha; } bytes;
};
struct ProjectedFogView
{
    float start, end, blue, green, red, alpha;
    unsigned int packed;
};
struct ProjectedRuntimeView
{
    unsigned char unknown000[0x300];
    ProjectedFogView fog;
    unsigned char unknown31c[0x54c - 0x31c];
    CameraView *camera;
};
struct ProjectedCameraView
{
    CameraVector position;
    unsigned char unknown00c[0xfc - 0xc];
    float fogStart, fogEnd;
};
struct ProjectedRendererView
{
    unsigned char unknown000[0x435140];
    D3DMATRIX world;
    unsigned char unknown435180[0x7bd88c - 0x435180];
    ProjectedColor tint;
    int tintEnabled;
};
typedef char ProjectedVmBaseOffset[(offsetof(ProjectedVmView, base) == 0x2b4) ? 1 : -1];
typedef char ProjectedVmMatrixOffset[(offsetof(ProjectedVmView, transformed) == 0x2f4) ? 1 : -1];
typedef char ProjectedVmPositionOffset[(offsetof(ProjectedVmView, position) == 0x3dc) ? 1 : -1];
typedef char ProjectedVmFlagsOffset[(offsetof(ProjectedVmView, flags) == 0x404) ? 1 : -1];
typedef char ProjectedRuntimeCameraOffset[(offsetof(ProjectedRuntimeView, camera) == 0x54c) ? 1 : -1];
typedef char ProjectedCameraFogOffset[(offsetof(ProjectedCameraView, fogStart) == 0xfc) ? 1 : -1];
typedef char ProjectedRendererWorldOffset[(offsetof(ProjectedRendererView, world) == 0x435140) ? 1 : -1];
typedef char ProjectedFogSize[(sizeof(ProjectedFogView) == 28) ? 1 : -1];
typedef char ProjectedRendererTintOffset[(offsetof(ProjectedRendererView, tint) == 0x7bd88c) ? 1 : -1];

__forceinline float ProjectedDistanceSquared(const CameraVector &value)
{
    return value.x * value.x + value.y * value.y + value.z * value.z;
}

__forceinline unsigned int ScaleProjectedChannel(unsigned char value, unsigned char scale)
{
    unsigned int result = (static_cast<unsigned int>(value) * scale) >> 7;
    if (result >= 256) result = 255;
    return result;
}

__forceinline ProjectedRuntimeView &ProjectedRuntime()
{
    return *reinterpret_cast<ProjectedRuntimeView *>(&g_MainRuntime);
}
__forceinline CameraVector *ProjectedCorner(int index)
{
    return reinterpret_cast<CameraVector *>(&g_AnimationQuad[index].x);
}
}

float __stdcall AnimationDistanceRoot(float squaredDistance)
{
    return static_cast<float>(sqrt(static_cast<double>(squaredDistance)));
}

int __stdcall BuildProjectedAnimationVmQuad(RenderBatchView *renderer, AnimationVmView *vm)
{
    ProjectedVmView *self = reinterpret_cast<ProjectedVmView *>(vm);
    if (!(self->flags & 0x4000) && (self->flags & 0x0c))
    {
        self->transformed = self->base;
        self->transformed._11 = self->scaleX * self->transformed._11;
        self->transformed._22 = self->scaleY * self->transformed._22;
        self->flags &= ~8u;
        D3DMATRIX rotation;
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
    D3DMATRIX world = self->transformed;
    world._41 = self->offset.x + self->position.x + self->extraOffset.x + world._41;
    world._42 = self->offset.y + self->position.y + self->extraOffset.y + world._42;
    world._43 = self->offset.z + self->position.z + self->extraOffset.z;
    CameraVector corners[4];
    switch ((self->flags >> 18) & 3)
    {
    case 0: corners[0].x = corners[2].x = -128.0f; corners[1].x = corners[3].x = 128.0f; break;
    case 1: corners[0].x = corners[2].x = 0.0f; corners[1].x = corners[3].x = 256.0f; break;
    case 2: corners[0].x = corners[2].x = -256.0f; corners[1].x = corners[3].x = 0.0f; break;
    }
    switch ((self->flags >> 20) & 3)
    {
    case 0: corners[0].y = corners[1].y = -128.0f; corners[2].y = corners[3].y = 128.0f; break;
    case 1: corners[0].y = corners[1].y = 0.0f; corners[2].y = corners[3].y = 256.0f; break;
    case 2: corners[0].y = corners[1].y = -256.0f; corners[2].y = corners[3].y = 0.0f; break;
    }
    corners[0].z = corners[1].z = corners[2].z = corners[3].z = 0.0f;
    D3DXVec3Project(ProjectedCorner(0), &corners[0], &ProjectedRuntime().camera->viewport,
        &ProjectedRuntime().camera->projection, &ProjectedRuntime().camera->view, &world);
    D3DXVec3Project(ProjectedCorner(1), &corners[1], &ProjectedRuntime().camera->viewport,
        &ProjectedRuntime().camera->projection, &ProjectedRuntime().camera->view, &world);
    D3DXVec3Project(ProjectedCorner(2), &corners[2], &ProjectedRuntime().camera->viewport,
        &ProjectedRuntime().camera->projection, &ProjectedRuntime().camera->view, &world);
    D3DXVec3Project(ProjectedCorner(3), &corners[3], &ProjectedRuntime().camera->viewport,
        &ProjectedRuntime().camera->projection, &ProjectedRuntime().camera->view, &world);
    reinterpret_cast<ProjectedRendererView *>(renderer)->world = world;
    return 0;
}

int __stdcall DrawProjectedAnimationVm(RenderBatchView *renderer, AnimationVmView *vm)
{
    BuildProjectedAnimationVmQuad(renderer, vm);
    int result = DrawAnimationVm(renderer, vm, 0);
    g_AnimationQuad[0].rhw = g_AnimationQuad[1].rhw =
        g_AnimationQuad[2].rhw = g_AnimationQuad[3].rhw = 1.0f;
    return result;
}

int __stdcall DrawBillboardAnimationVm(RenderBatchView *renderer, AnimationVmView *vm)
{
    if (BuildBillboardAnimationVmQuad(vm))
        return -1;
    return DrawAnimationVm(renderer, vm, 0);
}

int __stdcall DrawFoggedBillboardAnimationVm(RenderBatchView *renderer, AnimationVmView *vm)
{
    if (BuildBillboardAnimationVmQuad(vm))
        return -1;
    ProjectedVmView *self = reinterpret_cast<ProjectedVmView *>(vm);
    ProjectedRendererView *state = reinterpret_cast<ProjectedRendererView *>(renderer);
    ProjectedCameraView *camera = reinterpret_cast<ProjectedCameraView *>(ProjectedRuntime().camera);
    float fogRange = camera->fogStart - camera->fogEnd;
    ProjectedColor color;
    color.value = (self->flags & 0x8000) ? self->alternateColor : self->color;
    CameraVector position;
    position.x = self->offset.x + self->position.x;
    position.y = self->offset.y + self->position.y;
    position.z = self->offset.z + self->position.z;
    CameraVector translated;
    translated.x = self->extraOffset.x + position.x;
    translated.y = self->extraOffset.y + position.y;
    translated.z = self->extraOffset.z + position.z;
    position.x = translated.x - camera->position.x;
    position.y = translated.y - camera->position.y;
    position.z = translated.z - camera->position.z;
    float distance = AnimationDistanceRoot(ProjectedDistanceSquared(position));
    if (state->tintEnabled)
    {
        color.bytes.red = static_cast<unsigned char>(ScaleProjectedChannel(color.bytes.red, state->tint.bytes.red));
        color.bytes.green = static_cast<unsigned char>(ScaleProjectedChannel(color.bytes.green, state->tint.bytes.green));
        color.bytes.blue = static_cast<unsigned char>(ScaleProjectedChannel(color.bytes.blue, state->tint.bytes.blue));
        color.bytes.alpha = static_cast<unsigned char>(ScaleProjectedChannel(color.bytes.alpha, state->tint.bytes.alpha));
    }
    if (ProjectedRuntime().fog.start < distance)
    {
        distance = (ProjectedRuntime().fog.start - distance) / fogRange;
        if (distance >= 1.0f)
            return -1;
        unsigned char *bytes = reinterpret_cast<unsigned char *>(&g_AnimationQuad[0].color);
        bytes[0] = static_cast<unsigned char>(color.bytes.blue - static_cast<int>(
            (color.bytes.blue - static_cast<int>(ProjectedRuntime().fog.blue)) * distance));
        bytes[1] = static_cast<unsigned char>(color.bytes.green - static_cast<int>(
            (color.bytes.green - static_cast<int>(ProjectedRuntime().fog.green)) * distance));
        bytes[2] = static_cast<unsigned char>(color.bytes.red - static_cast<int>(
            (color.bytes.red - static_cast<int>(ProjectedRuntime().fog.red)) * distance));
        bytes[3] = static_cast<unsigned char>(static_cast<int>(color.bytes.alpha * (1.0f - distance)));
    }
    else
        g_AnimationQuad[0].color = color.value;
    g_AnimationQuad[1].color = g_AnimationQuad[0].color;
    g_AnimationQuad[2].color = g_AnimationQuad[0].color;
    g_AnimationQuad[3].color = g_AnimationQuad[0].color;
    return DrawAnimationVm(renderer, vm, 2);
}

int __stdcall DrawAdaptiveUnroundedAnimationVm(RenderBatchView *renderer, AnimationVmView *vm)
{
    ProjectedVmView *self = reinterpret_cast<ProjectedVmView *>(vm);
    if (self->rotation.z == 0.0f)
        BuildUnroundedAnimationVmQuad(vm, ProjectedCorner(0), ProjectedCorner(1), ProjectedCorner(2), ProjectedCorner(3));
    else
        BuildRotatedAnimationVmQuad(vm, ProjectedCorner(0), ProjectedCorner(1), ProjectedCorner(2), ProjectedCorner(3));
    return DrawAnimationVm(renderer, vm, 0);
}

void __stdcall BuildAnimationVmModeQuad(AnimationVmView *vm, CameraVector *quad)
{
    switch ((vm->flags.value >> 22) & 15)
    {
    case 0: BuildAnimationVmQuad(vm, &quad[0], &quad[1], &quad[2], &quad[3]); break;
    case 1: BuildRotatedAnimationVmQuad(vm, &quad[0], &quad[1], &quad[2], &quad[3]); break;
    case 2:
    case 3: BuildUnroundedAnimationVmQuad(vm, &quad[0], &quad[1], &quad[2], &quad[3]); break;
    }
}





#endif
