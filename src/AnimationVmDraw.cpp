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

int __stdcall DrawAdaptiveAnimationVm(RenderBatchView *renderer, AnimationVmView *vm)
{
    DrawVmView *self = reinterpret_cast<DrawVmView *>(vm);
    if (self->angle == 0.0f)
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
