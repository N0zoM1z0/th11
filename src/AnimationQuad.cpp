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
