#include "AnimationTexture.hpp"
#include "MainRuntime.hpp"

static __forceinline unsigned int SelectAnimationTextureFormat(unsigned int format)
{
    if (*reinterpret_cast<const unsigned int *>(g_MainRuntime.unknown00c + 0x1f4) & 1)
    {
        D3DFORMAT mapped = g_AnimationTextureFormats[format];
        if (mapped == D3DFMT_A8R8G8B8 || mapped == D3DFMT_UNKNOWN)
            format = 5;
        else if (mapped == D3DFMT_R8G8B8)
            format = 3;
    }
    return format;
}

int __stdcall CreateAnimationFileTexture(AnimationDocumentTextureView *texture,
    unsigned int format, int width, int height, int offsetX, int offsetY)
{
    format = SelectAnimationTextureFormat(format);
    texture->flags &= ~1u;
    IDirect3DTexture9 *created;
    if (D3DXCreateTextureFromFileInMemoryEx(
        static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice),
        texture->source, texture->sourceSize, 0, 0, 0, 0,
        g_AnimationTextureFormats[format], D3DPOOL_MANAGED, 1, ~0u, 0, 0, 0,
        &created) != D3D_OK)
        return -1;

    IDirect3DSurface9 *source;
    created->GetSurfaceLevel(0, &source);
    D3DSURFACE_DESC description;
    source->GetDesc(&description);
    if (description.Width == width && description.Height == height)
    {
        texture->texture = created;
        if (source)
        {
            source->Release();
            source = 0;
        }
    }
    else
    {
        static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->CreateTexture(
            width, height, 0, 0, g_AnimationTextureFormats[format], D3DPOOL_MANAGED,
            &texture->texture, 0);
        IDirect3DSurface9 *destination;
        texture->texture->GetSurfaceLevel(0, &destination);
        int copyWidth = width, copyHeight = height;
        if (offsetX + copyWidth > static_cast<int>(description.Width))
            copyWidth = static_cast<int>(description.Width) - offsetX;
        if (offsetY + copyHeight > static_cast<int>(description.Height))
            copyHeight = static_cast<int>(description.Height) - offsetY;
        RECT sourceRect = { offsetX, offsetY, offsetX + copyWidth, offsetY + copyHeight };
        D3DXLoadSurfaceFromSurface(destination, 0, 0, source, 0, &sourceRect, 1, 0);
        if (source)
        {
            source->Release();
            source = 0;
        }
        if (destination)
        {
            destination->Release();
            destination = 0;
        }
        if (created)
        {
            created->Release();
            created = 0;
        }
    }
    FixAnimationTextureTransparentPixels(texture);
    texture->bytesPerPixel = g_AnimationTextureBytesPerPixel[format];
    return width * height * g_AnimationTextureBytesPerPixel[format];
}

int __stdcall CreateAnimationEmbeddedTexture(AnimationDocumentTextureView *texture,
    const unsigned char *data, unsigned int format, int width, int height)
{
    IDirect3DSurface9 *surface = 0;
    format = SelectAnimationTextureFormat(format);
    texture->flags &= ~1u;
    const AnimationEmbeddedTextureView *header =
        reinterpret_cast<const AnimationEmbeddedTextureView *>(data);
    RECT rectangle = { 0, 0, header->width, header->height };
    if (D3DXCreateTexture(static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice),
        width, height, 1, 0, g_AnimationTextureFormats[format], D3DPOOL_MANAGED,
        &texture->texture) != D3D_OK)
    {
        if (surface)
            surface->Release();
        return -1;
    }
    texture->texture->GetSurfaceLevel(0, &surface);
    D3DXLoadSurfaceFromMemory(surface, 0, &rectangle, data + sizeof(*header),
        g_AnimationTextureFormats[header->format],
        header->width * g_AnimationTextureBytesPerPixel[header->format],
        0, &rectangle, 1, 0);
    texture->bytesPerPixel = g_AnimationTextureBytesPerPixel[format];
    if (surface)
        surface->Release();
    return g_AnimationTextureBytesPerPixel[format] * width * height;
}

int __stdcall CreateAnimationEmptyTexture(AnimationDocumentTextureView *texture,
    int width, int height, unsigned int format)
{
    texture->flags &= ~1u;
    D3DXCreateTexture(static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice),
        width, height, 1, 0, g_AnimationTextureFormats[format], D3DPOOL_MANAGED,
        &texture->texture);
    texture->bytesPerPixel = g_AnimationTextureBytesPerPixel[format];
    return width * height * g_AnimationTextureBytesPerPixel[format];
}

int __stdcall CreateAnimationRenderTarget(AnimationDocumentTextureView *texture)
{
    D3DPRESENT_PARAMETERS &presentation =
        *reinterpret_cast<D3DPRESENT_PARAMETERS *>(g_MainRuntime.unknown00c + 0xe8);
    texture->flags |= 1;
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->CreateTexture(
        presentation.BackBufferWidth, presentation.BackBufferHeight, 1,
        D3DUSAGE_RENDERTARGET, presentation.BackBufferFormat, D3DPOOL_DEFAULT,
        &texture->texture, 0);
    texture->bytesPerPixel = presentation.BackBufferFormat == D3DFMT_X8R8G8B8 ? 4 : 2;
    return 0;
}
