#include "SecondaryMotionMath.hpp"
#include <math.h>
#pragma intrinsic(sin, cos)

void __stdcall QuantizeSecondaryMotionPosition(SecondaryMotionView *self)
{
    float value = static_cast<float>(self->position.x * 100.0);
    float rounded = floor(value);
    self->position.x = static_cast<float>(rounded / 100.0);
    value = static_cast<float>(100.0 * self->position.y);
    rounded = floor(value);
    self->position.y = static_cast<float>(rounded / 100.0);
}

void __stdcall RotateSecondaryVector(SecondaryVector2 *output, const SecondaryVector2 *input, float angle)
{
    float sine = sin(angle);
    float cosine = cos(angle);
    output->x = input->x * cosine - sine * input->y;
    output->y = input->x * sine + cosine * input->y;
}
