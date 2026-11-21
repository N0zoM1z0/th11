#pragma once

#include "StageResource.hpp"
#include "AnimationVm.hpp"
#include "AnimationVmId.hpp"
#include "Random.hpp"
#include <stddef.h>

// The draw grid shares the binding block's six observed words.
struct StageMeshView
{
    int rows, columns;
    AnimationVmId *ids;
    AnimationVmView **rowVms;
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

typedef char StageObjectViewSize[(sizeof(StageObjectView) == 0x1c) ? 1 : -1];
typedef char StageObjectFlagsOffset[(offsetof(StageObjectView, flags) == 3) ? 1 : -1];
typedef char StagePrimitiveViewSize[(sizeof(StagePrimitiveView) == 8) ? 1 : -1];
typedef char StagePrimitiveVmIndexOffset[(offsetof(StagePrimitiveView, vmIndex) == 6) ? 1 : -1];

int __fastcall UpdateStageObjects(StageResourceView *self);
int __stdcall ExecuteStageScript(StageResourceView *self);
void __stdcall ResetStageMesh(StageMeshView *self, float x, float y, float width, float height);
extern RandomStateView g_TextRandomState;
