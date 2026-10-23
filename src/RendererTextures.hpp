#pragma once

#include "MainRuntime.hpp"
#include "RenderBatch.hpp"
#include <d3d9.h>

struct RendererTextureEntryView
{
    IDirect3DTexture9 *texture;
    unsigned int unknown04;
    unsigned int unknown08;
    unsigned int bytesPerPixel;
    unsigned int flags;

    __forceinline void RecreateForReset();
};

struct RendererTextureFileView
{
    unsigned char unknown000[0x10c];
    int textureCount;
    unsigned char unknown110[0x120 - 0x110];
    RendererTextureEntryView *textures;
};

typedef char RendererTextureEntrySize[(sizeof(RendererTextureEntryView) == 20) ? 1 : -1];
typedef char RendererTextureFlagsOffset[(offsetof(RendererTextureEntryView, flags) == 0x10) ? 1 : -1];
typedef char RendererTextureCountOffset[(offsetof(RendererTextureFileView, textureCount) == 0x10c) ? 1 : -1];
typedef char RendererTextureTableOffset[(offsetof(RendererTextureFileView, textures) == 0x120) ? 1 : -1];

void __cdecl PrepareRendererForReset();
void __cdecl RestoreRendererAfterReset();
