#pragma once

#include <stddef.h>

struct RenderVertex
{
    float x, y, z, rhw;
    unsigned int color;
    float u, v;
};

// Partial renderer view; the preceding storage and complete owner are open.
struct RenderBatchView
{
    unsigned char unknown000[0xac];
    unsigned int drawCalls;
    float cameraOffsetX;
    float cameraOffsetY;
    unsigned char unknown0b8[0x435620 - 0xb8];
    unsigned int quadCount;
    RenderVertex vertices[0x20000];
    RenderVertex *nextVertex;
    RenderVertex *batchStart;
};

typedef char RenderVertexSize[(sizeof(RenderVertex) == 28) ? 1 : -1];
typedef char RenderBatchCountOffset[(offsetof(RenderBatchView, quadCount) == 0x435620) ? 1 : -1];
typedef char RenderBatchVerticesOffset[(offsetof(RenderBatchView, vertices) == 0x435624) ? 1 : -1];
typedef char RenderBatchNextOffset[(offsetof(RenderBatchView, nextVertex) == 0x7b5624) ? 1 : -1];
typedef char RenderBatchStartOffset[(offsetof(RenderBatchView, batchStart) == 0x7b5628) ? 1 : -1];

extern RenderBatchView *g_RenderBatch;
void __stdcall FlushRenderBatch(RenderBatchView *self);
