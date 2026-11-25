#include "AnimationExecution.hpp"
#include "AnimationProjectedQuad.hpp"
#include "AnimationQuad.hpp"
#include "AnimationSpriteBinding.hpp"
#include "AnimationFan.hpp"
#include "SecondaryEmitterAnimation.hpp"

#include "AngleMath.hpp"
#include "MainRuntime.hpp"
#include <math.h>
#include <stdlib.h>

AnimationVmView *__stdcall ClearMissingAnimationId(unsigned int *id);
int __stdcall OffsetSecondaryTimer(StageTimerView *self, float amount);
void __stdcall RewindSecondaryTimer(StageTimerView *self, int frame);

__declspec(noinline) void __fastcall BuildModeQuad(CameraVector *quad, AnimationVmView *vm)
{
    switch ((vm->flags.value >> 22) & 15)
    {
    case 0:
        BuildAnimationVmQuad(vm, &quad[0], &quad[1], &quad[2], &quad[3]);
        break;
    case 1:
        BuildRotatedAnimationVmQuad(vm, &quad[0], &quad[1], &quad[2], &quad[3]);
        break;
    case 2:
    case 3:
        BuildUnroundedAnimationVmQuad(vm, &quad[0], &quad[1], &quad[2], &quad[3]);
        break;
    }
}

