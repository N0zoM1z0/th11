#include "AnimationSurfaceCopy.hpp"
#include "AnimationDocument.hpp"
#include "AnimationTexture.hpp"
#include "MainRuntime.hpp"

// TH11 00454EC0..00454FF8: copy a document texture region into the back
// buffer and mark the corresponding managed texture level dirty afterward.
// The source mirrors native early return and COM release order, including
// the absence of extra index, resource and rectangle-boundary checks.
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

    IDirect3DSurface9 *source;
    if (slots->files[command->documentIndex]->textures[command->textureIndex].texture->GetSurfaceLevel(0, &source) != D3D_OK)
    {
        backBuffer->Release();
        return;
    }

    RECT destination = {command->destinationX, command->destinationY,
        command->destinationX + command->destinationWidth,
        command->destinationY + command->destinationHeight};
    RECT sourceRect = {command->sourceX, command->sourceY,
        command->sourceX + command->sourceWidth,
        command->sourceY + command->sourceHeight};
    if (D3DXLoadSurfaceFromSurface(backBuffer, 0, &destination,
        source, 0, &sourceRect, 2, 0) == D3D_OK)
        slots->files[command->documentIndex]->textures[command->textureIndex].texture->AddDirtyRect(0);
    source->Release();
    backBuffer->Release();
}
