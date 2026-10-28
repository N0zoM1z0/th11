#pragma once

#include "StageResource.hpp"
#include "AnimationVm.hpp"
#include "Random.hpp"

// The draw grid shares the binding block's six observed words.
struct StageMeshView
{
    int rows, columns;
    unsigned int *ids;
    void *indices;
    RenderVertex *vertices;
    CameraVector *positions;
};

typedef char StageMeshSize[(sizeof(StageMeshView) == sizeof(VmBindingBlockView)) ? 1 : -1];
typedef char StageMeshVerticesOffset[(offsetof(StageMeshView, vertices) == 0x10) ? 1 : -1];

struct StageObjectView
{
    unsigned char unknown000[3];
    unsigned char flags;
    unsigned char unknown004[0x1c - 4];
};

struct StagePrimitiveView
{
    short kind, byteSize, unknown004, vmIndex;
};

int __stdcall UpdateStageObjects(StageResourceView *self);
// Reviewed external dependencies; their bodies remain open.
int __stdcall ExecuteStageScript(StageResourceView *self);
void __stdcall ResetStageMesh(StageMeshView *self, float x, float y, float width, float height);
extern RandomStateView g_TextRandomState;
