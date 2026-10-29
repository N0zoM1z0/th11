#include "SecondaryPairAccess.hpp"

void __stdcall SetSecondaryPairMode(SecondaryPairTween *self, int mode)
{
    self->mode = mode;
}

void __stdcall SetSecondaryPairInitial(SecondaryPairTween *self, const SecondaryVector2 *value)
{
    self->initial = *value;
}

void __stdcall SetSecondaryPairTarget(SecondaryPairTween *self, const SecondaryVector2 *value)
{
    self->target = *value;
}

void __stdcall ResetSecondaryPairControl1(SecondaryPairTween *self)
{
    self->control1.x = g_SecondaryPairControlPreset.x;
    self->control1.y = g_SecondaryPairControlPreset.y;
}

void __stdcall ResetSecondaryPairControl2(SecondaryPairTween *self)
{
    self->control2.x = g_SecondaryPairControlPreset.x;
    self->control2.y = g_SecondaryPairControlPreset.y;
}

void __stdcall SetSecondaryPairDuration(SecondaryPairTween *self, int duration)
{
    self->duration = duration;
}

void __stdcall ResetSecondaryPairTimer(SecondaryPairTween *self)
{
    if (!(self->time.flags & 1))
    {
        self->time.fraction = 0.0f;
        self->time.current = 0;
        self->time.previous = -999999;
        self->time.scale = &g_AnimationTimeScale;
        self->time.flags |= 1;
    }
    self->time.current = 0;
    self->time.previous = -1;
    self->time.fraction = 0.0f;
}

int __stdcall GetSecondaryPairDuration(const SecondaryPairTween *self)
{
    return self->duration;
}

void __stdcall StopSecondaryPairTween(SecondaryPairTween *self)
{
    self->duration = 0;
}

SecondaryVector2 *__stdcall SubtractSecondaryPair(SecondaryVector2 *output,
    const SecondaryVector2 *left, const SecondaryVector2 *right)
{
    output->x = left->x - right->x;
    output->y = left->y - right->y;
    return output;
}

SecondaryVector2 *__stdcall ScaleSecondaryPair(SecondaryVector2 *output,
    const SecondaryVector2 *input, float scale)
{
    output->x = input->x * scale;
    output->y = input->y * scale;
    return output;
}

SecondaryVector2 *__stdcall AddSecondaryPair(SecondaryVector2 *output,
    const SecondaryVector2 *left, const SecondaryVector2 *right)
{
    output->x = left->x + right->x;
    output->y = left->y + right->y;
    return output;
}
