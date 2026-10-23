#include "RendererTextures.hpp"

static __forceinline RendererTextureFileView **RendererFiles()
{
    return reinterpret_cast<RendererTextureFileView **>(
        g_RenderBatch->unknown0b8 + (0x4350c0 - 0xb8));
}

static __forceinline D3DPRESENT_PARAMETERS &TexturePresentation()
{
    return *reinterpret_cast<D3DPRESENT_PARAMETERS *>(g_MainRuntime.unknown00c + 0xe8);
}

__forceinline void RendererTextureEntryView::RecreateForReset()
{
    flags |= 1;
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->CreateTexture(
        TexturePresentation().BackBufferWidth, TexturePresentation().BackBufferHeight,
        1, D3DUSAGE_RENDERTARGET, TexturePresentation().BackBufferFormat,
        D3DPOOL_DEFAULT, &texture, 0);
    bytesPerPixel = TexturePresentation().BackBufferFormat == D3DFMT_X8R8G8B8 ? 4 : 2;
}

void __cdecl PrepareRendererForReset()
{
    RendererTextureFileView **slot = RendererFiles();
    int remaining = 32;
    do
    {
        if (*slot)
        {
            for (int i = 0; i < (*slot)->textureCount; ++i)
            {
                if (((*slot)->textures[i].flags & 1) && (*slot)->textures[i].texture)
                {
                    ((*slot)->textures + i)->texture->Release();
                    (*slot)->textures[i].texture = 0;
                }
            }
        }
        ++slot;
    } while (--remaining);
}

void __cdecl RestoreRendererAfterReset()
{
    RendererTextureFileView **slot = RendererFiles();
    int remaining = 32;
    do
    {
        if (*slot)
        {
            for (int i = 0; i < (*slot)->textureCount; ++i)
            {
                if ((*slot)->textures[i].flags & 1)
                    ((*slot)->textures + i)->RecreateForReset();
            }
        }
        ++slot;
    } while (--remaining);
}
