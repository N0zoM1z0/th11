#include "AnimationExecution.hpp"
#include "AnimationProjectedQuad.hpp"
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

namespace
{
__forceinline AnimationExecutionView *State(AnimationVmView *vm)
{ return reinterpret_cast<AnimationExecutionView *>(vm); }
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
    CameraVector world = Sum(Sum(self->position, self->otherPosition), self->translation);
    vertex[0].x = vertex[0].x + world.x;
    vertex[0].y = world.y + vertex[0].y;
    vertex[0].z = static_cast<float>(world.z + 0.0);
    ++vertex;
    vertex[0].color = color;
    vertex[0].rhw = 1.0f;
    vertex[0].u = self->uOffset + self->uv[1].x;
    vertex[0].v = v + self->vOffset;
    reinterpret_cast<AnimationExecutionPolarPoint *>(&vertex[0])->Set(angle,
        static_cast<float>(self->scale.y - self->scale.x * 0.5));
    vertex[0].z = 0.0f;
    CameraVector secondWorld = Sum(Sum(self->otherPosition, self->position), self->translation);
    vertex[0].x = vertex[0].x + secondWorld.x;
    vertex[0].y = secondWorld.y + vertex[0].y;
    vertex[0].z = static_cast<float>(secondWorld.z + 0.0);
    ++vertex;
}
}

int __stdcall ExecuteAnimationVm(AnimationVmView *vm)
{
    AnimationExecutionView *self = State(vm);
    if (!self->instruction) return 1;
    if (self->flags & 0x20000) return 0;
    float savedScale = g_AnimationTimeScale;
    if (self->flags & 0x20000000) g_AnimationTimeScale = 1.0f;
    AnimationInstruction *instruction;
    if (self->event) goto animation_event;
    for (;;)
    {
        instruction = self->instruction;
        if (instruction->frame > self->time.current) break;
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
        while ((label->opcode != 64 || Words(label)[0] != self->event) && label->opcode != -1)
        {
            if (label->opcode == 64 && Words(label)[0] == -1) fallback = label;
            label = Next(label);
        }
        self->flags &= ~0x1000u;
        self->event = 0;
        if (label->opcode != 64)
        {
            if (!fallback) goto animation_pause;
            label = fallback;
        }
        self->savedTime = self->time;
        self->savedInstruction = self->instruction;
        SetStageTimer(&self->time, label->frame);
        self->flags |= 1;
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
