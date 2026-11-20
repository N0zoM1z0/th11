#pragma once

#include "AnimationImmediateDraw.hpp"
#include "AnimationExecution.hpp"
#include "AnimationDocument.hpp"

union ImmediateColor
{
    unsigned int value;
    struct { unsigned char blue, green, red, alpha; } bytes;
};
struct ImmediateUv { float u, v; };
struct ImmediateVmTweenStorage
{
    StageVectorTween positionTween;
    AnimationIntVectorTween colorTween;
    AnimationIntScalarTween alphaTween;
    StageVectorTween rotationTween;
    AnimationFloatPairTween scaleTween;
    AnimationIntVectorTween color2Tween;
    AnimationIntScalarTween alpha2Tween;
    AnimationFloatScalarTween uVelocityTween, vVelocityTween;
    float uVelocity, vVelocity;
};

struct ImmediateVmView
{
    unsigned int unknown000;
    AnimationVmNode updateNode, drawNode;
    unsigned int unknown01c, persistent020;
    CameraVector rotation, angularVelocity;
    float scaleX, scaleY;
    float scaleVelocityX, scaleVelocityY;
    float width, height, offsetU, offsetV;
    StageTimerView time;
    ImmediateUv uv[4];
    ImmediateVmTweenStorage tweenState;
    D3DMATRIX base, transformed, textureTransform;
    ImmediateColor color, alternateColor;
    short event;
    unsigned short unknown37e;
    StageTimerView savedTime;
    AnimationInstruction *savedInstruction;
    int spriteFrame;
    short marker;
    unsigned short fileId, unknown3a0, scriptId;
    unsigned char *scriptStart;
    AnimationInstruction *instruction;
    AnimationSpriteView *sprite;
    AnimationFileView *file;
    unsigned int stripPairs;
    int integerVariables[3];
    float floatVariables[4];
    int integer3d4, integer3d8;
    CameraVector position, offset, extraOffset;
    RenderVertex *vertices;
    unsigned int flags;

    int DrawWorld(RenderBatchView *renderer);
};

typedef char ImmediateVmTweenStorageSize[(sizeof(ImmediateVmTweenStorage) == 0x224) ? 1 : -1];
typedef char ImmediateVmSize[(sizeof(ImmediateVmView) == 0x408) ? 1 : -1];
typedef char ImmediateVmRotationOffset[(offsetof(ImmediateVmView, rotation) == 0x24) ? 1 : -1];
typedef char ImmediateVmTimeOffset[(offsetof(ImmediateVmView, time) == 0x5c) ? 1 : -1];
typedef char ImmediateVmUvOffset[(offsetof(ImmediateVmView, uv) == 0x70) ? 1 : -1];
typedef char ImmediateVmTweenOffset[(offsetof(ImmediateVmView, tweenState) == 0x90) ? 1 : -1];
typedef char ImmediateVmTextureTransformOffset[(offsetof(ImmediateVmView, textureTransform) == 0x334) ? 1 : -1];
typedef char ImmediateVmColorOffset[(offsetof(ImmediateVmView, color) == 0x374) ? 1 : -1];
typedef char ImmediateVmSpriteOffset[(offsetof(ImmediateVmView, sprite) == 0x3ac) ? 1 : -1];
typedef char ImmediateVmFileOffset[(offsetof(ImmediateVmView, file) == 0x3b0) ? 1 : -1];
typedef char ImmediateVmStripPairsOffset[(offsetof(ImmediateVmView, stripPairs) == 0x3b4) ? 1 : -1];
typedef char ImmediateVmPositionOffset[(offsetof(ImmediateVmView, position) == 0x3dc) ? 1 : -1];
typedef char ImmediateVmVerticesOffset[(offsetof(ImmediateVmView, vertices) == 0x400) ? 1 : -1];
typedef char ImmediateVmFlagsOffset[(offsetof(ImmediateVmView, flags) == 0x404) ? 1 : -1];
typedef char ImmediateVmUpdateNodeOffset[(offsetof(ImmediateVmView, updateNode) == 0x004) ? 1 : -1];
typedef char ImmediateVmDrawNodeOffset[(offsetof(ImmediateVmView, drawNode) == 0x010) ? 1 : -1];
typedef char ImmediateVmAngularVelocityOffset[(offsetof(ImmediateVmView, angularVelocity) == 0x030) ? 1 : -1];
typedef char ImmediateVmScaleVelocityOffset[(offsetof(ImmediateVmView, scaleVelocityX) == 0x044) ? 1 : -1];
typedef char ImmediateVmWidthOffset[(offsetof(ImmediateVmView, width) == 0x04c) ? 1 : -1];
typedef char ImmediateVmSavedTimeOffset[(offsetof(ImmediateVmView, savedTime) == 0x380) ? 1 : -1];
typedef char ImmediateVmSavedInstructionOffset[(offsetof(ImmediateVmView, savedInstruction) == 0x394) ? 1 : -1];
typedef char ImmediateVmInstructionOffset[(offsetof(ImmediateVmView, instruction) == 0x3a8) ? 1 : -1];
typedef char ImmediateVmIntegerVariablesOffset[(offsetof(ImmediateVmView, integerVariables) == 0x3b8) ? 1 : -1];
typedef char ImmediateVmFloatVariablesOffset[(offsetof(ImmediateVmView, floatVariables) == 0x3c4) ? 1 : -1];
typedef char ImmediateVmExtraOffsetOffset[(offsetof(ImmediateVmView, extraOffset) == 0x3f4) ? 1 : -1];

struct ImmediateRendererView
{
    unsigned char unknown000[0xa8];
    unsigned int applications;
    unsigned char unknown0ac[0x435140 - 0xac];
    D3DMATRIX world;
    unsigned char unknown435180[0x4355b8 - 0x435180];
    unsigned int textureFactor;
    AnimationDocumentTextureView *texture;
    unsigned char blend, unknown4355c1, geometry;
    unsigned char unknown4355c3[3];
    unsigned char pointFilter, unknown4355c7;
    AnimationSpriteView *sprite;
    IDirect3DVertexBuffer9 *worldVertices;
    AnimationWorldVertex corners[4];
    unsigned int quadCount;
    unsigned char unknown435624[0x7bd88c - 0x435624];
    ImmediateColor tint;
    int tintEnabled;
};

typedef char ImmediateWorldVertexSize[(sizeof(AnimationWorldVertex) == 20) ? 1 : -1];
typedef char ImmediateRendererTextureOffset[(offsetof(ImmediateRendererView, texture) == 0x4355bc) ? 1 : -1];
typedef char ImmediateRendererCornersOffset[(offsetof(ImmediateRendererView, corners) == 0x4355d0) ? 1 : -1];
typedef char ImmediateRendererCountOffset[(offsetof(ImmediateRendererView, quadCount) == 0x435620) ? 1 : -1];
typedef char ImmediateRendererTintOffset[(offsetof(ImmediateRendererView, tint) == 0x7bd88c) ? 1 : -1];
