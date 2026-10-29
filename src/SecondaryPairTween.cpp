#include "SecondaryActorState.hpp"
#include "StageTween.hpp"

namespace
{
__forceinline SecondaryVector2 AddPair(const SecondaryVector2 &left, const SecondaryVector2 &right)
{
    SecondaryVector2 result = { left.x + right.x, left.y + right.y };
    return result;
}
__forceinline SecondaryVector2 SubtractPair(const SecondaryVector2 &left, const SecondaryVector2 &right)
{
    SecondaryVector2 result = { left.x - right.x, left.y - right.y };
    return result;
}
__forceinline SecondaryVector2 ScalePair(const SecondaryVector2 &value, float scale)
{
    SecondaryVector2 result = { value.x * scale, value.y * scale };
    return result;
}
}

float __stdcall SecondaryPairTweenFactor(SecondaryPairTween *self)
{
    float progress = self->time.fraction / self->duration;
    unsigned int curve = static_cast<unsigned int>(self->mode) - 1;
    return curve <= 15 ? EvaluateStageEasing(curve, progress) : progress;
}

SecondaryVector2 *__stdcall StepSecondaryPairTween(SecondaryVector2 *result, SecondaryPairTween *self)
{
    int duration = self->duration;
    if (duration > 0)
    {
        AdvanceStageTimer(&self->time);
        duration = self->duration;
        if (self->time.current >= duration)
        {
            if (!(self->time.flags & 1))
            {
                self->time.fraction = 0.0f;
                self->time.current = 0;
                self->time.previous = -999999;
                self->time.scale = &g_AnimationTimeScale;
                self->time.flags |= 1;
            }
            self->time.current = duration;
            self->time.previous = static_cast<int>(static_cast<unsigned int>(duration) - 1);
            self->time.fraction = static_cast<float>(duration);
            self->duration = 0;
            *result = self->mode == 7 ? self->initial : self->target;
            return result;
        }
    }
    if (self->mode == 7)
    {
        SecondaryVector2 value = AddPair(self->initial, self->target);
        self->initial = value;
        *result = value;
        return result;
    }
    if (self->mode == 17)
    {
        SecondaryVector2 value = AddPair(self->initial, self->control2);
        self->initial = value;
        *result = value;
        self->control2 = AddPair(self->control2, self->target);
        return result;
    }
    if (self->mode == 8)
    {
        float progress = self->time.fraction / duration;
        double t = progress;
        float a = static_cast<float>((t + t + 1.0) * (t - 1.0) * (t - 1.0));
        float b = static_cast<float>(t * t * (3.0 - (t + t)));
        float c = static_cast<float>((1.0 - t) * (1.0 - t) * t);
        float d = static_cast<float>((t - 1.0) * t * t);
        *result = AddPair(AddPair(AddPair(ScalePair(self->initial, a),
            ScalePair(self->target, b)), ScalePair(self->control1, c)),
            ScalePair(self->control2, d));
        return result;
    }
    float factor = SecondaryPairTweenFactor(self);
    *result = AddPair(ScalePair(SubtractPair(self->target, self->initial), factor), self->initial);
    return result;
}
