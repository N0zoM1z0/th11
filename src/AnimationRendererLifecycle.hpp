#pragma once

#include "StageResource.hpp"
#include "AnimationImmediateDraw.hpp"

struct AnimationRendererPrefixSlot
{
    int marker;
    unsigned char unknown04[0x24];
};
struct AnimationScratchVertex
{
    CameraVector position;
    float w, u, v;
};

// Complete observed allocation with typed VM lifetimes and partial physical fields.
// Unknown regions do not assert original classes or original data ownership.
struct AnimationRenderer
{
    AnimationRendererPrefixSlot prefix[4];
    unsigned char unknown0a0[8];
    unsigned int applications, drawCalls;
    float cameraOffsetX, cameraOffsetY;
    unsigned int unknown0b8;
    StageEmbeddedVm vms[0x1000];
    unsigned char used[0x1000];
    unsigned char unknown4350bc[0x435180 - 0x4350bc];
    StageEmbeddedVm defaultVm;
    unsigned int unknown4355b4;
    unsigned int textureFactor;
    AnimationDocumentTextureView *texture;
    unsigned char blend, cachedC1, geometry, cachedC3;
    unsigned char cachedC4, cachedC5, pointFilter, cachedC7;
    AnimationSpriteView *sprite;
    IDirect3DVertexBuffer9 *worldBuffer;
    AnimationWorldVertex corners[4];
    unsigned int quadCount;
    RenderVertex vertices[0x20000];
    RenderVertex *nextVertex, *batchStart;
    AnimationVmNode *activeHead, *activeTail, *otherHead, *otherTail;
    StageEmbeddedVm additionalVms[31];
    unsigned int unknown7bd888;
    unsigned int tint;
    int tintEnabled;

    AnimationRenderer();
    ~AnimationRenderer();
};

typedef char RendererHeadSize[(sizeof(AnimationRendererPrefixSlot) == 0x28) ? 1 : -1];
typedef char ScratchVertexSize[(sizeof(AnimationScratchVertex) == 24) ? 1 : -1];
typedef char RendererVmOffset[(offsetof(AnimationRenderer, vms) == 0xbc) ? 1 : -1];
typedef char RendererDefaultVmOffset[(offsetof(AnimationRenderer, defaultVm) == 0x435180) ? 1 : -1];
typedef char RendererBufferOffset[(offsetof(AnimationRenderer, worldBuffer) == 0x4355cc) ? 1 : -1];
typedef char RendererCornersOffset[(offsetof(AnimationRenderer, corners) == 0x4355d0) ? 1 : -1];
typedef char RendererAdditionalVmOffset[(offsetof(AnimationRenderer, additionalVms) == 0x7b563c) ? 1 : -1];
typedef char RendererAllocationSize[(sizeof(AnimationRenderer) == 0x7bd894) ? 1 : -1];

extern AnimationScratchVertex g_AnimationScratchQuad[4];
extern AnimationScratchVertex g_AnimationProjectedScratchQuad[4];
void __cdecl InitializeAnimationWorldBuffer();
// Actual reviewed VM-removal boundary, native private EDI renderer/EAX VM.
int __stdcall DestroyRendererAnimationVm(AnimationRenderer *renderer, AnimationVmView *vm);
