#include "AnimationDocument.hpp"
#include "ResourceFile.hpp"
#include "ErrorLog.hpp"
#include "MainRuntime.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AnimationDocumentView *__stdcall BeginLoadAnimationDocument(RenderBatchView *renderer,
    const char *filename, int index)
{
    char path[260];
    TraceAnimationLoad(g_AnimationLoadTraceFormat, filename);
    if (index >= 32)
    {
        AppendErrorLog(&g_RuntimeLog, g_AnimationSlotError);
        return 0;
    }
    sprintf(path, g_AnimationPathFormat, filename);
    AnimationChunkHeaderView *data = reinterpret_cast<AnimationChunkHeaderView *>(
        LoadResourceFile(path, 0, 0));
    int chunkIndex = 0;
    AnimationDocumentView *file = new AnimationDocumentView;
    if (file)
        memset(file, 0, sizeof(*file));
    reinterpret_cast<AnimationDocumentSlotsView *>(renderer)->files[index] = file;
    if (!data)
        return 0;
    file->index = index;
    file->data = data;
    strcpy(file->filename, filename);
    int scriptCount = data->scriptCount;
    int spriteCount = data->spriteCount;
    int textureCount = 1;
    for (AnimationChunkHeaderView *chunk = data; chunk->nextOffset; )
    {
        chunk = reinterpret_cast<AnimationChunkHeaderView *>(
            reinterpret_cast<unsigned char *>(chunk) + chunk->nextOffset);
        scriptCount += chunk->scriptCount;
        spriteCount += chunk->spriteCount;
        ++textureCount;
    }
    file->textureCount = textureCount;
    file->textures = static_cast<AnimationDocumentTextureView *>(malloc(textureCount * 20));
    memset(file->textures, 0, textureCount * 20);
    file->sprites = static_cast<AnimationSpriteView *>(malloc(spriteCount * 72));
    file->scripts = static_cast<unsigned char **>(malloc(scriptCount * 4));
    file->spriteCount = spriteCount;
    file->scriptCount = scriptCount;
    AnimationChunkHeaderView *chunk = data;
    while (chunk)
    {
        if (LoadAnimationChunkSource(file, chunkIndex, chunk) < 0)
            return 0;
        ++chunkIndex;
        if (!chunk->nextOffset)
            return file;
        chunk = reinterpret_cast<AnimationChunkHeaderView *>(
            reinterpret_cast<unsigned char *>(chunk) + chunk->nextOffset);
    }
    AppendErrorLog(&g_RuntimeLog, g_AnimationChunkError);
    return 0;
}

AnimationDocumentView *__stdcall AcquireAnimationDocument(int index, const char *filename)
{
    AnimationDocumentSlotsView *slots = reinterpret_cast<AnimationDocumentSlotsView *>(g_RenderBatch);
    if (slots->files[index])
    {
        TraceAnimationLoad(g_AnimationCachedTraceFormat, filename);
        return slots->files[index];
    }
    AnimationDocumentView *file = BeginLoadAnimationDocument(g_RenderBatch, filename, index);
    if (!file)
        return 0;
    file->busy = 1;
    while (!(g_MainRuntime.flags & 0x180))
    {
        Sleep(1);
        if (!file->busy)
            break;
    }
    TraceAnimationLoad(g_AnimationReadyTraceFormat, filename);
    return file;
}
