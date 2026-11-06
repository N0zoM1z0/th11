#include "AnimationExecution.hpp"

namespace
{
__forceinline AnimationExecutionView *State(AnimationVmView *vm)
{ return reinterpret_cast<AnimationExecutionView *>(vm); }
__forceinline void ResetTime(StageTimerView *time)
{ time->SetFrame(-1); }
__forceinline void ConfigureColor(AnimationIntVectorTween *self,
    const AnimationColorTriple *target, const AnimationColorTriple *initial,
    int duration, unsigned char mode)
{
    self->duration = duration;
    self->control1.x = self->control1.y = self->control1.z = 0;
    self->control2.x = self->control2.y = self->control2.z = 0;
    self->mode = mode;
    self->target.x = target->blue;
    self->target.y = target->green;
    self->target.z = target->red;
    self->initial.x = initial->blue;
    self->initial.y = initial->green;
    self->initial.z = initial->red;
    ResetTime(&self->time);
}
}

void __stdcall ResetAnimationVectorTweenTime(StageVectorTween *self)
{ ResetTime(&self->time); }

void __stdcall InsertAnimationRelatedNode(AnimationVmNode *after, AnimationVmNode *node)
{
    if (after->next)
    {
        node->next = after->next;
        after->next->previous = node;
    }
    after->next = node;
    node->previous = after;
}

void __stdcall SetAnimationColorTween(AnimationVmView *vm,
    const AnimationColorTriple *target, const AnimationColorTriple *initial,
    int duration, unsigned char mode)
{ ConfigureColor(&State(vm)->colorTween, target, initial, duration, mode); }

void __stdcall SetAnimationColor2Tween(AnimationVmView *vm,
    const AnimationColorTriple *target, const AnimationColorTriple *initial,
    int duration, unsigned char mode)
{ ConfigureColor(&State(vm)->color2Tween, target, initial, duration, mode); }

void __stdcall SetAnimationAlphaTween(AnimationVmView *vm, int duration,
    unsigned char mode, unsigned char initial, unsigned char target)
{
    AnimationIntScalarTween *self = &State(vm)->alphaTween;
    self->duration = duration;
    self->mode = mode;
    self->initial = initial;
    self->control1 = self->control2 = 0;
    self->target = target;
    ResetTime(&self->time);
}

void __stdcall SetAnimationAlpha2Tween(AnimationVmView *vm, int duration,
    unsigned char mode, unsigned char initial, unsigned char target)
{
    AnimationIntScalarTween *self = &State(vm)->alpha2Tween;
    self->mode = mode;
    self->duration = duration;
    self->target = target;
    self->initial = initial;
    ResetTime(&self->time);
}

void __stdcall SetAnimationScaleTween(AnimationVmView *vm,
    const AnimationFloatPair *target, const AnimationFloatPair *initial,
    int duration, unsigned char mode)
{
    AnimationFloatPairTween *self = &State(vm)->scaleTween;
    self->duration = duration;
    self->mode = mode;
    self->initial = *initial;
    self->target = *target;
    ResetTime(&self->time);
}

float __stdcall AnimationRandomRange(RandomStateView *random, float maximum)
{
    float unit = static_cast<float>(static_cast<float>(RandomNext32(random)) *
        (1.0 / 4294967296.0));
    maximum *= unit;
    return maximum;
}
