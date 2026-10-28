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
    AnimationDocumentTextureView *texture = file->textures + textureIndex;
    if (!chunk->hasData)
    {
        const char *name = reinterpret_cast<const char *>(base + chunk->nameOffset);
        if (*name == '@')
        {
            if (name[1] == 'R')
                CreateAnimationRenderTarget(texture);
            else
                file->textureBytes += CreateAnimationEmptyTexture(texture,
                    chunk->format, chunk->width, chunk->height);
        }
        else
        {
            int bytes = CreateAnimationFileTexture(texture, chunk->format,
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
        int bytes = CreateAnimationEmbeddedTexture(texture, base + chunk->textureOffset,
            chunk->format, chunk->width, chunk->height);
        if (bytes < 0)
        {
            AppendErrorLog(&g_RuntimeLog, g_AnimationEmbeddedTextureError);
            return -1;
        }
        file->textureBytes += bytes;
    }
    D3DSURFACE_DESC description;
    texture->texture->GetLevelDesc(0, &description);
    unsigned int *offset = reinterpret_cast<unsigned int *>(base + 0x40);
    for (int i = 0; i < chunk->spriteCount; ++i, ++offset, ++spriteIndex)
    {
        const float *rectangle = reinterpret_cast<const float *>(base + *offset + 4);
        AnimationSpriteView sprite;
        sprite.fileIndex = file->index;
        sprite.textureIndex = textureIndex;
        sprite.texture = file->textures + textureIndex;
        sprite.scaleX = static_cast<float>(description.Width) / chunk->width;
        sprite.scaleY = static_cast<float>(description.Height) / chunk->height;
        sprite.left = rectangle[0] * sprite.scaleX;
        sprite.top = rectangle[1] * sprite.scaleY;
        sprite.right = (rectangle[2] + rectangle[0]) * sprite.scaleX;
        sprite.bottom = (rectangle[3] + rectangle[1]) * sprite.scaleY;
        sprite.textureWidth = static_cast<float>(description.Width);
        sprite.textureHeight = static_cast<float>(description.Height);
        StoreAnimationSprite(file, spriteIndex, &sprite);
    }
    for (int i = 0; i < chunk->scriptCount; ++i, offset += 2)
        file->scripts[scriptIndex + i] = base + offset[1];
    return 1;
}
