#pragma once

#include "StageScript.hpp"
#include "AnimationVmFactory.hpp"

struct StageMeshVmView
{
    unsigned char unknown000[0x3b4];
    int pointCount;
    unsigned char unknown3b8[0x400 - 0x3b8];
    RenderVertex *vertices;
    unsigned int flags;
    unsigned char unknown408[0x414 - 0x408];
    int (__fastcall *callback)(AnimationVmView *vm);
    StageMeshView *mesh;
};

typedef char StageMeshVmVerticesOffset[(offsetof(StageMeshVmView, vertices) == 0x400) ? 1 : -1];
typedef char StageMeshVmCallbackOffset[(offsetof(StageMeshVmView, callback) == 0x414) ? 1 : -1];
typedef char StageMeshVmOwnerOffset[(offsetof(StageMeshVmView, mesh) == 0x418) ? 1 : -1];

void __stdcall PublishStageMesh(StageMeshView *self);
int __fastcall StageMeshVmCallback(AnimationVmView *vm);
AnimationVmView *__stdcall FindAnimationVmById(RenderBatchView *renderer, AnimationVmId id);
AnimationVmId __stdcall CreateStageMeshRow(int columns);
AnimationVmId __stdcall CreateAlternateStageMeshRow(int columns);
