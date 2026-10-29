#include "AnimationTween.hpp"

namespace
{
__forceinline int AddInt(int left, int right)
{
    return static_cast<int>(static_cast<unsigned int>(left) + static_cast<unsigned int>(right));
}
__forceinline int SubtractInt(int left, int right)
{
    return static_cast<int>(static_cast<unsigned int>(left) - static_cast<unsigned int>(right));
}
__forceinline void SetTweenTimer(StageTimerView *self, int frame)
{
    if (!(self->flags & 1))
    {
        self->fraction = 0.0f;
        self->current = 0;
        self->previous = -999999;
        self->scale = &g_AnimationTimeScale;
        self->flags |= 1;
    }
    self->current = frame;
    self->previous = static_cast<int>(static_cast<unsigned int>(frame) - 1);
    self->fraction = static_cast<float>(frame);
}
struct HermiteWeights { float a, b, c, d; };
__forceinline HermiteWeights Hermite(float progress)
{
    double t = progress;
    HermiteWeights result = {
        static_cast<float>((t + t + 1.0) * (t - 1.0) * (t - 1.0)),
        static_cast<float>(t * t * (3.0 - (t + t))),
        static_cast<float>((1.0 - t) * (1.0 - t) * t),
        static_cast<float>((t - 1.0) * t * t)};
    return result;
}
__forceinline AnimationIntVector Add(const AnimationIntVector &left, const AnimationIntVector &right)
{
    AnimationIntVector result = {AddInt(left.x, right.x), AddInt(left.y, right.y), AddInt(left.z, right.z)};
    return result;
}
__forceinline AnimationIntVector Subtract(const AnimationIntVector &left, const AnimationIntVector &right)
{
    AnimationIntVector result = {SubtractInt(left.x, right.x), SubtractInt(left.y, right.y), SubtractInt(left.z, right.z)};
    return result;
}
__forceinline AnimationIntVector Scale(const AnimationIntVector &value, float scale)
{
    AnimationIntVector result = {static_cast<int>(value.x * scale),
        static_cast<int>(value.y * scale), static_cast<int>(value.z * scale)};
    return result;
}
__forceinline AnimationFloatPair Add(const AnimationFloatPair &left, const AnimationFloatPair &right)
{
    AnimationFloatPair result = {left.x + right.x, left.y + right.y};
    return result;
}
__forceinline AnimationFloatPair Subtract(const AnimationFloatPair &left, const AnimationFloatPair &right)
{
    AnimationFloatPair result = {left.x - right.x, left.y - right.y};
    return result;
}
__forceinline AnimationFloatPair Scale(const AnimationFloatPair &value, float scale)
{
    AnimationFloatPair result = {value.x * scale, value.y * scale};
    return result;
}
}

AnimationIntVector __stdcall StepAnimationIntVectorTween(AnimationIntVectorTween *self)
{
    int duration = self->duration;
    if (duration > 0)
    {
        AdvanceStageTimer(&self->time);
        duration = self->duration;
        if (self->time.current >= duration)
        {
            SetTweenTimer(&self->time, self->duration);
            self->duration = 0;
            if (self->mode != 7) return self->target;
            return self->initial;
        }
    }
    if (self->mode == 7)
    {
        AnimationIntVector previous = self->initial;
        AnimationIntVector result = Add(previous, self->target);
        self->initial = result;
        return result;
    }
    if (self->mode == 17)
    {
        AnimationIntVector previous = self->initial;
        AnimationIntVector result = Add(previous, self->control2);
        self->initial = result;
        self->control2 = Add(self->control2, self->target);
        return result;
    }
    if (self->mode == 8)
    {
        HermiteWeights weights = Hermite(self->time.fraction / duration);
        return Add(Add(Add(Scale(self->initial, weights.a), Scale(self->target, weights.b)),
            Scale(self->control1, weights.c)), Scale(self->control2, weights.d));
    }
    float factor = AnimationIntVectorTweenFactor(self);
    return Add(Scale(Subtract(self->target, self->initial), factor), self->initial);
}

int __stdcall StepAnimationIntScalarTween(AnimationIntScalarTween *self)
{
    int duration = self->duration;
    if (duration > 0)
    {
        AdvanceStageTimer(&self->time);
        duration = self->duration;
        if (self->time.current >= duration)
        {
            SetTweenTimer(&self->time, self->duration);
            self->duration = 0;
            if (self->mode != 7) return self->target;
            return self->initial;
        }
    }
    if (self->mode == 7)
        return self->initial = AddInt(self->initial, self->target);
    if (self->mode == 17)
    {
        self->initial = AddInt(self->initial, self->control2);
        self->control2 = AddInt(self->control2, self->target);
        return self->initial;
    }
    if (self->mode == 8)
    {
        HermiteWeights weights = Hermite(self->time.fraction / duration);
        return static_cast<int>(weights.a * self->initial + weights.b * self->target +
            weights.c * self->control1 + weights.d * self->control2);
    }
    int initial = self->initial;
    float factor = AnimationIntScalarTweenFactor(self);
    return static_cast<int>(SubtractInt(self->target, initial) * factor + initial);
}

