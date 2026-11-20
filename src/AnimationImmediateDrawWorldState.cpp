#include "AnimationImmediateDraw.hpp"
#include "AnimationExecution.hpp"
#include "MainRuntime.hpp"
#include <d3d9.h>

namespace
{
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

typedef char ImmediateVmTweenStorageSize[
    (sizeof(ImmediateVmTweenStorage) == 0x224) ? 1 : -1];
typedef char ImmediateVmSize[(sizeof(ImmediateVmView) == 0x408) ? 1 : -1];
typedef char ImmediateVmRotationOffset[
    (offsetof(ImmediateVmView, rotation) == 0x24) ? 1 : -1];
typedef char ImmediateVmTimeOffset[
    (offsetof(ImmediateVmView, time) == 0x5c) ? 1 : -1];
typedef char ImmediateVmUvOffset[
    (offsetof(ImmediateVmView, uv) == 0x70) ? 1 : -1];
typedef char ImmediateVmTweenOffset[
    (offsetof(ImmediateVmView, tweenState) == 0x90) ? 1 : -1];
typedef char ImmediateVmTextureTransformOffset[
    (offsetof(ImmediateVmView, textureTransform) == 0x334) ? 1 : -1];
typedef char ImmediateVmColorOffset[
    (offsetof(ImmediateVmView, color) == 0x374) ? 1 : -1];
typedef char ImmediateVmSpriteOffset[
    (offsetof(ImmediateVmView, sprite) == 0x3ac) ? 1 : -1];
typedef char ImmediateVmFileOffset[
    (offsetof(ImmediateVmView, file) == 0x3b0) ? 1 : -1];
typedef char ImmediateVmStripPairsOffset[
    (offsetof(ImmediateVmView, stripPairs) == 0x3b4) ? 1 : -1];
typedef char ImmediateVmPositionOffset[
    (offsetof(ImmediateVmView, position) == 0x3dc) ? 1 : -1];
typedef char ImmediateVmVerticesOffset[
    (offsetof(ImmediateVmView, vertices) == 0x400) ? 1 : -1];
typedef char ImmediateVmFlagsOffset[
    (offsetof(ImmediateVmView, flags) == 0x404) ? 1 : -1];
typedef char ImmediateVmUpdateNodeOffset[
    (offsetof(ImmediateVmView, updateNode) == 0x004) ? 1 : -1];
typedef char ImmediateVmDrawNodeOffset[
    (offsetof(ImmediateVmView, drawNode) == 0x010) ? 1 : -1];
typedef char ImmediateVmAngularVelocityOffset[
    (offsetof(ImmediateVmView, angularVelocity) == 0x030) ? 1 : -1];
typedef char ImmediateVmScaleVelocityOffset[
    (offsetof(ImmediateVmView, scaleVelocityX) == 0x044) ? 1 : -1];
typedef char ImmediateVmWidthOffset[
    (offsetof(ImmediateVmView, width) == 0x04c) ? 1 : -1];
typedef char ImmediateVmSavedTimeOffset[
    (offsetof(ImmediateVmView, savedTime) == 0x380) ? 1 : -1];
typedef char ImmediateVmSavedInstructionOffset[
    (offsetof(ImmediateVmView, savedInstruction) == 0x394) ? 1 : -1];
typedef char ImmediateVmInstructionOffset[
    (offsetof(ImmediateVmView, instruction) == 0x3a8) ? 1 : -1];
typedef char ImmediateVmIntegerVariablesOffset[
    (offsetof(ImmediateVmView, integerVariables) == 0x3b8) ? 1 : -1];
typedef char ImmediateVmFloatVariablesOffset[
    (offsetof(ImmediateVmView, floatVariables) == 0x3c4) ? 1 : -1];
typedef char ImmediateVmExtraOffsetOffset[
    (offsetof(ImmediateVmView, extraOffset) == 0x3f4) ? 1 : -1];

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
    void ApplyWorldState(ImmediateVmView *vm);
};
struct ImmediateFogView
{
    float start, end, blue, green, red, alpha;
    unsigned int packed;
};
struct ImmediateCameraView
{
    unsigned char unknown000[0xfc];
    float fogStart, fogEnd;
    unsigned char unknown104[0x114 - 0x104];
    unsigned int fogColor;
};
struct ImmediateRuntimeView
{
    unsigned char unknown000[0x204];
    CameraVector fogEye;
    unsigned char unknown210[0x300 - 0x210];
    ImmediateFogView fog;
    unsigned char unknown31c[0x54c - 0x31c];
    ImmediateCameraView *camera;
};
typedef char ImmediateWorldVertexSize[(sizeof(AnimationWorldVertex) == 20) ? 1 : -1];
typedef char ImmediateRendererTextureOffset[(offsetof(ImmediateRendererView, texture) == 0x4355bc) ? 1 : -1];
typedef char ImmediateRendererCornersOffset[(offsetof(ImmediateRendererView, corners) == 0x4355d0) ? 1 : -1];
typedef char ImmediateRendererCountOffset[(offsetof(ImmediateRendererView, quadCount) == 0x435620) ? 1 : -1];
typedef char ImmediateRendererTintOffset[(offsetof(ImmediateRendererView, tint) == 0x7bd88c) ? 1 : -1];
typedef char ImmediateRuntimeCameraOffset[(offsetof(ImmediateRuntimeView, camera) == 0x54c) ? 1 : -1];

