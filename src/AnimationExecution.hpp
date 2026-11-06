#pragma once
#include "AnimationVm.hpp"
#include "AnimationTween.hpp"
#include "AnimationVmFactory.hpp"
#include "RenderBatch.hpp"
#include "Random.hpp"

struct AnimationInstruction
{
    short opcode;
    unsigned short byteSize;
    short frame;
    unsigned short variables;
};
struct AnimationColorTriple { unsigned char blue, green, red; };
// Observed interpreter fields; this is not a recovered original class.
struct AnimationExecutionView
{
    unsigned char unknown000[0x24];
    CameraVector rotation, angularVelocity;
    AnimationFloatPair scale, scaleVelocity;
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
    unsigned int color, color2;
    short event;
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
    int integerVariables[4];
    float floatVariables[4];
    int integer3d4, integer3d8;
    CameraVector position, translation, otherPosition;
    RenderVertex *vertices;
    unsigned int flags, flags2;
    unsigned char unknown40c[0x428 - 0x40c];
    int (__fastcall *update)(AnimationVmView *);
    int (__fastcall *spriteTransform)(AnimationVmView *, int);
    unsigned int unknown430;
};

extern CameraVector g_AnimationExecutionZero;
extern RandomStateView g_AnimationSceneRandomState, g_TextRandomState;
float __stdcall ReadAnimationFloat(AnimationVmView *vm, float encoded);
int __stdcall ReadAnimationInteger(AnimationVmView *vm, int encoded);
float *__stdcall AnimationFloatDestination(AnimationVmView *vm, float *encoded);
int *__stdcall AnimationIntegerDestination(AnimationVmView *vm, int *encoded);
void __stdcall SetAnimationUv(AnimationFloatPair *output, const CameraVector *point);
void __stdcall ResetAnimationVectorTweenTime(StageVectorTween *self);
void __stdcall InsertAnimationRelatedNode(AnimationVmNode *after, AnimationVmNode *node);
void __stdcall SetAnimationScaleTween(AnimationVmView *vm,
    const AnimationFloatPair *target, const AnimationFloatPair *initial, int duration, unsigned char mode);
void __stdcall SetAnimationColorTween(AnimationVmView *vm, const AnimationColorTriple *target,
    const AnimationColorTriple *initial, int duration, unsigned char mode);
void __stdcall SetAnimationColor2Tween(AnimationVmView *vm, const AnimationColorTriple *target,
    const AnimationColorTriple *initial, int duration, unsigned char mode);
void __stdcall SetAnimationAlphaTween(AnimationVmView *vm, int duration,
    unsigned char mode, unsigned char initial, unsigned char target);
void __stdcall SetAnimationAlpha2Tween(AnimationVmView *vm, int duration,
    unsigned char mode, unsigned char initial, unsigned char target);

float __stdcall AnimationRandomRange(RandomStateView *random, float maximum);
// Reviewed input/return boundaries; these dependency bodies remain open.
float __stdcall AnimationRandomSignedRange(RandomStateView *random, float maximum);
float __stdcall AnimationRandomUnit(RandomStateView *random);
float __stdcall AnimationRandomSignedUnit(RandomStateView *random);
float __stdcall SecondaryUnaryMathA(float value);
float __stdcall SecondaryUnaryMathB(float value);
float __stdcall AnimationUnaryMathC(float value);
AnimationVmId __stdcall CreateFrontSecondaryDrawAnimationVmId(AnimationFileView *file, int script, int layer);
struct AnimationExecutionPolarPoint
{
    float x, y;
    void Set(float angle, float radius);
};

typedef char AnimationInstructionSize[(sizeof(AnimationInstruction) == 8) ? 1 : -1];
typedef char AnimationExecutionSize[(sizeof(AnimationExecutionView) == 0x434) ? 1 : -1];
typedef char AnimationExecutionTimeOffset[(offsetof(AnimationExecutionView, time) == 0x5c) ? 1 : -1];
typedef char AnimationExecutionPositionTweenOffset[(offsetof(AnimationExecutionView, positionTween) == 0x90) ? 1 : -1];
typedef char AnimationExecutionColorTweenOffset[(offsetof(AnimationExecutionView, colorTween) == 0xdc) ? 1 : -1];
typedef char AnimationExecutionRotationTweenOffset[(offsetof(AnimationExecutionView, rotationTween) == 0x154) ? 1 : -1];
typedef char AnimationExecutionScaleTweenOffset[(offsetof(AnimationExecutionView, scaleTween) == 0x1a0) ? 1 : -1];
typedef char AnimationExecutionUVVelocityOffset[(offsetof(AnimationExecutionView, uVelocity) == 0x2ac) ? 1 : -1];
typedef char AnimationExecutionColorOffset[(offsetof(AnimationExecutionView, color) == 0x374) ? 1 : -1];
typedef char AnimationExecutionSavedTimeOffset[(offsetof(AnimationExecutionView, savedTime) == 0x380) ? 1 : -1];
typedef char AnimationExecutionInstructionOffset[(offsetof(AnimationExecutionView, instruction) == 0x3a8) ? 1 : -1];
typedef char AnimationExecutionVariablesOffset[(offsetof(AnimationExecutionView, integerVariables) == 0x3b4) ? 1 : -1];
typedef char AnimationExecutionPositionOffset[(offsetof(AnimationExecutionView, position) == 0x3dc) ? 1 : -1];
typedef char AnimationExecutionFlagsOffset[(offsetof(AnimationExecutionView, flags) == 0x404) ? 1 : -1];
