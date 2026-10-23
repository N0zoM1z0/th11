#pragma once

#include "MainRuntime.hpp"
#include "RenderBatch.hpp"
#include <d3d9.h>

struct RendererStateCacheView
{
    unsigned int value0;
    unsigned char flags4;
    unsigned char mode5;
    unsigned char mode6;
    unsigned char unknown7;
    unsigned char mode8;
};

typedef char RendererCacheFlagsOffset[(offsetof(RendererStateCacheView, flags4) == 4) ? 1 : -1];
typedef char RendererCacheModeOffset[(offsetof(RendererStateCacheView, mode8) == 8) ? 1 : -1];

void __cdecl ResetDeviceRenderStates();
