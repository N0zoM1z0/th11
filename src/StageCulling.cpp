#include "StageLayerDrawing.hpp"

extern "C" D3DMATRIX *WINAPI D3DXMatrixTranslation(
    D3DMATRIX *output, float x, float y, float z);
extern "C" CameraVector *WINAPI D3DXVec3ProjectArray(
    CameraVector *output, unsigned int outputStride,
    const CameraVector *source, unsigned int sourceStride,
    const D3DVIEWPORT9 *viewport,
    const D3DMATRIX *projection, const D3DMATRIX *view,
    const D3DMATRIX *world, unsigned int count);

// Preserve the original branch order. For unordered/NaN X or Y values
// neither comparison updates the corresponding screen-space extrema.
static __forceinline void ExtendProjectedBounds(const CameraVector &point,
    float &smallestX, float &largestX, float &smallestY, float &largestY)
{
    if (point.z > 0.0f && point.z <= 1.0f)
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
int __stdcall CullStageObject(const PerspectiveCameraView *camera,
    const StageObjectDrawView *object, const CameraVector *offset, float maxDistanceSquared)
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
    float maxX = static_cast<float>(object->position.x + halfExtents.x);
    float minX = static_cast<float>(object->position.x - halfExtents.x);
    float maxY = static_cast<float>(object->position.y + halfExtents.y);
    float minY = static_cast<float>(object->position.y - halfExtents.y);
    float maxZ = static_cast<float>(object->position.z + halfExtents.z);
    float minZ = static_cast<float>(object->position.z - halfExtents.z);

    CameraVector corners[12] = {
        {maxX, maxY, maxZ}, {maxX, maxY, minZ},
        {maxX, minY, maxZ}, {maxX, minY, minZ},
        {minX, maxY, maxZ}, {minX, maxY, minZ},
        {minX, minY, maxZ}, {minX, minY, minZ},
        {object->position.x, minY, minZ},
        {object->position.x, maxY, minZ},
        {object->position.x, minY, maxZ},
        {object->position.x, maxY, maxZ}
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

    float smallestX = 424.0f, largestX = 24.0f;
    float smallestY = 472.0f, largestY = 8.0f;
    // The native VC9 producer handles twelve projected points as two groups
    // of six in a single outer loop; each point has its own clipping branch.
    for (int group = 0; group < 2; ++group)
    {
        ExtendProjectedBounds(projected[group * 6 + 0], smallestX, largestX, smallestY, largestY);
        ExtendProjectedBounds(projected[group * 6 + 1], smallestX, largestX, smallestY, largestY);
        ExtendProjectedBounds(projected[group * 6 + 2], smallestX, largestX, smallestY, largestY);
        ExtendProjectedBounds(projected[group * 6 + 3], smallestX, largestX, smallestY, largestY);
        ExtendProjectedBounds(projected[group * 6 + 4], smallestX, largestX, smallestY, largestY);
        ExtendProjectedBounds(projected[group * 6 + 5], smallestX, largestX, smallestY, largestY);
    }
    // The native rectangle intersection includes all four viewport edges.
    if (largestX >= 32.0f && smallestX <= 416.0f &&
        largestY >= 16.0f && smallestY <= 464.0f)
        return 0;
    return 1;
}
