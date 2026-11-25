#pragma once

#include <d3d9.h>
#include <stddef.h>
#include "Camera.hpp"
#include "RenderBatch.hpp"
#include "StageResource.hpp"
#include "StageTweenTypes.hpp"
#include "AnimationTweenTypes.hpp"

struct AnimationVmView;
struct AnimationInstruction;
struct AnimationDocumentView;
struct AnimationFileView;
struct AnimationVmId;
struct AnimationVmNode
{
    AnimationVmView *vm;
    AnimationVmNode *next;
    AnimationVmNode *previous;

    __forceinline void Reset(AnimationVmView *owner)
    {
        next = 0;
        previous = 0;
        vm = owner;
    }
};

struct AnimationPersistentState
{
    unsigned int words[3];
};

union AnimationVmFlags
{
    unsigned int value;
    struct { unsigned short low, high; } halves;
};

struct AnimationVmView
{
    unsigned int unknown000;
    AnimationVmNode updateNode;
    AnimationVmNode drawNode;
    unsigned int unknown01c;
    unsigned int persistent020;
    CameraVector rotation, angularVelocity;
    AnimationFloatPair scale;
    AnimationFloatPair scaleVelocity;
    float width, height, uOffset, vOffset;
    StageTimerView time;
    AnimationFloatPair uv[4];
    StageVectorTween positionTween;
    AnimationIntVectorTween colorTween;
    AnimationIntScalarTween alphaTween;
    StageVectorTween rotationTween;
    AnimationFloatPairTween scaleTween;
    AnimationIntVectorTween color2Tween;
    AnimationIntScalarTween alpha2Tween;
    AnimationFloatScalarTween uVelocityTween, vVelocityTween;
    float uVelocity, vVelocity;
    D3DMATRIX transform;
    unsigned char unknown2f4[0x374 - 0x2f4];
    union
    {
        struct { unsigned int color, color2; };
        int spriteIndex;
    };
    unsigned short event;
    unsigned short unknown37e;
    StageTimerView savedTime;
    AnimationInstruction *savedInstruction;
    int spriteFrame;
    short marker;
    unsigned short fileId, unknown3a0, scriptId;
    unsigned char *scriptStart;
    AnimationInstruction *instruction;
    unsigned int unknown3ac;
    AnimationFileView *file;
    union
    {
        struct
        {
            int integerVariables[4];
            float floatVariables[4];
            int integer3d4, integer3d8;
            CameraVector position, translation, otherPosition;
            RenderVertex *vertices;
        };
        struct
        {
            unsigned char unknown3b4[8];
            int duration3bc;
            unsigned char unknown3c0[0x28];
            AnimationPersistentState persistent3e8;
            unsigned char unknown3f4[0x10];
        };
    };
    AnimationVmFlags flags;
    unsigned int flags2;
    unsigned char unknown40c[0x428 - 0x40c];
    int (__fastcall *update)(AnimationVmView *);
    int (__fastcall *spriteTransform)(AnimationVmView *, int);
    unsigned int unknown430;

    int __fastcall BindAnimationSprite(AnimationDocumentView *file, int sprite);
};

struct AnimationTextureEntry
{
    IDirect3DTexture9 *texture;
    unsigned char unknown04[16];
};

struct AnimationFileView
{
    unsigned short id;
    unsigned char unknown002[0x11c - 2];
    unsigned char **scripts;
    AnimationTextureEntry *textures;
    int bindingDisabled;

    // 0x00455B10; native callers pass this before the hidden result.
    AnimationVmId CreatePositionedVmId(const CameraVector *position, int script, int layer);
};

typedef char AnimationVmSize[(sizeof(AnimationVmView) == 0x434) ? 1 : -1];
typedef char AnimationVmRotationOffset[(offsetof(AnimationVmView, rotation) == 0x24) ? 1 : -1];
typedef char AnimationVmScaleOffset[(offsetof(AnimationVmView, scale) == 0x3c) ? 1 : -1];
typedef char AnimationVmTimeOffset[(offsetof(AnimationVmView, time) == 0x5c) ? 1 : -1];
typedef char AnimationVmUvOffset[(offsetof(AnimationVmView, uv) == 0x70) ? 1 : -1];
typedef char AnimationVmPositionTweenOffset[(offsetof(AnimationVmView, positionTween) == 0x90) ? 1 : -1];
typedef char AnimationVmScaleTweenOffset[(offsetof(AnimationVmView, scaleTween) == 0x1a0) ? 1 : -1];
typedef char AnimationVmTransformOffset[(offsetof(AnimationVmView, transform) == 0x2b4) ? 1 : -1];
typedef char AnimationVmFileOffset[(offsetof(AnimationVmView, file) == 0x3b0) ? 1 : -1];
typedef char AnimationVmChildDurationOffset[(offsetof(AnimationVmView, duration3bc) == 0x3bc) ? 1 : -1];
typedef char AnimationVmPositionOffset[(offsetof(AnimationVmView, position) == 0x3dc) ? 1 : -1];
typedef char AnimationVmFlagsOffset[(offsetof(AnimationVmView, flags) == 0x404) ? 1 : -1];
typedef char AnimationVmTextureEntrySize[(sizeof(AnimationTextureEntry) == 20) ? 1 : -1];
typedef char AnimationFileScriptOffset[(offsetof(AnimationFileView, scripts) == 0x11c) ? 1 : -1];
typedef char AnimationFileTextureOffset[(offsetof(AnimationFileView, textures) == 0x120) ? 1 : -1];

void __stdcall ResetAnimationVm(AnimationVmView *self);
void __stdcall BindAnimationScript(AnimationFileView *file, int scriptId, AnimationVmView *self);
int __stdcall ExecuteAnimationVm(AnimationVmView *self);