namespace
{
__forceinline AnimationExecutionView *State(AnimationVmView *vm)
{ return vm; }
#pragma optimize("y", on)
__forceinline RandomStateView *ExecutionRandom(AnimationVmView *vm)
{ return (vm->flags.value & 0x40000000) ? &g_AnimationSceneRandomState : &g_TextRandomState; }
__forceinline float ExecutionRuntimeFloat(unsigned int offset)
{ return *reinterpret_cast<float *>(reinterpret_cast<unsigned char *>(&g_MainRuntime) + offset); }
float __stdcall ReadAnimationFloat(AnimationVmView *vm, float encoded)
{
    AnimationExecutionView *self = State(vm);
    float randomResult;
    switch (static_cast<int>(encoded))
    {
    case 10000: return static_cast<float>(self->integerVariables[0]);
    case 10001: return static_cast<float>(self->integerVariables[1]);
    case 10002: return static_cast<float>(self->integerVariables[2]);
    case 10003: return static_cast<float>(self->integerVariables[3]);
    case 10004: return self->floatVariables[0];
    case 10005: return self->floatVariables[1];
    case 10006: return self->floatVariables[2];
    case 10007: return self->floatVariables[3];
    case 10008: return static_cast<float>(self->integer3d4);
    case 10009: return static_cast<float>(self->integer3d8);
    case 10011: randomResult = AnimationRandomUnit(ExecutionRandom(vm)); goto animation_random_result;
    case 10012: randomResult = AnimationRandomSignedUnit(ExecutionRandom(vm)); goto animation_random_result;
    case 10010: randomResult = AnimationRandomSignedRange(ExecutionRandom(vm), 3.1415927f); goto animation_random_result;
    case 10013: return self->position.x;
    case 10014: return self->position.y;
    case 10015: return self->position.z;
    case 10016: return ExecutionRuntimeFloat(0x434);
    case 10017: return ExecutionRuntimeFloat(0x438);
    case 10018: return ExecutionRuntimeFloat(0x43c);
    case 10019: return ExecutionRuntimeFloat(0x458);
    case 10020: return ExecutionRuntimeFloat(0x45c);
    case 10021: return ExecutionRuntimeFloat(0x460);
    case 10022: return static_cast<float>(RandomNext32(ExecutionRandom(vm)));
    default: return encoded;
    }
animation_random_result:
    return randomResult;
}
int __stdcall ReadAnimationInteger(AnimationVmView *vm, int encoded)
{
    AnimationExecutionView *self = State(vm);
    switch (encoded)
    {
    case 10000: return self->integerVariables[0];
    case 10001: return self->integerVariables[1];
    case 10002: return self->integerVariables[2];
    case 10003: return self->integerVariables[3];
    case 10004: return static_cast<int>(self->floatVariables[0]);
    case 10005: return static_cast<int>(self->floatVariables[1]);
    case 10006: return static_cast<int>(self->floatVariables[2]);
    case 10007: return static_cast<int>(self->floatVariables[3]);
    case 10008: return self->integer3d4;
    case 10009: return self->integer3d8;
    case 10022: return static_cast<int>(RandomNext32(ExecutionRandom(vm)));
    default: return encoded;
    }
}
int *__stdcall AnimationIntegerDestination(AnimationVmView *vm, int *encoded)
{
    AnimationExecutionView *self = State(vm);
    switch (*encoded)
    {
    case 10000: return &self->integerVariables[0];
    case 10001: return &self->integerVariables[1];
    case 10002: return &self->integerVariables[2];
    case 10003: return &self->integerVariables[3];
    case 10008: return &self->integer3d4;
    case 10009: return &self->integer3d8;
    default: return encoded;
    }
}
#pragma optimize("y", off)
__forceinline int *Words(AnimationInstruction *instruction)
{ return reinterpret_cast<int *>(instruction + 1); }
__forceinline float *Floats(AnimationInstruction *instruction)
{ return reinterpret_cast<float *>(instruction + 1); }
__forceinline int ReadI(AnimationVmView *vm, AnimationInstruction *instruction, int index)
{
    int value = Words(instruction)[index];
    return (instruction->variables & (1u << index)) ? ReadAnimationInteger(vm, value) : value;
}
__forceinline float ReadF(AnimationVmView *vm, AnimationInstruction *instruction, int index)
{
    if (instruction->variables & (1u << index))
        return ReadAnimationFloat(vm, Floats(instruction)[index]);
    return Floats(instruction)[index];
}
__forceinline int *IntegerDestination(AnimationVmView *vm, AnimationInstruction *instruction)
{
    int *value = Words(instruction);
    return (instruction->variables & 1) ? AnimationIntegerDestination(vm, value) : value;
}
__forceinline float *FloatDestination(AnimationVmView *vm, AnimationInstruction *instruction)
{
    float *value = Floats(instruction);
    return (instruction->variables & 1) ? AnimationFloatDestination(vm, value) : value;
}
__forceinline int Add(int left, int right)
{ return static_cast<int>(static_cast<unsigned int>(left) + right); }
__forceinline int Subtract(int left, int right)
{ return static_cast<int>(static_cast<unsigned int>(left) - right); }
__forceinline int Multiply(int left, int right)
{ return static_cast<int>(static_cast<unsigned int>(left) * right); }
__forceinline unsigned char &Byte(void *self, unsigned int offset)
{ return reinterpret_cast<unsigned char *>(self)[offset]; }
__forceinline void ReplaceBits(unsigned int &flags, unsigned int value, unsigned int mask)
{ flags ^= (value ^ flags) & mask; }
__forceinline AnimationInstruction *Next(AnimationInstruction *instruction)
{ return reinterpret_cast<AnimationInstruction *>(reinterpret_cast<unsigned char *>(instruction) + instruction->byteSize); }
__forceinline CameraVector Sum(const CameraVector &left, const CameraVector &right)
{
    CameraVector result = {left.x + right.x, left.y + right.y, left.z + right.z};
    return result;
}
__forceinline void Tick(StageTimerView *timer)
{
    int previous = timer->current;
    timer->previous = previous;
    if (*timer->scale > static_cast<double>(0.99f) && *timer->scale < 1.01f)
    {
        timer->current = Add(previous, 1);
        timer->fraction = static_cast<float>(timer->fraction + 1.0);
    }
    else
    {
        timer->fraction = *timer->scale + timer->fraction;
        timer->current = static_cast<int>(timer->fraction);
    }
}
__forceinline void ZeroControls(StageVectorTween &tween)
{
    tween.control1 = g_AnimationExecutionZero;
    tween.control2 = g_AnimationExecutionZero;
}
__forceinline void InlineUv(AnimationFloatPair *output, const CameraVector *point)
{
    output->x = static_cast<float>(point->x / 640.0);
    output->y = static_cast<float>(point->y / 480.0);
    if (output->x < 0.0f) output->x = 0.0f;
    if (output->y < 0.0f) output->y = 0.0f;
}
__forceinline void EmitStripPair(AnimationExecutionView *self, RenderVertex *&vertex,
    unsigned int color, float angle, float v)
{
    vertex[0].color = color;
    vertex[0].rhw = 1.0f;
    vertex[0].u = self->uOffset + self->uv[0].x;
    vertex[0].v = v + self->vOffset;
    reinterpret_cast<AnimationExecutionPolarPoint *>(&vertex[0])->Set(angle,
        static_cast<float>(self->scale.x * 0.5 + self->scale.y));
    vertex[0].z = 0.0f;
    float worldX = (self->position.x + self->otherPosition.x) + self->translation.x;
    float worldY = (self->position.y + self->otherPosition.y) + self->translation.y;
    float worldZ = (self->position.z + self->otherPosition.z) + self->translation.z;
    vertex[0].x = vertex[0].x + worldX;
    vertex[0].y = worldY + vertex[0].y;
    vertex[0].z = static_cast<float>(worldZ + 0.0);
    ++vertex;
    vertex[0].color = color;
    vertex[0].rhw = 1.0f;
    vertex[0].u = self->uOffset + self->uv[1].x;
    vertex[0].v = v + self->vOffset;
    reinterpret_cast<AnimationExecutionPolarPoint *>(&vertex[0])->Set(angle,
        static_cast<float>(self->scale.y - self->scale.x * 0.5));
    vertex[0].z = 0.0f;
    float secondWorldX = (self->otherPosition.x + self->position.x) + self->translation.x;
    float secondWorldY = (self->otherPosition.y + self->position.y) + self->translation.y;
    float secondWorldZ = (self->otherPosition.z + self->position.z) + self->translation.z;
    vertex[0].x = vertex[0].x + secondWorldX;
    vertex[0].y = secondWorldY + vertex[0].y;
    vertex[0].z = static_cast<float>(secondWorldZ + 0.0);
    ++vertex;
}
}

