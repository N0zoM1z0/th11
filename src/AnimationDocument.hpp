#pragma once

#include "RenderBatch.hpp"
#include <stddef.h>

struct AnimationChunkHeaderView
{
    unsigned char unknown000[4];
    unsigned short spriteCount, scriptCount;
    unsigned char unknown008[0x24 - 8];
    unsigned int nextOffset;
};

struct AnimationDocumentView
{
    int index;
    char filename[260];
    AnimationChunkHeaderView *data;
    int textureCount, scriptCount, spriteCount;
    void *sprites;
    unsigned char **scripts;
    void *textures;
    volatile int busy;
    unsigned char unknown128[0x0c];
};

struct AnimationDocumentSlotsView
{
    unsigned char unknown000[0x4350c0];
    AnimationDocumentView *files[32];
};

typedef char AnimationChunkNextOffset[(offsetof(AnimationChunkHeaderView, nextOffset) == 0x24) ? 1 : -1];
typedef char AnimationDocumentSize[(sizeof(AnimationDocumentView) == 0x134) ? 1 : -1];
typedef char AnimationDocumentDataOffset[(offsetof(AnimationDocumentView, data) == 0x108) ? 1 : -1];
typedef char AnimationDocumentBusyOffset[(offsetof(AnimationDocumentView, busy) == 0x124) ? 1 : -1];

extern const char g_AnimationLoadTraceFormat[], g_AnimationPathFormat[];
extern const char g_AnimationSlotError[], g_AnimationChunkError[];
extern const char g_AnimationCachedTraceFormat[], g_AnimationReadyTraceFormat[];
void __cdecl TraceAnimationLoad(const char *format, ...);
int __stdcall DecodeAnimationChunk(AnimationDocumentView *file, int index,
    AnimationChunkHeaderView *chunk);
AnimationDocumentView *__stdcall BeginLoadAnimationDocument(RenderBatchView *renderer,
    const char *filename, int index);
AnimationDocumentView *__stdcall AcquireAnimationDocument(int index, const char *filename);
