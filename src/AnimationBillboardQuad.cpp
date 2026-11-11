#include "AnimationProjectedQuad.hpp"
#include "PerspectiveCamera.hpp"
#include "MainRuntime.hpp"
#include <math.h>

#pragma intrinsic(sin, cos)

namespace
{
// Physical animation VM fields used by the billboard-only draw path.
// Original class and TU ownership are not inferred from this local view.
struct BillboardVmView
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
struct BillboardRuntimeView
{
    unsigned char unknown000[0x54c];
    PerspectiveCameraView *camera;
};
typedef char BillboardAngleOffset[(offsetof(BillboardVmView, angle) == 0x2c) ? 1 : -1];
typedef char BillboardPositionOffset[(offsetof(BillboardVmView, position) == 0x3dc) ? 1 : -1];
typedef char BillboardFlagsOffset[(offsetof(BillboardVmView, flags) == 0x404) ? 1 : -1];
typedef char BillboardCameraOffset[(offsetof(BillboardRuntimeView, camera) == 0x54c) ? 1 : -1];
}

// TH11 00450700..00450ACF (976 bytes). A camera-facing billboard uses the
// projected size of one camera-right vector instead of perspective matrix
// scale estimates; this is not the flat/unrotated VM quad path.
int __stdcall BuildBillboardAnimationVmQuad(AnimationVmView *vm)
{
    BillboardVmView *self = reinterpret_cast<BillboardVmView *>(vm);
    BillboardRuntimeView *runtime = reinterpret_cast<BillboardRuntimeView *>(&g_MainRuntime);
    PerspectiveCameraView *camera = runtime->camera;

    // Unit world axes with the VM's summed translation. The native code
    // projects an origin with this matrix before constructing any corners.
    D3DMATRIX world = {};
    world._11 = 1.0f;
    world._22 = 1.0f;
    world._33 = 1.0f;
    world._44 = 1.0f;
    world._41 = self->offset.x + self->position.x + self->extraOffset.x;
    world._42 = self->offset.y + self->position.y + self->extraOffset.y;
    world._43 = self->offset.z + self->position.z + self->extraOffset.z;

    CameraVector origin = {0.0f, 0.0f, 0.0f};
    CameraVector screenCenter;
    D3DXVec3Project(&screenCenter, &origin, &camera->viewport,
        &camera->projection, &camera->view, &world);
    // Unordered (NaN) depth also rejects: native FCOM status checks are
    // positive ordered >=0 and <=1 tests, not an inverted ordered OR.
    if (!(screenCenter.z >= 0.0f && screenCenter.z <= 1.0f))
        return -1;

    CameraVector screenRight;
    D3DXVec3Project(&screenRight, &camera->right, &camera->viewport,
        &camera->projection, &camera->view, &world);
    CameraVector difference = {
        screenRight.x - screenCenter.x,
        screenRight.y - screenCenter.y,
        screenRight.z - screenCenter.z
    };
    float distanceSquared = difference.x * difference.x +
        difference.y * difference.y + difference.z * difference.z;
    float halfScale = static_cast<float>(AnimationDistanceRoot(distanceSquared) * 0.5);
    float width = self->scaleX * halfScale * self->width;
    float height = halfScale * self->height * self->scaleY;

    float cosine = static_cast<float>(cos(self->angle));
    float sine = static_cast<float>(sin(self->angle));
    float localX[4], localY[4];
    switch ((self->flags >> 18) & 3)
    {
    case 0:
        localX[0] = localX[2] = static_cast<float>(-width * 0.5);
        localX[1] = localX[3] = static_cast<float>( width * 0.5);
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
        localY[2] = localY[3] = static_cast<float>( height * 0.5);
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
    // Neither axis has a native case-3 initialization. Keep that lack of a
    // fallback, as in the rotated non-billboard corner builder.
    for (int i = 0; i < 4; ++i)
    {
        g_AnimationQuad[i].x = screenCenter.x +
            (cosine * localX[i] - sine * localY[i]);
        g_AnimationQuad[i].y = screenCenter.y +
            cosine * localY[i] + sine * localX[i];
        g_AnimationQuad[i].z = screenCenter.z;
    }
    return 0;
}
