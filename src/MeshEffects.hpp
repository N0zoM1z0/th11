#pragma once

#include "StageMesh.hpp"

// Partial physical views; the original gameplay class identities remain open.
struct MeshEffectControllerView
{
    unsigned char unknown000[0x40];
    AnimationVmId vmId;
    unsigned char unknown044[0x480 - 0x44];
    StageScriptMesh *mesh;
    CameraVector origin;
    float parameter490, parameter494;
};

struct MeshEffectVmView
{
    unsigned char unknown000[0x3b4];
    int pointCount, phase;
    unsigned char unknown3bc[0x400 - 0x3bc];
    RenderVertex *vertices;
    unsigned char unknown404[0x428 - 0x404];
    int (__fastcall *callback)(AnimationVmView *vm);
};

typedef char MeshEffectMeshOffset[(offsetof(MeshEffectControllerView, mesh) == 0x480) ? 1 : -1];
typedef char MeshEffectOriginOffset[(offsetof(MeshEffectControllerView, origin) == 0x484) ? 1 : -1];
typedef char MeshEffectCallbackOffset[(offsetof(MeshEffectVmView, callback) == 0x428) ? 1 : -1];

int __stdcall InitializeColumnMeshEffect(MeshEffectControllerView *self);
int __stdcall InitializeSquareMeshEffect(MeshEffectControllerView *self);
int __fastcall SquareMeshVmCallback(AnimationVmView *vm);
void __cdecl UpdateMeshEffectSpellState();
void __stdcall QueueMeshEffectSound(int sound);
AnimationVmId __stdcall CreatePositionedAnimationVmId(AnimationFileView *file,
    const CameraVector *position, int script, int layer);
