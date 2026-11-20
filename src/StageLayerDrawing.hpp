#pragma once

#include "StageResource.hpp"

// Native section+0x18 stores an unbounded sequence of sixteen-byte entries;
// a negative object index terminates it.
struct StageLayerRecord
{
    short objectIndex;
    unsigned short flags;
    CameraVector offset;
};

struct StageObjectDrawView
{
    unsigned char unknown000[2];
    signed char layer;
    unsigned char flags;
    CameraVector position;
    CameraVector extents;
    // Variable-length signed-stride primitive commands begin at +0x1C.
    unsigned char commands[1];
};

struct StagePrimitiveDrawView
{
    short type;
    short byteSize;
    short scriptId;
    short vmIndex;
    float offsetX, offsetY, offsetZ;
    float scaleX, scaleY;
};

typedef char StageLayerRecordSize[(sizeof(StageLayerRecord) == 0x10) ? 1 : -1];
typedef char StageObjectCommandsOffset[(offsetof(StageObjectDrawView, commands) == 0x1c) ? 1 : -1];
typedef char StagePrimitiveDrawSize[(sizeof(StagePrimitiveDrawView) == 0x1c) ? 1 : -1];

// The fourth source parameter models the private perspective-camera input
// that the native caller places in EAX; the focused /GL pair keeps it there.
int __fastcall CullStageObject(const StageObjectDrawView *object,
    const CameraVector *offset, float maxDistanceSquared,
    const PerspectiveCameraView *camera);
int __stdcall DrawStageLayer(StageResourceView *self, int layer);
