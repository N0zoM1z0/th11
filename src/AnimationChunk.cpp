#include "AnimationDocument.hpp"
#include "ResourceFile.hpp"
#include "ErrorLog.hpp"
#include <stdio.h>

int __stdcall LoadAnimationChunkSource(AnimationDocumentView *file, int index,
    AnimationChunkHeaderView *chunk)
{
    unsigned int size;
    char path[260];
    if (chunk->version != 7)
    {
        AppendErrorLog(&g_RuntimeLog, g_AnimationVersionError);
        return -1;
    }
    if (!chunk->hasData)
    {
        const char *name = reinterpret_cast<const char *>(chunk) + chunk->nameOffset;
        if (*name != '@')
        {
            sprintf(path, g_AnimationPathFormat, name);
            unsigned char *data = LoadResourceFile(path, &size, 1);
            if (!data)
            {
                AppendErrorLog(&g_RuntimeLog, g_AnimationTextureReadError, name);
                return -1;
            }
            file->textures[index].sourceSize = size;
            file->textures[index].source = data;
        }
    }
    return 1;
}

AnimationDocumentView *__stdcall AdvanceAnimationDocument(AnimationDocumentView *file)
{
    TraceAnimationLoad(g_AnimationPostloadTraceFormat, file->index, file->busy);
    AnimationChunkHeaderView *chunk = file->data;
    int spriteIndex = 0, scriptIndex = 0, textureIndex = 0, index = 0;
    bool decoded = false;
    for (;;)
    {
        if (index == file->busy - 1)
        {
            if (DecodeAnimationChunk(file, textureIndex, spriteIndex, scriptIndex, chunk) < 0)
            {
                file->busy = 0;
                return 0;
            }
            decoded = true;
        }
        scriptIndex += chunk->scriptCount;
        spriteIndex += chunk->spriteCount;
        ++textureIndex;
        if (!chunk->nextOffset)
        {
            file->busy = 0;
            return file;
        }
        ++index;
        chunk = reinterpret_cast<AnimationChunkHeaderView *>(
            reinterpret_cast<unsigned char *>(chunk) + chunk->nextOffset);
        if (index == file->busy || decoded)
        {
            ++file->busy;
            return file;
        }
    }
}

int __stdcall DecodeAnimationChunk(AnimationDocumentView *file, int textureIndex,
    int spriteIndex, int scriptIndex, AnimationChunkHeaderView *chunk)
{
    if (!chunk)
    {
        AppendErrorLog(&g_RuntimeLog, g_AnimationChunkError);
        return -1;
    }
    if (chunk->version != 7)
    {
        AppendErrorLog(&g_RuntimeLog, g_AnimationVersionError);
        return -1;
    }
    unsigned char *base = reinterpret_cast<unsigned char *>(chunk);
    if (!chunk->hasData)
    {
        const char *name = reinterpret_cast<const char *>(base + chunk->nameOffset);
        if (*name == '@')
        {
            if (name[1] == 'R')
                CreateAnimationRenderTarget(file->textures + textureIndex);
            else
                file->textureBytes += CreateAnimationEmptyTexture(file->textures + textureIndex,
                    chunk->width, chunk->height, chunk->format);
        }
        else
        {
            int bytes = CreateAnimationFileTexture(file->textures + textureIndex, chunk->format,
                chunk->width, chunk->height, chunk->offsetX, chunk->offsetY);
            if (bytes < 0)
            {
                AppendErrorLog(&g_RuntimeLog, g_AnimationTextureCreateError, name);
                return -1;
            }
            file->textureBytes += bytes;
        }
    }
    else
    {
        int bytes = CreateAnimationEmbeddedTexture(file->textures + textureIndex, base + chunk->textureOffset,
            chunk->format, chunk->width, chunk->height);
        if (bytes < 0)
        {
            AppendErrorLog(&g_RuntimeLog, g_AnimationEmbeddedTextureError);
            return -1;
        }
        file->textureBytes += bytes;
    }
    D3DSURFACE_DESC description;
    file->textures[textureIndex].texture->GetLevelDesc(0, &description);
    unsigned int *offset = reinterpret_cast<unsigned int *>(base + 0x40);
    for (int i = 0; i < chunk->spriteCount; ++i, ++offset, ++spriteIndex)
    {
        const AnimationChunkSpriteView *rectangle =
            reinterpret_cast<const AnimationChunkSpriteView *>(base + *offset);
        AnimationSpriteView sprite;
        sprite.texture = file->textures + textureIndex;
        sprite.fileIndex = file->index;
        sprite.textureIndex = textureIndex;
        sprite.scaleX = static_cast<float>(description.Width) / chunk->width;
        sprite.scaleY = static_cast<float>(description.Height) / chunk->height;
        sprite.left = rectangle->x * sprite.scaleX;
        sprite.top = rectangle->y * sprite.scaleY;
        sprite.right = (rectangle->width + rectangle->x) * sprite.scaleX;
        sprite.bottom = (rectangle->height + rectangle->y) * sprite.scaleY;
        sprite.textureWidth = static_cast<float>(description.Width);
        sprite.textureHeight = static_cast<float>(description.Height);
        StoreAnimationSprite(file, spriteIndex, &sprite);
    }
    const AnimationChunkScriptView *script =
        reinterpret_cast<const AnimationChunkScriptView *>(offset);
    for (int i = 0; i < chunk->scriptCount; ++i, ++script)
        file->scripts[scriptIndex + i] = base + script->offset;
    return 1;
}
