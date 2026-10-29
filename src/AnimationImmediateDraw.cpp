#include "AnimationImmediateDraw.hpp"
#include "MainRuntime.hpp"
#include <math.h>

namespace
{
union ImmediateColor
{
    unsigned int value;
    struct { unsigned char blue, green, red, alpha; } bytes;
};
struct ImmediateUv { float u, v; };
struct ImmediateVmView
{
    unsigned char unknown000[0x24];
    CameraVector rotation;
    unsigned char unknown030[0x3c - 0x30];
    float scaleX, scaleY;
    unsigned char unknown044[8];
    float width, height, offsetU, offsetV;
    unsigned char unknown05c[0x70 - 0x5c];
    ImmediateUv uv[4];
    unsigned char unknown090[0x2b4 - 0x90];
    D3DMATRIX base, transformed, textureTransform;
    ImmediateColor color, alternateColor;
    unsigned char unknown37c[0x3ac - 0x37c];
    AnimationSpriteView *sprite;
    unsigned char unknown3b0[4];
    unsigned int stripPairs;
    unsigned char unknown3b8[0x3dc - 0x3b8];
    CameraVector position, offset, extraOffset;
    RenderVertex *vertices;
    unsigned int flags;
};
struct ImmediateRendererView
{
    unsigned char unknown000[0xa8];
    unsigned int applications;
    unsigned char unknown0ac[0x435140 - 0xac];
    D3DMATRIX world;
    unsigned char unknown435180[0x4355b8 - 0x435180];
    unsigned int textureFactor;
    AnimationDocumentTextureView *texture;
    unsigned char blend, unknown4355c1, geometry;
    unsigned char unknown4355c3[3];
    unsigned char pointFilter, unknown4355c7;
    AnimationSpriteView *sprite;
    IDirect3DVertexBuffer9 *worldVertices;
    AnimationWorldVertex corners[4];
    unsigned int quadCount;
    unsigned char unknown435624[0x7bd88c - 0x435624];
    ImmediateColor tint;
    int tintEnabled;
};
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
typedef char ImmediateVmTextureOffset[(offsetof(ImmediateVmView, textureTransform) == 0x334) ? 1 : -1];
typedef char ImmediateVmColorOffset[(offsetof(ImmediateVmView, color) == 0x374) ? 1 : -1];
typedef char ImmediateVmSpriteOffset[(offsetof(ImmediateVmView, sprite) == 0x3ac) ? 1 : -1];
typedef char ImmediateVmPositionOffset[(offsetof(ImmediateVmView, position) == 0x3dc) ? 1 : -1];
typedef char ImmediateVmVerticesOffset[(offsetof(ImmediateVmView, vertices) == 0x400) ? 1 : -1];
typedef char ImmediateVmFlagsOffset[(offsetof(ImmediateVmView, flags) == 0x404) ? 1 : -1];
typedef char ImmediateWorldVertexSize[(sizeof(AnimationWorldVertex) == 20) ? 1 : -1];
typedef char ImmediateRendererTextureOffset[(offsetof(ImmediateRendererView, texture) == 0x4355bc) ? 1 : -1];
typedef char ImmediateRendererCornersOffset[(offsetof(ImmediateRendererView, corners) == 0x4355d0) ? 1 : -1];
typedef char ImmediateRendererCountOffset[(offsetof(ImmediateRendererView, quadCount) == 0x435620) ? 1 : -1];
typedef char ImmediateRendererTintOffset[(offsetof(ImmediateRendererView, tint) == 0x7bd88c) ? 1 : -1];
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

float __stdcall AnimationExtentMagnitude(float value)
{
    return static_cast<float>(fabs(static_cast<double>(value)));
}

void __stdcall ApplyAnimationVmWorldState(RenderBatchView *renderer, AnimationVmView *vm)
{
    ImmediateVmView *self = reinterpret_cast<ImmediateVmView *>(vm);
    ImmediateRendererView *state = reinterpret_cast<ImmediateRendererView *>(renderer);
    if (state->blend != ((self->flags >> 4) & 7))
    {
        FlushRenderBatch(renderer);
        unsigned char blend = static_cast<unsigned char>((self->flags >> 4) & 7);
        state->blend = blend;
        switch (blend)
        {
        case 0:
            Device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            Device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); break;
        case 1:
            Device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            Device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE); break;
        case 2:
            Device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
            Device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR); break;
        case 3:
            Device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
            Device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO); break;
        case 4:
            Device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
            Device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); break;
        case 5:
            Device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
            Device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO); break;
        case 6:
            Device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCCOLOR);
            Device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); break;
        }
    }
    ImmediateColor color;
    color.value = (self->flags & 0x8000) ? self->alternateColor.value : self->color.value;
    if (state->tintEnabled)
    {
        color.bytes.red = static_cast<unsigned char>(ScaleChannel(color.bytes.red, state->tint.bytes.red));
        color.bytes.green = static_cast<unsigned char>(ScaleChannel(color.bytes.green, state->tint.bytes.green));
        color.bytes.blue = static_cast<unsigned char>(ScaleChannel(color.bytes.blue, state->tint.bytes.blue));
        color.bytes.alpha = static_cast<unsigned char>(ScaleChannel(color.bytes.alpha, state->tint.bytes.alpha));
    }
    if (state->textureFactor != color.value)
    {
        FlushRenderBatch(renderer);
        state->textureFactor = color.value;
        Device()->SetRenderState(D3DRS_TEXTUREFACTOR, color.value);
    }
    if (state->pointFilter != (self->flags >> 31))
    {
        FlushRenderBatch(renderer);
        unsigned char pointFilter = static_cast<unsigned char>((self->flags >> 31) & 1);
        state->pointFilter = pointFilter;
        if (!pointFilter)
        {
            Device()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
            Device()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        }
        else
        {
            Device()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
            Device()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        }
    }
    ++state->applications;
}

