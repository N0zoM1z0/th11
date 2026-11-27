#include "StageLayerDrawing.hpp"

extern "C" D3DMATRIX *WINAPI D3DXMatrixTranslation(
    D3DMATRIX *output, float x, float y, float z);
extern "C" CameraVector *WINAPI D3DXVec3ProjectArray(
    CameraVector *output, unsigned int outputStride,
    const CameraVector *source, unsigned int sourceStride,
    const D3DVIEWPORT9 *viewport,
    const D3DMATRIX *projection, const D3DMATRIX *view,
    const D3DMATRIX *world, unsigned int count);

// Preserve the native asymmetric x87 branches. If the first comparison is
// false (including unordered/NaN), the native code writes the point to min.
static __forceinline void ExtendProjectedBounds(const CameraVector &point,
    float &smallestX, float &largestX, float &smallestY, float &largestY)
{
    if (point.z >= 0.0f && point.z <= 1.0f)
    {
        if (point.x < smallestX)
            smallestX = point.x;
        else if (point.x > largestX)
            largestX = point.x;
        if (point.y < smallestY)
            smallestY = point.y;
        else if (point.y > largestY)
            largestY = point.y;
    }
}

// Complete native 0x403950 object-culling algorithm. The 12 input positions
// are eight cuboid corners plus four samples on the middle X plane.
int __fastcall CullStageObject(const StageObjectDrawView *object,
    const CameraVector *offset, float maxDistanceSquared,
    const PerspectiveCameraView *camera)
{
    // Retain the layer's three-float displacement value for the center
    // calculation; the original also rereads its Y/Z values from the caller.
    const CameraVector displacement = *offset;
    CameraVector worldCenter = {
        object->position.x + displacement.x,
        object->position.y + offset->y,
        object->position.z + offset->z
    };
    // The original stores all three camera-space eye coordinates as float
    // before subtracting them from the separately rounded object center.
    CameraVector eyePosition = {
        static_cast<float>(camera->position.x + camera->eyeOffset.x),
        static_cast<float>(camera->eyeOffset.y + camera->position.y),
        static_cast<float>(camera->eyeOffset.z + camera->position.z)
    };
    CameraVector delta = {
        static_cast<float>(worldCenter.x - eyePosition.x),
        static_cast<float>(worldCenter.y - eyePosition.y),
        static_cast<float>(worldCenter.z - eyePosition.z)
    };
    // The source comparison uses a rounded float distance, and only a
    // strictly greater distance is culled (unordered coordinates survive).
    const float distanceSquared = static_cast<float>(
        delta.z * delta.z + delta.x * delta.x + delta.y * delta.y);
    if (distanceSquared > maxDistanceSquared)
        return 1;

    // The three half-extents are individually stored as floats before
    // either corner endpoint is formed, matching the native x87 rounding.
    const float half = 0.5f;
    const CameraVector halfExtents = {
        static_cast<float>(object->extents.x * half),
        static_cast<float>(object->extents.y * half),
        static_cast<float>(object->extents.z * half)
    };
    const CameraVector &p = object->position;
    const CameraVector h = halfExtents;
    CameraVector corners[12] = {
        {static_cast<float>(p.x + h.x), static_cast<float>(p.y + h.y), static_cast<float>(p.z + h.z)},
        {static_cast<float>(p.x + h.x), static_cast<float>(p.y + h.y), static_cast<float>(p.z - h.z)},
        {static_cast<float>(p.x + h.x), static_cast<float>(p.y - h.y), static_cast<float>(p.z + h.z)},
        {static_cast<float>(p.x + h.x), static_cast<float>(p.y - h.y), static_cast<float>(p.z - h.z)},
        {static_cast<float>(p.x - h.x), static_cast<float>(p.y + h.y), static_cast<float>(p.z + h.z)},
        {static_cast<float>(p.x - h.x), static_cast<float>(p.y + h.y), static_cast<float>(p.z - h.z)},
        {static_cast<float>(p.x - h.x), static_cast<float>(p.y - h.y), static_cast<float>(p.z + h.z)},
        {static_cast<float>(p.x - h.x), static_cast<float>(p.y - h.y), static_cast<float>(p.z - h.z)},
        {p.x, static_cast<float>(p.y - h.y), static_cast<float>(p.z - h.z)},
        {p.x, static_cast<float>(p.y + h.y), static_cast<float>(p.z - h.z)},
        {p.x, static_cast<float>(p.y - h.y), static_cast<float>(p.z + h.z)},
        {p.x, static_cast<float>(p.y + h.y), static_cast<float>(p.z + h.z)}
    };
    // The original constructs an identity-valued matrix before applying
    // the translation through D3DX. Preserve the real sixteen float stores.
    D3DMATRIX translation;
    translation._12 = translation._13 = translation._14 = translation._21 =
    translation._23 = translation._24 = translation._31 = translation._32 =
    translation._34 = translation._41 = translation._42 = translation._43 = 0.0f;
    translation._11 = translation._22 = translation._33 = translation._44 = 1.0f;
    D3DXMatrixTranslation(&translation, offset->x, offset->y, offset->z);

    CameraVector projected[12];
    D3DXVec3ProjectArray(projected, sizeof(CameraVector), corners, sizeof(CameraVector),
        &camera->viewport, &camera->projection, &camera->view, &translation, 12);

    float smallestX = 424.0f, smallestY = 472.0f;
    float largestX = 24.0f, largestY = 8.0f;
    for (int i = 0; i < 12; ++i)
        ExtendProjectedBounds(projected[i], smallestX, largestX, smallestY, largestY);
    // The native rectangle intersection includes all four viewport edges.
    if (largestX >= 32.0f && smallestX <= 416.0f &&
        largestY >= 16.0f && smallestY <= 464.0f)
        return 0;
    return 1;
}
