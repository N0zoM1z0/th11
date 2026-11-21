#include "AnimationVmDraw.hpp"
#include <math.h>
#pragma intrinsic(sin, cos)

namespace
{
// Physical VM fields consumed by the rotated draw path. The shared VM view
// intentionally does not give names to every animation/rendering field.
struct RotatedDrawVmView
{
    unsigned char unknown000[0x2c];
    float angle;
    unsigned char unknown030[0x3c - 0x30];
    float scaleX, scaleY;
    unsigned char unknown044[0x4c - 0x44];
    float width, height;
    unsigned char unknown054[0x3dc - 0x54];
    CameraVector position, offset, extraOffset;
    unsigned char unknown400[4];
    unsigned int flags;
};
typedef char RotatedDrawAngleOffset[(offsetof(RotatedDrawVmView, angle) == 0x2c) ? 1 : -1];
typedef char RotatedDrawPositionOffset[(offsetof(RotatedDrawVmView, position) == 0x3dc) ? 1 : -1];
typedef char RotatedDrawFlagsOffset[(offsetof(RotatedDrawVmView, flags) == 0x404) ? 1 : -1];
}

// Full TH11 004503D0..0045060A rotated quad: four anchored local corners,
// one sine/cosine pair, a rotation around the summed VM position and shared Z.
void __stdcall BuildRotatedAnimationVmQuad(AnimationVmView *vm, CameraVector *first,
    CameraVector *second, CameraVector *third, CameraVector *fourth)
{
    const RotatedDrawVmView *self = reinterpret_cast<const RotatedDrawVmView *>(vm);
    float angle = self->angle;
    float cosine = static_cast<float>(cos(angle));
    float sine = static_cast<float>(sin(angle));
    float centerX = self->offset.x + self->position.x + self->extraOffset.x;
    float centerY = self->offset.y + self->position.y + self->extraOffset.y;
    float width = self->width * self->scaleX;
    float height = self->height * self->scaleY;
    float local_20, local_1c, local_18, local_14;
    float local_10, local_c, local_8, local_4;

    switch ((self->flags >> 18) & 3)
    {
    case 0:
        local_10 = static_cast<float>(-width * 0.5);
        local_c = static_cast<float>(width * 0.5);
        local_8 = local_10;
        local_4 = static_cast<float>(width * 0.5);
        break;
    case 1:
        local_8 = 0.0f;
        local_10 = local_8;
        local_c = width;
        local_4 = width;
        break;
    case 2:
        local_10 = -width;
        local_4 = 0.0f;
        local_c = local_4;
        local_8 = local_10;
        break;
    }
    switch ((self->flags >> 20) & 3)
    {
    case 0:
        local_20 = static_cast<float>(-height * 0.5);
        local_18 = static_cast<float>(height * 0.5);
        local_1c = local_20;
        local_14 = local_18;
        break;
    case 1:
        local_1c = 0.0f;
        local_20 = 0.0f;
        local_18 = height;
        local_14 = height;
        break;
    case 2:
        local_20 = -height;
        local_14 = 0.0f;
        local_18 = 0.0f;
        local_1c = local_20;
        break;
    }

    first->x = centerX + (cosine * local_10 - sine * local_20);
    first->y = centerY + (cosine * local_20 + sine * local_10);
    second->x = centerX + (local_c * cosine - local_1c * sine);
    second->y = centerY + (local_1c * cosine + local_c * sine);
    third->x = centerX + (local_8 * cosine - local_18 * sine);
    third->y = centerY + (local_18 * cosine + local_8 * sine);
    fourth->x = centerX + (local_4 * cosine - local_14 * sine);
    fourth->y = centerY + (local_14 * cosine + local_4 * sine);
    float z = self->offset.z + self->position.z + self->extraOffset.z;
    fourth->z = third->z = second->z = first->z = z;
}
