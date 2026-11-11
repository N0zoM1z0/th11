#include "AnimationSurfaceCopy.hpp"
#include "AnimationDocument.hpp"
#include "AnimationTexture.hpp"
#include "MainRuntime.hpp"

// TH11 00454EC0..00454FF8: capture a backbuffer region into the selected
// animation texture, and mark that texture level dirty afterward.
// The source mirrors native early return and COM release order, including
// the absence of extra index, resource and rectangle-boundary checks.
// The copy direction is backbuffer -> texture level 0 (then AddDirtyRect).
void __stdcall CopyAnimationTextureSurface(RenderBatchView *renderer,
    const AnimationSurfaceCopyView *command)
{
    AnimationDocumentSlotsView *slots =
        reinterpret_cast<AnimationDocumentSlotsView *>(renderer);
    AnimationDocumentView *file = slots->files[command->documentIndex];
    if (!file->textures[command->textureIndex].texture)
        return;

    FlushRenderBatch(renderer);
    IDirect3DSurface9 *backBuffer;
    if (static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->GetBackBuffer(
        0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer) != D3D_OK)
        return;

    IDirect3DSurface9 *textureSurface;
    if (slots->files[command->documentIndex]->textures[command->textureIndex].texture->GetSurfaceLevel(0, &textureSurface) != D3D_OK)
    {
        backBuffer->Release();
        return;
    }

    struct { RECT sourceRect, destination; } rects;
    rects.sourceRect.left = command->sourceX;
    rects.sourceRect.top = command->sourceY;
    rects.sourceRect.right = command->sourceX + command->sourceWidth;
    rects.sourceRect.bottom = command->sourceY + command->sourceHeight;
    rects.destination.left = command->destinationX;
    rects.destination.top = command->destinationY;
    rects.destination.right = command->destinationX + command->destinationWidth;
    rects.destination.bottom = command->destinationY + command->destinationHeight;
    if (D3DXLoadSurfaceFromSurface(textureSurface, 0, &rects.destination,
        backBuffer, 0, &rects.sourceRect, 2, 0) == D3D_OK)
        slots->files[command->documentIndex]->textures[command->textureIndex].texture->AddDirtyRect(0);
    textureSurface->Release();
    backBuffer->Release();
}