int __stdcall DrawWorldAnimationVm(RenderBatchView *renderer, AnimationVmView *vm)
{
    ImmediateVmView *self = reinterpret_cast<ImmediateVmView *>(vm);
    ImmediateRendererView *state = reinterpret_cast<ImmediateRendererView *>(renderer);
    D3DMATRIX world, rotation, texture;
    if (!(self->flags & 1)) return -1;
    if (!(self->flags & 2)) return -1;
    if (!self->color.bytes.alpha) return -1;
    if (renderer->quadCount)
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
    switch ((self->flags >> 18) & 3)
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
        {
            float extent = AnimationExtentMagnitude(static_cast<float>(self->width * self->scaleX * 0.5));
            world._41 = self->offset.x + self->position.x + self->extraOffset.x + extent;
        }
        break;
    }
    switch ((self->flags >> 20) & 3)
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
        {
            float extent = AnimationExtentMagnitude(static_cast<float>(self->height * self->scaleY * 0.5));
            world._42 = self->offset.y + self->position.y + self->extraOffset.y + extent;
        }
        break;
    }
    world._43 = self->offset.z + self->position.z;
    ApplyAnimationVmWorldState(renderer, vm);
    world._43 = self->offset.z + self->position.z + self->extraOffset.z;
    Device()->SetTransform(D3DTS_WORLD, &world);
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

int __stdcall DrawAnimationVmByMode(RenderBatchView *renderer, AnimationVmView *vm)
{
    ImmediateVmView *self = reinterpret_cast<ImmediateVmView *>(vm);
    if (!(self->flags & 1)) return -1;
    if (!(self->flags & 2)) return -1;
    if (!self->color.bytes.alpha) return -1;
    switch ((self->flags >> 22) & 15)
    {
    case 0: return DrawUnrotatedAnimationVm(renderer, vm);
    case 1: return DrawAdaptiveAnimationVm(renderer, vm);
    case 2: return DrawUnroundedAnimationVm(renderer, vm);
    case 3: return DrawAdaptiveUnroundedAnimationVm(renderer, vm);
    case 4: return DrawBillboardAnimationVm(renderer, vm);
    case 5: return DrawProjectedAnimationVm(renderer, vm);
    case 6: return DrawFoggedBillboardAnimationVm(renderer, vm);
    case 7: return DrawFoggedProjectedAnimationVm(renderer, vm);
    case 8: return DrawWorldAnimationVm(renderer, vm);
    case 9:
    case 12:
    case 13: return DrawAnimationVertexStrip(renderer, vm, self->vertices, self->stripPairs * 2u);
    case 11: return DrawAnimationVertexFan(renderer, vm, self->vertices, self->stripPairs * 2u);
    default: return 0;
    }
}