int __stdcall ExecuteAnimationVm(AnimationVmView *vm)
{
    register AnimationInstruction *instruction = State(vm)->instruction;
    register AnimationExecutionView *self = State(vm);
    union
    {
        float floatResult;
        unsigned int childId;
        AnimationVmView *child;
    } scratch;
    if (!instruction) return 1;
    if (self->flags.value & 0x20000) return 0;
    float savedScale = g_AnimationTimeScale;
    if (self->flags.value & 0x20000000) g_AnimationTimeScale = 1.0f;
    unsigned short event;
    if (self->event)
    {
        event = self->event;
        goto animation_event;
    }
    while ((instruction = self->instruction), instruction->frame <= self->time.current)
    {
        switch (instruction->opcode)
        {
#include "AnimationExecutionInstructions.inc"
        default:
            break;
        }
        self->instruction = Next(instruction);
        continue;
animation_jump:
        SetStageTimer(&self->time, Words(instruction)[3]);
        self->instruction = reinterpret_cast<AnimationInstruction *>(self->scriptStart + Words(instruction)[2]);
        continue;
animation_event:
        AnimationInstruction *fallback = 0;
        AnimationInstruction *label = reinterpret_cast<AnimationInstruction *>(self->scriptStart);
        while ((label->opcode != 64 || event != Words(label)[0]) && label->opcode != -1)
        {
            if (label->opcode == 64 && -1 == Words(label)[0]) fallback = label;
            label = Next(label);
        }
        self->flags.value &= ~0x1000u;
        self->event = 0;
        if (label->opcode != 64)
        {
            if (!fallback) goto animation_pause;
            label = fallback;
        }
        self->savedTime = self->time;
        self->savedInstruction = self->instruction;
        SetStageTimer(&self->time, label->frame);
        self->flags.value |= 1;
        self->instruction = Next(label);
    }
    goto animation_update;
animation_pause:
    OffsetSecondaryTimer(&self->time, -1.0f);
animation_update:
#include "AnimationExecutionUpdate.inc"
    if (self->update && self->update(vm)) return 1;
    Tick(&self->time);
    g_AnimationTimeScale = savedScale;
    return 0;
}
