#include "StageLayerDrawing.hpp"

extern "C" D3DMATRIX *WINAPI D3DXMatrixTranslation(
    D3DMATRIX *output, float x, float y, float z);
extern "C" CameraVector *WINAPI D3DXVec3ProjectArray(
    CameraVector *output, unsigned int outputStride,
    const CameraVector *source, unsigned int sourceStride,
    const D3DVIEWPORT9 *viewport,
    const D3DMATRIX *projection, const D3DMATRIX *view,
    const D3DMATRIX *world, unsigned int count);

// Complete native 0x403950 object-culling algorithm. The 12 input positions
// are eight cuboid corners plus four samples on the middle X plane.
int __stdcall CullStageObject(const PerspectiveCameraView *camera,
    const StageObjectDrawView *object, const CameraVector *offset, float maxDistanceSquared)
{
    CameraVector worldCenter = {
        object->position.x + offset->x,
        object->position.y + offset->y,
        object->position.z + offset->z
    };
    CameraVector delta = {
        worldCenter.x - (camera->position.x + camera->eyeOffset.x),
        worldCenter.y - (camera->position.y + camera->eyeOffset.y),
        worldCenter.z - (camera->position.z + camera->eyeOffset.z)
    };
    if (!(delta.z * delta.z + delta.x * delta.x + delta.y * delta.y <= maxDistanceSquared))
        return 1;

    const float half = 0.5f;
    float maxX = static_cast<float>(object->position.x + object->extents.x * half);
    float minX = static_cast<float>(object->position.x - object->extents.x * half);
    float maxY = static_cast<float>(object->position.y + object->extents.y * half);
    float minY = static_cast<float>(object->position.y - object->extents.y * half);
    float maxZ = static_cast<float>(object->position.z + object->extents.z * half);
    float minZ = static_cast<float>(object->position.z - object->extents.z * half);

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
    D3DMATRIX translation;
    D3DXMatrixTranslation(&translation, offset->x, offset->y, offset->z);

    CameraVector projected[12];
    D3DXVec3ProjectArray(projected, sizeof(CameraVector), corners, sizeof(CameraVector),
        &camera->viewport, &camera->projection, &camera->view, &translation, 12);

    float smallestX = 424.0f, largestX = 24.0f;
    float smallestY = 472.0f, largestY = 8.0f;
    for (int i = 0; i < 12; ++i)
    {
        const CameraVector &point = projected[i];
        if (point.z > 0.0f && point.z <= 1.0f)
        {
            if (point.x >= smallestX)
            {
                if (point.x > largestX)
                    largestX = point.x;
            }
            else
                smallestX = point.x;
            if (point.y > smallestY)
            {
                if (point.y > largestY)
                    largestY = point.y;
            }
            else
                smallestY = point.y;
        }
    }
    if (largestX > 32.0f && smallestX < 416.0 &&
        largestY > 16.0f && smallestY < 464.0)
        return 0;
    return 1;
}
