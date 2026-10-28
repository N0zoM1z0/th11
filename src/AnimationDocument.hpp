#pragma once

#include "RenderBatch.hpp"
#include <d3d9.h>
#include <stddef.h>

struct AnimationChunkHeaderView
{
    unsigned int version;
    unsigned short spriteCount, scriptCount;
    unsigned short unknown008, width, height, format;
    unsigned int nameOffset;
    short offsetX, offsetY;
    unsigned int unknown018, textureOffset;
    unsigned char hasData;
    unsigned char unknown021[3];
    unsigned int nextOffset;
    unsigned char unknown028[0x40 - 0x28];
};

struct AnimationDocumentTextureView
{
    IDirect3DTexture9 *texture;
    unsigned char *source;
    unsigned int sourceSize, bytesPerPixel, flags;
};

struct AnimationChunkSpriteView
{
    unsigned int id;
    float x, y, width, height;
};

struct AnimationChunkScriptView
{
    unsigned int id, offset;
};

struct AnimationSpriteView
{
    int fileIndex, textureIndex;
    AnimationDocumentTextureView *texture;
    float left, top, right, bottom;
    float textureHeight, textureWidth;
    float u0, v0, u1, v1;
    float height, width;
    float scaleX, scaleY;
    unsigned int unknown044;
};

struct AnimationDocumentView
{
    int index;
    char filename[260];
    AnimationChunkHeaderView *data;
    int textureCount, scriptCount, spriteCount;
    AnimationSpriteView *sprites;
    unsigned char **scripts;
    AnimationDocumentTextureView *textures;
    volatile int busy;
    int pendingDestroy;
    unsigned int textureBytes, unknown130;
};

struct AnimationDocumentSlotsView
{
    unsigned char unknown000[0x4350c0];
    AnimationDocumentView *files[32];
};

typedef char AnimationChunkNextOffset[(offsetof(AnimationChunkHeaderView, nextOffset) == 0x24) ? 1 : -1];
typedef char AnimationChunkSize[(sizeof(AnimationChunkHeaderView) == 0x40) ? 1 : -1];
typedef char AnimationDocumentTextureSize[(sizeof(AnimationDocumentTextureView) == 20) ? 1 : -1];
typedef char AnimationSpriteSize[(sizeof(AnimationSpriteView) == 72) ? 1 : -1];
typedef char AnimationSpriteScaleOffset[(offsetof(AnimationSpriteView, scaleX) == 0x3c) ? 1 : -1];
typedef char AnimationDocumentSize[(sizeof(AnimationDocumentView) == 0x134) ? 1 : -1];
typedef char AnimationDocumentDataOffset[(offsetof(AnimationDocumentView, data) == 0x108) ? 1 : -1];
typedef char AnimationDocumentBusyOffset[(offsetof(AnimationDocumentView, busy) == 0x124) ? 1 : -1];

extern const char g_AnimationLoadTraceFormat[], g_AnimationPathFormat[];
extern const char g_AnimationSlotError[], g_AnimationChunkError[];
extern const char g_AnimationCachedTraceFormat[], g_AnimationReadyTraceFormat[];
extern const char g_AnimationVersionError[], g_AnimationTextureReadError[];
extern const char g_AnimationTextureCreateError[], g_AnimationEmbeddedTextureError[];
extern const char g_AnimationPostloadTraceFormat[];
void __cdecl TraceAnimationLoad(const char *format, ...);
int __stdcall LoadAnimationChunkSource(AnimationDocumentView *file, int index,
    AnimationChunkHeaderView *chunk);
AnimationDocumentView *__stdcall AdvanceAnimationDocument(AnimationDocumentView *file);
int __stdcall DecodeAnimationChunk(AnimationDocumentView *file, int textureIndex,
    int spriteIndex, int scriptIndex, AnimationChunkHeaderView *chunk);
void __stdcall StoreAnimationSprite(AnimationDocumentView *file, int index,
    const AnimationSpriteView *sprite);

// Ordinary source conventions; original LTCG register assignments are separate.
int __stdcall CreateAnimationFileTexture(AnimationDocumentTextureView *texture,
    unsigned int format, int width, int height, int offsetX, int offsetY);
int __stdcall CreateAnimationEmbeddedTexture(AnimationDocumentTextureView *texture,
    const unsigned char *data, unsigned int format, int width, int height);
int __stdcall CreateAnimationEmptyTexture(AnimationDocumentTextureView *texture,
    int width, int height, unsigned int format);
int __stdcall CreateAnimationRenderTarget(AnimationDocumentTextureView *texture);
AnimationDocumentView *__stdcall BeginLoadAnimationDocument(RenderBatchView *renderer,
    const char *filename, int index);
AnimationDocumentView *__stdcall AcquireAnimationDocument(int index, const char *filename);