AnimationFloatPair __stdcall StepAnimationFloatPairTween(AnimationFloatPairTween *self)
{
    int duration = self->duration;
    if (duration > 0)
    {
        AdvanceStageTimer(&self->time);
        duration = self->duration;
        if (self->time.current >= duration)
        {
            SetTweenTimer(&self->time, self->duration);
            self->duration = 0;
            if (self->mode != 7) return self->target;
            return self->initial;
        }
    }
    if (self->mode == 7)
    {
        AnimationFloatPair previous = self->initial;
        self->initial = Add(previous, self->target);
        return self->initial;
    }
    if (self->mode == 17)
    {
        self->initial = Add(self->initial, self->control2);
        self->control2 = Add(self->control2, self->target);
        return self->initial;
    }
    if (self->mode == 8)
    {
        HermiteWeights weights = Hermite(self->time.fraction / duration);
        return Add(Add(Add(Scale(self->initial, weights.a), Scale(self->target, weights.b)),
            Scale(self->control1, weights.c)), Scale(self->control2, weights.d));
    }
    float factor = AnimationFloatPairTweenFactor(self);
    return Add(Scale(Subtract(self->target, self->initial), factor), self->initial);
}

float __stdcall StepAnimationFloatScalarTween(AnimationFloatScalarTween *self)
{
    int duration = self->duration;
    if (duration > 0)
    {
        AdvanceStageTimer(&self->time);
        duration = self->duration;
        if (self->time.current >= duration)
        {
            SetTweenTimer(&self->time, self->duration);
            self->duration = 0;
            if (self->mode != 7) return self->target;
            return self->initial;
        }
    }
    if (self->mode == 7)
        return self->initial = static_cast<float>(self->initial + self->target);
    if (self->mode == 17)
    {
        self->initial += self->control2;
        self->control2 += self->target;
        return self->initial;
    }
    if (self->mode == 8)
    {
        HermiteWeights weights = Hermite(self->time.fraction / duration);
        return static_cast<float>(weights.a * self->initial + weights.b * self->target +
            weights.c * self->control1 + weights.d * self->control2);
    }
    float factor = AnimationFloatScalarTweenFactor(self);
    return static_cast<float>((self->target - self->initial) * factor + self->initial);
}

float __stdcall AnimationIntVectorTweenFactor(AnimationIntVectorTween *self)
{
    float progress = self->time.fraction / self->duration;
    unsigned int curve = static_cast<unsigned int>(self->mode) - 1;
    return curve <= 15 ? EvaluateStageEasing(curve, progress) : progress;
}
float __stdcall AnimationIntScalarTweenFactor(AnimationIntScalarTween *self)
{
    float progress = self->time.fraction / self->duration;
    unsigned int curve = static_cast<unsigned int>(self->mode) - 1;
    return curve <= 15 ? EvaluateStageEasing(curve, progress) : progress;
}
float __stdcall AnimationFloatPairTweenFactor(AnimationFloatPairTween *self)
{
    float progress = self->time.fraction / self->duration;
    unsigned int curve = static_cast<unsigned int>(self->mode) - 1;
    return curve <= 15 ? EvaluateStageEasing(curve, progress) : progress;
}
float __stdcall AnimationFloatScalarTweenFactor(AnimationFloatScalarTween *self)
{
    float progress = self->time.fraction / self->duration;
    unsigned int curve = static_cast<unsigned int>(self->mode) - 1;
    return curve <= 15 ? EvaluateStageEasing(curve, progress) : progress;
}
void __stdcall ResetAnimationScalarTweenTime(AnimationScalarTweenTimeView *self)
{
    SetTweenTimer(&self->time, 0);
}

int __stdcall AnimationIntVectorTweenDuration(AnimationIntVectorTween *self) { return self->duration; }
int __stdcall AnimationFloatPairTweenDuration(AnimationFloatPairTween *self) { return self->duration; }
int __stdcall AnimationFloatScalarTweenDuration(AnimationFloatScalarTween *self) { return self->duration; }
void __stdcall SetAnimationFloatScalarTweenMode(AnimationFloatScalarTween *self, int mode) { self->mode = mode; }
void __stdcall SetAnimationFloatScalarTweenDuration(AnimationFloatScalarTween *self, int duration) { self->duration = duration; }
void __stdcall SetAnimationFloatScalarTweenInitial(AnimationFloatScalarTween *self, const float *value) { self->initial = *value; }
void __stdcall SetAnimationFloatScalarTweenTarget(AnimationFloatScalarTween *self, const float *value) { self->target = *value; }
void __stdcall SetAnimationFloatScalarTweenControl1(AnimationFloatScalarTween *self, const float *value) { self->control1 = *value; }
void __stdcall SetAnimationFloatScalarTweenControl2(AnimationFloatScalarTween *self, const float *value) { self->control2 = *value; }
