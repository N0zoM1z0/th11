#pragma once

#include "StageScript.hpp"

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
AnimationVmView *__stdcall FindAnimationVmById(RenderBatchView *renderer, unsigned int id);
unsigned int *__stdcall CreateStageMeshRow(unsigned int *id, int columns);
unsigned int *__stdcall CreateAlternateStageMeshRow(unsigned int *id, int columns);

// Reviewed external scene-VM factory; its body is not implemented here.
unsigned int *__stdcall CreateAnimationVmId(unsigned int *id, unsigned int file, int script, int layer);