__forceinline IDirect3DDevice9 *Device()
{
    void * volatile *device = reinterpret_cast<void * volatile *>(&g_MainRuntime.d3dDevice);
    return static_cast<IDirect3DDevice9 *>(*device);
}
__forceinline unsigned int ScaleChannel(unsigned char value, unsigned char scale)
{
    unsigned int result = (static_cast<unsigned int>(value) * scale) >> 7;
    if (result >= 256) result = 255;
    return result;
}
}

void RenderBatchView::ApplyAnimationVmWorldState(AnimationVmView *vm)
{
    ImmediateVmView *self = reinterpret_cast<ImmediateVmView *>(vm);
    ImmediateRendererView *state = reinterpret_cast<ImmediateRendererView *>(this);
    RenderBatchView *renderer = this;
    if (state->blend != ((self->flags >> 4) & 7))
    {
        FlushRenderBatch(renderer);
        unsigned char blend = static_cast<unsigned char>((self->flags >> 4) & 7);
        state->blend = blend;
        switch (blend)
        {
        case 0:
            Device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            Device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); break;
        case 1:
            Device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            Device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE); break;
        case 2:
            Device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
            Device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR); break;
        case 3:
            Device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
            Device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO); break;
        case 4:
            Device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
            Device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); break;
        case 6:
            Device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCCOLOR);
            Device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); break;
        case 5:
            Device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
            Device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO); break;
        }
    }
    ImmediateColor color;
    color.value = (self->flags & 0x8000) ? self->alternateColor.value : self->color.value;
    if (state->tintEnabled)
    {
        color.bytes.red = static_cast<unsigned char>(ScaleChannel(color.bytes.red, state->tint.bytes.red));
        color.bytes.green = static_cast<unsigned char>(ScaleChannel(color.bytes.green, state->tint.bytes.green));
        color.bytes.blue = static_cast<unsigned char>(ScaleChannel(color.bytes.blue, state->tint.bytes.blue));
        color.bytes.alpha = static_cast<unsigned char>(ScaleChannel(color.bytes.alpha, state->tint.bytes.alpha));
    }
    if (state->textureFactor != color.value)
    {
        FlushRenderBatch(renderer);
        state->textureFactor = color.value;
        Device()->SetRenderState(D3DRS_TEXTUREFACTOR, color.value);
    }
    if (state->pointFilter != (self->flags >> 31))
    {
        FlushRenderBatch(renderer);
        unsigned char pointFilter = static_cast<unsigned char>((self->flags >> 31) & 1);
        state->pointFilter = pointFilter;
        if (!pointFilter)
        {
            Device()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
            Device()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        }
        else
        {
            Device()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
            Device()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        }
    }
    ++state->applications;
}
