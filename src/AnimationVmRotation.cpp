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
    float cosine = static_cast<float>(cos(self->angle));
    float sine = static_cast<float>(sin(self->angle));
    float centerX = self->offset.x + self->position.x + self->extraOffset.x;
    float centerY = self->offset.y + self->position.y + self->extraOffset.y;
    float width = self->width * self->scaleX;
    float height = self->height * self->scaleY;
    float localX[4], localY[4];

    switch ((self->flags >> 18) & 3)
    {
    case 0:
        localX[0] = localX[2] = static_cast<float>(-width * 0.5);
        localX[1] = localX[3] = static_cast<float>(width * 0.5);
        break;
    case 1:
        localX[0] = localX[2] = 0.0f;
        localX[1] = localX[3] = width;
        break;
    case 2:
        localX[0] = localX[2] = -width;
        localX[1] = localX[3] = 0.0f;
        break;
    }
    switch ((self->flags >> 20) & 3)
    {
    case 0:
        localY[0] = localY[1] = static_cast<float>(-height * 0.5);
        localY[2] = localY[3] = static_cast<float>(height * 0.5);
        break;
    case 1:
        localY[0] = localY[1] = 0.0f;
        localY[2] = localY[3] = height;
        break;
    case 2:
        localY[0] = localY[1] = -height;
        localY[2] = localY[3] = 0.0f;
        break;
    }
    // Selector 3 has no writes to this axis in the native owner. In particular
    // no default initialization or fallback corner is supplied here.
    first->x = centerX + (cosine * localX[0] - sine * localY[0]);
    first->y = centerY + (cosine * localY[0] + sine * localX[0]);
    second->x = centerX + (cosine * localX[1] - sine * localY[1]);
    second->y = centerY + (cosine * localY[1] + sine * localX[1]);
    third->x = centerX + (cosine * localX[2] - sine * localY[2]);
    third->y = centerY + (cosine * localY[2] + sine * localX[2]);
    fourth->x = centerX + (cosine * localX[3] - sine * localY[3]);
    fourth->y = centerY + (cosine * localY[3] + sine * localX[3]);
    float z = self->offset.z + self->position.z + self->extraOffset.z;
    fourth->z = third->z = second->z = first->z = z;
}
