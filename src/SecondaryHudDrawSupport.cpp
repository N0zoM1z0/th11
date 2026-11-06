#include "SecondaryHudDraw.hpp"
#include "SecondaryActorRuntime.hpp"
#include "RenderBatch.hpp"
#include "MainRuntime.hpp"
#include "Camera.hpp"
#include <d3d9.h>

void __stdcall DecodeSecondaryHudBestTime(int *seconds, int *hundredths)
{
    int encoded = *reinterpret_cast<int *>(
        reinterpret_cast<unsigned char *>(g_MeshEffectSpell) + 0x90c);
    int second = (encoded / 100 % 100 + 34) % 100;
    int fraction = (encoded % 100 + 67) % 100;
    if (encoded / 100000 - 22 != second + fraction)
    {
        *seconds = 999;
        *hundredths = 99;
        return;
    }
    encoded = *reinterpret_cast<int *>(
        reinterpret_cast<unsigned char *>(g_MeshEffectSpell) + 0x90c);
    *seconds = (encoded / 100 % 100 + 34) % 100;
    *hundredths = (encoded % 100 + 67) % 100;
}

namespace
{
struct HudRectangleVertex
{
    CameraVector position;
    float rhw;
    unsigned int color;
};
typedef char HudRectangleVertexSize[(sizeof(HudRectangleVertex) == 20) ? 1 : -1];
__forceinline CameraVector RectanglePoint(float x, float y)
{
    CameraVector point = {x, y, 0.0f};
    return point;
}
__forceinline unsigned char &RendererByte(unsigned int offset)
{ return reinterpret_cast<unsigned char *>(g_RenderBatch)[offset]; }
__forceinline unsigned int &RendererWord(unsigned int offset)
{ return *reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(g_RenderBatch) + offset); }
}

void __stdcall DrawHudSolidRectangle(const float *rectangle, unsigned int color)
{
    FlushRenderBatch(g_RenderBatch);
    HudRectangleVertex vertices[4];
    vertices[0].position = RectanglePoint(rectangle[0], rectangle[1]);
    vertices[1].position = RectanglePoint(rectangle[2], rectangle[1]);
    vertices[2].position = RectanglePoint(rectangle[0], rectangle[3]);
    vertices[3].position = RectanglePoint(rectangle[2], rectangle[3]);
    for (int i = 3; i >= 0; --i)
    {
        vertices[i].rhw = 1.0f;
        vertices[i].color = color;
    }
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(HudRectangleVertex));
    RendererByte(0x4355c2) = 0xff;
    RendererWord(0x4355c8) = 0;
    RendererWord(0x4355bc) = 0;
    RendererByte(0x4355c1) = 0xff;
    RendererByte(0x4355c0) = 7;
    RendererByte(0x4355c3) = 0xff;
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
}
