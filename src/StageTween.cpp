#include "StageTween.hpp"

int __stdcall AdvanceStageTimer(StageTimerView *self)
{
    int previous = self->current;
    self->previous = previous;
    if (*self->scale > static_cast<double>(0.99f) && *self->scale < 1.01f)
    {
        int current = static_cast<int>(static_cast<unsigned int>(previous) + 1);
        self->current = current;
        self->fraction = static_cast<float>(self->fraction + 1.0);
        return current;
    }
    self->fraction = *self->scale + self->fraction;
    self->current = static_cast<int>(self->fraction);
    return self->current;
}

// SetStageTimer, Pack and the fog operators are inline only in this TU; other
// TUs see the plain header declarations and call the out-of-line copies.
inline void __stdcall SetStageTimer(StageTimerView *self, int frame)
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

inline void StageFogView::Pack()
{
    unsigned char *bytes = reinterpret_cast<unsigned char *>(&packed);
    bytes[0] = static_cast<unsigned char>(static_cast<int>(channels[0]));
    bytes[1] = static_cast<unsigned char>(static_cast<int>(channels[1]));
    bytes[2] = static_cast<unsigned char>(static_cast<int>(channels[2]));
    bytes[3] = static_cast<unsigned char>(static_cast<int>(channels[3]));
}

inline StageFogView StageFogView::operator+(const StageFogView &other) const
{
    StageFogView result;
    result.start = start + other.start;
    result.end = end + other.end;
    result.channels[0] = channels[0] + other.channels[0];
    result.channels[1] = channels[1] + other.channels[1];
    result.channels[2] = channels[2] + other.channels[2];
    result.channels[3] = channels[3] + other.channels[3];
    result.Pack();
    return result;
}

inline StageFogView StageFogView::operator-(const StageFogView &other) const
{
    StageFogView result;
    result.start = start - other.start;
    result.end = end - other.end;
    result.channels[0] = channels[0] - other.channels[0];
    result.channels[1] = channels[1] - other.channels[1];
    result.channels[2] = channels[2] - other.channels[2];
    result.channels[3] = channels[3] - other.channels[3];
    result.Pack();
    return result;
}

inline StageFogView StageFogView::operator*(float scale) const
{
    StageFogView result;
    result.start = start * scale;
    result.end = end * scale;
    result.channels[0] = channels[0] * scale;
    result.channels[1] = channels[1] * scale;
    result.channels[2] = channels[2] * scale;
    result.channels[3] = channels[3] * scale;
    result.Pack();
    return result;
}

float __stdcall StageVectorTweenFactor(StageVectorTween *self)
{
    float progress = self->time.fraction / self->duration;
    unsigned int curve = static_cast<unsigned int>(self->mode) - 1;
    return curve <= 15 ? EvaluateStageEasing(curve, progress) : progress;
}

float __stdcall StageFogTweenFactor(StageFogTween *self)
{
    float progress = self->time.fraction / self->duration;
    unsigned int curve = static_cast<unsigned int>(self->mode) - 1;
    return curve <= 15 ? EvaluateStageEasing(curve, progress) : progress;
}

static __forceinline CameraVector ScaleTweenVector(const CameraVector &value, float scale)
{
    CameraVector result = {value.x * scale, value.y * scale, value.z * scale};
    return result;
}

static __forceinline CameraVector AddTweenVector(const CameraVector &left, const CameraVector &right)
{
    CameraVector result = {left.x + right.x, left.y + right.y, left.z + right.z};
    return result;
}

static __forceinline CameraVector SubtractTweenVector(const CameraVector &left, const CameraVector &right)
{
    CameraVector result = {left.x - right.x, left.y - right.y, left.z - right.z};
    return result;
}

static __forceinline void HermiteCoefficients(float progress, float &a, float &b, float &c, float &d)
{
    a = static_cast<float>((2.0 * progress + 1.0) * ((progress - 1.0) * (progress - 1.0)));
    b = static_cast<float>((3.0 - 2.0 * progress) * (progress * progress));
    c = static_cast<float>((1.0 - progress) * (1.0 - progress) * progress);
    d = static_cast<float>((progress - 1.0) * progress * progress);
}

CameraVector __stdcall StepStageVectorTween(StageVectorTween *self)
{
    if (self->duration > 0)
    {
        AdvanceStageTimer(&self->time);
        if (self->time.current >= self->duration)
        {
            SetStageTimer(&self->time, self->duration);
            self->duration = 0;
            if (self->mode != 7)
                return self->target;
            return self->initial;
        }
    }
    if (self->mode == 7)
    {
        CameraVector previous = self->initial;
        CameraVector result = {previous.x + self->target.x,
            previous.y + self->target.y, previous.z + self->target.z};
        self->initial = result;
        return result;
    }
    if (self->mode == 17)
    {
        CameraVector previous = self->initial;
        CameraVector result = AddTweenVector(previous, self->control2);
        self->initial = result;
        self->control2 = AddTweenVector(self->control2, self->target);
        return result;
    }
    if (self->mode == 8)
    {
        float progress = self->time.fraction / self->duration;
        float a = static_cast<float>((2.0 * progress + 1.0) * ((progress - 1.0) * (progress - 1.0)));
        float b = static_cast<float>((3.0 - 2.0 * progress) * (progress * progress));
        float c = static_cast<float>((1.0 - progress) * (1.0 - progress) * progress);
        float d = static_cast<float>((progress - 1.0) * progress * progress);
        return AddTweenVector(AddTweenVector(AddTweenVector(
            ScaleTweenVector(self->initial, a), ScaleTweenVector(self->target, b)),
            ScaleTweenVector(self->control1, c)), ScaleTweenVector(self->control2, d));
    }
    float factor = StageVectorTweenFactor(self);
    return AddTweenVector(ScaleTweenVector(SubtractTweenVector(self->target, self->initial), factor), self->initial);
}

StageFogView __stdcall StepStageFogTween(StageFogTween *self)
{
    if (self->duration > 0)
    {
        AdvanceStageTimer(&self->time);
        if (self->time.current >= self->duration)
        {
            SetStageTimer(&self->time, self->duration);
            self->duration = 0;
            if (self->mode != 7)
                return self->target;
            return self->initial;
        }
    }
    if (self->mode == 7)
    {
        StageFogView previous = self->initial;
        StageFogView result = previous + self->target;
        self->initial = result;
        return result;
    }
    if (self->mode == 17)
    {
        StageFogView previous = self->initial;
        StageFogView result = previous + self->control2;
        self->initial = result;
        self->control2 = self->control2 + self->target;
        return result;
    }
    if (self->mode == 8)
    {
        float progress = self->time.fraction / self->duration;
        float a, b, c, d;
        HermiteCoefficients(progress, a, b, c, d);
        return ((self->initial * a + self->target * b) + self->control1 * c) + self->control2 * d;
    }
    float factor = StageFogTweenFactor(self);
    return (self->target - self->initial) * factor + self->initial;
}
