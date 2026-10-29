#include "SecondaryActorRuntime.hpp"
#include "AngleMath.hpp"

void __stdcall QuantizeSecondaryMotionPosition(SecondaryMotionView *self);
void __stdcall RotateSecondaryVector(SecondaryVector2 *output, const SecondaryVector2 *input, float angle);

void __stdcall AdvanceSecondaryMotion(SecondaryMotionView *self)
{
    if (!(self->flags & 1))
    {
        reinterpret_cast<SecondaryVector2 *>(&self->velocity)->SetPolar(self->angle, self->speed);
        self->velocity.z = 0.0f;
        return;
    }
    self->radius = self->radialVelocity + self->radius;
    self->angle = NormalizeAngle(self->speed + self->angle);
}

void __stdcall ComposeSecondaryMotion(SecondaryMotionView *self)
{
    if (!(self->flags & 1))
    {
        self->position.x = self->velocity.x + self->position.x;
        self->position.y = self->velocity.y + self->position.y;
        self->position.z = self->velocity.z + self->position.z;
        QuantizeSecondaryMotionPosition(self);
        return;
    }
    SecondaryVector2 offset;
    if (!(self->flags & 2))
        offset.SetPolar(self->angle, self->radius);
    else
    {
        SecondaryVector2 local;
        local.SetPolar(NormalizeAngle(self->angle - self->tilt), self->radius);
        local.x = self->ellipseRatio * local.x;
        RotateSecondaryVector(&offset, &local, self->tilt);
    }
    CameraVector position = { self->velocity.x + offset.x,
        self->velocity.y + offset.y, static_cast<float>(self->velocity.z + 0.0) };
    self->position = position;
    QuantizeSecondaryMotionPosition(self);
}

int __stdcall OffsetSecondaryTimer(StageTimerView *self, float amount)
{
    self->previous = self->current;
    if (*self->scale > static_cast<double>(0.99f) && *self->scale < 1.01f)
        self->fraction = amount + self->fraction;
    else
    {
        amount = *self->scale * amount;
        self->fraction = amount + self->fraction;
    }
    self->current = static_cast<int>(self->fraction);
    return self->current;
}
