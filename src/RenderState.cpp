#include "RenderState.hpp"

static __forceinline IDirect3DDevice9 *RuntimeDevice()
{
    return static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice);
}

static __forceinline DWORD FloatState(float value)
{
    return *reinterpret_cast<DWORD *>(&value);
}

void __cdecl ResetDeviceRenderStates()
{
    RuntimeDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    RuntimeDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
    RuntimeDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    RuntimeDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    RuntimeDevice()->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    RuntimeDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    RuntimeDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    RuntimeDevice()->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
    RuntimeDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    RuntimeDevice()->SetRenderState(D3DRS_ALPHAREF, 1);
    RuntimeDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
    RuntimeDevice()->SetRenderState(D3DRS_FOGENABLE, TRUE);
    RuntimeDevice()->SetRenderState(D3DRS_FOGDENSITY, FloatState(1.0f));
    RuntimeDevice()->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
    RuntimeDevice()->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
    RuntimeDevice()->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_XRGB(160, 160, 160));
    RuntimeDevice()->SetRenderState(D3DRS_FOGSTART, FloatState(1000.0f));
    RuntimeDevice()->SetRenderState(D3DRS_FOGEND, FloatState(5000.0f));
    RuntimeDevice()->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);

    RuntimeDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    RuntimeDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    RuntimeDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
    RuntimeDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    RuntimeDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    RuntimeDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
    RuntimeDevice()->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
    RuntimeDevice()->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

    RuntimeDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
    RuntimeDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    RuntimeDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    RuntimeDevice()->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
    RuntimeDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    RuntimeDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

    RenderBatchView *renderer = g_RenderBatch;
    if (renderer)
    {
        RendererStateCacheView &cache = *reinterpret_cast<RendererStateCacheView *>(
            renderer->unknown0b8 + (0x4355bc - 0xb8));
        cache.flags4 = 7;
        cache.mode5 = 0xff;
        cache.mode6 = 0xff;
        cache.value0 = 0;
        cache.mode8 = 0xff;
    }
}
