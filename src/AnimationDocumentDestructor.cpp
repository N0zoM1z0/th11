#include "StageResource.hpp"
#include "MenuController.hpp"
#include <stdlib.h>
#include <stddef.h>

typedef char DocumentDestroyCountOffset[
    (offsetof(AnimationDocumentView, textureCount) == 0x10c) ? 1 : -1];
typedef char DocumentDestroySpriteOffset[
    (offsetof(AnimationDocumentView, sprites) == 0x118) ? 1 : -1];
typedef char DocumentDestroyScriptOffset[
    (offsetof(AnimationDocumentView, scripts) == 0x11c) ? 1 : -1];
typedef char DocumentDestroyTextureOffset[
    (offsetof(AnimationDocumentView, textures) == 0x120) ? 1 : -1];
typedef char DocumentDestroyTailOffset[
    (offsetof(AnimationDocumentView, unknown130) == 0x130) ? 1 : -1];

// Complete original TH11 004549E0..00454AD2 / 243-byte document-owner
// cleanup. The original register convention is link-time private; keep a
// natural callable C++ function and the existing physical file layout.
void __stdcall DestroyAnimationDocument(AnimationDocumentView *file)
{
    if (!file->data)
        return;

    MarkMenuFileAnimations(g_RenderBatch,
        reinterpret_cast<AnimationFileView *>(file));

    for (int i = 0; i < file->textureCount; ++i)
    {
        AnimationDocumentTextureView *texture = &file->textures[i];
        if (texture->texture)
        {
            texture->texture->Release();
            texture->texture = 0;
        }
        if (texture->source)
        {
            free(texture->source);
            texture->source = 0;
        }
    }

    if (file->textures)
    {
        free(file->textures);
        file->textures = 0;
    }
    if (file->sprites)
    {
        free(file->sprites);
        file->sprites = 0;
    }
    if (file->scripts)
    {
        free(file->scripts);
        file->scripts = 0;
    }

    // The current shared view has a DWORD here, but native cleanup
    // dereferences and frees it as an owned pointer at physical +0x130.
    void **extraStorage = reinterpret_cast<void **>(
        reinterpret_cast<unsigned char *>(file) + 0x130);
    if (*extraStorage)
    {
        free(*extraStorage);
        *extraStorage = 0;
    }
    if (file->data)
    {
        free(file->data);
        file->data = 0;
    }
}
