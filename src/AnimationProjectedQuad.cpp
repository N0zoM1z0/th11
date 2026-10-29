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

__forceinline ProjectedRuntimeView &Runtime()
{
    return *reinterpret_cast<ProjectedRuntimeView *>(&g_MainRuntime);
}
__forceinline CameraVector *Corner(int index)
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
    D3DXVec3Project(Corner(0), &corners[0], &Runtime().camera->viewport,
        &Runtime().camera->projection, &Runtime().camera->view, &world);
    D3DXVec3Project(Corner(1), &corners[1], &Runtime().camera->viewport,
        &Runtime().camera->projection, &Runtime().camera->view, &world);
    D3DXVec3Project(Corner(2), &corners[2], &Runtime().camera->viewport,
        &Runtime().camera->projection, &Runtime().camera->view, &world);
    D3DXVec3Project(Corner(3), &corners[3], &Runtime().camera->viewport,
        &Runtime().camera->projection, &Runtime().camera->view, &world);
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
    ProjectedCameraView *camera = reinterpret_cast<ProjectedCameraView *>(Runtime().camera);
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
    if (Runtime().fog.start < distance)
    {
        distance = (Runtime().fog.start - distance) / fogRange;
        if (distance >= 1.0f)
            return -1;
        unsigned char *bytes = reinterpret_cast<unsigned char *>(&g_AnimationQuad[0].color);
        bytes[0] = static_cast<unsigned char>(color.bytes.blue - static_cast<int>(
            (color.bytes.blue - static_cast<int>(Runtime().fog.blue)) * distance));
        bytes[1] = static_cast<unsigned char>(color.bytes.green - static_cast<int>(
            (color.bytes.green - static_cast<int>(Runtime().fog.green)) * distance));
        bytes[2] = static_cast<unsigned char>(color.bytes.red - static_cast<int>(
            (color.bytes.red - static_cast<int>(Runtime().fog.red)) * distance));
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
        BuildUnroundedAnimationVmQuad(vm, Corner(0), Corner(1), Corner(2), Corner(3));
    else
        BuildRotatedAnimationVmQuad(vm, Corner(0), Corner(1), Corner(2), Corner(3));
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
